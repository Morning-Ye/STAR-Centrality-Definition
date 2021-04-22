const int nRounds = 6;
//0:nCorr for LumCor, 
//1:wLumCor for Vz Cor, 
//2:wLumCorAndVzCorr for shape Cor, 
//3:after apply the shapeWeight, look the final RefmultcCorr distribution, 
//5:after got the StRefmultCorr class, we need to use the StRefmultCor to generate the RemultCorr, and check whether our correction parameters were applied correctly. 

const int nRunPds = 6;
const int nVzBins = 14;
const int nRebinRefmult = 2;
const TString VzTitles[nVzBins] = 
{
	"-70<Vz<-60", "-60<Vz<-50", "-50<Vz<-40", "-40<Vz<-30", "-30<Vz<-20", "-20<Vz<-10", "-10<Vz<0",
	"0<Vz<10",    "10<Vz<20",   "20<Vz<30",   "30<Vz<40",   "40<Vz<50",   "50<Vz<60",   "60<Vz<70"
};
const TString RunPdTitles[nRunPds] =
{
	"trg01: 19130078#leq run #leq19131031", 
	"trg11: 19131037#leq run #leq19140025",
	"trg21: 19140030#leq run #leq19143017", 
	"trg31: 19144012#leq run #leq19144033",
	"trg41: 19144036#leq run #leq19145031", 
	"trg51: 19145034#leq run #leq19168040"
};

TH1F* H_TPCVz_perTRG[nRunPds];

TH2F* H_RefmultVsZdcX_AllVz[nRounds][nRunPds];
TH2F* H_RefmultVsZdcX[nRounds][nVzBins][nRunPds];
TProfile* prf_RefmultVsZdcX_AllVz[nRounds][nRunPds];
TProfile* prf_RefmultVsZdcX[nRounds][nVzBins][nRunPds];

TH1F* Href_aftLumCor[nRunPds][nVzBins];
TH1F* Hparmh_aftLumCor[nRunPds];

TH1F* Href_aftLumVzCor[nRunPds][nVzBins];
TH1F* Href_aftLumVzCor_AllVz[nRunPds];
TH1F* Href_aftLumVzCor_Center[nRunPds]; //|vz|<10cm
TH1F* Hparmh_aftLumVzCor[nRunPds];


TH1F* H_CorrParms_Lumi[nRunPds];//lumi cor parameters
TH1F* H_CorrParms_Vz[nRunPds];  //vz cor parameters
TH1F* Href_aftShapeCor_Center[nRunPds]; //|vz|<10cm
TH1F* HrefRatio_SubVz2Center[nRunPds][nVzBins];//shape cor parameters

//--------------------------------------------------
TH1F* Href_aftShapeCor_AllVz[nRunPds];
TH1F* HRatio_aftShapeCor_AllVz_OverTrg5[nRunPds];
TH1F* Href_aftShapeCor[nRunPds][nVzBins];
TH1F* HrefRatio_aftShapeCor_SubVz2Center[nRunPds][nVzBins];//final look
//--------------------------------------------------

TFile* inf;
void readFiles(const int roundIdx);
void draw(TString outDir, const int roundIdx);
void draw_noCorr(TString outDir);
void draw_wLumCorr(TString outDir);
void draw_wLumVzCorr(TString outDir, int isVzWindowCut);
void draw_Vz(TString outDir);

void saveFiles(TString outDir, const int roundIdx);
void checkQaAftCor(TString outDir, const int roundIdx);
//===============================================================================================
void new_mkRefQA(const int roundIdx = 3)
{
	const TString outDir = "outRefQA_new";
	system("mkdir -p outRefQA_new");
	
	readFiles(roundIdx);
	draw_Vz(outDir);

	if(roundIdx>=0) draw_noCorr(outDir);
	if(roundIdx>=1) draw_wLumCorr(outDir);
	if(roundIdx>=2)
	{
		draw_wLumVzCorr(outDir, 0);
		draw_wLumVzCorr(outDir, 1);
	}

	if(roundIdx>=3)
	{
		checkQaAftCor(outDir, roundIdx);
	}
	
	saveFiles(outDir, roundIdx);
}
//================================================================================================
void readFiles(const int roundIdx)
{
	//inf = new TFile(Form("./inputfiles/RefQA_run18AuAu_27GeV_MB_all_round%d.root",roundIdx), "READ");
	inf = new TFile(Form("./inputfiles/outRefQA_Run18_27GeV_all_round%d.root",roundIdx), "READ");
	cout<<"readin file: "<<inf->GetName()<<endl;
	
	for(int im=0; im<nRounds; im++)//0-no Corr, 1-CorLum, 2-CorVz, 3-CorLumVz
	{
		if(im > roundIdx ) continue;
		
		for(int irunpd=0; irunpd<nRunPds; irunpd++)
		{
			H_RefmultVsZdcX_AllVz[im][irunpd]   = (TH2F*)inf->Get( Form("hRefmultVsZdcX_im%d_AllVz_irun%d", im, irunpd) );
			prf_RefmultVsZdcX_AllVz[im][irunpd] = H_RefmultVsZdcX_AllVz[im][irunpd]->ProfileX( Form("prf_RefmultVsZdcX_im%d_AllVz_irun%d", im, irunpd));
			prf_RefmultVsZdcX_AllVz[im][irunpd] ->SetMarkerStyle(20);
			prf_RefmultVsZdcX_AllVz[im][irunpd] ->SetMarkerSize(0.6);
			prf_RefmultVsZdcX_AllVz[im][irunpd] ->SetMarkerColor(1);
			prf_RefmultVsZdcX_AllVz[im][irunpd] ->SetLineColor(1);
		
			//----------------------------------------------------------------------------------------------------------
			for(int ivz=0; ivz<nVzBins; ivz++)
			{
				H_RefmultVsZdcX[im][ivz][irunpd]   = (TH2F*)inf->Get( Form("hRefmultVsZdcX_im%d_ivz%d_irun%d", im, ivz, irunpd) );
				prf_RefmultVsZdcX[im][ivz][irunpd] = H_RefmultVsZdcX[im][ivz][irunpd]->ProfileX( Form("prf_RefmultVsZdcX_im%d_ivz%d_irun%d", im, ivz, irunpd) );
				prf_RefmultVsZdcX[im][ivz][irunpd] ->SetMarkerStyle(20);
				prf_RefmultVsZdcX[im][ivz][irunpd] ->SetMarkerSize(0.6);
				prf_RefmultVsZdcX[im][ivz][irunpd] ->SetMarkerColor(2+ivz);
				prf_RefmultVsZdcX[im][ivz][irunpd] ->SetLineColor(2+ivz);
			
				//----------------------------------------------------------------------------------------------------------
				if(im==1)
				{
					Href_aftLumCor[irunpd][ivz] = (TH1F*)H_RefmultVsZdcX[im][ivz][irunpd]->ProjectionY( Form("Href_aftLumCor_irunpd%d_ivz%d",irunpd,ivz) );
					Href_aftLumCor[irunpd][ivz] ->Rebin(nRebinRefmult); 
					Href_aftLumCor[irunpd][ivz] ->Scale(1./Href_aftLumCor[irunpd][ivz]->Integral());
				}
				else if(im==2)
				{
					Href_aftLumVzCor[irunpd][ivz] = (TH1F*)H_RefmultVsZdcX[im][ivz][irunpd]->ProjectionY( Form("Href_aftLumVzCor_irunpd%d_ivz%d",irunpd,ivz) );
					Href_aftLumVzCor[irunpd][ivz] ->Rebin(nRebinRefmult);
					Href_aftLumVzCor[irunpd][ivz] ->Scale(1./Href_aftLumVzCor[irunpd][ivz]->Integral());
				}
				else if(im==3)
				{
					Href_aftShapeCor[irunpd][ivz] = (TH1F*)H_RefmultVsZdcX[im][ivz][irunpd]->ProjectionY( Form("Href_aftShapeCor_irunpd%d_ivz%d",irunpd,ivz) );
					Href_aftShapeCor[irunpd][ivz] ->Rebin(nRebinRefmult);
					Href_aftShapeCor[irunpd][ivz] ->Scale(1./Href_aftShapeCor[irunpd][ivz]->Integral());
				}
				//----------------------------------------------------------------------------------------------------------
			}
			
			//----------------------------------------------------------------------------------------------------------
			if(im==2)
			{
				Href_aftLumVzCor_AllVz[irunpd] = (TH1F*)H_RefmultVsZdcX_AllVz[im][irunpd]->ProjectionY( Form("Href_aftLumVzCor_irunpd%d_AllVz",irunpd) );
				Href_aftLumVzCor_AllVz[irunpd] ->Rebin(nRebinRefmult);
				Href_aftLumVzCor_AllVz[irunpd] ->Scale(1./Href_aftLumVzCor_AllVz[irunpd]->Integral());
			}

			//----------------------------------------------------------------------------------------------------------
			if(im==3)
			{
				Href_aftShapeCor_AllVz[irunpd] = (TH1F*)H_RefmultVsZdcX_AllVz[im][irunpd]->ProjectionY( Form("Href_aftShapeCor_irunpd%d_AllVz",irunpd) );
				Href_aftShapeCor_AllVz[irunpd] ->Rebin(nRebinRefmult);
				Href_aftShapeCor_AllVz[irunpd] ->Scale(1./Href_aftShapeCor_AllVz[irunpd]->Integral());
			}
			//----------------------------------------------------------------------------------------------------------
		}//irunpd
	}

	//get the ratio of corrected Refmult in different Vz over AllVz
	if(roundIdx>=2)
	{
		for(int irunpd=0; irunpd<nRunPds; irunpd++)
		{
			Href_aftLumVzCor_Center[irunpd] = (TH1F*)Href_aftLumVzCor[irunpd][6]->Clone(Form("Href_aftLumVzCor_Center_irunpd%d",irunpd));//-10<vz<0
			Href_aftLumVzCor_Center[irunpd] ->Add( Href_aftLumVzCor[irunpd][7] );//add 0<vz<10 = |vz|<10cm
			Href_aftLumVzCor_Center[irunpd] ->Scale( 1./Href_aftLumVzCor_Center[irunpd]->Integral() );

			//--------------------------------------------------------------------------------
			//--------------------------------------------------------------------------------
			for(int ivz=0; ivz<nVzBins; ivz++)
			{
				HrefRatio_SubVz2Center[irunpd][ivz] = (TH1F*)Href_aftLumVzCor[irunpd][ivz]->Clone(Form("HrefRatio_SubVz2Center_irunpd%d_ivz%d",irunpd,ivz));
				HrefRatio_SubVz2Center[irunpd][ivz] -> Divide(Href_aftLumVzCor_Center[irunpd]);//divide |vz|<10cm
			}//ivz
			//--------------------------------------------------------------------------------
			//--------------------------------------------------------------------------------
			if(roundIdx==3)
			{
				Href_aftShapeCor_Center[irunpd] = (TH1F*)Href_aftShapeCor[irunpd][6]->Clone(Form("Href_aftShapeCor_Center_irunpd%d",irunpd));//-10<vz<0
				Href_aftShapeCor_Center[irunpd] ->Add( Href_aftShapeCor[irunpd][7] );//add 0<vz<10 = |vz|<10cm
				Href_aftShapeCor_Center[irunpd] ->Scale( 1./Href_aftShapeCor_Center[irunpd]->Integral() );

				for(int ivz=0; ivz<nVzBins; ivz++)
				{
					HrefRatio_aftShapeCor_SubVz2Center[irunpd][ivz] 
						= (TH1F*)Href_aftShapeCor[irunpd][ivz]->Clone(Form("HrefRatio_aftShapeCor_SubVz2Center_irunpd%d_ivz%d",irunpd,ivz));
					HrefRatio_aftShapeCor_SubVz2Center[irunpd][ivz] -> Divide(Href_aftShapeCor_Center[irunpd]);//divide |vz|<10cm
				}//ivz
			}//if roundIdx =3
			//--------------------------------------------------------------------------------
			//--------------------------------------------------------------------------------
		}//irunPd

		//--------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------
		if(roundIdx==3)
		{
			for(int irunpd=0; irunpd<nRunPds-1; irunpd++)
			{
				HRatio_aftShapeCor_AllVz_OverTrg5[irunpd] = (TH1F*)Href_aftShapeCor_AllVz[irunpd]->Clone(Form("HRatio_aftShapeCor_AllVz_OverTrg5_irunpd%d",irunpd));
				HRatio_aftShapeCor_AllVz_OverTrg5[irunpd]->Divide(Href_aftShapeCor_AllVz[5]);
			}
		}
		//--------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------
	}

	//get the vertex distribution for each trigger id
	for(int irunpd=0; irunpd<nRunPds; irunpd++)
	{
		H_TPCVz_perTRG[irunpd] = (TH1F*)inf->Get(Form("hTPCVz_perRunPd_%d",irunpd));
	}
}

//================================================================================================
void draw_Vz(TString outDir)
{
	TCanvas *c1 = new TCanvas("c1");
	c1->SetLogz(1);
	gStyle->SetOptStat(0);

	//TLatex tl;
	//tl.SetNDC();
	//tl.SetTextSize(0.055);
	//tl.SetTextColor(2);

	for(int irunpd=0; irunpd<nRunPds; irunpd++)
	{
		H_TPCVz_perTRG[irunpd]->Scale(1./H_TPCVz_perTRG[irunpd]->Integral());
		//H_TPCVz_perTRG[irunpd]->SetTitle(RunPdTitles[irunpd]);
		H_TPCVz_perTRG[irunpd]->Draw("pe");

		c1->SaveAs(outDir+"/"+Form("H_TPCVz_perTRG_irun%d",irunpd)+".png");
	}
}
//================================================================================================
void draw_noCorr(TString outDir)
{	
	for(int irunpd=0; irunpd<nRunPds; irunpd++)
	{
		H_CorrParms_Lumi[irunpd] = new TH1F( Form("H_CorrParms_Lumi_irunpd%d",irunpd), Form("H_CorrParms_Lumi_runpd%d;VzIndex;Parms(a=y,b=yErr)",irunpd), 8, 0, 8 );
	}

	TCanvas *c1 = new TCanvas("c1");
	c1->SetLogz(1);
	gStyle->SetOptStat(0);
	
	TLatex tl;
	tl.SetNDC();
	tl.SetTextSize(0.055);
	tl.SetTextColor(2);
	
	TF1* fitf = new TF1("fitf", "[0]*x+[1]", 0., 5.);
	fitf->SetParameters(-0.1, 100);

	for(int iRound=0; iRound<nRounds; iRound++)
	{
		if(iRound>0) break;

		for(int irunpd=0; irunpd<nRunPds; irunpd++)
		{
			float xMin = 0.10; float xMax = 2.3;
			float yMin = 50;   float yMax = 120.;
			
			if(irunpd==0){      xMax = 1.8;}
			else if(irunpd==1||irunpd==2){ xMax = 2.5;}
			else if(irunpd==3){ xMax = 1.8;}
			else if(irunpd==4){ xMax = 2.0; yMin=80; yMax=130.;}
			else if(irunpd==5){ xMax = 3.0; yMin=80; yMax=130;}

			prf_RefmultVsZdcX_AllVz[iRound][irunpd]->SetAxisRange(xMin,  xMax, "x");
			prf_RefmultVsZdcX_AllVz[iRound][irunpd]->SetAxisRange(80,    130,  "y");
			//prf_RefmultVsZdcX_AllVz[iRound][irunpd]->SetAxisRange(120,    160,  "y");
			prf_RefmultVsZdcX_AllVz[iRound][irunpd]->SetTitle(RunPdTitles[irunpd]);
			prf_RefmultVsZdcX_AllVz[iRound][irunpd]->SetYTitle("<Refmult>");
			prf_RefmultVsZdcX_AllVz[iRound][irunpd]->Draw("pe");
			
			prf_RefmultVsZdcX_AllVz[iRound][irunpd]->Fit(fitf,"R", "", xMin+0.0, xMax-0.4);
			tl.DrawLatex( 0.25, 0.80,  "y = a*x + b");
			tl.DrawLatex( 0.25, 0.72,  Form("a = %.3f #pm %.3f", fitf->GetParameter(0), fitf->GetParError(0)));
			tl.DrawLatex( 0.25, 0.66,  Form("b = %.3f #pm %.3f", fitf->GetParameter(1), fitf->GetParError(1)));

			H_CorrParms_Lumi[irunpd]->SetBinContent(1, fitf->GetParameter(0));//save lumCor factor for all Vz, will use in real corr
			H_CorrParms_Lumi[irunpd]->SetBinError(  1, fitf->GetParameter(1) );

			tl.DrawLatex( 0.70, 0.2,  "No Corr");
			
			TLegend *leg1 = new TLegend(0.72,0.70,0.89,0.89);
			leg1->SetBorderSize(0);
			leg1->AddEntry(prf_RefmultVsZdcX_AllVz[iRound][irunpd],  "|Vz|<70", "lp");
			//leg1->Draw("same");

			c1->SaveAs(outDir+"/"+Form("prf_RefmultVsZdcX_iRound%d_irun%d_Vz70",iRound,irunpd)+".png");

			for(int ivz=0; ivz<nVzBins; ivz++)
			{
				if(ivz==0)
				{
					prf_RefmultVsZdcX[iRound][ivz][irunpd]->SetAxisRange(xMin, xMax, "x");
					prf_RefmultVsZdcX[iRound][ivz][irunpd]->SetAxisRange(yMin, yMax, "y");
					prf_RefmultVsZdcX[iRound][ivz][irunpd]->SetTitle(RunPdTitles[irunpd]);
					prf_RefmultVsZdcX[iRound][ivz][irunpd]->SetYTitle("<Refmult>");
					prf_RefmultVsZdcX[iRound][ivz][irunpd]->Draw("pe");
					tl.DrawLatex( 0.25, 0.80,  "y = a*x + b");
				}
				
				prf_RefmultVsZdcX[iRound][ivz][irunpd]->Draw("pesame");
				
				prf_RefmultVsZdcX[iRound][ivz][irunpd]->Fit(fitf,"R", "", xMin+0.1,xMax-0.4);

				H_CorrParms_Lumi[irunpd]->SetBinContent(2+ivz, fitf->GetParameter(0));//save lumCorr factor for sub Vz, not use
				H_CorrParms_Lumi[irunpd]->SetBinError(  2+ivz, fitf->GetParameter(1) );
				tl.DrawLatex( 0.70, 0.2,  "No Corr");
			}

			TLegend *leg2 = new TLegend(0.75,0.30,0.95,0.95);
			leg2->SetBorderSize(0);
			for(int ivz=0; ivz<nVzBins; ivz++) leg2->AddEntry(prf_RefmultVsZdcX[iRound][ivz][irunpd], VzTitles[ivz], "lp");
			leg2->Draw("same");

			c1->SaveAs(outDir+"/"+Form("prf_RefmultVsZdcX_iRound%d_irun%d_allVz",iRound,irunpd)+".png");
		}
	}
	
	delete c1;
	delete fitf;
}

void draw_wLumCorr(TString outDir)
{
	TCanvas *c1 = new TCanvas("c1");
	c1->SetLogy(0);
	gStyle->SetOptFit(1);
	TLatex tl;
	tl.SetNDC();
	tl.SetTextSize(0.055);
	tl.SetTextColor(2);
	
	//-------------------------------------------------------------------------------
	//draw <Refmult> vs ZDCx after LumCorr
	//-------------------------------------------------------------------------------
	TF1* fitf = new TF1("fitf", "[0]*x+[1]", 0., 5.);
	fitf->SetParameters(-0.1, 100);

	for(int iRound=0; iRound<nRounds; iRound++)
	{
		if(iRound != 1) continue;
		for(int irunpd=0; irunpd<nRunPds; irunpd++)
		{
			float xMin = 0.10; float xMax = 2.3;
			float yMin = 50;   float yMax = 120.;
			
			if(irunpd==0){      xMax = 1.8;}
			else if(irunpd==1||irunpd==2){ xMax = 2.5;}
			else if(irunpd==3){ xMax = 1.8;}
			else if(irunpd==4){ xMax = 2.0; yMin=80; yMax=130.;}
			else if(irunpd==5){ xMax = 3.0; yMin=80; yMax=130;}

			prf_RefmultVsZdcX_AllVz[iRound][irunpd]->SetAxisRange(xMin,  xMax, "x");
			prf_RefmultVsZdcX_AllVz[iRound][irunpd]->SetAxisRange(80,    130,  "y");
			prf_RefmultVsZdcX_AllVz[iRound][irunpd]->SetTitle(RunPdTitles[irunpd]);
			prf_RefmultVsZdcX_AllVz[iRound][irunpd]->SetYTitle("<Refmult>");
			prf_RefmultVsZdcX_AllVz[iRound][irunpd]->Draw("pe");
			
			prf_RefmultVsZdcX_AllVz[iRound][irunpd]->Fit(fitf,"R", "", xMin+0.0,xMax-0.4);
			tl.DrawLatex( 0.25, 0.80,  "y = a*x + b");
			tl.DrawLatex( 0.25, 0.72,  Form("a = %.3f #pm %.3f",fitf->GetParameter(0),fitf->GetParError(0)));
			tl.DrawLatex( 0.25, 0.66,  Form("b = %.3f #pm %.3f",fitf->GetParameter(1),fitf->GetParError(1)));

			tl.DrawLatex( 0.60, 0.2,  "After Lumi. Corr");
			
			TLegend *leg1 = new TLegend(0.72,0.70,0.89,0.89);
			leg1->SetBorderSize(0);
			leg1->AddEntry(prf_RefmultVsZdcX_AllVz[iRound][irunpd],  "|Vz|<70", "lp");
			//leg1->Draw("same");

			c1->SaveAs(outDir+"/"+Form("prf_RefmultVsZdcX_iRound%d_irun%d_Vz70",iRound,irunpd)+".png");

			for(int ivz=0; ivz<nVzBins; ivz++)
			{
				if(ivz==0)
				{
					prf_RefmultVsZdcX[iRound][ivz][irunpd]->SetAxisRange(xMin, xMax, "x");
					prf_RefmultVsZdcX[iRound][ivz][irunpd]->SetAxisRange(yMin, yMax, "y");
					prf_RefmultVsZdcX[iRound][ivz][irunpd]->SetTitle(RunPdTitles[irunpd]);
					prf_RefmultVsZdcX[iRound][ivz][irunpd]->SetYTitle("<Refmult>");
					prf_RefmultVsZdcX[iRound][ivz][irunpd]->Draw("pe");
					tl.DrawLatex( 0.25, 0.80,  "y = a*x + b");
				}
				
				prf_RefmultVsZdcX[iRound][ivz][irunpd]->Draw("pesame");
				
				prf_RefmultVsZdcX[iRound][ivz][irunpd]->Fit(fitf,"R", "", xMin+0.0,xMax-0.4);

				tl.DrawLatex( 0.60, 0.2,  "After Lumi. Corr");
			}

			TLegend *leg2 = new TLegend(0.75,0.30,0.95,0.95);
			leg2->SetBorderSize(0);
			for(int ivz=0; ivz<nVzBins; ivz++) leg2->AddEntry(prf_RefmultVsZdcX[iRound][ivz][irunpd], VzTitles[ivz], "lp");
			leg2->Draw("same");

			c1->SaveAs(outDir+"/"+Form("prf_RefmultVsZdcX_iRound%d_irun%d_allVz",iRound,irunpd)+".png");
		}
	}
	
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	//draw the refmult in different vz bin and different run period bin
	//error function:[0]*TMath::Erf(-[2]*(x-[1]))+[0]
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------

	float xRefMin = 300.; float xRefMax = 450.;
	TF1* ferr = new TF1("ferr", "[0]*TMath::Erf(-[2]*(x-[1]))+[0]", xRefMin, xRefMax);
	ferr->SetLineWidth(2);
	ferr->SetParName(0, "A");
	ferr->SetParName(1, "h");
	ferr->SetParName(2, "#sigma");

	TF1* fpol6 = new TF1("fpol6", "pol6", -70,70);
	fpol6->SetParameters(350., 0.001, 0., 0., 0., 0., 0.);
	
	for(int irunpd=0; irunpd<nRunPds; irunpd++)
	{
		Hparmh_aftLumCor[irunpd] = new TH1F(Form("Hparmh_aftLumCor_irunpd%d",irunpd), Form("Hparmh_aftLumCor_irunpd%d",irunpd), 14, -70, 70);
		H_CorrParms_Vz[irunpd]   = new TH1F(Form("H_CorrParms_Vz_irunpd%d",  irunpd), Form("H_CorrParms_Vz_irunpd%d",  irunpd), 7, 0, 7);

		c1->SetLogy(1);
		ferr->SetParameters(1.e-3, 357, 0.028);
		for(int ivz=0; ivz<nVzBins; ivz++)
		{
			Href_aftLumCor[irunpd][ivz]->SetTitle(RunPdTitles[irunpd]+": "+VzTitles[ivz]);
			//Href_aftLumCor[irunpd][ivz]->SetAxisRange(xRefMin, xRefMax, "x");
			//Href_aftLumCor[irunpd][ivz]->SetAxisRange(xRefMin, xRefMax, "x");
			Href_aftLumCor[irunpd][ivz]->SetMinimum(1.e-6);
			Href_aftLumCor[irunpd][ivz]->Draw("pe");
			//if()
			Href_aftLumCor[irunpd][ivz]->Fit(ferr,"ER+", "", xRefMin+0.0, xRefMax);
			ferr->SetLineWidth(3);
			ferr->SetLineColor(4);
			ferr->Draw("lsame");
			tl.DrawLatex( 0.60, 0.2,  "After Lumi. Corr");
			c1->SaveAs(outDir+"/"+Form("Href_aftLumCor_irunpd%d_ivz%d",irunpd,ivz)+".png");

			//save the parameter "h"
			Hparmh_aftLumCor[irunpd]->SetBinContent(ivz+1, ferr->GetParameter(1));
			Hparmh_aftLumCor[irunpd]->SetBinError(  ivz+1, ferr->GetParError(1) );
		}

		c1->SetLogy(0);
		Hparmh_aftLumCor[irunpd]->SetTitle(RunPdTitles[irunpd]);
		Hparmh_aftLumCor[irunpd]->SetAxisRange(300, 450, "y");;
		Hparmh_aftLumCor[irunpd]->SetYTitle("HighEndPoint(hValue)");
		Hparmh_aftLumCor[irunpd]->SetXTitle("TPC Vz(cm)");
		Hparmh_aftLumCor[irunpd]->SetMarkerStyle(20);
		Hparmh_aftLumCor[irunpd]->SetMarkerSize(2);
		Hparmh_aftLumCor[irunpd]->Draw("pe");

		Hparmh_aftLumCor[irunpd]->Fit(fpol6,"ER+", "", -70., 70);

		tl.DrawLatex( 0.60, 0.2,  "After Lumi. Corr");

		c1->SaveAs(outDir+"/"+Form("Hparmh_aftLumCor_irunpd%d",irunpd)+".png");

		//save the fitting parameters
		for(int ip=0; ip<7; ip++)
		{
			H_CorrParms_Vz[irunpd]->SetBinContent( ip+1, fpol6->GetParameter(ip) );
			H_CorrParms_Vz[irunpd]->SetBinError(   ip+1, fpol6->GetParError(ip) );
		}
	}
	
	delete ferr;
	delete fpol6;
}

//================================================================================================
void draw_wLumVzCorr(TString outDir, int isVzWindowCut = 0)
{
	TCanvas *c1 = new TCanvas("c1");
	c1->SetLogz(1);
	gStyle->SetOptStat(0);
	
	TLatex tl;
	tl.SetNDC();
	tl.SetTextSize(0.055);
	tl.SetTextColor(2);
	
	gStyle->SetOptFit(1);
	//draw the refmult in different vz bin and different run period bin
	//error function:[0]*TMath::Erf(-[2]*(x-[1]))+[0]
	
	float xRefMin = 300.; float xRefMax = 450.;
	TF1* ferr = new TF1("ferr", "[0]*TMath::Erf(-[2]*(x-[1]))+[0]", xRefMin, xRefMax);
	ferr->SetParName(0, "A");
	ferr->SetParName(1, "h");
	ferr->SetParName(2, "#sigma");
	ferr->SetParameters(1.e-3,360,0.028);
	
	TF1* fpol0 = new TF1("fpol0", "pol0", -70,70);
	fpol0->SetParameter(0,350.);
	
	c1->SetLogy(0);
	for(int irunpd=0; irunpd<nRunPds; irunpd++)
	{
		Hparmh_aftLumVzCor[irunpd] = new TH1F(Form("Hparmh_aftLumVzCor_irunpd%d",irunpd), Form("Hparmh_aftLumVzCor_irunpd%d",irunpd), 14, -70, 70);

		c1->SetLogy(1);
		for(int ivz=0; ivz<nVzBins; ivz++)
		{
			Href_aftLumVzCor[irunpd][ivz]->SetTitle(RunPdTitles[irunpd]+": "+VzTitles[ivz]);
			//Href_aftLumVzCor[irunpd][ivz]->SetAxisRange(xRefMin, xRefMax, "x");
			Href_aftLumVzCor[irunpd][ivz]->SetAxisRange(0., 500., "x");
			Href_aftLumVzCor[irunpd][ivz]->SetAxisRange(1.e-6, 1.e-1, "y");
			//Href_aftLumVzCor[irunpd][ivz]->SetMinimum(0.1);
			Href_aftLumVzCor[irunpd][ivz]->SetLineColor(1+ivz);
			Href_aftLumVzCor[irunpd][ivz]->SetMarkerColor(1);
			Href_aftLumVzCor[irunpd][ivz]->Draw("pe");
			Href_aftLumVzCor[irunpd][ivz]->Fit(ferr,"ER", "", xRefMin+0.0, xRefMax);
			ferr->SetLineColor(4);
			ferr->Draw("lsame");
			tl.DrawLatex( 0.50, 0.2,  "After Lumi and Vz Corr");
			c1->SaveAs(outDir+"/"+Form("Href_aftLumVzCor_irunpd%d_ivz%d",irunpd,ivz)+".png");

			//save the parameter "h"
			Hparmh_aftLumVzCor[irunpd]->SetBinContent(ivz+1, ferr->GetParameter(1));
			Hparmh_aftLumVzCor[irunpd]->SetBinError(  ivz+1, ferr->GetParError(1) );
		}

		c1->SetLogy(0);
		Hparmh_aftLumVzCor[irunpd]->SetTitle(RunPdTitles[irunpd]);
		Hparmh_aftLumVzCor[irunpd]->SetAxisRange(300, 450, "y");;
		Hparmh_aftLumVzCor[irunpd]->SetYTitle("HighEndPoint(hValue)");
		Hparmh_aftLumVzCor[irunpd]->SetXTitle("TPC Vz(cm)");
		Hparmh_aftLumVzCor[irunpd]->SetMarkerStyle(20);
		Hparmh_aftLumVzCor[irunpd]->SetMarkerSize(2);
		Hparmh_aftLumVzCor[irunpd]->Draw("pe");

		Hparmh_aftLumVzCor[irunpd]->Fit(fpol0,"ER+", "", -70., 70);

		tl.DrawLatex( 0.50, 0.2,  "After Lumi and Vz Corr");

		c1->SaveAs(outDir+"/"+Form("Hparmh_aftLumVzCor_irunpd%d",irunpd)+".png");
	}	

	//overlaythe Refmult for different vz in same run period
	//compare their shapes
		
	gStyle->SetOptFit(0);
	for(int irunpd=0; irunpd<nRunPds; irunpd++)
	{
		c1->SetLogy(1);

		Href_aftLumVzCor[irunpd][6]->SetTitle(RunPdTitles[irunpd]);
		Href_aftLumVzCor[irunpd][6]->SetLineColor(1+6);
		Href_aftLumVzCor[irunpd][6]->SetAxisRange(0, 550, "x");
		Href_aftLumVzCor[irunpd][6]->SetAxisRange(0.000001, 0.1, "y");
		Href_aftLumVzCor[irunpd][6]->GetFunction("ferr")->SetBit(TF1::kNotDraw);
		Href_aftLumVzCor[irunpd][6]->SetStats(0);
		Href_aftLumVzCor[irunpd][6]->Draw("pe");
		
		for(int ivz=0; ivz<nVzBins; ivz++)
		{
			if(ivz==6) continue;

			Href_aftLumVzCor[irunpd][ivz]->GetFunction("ferr")->SetBit(TF1::kNotDraw);
			Href_aftLumVzCor[irunpd][ivz]->SetStats(0);
				
			//---------------------------------------------------------------------------
			//because of online vpd cut
			if(isVzWindowCut==1)
			{
				if(irunpd==0||irunpd==1||irunpd==3)
				{
					if(ivz<=1||ivz>=12) continue;
				}
				else if(irunpd==2)
				{
					if(ivz<=3||ivz>=10) continue;	
				}
			}
			//---------------------------------------------------------------------------
			Href_aftLumVzCor[irunpd][ivz]->Draw("pesame");
		}
		
		TLegend *leg_cmpShape = new TLegend(0.80,0.30,0.99,0.95);
		leg_cmpShape->SetBorderSize(0);
		for(int ivz=0; ivz<nVzBins; ivz++) 
		{
			//---------------------------------------------------------------------------
			//because of online vpd cut
			if(isVzWindowCut==1)
			{
				if(irunpd==0||irunpd==1||irunpd==3)
				{
					if(ivz<=1||ivz>=12) continue;
				}
				else if(irunpd==2)
				{
					if(ivz<=3||ivz>=10) continue;	
				}
			}
			//---------------------------------------------------------------------------
			leg_cmpShape->AddEntry(Href_aftLumVzCor[irunpd][ivz], VzTitles[ivz], "lp");
		}
	
		leg_cmpShape->Draw("same");

		tl.DrawLatex( 0.20, 0.2,  "After Lumi and Vz Corr");

		//-----------------------------------------------------------------------------------------------------------
		if(isVzWindowCut==0)
		{
			c1->SaveAs(outDir+"/"+Form("Href_aftLumVzCor_irunpd%d_compShape",irunpd)+".png");
		}
		else if(isVzWindowCut==1)
		{
			c1->SaveAs(outDir+"/"+Form("Href_aftLumVzCor_irunpd%d_compShape_VzWindow",irunpd)+".png");
		}
		//-----------------------------------------------------------------------------------------------------------
		
		c1->Clear();	
		//-----------------------------------------------------------------------------------------------------------
		//compare ratios of sub Vz to AllVz
		c1->SetLogy(0);
		HrefRatio_SubVz2Center[irunpd][6]->SetTitle(RunPdTitles[irunpd]);
		HrefRatio_SubVz2Center[irunpd][6]->SetYTitle("Ratio To |Vz|<10cm");
		HrefRatio_SubVz2Center[irunpd][6]->SetLineColor(1+6);
		HrefRatio_SubVz2Center[irunpd][6]->SetAxisRange(0,  550, "x");
		HrefRatio_SubVz2Center[irunpd][6]->SetAxisRange(0., 2.0, "y");
		HrefRatio_SubVz2Center[irunpd][6]->Draw("hist");

		for(int ivz=0; ivz<nVzBins; ivz++)
		{
			if(ivz==6) continue;
			//---------------------------------------------------------------------------
			//because of online vpd cut
			if(isVzWindowCut==1)
			{
				if(irunpd==0||irunpd==1||irunpd==3)
				{
					if(ivz<=1||ivz>=12) continue;
				}
				else if(irunpd==2)
				{
					if(ivz<=3||ivz>=10) continue;	
				}
			}
			
			HrefRatio_SubVz2Center[irunpd][ivz]->SetMarkerStyle(20);
			HrefRatio_SubVz2Center[irunpd][ivz]->SetMarkerColor(1+ivz);
			HrefRatio_SubVz2Center[irunpd][ivz]->SetLineColor(1+ivz);
			HrefRatio_SubVz2Center[irunpd][ivz]->SetMarkerSize(0.2);
			HrefRatio_SubVz2Center[irunpd][ivz]->SetLineWidth(2);
			HrefRatio_SubVz2Center[irunpd][ivz]->Draw("histsame");
		}
		
		TLegend *leg_SubVz2Center = new TLegend(0.80,0.30,0.99,0.95);
		leg_SubVz2Center->SetBorderSize(0);
		for(int ivz=0; ivz<nVzBins; ivz++) 
		{
			//---------------------------------------------------------------------------
			//because of online vpd cut
			if(isVzWindowCut==1)
			{
				if(irunpd==0||irunpd==1||irunpd==3)
				{
					if(ivz<=1||ivz>=12) continue;
				}
				else if(irunpd==2)
				{
					if(ivz<=3||ivz>=10) continue;	
				}
			}
			//---------------------------------------------------------------------------
			leg_SubVz2Center->AddEntry(HrefRatio_SubVz2Center[irunpd][ivz], VzTitles[ivz], "lp");
		}
		leg_SubVz2Center->Draw("same");

		tl.DrawLatex( 0.20, 0.2,  "After Lumi and Vz Corr");
		
		if(isVzWindowCut==0)
		{
			c1->SaveAs(outDir+"/"+Form("HrefRatio_SubVz2Center_irunpd%d_SubVz2Center",irunpd)+".png");
			c1->SaveAs(outDir+"/"+Form("HrefRatio_SubVz2Center_irunpd%d_SubVz2Center",irunpd)+".pdf");
		}
		else if(isVzWindowCut==1)
		{
			c1->SaveAs(outDir+"/"+Form("HrefRatio_SubVz2Center_irunpd%d_SubVz2Center_VzWindow",irunpd)+".png");
			c1->SaveAs(outDir+"/"+Form("HrefRatio_SubVz2Center_irunpd%d_SubVz2Center_VzWindow",irunpd)+".pdf");
		}
		//-----------------------------------------------------------------------------------------------------------
	}
}

//================================================================================================
void checkQaAftCor(TString outDir, const int roundIdx)
{
	TCanvas *c1 = new TCanvas("c1");
	c1->SetLogz(1);
	gStyle->SetOptStat(0);
	
	TLatex tl;
	tl.SetNDC();
	tl.SetTextSize(0.055);
	tl.SetTextColor(2);
	
	gStyle->SetOptFit(1);
	//TH1F* Href_aftShapeCor[nRunPds][nVzBins];
	//TH1F* Href_aftShapeCor_AllVz[nRunPds];
	//TH1F* HrefRatio_aftShapeCor_SubVz2Center[nRunPds][nVzBins];//final look
	
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	//1.1 overlay refmult for different vz bin in each runPd or trig
	for(int irunpd=0; irunpd<nRunPds; irunpd++)
	{
		c1->SetLogy(1);
		for(int ivz=0; ivz<nVzBins; ivz++) 
		{
			Href_aftShapeCor[irunpd][ivz]->SetTitle("Check Refmult in Sub. Vz for "+RunPdTitles[irunpd]);
			Href_aftShapeCor[irunpd][ivz]->SetLineColor(ivz+1);
			Href_aftShapeCor[irunpd][ivz]->SetLineWidth(2);
			Href_aftShapeCor[irunpd][ivz]->SetAxisRange(1.e-6, 1.0, "y");

			if(ivz==0) Href_aftShapeCor[irunpd][ivz]->Draw("pe");
			else Href_aftShapeCor[irunpd][ivz]->Draw("pesame");
		}

		TLegend *leg_cmpShape = new TLegend(0.80,0.30,0.99,0.90);
		leg_cmpShape->SetBorderSize(0);
		for(int ivz=0; ivz<nVzBins; ivz++) 
		{
			leg_cmpShape->AddEntry(Href_aftShapeCor[irunpd][ivz], VzTitles[ivz], "lp");
		}
	
		leg_cmpShape->Draw("same");

		c1->SaveAs( Form(outDir+"/Href_aftShapeCor_diffVz_irunpd%d.png", irunpd) );
	

		c1->SetLogy(0);
		//-----------------------------------------------------------------------------------------------------------
		for(int ivz=0; ivz<nVzBins; ivz++) 
		{
			HrefRatio_aftShapeCor_SubVz2Center[irunpd][ivz]->SetTitle("Check Ratio (Vz(i)/|Vz|<10) for "+RunPdTitles[irunpd]);
			HrefRatio_aftShapeCor_SubVz2Center[irunpd][ivz]->SetLineColor(ivz+1);
			HrefRatio_aftShapeCor_SubVz2Center[irunpd][ivz]->SetLineWidth(2);
			HrefRatio_aftShapeCor_SubVz2Center[irunpd][ivz]->SetAxisRange(0., 2.0, "y");

			if(ivz==0) HrefRatio_aftShapeCor_SubVz2Center[irunpd][ivz]->Draw("pe");
			else HrefRatio_aftShapeCor_SubVz2Center[irunpd][ivz]->Draw("pesame");
		}

		TLegend *leg_cmpRatio = new TLegend(0.80,0.30,0.99,0.91);
		leg_cmpRatio->SetBorderSize(0);
		for(int ivz=0; ivz<nVzBins; ivz++) 
		{
			leg_cmpRatio->AddEntry(HrefRatio_aftShapeCor_SubVz2Center[irunpd][ivz], VzTitles[ivz], "lp");
		}
	
		leg_cmpRatio->Draw("same");

		c1->SaveAs( Form(outDir+"/HrefRatio_aftShapeCor_SubVz2Center_irunpd%d.png", irunpd) );
	}
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	//1.2 compare the ratio of refmult in subVz to center in each runPd

		
	c1->SetLogy(1);
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	//2.1 overlay refmult of center |vz|<10cm compare different runPd
	for(int irunpd=0; irunpd<nRunPds; irunpd++)
	{
		Href_aftShapeCor_AllVz[irunpd]->SetTitle("Check Refmult Between Run Period");
		Href_aftShapeCor_AllVz[irunpd]->SetLineColor(irunpd+1);
		Href_aftShapeCor_AllVz[irunpd]->SetLineWidth(2);
		//if(irunpd==3) 
		if(irunpd==0) 
		{
			Href_aftShapeCor_AllVz[irunpd]->SetAxisRange(1.e-6, 0.1, "y");
			Href_aftShapeCor_AllVz[irunpd]->Draw("pe");
		}
		else
		{
			Href_aftShapeCor_AllVz[irunpd]->Draw("pesame");
		}
	}

	TLegend *leg_Refmult_diffRunPd = new TLegend(0.12,0.16,0.70,0.50);
	leg_Refmult_diffRunPd->SetBorderSize(0);
	for(int irunpd=0; irunpd<nRunPds; irunpd++)
	{
		leg_Refmult_diffRunPd->AddEntry(Href_aftShapeCor_AllVz[irunpd], RunPdTitles[irunpd], "lp");
	}
	leg_Refmult_diffRunPd->Draw("same");

	c1->SaveAs(outDir+"/Href_aftShapeCor_AllVz_comRunPd.png");
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------

	c1->SetLogy(0);
	for(int irunpd=0; irunpd<nRunPds-1; irunpd++)
	{
		HRatio_aftShapeCor_AllVz_OverTrg5[irunpd]->SetTitle("Check Ratio: (Trig(0i)/Trig05)");
		HRatio_aftShapeCor_AllVz_OverTrg5[irunpd]->SetLineColor(irunpd+1);
		HRatio_aftShapeCor_AllVz_OverTrg5[irunpd]->SetLineWidth(2);
		//if(irunpd==3) 
		if(irunpd==0) 
		{
			HRatio_aftShapeCor_AllVz_OverTrg5[irunpd]->SetYTitle("Ratio (Trig(0i)/Trig05)");
			HRatio_aftShapeCor_AllVz_OverTrg5[irunpd]->SetAxisRange(0.0,2.0, "y");
			HRatio_aftShapeCor_AllVz_OverTrg5[irunpd]->Draw("pe");
		}
		else
		{
			HRatio_aftShapeCor_AllVz_OverTrg5[irunpd]->Draw("pesame");
		}
	}
	
	TLegend *leg_RefmultRatio_diffRunPd = new TLegend(0.12,0.60,0.70,0.88);
	leg_RefmultRatio_diffRunPd->SetBorderSize(0);
	for(int irunpd=0; irunpd<nRunPds-1; irunpd++)
	{
		leg_RefmultRatio_diffRunPd->AddEntry(HRatio_aftShapeCor_AllVz_OverTrg5[irunpd], RunPdTitles[irunpd], "lp");
	}
	leg_RefmultRatio_diffRunPd->Draw("same");
	
	c1->SaveAs(outDir+"/HRatio_aftShapeCor_AllVz_OverTrg5_comRunPd.png");
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------

	//2.2 compare the ratio of refmult in subVz to center in each runPd
}
//================================================================================================
void saveFiles(TString outDir, const int roundIdx)
{
	TFile *outf = new TFile(Form(outDir+"/out_CorrParms_Refmult_Run18_27GeV_forRound%d"+".root",roundIdx+1), "recreate");
	cout<<"outputfile: "<<outf->GetName()<<endl;
	outf->cd();
	for(int irunpd=0; irunpd<nRunPds; irunpd++)
	{
		if(roundIdx >= 0) H_CorrParms_Lumi[irunpd]->Write();
		if(roundIdx >= 1) H_CorrParms_Vz[irunpd]->Write();
		
		if(roundIdx >= 2) 
		{
			for(int ivz=0; ivz<nVzBins; ivz++)
			{
				HrefRatio_SubVz2Center[irunpd][ivz]->SetTitle(RunPdTitles[irunpd]+": "+VzTitles[ivz]);
				HrefRatio_SubVz2Center[irunpd][ivz]->Write();
			
				Href_aftLumVzCor[irunpd][ivz]->Write();
			}
		}
	}
	outf->Close();
	//-------------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------------
	if(roundIdx ==2 )
	{
		TFile *out2 = new TFile(outDir+"/out_QaCheck_Refmult_Run18_27GeV.root", "recreate");
		cout<<"outputfile: "<<out2->GetName()<<endl;
		out2->cd();
		for(int irunpd=0; irunpd<nRunPds; irunpd++)
		{
			for(int ivz=0; ivz<nVzBins; ivz++)
			{
				HrefRatio_SubVz2Center[irunpd][ivz]->SetTitle(RunPdTitles[irunpd]+": "+VzTitles[ivz]);
				HrefRatio_SubVz2Center[irunpd][ivz]->Write();
			}
		}
		out2->Close();
	}


	//-------------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------------
	if(roundIdx ==3 )
	{
		TFile *out3 = new TFile(outDir+"/out_QaCheck_Refmult_Run18_27GeV_afterShapeCor.root", "recreate");
		cout<<"outputfile: "<<out3->GetName()<<endl;
		out3->cd();
		for(int irunpd=0; irunpd<nRunPds; irunpd++)
		{
			for(int ivz=0; ivz<nVzBins; ivz++)
			{
				Href_aftShapeCor[irunpd][ivz]->Write();
				HrefRatio_aftShapeCor_SubVz2Center[irunpd][ivz]->SetTitle(RunPdTitles[irunpd]+": "+VzTitles[ivz]);
				HrefRatio_aftShapeCor_SubVz2Center[irunpd][ivz]->Write();
			}
		}
		out3->Close();
	}
}











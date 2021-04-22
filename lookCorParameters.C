const int nRunPds = 6;
const int nVzBins = 14;
TH1F* HrefRatio_SubVz2Center[nRunPds][nVzBins];

void lookCorParameters( )
{
	TFile *inf = new TFile("./outRefQA_new/out_CorrParms_Refmult_Run18_27GeV_forRound3.root", "read");
	for(int irun=0; irun<nRunPds; irun++)
	{
		cout<<"//========================================="<<endl;
		cout<<"//irun: "<<irun<<endl;
		cout<<"{"<<endl;
		for(int ivz=0; ivz<nVzBins; ivz++)
		{
			HrefRatio_SubVz2Center[irun][ivz] = (TH1F*)inf->Get(Form("HrefRatio_SubVz2Center_irunpd%d_ivz%d",irun,ivz));
			cout<<"//ivz: "<<ivz<<endl;
			cout<<"\"";
			for(int ib=0; ib<HrefRatio_SubVz2Center[irun][ivz]->GetNbinsX(); ib++)
			{
				if(ib<HrefRatio_SubVz2Center[irun][ivz]->GetNbinsX()-1)
				{
					cout<<HrefRatio_SubVz2Center[irun][ivz]->GetBinContent(ib+1)<<",";
				}
				else
				{
					cout<<HrefRatio_SubVz2Center[irun][ivz]->GetBinContent(ib+1);
				}
			}

			if(ivz<nVzBins-1) cout<<"\""<<","<<endl;
			else if(ivz==nVzBins-1) cout<<"\""<<endl;
		}

		if(irun<nRunPds-1) cout<<"\n},"<<endl;
		else cout<<"\n}"<<endl;
	}
}

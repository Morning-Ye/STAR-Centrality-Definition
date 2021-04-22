//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Sep 27 15:41:13 2019 by ROOT version 5.34/30
// from TTree mEvent/mEvent
// found on file: /star/data01/pwg/zaochen/analysis/Run18/Run18_27GeV/outRefTree/checkStRefMultCorr/half1/reftree_1B45F9212ECAA31DF51D4B829AB9D02B_415.root
//////////////////////////////////////////////////////////

#ifndef mEvent_h
#define mEvent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class mEvent 
{
	public :
		TTree          *fChain;   //!pointer to the analyzed TTree or TChain
		Int_t           fCurrent; //!current Tree number in a TChain

		// Declaration of leaf types
		Int_t           mtrigWord;
		Float_t         mfield;
		Float_t         mzdcX;
		Float_t         mbbcX;
		Int_t           mrunId;
		Int_t           mrefmult;
		Bool_t          mIsPileUp;
		Float_t         mrefmultCorr;
		Float_t         mreweight;
		Int_t           mcent9;
		Int_t           mcent16;
		Int_t           mnTofMatch;
		Float_t         mvx;
		Float_t         mvy;
		Float_t         mvz;
		Float_t         mvpdVz;

		// List of branches
		TBranch        *b_mtrigWord;   //!
		TBranch        *b_mfield;   //!
		TBranch        *b_mzdcX;   //!
		TBranch        *b_mbbcX;   //!
		TBranch        *b_mrunId;   //!
		TBranch        *b_mrefmult;   //!
		TBranch        *b_mIsPileUp;   //!
		TBranch        *b_mrefmultCorr;   //!
		TBranch        *b_mreweight;   //!
		TBranch        *b_mcent9;   //!
		TBranch        *b_mcent16;   //!
		TBranch        *b_mnTofMatch;   //!
		TBranch        *b_mvx;   //!
		TBranch        *b_mvy;   //!
		TBranch        *b_mvz;   //!
		TBranch        *b_mvpdVz;   //!

		mEvent(TTree *tree=0);
		virtual ~mEvent();
		virtual Int_t    Cut(Long64_t entry);
		virtual Int_t    GetEntry(Long64_t entry);
		virtual Long64_t LoadTree(Long64_t entry);
		virtual void     Init(TTree *tree);
		virtual void     Loop();
		virtual Bool_t   Notify();
		virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef mEvent_cxx
mEvent::mEvent(TTree *tree) : fChain(0) 
{
	// if parameter tree is not specified (or zero), connect the file
	// used to generate this class and read the Tree.
	if (tree == 0) {
		TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/star/data01/pwg/zaochen/analysis/Run18/Run18_27GeV/outRefTree/checkStRefMultCorr/half1/reftree_1B45F9212ECAA31DF51D4B829AB9D02B_415.root");
		if (!f || !f->IsOpen()) {
			f = new TFile("/star/data01/pwg/zaochen/analysis/Run18/Run18_27GeV/outRefTree/checkStRefMultCorr/half1/reftree_1B45F9212ECAA31DF51D4B829AB9D02B_415.root");
		}
		f->GetObject("mEvent",tree);

	}
	Init(tree);
}

mEvent::~mEvent()
{
	if (!fChain) return;
	delete fChain->GetCurrentFile();
}

Int_t mEvent::GetEntry(Long64_t entry)
{
	// Read contents of entry.
	if (!fChain) return 0;
	return fChain->GetEntry(entry);
}
Long64_t mEvent::LoadTree(Long64_t entry)
{
	// Set the environment to read one entry
	if (!fChain) return -5;
	Long64_t centry = fChain->LoadTree(entry);
	if (centry < 0) return centry;
	if (fChain->GetTreeNumber() != fCurrent) {
		fCurrent = fChain->GetTreeNumber();
		Notify();
	}
	return centry;
}

void mEvent::Init(TTree *tree)
{
	// The Init() function is called when the selector needs to initialize
	// a new tree or chain. Typically here the branch addresses and branch
	// pointers of the tree will be set.
	// It is normally not necessary to make changes to the generated
	// code, but the routine can be extended by the user if needed.
	// Init() will be called many times when running on PROOF
	// (once per file to be processed).

	// Set branch addresses and branch pointers
	if (!tree) return;
	fChain = tree;
	fCurrent = -1;
	fChain->SetMakeClass(1);

	fChain->SetBranchAddress("mtrigWord", &mtrigWord, &b_mtrigWord);
	fChain->SetBranchAddress("mfield", &mfield, &b_mfield);
	fChain->SetBranchAddress("mzdcX", &mzdcX, &b_mzdcX);
	fChain->SetBranchAddress("mbbcX", &mbbcX, &b_mbbcX);
	fChain->SetBranchAddress("mrunId", &mrunId, &b_mrunId);
	fChain->SetBranchAddress("mrefmult", &mrefmult, &b_mrefmult);
	fChain->SetBranchAddress("mIsPileUp", &mIsPileUp, &b_mIsPileUp);
	fChain->SetBranchAddress("mrefmultCorr", &mrefmultCorr, &b_mrefmultCorr);
	fChain->SetBranchAddress("mreweight", &mreweight, &b_mreweight);
	fChain->SetBranchAddress("mcent9", &mcent9, &b_mcent9);
	fChain->SetBranchAddress("mcent16", &mcent16, &b_mcent16);
	fChain->SetBranchAddress("mnTofMatch", &mnTofMatch, &b_mnTofMatch);
	fChain->SetBranchAddress("mvx", &mvx, &b_mvx);
	fChain->SetBranchAddress("mvy", &mvy, &b_mvy);
	fChain->SetBranchAddress("mvz", &mvz, &b_mvz);
	fChain->SetBranchAddress("mvpdVz", &mvpdVz, &b_mvpdVz);
	Notify();
}

Bool_t mEvent::Notify()
{
	// The Notify() function is called when a new file is opened. This
	// can be either for a new TTree in a TChain or when when a new TTree
	// is started when using PROOF. It is normally not necessary to make changes
	// to the generated code, but the routine can be extended by the
	// user if needed. The return value is currently not used.

	return kTRUE;
}

void mEvent::Show(Long64_t entry)
{
	// Print contents of entry.
	// If entry is not specified, print current entry
	if (!fChain) return;
	fChain->Show(entry);
}
Int_t mEvent::Cut(Long64_t entry)
{
	// This function may be called from Loop.
	// returns  1 if entry is accepted.
	// returns -1 otherwise.
	return 1;
}
#endif // #ifdef mEvent_cxx

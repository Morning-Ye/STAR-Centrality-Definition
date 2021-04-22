#include <TSystem>

class StMaker;
class StChain;
class StPicoDstMaker;

StChain *chain;

void prodRefTree(const Char_t *inputFile="bugRuns36.list", const Char_t *outputFile="testRef_tree.root")
{
    //Load all the System libraries
    gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
    loadSharedLibraries();

    gSystem->Load("StChain");
    gSystem->Load("StPicoEvent");
    gSystem->Load("StPicoDstMaker");
	gSystem->Load("StPico2RefTreeMaker");

    chain = new StChain();

    StPicoDstMaker *picoMaker = new StPicoDstMaker(2, inputFile, "picoDst");

    //St_db_Maker *dbMk = new St_db_Maker("db","MySQL:StarDb","$STAR/StarDb","StarDb");
    //dbMk->SetDateTime(2016*1e4+101,0);
    //StMagFMaker *magfMk = new StMagFMaker; 

    StPico2RefTreeMaker *anaMaker = new StPico2RefTreeMaker("ana",picoMaker,outputFile);

	anaMaker->addTrigger(610001, 0); //mb
	anaMaker->addTrigger(610011, 0); //mb
	anaMaker->addTrigger(610021, 0); //mb
	anaMaker->addTrigger(610031, 0); //mb
	anaMaker->addTrigger(610041, 0); //mb
	anaMaker->addTrigger(610051, 0); //mb

	anaMaker->addTrigger(610001, 1); //mbtrg1
	anaMaker->addTrigger(610011, 2); //mbtrg2
	anaMaker->addTrigger(610021, 3); //mbtrg3
	anaMaker->addTrigger(610031, 4); //mbtrg4
	anaMaker->addTrigger(610041, 5); //mbtrg5
	anaMaker->addTrigger(610051, 6); //mbtrg6

    chain->Init();
    cout << "chain->Init();" << endl;

	int nEvents = picoMaker->chain()->GetEntries();
	cout << " Total entries = " << nEvents << endl;
	
	int total = 0;
	for ( int i = 0; i < nEvents; i++ )
	{
		if ( i%1000 == 0 )
		{
			cout << "Working on eventNumber " << i << " of "<< nEvents << " (" << fixed <<setprecision(1) << "finished "<< ((float)i/(float)nEvents)*100.0 << "%)" << endl;
		}

		chain->Clear();
		int iret = chain->Make(i);

		if ( iret )
		{
			cout << "Bad return code!" << iret << endl;
			break;
		}

        total++;
    }

    cout << "****************************************** " << endl;
    cout << "Work done... now its time to close up shop!" << endl;
    cout << "****************************************** " << endl;
    chain->Finish();
    cout << "****************************************** " << endl;
    cout << "total number of events  " << total << endl;
    cout << "****************************************** " << endl;

    delete chain;
}

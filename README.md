
# Table of Contents

1.  [Instructions for  Centrality defintion](#orgd09c76e)
    1.  [Slides summarizing Centrality Definition study for Run18 27 GeV Au+Au data](#org471b253)
    2.  [Refmult Correction in Real Data (First part) and Glauber Model Simulation (Second part)](#orgbc543ba)
    3.  [First part](#orgae8a99e)
    3.  [Second part](#org14422d1)



<a id="orgd09c76e"></a>

# Instructions for  Centrality defintion

Note: This document is from Zaochen's personal experience, not the STAR official instructions.
If anybody find there are any mistakes or there are something can be improved please feel free to contact me, thanks!
(zaochen.ye.2017@gmail.com, zaochen.ye@rice.edu). Hope all users could help future people do the centrality definition
at STAR easier with a better instruction document.
All the codes or scripts used in this instruction can be downloaded from this github page or from Zaochen's personal RCF directory.

<a id="org471b253"></a>

## Slides summarizing Centrality Definition study for Run18 27 GeV Au+Au data

Please find the detailes about the centrality definition studies,
here are the full example for the Run18 27 GeV study.

The detailed summary about the Refmult corrections can be found at:
<https://drupal.star.bnl.gov/STAR/system/files/Centrality_for_Run18_27GeVAuAu_ZaochenYe_20190827.pdf>

This was presented to the centrality meeting, and a follow up slides:
<https://drupal.star.bnl.gov/STAR/system/files/CentralityDefinition_Run18_27GeV_ZaochenYe_20190930.pdf>


<a id="orgbc543ba"></a>

## Refmult Correction in Real Data (First part) and Glauber Model Simulation (Second part)

I prepared some instructions on the coding part, they are all used
for the Run18 27GeV AuAu data, you may take them as a starting
point. I can imagine a few places need to be modified for other
datasets.


<a id="orgae8a99e"></a>

## First part

The first part are Procedures to do the Refmult Correction in Real
Data.

The Raw Refmult distribution in the Real data usually has some
dependence on the Luminosity (usually use ZDCrates) and Vz, which
could be related to the tracking efficiency and acceptance effects.

I prepared the codes to perform this study:

Save a MiniTree from PicoDst file first and then do the correction
study based on this minitree. The mini tree will only save the
information for the centrality definition study

-   Read PicoDst file and Save the MiniTree
    -   `/star/u/zye20/allScripts/analysis/Run18/Run18_27GeV/prodRefTree.C`
    -   To run test: `root4star -b -q -l prodRefTree.C`
    -   To submit jobs: `submit_RefTree_half1.xml` and `submit_RefTree_half2.xml`
        -   For 27GeV there are two datasets, which were produced
            separately, if you are working on other datasets, you may not
            need two .xml to submit jobs
    -   Do Correction Study based on the MiniTree
        
        -   `/star/u/zye20/allScripts/analysis/Run18/readAna/RefmultCorr`
        -   To run test: `./refmultCorr test.list test roundIndex`,
            -   `roundIndex` = 0,1,2,3 or 4. (In order to run them, you need
                to have the root file in the `./inputfiles`, named as
                `out_CorrParms_Refmult_Run18_27GeV_forRound*.root`)
                -   0 means read the Raw Refmult from data, fill histograms
                    for the Luminosity Correction factors,
                -   1 means apply Luminosity Correction on Refmult and fill
                    the histogram for Vz Correction factors,
                -   2 means apply Luminosity and Vz correction and then fill
                    the histograms for Shape re-weight,
                -   3 means apply Luminosity, Vz correction and the Shape
                    re-weight to get the final corrected Refmult
                    distribution. These distributions will be used as the
                    input RefmultCorr the Glauber Model simulation study. The
                    codes to quick look at the shape re-weight factors:
                    (lookCorParameters.C)
                -   4. Skip for now. Will do this later.
        
        -   To submit jobs: `submit_half1_round*.xml` and
            `submit_half2_round*.xml`, here `*` stands for 0,1,2,3
            or 4. After each round of jobs finished, you will need the
            following code to obtain the correction parameters based on
            the output of each round and get the output parameters root
            files for the next round of jobs. (`new_mkRefQA.C`)
        -   Please note that before obtaining the Reweight factor (Ratio
            of MC/Data of Refmult), we don’t need to run for “4”, 4 will
            be run while applying the Reweight factor and compared to the
            MC. This step should be done after the Glauber Model Studies.


<a id="org14422d1"></a>

### Second part

The second part is the Glauber Model simulation.

I am not sure whether there is an official package from STAR, but I
do have several packages which were used for previous centrality
studies
`/star/u/zye20/allScripts/analysis/GlauberSimu/glauberTune_example/`).
If you want to start with the one I modified for Run18 27 GeV AuAu
centrality definition. You can try the following procedures:

The procedures to run the Glauber Model simulation

-   Copy the codes to your own directory, the codes is put at:
    -   `/star/u/zye20/allScripts/analysis/GlauberSimu/glauberTune_example/glauberTune_example_zaochen`
-   `./prepare.sh` (to create needed directories, please look into this
    simple script before run it, just in case of deleting your
    important files by mistake)
    -   A tip here, put all the codes under your RCF home directory and
        set the outDir to be on your PWG disk.
-   Then execute `cons`, (to compile all the codes, please be sure
    there is no errors to compile the codes)
-   `./all_submit_doFastGlauberMcMaker.csh AuAu 27 0 5 1`
    -   The first input parameter:       27 is the energy per nucleon
    -   The second input parameter: 0 is the start job index
    -   The third input parameter:      5 is the end job index
    -   The fourth input parameter: 1 means "submit jobs”; 0 means “only
        test, won't submit"
    -   For the test run, you can change the `nevents` in
        `all_submit_doFastGlauberMcMaker.csh` to 10, to submit a test run,
        after all jobs finish successfully and the output files looks
        good (if you don’t know they are good or not, just move forward,
        you will know it later), then change it to the 100k (100000) or
        500k, for real submissions.
-   After the previous jobs finished, then you can Scan Negative
    Binomial Distribution (NBD) fitting parameters for the best
    parameters
    -   `./creatList.csh` (to prepare the input TTree file list )
    -   `root4star -b -q -l addNcollVsNpart.C` (create `ncoll_npart.root`
        file in the current directory, this will be used for the NBD scan
        on the parameters: npp, k, x, and eff)
    -   The scan ranges of npp, k, x and eff can be defined in
        `submit_doScan.pl`, in order to set a reasonable ranges, you
        better check the best parameters from previous study first, the
        previous parameters can be found in
        `StRoot/StCentralityMaker/StCentrality.cxx`, if you only want to
        submit 1 job with a constant npp, k, x and eff values, you can
        set nppbin, kin, xbin, effbin = 1 and let the min and max of npp,
        k, x and eff be same value.
    -   Please note that, the goal of scan is to find the best parameters
        matching to real data, the real data here is the Refmult
        distribution after all corrections (luminosity, vertex…), usually
        called RefmultCorr. In this example, you can find the Refmult is
        saved in a root file `run18_27Gev_MB_refMultCorr.root`, the
        corresponding histogram is named as `hRefMultCorr`, the name of
        the root file need to be given in the `doScanX_my.csh`, and the
        name of the histogram need to be given in
        `doNbdFitMaker.C`. Please make sure you have the input root file
        under this directory.
    -   Another thing need to be noted is the Refmult lower limit cut in
        `doScanX_my.csh`, usually cut on Refmult>100 for the 200 GeV AuAu
        Refmult scan, for 27 GeV, we choose to cut on 50. This cut is the
        fitting Refmult fitting range (while compare MC to Data).
    -   After you selected the parameter ranges, put the root file with
        hRefmultCorr under this directory, set the Refmult Cut and modify
        the names in the script and code as above, you can then start to
        submit jobs for the scan by `./submit_doScan.pl`
    -   After the above jobs finished, you will find the output root
        files under `RatioCHi2Files` which include `chi2*.root` and
        `Ratio*.root`, they are named by the events, npp, k, x and eff
        values. The out log files corresponding these jobs can be found
        at `LOG_Scan`.
    -   Then you can `cd getBestChi2_fromCat`, run the following scripts to
        find the best Chi2/ndf root files, and then based on the names of
        the root file, you will know all the parameter values.
        -   `./extractChi2Lines.sh`, it will read the output log files and
            write down the lines with Chi2/ndf information
        -   `./genMinChi2List.sh`, it will read the output from previous step
            and find the least Chi2/ndf root files and write them into a
            file “minChi2<sub>file.list</sub>”, please note that, here we still don’t
            know the eff values corresponding to the least Chi2/ndf, we
            need one more step
        -   `./getBestChi2RootFile.sh`, it will finally find best parameters
            of npp,k,x,eff and the root files corresponding to the least
            Chi2/ndf and copy them into `bestChisRootFile`
    -   After obtained all the parameter values, and the corresponding
        output root files, we can get the Ratio of MC/Data in the root
        file (`Ratio_npp1.270_k1.800_x0.160_eff0.110.root`), named as
        `hRatio`, you can then use this ratio to weight your RefmultCorr
        in the real data, and obtain the RefmultCorr<sub>Weighted</sub> and save
        into a new root file, in my case I name the root file as
        `run18_27Gev_MB_refMultCorr_Weighted.root`, you can then fix the
        fitting parameters in `./submit_doScan.pl` and modify the input
        root file name in `doScanX_my.csh` and input histogram name in
        `doNbdFitMaker.C`, and then submit it a single job by
        `./submit_doScan.pl` to get a new `hRatio` histogram in the
        `Ratio*root` file. You should find that the new `hRatio` should be
        flat at 1, since the the new input `hRefmultCorr_Weighted` is
        obtained by weighting the RefmultCorr with MC/Data ratio. It is
        always good to make a plot to compare the hRefmultCorr,
        hRefmultsim, hRatio before and after reweitht, to make sure
        everything working correctly.
    -   If everything so far looks good, then you finished most parts of
        the steps. Now move to the systematic uncertainty calculaltion in
        the next step.
-   Systematic Uncertainty Estimation:
    -   Go the `LOG_Scan`, and find the output log files like
        `doScanX_my_1_27_1_8_0_16_0_11.out`, the name is defined based on
        the parameter values, so it should be easy to find it.  Copy the
        line 173-204 and paste them to
        `StRoot/StCentralityMaker/StCentrality.cxx` and change all the
        npp, k, x and eff values to those you obtained in the same code.
    -   `./createList.csh`
    -   `./all_doAnalysisMaker.csh AuAu_27GeV kFALSE kFALSE`
        -   In this step it will read the output tree files generated at
            the first step from `all_submit_doFastGlauberMcMaker.csh`,
        -   Write down the new output files `ana*.root`
        -   Produce all tables under `./table`
    -   `./all_doPlotMaker.csh 27`
        -   This will make all the plots and save into `./figure`


/**********************************************************************************
 * Project   : MLBWOProcessor - A processor for TopMassSecVtx/mlbwidth output     *
 * Package   : ROOT                                                               *
 * Root Macro: root -l<ENTER>, .L MLBWOProcessor.C                                *
 *                                                                                *
 * A standalone application to handle output from Benjamin Stieger's mlbwidth     *
 *                                                                                *
 * NOTE: You should not have to change anything except for the array contents     *
 *       to produce output with this code! To compile:                            *
 *                                                                                *
 *          g++ -o <executable name you want> MLBWidthOProcessor.C                *
 *                  `root-config --cflags --glibs`                                *
 *                                                                                *
 *       Then run the executable with the first argument as the file you want     *
 *       to process.                                                              *
 *                                                                                *
 * Author: Evan Coleman, 2015                                                     *
 **********************************************************************************/

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <map>
#include <string>

#include "TSystem.h"
#include "TROOT.h"
#include "TPad.h"
#include "TLatex.h"
#include "TLine.h"
#include "TBox.h"
#include "TASImage.h"
#include "TImage.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TH1F.h"
#include "TFile.h"
#include "TRegexp.h"
#include "TString.h"
#include "TList.h"

using std::cout;
using std::endl;
using std::vector;
using std::cin;

//***************************************************************************//
// Global Variables (to be changed by user in main()?)
//  o char*[] leps - contains the lepton combinations we care about in the
//                   final state
//  o char*[] procs - contains the regular expressions separating processes
//                    from one another (see TRegExp for more information,
//                    or regexr.com). Note they should not contain "//(g)"
//                    formatting, and escape sequences can cause errors.
//  o char*[] procReplace - contains the names we'd like to use to replace the
//                          given process names when we output everything to
//                          a MassFit_print-formatted root file
//  o char*[] dataprocs - Key words to look for in the titles of data
//                        data histograms. They should correspond in order 
//                        to leps
//  o char*[] yieldLaTeX - The LaTeX to use for each procs[i] name in the 
//                         yields table
//  o TString outfileName - What title to give the MassFit-formatted output
//  o double[][] eCounts - Keeps track of event yields for each process-
//                         final-state pair, including data
//                         [leps.size()][procs.size()+1]
//  o double[][] eErrors - Keeps track of event yields for each process-
//                         final-state pair, including data
//***************************************************************************//
const char* leps[5]  = { "E", "EE", "EM", "MM", "M" };
const char* procs[6] = { "DY", "WW", "[^T]W[1234]?Jets", "QCD", "SingleTbar", "TTW?Jets" };
const char* procReplace[6] = { "DrellYan", "Diboson", "WJets", "QCD", "SingleTop", "TTbar" };
const char* dataprocs[5] = { "SingleElectron2012A", "DoubleElectron2012A", 
                             "MuEG2012A", "DoubleMu2012A", "SingleMu2012A" };
const char* yieldLaTeX[6] = { "Drell-Yan", "Diboson", "W+Jets", "QCD", "Single-top", 
                              "$t\\bar{t}$" };
const char* lepsLaTeX[5] = { "e", "ee", "e$\\mu$", "$\\mu\\mu$", "$\\mu$" };
const char* signalNames[1] = { "TTbar" };
const TString outfileName("../2012_combined_EACTLJ.root");

/////////////////////////////////////////////////////
//                      Utils                      //
/////////////////////////////////////////////////////
#define GETARRSIZE(arr) (sizeof((arr))/sizeof((arr[0])))

const int lepsSize          = GETARRSIZE(leps);
const int procsSize         = GETARRSIZE(procs);
const int procReplaceSize   = GETARRSIZE(procReplace);
const int dataprocsSize     = GETARRSIZE(dataprocs);
const int yieldLaTeXSize    = GETARRSIZE(yieldLaTeX);
const int lepsLaTeXSize     = GETARRSIZE(lepsLaTeX);
const int signalNamesLength = GETARRSIZE(signalNames);

double eCounts[lepsSize][procsSize+1];
double eErrors[lepsSize][procsSize+1];

double nominalWidth = 1.5;
vector<std::pair<double, TString> > moreFiles;
bool interpolate = false;
int interpolations = 0;

//Returns true if the arrays are sized properly
bool validateArrays() {
  return (lepsSize      == lepsLaTeXSize
           && lepsSize  == dataprocsSize
           && procsSize == procReplaceSize
           && procsSize == yieldLaTeXSize);
}

/////////////////////////////////////////////////////
//        Formatting Yields Data in LaTeX          //
/////////////////////////////////////////////////////

//LaTeX formatting for individual results
TString GetLatex(int lep, int proc) {
  char b[128];
  sprintf(b, "%.0f $\\pm$ %.0f", round(eCounts[lep][proc]), round(eErrors[lep][proc]));
  return TString(b);
}

//LaTeX formatting for sums of results
TString GetLatexSum(bool rowSum, int ind) {
  double sum = 0;
  double err = 0;

  //complicated sum
  if(rowSum) {
    // if we sum via rows, go through the rows
    for(int i=0; i<lepsSize; i++) {
      sum+=eCounts[i][ind];
      err = sqrt(pow(err,2) + pow(eErrors[i][ind],2));
    }
  } else {
    // do we want to sum all MC? if ind>=0, no
    if(ind>=0) {
      for(int i=0; i<procsSize; i++) {
        sum+=eCounts[ind][i];
        err = sqrt(pow(err,2) + pow(eErrors[ind][i],2));
      }
    } else {
      for(int i=0; i<lepsSize; i++) {
        for(int j=0; j<procsSize; j++) {
          sum += eCounts[i][j];
          err = sqrt(pow(err,2) + pow(eErrors[i][j],2));
        }
      }
    }
  }
 
  char b[128];
  sprintf(b, "%.0f $\\pm$ %.0f", round(sum), round(err));
  return TString(b);
}

//LaTeX formatting for final row of ratios
TString GetLatexRatio(int ind) {
  double sumMC   = 0;
  double errMC   = 0;
  double data    = 0;
  double errData = 0;
  //if we want to look at data/MC for just one column, do so
  if(ind<=lepsSize && ind>0) {
    data = eCounts[ind-1][procsSize];
    errData = eErrors[ind-1][procsSize];

    for(int i=0; i<procsSize; i++) {
      sumMC += eCounts[ind-1][i];
      errMC =  sqrt(pow(errMC,2) + pow(eErrors[ind-1][i],2));
    }
  } else {
    // if not, sum all of MC and data and take the ratios
    for(int i=0; i<lepsSize; i++) {
      data    += eCounts[i][ind];
      errData =  sqrt(pow(errData,2) + pow(eErrors[i][ind],2));
      for(int j=0; j<procsSize; j++) {
        sumMC += eCounts[i][j];
        errMC = sqrt(pow(errMC,2) + pow(eErrors[i][j],2));
      }
    }
  }

  char b[128];
  sprintf(b, "%.3f $\\pm$ %.3f", data/sumMC, data/sumMC*sqrt(pow(errData/data,2) + pow(errMC/sumMC,2)));
  return TString(b);
}

/////////////////////////////////////////////////////
//       Formatting output histogram names         //
/////////////////////////////////////////////////////

TString formatName(const char* histoName, double signalWidth) {
    TObjArray *nameParts = (TObjArray*) TString(histoName).ReplaceAll("_mlbwa_",'#').Tokenize(TString("#"));

    cout<<nameParts->At(0)->GetName()<<" "<<nameParts->At(1)->GetName()<<endl;
    // get the two significant parts of the name and split into relevant information
    TObjArray *procPart = (TObjArray*) TString(nameParts->At(0)->GetName()).Tokenize("_");
    TObjArray *histPart = (TObjArray*) TString(nameParts->At(1)->GetName()).Tokenize("_");

    // obtain useful TStrings
    TString proces = TString(procPart->At(1)->GetName());
    TString lepton = TString(histPart->At(0)->GetName());
    TString delmtr = TString("_");

    // if data, just say data 
    if(TString(procPart->At(0)->GetName()).Contains("Data")) proces = TString("Data");
    else {
      cout<<" - formatting "<<proces<<endl;
      for(int pInd = 0; pInd<procsSize; pInd++) {
        if((delmtr+proces).Contains(TRegexp(procs[pInd]))) {
            cout<<" -- match found, "<<procs[pInd]<<endl;
            proces = procReplace[pInd];
            cout<<" -- procs now = "<<proces<<endl;

            // loop through the signal names and format with signalWidth if
            // there's a match (i.e., mlbwa__TTbar_7.5_E)
            for(int sigCheck=0; sigCheck<signalNamesLength; sigCheck++) {
              if(proces==signalNames[sigCheck]) {
                cout<<" --- checking against "<<signalNames[sigCheck]<<endl;
                cout<<" --- procs is a signal process"<<endl;
                char wid[32];
                sprintf(wid, "%.2f",signalWidth);
                proces += delmtr+TString(wid);
                cout<<" --- procs now = "<<proces<<endl;
                break;
              }
            }
            break;
        } else {
            cout<<" -- not a match: "<<procs[pInd]<<" and "<<proces<<endl;
        }
      }
    }

    // mlbwa__<Process>_<E/EE/EM/MM/M>
    return TString("mlbwa__")+proces+delmtr+lepton;
}

/*************************************************************************************
 * getYields: Goes through each Counts histogram for every process and subprocess
 *            (i.e., EE & Drell-Yan, E & WJets, etc.) and properly adds the yields
 *            for data and MC. Reports ratios of Data:MC as well.
 *  input:  the main() arguments array
 *  output: writes to stdout the LaTeX-formatted table of yields (pipe the output to 
 *          save)
 ***********************************/
void getYields(const char* argv[]) { 
  //open mlbwidth output file
  TFile *f = new TFile(argv[2]);
  f->cd();

  //very inefficient, but for now it works
  //loop over all procs, leps and figure out the event counts
  for(int i=0; i<lepsSize; i++) {
    char a[128];
    sprintf(a, "mlbwa_%s_Count", leps[i]);
    TDirectory *tDir = (TDirectory*) f->Get(a);
    TList *alok = tDir->GetListOfKeys();

    for(int j=0; j<procsSize; j++) {
      for(int k=0; alok->At(k)->GetName() != alok->Last()->GetName(); k++) {
        if(TString(alok->At(k)->GetName()).Contains(TRegexp(procs[j]))) {
          char b[128];
          sprintf(b, "mlbwa_%s_Count/%s", leps[i], alok->At(k)->GetName());

          TH1F *h = (TH1F*) f->Get(b);
          eCounts[i][j] += h->GetSumOfWeights();
          eErrors[i][j] =  sqrt(pow(eErrors[i][j],2) + pow(h->GetBinError(2),2));

          delete h;
        }
      }
    }

    char d[128];
    sprintf(d, "mlbwa_%s_Count/%s", leps[i], alok->Last()->GetName());
    if(d == "") { exit(EXIT_FAILURE); }

    TH1F *tth = (TH1F*) f->Get(d);
    eCounts[i][procsSize] = tth->GetEntries();
    double integral = tth->GetSumOfWeights();
    eErrors[i][procsSize] = tth->GetBinError(2)*eCounts[i][procsSize]/integral;
    delete tth;
  }

  //Get a string to tell us how many columns we want (size leps + 1)
  char cols[] = "c";
  for(int i=0; i<lepsSize; i++) {
    strcat(cols, "c");
  }

  //print out LaTeX:
  //formatting
  cout<<"\\documentclass[12pt,a4paper,titlepage]{article}"<<endl;
  cout<<"\\usepackage[utf8]{inputenc}"<<endl;
  cout<<"\\usepackage{amsmath}"<<endl;
  cout<<"\\usepackage{amsfonts}"<<endl;
  cout<<"\\usepackage{amssymb}"<<endl;
  cout<<"\\usepackage{hyperref}\n\n"<<endl;
  cout<<"\\usepackage[margin=0.0025in]{geometry}"<<endl;
  cout<<"\\begin{document}"<<endl;
  cout<<"\\begin{tabular}{l|"<<cols<<"} \\\\"<<endl;

  //Print out the table header
  cout<<"Sample & ";
  for(int i=0; i<lepsLaTeXSize; i++) {
    cout<<lepsLaTeX[i]<<" & ";
  }
  cout<<"Sum \\\\ \\hline\\hline"<<endl;

  //Printing out the yields for each process
  for(int i=0; i<procsSize; i++) {
      cout<<yieldLaTeX[i]<<" & ";
    for(int j=0; j<lepsSize; j++) {
      cout<<GetLatex(j,i)<<" & ";
    }
    cout<<GetLatexSum(true,i)<<"\\\\"<<(i==procsSize-1 ? "\\hline" : "")<<endl;
  }
 
  //Print out the total MC yield
  cout<<"Total MC & ";
  for(int i=0; i<lepsSize; i++) {
    cout<<GetLatexSum(false,i)<<" & ";
  }
  cout<<GetLatexSum(false,-1)<<"\\\\"<<endl;

  //Print out the data yield
  cout<<"Data & ";
  for(int i=0; i<lepsSize; i++) {
    cout<<GetLatex(i,lepsSize+1)<<" & ";
  }
  cout<<GetLatexSum(true,lepsSize+1)<<"\\\\\\hline\\hline"<<endl;
  
  //Print out the data:MC ratio
  cout<<"Data/MC & ";
  for(int i=1; i<=procsSize;i++) {
    cout<<GetLatexRatio(i)<<(i==procsSize ? "" : " & ");
  }
  cout<<"\\\\"<<endl;

  //We did it! End the document
  cout<<"\\end{tabular}"<<endl;
  cout<<"\\end{document}"<<endl;
}

/*************************************************************************************
 * getKS: Searches through the histograms in the plotter output, adds the MC together
 *        for each field, and compares the MC with the Data histogram using a KS test
 *  input:  the main() arguments array
 *  output: writes to stdout the (human-readable) KS statistics of pairs of histograms
 *
 *  Structure-wise: this is fine, can be implemented into class easily.
 ***********************************/
void getKS(const char* argv[]) {
  //open the input TFile
  TFile *f = new TFile(argv[2]);
  f->cd();

  //get the filesystem information from the file
  TList *alokDirs = (TList*) f->GetListOfKeys();

  //loop through the directories in the input file
  for(int idir=0; alokDirs->At(idir-1) != alokDirs->Last(); idir++) {
    TDirectory *cDir  = (TDirectory*) f->Get(alokDirs->At(idir)->GetName());
    TList *alokHistos = (TList*)      cDir->GetListOfKeys();

    // create the MC histogram and start collecting relevant MC histograms
    // from the current directory
    TList *aloh   = new TList;
    // loop through keys (histograms) in current directory
    for(int ihisto=0; alokHistos->At(ihisto) != alokHistos->Last(); ihisto++) {
      if(TString(alokHistos->At(ihisto)->GetName()).Contains("MC8TeV")) {
        TH1F *cHisto = (TH1F*) cDir->Get(alokHistos->At(ihisto)->GetName());
        aloh->Add(cHisto);
      }
    }
 
    //merge the data histograms into one histogram
    TH1F *MCHisto = (TH1F*) (aloh->Last())->Clone(TString(cDir->GetName()) + TString("MCHisto"));
    aloh->RemoveLast();
    MCHisto->Merge(aloh);

    cout<<"-------------------- "<<cDir->GetName()<<" -----------------------"<<endl;
    //now create the data histogram and run the KS test
    TH1F *DataHisto = (TH1F*) cDir->Get(alokHistos->Last()->GetName());
    cout<<"  ---> KS Test: "<<cDir->GetName()<<" has probability "<<MCHisto->KolmogorovTest(DataHisto, "D")<<"\n"<<endl;
  }
}

/*************************************************************************************
 * createMFOutfile: moves the MLB distributions into an output file for use in 
 *                  R. Nally's MassFit.C code.
 *  input:  the main() arguments array
 *  output: writes to an output file the histograms, in a MassFit.C-readable format 
 *
 *  TODO: implement into class.
 ***********************************/
void createMFOutfile(const char* argv[]) {
  TFile *f = new TFile(argv[2]);

  //create the output file we'd like to write histograms to
  TFile *output = new TFile(outfileName, "RECREATE");
  
  //get the filesystem information from the file
  f->cd();
  TList *alokDirs = (TList*) f->GetListOfKeys();

  //create a list of "All" histograms and initialize (TODO)
  TList *allHists = new TList;

  //loop through the directories in the input file
  for(int idir=0; alokDirs->At(idir-1) != alokDirs->Last(); idir++) {
    //if it's not mlb, we don't care
    if(!TString(alokDirs->At(idir)->GetName()).Contains("_Mlb")) continue;
    
    //get the file directory information, its histograms
    TDirectory *cDir  = (TDirectory*) f->Get(alokDirs->At(idir)->GetName());
    TList *alokHistos = (TList*)      cDir->GetListOfKeys();

    //loop through the histograms in the current directory 
    for(int ihisto=0; alokHistos->At(ihisto-1) != alokHistos->Last(); ihisto++) {
      // don't consider the graph objects
      if(TString(alokHistos->At(ihisto)->GetName()).Contains("Graph_")) continue; 

      // clone the histogram, give it a new name
      TString cloneName = formatName(alokHistos->At(ihisto)->GetName(),nominalWidth);
      TH1F *thisto = (TH1F*) cDir->Get(alokHistos->At(ihisto)->GetName());
      TH1F *tclone = (TH1F*) thisto->Clone(cloneName);

      // open the outfile and write the histo in
      output->cd();
      TList *outkeys = (TList*) output->GetListOfKeys();

      // if the histogram already exists, add thisto to the existing one
      // messy, but removes the need for magic
      for(int iout=0; outkeys->At(iout-1) != outkeys->Last(); iout++) {
        if(outkeys->At(iout)->GetName() == cloneName) {
          cout<<" - found another histogram in output with name "<<cloneName<<endl;
          TH1F *theHisto = (TH1F*) output->Get(cloneName);
          cout<<" - got the same histogram from the output file"<<endl;
          TH1F *tHclone  = (TH1F*) theHisto->Clone(cloneName);
          cout<<" - cloned the histogram"<<endl;

          cout<<" - adding in clone"<<endl;
          tclone->Add(tHclone);

          cout<<" - deleting the original histogram from the output file"<<endl;
          output->Delete(cloneName + TString(";1"));
          cout<<" - deleted thing from output file"<<endl;
          cout<<" - tclone looks like "<<tclone<<endl;
        }
      }
      cout<<" - writing the tclone to file"<<endl;
      tclone->Write();

      // reopen the input root file and clean up
      f->cd();
      delete thisto;
      delete tclone;
    }
  }

  f->Close();
  output->cd();
  output->Close();

  // Now we want to collect signal histograms of different weights
  
  // Have to reopen the outfile because ROOT is odd
  TFile *output2 = new TFile(outfileName, "UPDATE");
  output2->cd();

  // Loop through the additional files
  for(std::vector<std::pair<double, TString> >::const_iterator pf=moreFiles.begin();
          pf!=moreFiles.end();
          pf++) {

      // This is the current file and width
      TFile *curFile = new TFile(pf->second, "READ");
      double curWid  = pf->first;

      // Loop through lepton final states
      for(int i=0; i<lepsSize; i++) {
          curFile->cd();

          // Get the desired directory
          char dirName[128];
          sprintf(dirName, "mlbwa_%s_Mlb", leps[i]);
          TDirectory *curDir = (TDirectory*) curFile->Get(dirName);

          // Get the names of the first histogram in the directory
          //     (we don't want more than one additional signal histo per extra file)
          // Format it with the usual method
          TString histName   = TString(curDir->GetListOfKeys()->First()->GetName());
          TString cloneName  = formatName(histName,curWid);

          // Get the mlb histo
          TH1D *curHisto = (TH1D*) curDir->Get(histName)->Clone(cloneName);

          // Write to the outfile
          output2->cd();
          curHisto->Write();
      }

      curFile->Close();
  }

  output2->cd();
  output2->Close();


}



#ifndef __CINT__
int main(int argc, const char* argv[]) {
  if(argc>3) {
    cout<<"Detected more than one width-input file pair. Adding in files..."<<endl;
    
    for(int i=3; i+1<argc; i+=2) {
      cout<<" - width "<<argv[i]<<" and location "<<argv[i+1]<<endl;
      std::pair<double, TString> tpair(TString(argv[i]).Atof(), TString(argv[i+1]));
      moreFiles.push_back(tpair); 
    }

    cout<<"\n"<<endl;
  }

  cout<<"Analyzing the output: "<<argv[2]<<" with nominal width "<<argv[1]<<"\n\n"<<endl;
  nominalWidth = TString(argv[1]).Atof();

  if(!validateArrays()) {
    cout<<"Arrays not formatted properly. Please check your "
        <<"input and recompile."<<endl;
    exit(EXIT_FAILURE);
  }

  cout<<"Here is the LaTeX for the yields table:"<<endl;
  getYields(argv);

  cout<<"\n\nHere is the KS information for the histograms:"<<endl;
  getKS(argv); 

  cout<<"\n\nLet me write the MassFit-readable file for you as well..."<<endl;
  createMFOutfile(argv);
  cout<<"...done!"<<endl; 
}
#endif

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
const char* procs[4] = { "DY", "tW", "t-ch", "TTbar" };
const char* procReplace[4] = { "DrellYan", "SingleTop-tW", "SingleTop-t", "TTbar" };
const char* yieldLaTeX[4] = { "Drell-Yan", "tW", "t-channel", "$t\\bar{t}$" };
const char* lepsLaTeX[5] = { "e", "ee", "e$\\mu$", "$\\mu\\mu$", "$\\mu$" };
const char* signalNames[1] = { "TTbar" };
const TString outfileName("2012_combined_EACTLJ");
const TString choiceMLB("min"); 

/////////////////////////////////////////////////////
//                      Utils                      //
/////////////////////////////////////////////////////
#define GETARRSIZE(arr) (sizeof((arr))/sizeof((arr[0])))

const int lepsSize          = GETARRSIZE(leps);
const int procsSize         = GETARRSIZE(procs);
const int procReplaceSize   = GETARRSIZE(procReplace);
const int yieldLaTeXSize    = GETARRSIZE(yieldLaTeX);
const int lepsLaTeXSize     = GETARRSIZE(lepsLaTeX);
const int signalNamesLength = GETARRSIZE(signalNames);

double eCounts[lepsSize][procsSize+1];
double eErrors[lepsSize][procsSize+1];

double nominalWidth = 1.5;
vector<double> moreWidths;

//Returns true if the arrays are sized properly
bool validateArrays() {
  return (lepsSize      == lepsLaTeXSize
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
    data    = eCounts[ind-1][procsSize];
    errData = eErrors[ind-1][procsSize];

    for(int i=0; i<procsSize; i++) {
      sumMC += eCounts[ind-1][i];
      errMC =  sqrt(pow(errMC,2) + pow(eErrors[ind-1][i],2));
    }
  } else {
    // if not, sum all of MC and data and take the ratios
    for(int i=0; i<lepsSize; i++) {
      data    += eCounts[i][procsSize];
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
    TObjArray *nameParts = (TObjArray*) TString(histoName).Tokenize("_");

    // obtain useful TStrings
    TString lepton = TString(nameParts->At(1)->GetName());
    TString proces = TString(nameParts->Last()->GetName());
    TString delmtr = TString("_");

    // if data, just say data 
    // if(TString(procPart->At(0)->GetName()).Contains("Data")) proces = TString("Data");
    if(nameParts->GetSize() < 5 || proces == choiceMLB) proces = TString("Data");
    else {
      cout<<" - formatting "<<proces<<endl;

      for(int pInd = 0; pInd<procsSize; pInd++) {

        // if the regular expression matches
        if((delmtr+proces).Contains(TRegexp(procs[pInd]))) {
            cout<<" -- match found, "<<procs[pInd]<<endl;
            proces = procReplace[pInd];
            cout<<" -- procs now = "<<proces<<endl;

            // loop through the signal names and format with signalWidth
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
      for(int k=0; k < alok->GetSize(); k++) {
        if(TString(alok->At(k)->GetName()).Contains(TRegexp(procs[j]))) {
          char b[128];
          sprintf(b, "mlbwa_%s_Count/%s", leps[i], alok->At(k)->GetName());

          TH1F *h = (TH1F*) f->Get(b);
          eCounts[i][j] += h->GetSumOfWeights();
          eErrors[i][j] =  sqrt(pow(eErrors[i][j],2) + pow(h->GetBinError(2),2));

          delete h;
        }
      }

      if(eErrors[i][j] == 0 && eCounts[i][j] > 0) {
          cout << "Errors are 0 for i = " << i << ", j = " << j << "." << endl;
          eErrors[i][j] = sqrt(eCounts[i][j]);
      }
    }

    // data information
    char d[128];
    sprintf(d, "mlbwa_%s_Count/mlbwa_%s_Count", leps[i], leps[i]);

    TH1F *tth = (TH1F*) f->Get(d);
    eCounts[i][procsSize] = tth->GetEntries();
    double integral = tth->GetSumOfWeights();
    eErrors[i][procsSize] = tth->GetBinError(2)*eCounts[i][procsSize]/integral;
    
    if(eErrors[i][procsSize] == 0 && eCounts[i][procsSize] > 0) {
        cout << "Errors are 0 for i = " << i << ", data." << endl;
        eErrors[i][procsSize] = sqrt(eCounts[i][procsSize]);
    }

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
    cout<<GetLatex(i,procsSize)<<" & ";
  }
  cout<<GetLatexSum(true,procsSize)<<"\\\\\\hline\\hline"<<endl;
  
  //Print out the data:MC ratio
  cout<<"Data/MC & ";
  for(int i=1; i<=lepsSize+1;i++) {
    cout<<GetLatexRatio(i)<<(i==lepsSize+1 ? "" : " & ");
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
 ***********************************/
void getKS(const char* argv[]) {
  
  TFile *f = new TFile(argv[2]);
  f->cd();

  //get the filesystem information from the file
  TList *alokDirs = (TList*) f->GetListOfKeys();

  //loop through the directories in the input file
  for(size_t idir=0; idir < alokDirs->GetSize(); idir++) {

    TDirectory *cDir  = (TDirectory*) f->Get(alokDirs->At(idir)->GetName());
    TList *alokHistos = (TList*)      cDir->GetListOfKeys();


    if(alokHistos->GetSize() < 2) continue;
    if(TString(cDir->GetName()).Contains("Tcmp")) continue;
    if(TString(cDir->GetName()).Contains("ratevsrun")) continue;

    // create the MC histogram and start collecting relevant MC histograms
    TList *aloh   = new TList;

    // loop through histograms in current directory
    for(int ihisto=0; ihisto < alokHistos->GetSize(); ihisto++) {
        TH1F *cHisto = (TH1F*) cDir->Get(alokHistos->At(ihisto)->GetName());

        if(TString(cHisto->GetName()).Contains("Graph")) continue;
        if(TString(cHisto->GetName()) == TString(cDir->GetName())) continue;
        aloh->Add(cHisto);
    }

    TH1F *MCHisto = (TH1F*) (aloh->Last())->Clone(TString(cDir->GetName()) + TString("MCHisto"));
    aloh->RemoveLast();
    MCHisto->Merge(aloh);

    //now create the data histogram and run the KS test
    cout<<"-------------------- "<<cDir->GetName()<<" -----------------------"<<endl;
    TH1F *DataHisto = (TH1F*) cDir->Get(cDir->GetName());
    cout << "  ---> KS Test: " << DataHisto->GetName()   << " has probability " 
         << MCHisto->KolmogorovTest(DataHisto, "D") << "\n" << endl;
  }
}

/*************************************************************************************
 * createMFOutfile: moves the MLB distributions into an output file for use in 
 *                  R. Nally's MassFit.C code.
 *  input:  the main() arguments array
 *  output: writes to an output file the histograms, in a MassFit.C-readable format 
 ***********************************/
void createMFOutfile(const char* argv[]) {
  
  TFile *f = new TFile(argv[2]);
  TFile *output = new TFile(TString(outfileName)+"_"+argv[3]+".root", "RECREATE");
  
  // get filesystem information
  f->cd();
  TList *alokDirs = (TList*) f->GetListOfKeys();

  // create list of "All" histograms and initialize (TODO)
  TList *allHists = new TList;

  // loop through directories input file
  for(int idir=0; alokDirs->At(idir-1) != alokDirs->Last(); idir++) {

    // must contain mlb 
    if(!TString(alokDirs->At(idir)->GetName()).Contains("_Mlb_"+choiceMLB)) continue;
    
    // get file directory information
    TDirectory *cDir  = (TDirectory*) f->Get(alokDirs->At(idir)->GetName());
    TList *alokHistos = (TList*)      cDir->GetListOfKeys();

    // loop through the histograms in the current directory 
    for(int ihisto=0; alokHistos->At(ihisto-1) != alokHistos->Last(); ihisto++) {

      // don't consider the graph objects
      if(TString(alokHistos->At(ihisto)->GetName()).Contains("Graph_")) continue; 

      // clone the histogram, give it a new name
      TString cloneName = formatName(alokHistos->At(ihisto)->GetName(),nominalWidth);
      TH1F *thisto = (TH1F*) cDir->Get(alokHistos->At(ihisto)->GetName());
      TH1F *tclone = (TH1F*) thisto->Clone(cloneName);

      // open outfile and write histo 
      output->cd();
      TList *outkeys = (TList*) output->GetListOfKeys();

      // if histogram already exists, add thisto to the existing one
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
          cout<<" - tclone looks like "<<tclone<<endl;
        }
      }
      cout<<" - writing the tclone to file"<<endl;
      tclone->Write();

      // clean up
      f->cd();
      delete thisto;
      delete tclone;
    }
  }

  // Now we want to collect signal histograms of different weights
  
  cout << "LOOP BEGINNING" << endl;
  // Loop through the additional widths
  for(size_t iWid = 0; iWid < moreWidths.size(); iWid++) {
      double curWid  = moreWidths.at(iWid);

      // Loop through lepton final states
      for(int i=0; i<lepsSize; i++) {
          f->cd();

          // Get desired directory
          char tdirName[128];
          sprintf(tdirName, "mlbwa_%s_%.2f_Mlb_%s", leps[i], curWid, choiceMLB.Data());
          TString dirName = TString(tdirName).ReplaceAll(".", "p");

          TDirectory *curDir = (TDirectory*) f->Get(dirName);

          // Get the names of the first histogram in the directory
          //     (we don't want more than one additional signal histo per extra file)
          TString histName   = TString(curDir->GetListOfKeys()->First()->GetName());
          TString cloneName  = formatName(histName,curWid);

          // Get mlb histo
          TH1D *curHisto = (TH1D*) curDir->Get(histName)->Clone(cloneName);

          // Write to outfile
          output->cd();
          curHisto->Write();
       }
  }

  f->Close();
  output->cd();
  output->Close();

}



#ifndef __CINT__
int main(int argc, const char* argv[]) {

  // process arguments
  if(argc>4) {
    cout<<"Detected more than one width-input width. Adding in..." << endl;

    for(int i=4; i<argc; i++) {
      cout<<" - width "<<argv[i]<<endl;
      moreWidths.push_back(TString(argv[i]).Atof()); 
    }

    cout<<"\n"<<endl;
  }

  cout<<"Analyzing the output: "<<argv[2]<<" with nominal width "<<argv[1]<<"\n\n"<<endl;
  nominalWidth = TString(argv[1]).Atof();

  // check if program is setup properly
  if(!validateArrays()) {
    cout<<"Arrays not formatted properly. Please check your "
        <<"input and recompile."<<endl;
    exit(EXIT_FAILURE);
  }

  // YIELDS
  cout<<"Here is the LaTeX for the yields table:"<<endl;
  getYields(argv);

  // KS
  //cout<<"\n\nHere is the KS information for the histograms:"<<endl;
  //getKS(argv); 

  // RooPoisson OUTFILE
  //cout<<"\n\nLet me write the MassFit-readable file for you as well..."<<endl;
  //createMFOutfile(argv);

  cout<<"\n\n...done!"<<endl; 
}
#endif

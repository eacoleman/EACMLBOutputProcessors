/**********************************************************************************
 * Project   : MLBWOProcessor - A processor for TopMassSecVtx/mlbwidth output     *
 * Package   : ROOT                                                               *
 * Root Macro: root -l<ENTER>, .L MLBWOProcessor.C                                *
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


//magic (for now)
const char* leps[5]  = { "E", "EE", "EM", "MM", "M" };
const char* procs[6] = { "DYJets", "WW", "^W[1234]?Jets", "QCD", "SingleTbar", "TTW?Jets" };
const char* procReplace[6] = { "DrellYan", "Diboson", "WJets", "QCD", "SingleTop", "TTbar_1" };
const char* dataprocs[5] = { "SingleElectron2012A", "DoubleElectron2012A", 
                             "MuEG2012A", "DoubleMu2012A", "SingleMu2012A" };

double eCounts[5][7];
double eErrors[5][7];

/////////////////////////////////////////////////////
//        Formatting Yields Data in LaTeX          //
/////////////////////////////////////////////////////

//LaTeX formatting for individual results
TString GetLatex(int lep, int proc) {
  // output a kickin' TString
  char b[128];
  sprintf(b, "%.0f $\\pm$ %.0f", round(eCounts[lep-1][proc]), round(eErrors[lep-1][proc]));
  return TString(b);
}

//LaTeX formatting for sums of results
TString GetLatexSum(bool rowSum, int ind) {
  double sum = 0;
  double err = 0;

  //complicated sum
  if(rowSum) {
    // if we sum via rows, go through the rows
    for(int i=0; i<5; i++) {
      sum+=eCounts[i][ind];
      err = sqrt(pow(err,2) + pow(eErrors[i][ind],2));
    }
  } else {
    // do we want to sum all MC? if ind>=0, no
    if(ind>=0) {
      for(int i=0; i<6; i++) {
        sum+=eCounts[ind][i];
        err = sqrt(pow(err,2) + pow(eErrors[ind][i],2));
      }
    } else {
      for(int i=0; i<5; i++) {
        for(int j=0; j<6; j++) {
          sum += eCounts[i][j];
          err = sqrt(pow(err,2) + pow(eErrors[i][j],2));
        }
      }
    }
  }
 
  //output a fantastic TString
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
  if(ind<6 && ind>0) {
    data = eCounts[ind-1][6];
    errData = eErrors[ind-1][6];

    for(int i=0; i<6; i++) {
      sumMC += eCounts[ind-1][i];
      errMC =  sqrt(pow(errMC,2) + pow(eErrors[ind-1][i],2));
    }
  } else {
    // if not, sum all of MC and data and take the ratios
    for(int i=0; i<5; i++) {
      data    += eCounts[i][ind];
      errData =  sqrt(pow(errData,2) + pow(eErrors[i][ind],2));
      for(int j=0; j<6; j++) {
        sumMC += eCounts[i][j];
        errMC = sqrt(pow(errMC,2) + pow(eErrors[i][j],2));
      }
    }
  }

  //output the nicest TString you've ever seen
  char b[128];
  sprintf(b, "%.3f $\\pm$ %.3f", data/sumMC, data/sumMC*sqrt(pow(errData/data,2) + pow(errMC/sumMC,2)));
  return TString(b);
}

/////////////////////////////////////////////////////
//       Formatting output histogram names         //
/////////////////////////////////////////////////////

TString formatName(const char* histoName) {
    TObjArray *nameParts = (TObjArray*) TString(histoName).ReplaceAll("_mlbwa_",'#').Tokenize(TString("#"));

    cout<<nameParts->At(0)->GetName()<<" "<<nameParts->At(1)->GetName()<<endl;
    // get the two significant parts of the name and split into relevant information
    TObjArray *procPart = (TObjArray*) TString(nameParts->At(0)->GetName()).Tokenize("_");
    TObjArray *histPart = (TObjArray*) TString(nameParts->At(1)->GetName()).Tokenize("_");

    TString proces = TString(procPart->At(1)->GetName());
    TString lepton = TString(histPart->At(0)->GetName());
    TString delmtr = TString("_");

    // if data, just say data 
    if(TString(procPart->At(0)->GetName()).Contains("Data")) proces = TString("Data");
    else {
      cout<<" - formatting "<<proces<<endl;
      for(int pInd = 0; pInd<6; pInd++) {
        if(proces.Contains(TRegexp(procs[pInd]))) {
            cout<<" -- match found, "<<procs[pInd]<<endl;
            proces = procReplace[pInd];
            cout<<" -- procs now = "<<proces<<endl;
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
  TFile *f = new TFile(argv[1]);
  f->cd();

  //very inefficient, but for now it works
  //loop over all magic and figure out the event counts
  for(int i=0; i<5; i++) {
    char a[128];
    sprintf(a, "mlbwa_%s_Count", leps[i]);
    TDirectory *tDir = (TDirectory*) f->Get(a);
    TList *alok = tDir->GetListOfKeys();

    for(int j=0; j<6; j++) {
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
    eCounts[i][6] = tth->GetEntries();
    double integral = tth->GetSumOfWeights();
    eErrors[i][6] = tth->GetBinError(2)*eCounts[i][6]/integral;
    delete tth;
  }


  //print out LaTeX:
  //formatting
  cout<<"\\documentclass[12pt,a4paper,titlepage]{article}"<<endl;
  cout<<"\\usepackage[utf8]{inputenc}"<<endl;
  cout<<"\\usepackage{amsmath}"<<endl;
  cout<<"\\usepackage{amsfonts}"<<endl;
  cout<<"\\usepackage{amssymb}"<<endl;
  cout<<"\\usepackage{hyperref}\n\n"<<endl;
  cout<<"\\usepackage[margin=0.25in]{geometry}"<<endl;
  cout<<"\\begin{document}"<<endl;
  cout<<"\\begin{tabular}{l|cccccc} \\\\"<<endl;
  cout<<"Sample & e & ee & e$\\mu$ & $\\mu\\mu$ & $\\mu$ & Sum \\\\ \\hline\\hline"<<endl;

  //accessing the array (yes, I know, there's a better way, I don't care right now)
  cout<<"Drell-Yan & "<<GetLatex(1,0)<<" & "<<GetLatex(2,0)<<" & "<<GetLatex(3,0)<<" & "
      <<GetLatex(4,0)<<" & "<<GetLatex(5,0)<<" & "<<GetLatexSum(true, 0)<<"\\\\"<<endl;
  cout<<"Diboson & "<<GetLatex(1,1)<<" & "<<GetLatex(2,1)<<" & "<<GetLatex(3,1)<<" & "
      <<GetLatex(4,1)<<" & "<<GetLatex(5,1)<<" & "<<GetLatexSum(true, 1)<<"\\\\"<<endl;
  cout<<"W+Jets & "<<GetLatex(1,2)<<" & "<<GetLatex(2,2)<<" & "<<GetLatex(3,2)<<" & "
      <<GetLatex(4,2)<<" & "<<GetLatex(5,2)<<" & "<<GetLatexSum(true, 2)<<"\\\\"<<endl;
  cout<<"QCD & "<<GetLatex(1,3)<<" & "<<GetLatex(2,3)<<" & "<<GetLatex(3,3)<<" & "
      <<GetLatex(4,3)<<" & "<<GetLatex(5,3)<<" & "<<GetLatexSum(true, 3)<<"\\\\"<<endl;
  cout<<"Single-top & "<<GetLatex(1,4)<<" & "<<GetLatex(2,4)<<" & "<<GetLatex(3,4)<<" & "
      <<GetLatex(4,4)<<" & "<<GetLatex(5,4)<<" & "<<GetLatexSum(true, 4)<<"\\\\"<<endl;
  cout<<"$t\\bar{t}$ & "<<GetLatex(1,5)<<" & "<<GetLatex(2,5)<<" & "<<GetLatex(3,5)<<" & "
      <<GetLatex(4,5)<<" & "<<GetLatex(5,5)<<" & "<<GetLatexSum(true, 5)<<"\\\\\\hline"<<endl;
  cout<<"Total MC & "<<GetLatexSum(false,0)<<" & "<<GetLatexSum(false,1)<<" & "<<GetLatexSum(false,2)<<" & "
      <<GetLatexSum(false,3)<<" & "<<GetLatexSum(false,4)<<" & "<<GetLatexSum(false,-1)<<"\\\\\\hline"<<endl;
  cout<<"Data & "<<GetLatex(1,6)<<" & "<<GetLatex(2,6)<<" & "<<GetLatex(3,6)<<" & "
      <<GetLatex(4,6)<<" & "<<GetLatex(5,6)<<" & "<<GetLatexSum(true,6)<<"\\\\\\hline\\hline"<<endl;
  cout<<"Data/MC & "<<GetLatexRatio(1)<<" & "<<GetLatexRatio(2)<<" & "<<GetLatexRatio(3)<<" & "
      <<GetLatexRatio(4)<<" & "<<GetLatexRatio(5)<<" & "<<GetLatexRatio(6)<<"\\\\"<<endl;

  //we did it! end the document
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
  TFile *f = new TFile(argv[1]);
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
 *  Structure-wise: can be implemented into class easily.
 ***********************************/
void createMFOutfile(const char* argv[]) {
  TFile *f = new TFile(argv[1]);

  //create the output file we'd like to write histograms to
  TFile *output = new TFile("2012_combined_EACMLB.root", "RECREATE");
  
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
    for(int ihisto=0; alokHistos->At(ihisto) != alokHistos->Last(); ihisto++) {
      // don't consider the graph objects
      if(TString(alokHistos->At(ihisto)->GetName()).Contains("Graph_")) continue; 

      // clone the histogram, give it a new name
      TString cloneName = formatName(alokHistos->At(ihisto)->GetName());
      TH1F *thisto = (TH1F*) cDir->Get(alokHistos->At(ihisto)->GetName());
      TH1F *tclone = (TH1F*) thisto->Clone(cloneName);

      // open the outfile and write the histo in
      output->cd();
      TList *outkeys = (TList*) output->GetListOfKeys();

      // if the histogram already exists, add thisto to the existing one
      // messy, but removes the need for magic
      for(int iout=0; outkeys->At(iout) != outkeys->Last(); iout++) {
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

    // have to work with the last (data) histogram specially because ROOT
    // doesn't create very nice iterators for directories.
    TString dataName = formatName(alokHistos->Last()->GetName());
    TH1F *thisto = (TH1F*) cDir->Get(alokHistos->Last()->GetName());
    TH1F *tclone = (TH1F*) thisto->Clone(dataName);
    output->cd();
    tclone->Write();
    f->cd();
    delete thisto;
    delete tclone;
  }

  output->cd();

  // annoying tidbit that needs to be fixed
  TH1F *mlbwa__QCD_EE = new TH1F("mlbwa_QCD_EE","mlbwa_QCD_EE",100,0,200);
  mlbwa__QCD_EE->Write();

  output->Close();
}

#ifndef __CINT__
int main(int argc, const char* argv[]) {

  cout<<"Analyzing the output: "<<argv[1]<<"\n\n"<<endl;
  
  cout<<"Here is the LaTeX for the yields table:"<<endl;
  getYields(argv);

  cout<<"\n\nHere is the KS information for the histograms:"<<endl;
  getKS(argv); 

  cout<<"\n\nLet me write the MassFit-readable file for you as well..."<<endl;
  createMFOutfile(argv);
  cout<<"...done!"<<endl; 

  return 0;
}
#endif

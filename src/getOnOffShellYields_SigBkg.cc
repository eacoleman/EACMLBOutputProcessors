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
using std::map;
using std::vector;
using std::cin;

int onCutoff = 153;
int offCutoff = 175;

vector<TString> mlbVec = { "min", "mdr", "inc" };
vector<TString> lfsVec = { "E", "EE", "EM", "MM", "M" };
vector<TString> procs  = { "DrellYan", "SingleTop-tW", "SingleTop-t", "TTbar_1.34" };
vector<TString> widVec = { "0.50", "1.00", "1.50", "2.00", "2.50", "3.00" };

TString infilebase("2012_combined_EACTLJ.root");

void getOnOffShellYields() {

    //  mlb          lfs            wid         on/off  yield
    map<TString, map<TString, map<TString, map<TString, float>>>> yields; 


    // collect all yields information so we can do nice sums eventually
    for(size_t iMlb = 0; iMlb < procs.size(); iMlb++) {
    for(size_t iLfs = 0; iLfs < lfsVec.size(); iLfs++) {
    //for(size_t iWid = 0; iWid < widVec.size(); iWid++) {
        TString cMlb = procs.at(iMlb);
        TString cLfs = lfsVec.at(iLfs);
        //TString cWid = widVec.at(iWid);

        TFile *tFile = new TFile(infilebase);
        TString histoLoc("mlbwa__"+cMlb+"_"+cLfs);

        TH1F* histo = (TH1F*) tFile->Get(histoLoc);

        cout << " - " << histoLoc << " " << histo << endl;

        int  onBin = histo->FindBin(onCutoff);
        int offBin = histo->FindBin(offCutoff);
        int maxBin = histo->GetNbinsX();

        yields[cMlb][cLfs][widVec.at(0)]["on"]
            = round(histo->Integral(0, onBin));
        yields[cMlb][cLfs][widVec.at(0)]["off"]
            = round(histo->Integral(offBin, maxBin));
    }}//}


    //print out LaTeX:
    //formatting
    cout<<"\\documentclass[12pt,a4paper,titlepage]{article}"<<endl;
    cout<<"\\usepackage[utf8]{inputenc}"<<endl;
    cout<<"\\usepackage{amsmath}"<<endl;
    cout<<"\\usepackage{amsfonts}"<<endl;
    cout<<"\\usepackage{amssymb}"<<endl;
    cout<<"\\usepackage{hyperref}"<<endl;
    cout<<"\\usepackage[margin=0.0025in]{geometry}"<<endl;
    cout<<"\\begin{document}"<<endl;
    cout<<"\\begin{tabular}{l|cccc} \\\\"<<endl;

    //Print out the table header
    cout << "Sample ";
    for(size_t iPrc = 0; iPrc < procs.size(); iPrc++) {
        cout << "& " << procs.at(iPrc) << " ";
    }
    cout << "\\\\ \\hline\\hline"<<endl;

    // loop again now that we have the yields information
    //for(size_t iWid = 0; iWid < widVec.size(); iWid++) {
        //TString cWid = widVec.at(iWid);

    for(size_t iLfs = 0; iLfs < lfsVec.size(); iLfs++) {
        TString cLfs = lfsVec.at(iLfs);

    // Get the on-shell yields
        //cout << cWid << " & ";
        cout << "On-shell " << cLfs << " & ";
    for(size_t iMlb = 0; iMlb < procs.size(); iMlb++) {
        TString cMlb = procs.at(iMlb);

        int onYield = yields[cMlb][cLfs][widVec.at(0)]["on"];

        cout << onYield << "$\\pm$ " << round(sqrt(onYield));
        
        if(iMlb == procs.size() - 1) {
            cout << "\\\\" << endl; 
        } else { cout << " & "; }
        
    }
    
    // Get the off-shell yields
        //cout << cWid << " & ";
        cout << "Off-shell " << cLfs << " & ";
    for(size_t iMlb = 0; iMlb < procs.size(); iMlb++) {
        TString cMlb = procs.at(iMlb);

        int ofYield = yields[cMlb][cLfs][widVec.at(0)]["off"];

        cout << ofYield << "$\\pm$ " << round(sqrt(ofYield));
        
        if(iMlb == procs.size() - 1) {
          cout<< "\\\\ \\hline \\hline" << endl;
        } else { cout << " & "; } 
    }
    
    }//}

    cout<<"\\end{tabular}"<<endl;
    cout<<"\\end{document}"<<endl;
}

int main(int argc, const char* argv[]) {

    getOnOffShellYields();

}

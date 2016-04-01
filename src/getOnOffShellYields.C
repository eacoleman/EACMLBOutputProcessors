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
vector<TString> widVec = { "1.33", "1.66", "2.00" };
vector<TString> prcVec = { "TTJets" };

TString infilebase("plotter.root");

void getOnOffShellYields() {

    //  mlb          lfs            wid         on/off  yield
    map<TString, map<TString, map<TString, map<TString, float>>>> yields; 


    // collect all yields information so we can do nice sums eventually
    for(size_t iMlb = 0; iMlb < mlbVec.size(); iMlb++) {
    for(size_t iLfs = 0; iLfs < lfsVec.size(); iLfs++) {
    for(size_t iWid = 0; iWid < widVec.size(); iWid++) {
        TString cMlb = mlbVec.at(iMlb);
        TString cLfs = lfsVec.at(iLfs);
        TString cWid = widVec.at(iWid);

        TFile *tFile = new TFile(infilebase);
        TString histoLoc("mlbwa_"+cLfs+"_"+cWid+"_Mlb_"+cMlb);

        TH1F* histo = (TH1F*) tFile->Get(histoLoc);
        int  onBin = histo->FindBin(onCutoff);
        int offBin = histo->FindBin(offCutoff);
        int maxBin = histo->GetNbinsX();

        yields[mlbVec.at(iMlb)][lfsVec.at(iMlb)][widVec.at(iMlb)]["on"]
            = round(histo->Integral(0, onBin));
        yields[mlbVec.at(iMlb)][lfsVec.at(iMlb)][widVec.at(iMlb)]["on"]
            = round(histo->Integral(offBin, maxBin));
    }}}


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
    cout<<"\\begin{tabular}{ll|ccc} \\\\"<<endl;

    //Print out the table header
    cout<<"Width & Sample & Minimum Mass & DR-Minimum Mass & Inclusive\\\\ \\hline\\hline"<<endl;

    // loop again now that we have the yields information
    for(size_t iWid = 0; iWid < widVec.size(); iWid++) {
        TString cWid = widVec.at(iWid);

    for(size_t iLfs = 0; iLfs < lfsVec.size(); iLfs++) {
        TString cLfs = lfsVec.at(iLfs);

    // Get the on-shell yields
        cout << cWid << " & ";
        cout << "On-shell " << cLfs << " & ";
    for(size_t iMlb = 0; iMlb < mlbVec.size(); iMlb++) {
        TString cMlb = mlbVec.at(iMlb);

        int onYield = yields[mlbVec.at(iMlb)][lfsVec.at(iMlb)][widVec.at(iMlb)]["on"];

        cout << onYield << "\\pm " << sqrt(onYield) << (iMlb == mlbVec.size() - 1 ? "\\\\n" : " & "); 
    }
    
    // Get the off-shell yields
        cout << cWid << " & ";
        cout << "Off-shell " << cLfs << " & ";
    for(size_t iMlb = 0; iMlb < mlbVec.size(); iMlb++) {
        TString cMlb = mlbVec.at(iMlb);

        int ofYield = yields[mlbVec.at(iMlb)][lfsVec.at(iMlb)][widVec.at(iMlb)]["off"];

        cout << ofYield << "\\pm " << sqrt(ofYield) << (iMlb == mlbVec.size() - 1 ? "\\\\ \\hline \\hline\n" : " & "); 
    }
    
    }}

    cout<<"\\end{tabular}"<<endl;
    cout<<"\\end{document}"<<endl;
}

int main(int argc, const char* argv[]) {

    if(argv[1]) infilebase = TString(argv[1]);
    getOnOffShellYields();

}

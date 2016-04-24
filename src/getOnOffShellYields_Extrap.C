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
using std::setfill;
using std::setw;


int onCutoff = 153;
int offCutoff = 175;

vector<TString> mlbVec = { "min", "mdr", "inc" };
vector<TString> lfsVec = { "E", "EE", "EM", "MM", "M" };
vector<TString> widVec = { "1.34", "0.50", "1.00", "1.50", "2.00", "2.50", "3.00",
                           "3.50", "4.00", "4.50", "5.00" };

TString infilebase("2012_combined_EACTLJ");

void getOnOffShellYields() {

    //  mlb          lfs            wid         on/off  yield
    map<TString, map<TString, map<TString, map<TString, float>>>> yields; 


    // collect all yields information so we can do nice sums eventually
    for(size_t iMlb = 0; iMlb < mlbVec.size(); iMlb++) {
        TString cMlb = mlbVec.at(iMlb);
        TFile *tFile = new TFile(infilebase + "_" + cMlb + ".root");
    for(size_t iLfs = 0; iLfs < lfsVec.size(); iLfs++) {
    for(size_t iWid = 0; iWid < widVec.size(); iWid++) {
        TString cLfs = lfsVec.at(iLfs);
        TString cWid = widVec.at(iWid);

        TString histoLoc("mlbwa__TTbar_"+cWid+"_"+cLfs);

        TH1F* histo = (TH1F*) tFile->Get(histoLoc);

        int  onBin = histo->FindBin(onCutoff);
        int offBin = histo->FindBin(offCutoff);
        int maxBin = histo->GetNbinsX();

        yields[cMlb][cLfs][cWid]["on"]
            = round(histo->Integral(0, onBin));
        yields[cMlb][cLfs][cWid]["off"]
            = round(histo->Integral(offBin, maxBin));
    }}}


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
    cout<<"\\centering"<<endl;

    //Print out the table header
    for(size_t iMlb = 0; iMlb < mlbVec.size(); iMlb++) {
        TString cMlb = mlbVec.at(iMlb);

        cout<<"\\begin{tabular}{ll|" << setfill('c') << setw(lfsVec.size()) << "" << "} \\\\"<<endl;

        cout << "Width (GeV) & ";
        cout << "Sample ";
        for(size_t iPrc = 0; iPrc < lfsVec.size(); iPrc++) {
            cout << "& " << lfsVec.at(iPrc) << " ";
        }
        cout << "\\\\ \\hline\\hline"<<endl;

        // loop again now that we have the yields information
        for(size_t iWid = 0; iWid < widVec.size(); iWid++) {
            TString cWid = widVec.at(iWid);

            // Get the on-shell yields
            cout << cWid << " & On-shell & ";
            for(size_t iLfs = 0; iLfs < lfsVec.size(); iLfs++) {
                TString cLfs = lfsVec.at(iLfs);

                int onYield = yields[cMlb][cLfs][cWid]["on"];

                cout << onYield << "$\\pm$ " << round(sqrt(onYield));

                if(iLfs == lfsVec.size() - 1) {
                    cout << "\\\\" << endl; 
                } else { cout << " & "; }

            }

            // Get the off-shell yields
            cout << " & Off-shell & ";
            for(size_t iLfs = 0; iLfs < lfsVec.size(); iLfs++) {
                TString cLfs = lfsVec.at(iLfs);

                int ofYield = yields[cMlb][cLfs][cWid]["off"];

                cout << ofYield << "$\\pm$ " << round(sqrt(ofYield));

                if(iLfs == lfsVec.size() - 1) {
                    cout<< "\\\\ \\hline \\hline" << endl;
                } else { cout << " & "; } 
            }

    }

        cout<<"\\end{tabular}"<<endl;
        cout<<"\\newpage"<<endl;
    }
        
    cout<<"\\end{document}"<<endl;
}

int main(int argc, const char* argv[]) {

    getOnOffShellYields();

}

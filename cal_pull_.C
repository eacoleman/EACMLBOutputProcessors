void cal_pull_()
{
//=========Macro generated from canvas: c_min/
//=========  (Wed Aug  5 19:36:10 2015) by ROOT version6.02/05
   TCanvas *c_min = new TCanvas("c_min", "",1683,405,600,600);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c_min->Range(0.66,-1.3,2.16,1.2);
   c_min->SetFillColor(0);
   c_min->SetBorderMode(0);
   c_min->SetBorderSize(2);
   c_min->SetLeftMargin(0.16);
   c_min->SetRightMargin(0.04);
   c_min->SetTopMargin(0.08);
   c_min->SetBottomMargin(0.12);
   c_min->SetFrameFillStyle(0);
   c_min->SetFrameBorderMode(0);
   c_min->SetFrameFillStyle(0);
   c_min->SetFrameBorderMode(0);
   
   Double_t pullGraph_fx1003[1] = {
   1};
   Double_t pullGraph_fy1003[1] = {
   1.399623};
   Double_t pullGraph_fex1003[1] = {
   0};
   Double_t pullGraph_fey1003[1] = {
   10.83834};
   TGraphErrors *gre = new TGraphErrors(1,pullGraph_fx1003,pullGraph_fy1003,pullGraph_fex1003,pullGraph_fey1003);
   gre->SetName("pullGraph");
   gre->SetTitle("Graph");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_pullGraph1003 = new TH1F("Graph_pullGraph1003","Graph",100,0.9,2.1);
   Graph_pullGraph1003->SetMinimum(-1);
   Graph_pullGraph1003->SetMaximum(1);
   Graph_pullGraph1003->SetDirectory(0);
   Graph_pullGraph1003->SetStats(0);
   Graph_pullGraph1003->SetLineStyle(0);
   Graph_pullGraph1003->SetMarkerStyle(20);
   Graph_pullGraph1003->GetXaxis()->SetTitle("Generated mass [GeV]");
   Graph_pullGraph1003->GetXaxis()->SetNdivisions(50205);
   Graph_pullGraph1003->GetXaxis()->SetLabelFont(42);
   Graph_pullGraph1003->GetXaxis()->SetLabelOffset(0.007);
   Graph_pullGraph1003->GetXaxis()->SetLabelSize(0.05);
   Graph_pullGraph1003->GetXaxis()->SetTitleSize(0.06);
   Graph_pullGraph1003->GetXaxis()->SetTitleOffset(0.9);
   Graph_pullGraph1003->GetXaxis()->SetTitleFont(42);
   Graph_pullGraph1003->GetYaxis()->SetTitle("pull mean [GeV]");
   Graph_pullGraph1003->GetYaxis()->SetLabelFont(42);
   Graph_pullGraph1003->GetYaxis()->SetLabelOffset(0.007);
   Graph_pullGraph1003->GetYaxis()->SetLabelSize(0.05);
   Graph_pullGraph1003->GetYaxis()->SetTitleSize(0.06);
   Graph_pullGraph1003->GetYaxis()->SetTitleOffset(1.22);
   Graph_pullGraph1003->GetYaxis()->SetTitleFont(42);
   Graph_pullGraph1003->GetZaxis()->SetLabelFont(42);
   Graph_pullGraph1003->GetZaxis()->SetLabelOffset(0.007);
   Graph_pullGraph1003->GetZaxis()->SetLabelSize(0.05);
   Graph_pullGraph1003->GetZaxis()->SetTitleSize(0.06);
   Graph_pullGraph1003->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_pullGraph1003);
   
   
   TF1 *pullFit1004 = new TF1("pullFit","pol0",0.9,2.1);
   pullFit1004->SetFillColor(19);
   pullFit1004->SetFillStyle(0);
   pullFit1004->SetMarkerStyle(20);
   pullFit1004->SetLineColor(2);
   pullFit1004->SetLineWidth(1);
   pullFit1004->GetXaxis()->SetLabelFont(42);
   pullFit1004->GetXaxis()->SetLabelOffset(0.007);
   pullFit1004->GetXaxis()->SetLabelSize(0.05);
   pullFit1004->GetXaxis()->SetTitleSize(0.06);
   pullFit1004->GetXaxis()->SetTitleOffset(0.9);
   pullFit1004->GetXaxis()->SetTitleFont(42);
   pullFit1004->GetYaxis()->SetLabelFont(42);
   pullFit1004->GetYaxis()->SetLabelOffset(0.007);
   pullFit1004->GetYaxis()->SetLabelSize(0.05);
   pullFit1004->GetYaxis()->SetTitleSize(0.06);
   pullFit1004->GetYaxis()->SetTitleOffset(1.25);
   pullFit1004->GetYaxis()->SetTitleFont(42);
   pullFit1004->SetParameter(0,1.399623);
   pullFit1004->SetParError(0,10.83834);
   pullFit1004->SetParLimits(0,0,0);
   gre->GetListOfFunctions()->Add(pullFit1004);
   gre->Draw("apz");
   
   TF1 *f31005 = new TF1("f3","pol1",-9,2);
   f31005->SetFillColor(19);
   f31005->SetFillStyle(0);
   f31005->SetMarkerStyle(20);
   f31005->SetLineColor(4);
   f31005->SetLineWidth(1);
   f31005->GetXaxis()->SetLabelFont(42);
   f31005->GetXaxis()->SetLabelOffset(0.007);
   f31005->GetXaxis()->SetLabelSize(0.05);
   f31005->GetXaxis()->SetTitleSize(0.06);
   f31005->GetXaxis()->SetTitleOffset(0.9);
   f31005->GetXaxis()->SetTitleFont(42);
   f31005->GetYaxis()->SetLabelFont(42);
   f31005->GetYaxis()->SetLabelOffset(0.007);
   f31005->GetYaxis()->SetLabelSize(0.05);
   f31005->GetYaxis()->SetTitleSize(0.06);
   f31005->GetYaxis()->SetTitleOffset(1.25);
   f31005->GetYaxis()->SetTitleFont(42);
   f31005->SetParameter(0,0);
   f31005->SetParError(0,0);
   f31005->SetParLimits(0,0,0);
   f31005->SetParameter(1,0);
   f31005->SetParError(1,0);
   f31005->SetParLimits(1,0,0);
   f31005->Draw("same");
   TLatex *   tex = new TLatex(0.96,0.936,"");
tex->SetNDC();
   tex->SetTextAlign(31);
   tex->SetTextFont(42);
   tex->SetTextSize(0.048);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.16,0.936,"CMS");
tex->SetNDC();
   tex->SetTextFont(61);
   tex->SetTextSize(0.06);
   tex->SetLineWidth(2);
   tex->Draw();
   c_min->Modified();
   c_min->cd();
   c_min->SetSelected(c_min);
}

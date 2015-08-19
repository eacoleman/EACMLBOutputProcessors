void cal_pullW_()
{
//=========Macro generated from canvas: c_min/
//=========  (Wed Aug 19 19:15:16 2015) by ROOT version6.02/05
   TCanvas *c_min = new TCanvas("c_min", "",1783,505,600,600);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c_min->Range(-0.5399999,0.675,8.46,1.3);
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
   
   Double_t pullWGraph_fx1005[5] = {
   1.5,
   3,
   4.5,
   6,
   7.5};
   Double_t pullWGraph_fy1005[5] = {
   1.643913,
   0.9920968,
   0.9730967,
   1.245343,
   0.7292887};
   Double_t pullWGraph_fex1005[5] = {
   0,
   0,
   0,
   0,
   0};
   Double_t pullWGraph_fey1005[5] = {
   0.1021724,
   0.08932956,
   0.02462952,
   0.05297478,
   0.9320205};
   TGraphErrors *gre = new TGraphErrors(5,pullWGraph_fx1005,pullWGraph_fy1005,pullWGraph_fex1005,pullWGraph_fey1005);
   gre->SetName("pullWGraph");
   gre->SetTitle("Graph");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_pullWGraph1005 = new TH1F("Graph_pullWGraph1005","Graph",100,0.9,8.1);
   Graph_pullWGraph1005->SetMinimum(0.75);
   Graph_pullWGraph1005->SetMaximum(1.25);
   Graph_pullWGraph1005->SetDirectory(0);
   Graph_pullWGraph1005->SetStats(0);
   Graph_pullWGraph1005->SetLineStyle(0);
   Graph_pullWGraph1005->SetMarkerStyle(20);
   Graph_pullWGraph1005->GetXaxis()->SetTitle("Generated mass [GeV]");
   Graph_pullWGraph1005->GetXaxis()->SetNdivisions(50205);
   Graph_pullWGraph1005->GetXaxis()->SetLabelFont(42);
   Graph_pullWGraph1005->GetXaxis()->SetLabelOffset(0.007);
   Graph_pullWGraph1005->GetXaxis()->SetLabelSize(0.05);
   Graph_pullWGraph1005->GetXaxis()->SetTitleSize(0.06);
   Graph_pullWGraph1005->GetXaxis()->SetTitleOffset(0.9);
   Graph_pullWGraph1005->GetXaxis()->SetTitleFont(42);
   Graph_pullWGraph1005->GetYaxis()->SetTitle("Pull width");
   Graph_pullWGraph1005->GetYaxis()->SetLabelFont(42);
   Graph_pullWGraph1005->GetYaxis()->SetLabelOffset(0.007);
   Graph_pullWGraph1005->GetYaxis()->SetLabelSize(0.05);
   Graph_pullWGraph1005->GetYaxis()->SetTitleSize(0.06);
   Graph_pullWGraph1005->GetYaxis()->SetTitleOffset(1.22);
   Graph_pullWGraph1005->GetYaxis()->SetTitleFont(42);
   Graph_pullWGraph1005->GetZaxis()->SetLabelFont(42);
   Graph_pullWGraph1005->GetZaxis()->SetLabelOffset(0.007);
   Graph_pullWGraph1005->GetZaxis()->SetLabelSize(0.05);
   Graph_pullWGraph1005->GetZaxis()->SetTitleSize(0.06);
   Graph_pullWGraph1005->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_pullWGraph1005);
   
   gre->Draw("apz");
   
   TF1 *f31006 = new TF1("f3","pol1",-8.5,17.5);
   f31006->SetFillColor(19);
   f31006->SetFillStyle(0);
   f31006->SetMarkerStyle(20);
   f31006->SetLineColor(4);
   f31006->SetLineWidth(1);
   f31006->SetLineStyle(2);
   f31006->GetXaxis()->SetLabelFont(42);
   f31006->GetXaxis()->SetLabelOffset(0.007);
   f31006->GetXaxis()->SetLabelSize(0.05);
   f31006->GetXaxis()->SetTitleSize(0.06);
   f31006->GetXaxis()->SetTitleOffset(0.9);
   f31006->GetXaxis()->SetTitleFont(42);
   f31006->GetYaxis()->SetLabelFont(42);
   f31006->GetYaxis()->SetLabelOffset(0.007);
   f31006->GetYaxis()->SetLabelSize(0.05);
   f31006->GetYaxis()->SetTitleSize(0.06);
   f31006->GetYaxis()->SetTitleOffset(1.25);
   f31006->GetYaxis()->SetTitleFont(42);
   f31006->SetParameter(0,1);
   f31006->SetParError(0,0);
   f31006->SetParLimits(0,0,0);
   f31006->SetParameter(1,0);
   f31006->SetParError(1,0);
   f31006->SetParLimits(1,0,0);
   f31006->Draw("same");
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

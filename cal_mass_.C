void cal_mass_()
{
//=========Macro generated from canvas: c_min/
//=========  (Wed Aug 19 19:14:57 2015) by ROOT version6.02/05
   TCanvas *c_min = new TCanvas("c_min", "",1783,505,425,598);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c_min->Range(-0.5399999,-1.485333,8.46,10.89244);
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
   
   Double_t massGraph_fx1001[5] = {
   1.5,
   3,
   4.5,
   6,
   7.5};
   Double_t massGraph_fy1001[5] = {
   0.4961544,
   3.818905,
   4.951188,
   6.749788,
   9.005304};
   Double_t massGraph_fex1001[5] = {
   0,
   0,
   0,
   0,
   0};
   Double_t massGraph_fey1001[5] = {
   0.4238443,
   0.1646091,
   0.1775682,
   0.2618714,
   0.003288113};
   TGraphErrors *gre = new TGraphErrors(5,massGraph_fx1001,massGraph_fy1001,massGraph_fex1001,massGraph_fey1001);
   gre->SetName("massGraph");
   gre->SetTitle("Graph");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_massGraph1001 = new TH1F("Graph_massGraph1001","Graph",100,0.9,8.1);
   Graph_massGraph1001->SetMinimum(0);
   Graph_massGraph1001->SetMaximum(9.90222);
   Graph_massGraph1001->SetDirectory(0);
   Graph_massGraph1001->SetStats(0);
   Graph_massGraph1001->SetLineStyle(0);
   Graph_massGraph1001->SetMarkerStyle(20);
   Graph_massGraph1001->GetXaxis()->SetTitle("Generated mass [GeV]");
   Graph_massGraph1001->GetXaxis()->SetNdivisions(50205);
   Graph_massGraph1001->GetXaxis()->SetLabelFont(42);
   Graph_massGraph1001->GetXaxis()->SetLabelOffset(0.007);
   Graph_massGraph1001->GetXaxis()->SetLabelSize(0.05);
   Graph_massGraph1001->GetXaxis()->SetTitleSize(0.06);
   Graph_massGraph1001->GetXaxis()->SetTitleOffset(0.9);
   Graph_massGraph1001->GetXaxis()->SetTitleFont(42);
   Graph_massGraph1001->GetYaxis()->SetTitle("estimated mass [GeV]");
   Graph_massGraph1001->GetYaxis()->SetLabelFont(42);
   Graph_massGraph1001->GetYaxis()->SetLabelOffset(0.007);
   Graph_massGraph1001->GetYaxis()->SetLabelSize(0.05);
   Graph_massGraph1001->GetYaxis()->SetTitleSize(0.06);
   Graph_massGraph1001->GetYaxis()->SetTitleOffset(1.22);
   Graph_massGraph1001->GetYaxis()->SetTitleFont(42);
   Graph_massGraph1001->GetZaxis()->SetLabelFont(42);
   Graph_massGraph1001->GetZaxis()->SetLabelOffset(0.007);
   Graph_massGraph1001->GetZaxis()->SetLabelSize(0.05);
   Graph_massGraph1001->GetZaxis()->SetTitleSize(0.06);
   Graph_massGraph1001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_massGraph1001);
   
   
   TF1 *meanFit1002 = new TF1("meanFit","pol1",0.9,8.1);
   meanFit1002->SetFillColor(19);
   meanFit1002->SetFillStyle(0);
   meanFit1002->SetMarkerStyle(20);
   meanFit1002->SetLineColor(2);
   meanFit1002->SetLineWidth(1);
   meanFit1002->SetChisquare(17.81005);
   meanFit1002->SetNDF(3);
   meanFit1002->GetXaxis()->SetLabelFont(42);
   meanFit1002->GetXaxis()->SetLabelOffset(0.007);
   meanFit1002->GetXaxis()->SetLabelSize(0.05);
   meanFit1002->GetXaxis()->SetTitleSize(0.06);
   meanFit1002->GetXaxis()->SetTitleOffset(0.9);
   meanFit1002->GetXaxis()->SetTitleFont(42);
   meanFit1002->GetYaxis()->SetLabelFont(42);
   meanFit1002->GetYaxis()->SetLabelOffset(0.007);
   meanFit1002->GetYaxis()->SetLabelSize(0.05);
   meanFit1002->GetYaxis()->SetTitleSize(0.06);
   meanFit1002->GetYaxis()->SetTitleOffset(1.25);
   meanFit1002->GetYaxis()->SetTitleFont(42);
   meanFit1002->SetParameter(0,-0.3585265);
   meanFit1002->SetParError(0,0.2108083);
   meanFit1002->SetParLimits(0,0,0);
   meanFit1002->SetParameter(1,1.248503);
   meanFit1002->SetParError(1,0.02811718);
   meanFit1002->SetParLimits(1,0,0);
   gre->GetListOfFunctions()->Add(meanFit1002);
   gre->Draw("apz");
   
   TF1 *f21003 = new TF1("f2","pol1",-8.5,17.5);
   f21003->SetFillColor(19);
   f21003->SetFillStyle(0);
   f21003->SetMarkerStyle(20);
   f21003->SetLineColor(4);
   f21003->SetLineWidth(1);
   f21003->GetXaxis()->SetLabelFont(42);
   f21003->GetXaxis()->SetLabelOffset(0.007);
   f21003->GetXaxis()->SetLabelSize(0.05);
   f21003->GetXaxis()->SetTitleSize(0.06);
   f21003->GetXaxis()->SetTitleOffset(0.9);
   f21003->GetXaxis()->SetTitleFont(42);
   f21003->GetYaxis()->SetLabelFont(42);
   f21003->GetYaxis()->SetLabelOffset(0.007);
   f21003->GetYaxis()->SetLabelSize(0.05);
   f21003->GetYaxis()->SetTitleSize(0.06);
   f21003->GetYaxis()->SetTitleOffset(1.25);
   f21003->GetYaxis()->SetTitleFont(42);
   f21003->SetParameter(0,0);
   f21003->SetParError(0,0);
   f21003->SetParLimits(0,0,0);
   f21003->SetParameter(1,1);
   f21003->SetParError(1,0);
   f21003->SetParLimits(1,0,0);
   f21003->Draw("same");
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

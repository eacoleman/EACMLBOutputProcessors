void LL()
{
//=========Macro generated from canvas: c_min/
//=========  (Tue Aug  4 11:08:17 2015) by ROOT version6.02/05
   TCanvas *c_min = new TCanvas("c_min", "",1683,405,600,600);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c_min->Range(0,0,1,1);
   c_min->SetFillColor(0);
   c_min->SetBorderMode(0);
   c_min->SetBorderSize(2);
   c_min->SetLeftMargin(0.16);
   c_min->SetRightMargin(0.04);
   c_min->SetTopMargin(0.08);
   c_min->SetBottomMargin(0.12);
   c_min->SetFrameFillStyle(0);
   c_min->SetFrameBorderMode(0);
   
   Double_t gr_fx1001[1] = {
   1};
   Double_t gr_fy1001[1] = {
   0};
   Double_t gr_fex1001[1] = {
   0};
   Double_t gr_fey1001[1] = {
   9.4};
   TGraphErrors *gre = new TGraphErrors(1,gr_fx1001,gr_fy1001,gr_fex1001,gr_fey1001);
   gre->SetName("gr");
   gre->SetTitle("Graph");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gr1001 = new TH1F("Graph_gr1001","Graph",100,0.9,2.1);
   Graph_gr1001->SetMinimum(-11.28);
   Graph_gr1001->SetMaximum(11.28);
   Graph_gr1001->SetDirectory(0);
   Graph_gr1001->SetStats(0);
   Graph_gr1001->SetLineStyle(0);
   Graph_gr1001->SetMarkerStyle(20);
   Graph_gr1001->GetXaxis()->SetTitle("mlbwa  m(lb) [GeV]");
   Graph_gr1001->GetXaxis()->SetLabelFont(42);
   Graph_gr1001->GetXaxis()->SetLabelOffset(0.007);
   Graph_gr1001->GetXaxis()->SetLabelSize(0.05);
   Graph_gr1001->GetXaxis()->SetTitleSize(0.06);
   Graph_gr1001->GetXaxis()->SetTitleOffset(0.9);
   Graph_gr1001->GetXaxis()->SetTitleFont(42);
   Graph_gr1001->GetYaxis()->SetTitle("-log (L/L_{max})");
   Graph_gr1001->GetYaxis()->SetLabelFont(42);
   Graph_gr1001->GetYaxis()->SetLabelOffset(0.007);
   Graph_gr1001->GetYaxis()->SetLabelSize(0.05);
   Graph_gr1001->GetYaxis()->SetTitleSize(0.06);
   Graph_gr1001->GetYaxis()->SetTitleOffset(1.25);
   Graph_gr1001->GetYaxis()->SetTitleFont(42);
   Graph_gr1001->GetZaxis()->SetLabelFont(42);
   Graph_gr1001->GetZaxis()->SetLabelOffset(0.007);
   Graph_gr1001->GetZaxis()->SetLabelSize(0.05);
   Graph_gr1001->GetZaxis()->SetTitleSize(0.06);
   Graph_gr1001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gr1001);
   
   
   TF1 *pol21002 = new TF1("pol2","pol2",0.9,2.1);
   pol21002->SetFillColor(19);
   pol21002->SetFillStyle(0);
   pol21002->SetMarkerStyle(20);
   pol21002->SetLineColor(2);
   pol21002->SetLineWidth(1);
   pol21002->GetXaxis()->SetLabelFont(42);
   pol21002->GetXaxis()->SetLabelOffset(0.007);
   pol21002->GetXaxis()->SetLabelSize(0.05);
   pol21002->GetXaxis()->SetTitleSize(0.06);
   pol21002->GetXaxis()->SetTitleOffset(0.9);
   pol21002->GetXaxis()->SetTitleFont(42);
   pol21002->GetYaxis()->SetLabelFont(42);
   pol21002->GetYaxis()->SetLabelOffset(0.007);
   pol21002->GetYaxis()->SetLabelSize(0.05);
   pol21002->GetYaxis()->SetTitleSize(0.06);
   pol21002->GetYaxis()->SetTitleOffset(1.25);
   pol21002->GetYaxis()->SetTitleFont(42);
   pol21002->SetParameter(0,1);
   pol21002->SetParError(0,0);
   pol21002->SetParLimits(0,0,0);
   pol21002->SetParameter(1,1);
   pol21002->SetParError(1,0);
   pol21002->SetParLimits(1,0,0);
   pol21002->SetParameter(2,1);
   pol21002->SetParError(2,0);
   pol21002->SetParLimits(2,0,0);
   gre->GetListOfFunctions()->Add(pol21002);
   gre->Draw("apz");
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
      tex = new TLatex(0.167,0.995,"CMS Preliminary, 19.7 fb^{-1} at #sqrt{s} = 8 TeV");
tex->SetNDC();
   tex->SetTextAlign(13);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04195804);
   tex->SetLineWidth(2);
   tex->Draw();
   c_min->Modified();
   c_min->cd();
   c_min->SetSelected(c_min);
}

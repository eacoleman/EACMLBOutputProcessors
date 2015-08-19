void cal_bias_()
{
//=========Macro generated from canvas: c_min/
//=========  (Wed Aug 19 10:50:08 2015) by ROOT version6.02/05
   TCanvas *c_min = new TCanvas("c_min", "",1758,480,600,600);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c_min->Range(-0.5399999,-0.65,8.46,0.6);
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
   
   Double_t biasGraph_fx1006[5] = {
   1.5,
   3,
   4.5,
   6,
   7.5};
   Double_t biasGraph_fy1006[5] = {
   -0.917234,
   0.8240947,
   0.3999668,
   0.7185501,
   1.58136};
   Double_t biasGraph_fex1006[5] = {
   0,
   0,
   0,
   0,
   0};
   Double_t biasGraph_fey1006[5] = {
   0.6928592,
   0.152635,
   0.1499794,
   0.2585853,
   0.04167287};
   TGraphErrors *gre = new TGraphErrors(5,biasGraph_fx1006,biasGraph_fy1006,biasGraph_fex1006,biasGraph_fey1006);
   gre->SetName("biasGraph");
   gre->SetTitle("Graph");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_biasGraph1006 = new TH1F("Graph_biasGraph1006","Graph",100,0.9,8.1);
   Graph_biasGraph1006->SetMinimum(-0.5);
   Graph_biasGraph1006->SetMaximum(0.5);
   Graph_biasGraph1006->SetDirectory(0);
   Graph_biasGraph1006->SetStats(0);
   Graph_biasGraph1006->SetLineStyle(0);
   Graph_biasGraph1006->SetMarkerStyle(20);
   Graph_biasGraph1006->GetXaxis()->SetTitle("Generated mass [GeV]");
   Graph_biasGraph1006->GetXaxis()->SetNdivisions(50205);
   Graph_biasGraph1006->GetXaxis()->SetLabelFont(42);
   Graph_biasGraph1006->GetXaxis()->SetLabelOffset(0.007);
   Graph_biasGraph1006->GetXaxis()->SetLabelSize(0.05);
   Graph_biasGraph1006->GetXaxis()->SetTitleSize(0.06);
   Graph_biasGraph1006->GetXaxis()->SetTitleOffset(0.9);
   Graph_biasGraph1006->GetXaxis()->SetTitleFont(42);
   Graph_biasGraph1006->GetYaxis()->SetTitle("Bias [GeV]");
   Graph_biasGraph1006->GetYaxis()->SetLabelFont(42);
   Graph_biasGraph1006->GetYaxis()->SetLabelOffset(0.007);
   Graph_biasGraph1006->GetYaxis()->SetLabelSize(0.05);
   Graph_biasGraph1006->GetYaxis()->SetTitleSize(0.06);
   Graph_biasGraph1006->GetYaxis()->SetTitleOffset(1.22);
   Graph_biasGraph1006->GetYaxis()->SetTitleFont(42);
   Graph_biasGraph1006->GetZaxis()->SetLabelFont(42);
   Graph_biasGraph1006->GetZaxis()->SetLabelOffset(0.007);
   Graph_biasGraph1006->GetZaxis()->SetLabelSize(0.05);
   Graph_biasGraph1006->GetZaxis()->SetTitleSize(0.06);
   Graph_biasGraph1006->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_biasGraph1006);
   
   
   TF1 *biasFit1007 = new TF1("biasFit","pol1",0.9,8.1);
   biasFit1007->SetFillColor(19);
   biasFit1007->SetFillStyle(0);
   biasFit1007->SetMarkerStyle(20);
   biasFit1007->SetLineColor(2);
   biasFit1007->SetLineWidth(1);
   biasFit1007->SetChisquare(19.46576);
   biasFit1007->SetNDF(3);
   biasFit1007->GetXaxis()->SetLabelFont(42);
   biasFit1007->GetXaxis()->SetLabelOffset(0.007);
   biasFit1007->GetXaxis()->SetLabelSize(0.05);
   biasFit1007->GetXaxis()->SetTitleSize(0.06);
   biasFit1007->GetXaxis()->SetTitleOffset(0.9);
   biasFit1007->GetXaxis()->SetTitleFont(42);
   biasFit1007->GetYaxis()->SetLabelFont(42);
   biasFit1007->GetYaxis()->SetLabelOffset(0.007);
   biasFit1007->GetYaxis()->SetLabelSize(0.05);
   biasFit1007->GetYaxis()->SetTitleSize(0.06);
   biasFit1007->GetYaxis()->SetTitleOffset(1.25);
   biasFit1007->GetYaxis()->SetTitleFont(42);
   biasFit1007->SetParameter(0,-0.330888);
   biasFit1007->SetParError(0,0.2059615);
   biasFit1007->SetParLimits(0,0,0);
   biasFit1007->SetParameter(1,0.2526513);
   biasFit1007->SetParError(1,0.02903884);
   biasFit1007->SetParLimits(1,0,0);
   gre->GetListOfFunctions()->Add(biasFit1007);
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
   c_min->Modified();
   c_min->cd();
   c_min->SetSelected(c_min);
}

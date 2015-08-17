{
    const char* leps[5] = { "E", "EE", "EM", "MM", "M" };
    const float wts[3] = { 3.00, 4.50, 6.00 };

    TFile *f = new TFile("2012_combined_EACMLB.root", "UPDATE");
    f->cd();

    for(int i=0;i<5; i++) {
        char b[128];
        sprintf(b,"mlbwa__TTbar_7.50_%s",leps[i]);
        TH1F *h = (TH1F*) f->Get(TString(b));

        for(int j=0; j<3; j++ ) {
            char c[128];
            sprintf(c, "mlbwa__TTbar_%.2f_%s", wts[j], leps[i]);
            f->Delete(TString(c) + TString(";1"));

            TH1F *th = (TH1F*) h->Clone(TString(c));
            th->Write();
                
        }
    }

}

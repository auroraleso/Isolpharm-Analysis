void gain(std::string filename)
{

    TFile * file = new TFile(filename.c_str());

    TH1F *h;
    TKey *key;
    TIter next( file->GetListOfKeys());

    TH2F * gain_d = new TH2F("","", 50, 0, 16, 16384, 2.567398, 16384*1.410091+2.567398);

    TCanvas * c = new TCanvas();
    c->SetLogz();

    int j = 0;

    std::string bkgname = "bkg";

    while( (key = (TKey*) next() ) )
    {
        h = (TH1F*) file->Get(key->GetName());

        if(!bkgname.compare(std::string(h->GetName())) ) continue;

        for (size_t i = 0; i < 16384; i++)
        {
            gain_d->SetBinContent(j, i, h->GetBinContent(i));
        }

        j++;
    }

    cout << j << endl;

    gain_d->SetStats(kFALSE);
    gain_d->GetYaxis()->SetRangeUser(0, 2200);

    gain_d->GetXaxis()->SetTitle("Time [hours]");
    gain_d->GetYaxis()->SetTitle("Energy [keV]");
    gain_d->Draw("COLZ");
    // gain_d->Draw("CONT1 SAME");

    return;
}

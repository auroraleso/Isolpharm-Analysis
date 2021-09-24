void read_spectra(std::string filename)
{
    // Opening the TFile
    TFile * file = new TFile(filename.c_str());

    // Create a TIterator
    TH1F *h;
    TKey *key;
    TIter next( file->GetListOfKeys());

    // Setting Log y on the TCanvas
    TCanvas * c = new TCanvas();


    std::vector<double> time;
    std::vector<double> rate;

    // Loop over histograms in TFile
    TH1F * hhh;

    bool flag = 1;

    while( (key = (TKey*) next() ) )
    {
        h = (TH1F*) file->Get(key->GetName());

        if(flag)
        {
          flag = 0;
          hhh = (TH1F*) h->Clone();
          continue;
        }

        hhh->Add(hhh,h,1,1);
    }

    hhh->Draw("HIST");


    return;
}

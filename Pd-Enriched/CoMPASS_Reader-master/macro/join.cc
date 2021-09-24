void read_spectra(std::string filename1, std::string filename2, std::string filename3)
{
    // Opening the TFile
    TFile * file1 = new TFile(filename1.c_str());
    TFile * file2 = new TFile(filename2.c_str());
    TFile * file3 = new TFile(filename3.c_str());
    // TFile * file4 = new TFile(filename4.c_str());
    // Create a TIterator
    TH1F *h;
    TKey *key;
    TIter next1( file1->GetListOfKeys());

    // Setting Log y on the TCanvas
    TCanvas * c = new TCanvas();
    c->SetLogy();

    TFile * file_joined = new TFile("joined.root","RECREATE");

    // Loop over histograms in TFile
    while( (key = (TKey*) next1() ) )
    {
        h = (TH1F*) file1->Get(key->GetName());
        if(  (strcmp(h->GetName(),"bkg") == 0) ) continue;

        h->Write();
    }

    TIter next2( file2->GetListOfKeys());
    while( (key = (TKey*) next2() ) )
    {
        h = (TH1F*) file2->Get(key->GetName());
        if(  (strcmp(h->GetName(),"bkg") == 0) ) continue;
        h->Write();
    }

    TIter next3( file3->GetListOfKeys());
    while( (key = (TKey*) next3() ) )
    {
        h = (TH1F*) file3->Get(key->GetName());
        if(  (strcmp(h->GetName(),"bkg") == 0) ) continue;
        h->Write();
    }

    // TIter next4( file4->GetListOfKeys());
    // while( (key = (TKey*) next4() ) )
    // {
    //     h = (TH1F*) file4->Get(key->GetName());
    //     if(  (strcmp(h->GetName(),"bkg") == 0) ) continue;
    //     h->Write();
    // }

    file_joined->Write();
    file_joined->Close();


    return;
}

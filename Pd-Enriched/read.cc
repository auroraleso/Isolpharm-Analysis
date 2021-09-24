/*
    Reading COMPASS ROOT file example
*/

void read(std::string filename)
{
    // Open ROOT file
    TFile * input_file = new TFile(filename.c_str());

    // TTreeReader
    TTreeReader tree_reader("Data_R", input_file);

    TTreeReaderValue<unsigned short> channel(tree_reader, "Channel");
    TTreeReaderValue<unsigned long long> timestamp(tree_reader, "Timestamp");
    TTreeReaderValue<unsigned short> energy(tree_reader, "Energy");
    TTreeReaderValue<unsigned int> flags(tree_reader, "Flags");

    // TH1F * h_raw    = new TH1F("h_raw","raw spectra",16384,0,16384);
    TH1F * h_filtered = new TH1F("h_filtered","filtered spectra",16384,0,16384);

    while(tree_reader.Next())
    {
        // h_raw->Fill(*energy);

        // Reject pileup & saturated events
        if( (*flags & 0xF000) == 0x8000 or (*flags & 0xF00) == 0x400 or (*flags & 0xF00) == 0x80)
        {
            continue;
        }
        else
        {
            h_filtered->Fill(*energy);
        }

    }

    h_filtered->Draw();


    return ;

}

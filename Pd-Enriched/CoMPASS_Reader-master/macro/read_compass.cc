struct CompassEvent
{
	UShort_t channel;
	ULong_t  timestamp;
	UShort_t board;
	UShort_t energy;
	Int_t flags;
};

void read(std::string filename_data, std::string filename_bkg)
{

	TFile * data_file = new TFile(filename_data.c_str());
	TFile * bkg_file  = new TFile(filename_bkg.c_str());

	TTreeReader data_reader("Data_R", data_file);
	TTreeReader bkg_reader("Data_R", bkg_file);

	TTreeReaderValue<UShort_t> data_energy(data_reader,"energy");
	TTreeReaderValue<ULong_t>  data_timestamp(data_reader,"timestamp");

	TTreeReaderValue<UShort_t> bkg_energy(bkg_reader,"energy");
	TTreeReaderValue<ULong_t>  bkg_timestamp(bkg_reader,"timestamp");

	// Energy Spectrum
	const int nBins = 16384;
	TH1F * h_spectra = new TH1F("h_energy", filename_data.c_str(), nBins,0,16384);
	TH1F * h_bkg     = new TH1F("h_bkg", filename_bkg.c_str(), nBins,0,16384);

	// Filling energy spectrum
	double m = 1.410091;
	double q = 2.567398;

	// Fill Source Spectrum

	while (data_reader.Next())
	{
		h_spectra->Fill( ( (*data_energy) + gRandom->Rndm() )*m+q );
	}

	// Fill Background Spectrum
	while (bkg_reader.Next())
	{
		h_bkg->Fill( ( (*bkg_energy) + gRandom->Rndm() )*m+q );
	}



	h_spectra->SetStats(kFALSE);
	h_spectra->Draw();



	return ;
}

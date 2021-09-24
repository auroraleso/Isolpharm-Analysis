struct CompassEvent
{
	UShort_t channel;
	ULong_t  timestamp;
	UShort_t board;
	UShort_t energy;
	Int_t flags;
};

void read(std::string filename)
{
	// Open ROOT
	TFile * in_file = new TFile(filename.c_str());

	// Get Data_R TTree
	TTree * in_tree = (TTree*) in_file->Get("Data_R");

	// Linking to TBranches
	CompassEvent event;

	TBranch * channel_branch   = in_tree->GetBranch("Channel");
	channel_branch->SetAddress(&event.channel);

	TBranch * timestamp_branch = in_tree->GetBranch("Timestamp");
	timestamp_branch->SetAddress(&event.timestamp);

	TBranch * board_branch     = in_tree->GetBranch("Board");
	board_branch->SetAddress(&event.board);

	TBranch * energy_branch    = in_tree->GetBranch("Energy");
	energy_branch->SetAddress(&event.energy);

	TBranch * flags_branch     = in_tree->GetBranch("Flags");
	flags_branch->SetAddress(&event.flags);

	// Get total number of events
	ULong_t nEntries = in_tree->GetEntries();

	// Energy Spectrum
	const int nBins = 16384;
	TH1F * h_spectra = new TH1F("h_energy", filename.c_str(), nBins,0,16384);


	int dt_events              = 0.0;
	int pileup_events          = 0.0;
	int saturation_events      = 0.0;
	int time_filter_not_passed = 0.0;
	int Ntriggers              = 0.0;
	float OCR                  = 0.0;
	float ICR;

	timestamp_branch->GetEntry(0);
	long int start = event.timestamp;
	timestamp_branch->GetEntry(nEntries-1);
	long int stop  = event.timestamp;


	for (size_t i = 0; i < nEntries; i++)
	{
		energy_branch->GetEntry(i);
		h_spectra->Fill(event.energy);
	}
	//
	// // DT-5725
	// TF1 * gausfit = new TF1("gausfit","gaus(0)",450,520);
	// TF1 * fitfunc = new TF1("fitfunc","gaus(0)+pol1(3)",450,520);

	// DT-5780
	TF1 * gausfit = new TF1("gausfit","gaus(0)",5500,7000);
	TF1 * fitfunc = new TF1("fitfunc","gaus(0)+pol1(3)",5500,7000);

	h_spectra->Fit("gausfit","R");

	fitfunc->SetParameter(0, gausfit->GetParameter(0));
	fitfunc->SetParameter(1, gausfit->GetParameter(1));
	fitfunc->SetParameter(2, gausfit->GetParameter(2));

	h_spectra->Fit("fitfunc","R");

	float mean  = fitfunc->GetParameter(1);
	float sigma = fitfunc->GetParameter(2);

	float ene_min = mean-3*sigma;
	float ene_max = mean+3*sigma;


	h_spectra->Draw();

	timestamp_branch->GetEntry(0);
	long int current_ts = event.timestamp;

	TH1F * timedistribution = new TH1F("td","", 5e3, 0,1e3);

	for (size_t i = 1; i < nEntries; i++)
	{
		timestamp_branch->GetEntry(i);
		flags_branch->GetEntry(i);

		// std::cout << std::hex << (event.flags) << " " << 0x8000 << std::endl;
		if( (event.flags & 0xF000) != 0x8000)
		{
			timedistribution->Fill( (event.timestamp - current_ts) *1e-6);
			current_ts = event.timestamp;
		}


	}

	TF1 * f1 = new TF1("e","expo",100,300);
	timedistribution->Fit("e","R");
	timedistribution->Draw();
	double true_rate = abs(f1->GetParameter(1))*1e6;



	for (int i = 0; i < nEntries; ++i)
	{
		// Retrieve Information
		energy_branch->GetEntry(i);
		flags_branch->GetEntry(i);

		// std::cout << "--> " << std::hex << event.flags<< std::endl;

		// if(event.energy > ene_max or event.energy < ene_min) continue;

		// 0x0 Good event
		// 0x80 Saturating at the gate - clipping

		// 1st Bit Position

		// if( event.flags == 0)
		// {
		// 	OCR+=1;
		// }

		if( (event.flags & 0xF) == 0x1)
		{
				dt_events++;
		}
		// 2nd Bit Position
		else if( (event.flags & 0xF0) == 0x80)
		{
				saturation_events++;
		}
		// 3rd Bit Position
		else if( (event.flags & 0xF00) == 0x400)
		{
				saturation_events++;

		// 6th Bit Positionuration_events++;
		}
		// 4th Bit Position
		else if( (event.flags & 0xF000) == 0x1000)
		{
				time_filter_not_passed++;
		}
		else if( (event.flags & 0xF000) == 0x8000)
		{
				pileup_events++;
		}
		// DT-5780
		else if( event.flags == 0)
		{
			OCR++;
		}
		// // DT-5725
		// else if( (event.flags&0xF000) == 0x4000)
		// {
		// 	OCR++;
		// }

	}


	long int rate_ev = 0;
	for (int i = 0; i < nEntries; ++i)
	{
		// Retrieve Information
		energy_branch->GetEntry(i);
		flags_branch->GetEntry(i);

		if(event.energy > ene_max or event.energy < ene_min) continue;

		// // DT5725
		// if( (event.flags & 0xF000)  == 0x4000)
		// {
		// 	rate_ev++;
		// }
		// DT5780
		if( event.flags == 0)
		{
			rate_ev++;
		}
	}

	ICR = OCR + pileup_events + saturation_events + time_filter_not_passed;

	double dt = 1.0 - (OCR+saturation_events)/ICR;
	std::cout << std::setprecision(20) << "pu " << pileup_events;
	std::cout << " , time " << time_filter_not_passed;
	std::cout << " , sat " <<  saturation_events;
	std::cout << " , trig " <<  Ntriggers;
	std::cout << " , ocr " << OCR;
	std::cout << " , icr " << ICR;
	std::cout << " , dt " <<  dt;
	std::cout << " , rate " <<  (rate_ev)/((stop-start)*1E-12) << std::endl;

	std::cout << "Entries : " << nEntries << std::endl;
	std::cout << "Time : " << ((stop-start)*1E-12) << std::endl;
	// h_spectra->Draw();

	return ;
}

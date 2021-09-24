#include "CompassROOT.hh"

CompassROOT::CompassROOT(std::string filename)
{
    // Open ROOT file
    f = new TFile(filename.c_str());

    // Get Data_R TTree
    tree = (TTree*) f->Get("Data_R");

    // Linking to TBranches

    channel_branch   = tree->GetBranch("Channel");
	channel_branch->SetAddress(&channel);

	timestamp_branch = tree->GetBranch("Timestamp");
	timestamp_branch->SetAddress(&timestamp);

	board_branch     = tree->GetBranch("Board");
	board_branch->SetAddress(&board);

	energy_branch    = tree->GetBranch("Energy");
	energy_branch->SetAddress(&energy);

	flags_branch     = tree->GetBranch("Flags");
	flags_branch->SetAddress(&flags);

    // Retrieve the total number of events
    nEntries = tree->GetEntries();

    // Retrieve the first timestamp
    timestamp_branch->GetEntry(0);
    first_timestamp = timestamp;

    // Retrieve the last timestamp
    timestamp_branch->GetEntry(nEntries-1);
    last_timestamp = timestamp;

}

CompassEvent CompassROOT::GetEvent(Int_t i)
{
    // Syncing Branch to entry i-th
    channel_branch->GetEntry(i);
    timestamp_branch->GetEntry(i);
    board_branch->GetEntry(i);
    energy_branch->GetEntry(i);
    flags_branch->GetEntry(i);

    // Building a CoMPASS Event

    CompassEvent e;

    e.channel   = channel;
    e.timestamp = timestamp;
    e.board     = board;
    e.energy    = energy;
    e.flags     = flags;

    return e;
}


CompassROOT::~CompassROOT()
{
    delete f;
    delete tree;

    delete channel_branch;
    delete timestamp_branch;
    delete board_branch;
    delete energy_branch;
    delete flags_branch;
}

#ifndef COMPASS_ROOT_H
#define COMPASS_ROOT_H

// STL Includes
#include <string>

// ROOT Includes
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

#include "CompassEvent.hh"


class CompassROOT
{
    public:

        // Constructor
        CompassROOT(std::string );
        // Destructor
        ~CompassROOT();

        ULong_t GetEntries(){ return nEntries; };
        ULong_t GetFirstTimestamp(){ return first_timestamp; };
        ULong_t GetAcquisitionLength(){ return (last_timestamp - first_timestamp); };
        
        CompassEvent GetEvent(Int_t);
    private:

        // TFile
        TFile * f;

        // TTree
        TTree * tree;

        // TBranches
        TBranch * channel_branch;
        TBranch * timestamp_branch;
        TBranch * board_branch;
        TBranch * energy_branch;
        TBranch * flags_branch;

        // TTree variables
        UShort_t channel;
        ULong_t  timestamp;
        UShort_t board;
        UShort_t energy;
        Int_t    flags;

        ULong_t first_timestamp;
        ULong_t last_timestamp;
        ULong_t nEntries;

};

#endif

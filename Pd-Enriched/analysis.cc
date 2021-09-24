/*
--------------------------------------------------------------------------------
                    Spectroscopy Data Reader - CoMPASS Format
--------------------------------------------------------------------------------

> Author: Luca Morselli

> Laboratori Nazionali di Legnaro - Istituto Nazionale di Fisica Nucleare
> Università degli studi di Ferrara - Dipartimento di Fisica e Scienze della Terra

Different Operation Modes:

1) Split data in evenly time-spaced energy spectra

2) Extracting energy spectum at a given acquisition time of custom length

3) Offline Coincidence between two channels

--------------------------------------------------------------------------------
*/

// STL Includes
#include <ctime>
#include <chrono>
#include <string>
// ROOT Includes
#include "TApplication.h"
#include "TSpectrum.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TBranch.h"
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TGraph.h"
#include "Rtypes.h"
#include "TCanvas.h"

// Custom Includes
#include "Utils.hh"
#include "Buffer.hh"
#include "CompassROOT.hh"
#include "CompassEvent.hh"
#include "RunInformation.hh"

void Analysis(int argc, char **argv)
{
    // User defined acquisition time window
    time_t time_start_user = 0;
    time_t time_stop_user  = 0;

    // Acquisition time window
    time_t time_start_daq;
    time_t time_stop_daq;

    std::string time_start_UTC;
    std::string time_stop_UTC;

    // Acquisition ROOT filename
    std::string root_filename;
    std::string data_name;

    // Background ROOT filename
    std::string background_filename;

    // Calibration Parameters
    float m = 1.0;
    float q = 0.0;

    bool scan_flag = false;
    int time_window = 1; // Default time window is 1 hour.

    bool verbosity = false;

    // Parser flag
    int c;

    const char* const short_opts = "Hs:e:i:S:N:f:vm:q:b:";
    const option long_opts[] = {
            {"Help", no_argument, nullptr, 'H'},
            {"start", required_argument, nullptr, 's'},
            {"end", required_argument, nullptr, 'e'},
            {"info", required_argument, nullptr, 'i'},
            {"scan", required_argument, nullptr, 'S'},
            {"seconds", required_argument, nullptr, 'N'},
            {"file", required_argument, nullptr, 'f'},
            {"verbose", required_argument, nullptr, 'v'},
            {"slope", required_argument, nullptr, 'm'},
            {"constant", required_argument, nullptr, 'q'},
            {"background", required_argument, nullptr, 'b'},
            {nullptr, no_argument, nullptr, 0}
    };

    while ( ( c = getopt_long(argc, argv, short_opts, long_opts, nullptr) ) != -1)
    {
        switch (c)
        {
            case 'H':

                print_usage("CoMPASS Data Reader.");
                return ;

            case 's':

                time_start_user = to_unix_timestamp(optarg);
                break;

            case 'e':

                time_stop_user = to_unix_timestamp(optarg);
                break;

            case 'f':
                root_filename = optarg;
                break;

            case 'i':
            {
                RunInformation ri;
                ri = GetRunInformation(optarg);

                data_name = ri.name;

                time_start_daq = ri.start;
                time_start_UTC = ri.time_start_UTC;

                time_stop_daq  = ri.stop;
                time_stop_UTC = ri.time_stop_UTC;

                break;
            }

            case 'N':

                time_stop_user = time_start_user + std::stoi(optarg);
                break;

            case 'S':

                scan_flag = true;
                time_window = std::stod(optarg);
                break;

            case 'v':

                verbosity = true;
                break;

            case 'm':

                m = std::stof(optarg);
                break;

            case 'q':

                q = std::stof(optarg);
                break;

            case 'b':
                background_filename = optarg;
                break;

            default:

                print_usage("CoMPASS Data Reader.");
                return ;

        }
    }

    // Check for input file
    if(root_filename == "" or background_filename == "")
    {
        std::cout << "No All Input File Given. Routine closed with error." << std::endl;
        return ;
    }

    std::cout << R"(
       ____      __  __ ____   _    ____ ____    ____                _
      / ___|___ |  \/  |  _ \ / \  / ___/ ___|  |  _ \ ___  __ _  __| | ___ _ __
     | |   / _ \| |\/| | |_) / _ \ \___ \___ \  | |_) / _ \/ _` |/ _` |/ _ | '__|
     | |__| (_) | |  | |  __/ ___ \ ___) ___) | |  _ |  __| (_| | (_| |  __| |
      \____\___/|_|  |_|_| /_/   \_|____|____/  |_| \_\___|\__,_|\__,_|\___|_|

                )" << std::endl;

    // Print Selected Operation Mode

    if( scan_flag == true )
    {
        std::cout << std::endl;
        std::cout << "CoMPASS Reader in scanning mode";
        std::cout << std::endl << std::endl;
    }

    // Print Run Information
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "Analyzing " << data_name << " file" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "Acquisition Start Date: " << time_start_UTC << std::endl;
    std::cout << "Acquisition Stop Date: " << time_stop_UTC << std::endl;

    if(time_start_user != 0)
    {
        std::cout << "User Start Date: " << from_unix_to_date(time_start_user) << std::endl;
        std::cout << "User Stop Date: " << from_unix_to_date(time_stop_user) << std::endl;
    }

    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    // CH0 - File
    CompassROOT * ch0_data = new CompassROOT(root_filename);
    CompassEvent event;
    ULong_t nEntries = ch0_data->GetEntries();

    // CH0 - Background
    CompassROOT * ch0_background = new CompassROOT(background_filename);
    ULong_t nEntries_bkg = ch0_background->GetEntries();

    float bkg_scaling_factor = (time_window*1E12)/ch0_background->GetAcquisitionLength();

    const int nBins = 16384;
    TH1F * h_bkg = new TH1F("bkg","", nBins, 0, nBins);

    // Retrieve Histogram Spectrum
    std::cout << "Reading background . . ." << std::endl;

    for (size_t i = 0; i < nEntries_bkg; i++)
    {
        event = ch0_background->GetEvent(i);
        h_bkg->Fill((event.energy + gRandom->Rndm()));
    }

    // Calibrating the X-axis
    h_bkg->GetXaxis()->Set(nBins, q, m*nBins+q);

    TH1F * h = new TH1F("h","", nBins, 0, nBins);
    h->GetXaxis()->SetRangeUser(0,1000);

    if(scan_flag)
    {
        std::cout << "Scanning file . . ." << std::endl;

        // Acquisition Buffer
        Buffer buffer(time_window, ch0_data->GetFirstTimestamp());

        // Setting Calibration Parameters
        buffer.SetCalibrationM(m);
        buffer.SetCalibrationQ(q);

        // Setting Background Histogram
        buffer.SetBkgHistogram(h_bkg, bkg_scaling_factor);

        for (size_t i = 0; i < nEntries; i++)
        {
            // Retrieve the i-th event
            event = ch0_data->GetEvent(i);

            // Progress Bar Status
            if(buffer.CheckTime(event.timestamp))
            {
                buffer.Add(event);
            }
        }

        // Output to ROOT File
        std::string output_filename = data_name+"_scanned_spectra_" + std::to_string(time_window) + "_seconds.root";
        TFile * output_file = new TFile(output_filename.c_str(),"RECREATE");

        std::vector<TH1F*> spectra = buffer.GetEnergySpectra();


        for (auto h : spectra)
        {
            h->SetName(from_unix_to_date( ((time_t)(std::stod(h->GetName()))) + time_start_daq).c_str() ) ;

            // Subtract the background
            h->Write();
        }

        h_bkg->Scale(bkg_scaling_factor);
        h_bkg->Write();

        std::cout << std::endl << std::endl;
        std::cout << "Saving spectra to :" << output_filename << std::endl;
        output_file->Write();
        output_file->Close();
    }
    else
    {
        // UNIX Timestamps
        long int event_unix_timestamp;

        for (size_t i = 0; i < nEntries; i++)
        {
            event = ch0_data->GetEvent(i);

            // Progress Bar Status
            if(i%((int)(nEntries*0.001)) == 0) printProgBar(float(i*100)/(nEntries), -100, -100, -100);

            // Building Event UNIX Timestamp
            event_unix_timestamp = (event.timestamp*1e-12) + time_start_daq;

            if(event_unix_timestamp > time_start_user and event_unix_timestamp < time_stop_user)
            {
                // Adding a uniform random number between [0,1]
                // For restoring the ADC truncation
                h->Fill((event.energy+gRandom->Rndm()));
            }
        }

        // Calibrate the X-axis
        h->GetXaxis()->Set(nBins, q, m*nBins+q);

        // Subtracting the Background
        h->Add(h_bkg, bkg_scaling_factor);

        // Saving Histogram to file
        std::string from_time = from_unix_to_date(time_start_user);
        std::string to_time = from_unix_to_date(time_stop_user);
        std::string output_filename = data_name + "_from_" + from_time + "_to_" + to_time + ".root";

        TFile * output_file = new TFile(output_filename.c_str(),"RECREATE");
        h->Write();
        output_file->Write();
        output_file->Close();

    }

    return ;

}

int main(int argc, char **argv)
{
    TApplication app("ROOT", &argc, argv);
    Analysis(app.Argc(), app.Argv());

    return 0;
}

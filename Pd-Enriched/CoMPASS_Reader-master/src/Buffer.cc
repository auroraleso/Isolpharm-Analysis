// STL Includes
#include <iostream>

// ROOT Includes
#include "TH1F.h"
#include "TF1.h"
#include "TRandom.h"
#include "TCanvas.h"

// Custom Includes
#include "Buffer.hh"
#include "CompassEvent.hh"

Buffer::Buffer() : buffer_window(1), last_time_trigger(0), id(0){}
Buffer::Buffer(Int_t bw) : buffer_window(bw), last_time_trigger(0), id(0) {}
Buffer::Buffer(Int_t bw, ULong_t time) : buffer_window(bw), last_time_trigger(time), id(0) {}
Buffer::~Buffer(){}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

double Buffer::DeadTimeCorrectionFactor(double rate)
{
    // // OLD DEAD TIME CALIBRATION PARAMETERS (January 2021)
    // double p0 = 0.985904;
    // double p1 = 3.07385e-05;
    // double p2 = -6.96958e-10;
    // double p3 = 7.40856e-14;

    // New Dead Time Calibration Parameters (July 2021)
    double p0 = 0.985368;
    double p1 = 1.85602e-05;
    double p2 = -1.44854e-10;
    double p3 = 3.05316e-14;

    double correction_factor = p0 + p1*rate + p2*rate*rate + p3*rate*rate*rate;

    return correction_factor >= 1 ? correction_factor : 1.0 ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void Buffer::Add(CompassEvent e)
{
    events_array.push_back(e);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Buffer::CheckTime(ULong_t t)
{
    if( (t - last_time_trigger) > (buffer_window*1E12))
    {

        times.push_back( last_time_trigger + (buffer_window*1E12*0.5) );

        // Computing Dead Time
        this->ComputeDeadTime();
        std::cout << "------------------- " << id << " -------------------" << std::endl;
        id++;
        // Compute Energy Spectra
        this->FillEnergySpectrum(std::to_string(times.back()*1E-12));

        // Clear Events Arrays
        events_array.clear();

        // Setting this time as last
        last_time_trigger = t;

        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void Buffer::FillEnergySpectrum(std::string hname)
{
    TH1F * h = new TH1F(hname.c_str(), "", 16384, 0, 16384);

    for (size_t i = 0; i < events_array.size(); i++)
    {
        // Reject pileup events
        if( (events_array[i].flags & 0xF000) == 0xC000 or (events_array[i].flags & 0xF000) == 0x8000) continue;
        // Reject saturated events_array[i]s
        if( ((events_array[i].flags & 0xF00) == 0x400 or (events_array[i].flags & 0xF0) == 0x80) and  ((events_array[i].flags & 0xF000) != 0xc000 and (events_array[i].flags & 0xF000) != 0x8000) ) continue;

        // Adding a uniform random number between [0,1]
        // For restoring the ADC truncation
        if( (events_array[i].flags == 0 or (events_array[i].flags & 0xF00) == 0x800) and (events_array[i].flags & 0xF000) != 0x8000 ) h->Fill( ((events_array[i].energy)+gRandom->Rndm()));
    }

    // Calibrate the X-axis
    h->GetXaxis()->Set(16384, q, q+16384*m);

    // Dead Time Correction
    // float dead_time_correction_factor = 1.0/(1- (dead_time_array.back()) );
    // h->Scale(dead_time_correction_factor);

    // Dead Time Correction Experimental Correction
    float measured_rate = h->GetEntries()/(buffer_window);
    float correction_factor = this->DeadTimeCorrectionFactor(measured_rate);
    h->Scale(correction_factor);
    std::cout << "buffer window : " << buffer_window << std::endl;
    std::cout << measured_rate  << " -->  CF : " << correction_factor << std::endl;

    // Background Subtraction
    h->Add(h_bkg, -1);

    spectra_array.push_back((TH1F*)h->Clone());
    delete h;

    return ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void Buffer::ComputeDeadTime()
{
    long int dt_events              = 0;
    long int pileup_events          = 0;
    long int saturation_events      = 0;
    long int time_filter_not_passed = 0;
    long int Ntriggers              = 0;
    long int OCR                    = 0;
    long int ICR;

    for (CompassEvent event : events_array)
    {

        ////////////////////////////////////////////////////////////////////////////
        // For CAEN DT5725N
        ////////////////////////////////////////////////////////////////////////////

        // // 1st Bit Position
        // // if( event.flags == 0 or (event.flags & 0xF00) == 0x800 ) // Firmware DT5780
        // if( (event.flags & 0xF000) == 0x4000 and ((event.flags & 0xF00) != 0x400 and (event.flags & 0xF0) != 0x80) )// Firmware DT5725
        // {
        //   OCR++;
        // }
        //
        // // 3rd Bit Position
        // if( ((event.flags & 0xF00) == 0x400 or (event.flags & 0xF0) == 0x80) and  ((event.flags & 0xF000) != 0xc000 and (event.flags & 0xF000) != 0x8000) )
        // {
        //     saturation_events++;
        // }
        // if( (event.flags & 0xF000) == 0xC000 or (event.flags & 0xF000) == 0x8000)
        // {
        //     pileup_events++;
        // }

        ////////////////////////////////////////////////////////////////////////////
        // For CAEN DT5780N
        ////////////////////////////////////////////////////////////////////////////

        // 1st Bit Position
        if( (event.flags == 0 or (event.flags & 0xF00) == 0x800) and (event.flags & 0xF000) != 0x8000 )
        {
          OCR++;
        }

        // 3rd Bit Position
        if( ((event.flags & 0xF00) == 0x400 or (event.flags & 0xF0) == 0x80 or (event.flags & 0xF00) == 0xc00) and  ( (event.flags & 0xF000) != 0x8000) )
        {
            saturation_events++;
        }
        if( (event.flags & 0xF000) == 0x8000 )
        {
            pileup_events++;
        }
    }

    // Tesi Caterina
    double dead_time_scale_factor = 1.32;

    // Computing ICR
    ICR = OCR + pileup_events + saturation_events + time_filter_not_passed;

    // COMPASS Reader Formula
    // float dt = 1.0 - ((OCR+saturation_events)*1.0)/ICR;
    // Experimental Determination

    // Print out info
    std::cout << "pu " << pileup_events;
    std::cout << ", time " << time_filter_not_passed;
    std::cout << ", sat " <<  saturation_events;
    std::cout << ", trig " <<  Ntriggers;
    std::cout << ", ocr " << OCR;
    std::cout << ", icr " << ICR;
    // std::cout << ", dt " <<  dt;
    // std::cout << ", dtcorr " <<  dt*dead_time_scale_factor;
    std::cout << ", size " <<  events_array.size();
    std::cout << ", rate " <<  events_array.size()/3600 << std::endl;

    // dead_time_array.push_back(dt*dead_time_scale_factor);

    return ;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
void Buffer::ComputeDeadTime()
{
    int dt_events              = 0.0;
	  int pileup_events          = 0.0;
	  int saturation_events      = 0.0;
    int time_filter_not_passed = 0.0;
    int Ntriggers              = 0.0;
    float OCR                  = 0.0;
    float ICR = 0;

    // std::cout << "Rate [Hz] --> " << (events_array.size()*1.0)/3600 << std::endl;

    ULong_t current_ts = events_array[0].timestamp;
    ULong_t time_difference;

    TH1F * timedistribution = new TH1F("td","", 5e3, 0,1e3);

    for (CompassEvent event : events_array)
	  {
      if( (event.flags & 0xF000) != 0x8000)
      {
        timedistribution->Fill( (event.timestamp - current_ts) *1e-6);
      }

      current_ts = event.timestamp;

	  }

    TF1 * f1 = new TF1("e","expo",100,300);
    timedistribution->Fit("e","R");
    timedistribution->SaveAs("f.root");
    double true_rate = abs(f1->GetParameter(1))*1e6;

    // std::cout << "Rate [Hz] : " << events_array.size()/120 << "\tTrue Rate [Hz] : " << true_rate << "\tDT : " << 1 - (events_array.size()/120)/true_rate << std::endl;

    delete timedistribution;
    delete f1;

    // ICR = OCR + pileup_events + saturation_events + Ntriggers + time_filter_not_passed;

    // float dt = 1.0 - (OCR+saturation_events)/ICR;
    // Return to other object
    // std::cout << "\t" << std::setprecision(4) << ICR << "\t" << ICR/3600 << std::endl;
    // std::cout << "\t-->\t" << pileup_events;
    // std::cout << ", " << time_filter_not_passed;
    // std::cout << ", " <<  saturation_events;
    // std::cout << ", " <<  Ntriggers;
    // std::cout << ", " << OCR;
    // std::cout << ", " << ICR;
    // std::cout << ", " <<  std::setprecision(4) << dt;
    // std::cout << ", " <<  events_array.size() << std::endl;

    dead_time_array.push_back(1);

    return ;
}
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void Buffer::SetBkgHistogram(TH1F* _h_bkg, float _scaling)
{
    h_bkg = (TH1F*) _h_bkg->Clone();
    h_bkg->Scale(_scaling);

    return ;
}

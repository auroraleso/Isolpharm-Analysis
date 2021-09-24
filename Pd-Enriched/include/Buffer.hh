#ifndef BUFFER_H
#define BUFFER_H

#include "TH1F.h"
#include "Rtypes.h"
#include "CompassEvent.hh"

class Buffer
{
    public:

        Buffer();
        Buffer(Int_t bw);
        Buffer(Int_t bw, ULong_t t);
        ~Buffer();

        bool CheckTime(ULong_t t);
        void Add(CompassEvent e);

        std::vector<float> GetTimes(){ return times; };
        std::vector<float> GetDeadTimes(){ return dead_time_array; };
        std::vector<TH1F*> GetEnergySpectra(){ return spectra_array; };

        void SetCalibrationM(float _m){ m = _m; }
        void SetCalibrationQ(float _q){ q = _q; }
        void SetBkgHistogram(TH1F* _h_bkg, float _scaling);

        void FillEnergySpectrum(std::string);

        double DeadTimeCorrectionFactor(double);

    private:

        // Buffer Window
        // in hours
        Int_t buffer_window;

        // Last time trigger
        ULong_t last_time_trigger;

        // Dead Time Computations
        std::vector<float> times;
        std::vector<float> dead_time_array;

        std::vector<TH1F*> spectra_array;

        // Calibration Parameters
        float m;
        float q;

        // Background
        TH1F * h_bkg;

        // Event Holder
        std::vector<CompassEvent> events_array;

        void ComputeDeadTime();

        int id;

};

#endif

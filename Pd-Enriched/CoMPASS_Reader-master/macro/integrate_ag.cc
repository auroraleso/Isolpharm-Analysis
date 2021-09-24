void integrate(std::string filename, double energy1, double energy2, int step, bool ag)
{
    // Opening the TFile
    TFile * file = new TFile(filename.c_str());

    // Create a TIterator
    TH1F *h;
    TKey *key;
    TIter next( file->GetListOfKeys());

    double t[1000];
    double N1[1000];
    double N2[1000];
    double N3[1000];

    double chi2[1000];

    // Loop over histograms in TFile
    int j = 0;

    bool firsttime_flag = 1;
    double firsttime;

    int skip = 0;

    while( (key = (TKey*) next() ) )
    {
      if( skip < 10)
      {
        skip++;
        continue;
      }

      // Get the histogram
      h = (TH1F*) file->Get(key->GetName());

      if( !((std::string)"bkg").compare(h->GetName()) ) continue;

      // Setting Log y on the TCanvas
      TCanvas * c = new TCanvas(h->GetName());
      // c->SetLogy();.q


      // Find only
      TF1 * fGauss_1  = new TF1("fGauss1","[0]*exp(-0.5*( (x-[1])*(x-[1]) )/([2]*[2]) )", energy1-20, energy1+20);
      TF1 * fGauss_2  = new TF1("fGauss2","[0]*exp(-0.5*( (x-[1])*(x-[1]) )/([2]*[2]) )", energy2-20, energy2+20);
      TF1 * fExp      = new TF1("fExp","[0]*exp(-[1]*x)",0,2000);
      TF1 * fDGausExp = new TF1("fDGausExp","[0]*exp(-0.5*( (x-[1])*(x-[1]) )/([2]*[2]) ) + [3]*exp(-0.5*( (x-[4])*(x-[4]) )/([5]*[5]) ) + [6]*exp(-[7]*x)", 280, 370);
      TF1 * fDGausExp2 = new TF1("fDGausExp2","[0]*exp(-0.5*( (x-[1])*(x-[1]) )/([2]*[2]) ) + [3]*exp(-0.5*( (x-[4])*(x-[4]) )/([5]*[5]) )", 280, 370);

      // Fitting Exponential background
      // ---------------------------------------------------------------------------------------------

      int bin_low1 = h->FindBin(190);
      int bin_high1 = h->FindBin(200);

      int bin_low2 = h->FindBin(260);
      int bin_high2 = h->FindBin(290);

      int bin_low3 = h->FindBin(370);
      int bin_high3 = h->FindBin(390);

      double x[1000];
      double y[1000];
      double yerr[1000];

      int n = bin_high1 - bin_low1 + bin_high2 - bin_low2 + bin_high3 - bin_low3;

      for (size_t i = bin_low1; i < bin_high1; i++)
      {
        x[i-bin_low1] = h->GetBinCenter(i);
        y[i-bin_low1] = h->GetBinContent(i);
        yerr[i-bin_low1] = sqrt(h->GetBinContent(i));
      }

      for (size_t i = bin_low2; i < bin_high2; i++)
      {
        x[bin_high1-bin_low1+i-bin_low2] = h->GetBinCenter(i);
        y[bin_high1-bin_low1+i-bin_low2] = h->GetBinContent(i);
        yerr[bin_high1-bin_low1+i-bin_low2] = sqrt(h->GetBinContent(i));
      }

      for (size_t i = bin_low3; i < bin_high3; i++)
      {
        x[bin_high1-bin_low1+bin_high2-bin_low2+i-bin_low3] = h->GetBinCenter(i);
        y[bin_high1-bin_low1+bin_high2-bin_low2+i-bin_low3] = h->GetBinContent(i);
        yerr[bin_high1-bin_low1+bin_high2-bin_low2+i-bin_low3] = sqrt(h->GetBinContent(i));
      }

      // Fitting Exponential background
      // ---------------------------------------------------------------------------------------------

      TGraphErrors * background = new TGraphErrors(n, x,y,0, yerr);
      background->Fit("fExp","Q","",180,210);
      background->Fit("fExp","Q","",100,250);
      background->Fit("fExp","Q","",100,310);
      background->Fit("fExp","Q","",260,400);
      // TCanvas *ccc = new TCanvas();
      background->Draw("AP");
      // fExp->Draw("SAME");
      // TCanvas *ccc2 = new TCanvas();
      // cout << fExp->GetChisquare()/fExp->GetNDF() << endl;

      fDGausExp->SetParLimits(1,energy1-1,energy1+1);
      fDGausExp->SetParameter(1, energy1);
      fDGausExp->SetParameter(2, 4);
      fDGausExp->SetParLimits(4,energy2-1,energy2+1);
      fDGausExp->SetParameter(4, energy2);
      fDGausExp->SetParameter(5, 4);
      fDGausExp->SetParameter(6, fExp->GetParameter(0));
      fDGausExp->FixParameter(6, fExp->GetParameter(0));
      fDGausExp->SetParameter(7, fExp->GetParameter(1));
      fDGausExp->FixParameter(7, fExp->GetParameter(1));

      h->Fit("fDGausExp","QRBN");

      fGauss_1->SetParameter(0, fDGausExp->GetParameter(0));
      fGauss_1->SetParameter(1, fDGausExp->GetParameter(1));
      fGauss_1->SetParameter(2, fDGausExp->GetParameter(2));
      fGauss_2->SetParameter(0, fDGausExp->GetParameter(3));
      fGauss_2->SetParameter(1, fDGausExp->GetParameter(4));
      fGauss_2->SetParameter(2, fDGausExp->GetParameter(5));

      h->GetXaxis()->SetRangeUser(311-30, energy2+40);
      h->GetYaxis()->SetRangeUser(0,5e4);
      h->SetLineColor(kBlack);
      h->SetMarkerStyle(20);
      h->SetMarkerSize(0.6);


      TH1F* hbkg = (TH1F*) h->Clone();

      for (size_t i = 0; i < 16384; i++)
      {
        hbkg->SetBinContent(i, fExp->Eval(hbkg->GetBinCenter(i)));
      }

      h->Add(h,hbkg,1,-1);

      fDGausExp2->SetParameter(0, fGauss_1->GetParameter(0));
      fDGausExp2->SetParameter(1, fGauss_1->GetParameter(1));
      fDGausExp2->SetParameter(2, fGauss_1->GetParameter(2));
      fDGausExp2->SetParameter(3, fGauss_2->GetParameter(0));
      fDGausExp2->SetParameter(4, fGauss_2->GetParameter(1));
      fDGausExp2->SetParameter(5, fGauss_2->GetParameter(2));

      h->Fit("fDGausExp2","QNR");

      chi2[j] = fDGausExp2->GetChisquare()/fDGausExp2->GetNDF();

      fGauss_1->SetParameter(0, fDGausExp2->GetParameter(0));
      fGauss_1->SetParameter(1, fDGausExp2->GetParameter(1));
      fGauss_1->SetParameter(2, fDGausExp2->GetParameter(2));
      fGauss_2->SetParameter(0, fDGausExp2->GetParameter(3));
      fGauss_2->SetParameter(1, fDGausExp2->GetParameter(4));
      fGauss_2->SetParameter(2, fDGausExp2->GetParameter(5));


      h->Draw("E1");
      // fExp->SetLineColor(kBlue);
      // fExp->Draw("SAME");

      fGauss_1->SetLineColor(kOrange+7);
      fGauss_1->Draw("SAME");

      fGauss_2->SetLineColor(kOrange+7);
      fGauss_2->Draw("SAME");
      fDGausExp2->Draw("SAME");


      double counts1 = (fGauss_1->Integral(fGauss_1->GetParameter(1)-2*abs(fGauss_1->GetParameter(2)), fGauss_1->GetParameter(1)+2*abs(fGauss_1->GetParameter(2))) ) / 1.410091;
      double counts2 = (fGauss_2->Integral(fGauss_2->GetParameter(1)-2*abs(fGauss_2->GetParameter(2)), fGauss_2->GetParameter(1)+2*abs(fGauss_2->GetParameter(2))) ) / 1.410091;

      std::cout << "Peak: "  <<  fGauss_1->GetParameter(1) << " " << counts1 << " " << counts2 << " " << fDGausExp2->GetChisquare()/fDGausExp2->GetNDF() << " " << h->GetName() << std::endl;

      // Convert string date to time since epoch (seconds)
      std::tm tt{};
      std::istringstream sstream(h->GetName());
      sstream >> std::get_time(&tt, "%Y_%m_%d_%H_%M_%S");
      std::time_t time_stamp = mktime(&tt);

      if(firsttime_flag)
      {
        firsttime_flag = 0;
        firsttime = (time_stamp*1.0)/60;
      }

      t[j]  = (time_stamp*1.0)/60 - firsttime + 30;
      N1[j] = counts1;
      N2[j] = counts2;

      j++;

    }

    TCanvas *cchi2 = new TCanvas();

    TGraph * chi2g = new TGraph(j,t,chi2);
    chi2g->Draw();

    double a1[j];
    double a_err1[j];
    double a2[j];
    double a_err2[j];
    double a2_res[j];

    for (size_t i = 0; i < j; i++)
    {
      a_err1[i] = sqrt(N1[i])/N1[i];
      a1[i]     = log(N1[i]);
      a_err2[i] = sqrt(N2[i])/N2[i];
      a2[i]     = log(N2[i]);
    }

    TCanvas * c1 = new TCanvas();
    TGraphErrors * g1 = new TGraphErrors(j,t,a1,0,a_err1);
    g1->Draw();

    TF1 * fLinear_1 = new TF1("fLin1", "pol1");
    g1->Fit("fLin1");
    std::cout << "T 1/2 : " << abs((1.0/(fLinear_1->GetParameter(1)))*log(2)) / (60.0)<< "+-" <<  (fLinear_1->GetParError(1)/(fLinear_1->GetParameter(1)*fLinear_1->GetParameter(1)))*(log(2.0)/(60.0)) << std::endl;
    g1->Draw();

    if(ag)
    {
      TCanvas * c2 = new TCanvas();
      TGraphErrors * g2 = new TGraphErrors(j,t,a2,0,a_err2);

      TF1 * fLinear_2 = new TF1("fLin2", "pol1");
      g2->Fit("fLin2");
      std::cout << "T 1/2 : " << abs((1.0/(fLinear_2->GetParameter(1)))*log(2)) / (60.0*24) << "+-" <<  (fLinear_2->GetParError(1)/(fLinear_2->GetParameter(1)*fLinear_2->GetParameter(1)))*(log(2.0)/(60.0*24)) << std::endl;
      g2->Draw();

      for (size_t i = 0; i < j; i++)
      {
        a2_res[i] = a2[i] - fLinear_2->Eval(t[i]);
      }

      TGraphErrors * residual_ag = new TGraphErrors(j,t,a2_res,0,a_err2);
      TCanvas * cc3 = new TCanvas();
      residual_ag->Draw("AP");

    }

    double activity[j];
    double activity_error[j];

    double acquisition_time = 3600;
    double Pg = 0.067;
    double PgError = 0.003;
    double efficiency = 0.00237159;
    double efficiency_error = efficiency*0.02;
    double rate;

    std::cout << "time,act,erract" << std::endl;
    for (size_t i = 0; i < j; i++)
    {
      rate        = N2[i]/acquisition_time;
      activity[i] = N2[i]/(acquisition_time*Pg*efficiency);
      activity_error[i] = sqrt( ( sqrt(N2[i])*sqrt(N2[i])/(Pg*Pg*efficiency*efficiency*acquisition_time*acquisition_time) ) +
                                ( (N2[i]*N2[i]*PgError*PgError)/(efficiency*efficiency*acquisition_time*acquisition_time*Pg*Pg*Pg*Pg) ) +
                                ( (N2[i]*N2[i]*efficiency*efficiency*0.02*0.02)/(efficiency*efficiency*efficiency*efficiency*acquisition_time*acquisition_time*Pg*Pg) ) );

      std::cout << t[i]/60+47 << "," << activity[i] << "," << activity_error[i] << std::endl;
    }



    return;
}

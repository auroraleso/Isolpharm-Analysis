void integrate(std::string filename, double energy, int step)
{
    // Opening the TFile
    TFile * file = new TFile(filename.c_str());

    // Create a TIterator
    TH1F *h;
    TKey *key;
    TIter next( file->GetListOfKeys());

    double t[1000];
    double N[1000];

    // Loop over histograms in TFile
    int j = 0;

    // Peak Integration
    while( (key = (TKey*) next() ) )
    {
      // Get the histogram
      h = (TH1F*) file->Get(key->GetName());

      if( !((std::string)"bkg").compare(h->GetName()) ) continue;

      // Setting Log y on the TCanvas
      TCanvas * c = new TCanvas(h->GetName());
      c->SetLogy();

      // Find only
      TF1 * fGauss  = new TF1("fGauss","[0]*exp(-0.5*( (x-[1])*(x-[1]) )/([2]*[2]) )", energy-40, energy+40);
      TF1 * fLinear = new TF1("fLinear","[0]*x+[1]", energy-20, energy+40);
      TF1 * fLinGaus = new TF1("fLinGaus","[0]*exp(-0.5*( (x-[1])*(x-[1]) )/([2]*[2]) ) + [3]*x+[4]", energy-4, energy+30);
      h->Fit("fLinear","Q","",energy+20, energy+40);

      fLinGaus->SetParameter(1, energy);
      fLinGaus->SetParameter(2, 2);
      fLinGaus->SetParameter(3, fLinear->GetParameter(0));
      fLinGaus->SetParameter(4, fLinear->GetParameter(1));

      h->Fit("fLinGaus","RQ");

      fLinear->SetParameter(0, fLinGaus->GetParameter(3));
      fLinear->SetParameter(1, fLinGaus->GetParameter(4));

      fGauss->SetParameter(0, fLinGaus->GetParameter(0));
      fGauss->SetParameter(1, fLinGaus->GetParameter(1));
      fGauss->SetParameter(2, fLinGaus->GetParameter(2));

      h->GetXaxis()->SetRangeUser(0,120);

      h->Draw();
      fLinear->SetLineColor(kOrange);
      fGauss->SetLineColor(kOrange-3);
      fLinear->Draw("SAME");
      fGauss->Draw("SAME");

      double counts = 2*fGauss->Integral(fGauss->GetParameter(1), fGauss->GetParameter(1)+2*abs(fGauss->GetParameter(2)));
      std::cout << counts << std::endl;
      t[j] = step*j;
      N[j] = counts;

      std::cout << "Time ------> " << step*j << std::endl;

      j++;
    }

    double a[j];
    double a_err[j];
    double res[j];

    for (size_t i = 0; i < j; i++)
    {
      a_err[i] = sqrt(N[i])/N[i];
      a[i]     = log(N[i]);
    }

    TCanvas * c1 = new TCanvas();
    TGraphErrors * g = new TGraphErrors(j,t,a,0,a_err);

    TF1 * fLinear = new TF1("fLin", "[0]+x*[1]");

    g->Fit("fLin","QN");
    fLinear->Draw();
    cout << "chisquare/ndf: " <<  fLinear->GetChisquare()/fLinear->GetNDF() << endl;
    g->Draw("AP");

    std::cout << "T 1/2 : " << abs((1.0/(fLinear->GetParameter(1)))*log(2)) / 60.0 << "+-" <<  (fLinear->GetParError(1)/(fLinear->GetParameter(1)*fLinear->GetParameter(1)))*(log(2.0)/60.0) << std::endl;

    for (size_t i = 0; i < j; i++)
    {
        res[i] = a[i] - fLinear->Eval(t[i]);
    }

    TCanvas * c2 = new TCanvas();
    TGraphErrors * gres = new TGraphErrors(j,t,res,0,a_err);
    gres->Draw();

    TCanvas * c3 = new TCanvas();
    double activity[j];
    double activity_error[j];

    double acquisition_time = 1200;
    double Pg = 0.0366;
    double PgError = 0.003;
    // double efficiency = 0.000617461; //70cm
    double efficiency = 0.00318966;     //30cm
    double efficiency_error = efficiency*0.02;
    double rate;

    // double time_offset = 17640.0/3600;  // 28/10
    // double time_offset = 96720.0/3600;  // 29/10
    // double time_offset = 171180.0/3600; // 30/10
    double time_offset = 279188.0/3600; // 31/10

    std::cout << "time,act,erract" << std::endl;
    for (size_t i = 0; i < j; i++)
    {
      rate        = N[i]/acquisition_time;
      activity[i] = N[i]/(acquisition_time*Pg*efficiency);
      activity_error[i] = sqrt( ( sqrt(N[i])*sqrt(N[i])/(Pg*Pg*efficiency*efficiency*acquisition_time*acquisition_time) ) +
                                ( (N[i]*N[i]*PgError*PgError)/(efficiency*efficiency*acquisition_time*acquisition_time*Pg*Pg*Pg*Pg) ) +
                                ( (N[i]*N[i]*efficiency*efficiency*0.02*0.02)/(efficiency*efficiency*efficiency*efficiency*acquisition_time*acquisition_time*Pg*Pg) ) );

      std::cout << t[i]/60+time_offset << "," << activity[i] << "," << activity_error[i] << std::endl;
    }




    return;
}

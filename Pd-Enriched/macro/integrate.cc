void integrate(std::string filename, double energy1, double energy2, int step)
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

    // Loop over histograms in TFile
    int j = 0;

    while( (key = (TKey*) next() ) )
    {
      // Get the histogram
      h = (TH1F*) file->Get(key->GetName());

      if( !((std::string)"bkg").compare(h->GetName()) ) continue;

      // Setting Log y on the TCanvas
      TCanvas * c = new TCanvas(h->GetName());
      // c->SetLogy();.q


      // Find only
      TF1 * fGauss_1  = new TF1("fGauss1","[0]*exp(-0.5*( (x-[1])*(x-[1]) )/([2]*[2]) )", energy1-20, energy1+20);
      TF1 * fGauss_2  = new TF1("fGauss2","[0]*exp(-0.5*( (x-[1])*(x-[1]) )/([2]*[2]) )", energy2-20, energy2+20);
      // TF1 * fGauss_3  = new TF1("fGauss3","[0]*exp(-0.5*( (x-[1])*(x-[1]) )/([2]*[2]) )", energy3-50, energy3+20);
      TF1 * fExp      = new TF1("fExp","[0]*exp(-[1]*x)",0,2000);
      TF1 * fDGausExp = new TF1("fDGausExp","[0]*exp(-0.5*( (x-[1])*(x-[1]) )/([2]*[2]) ) + [3]*exp(-0.5*( (x-[4])*(x-[4]) )/([5]*[5]) ) + [6]*exp(-[7]*x)", energy1-40, energy2+40);
      // TF1 * fDGausExp = new TF1("fDGausExp","[0]*exp(-0.5*( (x-[1])*(x-[1]) )/([2]*[2]) ) + [3]*exp(-0.5*( (x-[4])*(x-[4]) )/([5]*[5]) ) + [6]*exp(-[7]*x) + [8]*exp(-0.5*( (x-[9])*(x-[9]) )/([10]*[10]) )", energy3-40, energy2+40);

      h->Fit("fExp","","",180, 220);
      // h->Fit("fExp","","",energy1-50, energy1-20);
      // h->Fit("fExp","","",energy1-90, energy1-20);

      fDGausExp->SetParameter(1, energy1);
      fDGausExp->SetParameter(2, 2);
      fDGausExp->SetParameter(4, energy2);
      fDGausExp->SetParameter(5, 2);
      // fDGausExp->SetParameter(8,2e3);
      // fDGausExp->SetParameter(9, energy3);
      // fDGausExp->SetParameter(10, 2);
      fDGausExp->SetParameter(6, fExp->GetParameter(0));
      fDGausExp->FixParameter(6, fExp->GetParameter(0));
      fDGausExp->SetParameter(7, fExp->GetParameter(1));
      fDGausExp->FixParameter(7, fExp->GetParameter(1));

      h->Fit("fDGausExp","QRB");
      h->Fit("fDGausExp","QRB");

      fExp->SetParameter(0, fDGausExp->GetParameter(6));
      fExp->SetParameter(1, fDGausExp->GetParameter(7));
      fGauss_1->SetParameter(0, fDGausExp->GetParameter(0));
      fGauss_1->SetParameter(1, fDGausExp->GetParameter(1));
      fGauss_1->SetParameter(2, fDGausExp->GetParameter(2));
      fGauss_2->SetParameter(0, fDGausExp->GetParameter(3));
      fGauss_2->SetParameter(1, fDGausExp->GetParameter(4));
      fGauss_2->SetParameter(2, fDGausExp->GetParameter(5));
      // fGauss_3->SetParameter(0, fDGausExp->GetParameter(8));
      // fGauss_3->SetParameter(1, fDGausExp->GetParameter(9));
      // fGauss_3->SetParameter(2, fDGausExp->GetParameter(10));


      h->GetXaxis()->SetRangeUser(energy1-50,energy2+100);
      h->GetYaxis()->SetRangeUser(0,h->GetMaximum());
      h->SetLineColor(kBlack);
      h->SetMarkerStyle(20);
      h->SetMarkerSize(0.6);
      h->Draw("E1");

      fExp->SetLineColor(kBlue);
      fExp->Draw("SAME");

      fGauss_1->SetLineColor(kOrange-3);
      fGauss_1->Draw("SAME");

      fGauss_2->SetLineColor(kOrange+7);
      fGauss_2->Draw("SAME");
      //
      // fGauss_3->SetLineColor(kOrange+10);
      // fGauss_3->Draw("SAME");

      fDGausExp->Draw("SAME");

      double counts1 = (fGauss_1->Integral(fGauss_1->GetParameter(1)-2*fGauss_1->GetParameter(2), fGauss_1->GetParameter(1)+2*fGauss_1->GetParameter(2)) ) / 1.410091;
      double counts2 = (fGauss_2->Integral(fGauss_2->GetParameter(1)-2*fGauss_2->GetParameter(2), fGauss_2->GetParameter(1)+2*fGauss_2->GetParameter(2)) ) / 1.410091;
      // double counts3 = (fGauss_3->Integral(fGauss_3->GetParameter(1)-2*fGauss_3->GetParameter(2), fGauss_3->GetParameter(1)+2*fGauss_3->GetParameter(2)) ) / 1.410091;

      // std::cout << counts1 << " " << counts2 << " " << std::endl;

      t[j] = step*j;
      N1[j] = counts1;
      N2[j] = counts2;
      // N3[j] = counts3;

      j++;
    }

    double a1[j];
    double a_err1[j];
    double a2[j];
    double a_err2[j];
    // double a3[j];
    // double a_err3[j];

    for (size_t i = 0; i < j; i++)
    {
      a_err1[i] = sqrt(N1[i])/N1[i];
      a1[i]     = log(N1[i]);
      a_err2[i] = sqrt(N2[i])/N2[i];
      a2[i]     = log(N2[i]);
      // a_err3[i] = sqrt(N3[i])/N3[i];
      // a3[i]     = log(N3[i]);
    }

    TCanvas * c1 = new TCanvas();
    TGraphErrors * g1 = new TGraphErrors(j,t,a1,0,a_err1);
    g1->Draw();

    TF1 * fLinear_1 = new TF1("fLin1", "pol1");
    g1->Fit("fLin1");
    std::cout << "T 1/2 : " << abs((1.0/(fLinear_1->GetParameter(1)))*log(2)) / 60.0 << std::endl;
    g1->Draw();

    TCanvas * c2 = new TCanvas();
    TGraphErrors * g2 = new TGraphErrors(j,t,a2,0,a_err2);

    TF1 * fLinear_2 = new TF1("fLin2", "pol1");
    g2->Fit("fLin2");
    std::cout << "T 1/2 : " << abs((1.0/(fLinear_2->GetParameter(1)))*log(2)) / 60.0 << std::endl;
    g2->Draw();





    // std::cout << "T 1/2 : " << abs((1.0/(fLinear->GetParameter(1)))*log(2)) / 60.0 << std::endl;
    return;
}

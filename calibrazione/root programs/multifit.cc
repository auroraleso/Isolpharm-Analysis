
#include <iostream>

#include "TROOT.h"
#include "TMath.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"


 /*           ------- come usarmi  --------
            nel file metti Q, Vout, 0, errVout
            --------------------------------
            do errore alla fine perchè 
            sono un bimbo speciale,
            quindi se devi ricompilarmi devi dare un .exit(), 
            poi rientrare e compilare
 */



double_t fitpol3(double_t *x, double_t *par) 
{
   double_t fitval2 = par[0] + par[1]*x[0]+par[2]*x[0]*x[0]+par[3]*x[0]*x[0]*x[0];
  return fitval2;
}
double residuals(double *x, double *par) {
  double fitval = par[0] + par[1]*0;
   
  return fitval;
}

 

void multifit() {
  //crea un puntatore e nel costruttore gli passiamo direttamente il nome del file da cui prendere i dati
  TGraph *g = new TGraphErrors("dt80.txt");
  TGraph *g1 = new TGraphErrors("dt302.txt");
  TGraph *g2 = new TGraphErrors("dt356.txt");
    // associo un nome al seti di dati, do' un nome al puntatore del TGraphErrors
  g->SetName("fit80");
  g1->SetName("fit302");
  g2->SetName("fit356");

  TCanvas * c1 = new TCanvas("c1", "fit dead time",
                             27, 50, 1020, 760);
  g->SetMarkerStyle(8);
  g->SetMarkerSize(0.7);
  g->SetMarkerColor(2);
  g->Draw("AP");
  g->SetTitle("calib");
  g->GetXaxis()->SetTitle("Filtration Rate ");
  g->GetYaxis()->SetTitle("F");

  g1->SetMarkerStyle(8);
  g1->SetMarkerSize(0.7);
  g1->SetMarkerColor(4);
  g1->Draw("AP");
  g1->SetTitle("calib");
  g1->GetXaxis()->SetTitle("Filtration Rate ");
  g1->GetYaxis()->SetTitle("F");

  g2->SetMarkerStyle(8);
  g2->SetMarkerSize(0.7);
  g2->SetMarkerColor(8);
  g2->Draw("AP");
  g2->SetTitle("calib");
  g2->GetXaxis()->SetTitle("Filtration Rate ");
  g2->GetYaxis()->SetTitle("F");
  
  // definisco funzione del fit
  // la chiamo f1 e la associo alla funzione esponenziale che ho creato prima di nome fitf2. 
  //Devo dare i parametri xmin e xmax
  TF1 *f1 = new TF1("f1", fitpol3, 700, 18000, 4);
  TF1 *f2 = new TF1("f2", fitpol3, 700, 18000, 4);
  TF1 *f3 = new TF1("f3", fitpol3, 700, 18000, 4);

    f1->SetLineStyle(1);
    f1->SetLineColor(2);
    f1->SetLineWidth(3);
    f1->SetParameters(0,0,0,0);
    f1->SetParNames("par0", "par1", "par2", "par3");
    g->Fit(f1,"RS");
    double p0= f1->GetParameter(0);
    double p1 = f1->GetParameter(1);
    double ep0= f1->GetParError(0);
    double ep1 = f1->GetParError(1);
  
  //stampo questi valori che ho recuperato
  std::cout << "\nParametri pol3 80 keV"
            << p0<< " +- " << ep0
            << p1<< " +- " << ep1
            << std::endl;


    f2->SetLineStyle(1);
    f2->SetLineColor(4);
    f2->SetLineWidth(3);
    f2->SetParameters(0,0,0,0);
    f2->SetParNames("par0", "par1", "par2", "par3");
    g1->Fit(f2,"RS");
    double p01= f2->GetParameter(0);
    double p11 = f2->GetParameter(1);
    double ep01= f2->GetParError(0);
    double ep11 = f2->GetParError(1);
  
  //stampo questi valori che ho recuperato
  std::cout << "\nParametri pol3 302 keV"
            << p01<< " +- " << ep01
            << p11<< " +- " << ep11
            << std::endl;

    f3->SetLineStyle(1);
    f3->SetLineColor(8);
    f3->SetLineWidth(3);
    f3->SetParameters(0,0,0,0);
    f3->SetParNames("par0", "par1", "par2", "par3");
    g2->Fit(f3,"RS");
    double p02= f3->GetParameter(0);
    double p12 = f3->GetParameter(1);
    double ep02= f3->GetParError(0);
    double ep12= f3->GetParError(1);
  
  //stampo questi valori che ho recuperato
  std::cout << "\nParametri pol3 80 keV"
            << p02<< " +- " << ep02
            << p12<< " +- " << ep12
            << std::endl;
TMultiGraph* mg = new TMultiGraph();
    mg->Add(g);
    mg->Add(g1);
    mg->Add(g2);
    mg->Draw("ap");
    mg->SetTitle("Plot F vs Filtered Rate");
    mg->GetYaxis()->SetTitle("F= #frac{Peak Area isolated souce}{Peak Area with second source } ");
    mg->GetXaxis()->SetTitle("Filtered rate");
    f1->Draw("same");
    f2->Draw("same");
    f3->Draw("same");

    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    //legend->SetHeader("","C"); // option "C" allows to center the header
    legend->AddEntry("f1","80.9979(11) keV","l");
    legend->AddEntry("f2","302.8512(16) keV","l");
    legend->AddEntry("f3","356.0134(17) keV","l");
    legend->Draw();

  //c1->Print("fitsalitashaper.pdf");

  //residui del fit
  /*TCanvas* c2 = new TCanvas("c2", "residuisalita",
                             27, 50, 1020, 760);
  TGraphErrors* gr = (TGraphErrors*)g->Clone("gres");
  gr->SetTitle("residui");
  gr->GetXaxis()->SetTitle("f");
  gr->GetYaxis()->SetTitle("Residui");

  for (int i = 0; i < gr->GetN(); i++) {
      double x = gr->GetX()[i];
      double res = gr->GetY()[i] - f1->Eval(x);
      gr->SetPoint(i, x, res);
  }
 
 
  for (int i = 0; i < gr->GetN(); i++) {
        double x = gr->GetX()[i];
      double ex = gr->GetEX()[i];
      double ey = gr->GetEY()[i];
      double eres2 = ey * ey + ex * ex * f1->Derivative(x) * f1->Derivative(x);
      gr->SetPointError(i, 0, TMath::Sqrt(eres2)); // ora ex=0 perche' lo abbiamo riportato sulle y
  }


  
  TF1 *zero = new TF1("zero", fitzero, 0, 10000, 2);
    zero->SetLineStyle(9);
    zero->SetLineColor(7);
   TMultiGraph* mg1 = new TMultiGraph();
    mg1->Add(gr);
    mg1->Draw("AP");
    mg1->SetTitle("Residui efficienza HPGe");
    mg1->GetYaxis()->SetTitle("Efficiency ");
    mg1->GetXaxis()->SetTitle("Residuals (kEv)");
    zero->Draw("same");
 
 // c2->Print("Residuiobliq.pdf");
    
    auto r = g->Fit(f1,"RS0"); // R = Region   S = StoreResults
    gStyle->SetOptFit();
    TMatrixDSym cov = r->GetCovarianceMatrix();  //  invoco la matrice di covarianza
    double chi2   = r->Chi2();
    std::cout << "\nCOVARIANCE MATRIX :\n ";
    cov.Print();*/

   
  return;
}

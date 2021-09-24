
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



double_t fitexp(double_t *x, double_t *par) 
{
   double_t fitval2 = exp( par[0]+par[1]*x[0]);
  return fitval2;
}
double fitzero2(double *x, double *par) {
  double fitval = par[0] + par[1]*0;
   
  return fitval;
}

 

void exp() {
  //crea un puntatore e nel costruttore gli passiamo direttamente il nome del file da cui prendere i dati
  TGraphErrors *g = new TGraphErrors("575bq.txt");
    // associo un nome al seti di dati, do' un nome al puntatore del TGraphErrors
  g->SetName("fit");

  TCanvas * c1 = new TCanvas("c1", "fit 1",
                             27, 50, 1020, 760);
  g->SetMarkerStyle(8);
  g->SetMarkerSize(0.5);
  g->SetMarkerColor(1);
  //  "ap" significa che deve disegnare anche gli assi ( a sta per assi) e che deve disegnare i punti con un marker i cui parametri sono impostati precedentemente (p per i punti)
  g->Draw("AP");
  g->SetTitle("calib");
  g->GetXaxis()->SetTitle("efficiency ");
  g->GetYaxis()->SetTitle(" E (keV) ");


  double min=0;
  double max=200;


  // definisco funzione del fit
  // la chiamo f1 e la associo alla funzione esponenziale che ho creato prima di nome fitf2. 
  //Devo dare i parametri xmin e xmax
  TF1 *f1 = new TF1("f1", fitexp, min, max, 2);
  f1->SetLineStyle(1);
    f1->SetLineColor(2);
    f1->SetLineWidth(2);
  // setta i valori iniziali dei parametri
  f1->SetParameters(0,0);
    // (nome parametro 0, nome parametro 1, nome parametro2)
  f1->SetParNames("a", "b");

  
  g->Fit(f1,"RS");
  double p0= f1->GetParameter(0);
    double p1 = f1->GetParameter(1);
 // prendo l'errore del fit che mi hp0dato sul parametro
  double ep0= f1->GetParError(0);
    double ep1 = f1->GetParError(1);
  
  //stampo questi valori che ho recuperato
  std::cout << "\nParametri retta "
            << p0<< " +- " << ep0
            << p1<< " +- " << ep1<<std::endl
            <<" tempo dimezzamento= "<<-log(2)/p1/24
            << std::endl;

          
TMultiGraph* mg = new TMultiGraph();
    mg->Add(g);
    mg->Draw("ap");
    mg->SetTitle("Pd-enriched,  Area 575 peak versus time for HPGe");
    mg->GetYaxis()->SetTitle("ln(Peak Area)");
    mg->GetXaxis()->SetTitle("T [h] from irradiation ending");
    f1->Draw("same");

  //c1->Print("fitsalitashaper.pdf");

  //residui del fit
  TCanvas* c2 = new TCanvas("c2", "2",
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


  
  TF1 *zero = new TF1("zero", fitzero2, min, max, 2);
    zero->SetLineStyle(9);
    zero->SetLineColor(7);
   TMultiGraph* mg1 = new TMultiGraph();
    mg1->Add(gr);
    mg1->Draw("AP");
    mg1->SetTitle("Pd-enriched,  Residuals from area 575 peak versus time for HPGe");
    mg1->GetXaxis()->SetTitle("T [h] from irradiation ending");
    mg1->GetYaxis()->SetTitle("Residuals ");
    zero->Draw("same");
 
 
   
  return;
}

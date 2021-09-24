
#include <iostream>
#include <random>
#include <chrono>
#include "TROOT.h"
#include "TMath.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include <math.h>




double fiteff2(double *x, double *par) 
{
   double fitval2 = par[0] * (exp(-par[1]*pow(*x,par[2]))+exp(-par[3]*pow(*x,par[4])))*(1-exp(par[5]*pow(*x,par[6])));
  return fitval2;
}
double fitzero22(double *x, double *par) {
  double fitval = par[0] + par[1]*0;
   
  return fitval;
}

 

void eff2() {
 
  TGraphErrors *g = new TGraphErrors("effHPGe2.txt");
    
  g->SetName("fit");

  TCanvas * c1 = new TCanvas("c1", "efficiency curve ",
                             27, 50, 1020, 760);
  g->SetMarkerStyle(8);
  g->SetMarkerSize(1);
  g->SetMarkerColor(1);
 
  g->Draw("AP");
  g->SetTitle("efficiency");
  g->GetXaxis()->SetTitle("x");
  g->GetYaxis()->SetTitle("y");
  
 
  TF1 *f1 = new TF1("f1", fiteff2, 36, 1500, 7);
  f1->SetLineStyle(1);
    f1->SetLineColor(2);
    f1->SetLineWidth(1);
  
f1->SetParameter(0,-1);
  f1->SetParameter(1,-2.7);
  f1->SetParameter(2,-1.198);
  f1->SetParameter(3,-9.093);
  f1->SetParameter(4,-1.513);
  f1->SetParameter(5,-0.3612);
  f1->SetParameter(6,-0.008296);
  f1->SetParNames("par0", "par1", "par2", "par3", "par4", "par5", "par6");

  
  g->Fit(f1,"RS");

  double p0= f1->GetParameter(0);
  double p1 = f1->GetParameter(1);
  double p2= f1->GetParameter(2);
  double p3 = f1->GetParameter(3);
  double p4= f1->GetParameter(4);
  double p5 = f1->GetParameter(5);
  double p6= f1->GetParameter(6);

 
  double ep0= f1->GetParError(0);
  double ep1 = f1->GetParError(1);
  double ep2 = f1->GetParError(2);
  double ep3 = f1->GetParError(3);
  double ep4 = f1->GetParError(4);
  double ep5 = f1->GetParError(5);
  double ep6 = f1->GetParError(6);
  

  /*
              Printing parameters  

std::cout << "\nParametri del fit "
            << p0<< " +- " << ep0<<std::endl
            << p1<< " +- " << ep1<<std::endl
            << p2<< " +- " << ep2<<std::endl
            << p3<< " +- " << ep3<<std::endl
            << p4<< " +- " << ep4<<std::endl
            << p5<< " +- " << ep5<<std::endl
            << p6<< " +- " << ep6<<std::endl
            << std::endl;
            
  */



//Draw on a canvas
TMultiGraph* mg = new TMultiGraph();
    mg->Add(g);
    mg->Draw("ap");
    mg->SetTitle("Efficiency HPGe");
    mg->GetXaxis()->SetTitle("E (keV)");
    mg->GetYaxis()->SetTitle("Efficiency");
    f1->Draw("same");
    TFitResultPtr r1 = g->Fit(f1,"S");
   
   

 /* 
                                               Printing needed values 


 std::cout<<" val eff for 342: "<<f1->Eval(342)<<" val eff for 245: "<<f1->Eval(245)<<" val eff for 88: "<<f1->Eval(88)<<" val eff for 309: "
  <<f1->Eval(309)<<" val eff for 311: "<<f1->Eval(311)<<" val eff for 575: "<<f1->Eval(575)<<
  " val eff for 647: "<<f1->Eval(647)<<" val eff for 602: "<<f1->Eval(602)<<" val eff for 632: "<<f1->Eval(632)<<" val eff for 623: "<<f1->Eval(623)<<" val eff for 581: "<<f1->Eval(581)<<
  " val eff for 547: "<<f1->Eval(547)<<" val eff for 70: "<<f1->Eval(70)<<std::endl;*/
  

  /* 
                                                  COMPUTE RESIDUALS

  WARNING: if you're using this piece of code, every time you need to run it againg you have to 
  qui root in order to avoid crushes. (Idk wher's the problem).*/

  
  TCanvas* c2 = new TCanvas("c2", "residuisalita",
                             27, 50, 1020, 760);
  TGraphErrors* gr = (TGraphErrors*)g->Clone("gres");
  gr->SetTitle("residui");
  gr->GetXaxis()->SetTitle("f");
  gr->GetYaxis()->SetTitle("Residui");

 
 
  for (int i = 0; i < gr->GetN(); i++) {
        double x = gr->GetX()[i];
         double res = gr->GetY()[i] - f1->Eval(x);
     
      double ex = gr->GetEX()[i];
      double ey = gr->GetEY()[i];
      double eres2 = ey * ey + ex * ex * f1->Derivative(x) * f1->Derivative(x);
       gr->SetPoint(i, x, res);
      // gr->SetPointError(i,0, eres2);
       
      std::cout<<res<<" "<<x<<" "<<eres2<<std::endl;
  }

  TF1 *zero = new TF1("zero", fitzero22, 36, 1500, 2);

    zero->SetLineStyle(9);
    zero->SetLineColor(7);
   TMultiGraph* mg1 = new TMultiGraph();
    mg1->Add(gr);
    mg1->Draw("AP");
    mg1->SetTitle("Residuals Efficiency HPGe");
    mg1->GetYaxis()->SetTitle("Residuals ");
    mg1->GetXaxis()->SetTitle("Efficiency");
    zero->Draw("same");
  


 
    /*auto r = g->Fit(f1,"RS0"); // R = Region   S = StoreResults
    gStyle->SetOptFit();
    TMatrixDSym cov = r->GetCovarianceMatrix();  
    double chi2   = r->Chi2();
    std::cout << "\nCOVARIANCE MATRIX :\n ";
    cov.Print();*/

   
  return;
}

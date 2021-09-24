
using namespace std;

//C++ LIBRARIES
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
//ROOT LIBRARIES
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMath.h"


/*-------- MAIN -------*/

void plot()
{   
    string name="172bq.txt";
    ifstream f;
    vector<double> x, y, ex, ey;

    f.open(name);
    double i = 0;
    while(f >> i) 
    {

        x.push_back(i); 
        //x[i]=x[i]/3600;  
        f >> i;
        y.push_back(i); 
        /*y[i]=y[i];
        f>>i; 
        ex.push_back(i);  
        f>>i;
        ey.push_back(i);*/

    }    
    
 auto graph = new TGraph(x.size(), &x[0], &y[0]);
 //TF1 *f1 = new TF1("f1","pol0+expo(1)",3,100);
 //graph->Fit(f1,"RB");
 TCanvas* c5 = new TCanvas("c5", "Pd-enriched,  Activity 172 peak versus time for HPGe",
                             27, 50, 1020, 760);
    
    
    TMultiGraph* mg3 = new TMultiGraph();
    mg3->Add(graph);
   
    graph->SetFillColorAlpha(kRed-7,0.4);
    graph->SetMarkerColor(1);
    graph->SetMarkerStyle(8);
    graph->SetMarkerSize(0.5);
    mg3->Draw("APE4");

    //std::cout<<"zero projection "<<f1->Eval(0)<<std::endl;
    mg3->SetTitle("Pd-enriched,  Pd-enriched,  Activity 172 peak versus time for HPGe");
    mg3->GetYaxis()->SetTitle("Activity [Bq]");
    mg3->GetXaxis()->SetTitle("T [hours] from irradiation ending");

   return;
}

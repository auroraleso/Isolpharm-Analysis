
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
#include "TLegend.h"

/*-------- MAIN -------*/

void multiPlot()
{   
    string name1="montecarlo.txt";
    string name2="342bq.txt";
    string name3="245bq.txt";
    ifstream f1,f2,f3;
    vector<double> x, y, x1, y1,x2,y2;

    f1.open(name1);
    f2.open(name2);
    f3.open(name3);
    double i = 0;
    while(f1 >> i) 
    {

        x.push_back(i);   
        f1 >> i;
        y.push_back(i);    

    }    
    double j = 0;
    while(f2 >> j) 
    {

        x1.push_back(j);   
        f2 >> j;
        y1.push_back(j);    

    }  
    double k = 0;
    while(f3 >> k) 
    {

        x2.push_back(k);   
        f3 >> k;
        y2.push_back(k);    

    }      
 TGraph* graph1 = new TGraph(x.size(), &x[0], &y[0]);
 TGraph* graph2 = new TGraph(x1.size(), &x1[0], &y1[0]);
 TGraph* graph3 = new TGraph(x2.size(), &x2[0], &y2[0]);
 TCanvas* c5 = new TCanvas("c5", "Pd-enriched,  HPGe detector, Ag111 Activity",
                             27, 50, 1020, 760);
    TMultiGraph* mg3 = new TMultiGraph();
    mg3->Add(graph1);
    graph1->SetMarkerColor(4);
    graph1->SetLineColor(4);
    graph1->SetLineWidth(3);
    graph1->SetMarkerStyle(7);
    
    mg3->Add(graph2);
    graph2->SetMarkerColor(3);
    graph2->SetLineColor(3);
    graph2->SetLineWidth(3);
    graph2->SetMarkerStyle(7);
    mg3->Add(graph3);
    graph3->SetMarkerColor(2);
    graph3->SetLineColor(2);
    graph3->SetLineWidth(3);
    graph3->SetMarkerStyle(7);
   mg3->Draw("AP");
    mg3->SetTitle("Pd-enriched,  HPGe detector, Ag111 Activity");
    mg3->GetYaxis()->SetTitle("Activity [Bq] ");
    mg3->GetXaxis()->SetTitle("T [s] from irradiation ending");


TLegend *leg = new TLegend(0.1, 0.7, 0.3, 0.9);
leg->SetFillColor(0);
leg->SetHeader("Legend");
leg->AddEntry(graph1, "Montecarlo simulation", "lp");
leg->AddEntry(graph2, "342 keV", "lp");
leg->AddEntry(graph3, "245 keV", "lp");
leg->Draw();
   return;
}

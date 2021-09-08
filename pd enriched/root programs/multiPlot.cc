
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
   
    string name2="342bq.txt";
    string name3="245bq.txt";
    ifstream f2,f3;
    vector<double>  x1, y1,x2,y2,ex,ey,ex1,ey1;

   
    f2.open(name2);
    f3.open(name3);
    
    double j = 0;
    while(f2 >> j) 
    {

        x1.push_back(j);   
        f2 >> j;
        y1.push_back(j);  
        y1[j]=y1[j];
        f2>>j;
        ex.push_back(j);  
        f2>>j;
        ey.push_back(j);


    }  
    double k = 0;
    while(f3 >> k) 
    {

        x2.push_back(k);   
        f3 >> k;
        y2.push_back(k);   
        y2[k]=y2[k];
        f3>>k;
        ex1.push_back(k);  
        f3>>k;
        ey1.push_back(k); 

    }      

 auto graph2 = new TGraphErrors(x1.size(), &x1[0], &y1[0],&ex[0],&ey[0]);
 auto graph3 = new TGraphErrors(x2.size(), &x2[0], &y2[0],&ex1[0],&ey1[0]);
 TCanvas* c1 = new TCanvas("c1", "Pd-enriched,  HPGe detector, Ag111 Activity",
                             27, 50, 1020, 760);
    TMultiGraph* mg3 = new TMultiGraph();
    
    mg3->Add(graph2);
    graph2->SetMarkerColor(3);
    graph2->SetLineColor(3);
    graph2->SetLineWidth(3);
    graph2->SetMarkerSize(0.5);
    graph2->SetMarkerStyle(8);
    mg3->Add(graph3);
    graph3->SetMarkerColor(2);
    graph3->SetLineColor(2);
    graph3->SetLineWidth(3);
    graph3->SetMarkerSize(0.5);
    graph3->SetMarkerStyle(8);
    mg3->Draw("AP");
    mg3->SetTitle("Pd-enriched,  HPGe detector, Ag111 Activity");
    mg3->GetYaxis()->SetTitle("Activity [Bq] ");
    mg3->GetXaxis()->SetTitle("T [s] from irradiation ending");


TLegend *leg = new TLegend(0.1, 0.7, 0.3, 0.9);
leg->SetFillColor(0);
leg->SetHeader("Legend");

leg->AddEntry(graph2, "342 keV", "lp");
leg->AddEntry(graph3, "245 keV", "lp");
leg->Draw();
   return;
}

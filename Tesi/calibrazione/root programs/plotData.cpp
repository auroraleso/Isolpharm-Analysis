
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
    string name="RES.txt";
    ifstream f;
    vector<double> x, y, x1, y1;

    f.open(name);
    double i = 0;
    while(f >> i) 
    {

        x.push_back(i);   
        f >> i;
        y.push_back(i);    

    }    
 TGraph* graph = new TGraph(x.size(), &x[0], &y[0]);
 TCanvas* c5 = new TCanvas("c5", "plot LTspice and data Shaper",
                             27, 50, 1020, 760);
    TMultiGraph* mg3 = new TMultiGraph();
    mg3->Add(graph);
    graph->SetLineColor(9);
    graph->SetLineWidth(3);
    mg3->Draw("al");
    mg3->SetTitle("plot LTspice and data Shaper");
    mg3->GetYaxis()->SetTitle("V (mV)");
    mg3->GetXaxis()->SetTitle("T (s)");

   return;
}

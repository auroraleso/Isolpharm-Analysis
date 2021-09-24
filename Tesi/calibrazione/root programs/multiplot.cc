
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
void multipl(string name,vector<double> x, vector<double> y)
{
    
    ifstream f;
    f.open(name);
    double i = 0;
    while(f >> i) 
    {

        x.push_back(i);   
        f >> i;
        y.push_back(i);    

    }    
    f.close();
    
}

void multiplot()
{   
    string name,name1,name2;
    
    vector<double> x,x1,x2,y,y1,y2;


    name="dt80.txt";
    name1="dt302.txt";
    name2="dt356.txt";


    ifstream f;
    f.open(name);
    double i = 0;
    while(f >> i) 
    {

        x.push_back(i);   
        f >> i;
        y.push_back(i);    

    }    
    
ifstream f1;
    f1.open(name1);
    i = 0;
    while(f1 >> i) 
    {

        x1.push_back(i);   
        f1 >> i;
        y1.push_back(i);    

    }    

ifstream f2;
    f2.open(name2);
    i = 0;
    while(f2 >> i) 
    {

        x2.push_back(i);   
        f2 >> i;
        y2.push_back(i);    

    }    
    //multipl(name,x,y);
    //multipl(name1,x1,y1);
    //multipl(name2,x2,y2);


 TGraph* graph = new TGraph(x.size(), &x[0], &y[0]);
 TGraph* graph1 = new TGraph(x1.size(), &x1[0], &y1[0]);
 TGraph* graph2 = new TGraph(x2.size(), &x2[0], &y2[0]);
 TCanvas* c5 = new TCanvas("c1", "plot dt correction",
                             27, 50, 1020, 760);
    TMultiGraph* mg3 = new TMultiGraph();
    mg3->Add(graph);
    mg3->Add(graph1);
    mg3->Add(graph2);

    graph->SetMarkerColor(9);
    

    graph1->SetLineColor(21);
    graph1->SetLineWidth(3);

    graph2->SetLineColor(11);
    graph2->SetLineWidth(3);


    mg3->Draw("ALP");
    mg3->SetTitle("plot LTspice and data Shaper");
    mg3->GetXaxis()->SetTitle("Filtration Rate");
    mg3->GetYaxis()->SetTitle("R");

   return;
}

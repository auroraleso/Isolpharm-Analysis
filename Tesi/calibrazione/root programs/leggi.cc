/*
    Reading COMPASS ROOT file example
*/
#include <string>
#include "../include/TFile.h"
#include "../include/TH1F.h"
#include "../include/TTreeReaderValue.h"
#include "../include/TTreeReader.h"
void leggi(std::string filename)
{
    // Open ROOT file
    TFile * input_file = new TFile(filename.c_str());

    // TTreeReader
    TTreeReader tree_reader("Data_R", input_file);

    TTreeReaderValue<unsigned short> channel(tree_reader, "Channel");
    TTreeReaderValue<unsigned long long> timestamp(tree_reader, "Timestamp"); //si legge per ogni evento e mi da il tempo dell'evento. picosec
    TTreeReaderValue<unsigned short> energy(tree_reader, "Energy");
	TTreeReaderValue<unsigned int> flags(tree_reader, "Flags");
    TH1F * h = new TH1F("h",filename.c_str(),16384,0,16384);
int n=0,countPile=0,countsat1=0,countsat2=0, countClean=0;
unsigned long long first_timestamp, last_timestamp;
bool first=true;
    while(tree_reader.Next())
    {
    if (first)
    {
    	first=false;
    	first_timestamp=*timestamp;
    }
        h->Fill(*energy);
        
 
    //updating counters according to flags 
    if ((*flags & 0xF000)==0x8000) countPile++;
   if ((*flags & 0xF00)==0x400  ) countsat1++;
    if (*flags==0  ) countClean++;
   if ((*flags & 0xF0)==0x80) countsat2++; //not right prob
   
   
    
        
        last_timestamp=*timestamp;
        
    }
  //printing counters for kind of events occoured
   std::cout<<"Clean events: "<<countClean<<std::endl<<"pile up events n "<<countPile<<std::endl<<"saturated events digitalizer n "<<countsat1<<std::endl<<"saturated events input n "<<countsat2<<std::endl;
   
   //computing ICR
   double ICR=countClean+countPile+countsat1+countsat2;
   
   //computing dead time
   long double dt=1-(countClean+countsat1+countsat2)/ICR;
   //set correction factor to rescalate histogram
   double cf= 1./(1-dt*1.28);
   
   //printing results 
   std::cout<<"ICR="<<ICR<<std::endl<<"Dead time="<<dt<<std::endl<<"Correction factor="<<cf<<std::endl;
   //set kind of detector in order to plot calibrated histo 
   std::cout<<"write 1 for HPGe, 2 for LaBr3:"<<std::endl;
   int kind;
   std::cin>>kind;
   //for HPGe
   if (kind==1) h->GetXaxis()->Set(16384,0,16384*0.05491+0.2428); 
   //for  LABr3
   if (kind==2) h->GetXaxis()->Set(16384,0,16384*0.346+16384*16384*pow(10,-6)*1.466+2.616); 
	if (kind !=2 and kind !=1) std::cout<<"no calibration on assX."<<std::endl;
	//scaling histo according to 'cf',
	// correction factor due to dead time 'dt'
	h->Scale(cf);
   	h->Draw();
	//h->SaveAs

    return ;

}


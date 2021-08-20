/*
    Reading COMPASS ROOT file example
*/
#include <string>
#include "../include/TFile.h"
#include "../include/TH1F.h"
#include "../include/TTreeReaderValue.h"
#include "../include/TTreeReader.h"
void dtGe(std::string filename)
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
    else if ((*flags & 0xF00)==0x400  ) countsat1++;
    else if (*flags==0 || (*flags & 0xF00)==0x800 ) countClean++;
    else if ((*flags & 0xF0)==0x80) countsat2++;
   
   
    
        
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
 
    h->GetXaxis()->Set(16384,0,0.05491+0.2428*16384); 
   //for  LABr3

	//scaling histo according to 'cf',
	// correction factor due to dead time 'dt'
	h->Scale(cf);
   	h->Draw();
	//h->SaveAs

    return ;

}


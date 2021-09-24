

void gaussbis(std::string filename)
{

// Opening the TFile
    TFile * file = new TFile(filename.c_str());
	std::ofstream t1("C3_172.txt");
	
    // Create a TIterator
    TH1F *h;
    TKey *key;
    TIter next( file->GetListOfKeys());

    // Looping over TH1F
    int a = 1; //for the name of tf1 saved
    while( (key = (TKey*) next() ) )
    {
     std::string tmp = std::to_string(a);
    char const *name = tmp.c_str();
    a++;
    std::cout<<"_______________________________________________________________________________________"<<std::endl;

        // Retrieve the histogram
    h = (TH1F*) file->Get(key->GetName());

        /* creating gaussian curve */
auto g8= new TF1 ("m8", "gaus", 170, 174.4);
        /* creating background line */
auto bg10= new TF1 ("b10", "pol1", 176,180.4);
  
  /* Fitting each gaus fn and adding them to the existing list of functions */
  /* the + sign adds the newly fitted fn to the existing fn. */
  
  h-> Fit (g8,"R+");
  h-> Fit (bg10,"R+");
 
  Double_t par[5]; /* parameter array */

   /* get parameters from the fit, it first fits & takes the parameter from there */
  
  g8-> GetParameters (&par[0]);
  bg10-> GetParameters (&par[3]);

  h->SaveAs(name);
  //std::cout<<h->GetBinWidth(1)<<std::endl;
  
  t1<<(g8->Integral(170, 174.4)-bg10->Integral(176,180.4))/0.2428<<"  "<<sqrt((g8->Integral(169.9, 174.4)+bg10->Integral(176,180.4))/0.2428)<<std::endl;
     
  gStyle->SetOptFit(1); // To print the parameters on the canvas
  sleep(1);
  
} 
  /* projecting the values of trkmm present in root file on the empty histogram,h */
 std::cout<<"_______________________________________________________________________________________"<<std::endl;

}


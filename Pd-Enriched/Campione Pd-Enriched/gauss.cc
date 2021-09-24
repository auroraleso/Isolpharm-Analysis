/*
Name of the programmer: Diksha Garg
Date: 16 May, 2017
Comment: This program loads a root file and then fits 2 gaus functions(pre-defined) to the trkmm parameter of the root file. 
*/

void gauss(std::string filename)
{
/*std::string namef, name;
std::cout<<"insert name file.root: ";
std::cin>>namef;
std::cout<<"insert name histo: ";
std::cin>>name;*/
// Opening the TFile
    TFile * file = new TFile(filename.c_str());
	std::ofstream t1("C1_172.txt");
	//std::ofstream t2("C1_342.txt");
	//std::ofstream t3("C1_311.txt");
	//std::ofstream t4("C1_575.txt");
	//std::ofstream t5("C1_602.txt");
	//std::ofstream t6("C1_647.txt");
	//std::ofstream t7("C1_632.txt");
    // Create a TIterator
    TH1F *h;
    TKey *key;
    TIter next( file->GetListOfKeys());

    // Looping over TH1F
    while( (key = (TKey*) next() ) )
    {
    std::cout<<"_______________________________________________________________________________________"<<std::endl;
        // Retrieve the histogram
        h = (TH1F*) file->Get(key->GetName());

        /* creating gaussian curves */
auto g1= new TF1 ("m1","gaus",85.7,90.1);
 auto g2= new TF1 ("m2","gaus",339.9,344.5);
 auto g3= new TF1 ("m3","gaus",309.3,313);
 auto g4= new TF1 ("m4","gaus",573.5,577.5);
 auto g5=  new TF1 ("m5","gaus",644.5,647.5);
 auto g6=   new TF1 ("m6","gaus",601,604.5);
auto g7=   new TF1 ("m7","gaus",630.25,635.5);
auto g8= new TF1 ("m8", "gaus", 169.9, 174.4);
  
  auto bg1= new TF1 ("bg1","pol1",80,84.4);
  auto bg2= new TF1 ("bg2","pol1",334,338.6);
  auto bg3= new TF1 ("bg3","pol1",298.3,302);
  auto bg4= new TF1 ("bg4","pol1",568,572);
  auto bg5= new TF1 ("bg5","pol1",643.8,644.77);
  auto bg6= new TF1 ("bg6","pol1",597.5,600.97);
  auto bg7= new TF1 ("bg7","pol1",646.8,648.75);
  auto bg8= new TF1 ("bg8","pol1",627,630);
  auto bg9= new TF1 ("b9","pol1",636,638);
  auto bg10= new TF1 ("b10", "pol1", 176,180.5);
  //  g3= new TF1 ("m3","gaus",12,17);

 // TF1 *f1= new TF1("double_gaus","gaus(0)+gaus(3)",4,16); /* define combined function*/

  /* Fitting each gaus fn and adding them to the existing list of functions */
  /* the + sign adds the newly fitted fn to the existing fn. */
  h-> Fit (g1,"R");
  h-> Fit (g2,"R+");
  h-> Fit (g3,"R+");
  h-> Fit (g4,"R+");
  h-> Fit (g5,"R+");
  h-> Fit (g6,"R+");
  h-> Fit (g7,"R+");
  h-> Fit (g8,"R+");
  h-> Fit (bg1,"R+");
  h-> Fit (bg2,"R+");
  h-> Fit (bg3,"R+");
  h-> Fit (bg4,"R+");
  h-> Fit (bg5,"R+");
  h-> Fit (bg6,"R+");
  h-> Fit (bg7,"R+");
  h-> Fit (bg8,"R+");
  h-> Fit (bg9,"R+");
  h-> Fit (bg10,"R+");
 // int x=h->GetXAxis()->GetNBinsX();
  //  h-> Fit (g3,"R+");

  Double_t par[44]; /* parameter array */

   /* get parameters from the fit, it first fits & takes the parameter from there */
  g1-> GetParameters (&par[0]);
  g2-> GetParameters (&par[3]);
  g3-> GetParameters (&par[6]);
  g4-> GetParameters (&par[9]);
  g5-> GetParameters (&par[12]);
  g6-> GetParameters (&par[15]);
  g7-> GetParameters (&par[18]);
  g8-> GetParameters (&par[21]);

  bg1-> GetParameters (&par[24]);
  bg2-> GetParameters (&par[26]);
  bg3-> GetParameters (&par[28]);
  bg4-> GetParameters (&par[30]);
  bg5-> GetParameters (&par[32]);
  bg6-> GetParameters (&par[34]);
  bg7-> GetParameters (&par[36]);
  bg8-> GetParameters (&par[38]);
  bg9-> GetParameters (&par[40]);
  bg10-> GetParameters (&par[42]);
 

  /* use the parameters on the combined function */
  //double_gaus-> SetParameters (par);

  //h->Fit("double_gaus","+","",4,16); /*fitting gaussian curve on the histogram */ 
  h->Draw("e1");
  std::cout<<h->GetBinWidth(1)<<std::endl;
  
  t1<<(g8->Integral(169.9, 174.4)-bg1->Integral(176,180.5))/0.2428<<"+-"<<sqrt((g8->Integral(169.9, 174.4)+bg1->Integral(176,180.5))/0.2428)<<endl;
  
  //t1<<(g1->Integral(85.7,90.1)-bg1->Integral(80,84.4))/0.2428<<"  " << sqrt((g1->Integral(85.7,90.1)+bg1->Integral(80,84.4))/0.2428)<<endl;
  
  
  //t2<<(g2->Integral(339.9,344.5)-bg2->Integral(334,338.6))/0.2428<<"  " << sqrt((g2->Integral(339.9,344.5)+bg2->Integral(334,338.6))/0.2428)<<endl;
  
  
 // t3<<(g3->Integral(309.3,313)-bg3->Integral(298.3,302))/0.2428<<"  " << sqrt((g3->Integral(309,313)+bg3->Integral(300,304))/0.2428)<<endl;
   
   
   //t4<<(g4->Integral(573.5,577.5)-bg4->Integral(568,572))/0.2428<<"  " << sqrt((g4->Integral(573.5,577.5)+bg4->Integral(568,572))/0.2428)<<endl;
   
//   t5<<(g5->Integral(644.5,647.5)-bg7->Integral(646.8,648.75)-bg5->Integral(643.8,644.77))/0.2428<<"  " << sqrt((g5->Integral(644.5,647.3)+bg5->Integral(643.8,644.8))/0.2428)<<endl;
   
 //  t6<<(g6->Integral(601,604.5)-bg6->Integral(597.5,600.97))/0.2428<<"  " << sqrt((g6->Integral(601,604.5)+bg6->Integral(597.5,600.97))/0.2428)<<endl;
  //    t7<<(g7->Integral(630.25,635.5)-bg8->Integral(627,630)-bg9->Integral(636,638))/0.2428<<"  " << sqrt((g7->Integral(630.25,635.5)+bg8->Integral(636,638)+bg9->Integral(627,630))/0.2428)<<endl;
   
  gStyle->SetOptFit(1); // To print the parameters on the canvas
  sleep(1);
  

    } 
    
 
  
  /* projecting the values of trkmm present in root file on the empty histogram,h */
 // T1-> Project ("h","abs(trkmm[0])","abs(trkmm[0])> 4 && abs(trkmm[0])<16"); 
 
 std::cout<<"_______________________________________________________________________________________"<<std::endl;

}


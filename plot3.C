int ic=0;
TCanvas* c1;
void gerrors2_( 
std::string name,
double exp_x[] , double exp_n4n_y[] , double exp_n6n_y[] , double exp_n4n_ey[] , double exp_n6n_ey[] , double sim_x[] , double sim_n4n[] , double sim_n6n[] 
) {
   //Draw two graphs with error bars
   // To see the output of this macro, click begin_html <a href="gif/gerrors2.gif">here</a>. end_html
   //Author: Rene Brun
   
//std::cout << ic << std::endl;
if ( ic == 0 ) {

   //TCanvas *c1 = new TCanvas("c1","SATIF-BNL",200,10,700,500);
   c1 = new TCanvas("c1","SATIF-BNL",200,10,700,500);
   c1->SetFillColor(0);

      // draw a frame to define the range
   //hr->SetTitle("Concrete 2.83GeV");
   c1->GetFrame()->SetBorderSize(12);
   c1->GetFrame()->SetFillColor(21);

   c1->Divide(2,2);
}
   ic++;

   c1->cd(ic)->SetGrid();
   c1->cd(ic)->SetLogy();

   TH1F *hr = c1->cd(ic)->DrawFrame(0,1.0e-35,400,1.0E-28);
   hr->GetXaxis()->CenterTitle();
   hr->GetXaxis()->SetTitleOffset(1.25);
   hr->GetXaxis()->SetTitleSize(0.042);
   hr->GetXaxis()->SetLabelSize(0.042);
   hr->GetYaxis()->CenterTitle();
   hr->GetYaxis()->SetTitleOffset(1.25);
   hr->GetYaxis()->SetTitleSize(0.042);
   hr->GetYaxis()->SetLabelSize(0.042);
   hr->SetTitle( name.c_str() );
   hr->SetXTitle("Depth [cm]");
   hr->SetYTitle("Reaction Rate [1/proton on target]");

   const Int_t n = 8;
   Double_t exp_ex[n] = { 0, 0, 0, 0, 0, 0, 0, 0 };

   // create first graph
   TGraphErrors *gr1 = new TGraphErrors(n,exp_x,exp_n4n_y,exp_ex,exp_n4n_ey);
   TGraphErrors *gr2 = new TGraphErrors(n,exp_x,exp_n6n_y,exp_ex,exp_n6n_ey);
   gr1->SetMarkerColor(kBlue);
   gr1->SetMarkerStyle(21);
   gr1->Draw("P");

   // create second graph
   gr2->SetMarkerColor(kRed);
   gr2->SetMarkerStyle(21);
   gr2->Draw("P");

   const Int_t sim_n = 14;
   Double_t sim_n4n_yy[sim_n];
   Double_t sim_n6n_yy[sim_n];
   double A= 1.99E-26;
   for ( int i=0; i<sim_n;i++) { 
     sim_n4n_yy[i]=A*sim_n4n[i]/(sim_x[i]+101.1);
     sim_n6n_yy[i]=A*sim_n6n[i]/(sim_x[i]+101.1);
   }

   TGraph *sim_gr = new TGraph(sim_n,sim_x,sim_n4n_yy);
   sim_gr->SetLineColor(kBlue);
   sim_gr->Draw("L");
   TGraph *sim_gr2 = new TGraph(sim_n,sim_x,sim_n6n_yy);
   sim_gr2->SetLineColor(kRed);
   sim_gr2->Draw("L");

   c1->Update();


   //CHI-Square
   double chi2_n4n = 0;
   double X_n4n = 0;
   double chi2_n6n = 0;
   double X_n6n = 0;
   double chi,chi2;
   double x;
   for ( int i = 0 ; i < 7 ; i++ ) { 
      chi = ( exp_n4n_y[i] - sim_n4n_yy[i*2] ) / exp_n4n_ey[i];
      //chi = ( exp_n4n_y[i] - sim_n4n_yy[i*2] ) / exp_n4n_y[i];
      chi2 = chi*chi;
      chi2_n4n += chi2;

      chi = ( exp_n6n_y[i] - sim_n6n_yy[i*2] ) / exp_n6n_ey[i];
      //chi = ( exp_n6n_y[i] - sim_n6n_yy[i*2] ) / exp_n6n_y[i];
      chi2 = chi*chi;
      chi2_n6n += chi2;

      x = fabs(log(sim_n4n_yy[i*2]/exp_n4n_y[i]));
      X_n4n += x;
      x = fabs(log(sim_n6n_yy[i*2]/exp_n6n_y[i]));
      X_n6n += x;
   }

   std::stringstream ss_n4n;
   std::string result_n4n="(n,4n) ";
   //ss_n4n << sqrt(chi2_n4n)/7;
   ss_n4n << chi2_n4n/7;
   result_n4n += ss_n4n.str();

   std::string result_n6n="(n,6n) ";
   std::stringstream ss_n6n;
   //ss_n6n << sqrt(chi2_n6n)/7;
   ss_n6n << chi2_n6n/7;
   result_n6n += ss_n6n.str();

   TLegend* leg = new TLegend(0.6,0.7,0.9,0.9);
   leg->AddEntry(gr1,"DATA (n,4n)","P");
   leg->AddEntry(gr2,"DATA (n,6n)","P");
   leg->AddEntry(sim_gr,"G4 (n,4n)","L");
   leg->AddEntry(sim_gr2,"G4 (n,6n)","L");
   leg->Draw();

   TPave* pave = new TPave(0.6,0.5,0.9,0.7,1,"brNDC");
   pave->Draw();

   TText* text = new TText();
   text->SetNDC();
   text->DrawText(0.66,0.63,"CHI2/N");
   text->DrawText(0.62,0.57,result_n4n.c_str());
   text->DrawText(0.62,0.51,result_n6n.c_str());
   /*
   text->DrawText(0.62,0.82,"(n,4n) DATA");
   text->DrawText(0.62,0.72,"(n,6n) DATA");
   text->DrawText(0.62,0.62,"(n,4n) G4");
   text->DrawText(0.62,0.52,"(n,6n) G4");
*/

   //legend = new TLegend( 0.6 , 0.5 , 0.9 , 0.9);
   //legend->Draw();
   //TLegendEntry* l1 = legend->AddEndtry(sim_gr,"SS","l");


   std::cout << name << " CHI2/N: " << chi2_n4n/7 << " " << chi2_n6n/7 << std::endl;
   //std::cout <<"X:    " << X_n4n << " " << X_n6n << std::endl;

   //std::string filename = name;
   //filename += ".gif";
   //c1->Print( filename.c_str() );
}

void plot( std::string PL ) {

   //Concrete
   Double_t exp_concrete_x[8]  = { 45.3, 91.1, 136.9, 182.7, 228.5, 274.3, 320.1, 365.9 };

   Double_t exp_concrete_low_n4n_y[8]  = { 2.01E-30, 3.69E-31, 7.42E-32, 1.97E-32, 5.73E-33, 1.49E-33, 4.53E-34, 1.78E-34};
   Double_t exp_concrete_low_n6n_y[8]  = { 1.02E-30, 2.41E-31, 5.07E-32, 1.47E-32, 3.68E-33, 1.08E-33, 3.3E-34, 9.11E-35};
   Double_t exp_concrete_low_n4n_ey[8] = { 2.28E-31, 4.04E-32, 8.12E-33, 2.19E-33, 6.4E-34, 1.71E-34, 1.35E-34, 1.04E-34};
   Double_t exp_concrete_low_n6n_ey[8] = { 1.43E-31, 3.3E-32, 7.04E-33, 2.05E-33, 5.29E-34, 1.57E-34, 5.41E-35, 2.29E-35};

   Double_t exp_concrete_high_n4n_y[8]  = { 1.11E-29, 1.84E-30, 3.9E-31, 1.09E-31, 3.16E-32, 8.33E-33, 3.14E-33, 1.06E-33};
   Double_t exp_concrete_high_n6n_y[8]  = { 8.38E-30, 1.14E-30, 2.73E-31, 8.25E-32, 1.99E-32, 6.23E-33, 1.87E-33, 4.68E-34};
   Double_t exp_concrete_high_n4n_ey[8] = { 1.23E-30, 2.02E-31, 4.32E-32, 1.22E-32, 3.51E-33, 9.36E-34, 4.76E-34, 1.84E-34};
   Double_t exp_concrete_high_n6n_ey[8] = { 1.17E-30, 1.56E-31, 3.74E-32, 1.15E-32, 2.78E-33, 8.91E-34, 2.65E-34, 6.83E-35};

   Double_t sim_concrete_x[14]  = { 45.3, 68.2, 91.1, 114, 136.9, 159.8, 182.7, 205.6, 228.5, 251.4, 274.3, 297.2, 320.1, 343 };

   //Iron
   Double_t exp_iron_x[8]  = { 23.4, 52.6, 81.8, 111, 140.2, 169.4, 198.6, 227.8 };

   Double_t exp_iron_low_n4n_y[8]  = { 2.49E-30, 2.8E-31, 5.5E-32, 7.61E-33, 1.2E-33, 3.91E-34, 8.15E-35 };
   Double_t exp_iron_low_n6n_y[8]  = { 1.29E-30, 1.51E-31, 2.89E-32, 4.33E-33, 7.44E-34, 1.58E-34, 4.57E-35, 4.26E-35 };
   Double_t exp_iron_low_n4n_ey[8] = { 2.81E-31, 3.03E-32, 6.13E-33, 8.39E-34, 1.54E-34, 8.2E-35, 5.24E-35 };
   Double_t exp_iron_low_n6n_ey[8] = { 1.8E-31, 2.06E-32, 4.13E-33, 6.12E-34, 1.09E-34, 2.46E-35, 1.5E-35, 1.26E-35 };

   Double_t exp_iron_high_n4n_y[8]  = { 1.36E-29, 1.29E-30, 2.51E-31, 3.55E-32, 5.32E-33, 1.43E-33, 2.93E-34, 9.88E-35 };
   Double_t exp_iron_high_n6n_y[8]  = { 9.08E-30, 7.18E-31, 1.7E-31, 2.47E-32, 3.4E-33, 8.76E-34, 1.67E-34, 5.97E-35 };
   Double_t exp_iron_high_n4n_ey[8] = { 1.50E-30, 1.43E-31, 2.77E-32, 4.64E-33, 6E-34, 1.73E-34, 4.34E-35, 4.66E-35 };
   Double_t exp_iron_high_n6n_ey[8] = { 1.25E-30, 9.82E-32, 2.33E-32, 3.47E-33, 5.03E-34, 1.32E-34, 3.04E-35, 1.4E-35 };

   Double_t sim_iron_x[14]  = { 23.4, 38, 52.6, 67.2, 81.8, 96.4, 111, 125.6, 140.2, 154.8, 169.4, 184, 198.6, 213.2 };


   std::string filename;
   filename = "result_flux";
   filename += "_";
   filename += PL;
   ifstream data ( filename.c_str() );
   std::string sdummy;
   double ddummy;
   double dn4n;
   double dn6n;
   int idummy;
   double sim_n4n[14];
   double sim_n6n[14];

   std::string name;
   //Concrete 2.83GeV
   for ( int i = 0 ; i < 14 ; i++ ) {
      data >> sdummy >> idummy >> dn4n >> dn6n >> ddummy >> ddummy;
      sim_n4n[i]=dn4n;
      sim_n6n[i]=dn6n;
   }
   name="Concrete_2.83GeV";
   name+=" ";
   name+=PL;
   gerrors2_( name, exp_concrete_x , exp_concrete_low_n4n_y, exp_concrete_low_n6n_y, exp_concrete_low_n4n_ey, exp_concrete_low_n6n_ey, sim_concrete_x, sim_n4n , sim_n6n );

   //Iron 2.83GeV
   for ( int i = 0 ; i < 14 ; i++ ) {
      data >> sdummy >> idummy >> dn4n >> dn6n >> ddummy >> ddummy;
      sim_n4n[i]=dn4n;
      sim_n6n[i]=dn6n;
   }
   name="Iron_2.83GeV";
   name+=" ";
   name+=PL;
   gerrors2_( name, exp_iron_x , exp_iron_low_n4n_y, exp_iron_low_n6n_y, exp_iron_low_n4n_ey, exp_iron_low_n6n_ey,sim_iron_x, sim_n4n , sim_n6n );

   //Concrete 24GeV
   for ( int i = 0 ; i < 14 ; i++ ) {
      data >> sdummy >> idummy >> dn4n >> dn6n >> ddummy >> ddummy;
      sim_n4n[i]=dn4n;
      sim_n6n[i]=dn6n;
   }
   name="Concrete_24GeV";
   name+=" ";
   name+=PL;
   gerrors2_(name,exp_concrete_x , exp_concrete_high_n4n_y, exp_concrete_high_n6n_y, exp_concrete_high_n4n_ey, exp_concrete_high_n6n_ey, sim_concrete_x, sim_n4n , sim_n6n );

   //Iron 24GeV
   for ( int i = 0 ; i < 14 ; i++ ) {
      data >> sdummy >> idummy >> dn4n >> dn6n >> ddummy >> ddummy;
      sim_n4n[i]=dn4n;
      sim_n6n[i]=dn6n;
   }
   name="Iron_24GeV";
   name+=" ";
   name+=PL;
   gerrors2_(name,exp_iron_x , exp_iron_high_n4n_y, exp_iron_high_n6n_y, exp_iron_high_n4n_ey, exp_iron_high_n6n_ey,sim_iron_x, sim_n4n , sim_n6n );

   std::string plot_name =PL;
   plot_name += ".gif";
   //plot_name += ".eps";
   gROOT->GetListOfCanvases()->Print(plot_name.c_str(), 1);
}


void plot3( std::string PL ) {
   plot( PL );
   exit(EXIT_SUCCESS);
   //abort();
}

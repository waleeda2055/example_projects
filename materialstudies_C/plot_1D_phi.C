void plot_1D_phi()
{

gROOT->Reset();

  // use plain black on white colors
  gStyle->SetFrameBorderMode(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetStatColor(0);
  //gStyle->SetFillColor(0);

  // set the paper & margin sizes
  gStyle->SetPaperSize(20,26);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.12);
  
  
  // use large Times-Roman fonts
//  gStyle->SetTextFont(132);
//  gStyle->SetTextSize(0.08);
//  gStyle->SetLabelFont(132,"x");
//  gStyle->SetLabelFont(132,"y");
//  gStyle->SetLabelFont(13,"z");
  gStyle->SetLabelSize(0.04,"x");
  gStyle->SetTitleSize(0.05,"x");
  gStyle->SetLabelSize(0.04,"y");
  gStyle->SetTitleSize(0.05,"y");
  gStyle->SetLabelSize(0.04,"z");
  gStyle->SetTitleSize(0.05,"z");
  
  // use bold lines and markers
  gStyle->SetMarkerStyle(20);
//  //gStyle->SetMarkerSize(0.5);
//  gStyle->SetHistLineWidth((Width_t)1.85);
  gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
  
  // get rid of X error bars and y error bar caps
  gStyle->SetErrorX(0.001);
  
  // do not display any of the standard histogram decorations
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);



Int_t ch1;
Char_t path1[50];
    strcpy(path1, "cumulative/build-2015-06-08/");

//  cout << "Path is..." << path1 << "..."<< endl;

  Char_t filename[50];
  Char_t filesuffix[50];
  Char_t filesuffix_t[50];
  Char_t filesuffix_p[50];

  cout << "Name of .root file?: " << endl; 
  cin >> filesuffix;
  strcpy(filesuffix_t, filesuffix);
  strcpy(filesuffix_p, filesuffix);
  strcat(filesuffix_t, "_theta");
  strcat(filesuffix_p, "_phi");

  strcpy(filename, "MaterialScan_");
  strcat(filename, filesuffix);
  Char_t path2[100];
  strcpy(path2,path1); 
  strcat(path2,filename); 
  strcat(path2,".root");
  TFile *file = TFile::Open(path2);

  Char_t path3[50] = "Spherical";
  file->cd(path3);
  Char_t path4[100];
  strcpy(path4,path3); 
  strcat(path4,"/All_Regions_x0");

  cout << "filename is" << path2 << endl;
// cout << "path4=" << path4 << endl;

  // file->ls();
//  All_Regions_x0->Draw("colz");


  TH2D *h1 = (TH2D*)file->Get(path4);
  Int_t N = h1->GetSize(); 

// cout << N << endl;

  if (N != 252004) {
  cout << "Problem with histo size! " << endl;
} else {
  cout << "Using histo size 500x500" << endl;
}

  Int_t ntheta = 500;
  Int_t nphi = 500;


//   h1->Draw("colz");

  // full interval in theta and phi; do not change, plots can be
  // adjusted later with the filled histogram
  Double_t mintheta = 0; 
  Double_t maxtheta = 180.0; 
  Double_t minphi = 0.0;
  Double_t maxphi = 360.0;
  Int_t k = 0;

  // 1D histogram to store L/Lrad as a function of theta and phi:
  TH1F *h40 = new TH1F(filesuffix_t,filesuffix_t,ntheta,mintheta,maxtheta);
  TH1F *h50 = new TH1F(filesuffix_p,filesuffix_p,nphi,minphi,maxphi);

 // read the content of the 2D histo produced by MaterialScan.cc
  Double_t x[N]; 
  Double_t y[N]; 
  Double_t z[N];

//  cout << "here" << endl;

  for(Int_t j = 1; j <=nphi; j++) { 
//    for(Int_t i = 50; i <= 416; i++) {
    for(Int_t i = 1; i <= 500; i++) {
      Int_t a = h1->GetBin(i,j);
      Double_t radl = h1->GetBinContent(a);
        x[k]= (Double_t) mintheta+((maxtheta-mintheta)/ntheta)*(i-0.5);
        y[k]= (Double_t) minphi+((maxphi-minphi)/nphi)*(j-0.5);

// cout << i << j<<  x[k] << endl;;
      z[k] = (Double_t) radl*1.0;
      // fill the new histogram L/Lrad vs theta for all values of phi and vis versa
      h40->Fill(x[k],z[k]/nphi);
      h50->Fill(y[k],z[k]/ntheta);
      k = k + 1;
      }
   }

   // write the new histogram to file:
   TFile f("output_phi.root","update");

   h40->Write();
   h50->Write();

}


void compare_1D() {

// GraphicsSetup();

  // use bold lines and markers
  gStyle->SetMarkerStyle(20);
//  //gStyle->SetMarkerSize(0.5);
//  gStyle->SetHistLineWidth((Width_t)1.85);
  gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // get rid of X error bars and y error bar caps
  gStyle->SetErrorX(0.001);

 // do not display any of the standard histogram decorations
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  TFile *f1 = TFile::Open("/gs/project/belle2-mcgill/users/waleeda/rel_06/output.root");
  //f1->cd("Spherical");
  h1 = (TH1F*)gDirectory->Get("SVD")->Clone();
  //hdiff = (TH2D*)gDirectory->Get("All_Regions_x0")->Clone();

  TFile *f2 = TFile::Open("/gs/project/belle2-mcgill/users/waleeda/rel_08/output.root");
  //f2->cd("Spherical");
  h2 = (TH1F*)gDirectory->Get("SVD")->Clone();

  h1->SetMaximum(.1);
 // h1->SetMinimum(-0.001);
  h2->SetMaximum(.1);
 // h2->SetMinimum(-0.001);

  //hdiff->Add(h2,-1);

  //hdiff->SetMaximum(5.0);
  //hdiff->SetMinimum(-5.0);
  h1->Draw("hist");
  h1->SetLineColor(kRed);
  h2->Draw("same hist");	
  h2->SetLineColor(kBlue);

   TLegend*leg = new TLegend(0.7,0.8,0.9,0.9);
   leg->AddEntry(h1,"rel 06","l");
   leg->AddEntry(h2,"rel 08","l");
   leg->Draw();
	

//   h1->Draw("hcolz");
//   h2->Draw("hcolz");

//  gPad->SetLogz(1);
//  hdiff->Draw("hcolz");


//  TCanvas * theCanvas  = (TCanvas *)gROOT->GetListOfCanvases()->FindObject("c1");
 




}

void GraphicsSetup()  {

  // use plain black on white colors
//  gStyle->SetFrameBorderMode(0);
//  gStyle->SetCanvasBorderMode(0);
//  gStyle->SetPadBorderMode(0);
//  gStyle->SetPadColor(0);
//  gStyle->SetCanvasColor(0);
//  gStyle->SetStatColor(0);
//  //gStyle->SetFillColor(0);
  
  // set the paper & margin sizes
//  gStyle->SetPaperSize(20,26);
//  gStyle->SetPadTopMargin(0.05);
//  gStyle->SetPadRightMargin(0.05);
//  gStyle->SetPadBottomMargin(0.16);
//  gStyle->SetPadLeftMargin(0.12);
  
  // use large Times-Roman fonts
// //  gStyle->SetTextFont(132);
// //  gStyle->SetTextSize(0.08);
// //  gStyle->SetLabelFont(132,"x");
// //  gStyle->SetLabelFont(132,"y");
// //  gStyle->SetLabelFont(13,"z");
//  gStyle->SetLabelSize(0.04,"x");
//  gStyle->SetTitleSize(0.05,"x");
//  gStyle->SetLabelSize(0.04,"y");
//  gStyle->SetTitleSize(0.05,"y");
//  gStyle->SetLabelSize(0.04,"z");
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

// gPad->SetTheta(30); // default is 30
// gPad->SetPhi(30); // default is 30

}


#include <fstream>
using namespace std ;

// This script is used to create a map of a plane in a test beam telescope. The input is a TTree including 
// MSC projected scattering angle distributions and reconstruction errors.
int ConvertMaterialScan()
{
	gSystem->Load("libProof.so");
	gSystem->Load("libTreePlayer.so");

	gROOT->Reset(); 
	gROOT->SetStyle("Plain"); 

	// display mode
	gStyle->SetPalette(1);
	gStyle->SetOptStat(11111111);

	// TString for the input root file name
	TString filename,histoname,range;
        TString image_type="_PXD_L1_zoom2";
        TString align_type="";
	filename="MaterialScan"+image_type;

	TFile *X0file = new TFile(filename+".root", "READ");

	// Get the X0 map
	TH2F *hX0map = (TH2F*)X0file->Get("Planar/All_Materials_x0");

	// Get the number of row and column bins
	int nbinsx=hX0map->GetXaxis()->GetLast();
	cout<<nbinsx<<endl;
	int nbinsy=hX0map->GetYaxis()->GetLast();
	cout<<nbinsy<<endl;

	// Get the u and v limits of the map
	double umin=10*hX0map->GetXaxis()->GetXmin();
	double umax=10*hX0map->GetXaxis()->GetXmax();
	double vmin=10*hX0map->GetYaxis()->GetXmin();
	double vmax=10*hX0map->GetYaxis()->GetXmax();

	// print out general information about the map setup
	cout<<"number of column bins: "<<nbinsx<<endl;
	cout<<"umin: "<<umin<<endl;
	cout<<"umax: "<<umax<<endl;
	cout<<"number of row bins: "<<nbinsy<<endl;
	cout<<"vmin: "<<vmin<<endl;
	cout<<"vmax: "<<vmax<<endl;

	// Recreation of the X0 map, including the areas of the profiles marked by boxes
	TH2F* map = new TH2F("map","",nbinsx,umin,umax,nbinsy,vmin,vmax);
        map->GetXaxis()->SetTitle("u[mm]");
        map->GetYaxis()->SetTitle("v[mm]");
        map->GetZaxis()->SetTitle("X/X0[%]");
	//map->SetMaximum(3.0);
	//map->SetMinimum(2.0);
	gStyle->SetOptStat(0);

	// Recreate image
	for(int i=0; i<=nbinsx; i++)
	{
		double X0=0.0;
		double X0err=0.0;
		double theta1mean=0.0;
		double theta2mean=0.0;		
		
		for(int j=0;j<=nbinsy;j++)
		{
			X0=hX0map->GetBinContent(i,j);
			map->SetBinContent(i,j,100.0*X0);	
		}
	}

	// Open new one to store the profile histograms and the map
        TString resultsfilename=filename+"_converted.root";
	TFile *resultsfile = new TFile(resultsfilename, "RECREATE");

	// Canvas for drawing the X0 image
	TCanvas* c1=new TCanvas("c1","c1",1400, 800);
	c1->cd();
      	map->GetZaxis()->SetLabelSize(0.02);
	map->Draw("colz");
	map->Write();

	// Close root files
	X0file->Close();
        resultsfile->Close();

	return 0;
}



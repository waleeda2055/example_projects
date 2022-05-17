#include <fstream>
using namespace std ;

// This script is used to create a map of a plane in a test beam telescope. The input is a TTree including 
// MSC projected scattering angle distributions and reconstruction errors.
int CompareProfiles()
{
	gSystem->Load("libProof.so");
	gSystem->Load("libTreePlayer.so");

	gROOT->Reset(); 
	gROOT->SetStyle("Plain"); 

	// display mode
	gStyle->SetPalette(1);
	gStyle->SetOptStat(11111111);

        // Number of input files (should always be 2)
        const int nfiles=2;

        // Number of profiles
        const int nprofiles=2;

	// TString for the input root file names
	TString filename[nfiles];

        // Material Scan file name, change if needed
	filename[0]="MaterialScan_PXD_L1_zoom_converted";

        // X0 image file name, change if needed
	filename[1]="X0_merge-image-50mum-minus15MeV1mm-lambda1172_onlyimage";

        // root input files
        TFile *X0file[nfiles];

        // x0 images 2D histos
	TH2F *hX0map[nfiles];

        // Folder structures
        TString folderstructure[nfiles];
        folderstructure[0]="map";
        folderstructure[1]="map";

	// Definition of the u,v and row,col limits of the two profiles
	int binxmin[nfiles][nprofiles];
	int binxmax[nfiles][nprofiles];
	int binymin[nfiles][nprofiles];
	int binymax[nfiles][nprofiles];

	// Set column and row limits if the first profiles (mat scan) manually
        // You can check whether you have the correct profile area in the *_box.pdf files,
        // which are created by this script
        // ...[0][0] defines the u profile of the Material Scan file
        // ...[0][1] defines the v profile of the Material Scan file

	binxmin[0][0]=41;
	binxmax[0][0]=105;
	binymin[0][0]=50;
	binymax[0][0]=69;

	binxmin[0][1]=83;
	binxmax[0][1]=89;
	binymin[0][1]=161;
	binymax[0][1]=261;

	// Set column and row limits if the second profiles (x0 measurement image) manually
        // You can check whether you have the correct profile area in the *_box.pdf files,
        // which are created by this script
        // ...[1][0] defines the u profile of the X0 measurement file
        // ...[1][1] defines the v profile of the X0 measurement file

	binxmin[1][0]=3;
	binxmax[1][0]=67;
	binymin[1][0]=17;
	binymax[1][0]=40;

	binxmin[1][1]=48;
	binxmax[1][1]=52;
	binymin[1][1]=40;
	binymax[1][1]=140;

	double umin[nfiles][nprofiles];
	double umax[nfiles][nprofiles];
	double vmin[nfiles][nprofiles];
	double vmax[nfiles][nprofiles];

        // Max x0 value of the images
        double MaxX0value=2.5;

        // Min x0 value of the images
        double MinX0value=0.0;


        // In this for loop two profile root files are created, which contain the 2d images and 1D profiles
        // Also for checking whether the two profiles are compatible pdf files are created, where the position of the 
        // profiles can be checked
        for(int i=0;i<nfiles;i++) {

                // Open file
                X0file[i] = new TFile(filename[i]+".root", "READ");

                // Get x0 image
                hX0map[i] = (TH2F*)X0file[i]->Get(folderstructure[i]);

	        // Open new one to store the profile histograms and the map
                TString resultsfilename="profiles_"+filename[i]+".root";
	        TFile *resultsfile = new TFile(resultsfilename, "RECREATE");

	        // Create x0 image canvas
	        TCanvas* c=new TCanvas("c","c",1400, 800);
                gStyle->SetOptStat(0);
	        c->cd();
              	hX0map[i]->GetZaxis()->SetLabelSize(0.02);
              	hX0map[i]->SetMaximum(MaxX0value);
              	hX0map[i]->SetMinimum(MinX0value);
	        hX0map[i]->Draw("colz");
	        hX0map[i]->Write();

	        TBox* box;
	        TString description;

                for(int iprof=0;iprof<nprofiles;iprof++)
                {

		        // Calculate the u and v limit values from the column and row limit values
		        umin[i][iprof]=hX0map[i]->GetXaxis()->GetBinLowEdge(binxmin[i][iprof]);
		        umax[i][iprof]=hX0map[i]->GetXaxis()->GetBinUpEdge(binxmax[i][iprof]);
		        vmin[i][iprof]=hX0map[i]->GetYaxis()->GetBinLowEdge(binymin[i][iprof]);
		        vmax[i][iprof]=hX0map[i]->GetYaxis()->GetBinUpEdge(binymax[i][iprof]);

		        // Box to indicate the profile
		        box=new TBox(umin[i][iprof], vmin[i][iprof], umax[i][iprof], vmax[i][iprof]); 

		        box->SetFillStyle(0);
		        box->SetLineColor(1);
		        box->SetLineWidth(2);
		        box->Draw();
                }

                TString imagename=filename[i]+"_box.pdf";
	        c->SaveAs(imagename);


	        // Definition of u profile
	        TH1F* uprofile= new TH1F("uprofile", "", binxmax[i][0]-binxmin[i][0]+1, 0.0, umax[i][0]-umin[i][0]);
                uprofile->GetXaxis()->SetTitle("length[mm]");
                uprofile->GetYaxis()->SetTitle("X/X0[%]");
	
	        // First u Profile (x/x0 profile)	
	        for(int j=binxmin[i][0]; j<=binxmax[i][0]; j++)
	        {
		        double X0=0.0;
		        double X0err=0.0;		
		
		        for(int l=binymin[i][0];l<=binymax[i][0];l++)
		        {
			        X0+=hX0map[i]->GetBinContent(j,l)/(1.0*(binymax[i][0]-binymin[i][0])+1.0);
			        X0err+=pow(hX0map[i]->GetBinError(j,l)/(1.0*(binymax[i][0]-binymin[i][0])+1.0),2);
                        }	

		        uprofile->SetBinContent(j-binxmin[i][0],X0);

                        if(i==0) uprofile->SetBinError(j-binxmin[i][0]+1,0.0);
                        else uprofile->SetBinError(j-binxmin[i][0]+1,sqrt(X0err));
	        }

	        // Canvas for drawing this first u profile

                TCanvas* c2=new TCanvas("c2","c2",1400,800);
                gStyle->SetOptStat(0);
                c2->cd();
                uprofile->Draw("HE");
                uprofile->Write();
                TString profilename=filename[i]+"uprofile.pdf";

                c2->SaveAs(profilename);


	        // Definition of v profile
	        TH1F* vprofile= new TH1F("vprofile", "", binymax[i][1]-binymin[i][1]+1, 0.0, vmax[i][1]-vmin[i][1]);
                vprofile->GetXaxis()->SetTitle("length[mm]");
                vprofile->GetYaxis()->SetTitle("X/X0[%]");
	
	        // First v Profile (x/x0 profile)	
	        for(int j=binymin[i][1]; j<=binymax[i][1]; j++)
	        {
		        double X0=0.0;
		        double X0err=0.0;		
		
		        for(int l=binxmin[i][1];l<=binxmax[i][1];l++)
		        {
			        X0+=hX0map[i]->GetBinContent(l,j)/(1.0*(binxmax[i][1]-binxmin[i][1])+1.0);
			        X0err+=pow(hX0map[i]->GetBinError(l,j)/(1.0*(binxmax[i][1]-binxmin[i][1])+1.0),2);
                        }	

		        vprofile->SetBinContent(j-binymin[i][1],X0);

                        if(i==0) vprofile->SetBinError(j-binymin[i][1]+1,0.0);
                        else vprofile->SetBinError(j-binymin[i][1]+1,sqrt(X0err));
	        }

	        // Canvas for drawing this first u profile

                TCanvas* c3=new TCanvas("c3","c3",1400,800);
                gStyle->SetOptStat(0);
                c3->cd();
                vprofile->Draw("HE");
                vprofile->Write();
                profilename=filename[i]+"vprofile.pdf";

                c3->SaveAs(profilename);

                resultsfile->Close();

        } // for loop

        // open root files and draw both profiles in the same histogram

	// TString for the input root file name
	TString filename1;
	filename1="profiles_"+filename[0]+".root";
        TString legend1="Material scan";

	TFile *profilesfile1 = new TFile(filename1, "READ");

	TString filename2;
	filename2="profiles_"+filename[1]+".root";
        TString legend2="X0 measurement";

	TFile *profilesfile2 = new TFile(filename2, "READ");

	// Get the first X0 map u profile
	TH1F *h1 = (TH1F*)profilesfile1->Get("uprofile");

	// Get the second X0 map u profile
	TH1F *h2 = (TH1F*)profilesfile2->Get("uprofile");

	// Get the first X0 map v profile
	TH1F *h3 = (TH1F*)profilesfile1->Get("vprofile");

	// Get the second X0 map v profile
	TH1F *h4 = (TH1F*)profilesfile2->Get("vprofile");

        // Canvas with comparison of two x/x0 u profiles
	TCanvas* c3=new TCanvas("c3","c3",1400, 800);
        gStyle->SetOptStat(0);
	c3->cd();
        h2->SetLineColor(2);
	h2->SetMaximum(1.0);
	h2->Draw("HE");
        h1->Draw("HESame");

        leg = new TLegend(0.6,0.7,0.9,0.9);
        leg->AddEntry(h1,legend1,"le");
        leg->AddEntry(h2,legend2,"le");
        leg->Draw();

	c3->SaveAs("Comparison_uprofile.pdf");


	// Canvas with comparison of two x/x0 v profiles
	TCanvas* c4=new TCanvas("c4","c4",1400, 800);
        gStyle->SetOptStat(0);
	c4->cd();
        h4->SetLineColor(2);
	h4->Draw("HE");
        h3->Draw("HESame");

        leg2 = new TLegend(0.6,0.7,0.9,0.9);
        leg2->AddEntry(h3,legend1,"le");
        leg2->AddEntry(h4,legend2,"le");
        leg2->Draw();

	c4->SaveAs("Comparison_vprofile.pdf");


	return 0;
}



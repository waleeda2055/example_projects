#include "TH1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <TLegend.h>
#include <TFile.h>
#include "TColor.h"
#include <TLatex.h>


void plot_wal_dR_mumu(){
TCanvas* c1 = new TCanvas("c1","c1",800,600);
    c1->cd();
    SetAtlasStyle();
    TFile *f0 = TFile::Open("/homes/chachapoya/waleeda/Documents/Higgs/scp/aug6/run3/hist-Hplus_aug6b.root");
    TFile *f1 = TFile::Open("/homes/chachapoya/waleeda/Documents/Higgs/scp/aug6/run4/hist-Hplus_aug6c.root");
    TFile *f2 = TFile::Open("/homes/chachapoya/waleeda/Documents/Higgs/scp/aug6/run5/hist-Hplus_aug6d.root");
    //TFile *f3 = TFile::Open("/Users/shreya/h1_h3_rootfiles/hist-ttbar_h3_60_ljets.root");
    //TFile *f4 = TFile::Open("/Users/shreya/h1_h3_rootfiles/hist-ttbar_h1_80_ljets.root");
    //TFile *f5 = TFile::Open("/Users/shreya/h1_h3_rootfiles/hist-ttbar_h3_80_ljets.root");
    

TH1F * h_10 = (TH1F*)f0->Get("dR_mumu");
TH1F * h_20 = (TH1F*)f1->Get("dR_mumu");
TH1F * h_30 = (TH1F*)f2->Get("dR_mumu");
//TH1F * h_40 = (TH1F*)f3->Get("n_ele");
//TH1F * h_60 = (TH1F*)f4->Get("n_ele");
//TH1F * h_80 = (TH1F*)f5->Get("n_ele");


        h_10->GetXaxis()->SetTitle("dR_{'mumu'}");
        h_10->GetYaxis()->SetTitle("Entries");   
        h_10->GetYaxis()->SetRangeUser(0,350);
        	//h_10->Scale(1./h_10->Integral());
		//h_20->Scale(1./h_20->Integral());
		//h_30->Scale(1./h_30->Integral());
		//h_40->Scale(1./h_40->Integral());
		//h_60->Scale(1./h_60->Integral());
		//h_80->Scale(1./h_80->Integral());
	

	       
		h_10->SetTitle("");

		h_10->Draw("hist");
		h_20->Draw("hist same");

		h_30->Draw("hist same");
		//h_40->Draw("hist same");
	       
		//h_60->Draw("hist same");
		//h_80->Draw("hist same");
	

		h_10->SetLineColor(kCyan); 
		h_20->SetLineColor(kRed+1);
		h_30->SetLineColor(kOrange);
		//h_40->SetLineColor(kGreen);
		//h_60->SetLineColor(kBlue);
		//h_80->SetLineColor(kMagenta);
		

		TLegend *leg = new TLegend(0.8,0.5,0.6,0.75);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.04);
		leg->AddEntry(h_10,"m_{a} = 10 GeV","l");
		leg->AddEntry(h_20,"m_{a} = 9 GeV","l");
		leg->AddEntry(h_30,"m_{a} = 8 GeV","l");
		//leg->AddEntry(h_40,"m_{a} = 60 GeV, h3_odd","l");
		//leg->AddEntry(h_60,"m_{a} = 80 GeV, h1_odd","l");
		//leg->AddEntry(h_80,"m_{a} = 80 GeV, h3_odd","l");
		

		leg->Draw();
              
                //Can change the postion of the label by adjusting the values
	        ATLASLabel(0.52,0.8,"Simulation Internal");
		
		TLatex latex;
		latex.SetNDC();
		latex.SetTextColor(kBlack);
		latex.SetTextSize(0.04);
		latex.SetTextAlign(13);  //align at top
	      //  latex.DrawLatex(.2,.8, title);
		//c1->Update();
		c1->SaveAs("dR_mumu.png");
	       
	    }




#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TTree.h>
#include <TString.h>
#include <TBranch.h>
#include <TObjArray.h>
#include <TDirectory.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TSystem.h>
#include <TEnv.h>

#include <vector>
#include <set>
#include <iostream>
#include <string>

using namespace std;

void drawDiff(TH1* newHisto, TH1* oldHisto, TH1* diff, TString branchname){

  TCanvas *cc = new TCanvas(branchname, branchname, 1200, 600);
  cc->Divide(3,1);
  // diff->SetXTitle(branchname);
  cc->cd(1);
  newHisto->SetXTitle(branchname+" (new)");
  newHisto->Draw();
  cc->cd(2);
  oldHisto->SetXTitle(branchname+" (old)");
  oldHisto->Draw();
  cc->cd(3);
  diff->SetXTitle(branchname+" (diff)");
  diff->Draw();
  cc->SaveAs(TString("compareHaantuples/")+branchname+".png");
  cc->SaveAs(TString("compareHaantuples/")+branchname+".pdf");

}

void drawDiffSame(TH1* newHisto, TH1* oldHisto, TH1* diff, TString branchname){

  TCanvas *cc = new TCanvas(branchname, branchname, 1000, 600);
  cc->Divide(2,1);
  cc->cd(1)->SetLogy();
  newHisto->SetXTitle(branchname);
  newHisto->SetLineColor(1);
  newHisto->Draw();
  oldHisto->SetLineColor(2);
  oldHisto->Draw("sames");

  TLegend *leg = new TLegend(0.65,0.8,0.8,0.9);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(newHisto,"new","l");
  leg->AddEntry(oldHisto,"old","l");
  leg->Draw();

  cc->cd(2);
  diff->SetXTitle(branchname+" (diff)");
  diff->Draw("hist");
  cc->SaveAs(TString("compareHaantuples/")+branchname+".png");
  cc->SaveAs(TString("compareHaantuples/")+branchname+".pdf");

}

void dumpSet(set<TString> tset){

  for(auto a : tset) cout << a << endl;

}

void compareTrees(TTree* oldTree, TTree* newTree, bool useSameBinning){

  cout << "############ comparing " << oldTree->GetName() <<" tree #################" << endl;

  int oldEntries = oldTree->GetEntries();
  int newEntries = newTree->GetEntries();

  if(oldEntries == newEntries){
    cout << " old and new trees have the same number of entries = " << newEntries << endl;
  }
  else{
    cout << " different number of entries new=" <<newEntries << " old=" << oldEntries << endl;
    cout << "---> all branches will be different " << endl;
  }

  set<TString> addedBranchList;
  set<TString> removedBranchList;
  set<TString> diffBranchList;
  set<TString> okBranchList;
  

  TObjArray *newBranchList = newTree->GetListOfBranches();

  for(int i=0; i<newBranchList->GetSize(); ++i){

    TBranch *newBranch = dynamic_cast<TBranch*>(newBranchList->At(i));
    TString branchName = newBranch->GetName();

    if(oldTree->FindBranch(branchName)){

      /// if at some points we want to add cuts
      TString cut ="";

      //// if at some point we want to compare specific branchs
      //if(!branchName.BeginsWith(""))continue;
      //if(!branchName.EndsWith(""))continue;
      //if(!branchName.EqualTo(""))continue;
     

      TString newHistoName= branchName+"_new";
      TString oldHistoNameSB= branchName+"_oldSB";
      TString oldHistoName= branchName+"_old";

      newTree->Draw(branchName+">>"+newHistoName,cut,"goff");
      void *newHistoTmp=0;
      gDirectory->GetObject(newHistoName,newHistoTmp);
      TH1 *newHisto  = static_cast<TH1*>(newHistoTmp);

      TH1* oldHistoSB = (TH1*)newHisto->Clone(oldHistoNameSB);
      oldTree->Draw(branchName+">>"+oldHistoNameSB,cut,"goff");

      oldTree->Draw(branchName+">>"+oldHistoName,cut,"goff");
      void *oldHistoTmp=0;
      gDirectory->GetObject(oldHistoName,oldHistoTmp);
      TH1 *oldHisto  = static_cast<TH1*>(oldHistoTmp);

      TH1* diff = (TH1*)oldHistoSB->Clone(branchName+"_difference");
      diff->Scale(-1);
      diff->Add(newHisto);


      if(useSameBinning){
      }
	
      bool OK = true;
      for(int i=0; i<diff->GetNbinsX()+2; ++i){
	if(diff->GetBinContent(i)){
	  OK=false;
	  break;
	}
      }

      if(OK ){
	okBranchList.insert(branchName);
	//drawDiffSame(newHisto, oldHistoSB, diff, branchName);
	continue;
      }

      diffBranchList.insert(branchName);

      if(useSameBinning){
	drawDiffSame(newHisto, oldHistoSB, diff, branchName);
      }
      else{
	drawDiff(newHisto, oldHisto, diff, branchName);
      }

    }
    else{
      addedBranchList.insert(branchName);
    }

  }


  TObjArray *oldBranchList = oldTree->GetListOfBranches();
  for(int i=0; i<oldBranchList->GetSize(); ++i){
    TBranch * oldBranch = dynamic_cast<TBranch*>(oldBranchList->At(i));
    TString branchName = oldBranch->GetName();
    if(!newTree->FindBranch(branchName)){
      removedBranchList.insert(branchName);
    }
  }

  cout << "List of OK branches:" << endl;
  cout << "--------------------" << endl;
  dumpSet(okBranchList);
  cout <<"----> " << okBranchList.size() << " branches are OK" << endl;

  cout << "List of add branches:" << endl;
  cout << "---------------------" << endl;
  dumpSet(addedBranchList);
  cout <<"----> " << addedBranchList.size() << " branches are added" << endl;

  cout << "List of removed branches:" << endl;
  cout << "-------------------------" << endl;
  dumpSet(removedBranchList);
  cout <<"----> " << removedBranchList.size() << " branches are removed" << endl;


  cout << "List of diff branches:" << endl;
  cout << "----------------------" << endl;
  dumpSet(diffBranchList);
  cout <<"----> " << diffBranchList.size() << " branches are diff" << endl;

  cout << "############ end comparing " << oldTree->GetName() <<" tree #################" << endl;

}

void compareTTHOutFile(TString oldFileName, 
		       TString newFileName, 
		       TString treeName1,
		       TString treeName2,
		       TString outdir="compareHaantuples", 
		       bool drawsame=true){

  gSystem->Exec("rm -rf "+outdir);
  gSystem->Exec("mkdir -p "+outdir);

  //gEnv->SetValue("Root.ErrorIgnoreLevel=Info");
  gEnv->SetValue("Root.ErrorIgnoreLevel=Warning");

  gStyle->SetOptStat(111111);

  TChain *ch2 = new TChain(treeName1);
  TChain *ch1 = new TChain(treeName2);

  ch1->Add(newFileName);
  ch2->Add(oldFileName);

  compareTrees(ch2, ch1, drawsame);

}


#ifndef __CLING__

void usage(string ss){
  cout << "Usage: " << ss << " oldfilename newfilename [treeName1] [treeName2] [outdir] " << endl;
  cout << "default treeName1=\"tree_NoSys\" treeName2=\"tree_NoSys\" outdir=\"compareHaantuples\" " << endl;
}

int main(int argc, char **argv){

  if(argc<3){
    usage(argv[0]);
    return 1;
  }

  TString oldFileName = argv[1];
  TString newFileName = argv[2];

  TString treeName1="tree_NoSys";
  TString treeName2="tree_NoSys";
  TString outdir="compareHaantuples";

  if(argc>3){
    treeName1 = argv[3];
    treeName2 = argv[3];
  }

  if(argc>4)treeName2 = argv[4];
  if(argc>5)outdir = argv[5];


  compareTTHOutFile(oldFileName, newFileName, treeName1, treeName2, outdir);

  return 0;

}

#endif

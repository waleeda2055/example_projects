
#include "OutputReader.h"

#include <TSystem.h>
#include <TLegend.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <THStack.h>
#include <TLatex.h>
#include "AtlasStyle.h"
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "AtlasStyle.h"

TString date;
TString base; 

void normHisto(TH1* h, bool addflow=false){

  int nbins=h->GetNbinsX();
  double integral = 0;
  if(addflow) integral=h->Integral(0,nbins+1);
  else integral=h->Integral(1,nbins);

  h->Scale(1./integral);

}

vector<TString> TreeNameVector(TString filename){
  string treeName;
  vector <TString> allTrees;
  ifstream TreeNames(filename);
  if (TreeNames.is_open()){
    while(!TreeNames.eof()){
      getline(TreeNames, treeName);
      if (treeName != "") allTrees.push_back(treeName);
    }
  }
  return allTrees;
}

TString buildEventWeight(TString weight, TString cuts){

  TString eventweight ="";
  if(weight != "" && cuts != ""){
    eventweight = "("+weight+")*("+cuts+")";
  }
  else if(weight != ""){
    eventweight=weight;
  }
  else if(cuts != ""){
    eventweight=cuts;
  }
  
  //cout<<weight<<endl; 
  return eventweight;

}



class VariableData{

public:
  VariableData(TString n,
	       TString b,
	       TString t,
	       int nb,
	       double blo,
	       double bu,
	       TString opt=""){

    name=n;
    branch=b;
    title=t;
    nbins=nb;
    blow=blo;
    bup=bu;
    customBins=false;
    option=opt;


  }

  TString name;
  TString branch;
  TString title;
  int nbins;
  double blow;
  double bup;
  double* bins;
  bool customBins;
  TString option;


};



struct RegionData{

  TString name;
  TString region;
  TString label;
  TString cuts;
  int rebin;

};

RegionData getRegionData(TString name,
			 TString region,
			 TString label,
			 TString cuts="",
			 int rebin = 1){
  RegionData r;
  r.name=name;
  r.region=region;
  r.label=label;
  r.cuts=cuts;
  r.rebin=rebin;
  return r;
}


struct WeightData{

  TString name;
  TString weight;
  TString weight_ttbar;
  TString label;
  TString histoName1;
  TString histoName2;
  bool ttb2;

};

WeightData getWeightData(TString name,
			 TString weight,
			 TString label){
  WeightData r;
  r.name=name;
  r.weight=weight;
  r.weight_ttbar=weight;
  r.label=label;
  r.histoName1="";
  r.histoName2="";
  r.ttb2=false;
  return r;
}


//TLegend *getTLegend(float xlow=0.8, float ylow=0.6, float xup=0.93, float yup=0.92){ //orignal 
TLegend *getTLegend(float xlow=0.75, float ylow=0.75, float xup=0.90, float yup=0.90){  //for smoothing sys plots


  TLegend* leg = new TLegend(xlow,ylow,xup,yup);
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);

  return leg;
}

TLatex *getTLatex(TString label, float xlow = 0.20, float ylow = 0.88){

  TLatex *lat = new TLatex(xlow, ylow, label);
  lat->SetNDC();
  lat->SetTextFont(42);
  lat->SetTextSize(0.024 * 1.7);
  lat->SetTextColor(1);

  return lat;
}


void drawDataNorm(OutputReader *reader, VariableData Variable, RegionData Region, TString signal_name=""){

  TString region = Region.region;
  int rebin = Region.rebin;
  TString var = Variable.branch;
  TString name="norm_"+Variable.name+"_"+Region.name;
  if(""!=signal_name) name = name + "_" + signal_name; 
  TString option = Variable.option;
  TString eventweight =buildEventWeight("event_weight",Region.cuts);
  TString eventweightDDZ =buildEventWeight("event_weight*jet0_weight",Region.cuts);
  
  int nbins=Variable.nbins/rebin;
  double blow=Variable.blow;
  double bup =Variable.bup;
    
  vector <TH1*> hist_vector;
  vector <TString> histname_vector;
  if(signal_name==""){
    //histname_vector = {"ttbar","DDZ","a20", "a30", "a40", "a50", "a60"};
    //histname_vector = {"ttbar","Zjets+Wjets","SingTop","DiBos","ttV","Wjets","dy","a2","a6","a10","a14"};
    //histname_vector = {"ttbar_AF2","ttbar_genSys","ttbar_showerSys","ttbar_radUp","ttbar_radDown"}; 
    histname_vector = {"ttbar_AF2","ttbar_radUp"};
    //histname_vector = {"ttbar_AF2","ttbar_showerSys"};
    //histname_vector = {"Zjets_other"};
    // histname_vector = {"Zjets_BplusC"};
    //histname_vector = {"ttbar_AF2","ttbar_genSys"}; //for smooething 
    //histname_vector = {"H100a15","H120a15","H120a30","H140a15","H140a30","H140a45","H160a15","H160a30","H160a45","H160a60","H160a75"};
    //histname_vector = {"H160a15","H160a30","H160a45","H160a60","H160a75"};
    //histname_vector = {"H100a15","H120a15","H120a30","H140a15","H140a30","H140a45"};   
  }
  //else histname_vector = {"ttbar","Zjets","Wjets","SingTop","DiBos","ttV","dy",signal_name};
  else histname_vector = {signal_name};

  for (auto histname:histname_vector){
    if ("DDZ"==histname){
      hist_vector.push_back(reader->getHistoFromTree(histname,region,var,name,nbins,blow,bup,eventweightDDZ));}
    else{
      hist_vector.push_back(reader->getHistoFromTree(histname,region,var,name,nbins,blow,bup,eventweight));}
  }

   vector <int> colors = {kRed,kBlue,kGreen,kViolet,kOrange,kCyan,kPink,kYellow,9};
  //TColor::GetColor("#8bd3fb"),TColor::GetColor("#ffae32"), TColor::GetColor("#34992f"),TColor::GetColor("#ffdc00"), TColor::GetColor("#ff839b"),TColor::GetColor("#785d9f"),
  //TColor::GetColor("#3399ff")

  //vector <int> colors = {kGreen, 7, 9, kRed, kBlue, kViolet, kOrange};
  vector <int> markers = {1,1,1,1,1,1,1};
  TCanvas *c_normHistos = new TCanvas(name,name,200,10,700,500);

  TLegend *leg = getTLegend();
  //TLatex *l2 = getTLatex(Region.label);

  float max = 0;
  for(unsigned int i=0; i<hist_vector.size();i++){

    hist_vector[i]->SetLineColor(colors[i]);
    hist_vector[i]->SetMarkerColor(colors[i]);
    hist_vector[i]->SetMarkerStyle(markers[i]);
    //normHisto(hist_vector[i]); //original. used to noramilze the histo
    if((hist_vector[i]->GetMaximum())>max){  
      max = hist_vector[i]->GetMaximum();
    }
  }

  if("log"==option){
    c_normHistos->cd()->SetLogy();
    hist_vector[0]->SetMaximum(max*50);
  }
  else hist_vector[0]->SetMaximum(max*1.30);

  // original to make regualr plots  
    
  for(unsigned int i=0; i<hist_vector.size();i++){
    if(0==i){
      hist_vector[i]->GetXaxis()->SetTitle(Variable.title);
      //hist_vector[i]->GetXaxis()->SetRange(1, hist_vector[i]->GetNbinsX() + 1); //WA get overflow
      //hist_vector[i]->GetYaxis()->SetTitle("Normalized to 1");
      hist_vector[i]->GetYaxis()->SetTitle("Events / 2.5 GeV");
      //hist_vector[i]->Draw("hist cp");
      hist_vector[i]->Draw("hist");
    }
    else{
      //if (i<1) hist_vector[i]->Draw("histsame cp"); no errors
      if (i<1) hist_vector[i]->Draw("hist");
      else hist_vector[i]->Draw("histsame");      
    }
    //leg->AddEntry(hist_vector[i],histname_vector[i],"LP");
    if (i==0)
     leg->AddEntry(hist_vector[0],"Nominal","LP"); //custom legend
    else if(i==1)
     //leg->AddEntry(hist_vector[1],"Generator Sys","LP");
    leg->AddEntry(hist_vector[1],"Radiation Sys","LP");
    //leg->AddEntry(hist_vector[1],"Shower Sys","LP");

  }
    
 
  //to plot sys-nom/sys 
 
  /* 
  TH1* h_mc_sum = (TH1*)hist_vector[1]->Clone();
  h_mc_sum->Add(hist_vector[0],-1);

  
  //clone data in to h_ratio
  TH1* h_ratio = (TH1*)h_mc_sum->Clone();
  h_ratio->Divide(hist_vector[0]);
  h_ratio->SetXTitle(Variable.title);
  h_ratio->SetYTitle("sys-nom/nom");
  //h_ratio->Fit("pol0"); //for shower and rad
  h_ratio->Fit("pol1"); //for gensys
  h_ratio->Draw("same e");
  gStyle->SetOptFit(0); //to hide fit paramaters
  */  
 
  //for smoothing zjets and getting integral error 
  
  /*
  int x1 = 12; 
  int x2 = 75;
  //TF1 * fn = new TF1("fn","[0] + [1]*x + [2]*x*x",x1,x2);
  //TF1 * fn = new TF1("fn","[0]",12.,78.); 
  TF1 * fn = new TF1("fn","[0]+exp([1]*(x-[2]))",12.,78.); // CplusExp
  //TF1 * fn = new TF1("fn","[0]+[1]/(pow(x*x-[2]*[2],2)+[2]*[2]*[3]*[3])",12.,78.); BW
  //fn2->SetParameter(0,0.5); //base
  //fn2->SetParameter(1,2E7); //height 
  //fn2->SetParameter(2,91.187); //mz
  //fn2->SetParameter(3,2.4952); //GammaZ 
  
  //fn->SetParameter(0,.33);

  //hist_vector[0]->Fit(fn);
  auto fitResult = hist_vector[0]->Fit(fn,"SL"); // L for likelihood fit 
  //TF1 *fn = hist_vector[0]->GetFunction("myfunc");
  int np = 0;
  double xvals[100];
  double yvals[100];
  double dxvals[100];
  double dyvals[100];
  //int x1 =0;
  //int x2 = hist_vector[0]->GetNbinsX();
  //std::cout << x2 << std::endl;
  //auto covMatrix = fitResult->GetCovarianceMatrix();
  std::cout << "Covariance matrix from the fit ";
  //covMatrix.Print();

   
  for (int i=x1;i<x2;i++) {
    double a = i; // start of integration
    double b = i+1; // end of integration
    double integ = fn->Integral(a,b);
    //double dinteg = fn->IntegralError(a,b, fitResult->GetParams() , covMatrix.GetMatrixArray() );
    double dinteg = fn->IntegralError(a,b);
    //std::cout<<"From "<<a<<" to "<<b<<" total="<<integ<<"+-"<<dinteg<<std::endl;
    std::cout<<integ<<std::endl;
    std::cout<<dinteg<<std::endl;
    xvals[np] = (a+b)/2.;
    yvals[np] = integ;
    dxvals[np] = (b-a)/2.;
    dyvals[np] = dinteg;
    np++;
  }
    
  */
  
  
  leg->Draw();
  SetAtlasStyle();
  ATLASLabel(0.22,0.85,"Internal");
  TLatex latex;
  latex.SetNDC();
  latex.SetTextColor(kBlack);
  latex.SetTextSize(0.04);
  latex.SetTextAlign(13);  //align at top
  latex.DrawLatex(.22,.83, "#sqrt{s} = 13 TeV, 139 fb^{-1}");
  
  c_normHistos->Update();

 
  gSystem->Exec("mkdir -p "+base+"/"+Region.name+"/norm/");
  c_normHistos->SaveAs(base+"/"+Region.name+"/norm/"+name+".png");
 
  /* 
     
  TFile *fOut = new TFile(base+"/smoothing_sys.root", "RECREATE");
  h_ratio->Draw();
  h_ratio->Write();
  fOut->Close();
  */ 
  
  /*
  TFile *fOut = new TFile(base+"/Zjets_smooth.root", "RECREATE");
  hist_vector[0]->Draw();
  //fn->Draw();
  hist_vector[0]->Write();
  fOut->Close();
  */
}


void drawDataStack(OutputReader* reader, VariableData Variable, RegionData Region, TString signal_name="", bool useDDZ = 1, TString RegionName = ""){

  TString region = Region.region;
  int rebin = Region.rebin;
  TString var = Variable.branch;
  TString option = Variable.option;
  TString name="stack_"+Variable.name+"_"+Region.name;
  if(""!=signal_name) name = name + "_" + signal_name; 

  TString eventweight =buildEventWeight("event_weight",Region.cuts);
  TString eventweightDDZ =buildEventWeight("event_weight*jet0_weight",Region.cuts);

  int nbins=Variable.nbins/rebin;
  if(nbins<1) nbins = 1;
  double blow=Variable.blow;
  double bup =Variable.bup;
  vector <TString> histname_vector;
  vector <TH1*> hist_vector;
  if(signal_name==""){
    histname_vector = {"H100a15","H120a15","H120a30","H140a15","H140a30","H140a45","H160a15","H160a30","H160a45","H160a60","H160a75"};
    //histname_vector = {"H160a15","H160a30","H160a45","H160a60","H160a75"};
    //histname_vector = {"a20", "a30", "a40", "a50", "a60"};
  }
  else if(signal_name=="None") histname_vector.clear();
  else histname_vector = {signal_name};

  for (auto histname:histname_vector){
    hist_vector.push_back(reader->getHistoFromTree(histname,region,var,name,nbins,blow,bup,eventweight));
  }

  vector <int> colors = {kRed, kBlue, kViolet, kOrange, kGreen};
  TCanvas *c_stackHistos = new TCanvas(name,name,200,10,700,500);
  THStack* histStack = new THStack(name,name);
  TLegend *leg = getTLegend();
  //TLatex *l2 = getTLatex(Region.label);

  TPad* pad0 = new TPad("pad0","pad0",0,0.2,1,1,0,0,0);
  pad0->SetTickx(false);
  pad0->SetTicky(false);
  pad0->SetTopMargin(0.05);
  pad0->SetBottomMargin(0.12);
  pad0->SetLeftMargin(0.14);
  pad0->SetRightMargin(0.05);
  pad0->SetFrameBorderMode(0);
  pad0->SetFillStyle(0);
  
  TPad* pad1 = new TPad("pad1","pad1",0,0.,1,0.28,0,0,0);
  pad1->SetTickx(false);
  pad1->SetTicky(false);
  pad1->SetTopMargin(0.0);
  pad1->SetBottomMargin(0.4);
  pad1->SetLeftMargin(0.14);
  pad1->SetRightMargin(0.05);
  pad1->SetFrameBorderMode(0);
  pad1->SetFillStyle(0);
  pad0->Draw();
  pad1->Draw();


  TH1 *h_ttbar = reader->getHistoFromTree("ttbar",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_zjets = reader->getHistoFromTree("Zjets",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_wjets = reader->getHistoFromTree("Wjets",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_data = reader->getHistoFromTree("data",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_SingleTop = reader->getHistoFromTree("SingTop",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_DiBoson = reader->getHistoFromTree("DiBos",region,var,name,nbins,blow,bup,eventweight);
  //TH1 *h_ttV = reader->getHistoFromTree("ttV",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_ttV = reader->getHistoFromTree("ttZ",region,var,name,nbins,blow,bup,eventweight);
  
  
  h_wjets->SetFillColor(TColor::GetColor("#785d9f"));
  h_ttV->SetFillColor(TColor::GetColor("#ff839b"));
  h_DiBoson->SetFillColor(TColor::GetColor("#ffdc00"));
  h_SingleTop->SetFillColor(TColor::GetColor("#34992f"));
  h_zjets->SetFillColor(TColor::GetColor("#ffae32"));
  h_ttbar->SetFillColor(TColor::GetColor("#8bd3fb"));

  //TH1 *h_ddz = reader->getHistoFromTree("DDZ",region,var,name,nbins,blow,bup,eventweightDDZ);
  //TH1 *h_ddz;
  //if(useDDZ) h_ddz = reader->getHistoFromTree("DDZ",region,var,name,nbins,blow,bup,eventweight);
  // else h_ddz = reader->getHistoFromTree("Zjets",region,var,name,nbins,blow,bup,eventweight);

  //TH1 *h_other = reader->getHistoFromTree("Wjets+ttV+db+st",region,var,name,nbins,blow,bup,eventweight);

  histStack->Add(h_wjets);
  histStack->Add(h_ttV);
  histStack->Add(h_DiBoson);
  histStack->Add(h_SingleTop);
  histStack->Add(h_zjets);
  histStack->Add(h_ttbar);


  pad0->cd();
  histStack->Draw("hist");
  h_data->SetMarkerSize(.75);
  h_data->Draw("same");

  h_data->SetMarkerStyle(20); 
  h_data->SetMarkerColor(kBlack);

  int integral = h_ttbar->Integral(0,nbins+1) + h_zjets ->Integral(0,nbins+1) + h_wjets ->Integral(0,nbins+1) + 
  h_SingleTop->Integral(0,nbins+1) + h_DiBoson->Integral(0,nbins+1) + h_ttV->Integral(0,nbins+1) ;

  //h_ttbar->GetXaxis()->SetRange(1, h_ttbar->GetNbinsX() + 1); //get overflow
  //h_wjets->GetXaxis()->SetRange(1, h_wjets->GetNbinsX() + 1); //get overflow
  //h_zjets->GetXaxis()->SetRange(1, h_zjets->GetNbinsX() + 1); //get overflow

  leg->AddEntry(h_data,"data","LP");
  leg->AddEntry(h_ttbar,"t#bar{t}","F");
  leg->AddEntry(h_zjets,"Z+jets","F");
  leg->AddEntry(h_SingleTop,"SingTop","F");
  leg->AddEntry(h_DiBoson,"DiBos","F");
  leg->AddEntry(h_ttV,"ttV","F");
  leg->AddEntry(h_wjets,"W+jets","F");


  //if(RegionName!="") leg->AddEntry(h_ddz,"Zjets","F");
  //else leg->AddEntry(h_ddz,"ddz","F");
  //leg->AddEntry(h_other,"other","F");
  TH1 * hist = (TH1*)histStack->GetHistogram();
  hist->GetXaxis()->SetTitle(Variable.title);
  hist->GetYaxis()->SetTitle("Events");
  hist->GetXaxis()->SetLabelSize(0);
  hist->GetXaxis()->SetTitleSize(0);
  //hist->GetYaxis()->SetLabelSize(.15);
  hist->GetYaxis()->SetTitleOffset(1.0);

  
  float max = histStack->GetMaximum();
  if(h_data->GetMaximum()>max) max = h_data->GetMaximum();

  for(unsigned int i=0; i<hist_vector.size();i++){

    hist_vector[i]->SetMarkerColor(colors[i]);
    hist_vector[i]->SetLineColor(colors[i]);
    //hist_vector[i]->GetXaxis()->SetRange(1, hist_vector[i]->GetNbinsX() + 1); //WA get overflow
    //hist_vector[i]->Scale(integral/(hist_vector[i]->Integral(0,nbins+1)));
    hist_vector[i]->Draw("histsame");
    //cout <<hist_vector[i]->GetBinContent(5)<<endl;
    leg->AddEntry(hist_vector[i],histname_vector[i],"L");
    if(hist_vector[i]->GetMaximum()>max) max = hist_vector[i]->GetMaximum();
  }   

  if("log"==option){
    pad0->cd()->SetLogy();
    histStack->SetMaximum(max*30);
  }
  else histStack->SetMaximum(max*1.2);

  SetAtlasStyle();
  //ATLASLabel(0.55,0.85,"Internal");
  TLatex latex;
  latex.SetNDC();
  latex.SetTextColor(kBlack);
  latex.SetTextSize(0.04);
  latex.SetTextAlign(13);  //align at top
  latex.DrawLatex(.55,.83, "#sqrt{s}=13 TeV, 139 fb^{-1}");

  leg->Draw(); 
  //l2->Draw();


  //divide Data/MC
  TH1* h_mc_sum = (TH1*)h_ttbar->Clone();
  h_mc_sum->Add(h_wjets);
  h_mc_sum->Add(h_ttV);
  h_mc_sum->Add(h_DiBoson);
  h_mc_sum->Add(h_SingleTop);
  h_mc_sum->Add(h_zjets);

 
  //clone data in to h_ratio
  TH1* h_ratio = (TH1*)h_data->Clone();  
  h_data->SetDirectory(0);
  h_ratio->GetXaxis()->SetLabelSize(0);
  h_ratio->GetXaxis()->SetTitleSize(0);
  h_mc_sum->SetMarkerSize(.8); 
  h_mc_sum->SetDirectory(0);
  h_ratio->Divide(h_mc_sum);
  h_ratio->SetXTitle(Variable.title);
  h_ratio->SetYTitle("Data/MC");
  pad1->cd();
  h_ratio->Draw();

  h_ratio->GetYaxis()->SetRangeUser(0.5,1.7);
  h_ratio->GetXaxis()->SetTitleSize(0.14);
  h_ratio->GetXaxis()->SetLabelSize(0.15);
  h_ratio->GetXaxis()->SetLabelOffset(0.05);
  h_ratio->GetXaxis()->SetTitleOffset(1.3);

  h_ratio->GetYaxis()->SetTitleSize(0.12);
  h_ratio->GetYaxis()->SetLabelSize(0.10);  
  h_ratio->GetYaxis()->SetNdivisions(509);
  h_ratio->GetYaxis()->SetTitleOffset(.38);

  TLine *hline = new TLine(h_ratio->GetXaxis()->GetXmin(),1,h_ratio->GetXaxis()->GetXmax(),1);  
  hline->SetLineColor(kRed);
  //hline->SetLineWidth(2);
  hline->SetLineStyle(2);
  hline->Draw();


  if(RegionName!=""){
    gSystem->Exec("mkdir -p plots/IsoStudy/stack/"+RegionName);
    c_stackHistos->SaveAs("plots/IsoStudy/stack/"+RegionName +"/"+name+".png");
  }
  else{
    gSystem->Exec("mkdir -p "+base+"/"+Region.name+"/stack/");
    c_stackHistos->SaveAs(base+"/"+Region.name+"/stack/"+name+".png");
  }

  TFile *fOut = new TFile(base+"/dimuonhisto1GeV_all.root", "RECREATE");
  h_mc_sum->Draw();
  h_mc_sum->Write();
  fOut->Close();
}


void drawNoDataStack(OutputReader* reader, VariableData Variable, RegionData Region, TString signal_name="", bool useDDZ = 1, TString RegionName = ""){

  TString region = Region.region;
  int rebin = Region.rebin;
  TString var = Variable.branch;
  TString option = Variable.option;
  TString name="stack_"+Variable.name+"_"+Region.name;
  if(""!=signal_name) name = name + "_" + signal_name; 

  TString eventweight =buildEventWeight("event_weight",Region.cuts);
  TString eventweightDDZ =buildEventWeight("event_weight*jet0_weight",Region.cuts);

  int nbins=Variable.nbins/rebin;
  if(nbins<1) nbins = 1;
  double blow=Variable.blow;
  double bup =Variable.bup;
  vector <TString> histname_vector;
  vector <TH1*> hist_vector;
  if(signal_name==""){
    //histname_vector = {"a20", "a30", "a40", "a50", "a60"};
   //histname_vector = {"H100a15","H120a15","H120a30","H140a15","H140a30","H140a45","H160a15","H160a30","H160a45","H160a60","H160a75"};
    histname_vector = {"H160a15","H160a30","H160a45","H160a60","H160a75"}; 
   //histname_vector = {"H100a15","H120a15","H120a30","H140a15","H140a30","H140a45"}; 
   //histname_vector = {"H100a15","H120a15","H120a30","H140a30","H160a15","H160a75"};  

  }
  else if(signal_name=="None") histname_vector.clear();
  else histname_vector = {signal_name};

  for (auto histname:histname_vector){
    hist_vector.push_back(reader->getHistoFromTree(histname,region,var,name,nbins,blow,bup,eventweight));
  }

  vector <int> colors = {kRed, kBlue, kViolet, kOrange, kGreen, kCyan};
  TCanvas *c_stackHistos = new TCanvas(name,name,200,10,700,500);
  THStack* histStack = new THStack(name,name);
  TLegend *leg = getTLegend();
  //TLatex *l2 = getTLatex(Region.label);

  TH1 *h_ttbar = reader->getHistoFromTree("ttbar",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_zjets = reader->getHistoFromTree("Zjets_other",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_wjets = reader->getHistoFromTree("Wjets",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_data = reader->getHistoFromTree("data",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_SingleTop = reader->getHistoFromTree("SingTop",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_DiBoson = reader->getHistoFromTree("DiBos",region,var,name,nbins,blow,bup,eventweight);
  //TH1 *h_ttV = reader->getHistoFromTree("ttV",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_ttV = reader->getHistoFromTree("ttZ",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_ttW = reader->getHistoFromTree("ttW",region,var,name,nbins,blow,bup,eventweight);

  //TH1 *h_dy = reader->getHistoFromTree("dy",region,var,name,nbins,blow,bup,eventweight);


  h_wjets->SetFillColor(TColor::GetColor("#785d9f"));
  h_ttV->SetFillColor(TColor::GetColor("#ff839b"));
  h_DiBoson->SetFillColor(TColor::GetColor("#ffdc00"));
  h_SingleTop->SetFillColor(TColor::GetColor("#34992f"));
  //h_zjets->SetFillColor(TColor::GetColor("#ffae32"));
  h_ttbar->SetFillColor(TColor::GetColor("#8bd3fb"));
  h_ttW->SetFillColor(TColor::GetColor("#ab82ff"));


  //TH1 *h_ddz = reader->getHistoFromTree("DDZ",region,var,name,nbins,blow,bup,eventweightDDZ);
  //TH1 *h_ddz;
  //if(useDDZ) h_ddz = reader->getHistoFromTree("DDZ",region,var,name,nbins,blow,bup,eventweight);
  //else h_ddz = reader->getHistoFromTree("Zjets",region,var,name,nbins,blow,bup,eventweight);

  //TH1 *h_other = reader->getHistoFromTree("Wjets+ttV+db+st",region,var,name,nbins,blow,bup,eventweight);

  //histStack->Add(h_dy);
  
  //histStack->Add(h_wjets);
  //histStack->Add(h_ttV);
  //histStack->Add(h_DiBoson);
  //histStack->Add(h_SingleTop);
  histStack->Add(h_zjets);
  //histStack->Add(h_ttbar);
  //histStack->Add(h_ttW);
  histStack->Draw("hist e0");
  //h_data->SetMarkerSize(.8);
  //h_data->Draw("same");


  //h_data->SetMarkerStyle(20); h_data->SetMarkerColor(kBlack);

  int integral = h_ttbar->Integral(0,nbins+1) + h_zjets ->Integral(0,nbins+1) + h_wjets ->Integral(0,nbins+1);

  //h_ttbar->GetXaxis()->SetRange(1, h_ttbar->GetNbinsX() + 1); //get overflow
  //h_wjets->GetXaxis()->SetRange(1, h_wjets->GetNbinsX() + 1); //get overflow
  //h_zjets->GetXaxis()->SetRange(1, h_zjets->GetNbinsX() + 1); //get overflow

  //leg->AddEntry(h_data,"data","LP");
  leg->AddEntry(h_ttbar,"t#bar{t}","F");
  leg->AddEntry(h_zjets,"Zjets","F");
  leg->AddEntry(h_SingleTop,"SingTop","F");
  leg->AddEntry(h_DiBoson,"DiBos","F");
  leg->AddEntry(h_ttV,"ttZ","F");
  leg->AddEntry(h_wjets,"W+jets","F");
  leg->AddEntry(h_ttW,"ttW","F");

  //if(RegionName!="") leg->AddEntry(h_ddz,"Zjets","F");
  //else leg->AddEntry(h_ddz,"ddz","F");
  //leg->AddEntry(h_other,"other","F");
  TH1 * hist = (TH1*)histStack->GetHistogram();
  hist->GetXaxis()->SetTitle(Variable.title);
  //hist->GetYaxis()->SetTitle("Events / 1.5 GeV");

  float max = histStack->GetMaximum();
  //if(h_data->GetMaximum()>max) max = h_data->GetMaximum();
 
  for(unsigned int i=0; i<hist_vector.size();i++){

    hist_vector[i]->SetMarkerColor(colors[i]);
    hist_vector[i]->SetLineColor(colors[i]);
    //hist_vector[i]->GetXaxis()->SetRange(1, hist_vector[i]->GetNbinsX() + 1); //WA get overflow
    //hist_vector[i]->Scale(integral/(hist_vector[i]->Integral(0,nbins+1)));
    hist_vector[i]->Draw("histsame");
    leg->AddEntry(hist_vector[i],histname_vector[i],"L");
    if(hist_vector[i]->GetMaximum()>max) max = hist_vector[i]->GetMaximum();
  }   

  if("log"==option){
    c_stackHistos->cd()->SetLogy();
    histStack->SetMaximum(max*30);
  }
  else histStack->SetMaximum(13);
 
  histStack->SetMinimum(0);


  SetAtlasStyle();
  //ATLASLabel(0.55,0.85,"Internal");
  TLatex latex;
  latex.SetNDC();
  latex.SetTextColor(kBlack);
  latex.SetTextSize(0.04);
  latex.SetTextAlign(13);  //align at top
  latex.DrawLatex(.55,.83, "#sqrt{s}=13 TeV, 139 fb^{-1}");

  leg->Draw(); 
  //l2->Draw();

  if(RegionName!=""){
    gSystem->Exec("mkdir -p plots/IsoStudy/noDatastack/"+RegionName);
    c_stackHistos->SaveAs("plots/IsoStudy/noDatastack/"+RegionName +"/"+name+".png");
  }
  else{
    gSystem->Exec("mkdir -p "+base+"/"+Region.name+"/noDatastack/");
    c_stackHistos->SaveAs(base+"/"+Region.name+"/noDatastack/"+name+".png");
  }

}




TGraph *calculateSign(TH1* h_signal, TH1* h_bgr1, TH1* h_bgr2,  TString side = "left", float sys_bgr1 = 0, float sys_bgr2 = 0){

  int nbins = h_signal->GetNbinsX();

  float int_signal = 0;
  float int_zjets = 0;
  float int_ttbar = 0;
  int size = 0;

  double pt[nbins+1], significance[nbins+1], significance_sys[nbins+1];

  for (int i=0; i<nbins+1; i++){
    if("left"==side){
      int_signal = h_signal->Integral(i,nbins+1);
      int_zjets = h_bgr1->Integral(i,nbins+1);
      int_ttbar = h_bgr2->Integral(i,nbins+1);
    }
    if("right"==side){
      int_signal = h_signal->Integral(0,nbins+1-i);
      int_zjets = h_bgr1->Integral(0,nbins+1-i);
      int_ttbar = h_bgr2->Integral(0,nbins+1-i);  
    }
    if ((int_ttbar+int_zjets)>0||true){ //only calculate significance if there are events left for the backgrounds
      float sign_sys = 0;
      if((int_ttbar+int_zjets)>0)
	sign_sys = int_signal/sqrt((int_zjets+int_ttbar) + (int_zjets*sys_bgr1+int_zjets*sys_bgr1)*(int_ttbar*sys_bgr2+int_ttbar*sys_bgr1));
      else sign_sys = 0;

      if("left"==side){
	pt[i] = h_signal->GetBinCenter(i);
      }
      else pt[i] = h_signal->GetBinCenter(nbins+1-i);

      significance_sys[i] = sign_sys;
      size = i;
      //cout << "pt: " << i << ", S/sqrt(BG): " << sign << ", sig_sys: " << sign_sys <<endl;
    }
  }

  TGraph* gr1 = new TGraph(size+1,pt,significance_sys);

  return gr1;

}



void drawDataSign(OutputReader* reader, VariableData Variable, RegionData Region, TString signal_name){

  TString region = Region.region;
  TString var = Variable.branch;
  TString name="sign_"+Variable.name+"_"+Region.name+"_"+signal_name;

  TString eventweight =buildEventWeight("event_weight",Region.cuts);
  TString eventweightDDZ =buildEventWeight("event_weight*jet0_weight",Region.cuts);
  
  int nbins=Variable.nbins;
  double blow=Variable.blow;
  double bup =Variable.bup;

  
  //TH1* h_dbz = reader->getHistoFromTree("DDZ",region,var,name,nbins,blow,bup,eventweightDDZ);
  TH1* h_a20 = reader->getHistoFromTree(signal_name,region,var,name,nbins,blow,bup,eventweight);

  TH1 *h_ttbar = reader->getHistoFromTree("ttbar",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_zjets = reader->getHistoFromTree("Zjets",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_wjets = reader->getHistoFromTree("Wjets",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_SingleTop = reader->getHistoFromTree("SingTop",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_DiBoson = reader->getHistoFromTree("DiBos",region,var,name,nbins,blow,bup,eventweight);
  //TH1 *h_ttV = reader->getHistoFromTree("ttV",region,var,name,nbins,blow,bup,eventweight);
  TH1 *h_ttV = reader->getHistoFromTree("ttZ",region,var,name,nbins,blow,bup,eventweight);

  TH1* h_mc_sum = (TH1*)h_zjets->Clone();
  h_mc_sum->Add(h_wjets);
  h_mc_sum->Add(h_ttV);
  h_mc_sum->Add(h_DiBoson);
  h_mc_sum->Add(h_SingleTop);
      
  vector <TGraph*> vector_tgraph;

  TGraph *gr1 = calculateSign(h_a20, h_mc_sum, h_ttbar);
  //TGraph *gr2 = calculateSign(h_a20, h_zjets, h_ttbar, "left",0.1, 0.1);
  TGraph *gr2 = calculateSign(h_a20, h_mc_sum, h_ttbar, "left");
  TGraph *gr3 = calculateSign(h_a20, h_mc_sum, h_ttbar,"right");

  vector_tgraph.push_back(gr1);
  vector_tgraph.push_back(gr2);
  //vector_tgraph.push_back(gr3); from right 

  vector <int> colors = {kGreen+1, kBlue+1, kRed+1};
  vector <int> markers = {22,22,22};
  TCanvas *tgraph_sign = new TCanvas(name,name,200,10,700,500);
  TLegend *leg = getTLegend(0.7,0.8);
  TLatex *l2 = getTLatex(Region.label+","+signal_name);
  TMultiGraph *mg = new TMultiGraph();

  double max = 0;
  unsigned int size = gr1->GetN();
  double xi = 0;
  double yi = 0;

  for(unsigned int i=0;i<vector_tgraph.size();i++){
    for(unsigned int j=0;j<size;j++){
      vector_tgraph[i]->GetPoint(j,xi,yi);
      if(yi>max) max = yi;
    }
    vector_tgraph[i]->SetLineColor(colors[i]);
    vector_tgraph[i]->SetMarkerColor(colors[i]);
    vector_tgraph[i]->SetMarkerStyle(markers[i]);


    if(0==i){
      vector_tgraph[i]->GetXaxis()->SetTitle(Variable.title);
      vector_tgraph[i]->GetYaxis()->SetTitle("Significance"); 
      //mg->GetXaxis()->SetTitle(Variable.title);
      //mg->GetYaxis()->SetTitle("Significance");
      //mg->Add(vector_tgraph[i]);
      vector_tgraph[i]->Draw();
    }
    else {
      vector_tgraph[i]->Draw("LP");
      //mg->Add(vector_tgraph[i]);
    }
  }

  //mg->Draw("alp");

  vector_tgraph[0]->SetMaximum(max*1.5);
  vector_tgraph[0]->SetMinimum(0);

  leg->AddEntry(vector_tgraph[1],"S/#sqrt{B}, >= cut","l");
  //leg->AddEntry(vector_tgraph[1],"S/#sqrt{B}, >= cut, from left","l");
  //leg->AddEntry(vector_tgraph[1],"Sign., cut from left, 10% BGerr","l");
  //leg->AddEntry(vector_tgraph[2],"S/#sqrt{B}, <= cut, from right","l");


  leg->Draw();
  l2->Draw();

  gSystem->Exec("mkdir -p "+base+"/"+Region.name+"/sign/");
  tgraph_sign->SaveAs(base+"/"+Region.name+"/sign/"+name+".png");

}



void drawExample(OutputReader* reader){

 
  //RegionData SR20 = getRegionData("SR20", "all", "SR20","klbbmm_m>110 && klbbmm_m<140 && met<60 && dimuon_m>19 && dimuon_m<21 && klfit_loglikelihood>-8",2);
  //RegionData SR30 = getRegionData("SR30", "all", "SR30","klbbmm_m>110 && klbbmm_m<140 && met<60 && dimuon_m>29 && dimuon_m<31 && klfit_loglikelihood>-8",2);
  //RegionData SR40 = getRegionData("SR40", "all", "SR40","klbbmm_m>110 && klbbmm_m<140 && met<60 && dimuon_m>39 && dimuon_m<41 && klfit_loglikelihood>-8",2);
  //RegionData SR50 = getRegionData("SR50", "all", "SR50","klbbmm_m>110 && klbbmm_m<140 && met<60 && dimuon_m>49 && dimuon_m<51 && klfit_loglikelihood>-8",2);
  //RegionData SR60 = getRegionData("SR60", "all", "SR60","klbbmm_m>110 && klbbmm_m<140 && met<60 && dimuon_m>59 && dimuon_m<61 && klfit_loglikelihood>-8",2);
  //RegionData TCR = getRegionData("TCR", "all", "TCR","klbbmm_m>110 && klbbmm_m<140 && met>60 && dimuon_m>16 && dimuon_m<64 && klfit_loglikelihood>-8",2);
  //RegionData SB1 = getRegionData("SB1", "all", "SB1","klbbmm_m > 80 && klbbmm_m<110  && met<60 && dimuon_m>16 && dimuon_m<64 && klfit_loglikelihood>-8",2);
  //RegionData SB2 = getRegionData("SB2", "all", "SB2","klbbmm_m>140 && klbbmm_m<170 && met<60 && dimuon_m>16 && dimuon_m<64 && klfit_loglikelihood>-8",2);
  //RegionData noDimuon = getRegionData("noDimuon", "all", "no Dimuon cut","klbbmm_m>110 && klbbmm_m<140 && met<60 && klfit_loglikelihood>-8",2);
  //RegionData noHiggs = getRegionData("noHiggs", "all", "no cut on Higgs mass"," met<60 && klfit_loglikelihood>-8");
  //RegionData presel = getRegionData("presel", "all", "presel region","klfit_loglikelihood>-11");
  RegionData all = getRegionData("all", "all", "all region","");
  RegionData CRe = getRegionData("CRe", "all", "CRe","nBjets_77 >=0");
  //RegionData CR2e = getRegionData("CR2e", "all", "CR2e","dimuon_m >= 12 && dimuon_m <= 77 && nElectrons_signal == 1 && nMuons_signal ==2 && isOS ==0 && elec1Pt >= 20 && nBjets_77 >= 1 && nJet20 >=3");
  RegionData CRss = getRegionData("CRss", "all","CRss","( mu2Pt/mu1Pt > .2 || Mmu1e_ss > 0 ) && dimuon_m >= 12 && dimuon_m <= 77 && nElectrons_signal == 1 && nMuons_signal ==2 && isOS ==0 && elec1Pt >= 20 && nBjets_77 >= 1 && nJet20 >=3");
  RegionData CRe0fake = getRegionData("CRe0fake", "all","CRe0fake","(mu2Pt/mu1Pt > .2 || Mmu1e_ss > 0 ) && dimuon_m >= 12 && dimuon_m <= 77 && nElectrons_signal == 1 && nMuons_signal ==2 && isOS ==1 && elec1Pt <= 20 && nBjets_77 >= 1 && nJet20 >=3"); // used on same-sign muon samples for fake-enrich in Mmumu
  RegionData CRe0j4fake = getRegionData("CRe0j4fake", "all", "CRe0j4fake","nMuons_signal == 2 && nElectrons_signal == 0 && nJet20 >=4 && nBjets_77 >= 1");
  RegionData CRe1fake = getRegionData("CRe1fake", "all", "CRe1fake","nElectrons_signal > 0"); //  used on same-sign muons to see Mmu1e and Mmu2e
  RegionData CRe1j4fake = getRegionData("CRe1j4fake", "all", "CRe1j4fake","nElectrons_signal > 0 && nJet20 >=4 && nBjets_77 >= 1");
  RegionData CRZ = getRegionData("CRZ", "all", "CRZ","dimuon_m >= 77 && dimuon_m <= 105 && nElectrons_signal == 1 && nMuons_signal ==2 && isOS ==1 && elec1Pt <= 20 && nBjets_77 >= 1 && nJet20 >=3");
  RegionData CRttZ = getRegionData("CRttZ", "all", "CRttZ","dimuon_m >= 77 && dimuon_m <= 105 && nElectrons_signal == 1 && nMuons_signal ==2 && isOS ==1 && elec1Pt >= 20 && nBjets_77 >= 1 && nJet20 >=3");

  RegionData VR = getRegionData("VR", "all", "VR","Mmu1e_os >=30 && Mmu1e_os <=110 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20");
  //RegionData SR = getRegionData("SR", "all","SR"," ( mu2Pt/mu1Pt > .2 || Mmu1e_ss > 0 ) && dimuon_m >= 11 && dimuon_m <= 79 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal ==2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20 && event_number!= 10396069  && event_number!=16148384");
  RegionData SR = getRegionData("SR", "all","SR","( mu2Pt/mu1Pt > .2 || Mmu1e_ss > 0 ) && dimuon_m >= 12 && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal ==2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20"); 
 //RegionData SR = getRegionData("SR", "all","SR"," ( mu2Pt/mu1Pt > .2 || Mmu1e_ss > 0 ) && dimuon_m >= 11 && dimuon_m <= 79 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal ==2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20 && event_number!=10396069 && event_weight >= 0");
  //  RegionData SR = getRegionData("SR", "all","SR","( mu2Pt/mu1Pt > .2 || Mmu1e_ss > 0 ) && dimuon_m >= 12 && dimuon_m <= 80 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal ==2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20");
      
  //RegionData SR2 = getRegionData("SR2", "all", "SR2","dimuon_m < 80 && nJet20 >=4 && nBjets_77 > 0 && nElectrons_signal > 0");
  RegionData SR3 = getRegionData("SR3", "all", "SR3","dimuon_m < 80 && nJet20 >=4 && nBjets_77 >= 1 && nElectrons_signal >=1 && elec1Pt >= 20 & mu2Pt >= 10");
  RegionData SR0e = getRegionData("SR0e", "all", "SR0e","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2");
  RegionData SR1e = getRegionData("SR1e", "all", "SR1e","(mu2Pt/mu1Pt > 0 ) && dimuon_m >= 12 && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nBjets_77 >= 1"); 
  RegionData SR2e = getRegionData("SR2e", "all", "SR2e"," (mu2Pt/mu1Pt > 0) && dimuon_m >= 12 && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3");
  RegionData SR3e = getRegionData("SR3e", "all", "SR3e","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1");
  RegionData SR4e = getRegionData("SR4e", "all", "SR4e"," (pTfrac_mu2_mu1 > 0) &&  dimuon_m >= 12 && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nBjets_77 >= 1 && nJet20 >=3 && elec1Pt >= 20");

  RegionData CR0e = getRegionData("CR0e", "all", "CR0e","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==0 && nElectrons_signal == 1 && nMuons_signal == 2");
  RegionData CR1e = getRegionData("CR1e", "all", "CR1e","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==0 && nElectrons_signal == 1 && nMuons_signal == 2 && nBjets_77 == 2");
  RegionData CR2e = getRegionData("CR2e", "all", "CR2e","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==0 && nElectrons_signal == 1 && nMuons_signal == 2 && nBjets_77 == 1");
  RegionData CR3e = getRegionData("CR3e", "all", "CR3e","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==0 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3");
  RegionData CR4e = getRegionData("CR4e", "all", "CR4e","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==0 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 == 1");

  
  RegionData VR_Mmu1e = getRegionData("VR_Mmu1e", "all", "VR_Mmu1e","Mmu1e_os >=15 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=4 && nBjets_77 >= 1 && elec1Pt >= 20");
  RegionData VR_Mmu1e_50 = getRegionData("VR_Mmu1e_50", "all", "VR_Mmu1e_50","Mmu1e_os >= 50 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=4 && nBjets_77 >= 1 && elec1Pt >= 20");

  RegionData VR_Mmu2e = getRegionData("VR_Mmu2e", "all", " VR_Mmu2e","Mmu2e_os >= 15 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=4 && nBjets_77 >= 1 && elec1Pt >= 20");

  RegionData VR_Mmu2e_50 = getRegionData("VR_Mmu2e_50", "all", " VR_Mmu2e","Mmu2e_os >= 50 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=4 && nBjets_77 >= 1 && elec1Pt >= 20");
  
  vector<VariableData> vars;
  
 
 
  //pT frac variables 
   
  //vars.push_back(VariableData("pTfrac_mu2_mu1","pTfrac_mu2_mu1","pT frac mu2/mu1",20,0,1));
  
  /*vars.push_back(VariableData("pTfrac_el_mu1","pTfrac_e1_mu1","pT frac e1/mu1",20,0,2));
  vars.push_back(VariableData("pTfrac_mu2_e1","pTfrac_mu2_e1","pT frac mu2/e1",20,0,2));
  vars.push_back(VariableData("dimuon_m_SR4G","dimuon_m","Dimuon mass [GeV]",16,13,77));
  vars.push_back(VariableData("pTfrac_mu2_mu1_m2eOS","pTfrac_mu2_mu1_m2eOS","pT frac mu2/mu1 m2eOS",20,0,2));
  vars.push_back(VariableData("pTfrac_mu2_mu1_m2eSS","pTfrac_mu2_mu1_m2eSS;","pTfrac mu2/mu1 m2eSS;",20,0,2));
  vars.push_back(VariableData("pTfrac_e1_mu1_m1eSS","pTfrac_e1_mu1_m1eSS","pTfrac e1/mu1 m1eSS",20,0,2));
  vars.push_back(VariableData("pTfrac_e1_mu1_m1eOS","pTfrac_e1_mu1_m1eOS;","pTfrac e1/mu1 m1eOS",20,0,2));
  vars.push_back(VariableData("pTfrac_e1_mu1_m2eSS","pTfrac_e1_mu1_m2eSS;","pTfrac e1/mu1 m2eSS;",20,0,2));
  vars.push_back(VariableData("pTfrac_e1_mu1_m2eOS","pTfrac_e1_mu1_m2eOS","pTfrac e1/mu1 m2eOS",20,0,2));
  */ 
 
    
  //mass variables 
  //vars.push_back(VariableData("dimuon_pt","dimuon_pt","Dimuon p_{T} [GeV]",35,0,140));
  //vars.push_back(VariableData("dimuon_eta","dimuon_eta","Dimuon #eta", 20,-4,4));
  //vars.push_back(VariableData("dimuon_phi","dimuon_phi","Dimuon #phi", 20,-4,4));
  
    
  //vars.push_back(VariableData("leadBJetPt","leadBJetPt","leadBJetPt [GeV]",60,0,180));
  //vars.push_back(VariableData("dimuon_m","dimuon_m","Dimuon mass [GeV]",24,15,75));
  
  //vars.push_back(VariableData("dimuon_m","dimuon_m","Dimuon mass [GeV]",30,15,125));

  //vars.push_back(VariableData("dimuon_m","dimuon_m","Dimuon mass [GeV]",30,15,125));

  

  //vars.push_back(VariableData("dimuon_m_CRZ","dimuon_m","Dimuon mass [GeV]",8,75,105));
  //vars.push_back(VariableData("dimuon_m_CRtt","dimuon_m","Dimuon mass [GeV]",15,110,170));
  //vars.push_back(VariableData("Wmass","Wmass","W mass [GeV]",20,40,120));
  // vars.push_back(VariableData("dimuon_m_SR4G","dimuon_m","Dimuon mass [GeV]",15,15,75)); 
  //vars.push_back(VariableData("dimuon_m_SR1.5G","dimuon_m","Dimuon mass [GeV]",42,12,75));
  //vars.push_back(VariableData("dimuon_m_SR5G","dimuon_m","Dimuon mass [GeV]",12,15,75));

  //vars.push_back(VariableData("dimuon_m_SR4G","dimuon_m","Dimuon mass [GeV]",16,11,75)); 
  //vars.push_back(VariableData("dimuon_m_SR1.5G","dimuon_m","Dimuon mass [GeV]",43,12,76.5)); 
 
  //vars.push_back(VariableData("dimuon_m_SR2p5G_low","dimuon_m","Dimuon mass [GeV]",24,15,75));
  vars.push_back(VariableData("dimuon_m_SR2p5G_low","dimuon_m","Dimuon mass [GeV]",26,12,77));
  //vars.push_back(VariableData("dimuon_m_SR3G_high","dimuon_m","Dimuon mass [GeV]",7,56,77));
  //vars.push_back(VariableData("dimuon_m_SR1G","dimuon_m","Dimuon mass [GeV]",67,12,79));
  //vars.push_back(VariableData("dimuon_m_SR3G_low","dimuon_m","Dimuon mass [GeV]",16,12,60));
  //vars.push_back(VariableData("dimuon_m_SR3G","dimuon_m","Dimuon mass [GeV]",21,12,75));



  //vars.push_back(VariableData("MCweight","MCweight","MCweight",20,-10,10));
  //vars.push_back(VariableData("event_weight","event_weight","event_weight",20,-10,10));


  
  /*
  vars.push_back(VariableData("Mmu1e","Mmu1e","Mmu1e [GeV]",20,10,80));
  vars.push_back(VariableData("Mmu2e","Mmu2e","Mmu2e [GeV]",20,10,80));
  //vars.push_back(VariableData("Mmu1e_os","Mmu1e_os","Mmu1e_os [GeV]",15,15,75));
  //vars.push_back(VariableData("Mmu2e_os","Mmu2e_os","Mmu2e_os [GeV]",15,15,75));  
  //vars.push_back(VariableData("Mmu1e_ss","Mmu1e_ss","Mmu1e_ss [GeV]",20,10,80));
  //vars.push_back(VariableData("Mmu2e_ss","Mmu2e_ss","Mmu2e_ss [GeV]",20,10,80));  
  
  */

  /*
  vars.push_back(VariableData("m_ct","m_ct","Co-transverse mass, m_ct [GeV]",80,30,240));   
  vars.push_back(VariableData("mT_W","mT_W","mT_W_mu1 [GeV]",30,20,200));
  vars.push_back(VariableData("mT_W_e1","mT_W_e1","mT_W_e1 [GeV]",30,20,200));

  
  vars.push_back(VariableData("elec1Pt","elec1Pt","elec_{1}p_{T} [GeV]",25,0,100));
  vars.push_back(VariableData("dRm1m2","dRm1m2","#DeltaRm_{1}m_{2}",25,0,5));
  vars.push_back(VariableData("leadCJetPt","leadCJetPt","lead central Jet Pt [GeV]",70,0,180));
  vars.push_back(VariableData("dijetPt","dijetPt","diJet Pt [GeV]",60,0,180));
  vars.push_back(VariableData("met","met","MET [GeV]",35,0,140));
  vars.push_back(VariableData("mu1Pt","mu1Pt","#mu_{1}p_{T} [GeV]",35,0,140));
  vars.push_back(VariableData("mu2Pt","mu2Pt","#mu_{2}p_{T} [GeV]",15,0,75));
  vars.push_back(VariableData("jet1Pt","jet1Pt"," jet1 p_{T} [GeV]",60,0,120));
  vars.push_back(VariableData("jet2Pt","jet2Pt"," jet2 p_{T} [GeV]",60,0,120));
  
  

  //eta and pt 
  
  vars.push_back(VariableData("dRm1m2","dRm1m2","#DeltaRm_{1}m_{2}",20,0,5));
  vars.push_back(VariableData("dRm1m2_log","dRm1m2","#DeltaRm_{1}m_{2}",50,0,5,"log"));
  //vars.push_back(VariableData("klbbmm_m","klbbmm_m","kl bb#mu#mu mass [GeV]", 60,0,360,"log"));
  //vars.push_back(VariableData("elec1Pt","elec1Pt","elec_{1}p_{T} [GeV]",70,0,140));
  vars.push_back(VariableData("bjet1_pt","bjet1_pt","b_{1}p_{T} [GeV]",60,0,120));
  vars.push_back(VariableData("bjet2_pt","bjet2_pt","b_{2}p_{T} [GeV]",60,0,120));
  vars.push_back(VariableData("mu1Eta","mu1Eta","#mu_{1}#eta",30,-3,3));
  vars.push_back(VariableData("mu2Eta","mu2Eta","#mu_{2}#eta",30,-3,3)); 
  vars.push_back(VariableData("leadCJetPt","leadCJetPt","lead central Jet Pt [GeV]",60,0,180));
  vars.push_back(VariableData("leadBJetPt","leadBJetPt","leadBJetPt [GeV]",60,0,180));  
  vars.push_back(VariableData("mu1Phi","mu1Phi","#phim_{1}",16,-4,4));
  vars.push_back(VariableData("mu2Phi","mu2Phi","#phim_{2}",16,-4,4));
  
 
 //angle variables
  
  vars.push_back(VariableData("dRMmmMbb","dRMmmMbb","#DeltaRM_{#mu#mu}M_{bb}",60,0,5));
  //vars.push_back(VariableData("dRMmmMbbkl","dRMmmMbbkl","kl #DeltaRM_{mm}M_{bb}",60,0,5));
  vars.push_back(VariableData("dRm1b1","dRm1b1","#DeltaRm_{1}b_{1}",50,0,5));
  vars.push_back(VariableData("dRm1b2","dRm1b2","#DeltaRm_{1}b_{2}",50,0,5));
  vars.push_back(VariableData("dRm2b2","dRm2b2","#DeltaRm_{2}b_{2}",50,0,5));
  vars.push_back(VariableData("dRm2b1","dRm2b1","#DeltaRm_{2}b_{1}",50,0,5)); 
  vars.push_back(VariableData("dRb1b2","dRb1b2","#DeltaRb_{1}b_{2}",50,0,5));
  vars.push_back(VariableData("dRmb1","dRmb1","#DeltaR largest ang. mu and bjet",20,0,5)); 
  vars.push_back(VariableData("dRmb4","dRmb4","#DeltaR smallest ang. mu and bjet",20,0,5)); 
  vars.push_back(VariableData("dRm1e1","dRm1e1","#DeltaRm_{1}e_{1}",20,0,5));
  vars.push_back(VariableData("dRm2e1","dRm2e1","#DeltaRm_{2}e_{1}",20,0,5));
  vars.push_back(VariableData("dRMmmW","dRMmmW","#DeltaRM_{#mu#mu}M_{W}",20,0,5));
  vars.push_back(VariableData("dRMmmb1","dRMmmb1","#DeltaRM_{#mu#mu}b_{1}",20,0,5)); 
  vars.push_back(VariableData("dRMmmb2","dRMmmb2","#DeltaRM_{#mu#mu}b_{2}",20,0,5));
  vars.push_back(VariableData("dPhimumujj","dPhimumujj","#DelataPhiM#mu#mujj",20,0,5));
  vars.push_back(VariableData("dRMmme1","dRMmme1","#DeltaRM_{#mu#mu}e_{1}",20,0,5)); 
  vars.push_back(VariableData("centrality","centrality","Centrality",20,0,1));
  
  */     
  
  //Multiplicity 
 
    //vars.push_back(VariableData("nElectrons_signal","nElectrons_signal","nElectrons_signal",6,-.5,5.5));
    //vars.push_back(VariableData("nMuons_signal","nMuons_signal","nMuons_signal",6,-.5,5.5));
    //vars.push_back(VariableData("nMuons_signal_log","nMuons_signal","nMuons_signal",6,-.5,5.5,"log"));
  
    //vars.push_back(VariableData("njetcentral20","njetcentral20","njetcentral20",8,-.5,7.5));
    //vars.push_back(VariableData("njetfwd20","njetfwd20","njetfwd20",6,-.5,5.5));
    //vars.push_back(VariableData("nbjet30_77","nbjet30_77","nbjet30_77",6,-.5,5.5));
    //vars.push_back(VariableData("nbjet30_60","nbjet30_60","nbjet30_60",6,-.5,5.5));    
    //vars.push_back(VariableData("nJet20","nJet20","nJet20",8,-.5,7.5));
    //vars.push_back(VariableData("nBjets_77","nBjets_77","nBjets_77",6,-.5,5.5));
 
    /*   
    vars.push_back(VariableData("isTrueMu1","isTrueMu1","isTrueMu1 ",3,-.5,2.5));
    vars.push_back(VariableData("isTrueMu2","isTrueMu2","isTrueMu2",3,-.5,2.5)); 
    vars.push_back(VariableData("isTrueElec","isTrueElec","isTrueElec",3,-.5,2.5));
    vars.push_back(VariableData("isFakeMu1","isFakeMu1","isFakeMu1 ",3,-.5,2.5));
    vars.push_back(VariableData("isFakeMu2","isFakeMu2","isFakeMu2",3,-.5,2.5));
    vars.push_back(VariableData("isFakeElec","isFakeElec","isFakeElec",3,-.5,2.5));   
    vars.push_back(VariableData("isPhoConvMu1","isPhoConvMu1","isPhoConvMu1 ",3,-.5,2.5));
    vars.push_back(VariableData("isPhoConvMu2","isPhoConvMu2","isPhoConvMu2",3,-.5,2.5));
    vars.push_back(VariableData("isPhoConvElec","isPhoConvElec","isPhoConvElec",3,-.5,2.5));

   */

    //vars.push_back(VariableData("muOrigin","muOrigin","muOrigin",21,-.5,20.5));
    //vars.push_back(VariableData("isTrueMu1","isTrueMu1","isTrueMu1",2,0,2));
    //vars.push_back(VariableData("isTrueMu2","isTrueMu2","isTrueMu2",2,0,2));
   //vars.push_back(VariableData("isTrueElec","isTrueElec","isTrueElec",2,0,2));
    



    /* 


    vars.push_back(VariableData("klfit_loglikelihood","klfit_loglikelihood","klfit", 60,-30,-2));
    vars.push_back(VariableData("dibjet_m","dibjet_m","Dibjet mass [GeV]",60,0,160,"log"));
    vars.push_back(VariableData("bbmm_m","bbmm_m","bb#mu#mu mass [GeV]",60,60,300,"log"));
    vars.push_back(VariableData("klbb_m","klbb_m","klbb mass [GeV]",60,10,80,"log"));
    vars.push_back(VariableData("massdiff","massdiff","#Deltam [GeV]",30,-30,30,"log"));
    vars.push_back(VariableData("klmassdiff","klmassdiff","kl #Delta m [GeV]",30,-10,10,"log"));
    vars.push_back(VariableData("bjet1Eta","bjet1Eta","b_{1} #eta",30,-3,3));
    vars.push_back(VariableData("bjet2Eta","bjet2Eta","b_{2} #eta",30,-3,3));   
    vars.push_back(VariableData("dibjet_pt","dibjet_pt","Dibjet p_{T} [GeV]",60,0,180));
    vars.push_back(VariableData("klfit_dibjet_pt","klfit_dibjet_pt","kl Dibjet p_{T} [GeV]",120,0,240));
   
    vars.push_back(VariableData("Higgs_pt","TMath::Sqrt(dimuon_pt*dimuon_pt+dibjet_pt*dibjet_pt+2*dimuon_pt*dibjet_pt*TMath::Cos(dibjet_phi-dimuon_phi))", "Higgs p_{T}",60,0,140));
    
    //vars.push_back(VariableData("BDT_a20_dR_stdcut","BDT_a20_dR_stdcut","BDT_a20_dR_stdcut", 60,-1,1));
    //vars.push_back(VariableData("BDT_a30_dR_stdcut","BDT_a30_dR_stdcut","BDT_a30_dR_stdcut", 60,-1,1));
    //vars.push_back(VariableData("BDT_a40_dR_stdcut","BDT_a40_dR_stdcut","BDT_a40_dR_stdcut", 60,-1,1));
    //vars.push_back(VariableData("BDT_a50_dR_stdcut","BDT_a50_dR_stdcut","BDT_a50_dR_stdcut", 60,-1,1));
    //vars.push_back(VariableData("BDT_a60_dR_stdcut","BDT_a60_dR_stdcut","BDT_a60_dR_stdcut", 60,-1,1));
    //vars.push_back(VariableData("BDT_a20_dR_dimuon_m_16_24","BDT_a20_dR_dimuon_m_16_24","BDT_a20_dR_dimuon_m_16_24", 60,-1,1));
    //vars.push_back(VariableData("BDT_a30_dR_dimuon_m_26_34","BDT_a30_dR_dimuon_m_26_34","BDT_a30_dR_dimuon_m_26_34", 60,-1,1));
    //vars.push_back(VariableData("BDT_a40_dR_dimuon_m_36_44","BDT_a40_dR_dimuon_m_36_44","BDT_a40_dR_dimuon_m_36_44", 60,-1,1));
    //vars.push_back(VariableData("BDT_a50_dR_dimuon_m_46_54","BDT_a50_dR_dimuon_m_46_54","BDT_a50_dR_dimuon_m_46_54", 60,-1,1));
    //vars.push_back(VariableData("BDT_a60_dR_dimuon_m_56_64","BDT_a60_dR_dimuon_m_56_64","BDT_a60_dR_dimuon_m_56_64", 60,-1,1));

    vars.push_back(VariableData("BDT_a20_dR_dimuon_m_16_24_config5","BDT_a20_dR_dimuon_m_16_24_config5","BDT_a20_dR_dimuon_m_16_24_config5", 60,-1,1));
    vars.push_back(VariableData("BDT_a30_dR_dimuon_m_26_34_config5","BDT_a30_dR_dimuon_m_26_34_config5","BDT_a30_dR_dimuon_m_26_34_config5", 60,-1,1));
    vars.push_back(VariableData("BDT_a40_dR_dimuon_m_36_44_config5","BDT_a40_dR_dimuon_m_36_44_config5","BDT_a40_dR_dimuon_m_36_44_config5", 60,-1,1));
    vars.push_back(VariableData("BDT_a50_dR_dimuon_m_46_54_config5","BDT_a50_dR_dimuon_m_46_54_config5","BDT_a50_dR_dimuon_m_46_54_config5", 60,-1,1));
    vars.push_back(VariableData("BDT_a60_dR_dimuon_m_56_64_config5","BDT_a60_dR_dimuon_m_56_64_config5","BDT_a60_dR_dimuon_m_56_64_config5", 60,-1,1));*/
  
  for(auto var : vars){
    
    //drawDataNorm(reader, var, all);
    drawDataNorm(reader, var,SR);
    //drawDataNorm(reader, var,CRss);
    //drawDataNorm(reader, var,CRttZ);
    //drawDataNorm(reader, var,CRZ);
    //drawDataNorm(reader, var,SR );
     
      /*
      drawDataNorm(reader, var, SR20,"a20");
          
      drawDataNorm(reader, var, SR30,"a30");
      drawDataNorm(reader, var, SR40,"a40");
      drawDataNorm(reader, var, SR50,"a50");
      drawDataNorm(reader, var, SR60,"a60");

      drawDataSign(reader, var, SR20,"a20");
      drawDataSign(reader, var, SR30,"a30");
      drawDataSign(reader, var, SR40,"a40");
      drawDataSign(reader, var, SR50,"a50");
      drawDataSign(reader, var, SR60,"a60");*/

    //signficance plots
    
    /*
 
    drawDataSign(reader, var, SR0e,"H160a15");
    //drawDataSign(reader, var, SR0e,"H160a15");
    //drawDataSign(reader, var, SR0e,"H160a30");
    drawDataSign(reader, var, SR0e,"H100a15"); 
    drawDataSign(reader, var, SR0e,"H160a75"); 
     
   
    //drawDataSign(reader, var, SR1e,"H160a30");
    //drawDataSign(reader, var, SR1e,"H160a60");
    //drawDataSign(reader, var, SR1e,"H160a45");    
    
    //drawDataSign(reader, var, SR2e,"H160a30");
    //drawDataSign(reader, var, SR2e,"H160a60");
    //drawDataSign(reader, var, SR2e,"H160a45");
  
    drawDataSign(reader, var, SR3e,"H120a15");
    drawDataSign(reader, var, SR3e,"H160a15");
    drawDataSign(reader, var, SR3e,"H100a15");
    drawDataSign(reader, var, SR3e,"H160a75");
    //drawDataSign(reader, var, SR0e,"H160a75");
   
    drawDataSign(reader, var, SR,"H160a15");
    drawDataSign(reader, var, SR,"H100a15");
    drawDataSign(reader, var, SR,"H120a15");
    drawDataSign(reader, var, SR,"H160a75");

    */
    /*
    drawDataSign(reader, var, SR4e,"H160a15");
    drawDataSign(reader, var, SR4e,"H160a30");
    drawDataSign(reader, var, SR4e,"H160a75");
    drawDataSign(reader, var, SR4e,"H160a60");
    drawDataSign(reader, var, SR4e,"H160a45");
    

    //drawDataSign(reader, var, SR2,"H160a45");
    //drawDataSign(reader, var, SR2,"H160a15");
    //drawDataSign(reader, var, SR2,"H160a30");
    //drawDataSign(reader, var, SR2,"H160a60"); 
    //drawDataSign(reader, var, SR2,"H160a75"); 

    

    drawDataSign(reader, var, SR0e,"H100a15");
    drawDataSign(reader, var, SR0e,"H120a15");
    drawDataSign(reader, var, SR0e,"H120a30");
    drawDataSign(reader, var, SR0e,"H140a15"); 
    drawDataSign(reader, var, SR0e,"H140a30");
    drawDataSign(reader, var, SR0e,"H140a45"); 

    drawDataSign(reader, var, SR3e,"H100a15");
    drawDataSign(reader, var, SR3e,"H120a15");
    drawDataSign(reader, var, SR3e,"H120a30");
    drawDataSign(reader, var, SR3e,"H140a15");
    drawDataSign(reader, var, SR3e,"H140a30");
    drawDataSign(reader, var, SR3e,"H140a45");

    drawDataSign(reader, var, SR4e,"H100a15");
    drawDataSign(reader, var, SR4e,"H120a15");
    drawDataSign(reader, var, SR4e,"H120a30");
    drawDataSign(reader, var, SR4e,"H140a15");
    drawDataSign(reader, var, SR4e,"H140a30");
    drawDataSign(reader, var, SR4e,"H140a45");
       
    */

    //MC-data plots 

    //drawDataStack(reader, var, all);
    //drawDataStack(reader, var, SR,"None");
    //drawDataStack(reader, var, CRss,"None");

    /*
    drawDataStack(reader, var, SR0e);
    drawDataStack(reader, var, SR1e); 
    drawDataStack(reader, var, SR2e);
    drawDataStack(reader, var, SR3e);
    drawDataStack(reader, var, SR4e);
    drawDataStack(reader, var, SR,"None"); 
    
    drawDataStack(reader, var, CR0e);
    drawDataStack(reader, var, CR1e);
    drawDataStack(reader, var, CR2e);
    drawDataStack(reader, var, CR3e);
    drawDataStack(reader, var, CR4e);
    */

    //drawDataStack(reader, var, CR2e,"None");
    //drawDataStack(reader, var, CR2e);
    //drawDataStack(reader, var, CRe,"None");
    //drawDataStack(reader, var, CRss,"None");
    //drawDataStack(reader, var, CR,"None");
    //drawDataStack(reader, var, all,"None");
    //drawDataStack(reader, var, CRe0fake);
    //drawDataStack(reader, var, CRe0j4fake,"None");
    //drawDataStack(reader, var, CRe0j4fake);
    //drawDataStack(reader, var,CRe1fake,"None");
    //drawDataStack(reader, var,CRe1j4fake);
    //drawDataStack(reader, var,CRe,"None");
    //drawDataStack(reader,var,VR,"None");
    //drawDataStack(reader, var,VR_Mmu1e_50);
    //drawDataStack(reader, var,VR_Mmu2e);
    //drawDataStack(reader, var,VR_Mmu2e_50);
 
    //drawDataStack(reader, var,SR);
    //drawDataStack(reader, var,SR,"None");

    //No data plots ..mainly just for the SR to see it without data

    //rawNoDataStack(reader, var, CRe0j4fake);
    //drawNoDataStack(reader, var, SR0e);
    //drawNoDataStack(reader, var, SR3e);
    //drawNoDataStack(reader, var, SR,"None");
    //drawNoDataStack(reader, var, SR3e);
    //drawNoDataStack(reader, var, SR,"None"); 
    //drawNoDataStack(reader, var, SR4e,"None");
 
    //drawDataStack(reader, var, CR);
    //drawDataStack(reader, var, CR_new);
    //drawDataStack(reader, var, noDimuon);
    //drawDataStack(reader, var, noHiggs);
    //drawDataStack(reader, var, SR20,"a20");
    //drawDataStack(reader, var, SR30,"a30");
    //drawDataStack(reader, var, SR40,"a40");
    //drawDataStack(reader, var, SR50,"a50");
    //drawDataStack(reader, var, SR60,"a60");
    //drawDataStack(reader, var, TCR);
    //drawDataStack(reader, var, SB1);
    //drawDataStack(reader, var, SB2);
    //drawDataStack(reader, var, all);
    //drawDataStack(reader, var, presel);

  }


}

void readData(OutputReader* reader){

  int lumi = 1.; 
  TString data_dir = getenv("HBBMUMU_DATA_PATH");
  TString directory = data_dir + "/data/";
  TString rundate = date; 
  TString rundate2 = "data";
  vector <TString> BDT_trees = TreeNameVector("BDTTreeNames.txt");
  /*
    for(auto tree_name:BDT_trees){
    if (tree_name.Index("data") != -1){
    reader->readTree(tree_name,"all",directory+"AllNewBDTFiles.root",tree_name);
    }
    if (tree_name.Index("DDZ") != -1){
    reader->readTree(tree_name,"all",directory+"AllNewBDTFiles.root", tree_name, lumi/100);
    }
    if (tree_name.Index("ttbar") != -1){
    reader->readTree(tree_name,"all",directory+"AllNewBDTFiles.root", tree_name, lumi);
    }
    if (tree_name.Index("0__") != -1){
    reader->readTree(tree_name,"all",directory+"AllNewBDTFiles.root", tree_name, lumi/10);
    }
    if (tree_name.Index("db") != -1){
    reader->readTree(tree_name,"all",directory+"AllNewBDTFiles.root", tree_name, lumi/100);
    }
    if (tree_name.Index("st") != -1){
    reader->readTree(tree_name,"all",directory+"AllNewBDTFiles.root", tree_name, lumi/100);
    }
    if (tree_name.Index("Wjets") != -1){
    reader->readTree(tree_name,"all",directory+"AllNewBDTFiles.root", tree_name, lumi/100);
    }
    if (tree_name.Index("ttV") != -1){
    reader->readTree(tree_name,"all",directory+"AllNewBDTFiles.root", tree_name, lumi/100);
    }
    }
  */


  //reader->readTree("data","all",directory+"AllNewBDTFiles.root","data");
  //reader->readTree("data","all","/home/adriana/HFbbmumu/rootFiles/rel21/skimSlimNtuples/FwdJets2Mu/NoSysBDTTraining/TrainBDT.root","data");
  
  //reader->readTree("DDZ","all",directory+"AllNewBDTFiles.root","DDZ__Nominal",lumi*0.0113);
  //reader->readTree("DDZ","all","/home/adriana/HFbbmumu/rootFiles/rel21/skimSlimNtuples/FwdJets2Mu/NoSysBDTTraining/TrainBDT.root","DDZ__Nominal",290);
  //reader->readTree("ttbar","all",directory+"AllNewBDTFiles.root","ttbar__Nominal",lumi*1.264);
  //reader->readTree("ttbar","all","/home/adriana/HFbbmumu/rootFiles/rel21/skimSlimNtuples/FwdJets2Mu/NoSysBDTTraining/TrainBDT.root","ttbar__Nominal",0.95);
   
   
  //reader->readTree("data","all","/eos/home-w/waahmed/afs_runs/new_ptag/data/2020/"+rundate+"/"+rundate2+"_a.root","data");
  //reader->readTree("data","all","/eos/home-w/waahmed/afs_runs/new_ptag/data/2020/"+rundate+"/"+rundate2+"_d.root","data");
  //reader->readTree("data","all","/eos/home-w/waahmed/afs_runs/new_ptag/data/2020/"+rundate+"/"+rundate2+"_e.root","data");

  //iso-studies

  //reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov2/ttbar_16a_looseMuElec_newdef6.root","ttbar__Nominal"); //.95 for CRttbar, .5 for CRfakes
  //reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov9/ttbar_16a_dilep_truthelec.root","ttbar__Nominal");
  //reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov5/ttbar_16a_tightElec.root","ttbar__Nominal"); //.95 for CRttbar, .5 for CRfakes
  //reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/jan4/ttbar_16a_looseMuElec.root","ttbar__Nominal");   
 
  
   
  //reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov9/ttbar_16a_dilep.root","ttbar__Nominal");
  //reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov9/ttbar_16d_dilep.root","ttbar__Nominal");
  //reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov9/ttbar_16e_dilep.root","ttbar__Nominal");


 
  //reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/ttbar_16d.root","ttbar__Nominal",.59);
  //reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/ttbar_16e.root","ttbar__Nominal",.59);
  
  //formate for when I ran locally, not on condor
  /*
  reader->readTree("Zjets","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/zjets_16d.root","Zjets__Nominal"); //1.1 for CRttbar
  reader->readTree("Zjets","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/zjets_16e.root","Zjets__Nominal");
  reader->readTree("Zjets","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/zjets_16a.root","Zjets__Nominal");

  reader->readTree("Wjets","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/Wjets_16a.root","Wjets__Nominal"); 
  reader->readTree("Wjets","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/Wjets_16d.root","Wjets__Nominal");
  reader->readTree("Wjets","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/Wjets_16e.root","Wjets__Nominal");
  
  reader->readTree("DiBos","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/diBoson_16a.root","diBoson__Nominal"); 
  reader->readTree("DiBos","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/diBoson_16d.root","diBoson__Nominal");
  reader->readTree("DiBos","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/diBoson_16e.root","diBoson__Nominal");
  
  reader->readTree("SingTop","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/singleTop_16a.root","singleTop__Nominal"); 
  reader->readTree("SingTop","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/singleTop_16d.root","singleTop__Nominal");
  reader->readTree("SingTop","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/singleTop_16e.root","singleTop__Nominal"); 

  reader->readTree("ttV","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/ttV_16a.root","ttV__Nominal"); 
  reader->readTree("ttV","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/ttV_16d.root","ttV__Nominal");
  reader->readTree("ttV","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/ttV_16e.root","ttV__Nominal"); 
  
  reader->readTree("data","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/data/tree_NoSys.root","data");
  reader->readTree("data","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/data/tree_NoSys.root","data");
  reader->readTree("data","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/data/tree_NoSys.root","data")
  */

  //reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/ttbar_16a.root","ttbar__Nominal"); //.95 for CRttbar, .5 for CRfakes
  //reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/ttbar_16d.root","ttbar__Nominal");
  //reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/"+rundate+"/ttbar_16a.root","ttbar__Nominal");

  //format with runs on condor

  //reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16a/ttbar/ttbar_16a_dilep.root","ttbar__Nominal"); //.95 for CRttbar, .5 for CRfakes
  //reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16d/ttbar/ttbar_16d_dilep.root","ttbar__Nominal");
  //reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16e/ttbar/ttbar_16e_dilep.root","ttbar__Nominal");
    
  reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16a/ttbar/tree_NoSys.root","ttbar__Nominal"); //.95 for CRttbar, .5 for CRfakes
  reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16d/ttbar/tree_NoSys.root","ttbar__Nominal");
  reader->readTree("ttbar","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16e/ttbar/tree_NoSys.root","ttbar__Nominal");
  
  //ttbar theory 
  reader->readTree("ttbar_AF2","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16a/ttbar/tree_AF2Nominal.root","ttbar__AF2Nominal"); //.95 for CRttbar, .5 for CRfakes
  reader->readTree("ttbar_AF2","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16d/ttbar/tree_AF2Nominal.root","ttbar__AF2Nominal");
  reader->readTree("ttbar_AF2","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16e/ttbar/tree_AF2Nominal.root","ttbar__AF2Nominal");  

  reader->readTree("ttbar_genSys","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16a/ttbar/tree_genSys.root","ttbar__genSysNominal"); //.95 for CRttbar, .5 for CRfakes
  reader->readTree("ttbar_genSys","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16d/ttbar/tree_genSys.root","ttbar__genSysNominal");
  reader->readTree("ttbar_genSys","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16e/ttbar/tree_genSys.root","ttbar__genSysNominal");
 
  reader->readTree("ttbar_radDown","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16a/ttbar/tree_radDown.root","ttbar__radDownSysNominal"); //.95 for CRttbar, .5 for CRfakes
  reader->readTree("ttbar_radDown","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16d/ttbar/tree_radDown.root","ttbar__radDownSysNominal");
  reader->readTree("ttbar_radDown","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16e/ttbar/tree_radDown.root","ttbar__radDownSysNominal");
 
  reader->readTree("ttbar_radUp","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16a/ttbar/tree_radUp.root","ttbar__radUpSysNominal"); //.95 for CRttbar, .5 for CRfakes
  reader->readTree("ttbar_radUp","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16d/ttbar/tree_radUp.root","ttbar__radUpSysNominal");
  reader->readTree("ttbar_radUp","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16e/ttbar/tree_radUp.root","ttbar__radUpSysNominal");
 
  reader->readTree("ttbar_showerSys","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16a/ttbar/tree_showerSys.root","ttbar__showerSysNominal");
  reader->readTree("ttbar_showerSys","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16d/ttbar/tree_showerSys.root","ttbar__showerSysNominal");
  reader->readTree("ttbar_showerSys","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/nov13/mc16e/ttbar/tree_showerSys.root","ttbar__showerSysNominal");
 
    
  reader->readTree("data","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16a/data/tree_NoSys.root","data");
  reader->readTree("data","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16d/data/tree_NoSys.root","data");
  reader->readTree("data","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16e/data/tree_NoSys.root","data");
  
  reader->readTree("Zjets","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16a/Zjets/tree_NoSys.root","Zjets__Nominal"); //1.1 for CRttbar
  reader->readTree("Zjets","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16d/Zjets/tree_NoSys.root","Zjets__Nominal");
  reader->readTree("Zjets","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16e/Zjets/tree_NoSys.root","Zjets__Nominal");
  //reader->readTree("Zjets","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/mar1/mc16e/Zjets/tree_NoSys.root","Zjets__Nominal");

  reader->readTree("Wjets","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16a/Wjets/tree_NoSys.root","Wjets__Nominal");
  reader->readTree("Wjets","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16d/Wjets/tree_NoSys.root","Wjets__Nominal");
  reader->readTree("Wjets","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16e/Wjets/tree_NoSys.root","Wjets__Nominal");

  reader->readTree("DiBos","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16a/diBoson/tree_NoSys.root","diBoson__Nominal");
  reader->readTree("DiBos","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16d/diBoson/tree_NoSys.root","diBoson__Nominal");
  reader->readTree("DiBos","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16e/diBoson/tree_NoSys.root","diBoson__Nominal");

  reader->readTree("SingTop","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16a/singleTop/tree_NoSys.root","singleTop__Nominal");
  reader->readTree("SingTop","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16d/singleTop/tree_NoSys.root","singleTop__Nominal");
  reader->readTree("SingTop","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16e/singleTop/tree_NoSys.root","singleTop__Nominal");

  reader->readTree("ttV","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16a/ttV/tree_NoSys.root","ttV__Nominal");
  reader->readTree("ttV","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16d/ttV/tree_NoSys.root ","ttV__Nominal");
  reader->readTree("ttV","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16e/ttV/tree_NoSys.root","ttV__Nominal");
 
  reader->readTree("ttZ","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16a/ttZ/tree_NoSys.root","ttZ__Nominal");
  reader->readTree("ttZ","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16d/ttZ/tree_NoSys.root ","ttZ__Nominal");
  reader->readTree("ttZ","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16e/ttZ/tree_NoSys.root","ttZ__Nominal");

  reader->readTree("ttW","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16a/ttW/tree_NoSys.root","ttW__Nominal");
  reader->readTree("ttW","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16d/ttW/tree_NoSys.root ","ttW__Nominal");
  reader->readTree("ttW","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/"+rundate+"/mc16e/ttW/tree_NoSys.root","ttW__Nominal");

  //checking compoistion of Z +HF
  reader->readTree("Zjets_B","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/jun16/zjets_Bfilter_16a.root","Zjets__Nominal"); //1.1 for CRttbar
  reader->readTree("Zjets_B","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/jun16/zjets_Bfilter_16d.root","Zjets__Nominal");
  reader->readTree("Zjets_B","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/jun16/zjets_Bfilter_16e.root","Zjets__Nominal");

  reader->readTree("Zjets_C","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/jun16/zjets_Cfilter_16a.root","Zjets__Nominal"); //1.1 for CRttbar
  reader->readTree("Zjets_C","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/jun16/zjets_Cfilter_16d.root","Zjets__Nominal");
  reader->readTree("Zjets_C","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/jun16/zjets_Cfilter_16e.root","Zjets__Nominal");

  reader->readTree("Zjets_other","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/jun16/zjets_other_16a.root","Zjets__Nominal"); //1.1 for CRttbar
  reader->readTree("Zjets_other","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/jun16/zjets_other_16d.root","Zjets__Nominal");
  reader->readTree("Zjets_other","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/jun16/zjets_other_16e.root","Zjets__Nominal");

  reader->readTree("Zjets_BplusC","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/jun23/mc16a/Zjets/Zjets_BplusC_16a.root","Zjets__Nominal");
  reader->readTree("Zjets_BplusC","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/jun23/mc16d/Zjets/Zjets_BplusC_16d.root ","Zjets__Nominal");
  reader->readTree("Zjets_BplusC","all","/eos/home-w/waahmed/afs_runs/new_ptag/bkg/2020/jun23/mc16e/Zjets/Zjets_BplusC_16e.root","Zjets__Nominal");


  ///mc16e/Zjets/Zjets_BplusC_16e.root 

    /*
    reader->readTree("ttV","all",directory+"AllNewBDTFiles.root","ttV__Nominal",lumi);
    reader->readTree("Wjets","all","/eos/home-w/waahmed/afs_runs/bkg/Wjets_jun2/mc16a/Wjets/tree_NoSys.root","Wjets__Nominal",lumi);
    reader->readTree("a20","all",directory+"AllNewBDTFiles.root","a20__Nominal",lumi*0.2);
    reader->readTree("a30","all",directory+"AllNewBDTFiles.root","a30__Nominal",lumi*0.12);
    reader->readTree("a40","all",directory+"AllNewBDTFiles.root","a40__Nominal",lumi*0.32);
    reader->readTree("a50","all",directory+"AllNewBDTFiles.root","a50__Nominal",lumi*0.53);
    reader->readTree("a60","all",directory+"AllNewBDTFiles.root","a60__Nominal",lumi*0.21);*/



}

void readSignal(OutputReader* reader){
  //double scaleXsec = 48.52 * 1.6e-3 / 14.15;  
  //double scaleXsec = 48.52 * 1.6e-3 / 1; //different xsec in the ntuples, assuming for now that all xsecs in the ntuples are 14.15, next time running put to 1 because xsec now corrected in the skimslim - need to correct this - WA
  double scaleXsec = 831.76 *1* 1 *3e-4 /2 ; // divide by 2 as it is the default xsec in ntuples for 160 GeV
  
  TString data_dir = getenv("HBBMUMU_DATA_PATH");
  //TString directory =   "/eos/home-w/waahmed/afs_runs/old_ptag/signal/jun2/";
  TString rundate = date;


  //TString directory = "/eos/home-w/waahmed/afs_runs/new_ptag/sig/" + rundate+"/new_xsec/";
  //TString directory = "/eos/home-w/waahmed/afs_runs/new_ptag/sig/2020/" + rundate +"/scaled/";
  TString directory = "/eos/home-w/waahmed/afs_runs/new_ptag/sig/2020/" + rundate +"/"; 

  //vector<TString> signalvector = {"14", "16", "18", "20", "25", "30", "40", "50", "55", "60", "62"};
  //vector<float> signalScale = {0.2, 0.2, 0.2, 0.2, 0.15, 0.12, 0.32, 0.53, 0.35, 0.21, 0.21}; //put by hand, interpolation by eye
  //vector<TString> signalvector = {"30"};
  vector<TString> signalvector = {"H100a15","H120a15","H120a30","H140a15","H140a30","H140a45","H160a15","H160a30","H160a45","H160a60","H160a75"};
  //vector<TString> signalvector = {"H140a30","H140a45"}; 
  vector<float> signalScale = {1}; // where to get this number from? WA

  for (unsigned int i=0; i<signalvector.size(); ++i){
    TString signal = signalvector.at(i);
    //reader->readTree("a"+signal,"all",directory+"SignalBDTFiles.root","signal"+signal+"__Nominal",scaleXsec*signalScale[i]);
    //reader->readTree("a"+signal,"all",directory+"a1"+signal+"/dummyyear/dummysample/tree_NoSys.root","dummysample__Nominal"); 
    //reader->readTree("a"+signal,"all",directory+"/sig30_16d.root","signal30__Nominal",.30); 
    reader->readTree(signal,"all",directory+signal+"_16a.root", signal+"__Nominal"); //use .1 for Br(t->bH) ==.1 with scaled MC. Use .2 for SR for 160 H+ plots, .015 for MPs for the unscaled to make plots look nice. 
    reader->readTree(signal,"all",directory+signal+"_16d.root", signal+"__Nominal");
    reader->readTree(signal,"all",directory+signal+"_16e.root", signal+"__Nominal");
  }
}


float getYields(OutputReader* reader, RegionData Region, TString sample_name){
 
  TString eventweight =buildEventWeight("event_weight",Region.cuts);
  TString no_eventweight =buildEventWeight("",Region.cuts);
  //TH1* h_yield = reader->getHistoFromTree(sample_name,"all","1",sample_name,1,0,2,no_eventweight); //without EW
  TH1* h_yield = reader->getHistoFromTree(sample_name,"all","1",sample_name,1,0,2,eventweight);
  return h_yield->GetBinContent(1);

}

void dumpRegions(OutputReader* reader){

  //RegionData TCR = getRegionData("TCR", "all", "TCR","klbbmm_m>110 && klbbmm_m<140 && met>60 && dimuon_m>16 && dimuon_m<64 && klfit_loglikelihood>-8",2);
  //RegionData SB = getRegionData("SB", "all", "SB","klbbmm_m > 80 && (klbbmm_m<110 || klbbmm_m>140) && klbbmm_m<170  && met<60 && dimuon_m>16 && dimuon_m<64 && klfit_loglikelihood>-8",2);

  
  RegionData CRe0j4fake = getRegionData("CRe0j4fake", "all", "CRe0j4fake","dimuon_m < 80 && nMuons_signal == 2 && nElectrons_signal == 0 && nJet20 >=4 && nBjets_77 >= 1");
  RegionData CRe1j4fake = getRegionData("CRe1j4fake", "all", "CRe1j4fake","dimuon_m > 75 && dimuon_m < 105 && nElectrons_signal > 0 && nBjets_77 >= 1");

  RegionData CRe = getRegionData("CRe", "all", "CRe","dimuon_m > 110 && dimuon_m < 170 && nElectrons_signal > 0");
 
  RegionData CRss = getRegionData("CRss", "all","CRss","( mu2Pt/mu1Pt > .2 || Mmu1e_ss > 0 ) && dimuon_m >= 12 && dimuon_m <= 77 && nElectrons_signal == 1 && nMuons_signal ==2 && isOS ==0 && elec1Pt >= 20 && nBjets_77 >= 1 && nJet20 >=3");

  RegionData CRZ = getRegionData("CRZ", "all", "CRZ"," ( mu2Pt/mu1Pt > .2 || Mmu1e_ss > 0 ) && dimuon_m >= 78 && dimuon_m <= 102 && nElectrons_signal == 1 && nMuons_signal ==2 && isOS ==1 && elec1Pt <= 20 && nBjets_77 >= 1 && nJet20 >=3");
  RegionData CRttZ = getRegionData("CRttZ", "all", "CRttZ"," ( mu2Pt/mu1Pt > .2 || Mmu1e_ss > 0 ) && dimuon_m >= 78 && dimuon_m <= 102 && nElectrons_signal == 1 && nMuons_signal ==2 && isOS ==1 && elec1Pt >= 20 && nBjets_77 >= 1 && nJet20 >=3");

  RegionData VR = getRegionData("VR", "all", "VR","( mu2Pt/mu1Pt > .2 || Mmu1e_ss > 0) && Mmu1e_os >=30 && Mmu1e_os <=110 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20");
   

  RegionData CRe1 = getRegionData("CRe1", "all", "CRe1","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==0 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20");

  RegionData SR2 = getRegionData("SR2", "all", "SR2","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==1 && nMuons_signal == 2 && nElectrons_signal == 1 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20");

  RegionData SRttW = getRegionData("SRttW", "all", "SRttW","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=4 && nBjets_77 >= 1 && elec1Pt >= 20 && mc_channel_number == 410155");
  RegionData SRttZ = getRegionData("SRttZ", "all", "SRttZ","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=4 && nBjets_77 >= 1 && elec1Pt >= 20 && mc_channel_number == 410219");


  
  
  RegionData SR3 = getRegionData("SR3", "all", "SR3","nJet20 >=2");
  //RegionData SR0e = getRegionData("SR0e", "all", "SR0e","dimuon_m > 15 && dimuon_m < 75 && nElectrons_signal > 0 && nBjets_77 >= 1");
  //RegionData SR1e = getRegionData("SR1e", "all", "SR1e","dimuon_m < 80 && nJet20 >=4 && nElectrons_signal > 0"); 
 
  RegionData SR0e = getRegionData("SR0e", "all", "SR0e","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2");
  RegionData SR1e = getRegionData("SR1e", "all", "SR1e","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nBjets_77 >= 1");
  RegionData SR2e = getRegionData("SR2e", "all", "SR2e","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3");
  RegionData SR3e = getRegionData("SR3e", "all", "SR3e","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1");
  RegionData SR4e = getRegionData("SR4e", "all", "SR4e","dimuon_m >= 12 && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20");
  RegionData SR = getRegionData("SR", "all", "SR"," (mu2Pt/mu1Pt > .2 || Mmu1e_ss > 0 ) && dimuon_m >= 12 && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20");  
 
  RegionData VR_Mmu1e = getRegionData("VR_Mmu1e", "all", " VR_Mmu1e","Mmu1e_os >=15 && Mmu1e_os <=100 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=4 && nBjets_77 >= 1 && elec1Pt >= 20");
  RegionData VR_Mmu1e_50 = getRegionData("VR_Mmu1e_50", "all", "VR_Mmu1e_50","Mmu1e_os >= 30 && Mmu1e_os <=110 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=4 && nBjets_77 >= 1 && elec1Pt >= 20"); 

  RegionData VR_Mmu2e = getRegionData("VR_Mmu2e", "all", " VR_Mmu2e","Mmu2e_os >= 15 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=4 && nBjets_77 >= 1 && elec1Pt >= 20");
  
  RegionData VR_Mmu2e_50 = getRegionData("VR_Mmu2e_50", "all", " VR_Mmu2e","Mmu2e_os >= 50 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=4 && nBjets_77 >= 1 && elec1Pt >= 20");
 
  TString signal;
  float n_sig_SR2;
  float n_sig_SR1; 
  float n_sig_CR; 
  float n_sig_VR;    
  float n_sig_CRttZ;
  float n_sig_CRZ;
 
  vector<TString> signalvector = {"H100a15","H120a15","H120a30","H140a15","H140a30","H140a45","H160a15","H160a30","H160a45","H160a60","H160a75"};
  for (unsigned int i=0; i<signalvector.size(); ++i){
    signal = signalvector.at(i);
    //n_sig_SR1 = getYields(reader, SR2, signal);
    n_sig_SR2 = getYields(reader, SR, signal);
    n_sig_CR = getYields(reader, CRss,signal);
    n_sig_CRZ = getYields(reader, CRZ,signal);
    n_sig_CRttZ = getYields(reader, CRttZ,signal); 
    n_sig_VR = getYields(reader,VR,signal); 
    //cout << "Yields CRttZ: signalname: " <<signal<< " events:" << n_sig_CRttZ << endl; 
    //cout << "Yields CRZ: signalname: " <<signal<< " events:" << n_sig_CRZ << endl;
    //cout << "Yields CRss: signalname: " <<signal<< " events:" << n_sig_CR << endl; 
    cout << "Yields SR: signalname: " <<signal<< " events:" << n_sig_SR2 << endl; 
  }
  //CRe1
  //float n_zjets_CR = getYields(reader, CRe1, "ttV");
  float n_data = getYields(reader, CRss, "data");
  float n_data_CRZ = getYields(reader, CRZ, "data");
  float n_data_CRttZ = getYields(reader, CRttZ, "data");


  //SR
  //float n_zjets_SR = getYields(reader, SRttW, "ttV");
  float n_zjets_SR0 = getYields(reader, CRZ, "Zjets_B");
  float n_zjets_SR1 = getYields(reader, CRZ, "Zjets_C");
  float n_zjets_SR2 = getYields(reader, CRZ, "Zjets_other");
  float n_zjets_SR3 = getYields(reader, SR, "Zjets_B");
  float n_zjets_SR4 = getYields(reader, SR, "Zjets_C");
  float n_zjets_SR5 = getYields(reader, SR, "Zjets_other");

  float n_ttbar_SR = getYields(reader, SR4e, "ttbar");
 // float n_data_SR = getYields(reader,SR5e, "data");

  //VR
  float n_ttZ_VR = getYields(reader,VR, "ttZ");
  float n_ttbar_VR = getYields(reader, VR, "ttbar");
  float n_data_VR = getYields(reader,VR,"data");
    

  //cout << "Yields CRss ttV " << n_zjets_CR << endl;
  //cout << "Yields CRss: ttbar: " << n_ttbar_CR << endl;
  //cout << "Yields CRss: data " << n_data << endl;
  //cout << "Yields CRZ: data " << n_data_CRZ << endl;
  //cout << "Yields CRttZ: data " << n_data_CRttZ << endl;

  
  //cout << "Yields VR: ttV " <<  n_ttZ_VR << endl; 
  //cout << "Yields VR: ttbar: " << n_ttbar_VR << endl;
  //cout << "Yields VR: data " << n_data_VR << endl;
  
  //cout << "Yields SR: data " << n_data_SR << endl;
  //cout << "Yields SR: ttbar: " << n_ttbar_SR << endl;
  //cout << "Yields SR: ttbar no cut " << n_ttbar_VR << endl;

  //cout << "Yields CRZ B  " << n_zjets_SR0 << endl;
  //cout << "Yields CRZ C  " << n_zjets_SR1 << endl;
  //cout << "Yields CRZ other  " << n_zjets_SR2 << endl;
  //cout << "Yields SR B  " << n_zjets_SR3 << endl;
  //cout << "Yields SR C " << n_zjets_SR4 << endl;
  //cout << "Yields SR other  " << n_zjets_SR5 << endl;
 
  ///////////--------original stuff----------------////////////////////


  //float n_ddz_TCR = getYields(reader, TCR, "DDZ");
  //float n_data_TCR = getYields(reader, TCR, "data");
  //float n_ttbar_SB = getYields(reader, SB, "ttbar");
  //float n_ddz_SB = getYields(reader, SB, "DDZ");
  //float n_data_SB = getYields(reader, SB, "data");


  //float mu_DDZ = (n_data_TCR*n_ttbar_SB - n_data_SB*n_ttbar_TCR)/(n_ddz_TCR*n_ttbar_SB - n_ddz_SB*n_ttbar_TCR);
  //float mu_ttbar = (n_data_SB - n_ddz_SB * mu_DDZ)/n_ttbar_SB;

  //cout << "Yields TCR: data: " << n_data_TCR << ", ttbar: " << n_ttbar_TCR << ", DDZ: " << n_ddz_TCR << endl;
  //cout << "Yields SB: data: " << n_data_SB << ", ttbar: " << n_ttbar_SB << ", DDZ: " << n_ddz_SB << endl;
  //cout << "Yields SB: data: " << n_data_SB << ", ttbar: " << n_ttbar_SB << endl;
  //cout << "mu_ttbar: " << mu_ttbar << ", mu_DDZ: " << mu_DDZ << endl; 


}


TString getDimuonCut(int masspoint, int range = 1){ 
  TString dimuonCut = TString("dimuon_m > ") + Form("%d",masspoint - range) + "&& dimuon_m < " + Form("%d",masspoint + range);
  return dimuonCut;
}


void doInterpolation(OutputReader* reader, TGraph *&yields_ggF, TGraph *&yields_VBF, TString BDTcut = "1"){

  //TString cuts_ggF = BDTcut + "&& klbbmm_m>110 && klbbmm_m<140 && met<60 && klfit_loglikelihood>-8 && mc_channel_number <= 346075 &&";
  TString cuts_ggF = "isOS ==1 && nElectrons_signal == 1 && nMuons_signal ==2 && isOS ==1 && elec1Pt >= 20 && nJet20 >=3 && nBjets_77 >=1 &&";
  TString cuts_VBF = BDTcut + "&& klbbmm_m>110 && klbbmm_m<140 && met<60 && klfit_loglikelihood>-8 && mc_channel_number > 346075 &&";
  //vector<int> masspoints_ggF = {14,16,20,25,30,40,55,60,62};
  vector<int> masspoints_ggF = {15,30,45,60,75}; 
  vector<int> masspoints_VBF = {14,20,62};

  yields_ggF = new TGraph(masspoints_ggF.size());
  //yields_VBF = new TGraph(masspoints_VBF.size());

  int i = 0;
  for(auto masspoint : masspoints_ggF){
    i +=1;
    RegionData SR = getRegionData(Form("SR%d",masspoint), "all", Form("SR%d",masspoint), cuts_ggF + getDimuonCut(masspoint,4));
    TH1* h_signal = reader->getHistoFromTree(Form("H160a%d",masspoint),SR.region,"1",Form("signal%d",masspoint),1,0,2,"event_weight *(" + SR.cuts + ")");
    float yield = h_signal->GetBinContent(1);
    yields_ggF->SetPoint(i-1,masspoint,yield);
  }


  //int j = 0;
  //for(auto masspoint : masspoints_VBF){
  //  j +=1;
  //  RegionData SR = getRegionData(Form("SR%d",masspoint), "all", Form("SR%d",masspoint), cuts_VBF + getDimuonCut(masspoint,1));
  //  TH1* h_signal = reader->getHistoFromTree(Form("a%d",masspoint),SR.region,"1",Form("signal%d",masspoint),1,0,2,"event_weight *(" + SR.cuts + ")");
  //  float yield = h_signal->GetBinContent(1);
  //  yields_VBF->SetPoint(j-1,masspoint,yield);
  //}


}

void drawGraph(vector<TGraph*> tgraph_vec, TString prodmode){
 TCanvas *tgraph_interpol = new TCanvas(prodmode,prodmode,200,10,700,500);
 vector<int> colors = {kAzure, kGreen, kRed, kViolet, kOrange, kMagenta};
   
 for (unsigned int i = 0; i< tgraph_vec.size();++i){
    cout<<i<<endl;
    tgraph_vec[i]->SetLineColor(colors[i]);
    tgraph_vec[i]->SetMarkerColor(colors[i]);
    if(0==i) tgraph_vec[i]->Draw("ALP");
    else tgraph_vec[i]->Draw("LPsame");
  }

  gSystem->Exec("mkdir -p plots/Interpolation/");
  tgraph_interpol->SaveAs("plots/Interpolation/yields_" + prodmode + ".png");
}


void doInterpolation(OutputReader* reader){
  vector<TGraph*> tgraph_ggF(1);
  vector<TGraph*> tgraph_VBF(1);

  doInterpolation(reader, tgraph_ggF[0], tgraph_VBF[0]);
  // doInterpolation(reader, tgraph_ggF[1], tgraph_VBF[1], "");
  //    //  //doInterpolation(reader, tgraph_ggF[2], tgraph_VBF[2], "BDT_a30_dR_dimuon_m_26_34_config5>0");
  //      //    //doInterpolation(reader, tgraph_ggF[3], tgraph_VBF[3], "BDT_a40_dR_dimuon_m_36_44_config5>0");
  //        //      //doInterpolation(reader, tgraph_ggF[4], tgraph_VBF[4], "BDT_a50_dR_dimuon_m_46_54_config5>0");
  //          //        //doInterpolation(reader, tgraph_ggF[5], tgraph_VBF[5], "BDT_a60_dR_dimuon_m_56_64_config5>0");
  drawGraph(tgraph_ggF,"ggF");
  //drawGraph(tgraph_VBF,"VBF");
}

                       





void DataMCHistoDraw_run2(int k){
  

  //sample command: root -l -b AtlasStyle.C AtlasLabels.C 'DataMCHistoDraw_run2.C(1)'
  date = "jan" + to_string(k); // dates that have good tuples: sept 23, jan6
  base = "/afs/cern.ch/work/w/waahmed/hfbbmumuanalysis/output_plots/"+date; 
  
  SetAtlasStyle();
  cout << " start job " << base <<endl;

  OutputReader* reader = new OutputReader();
  readData(reader);
  readSignal(reader);
  //doInterpolation(reader);
  drawExample(reader);
  //dumpRegions(reader);  
  //drawSamples(reader);
  //readIso(reader);
  //makeIsoRegion(reader);

}


/////////////////////////not using this stuff now//////////////////////////////////////////////////////////////////////




void drawCompareRegion(OutputReader *reader, VariableData Variable, vector <RegionData> Regions, TString sample_name){
  
  TString var = Variable.branch;
  TString option = Variable.option;
  double blow=Variable.blow;
  double bup =Variable.bup;

  int maxrebin = 0;
  vector <TString> name_vector;
  vector <TString> eventweights;
  TString name = "compRegion_"+Variable.name+"_"+sample_name+"_"+Regions[0].name; 

  for(auto Region : Regions){
    if (Region.rebin>maxrebin) maxrebin = Region.rebin;
    eventweights.push_back(buildEventWeight("event_weight",Region.cuts));
  }
  int nbins = Variable.nbins/maxrebin;

  vector <TH1*> hist_vector;
  for (unsigned int i=0; i<Regions.size();i++){
    hist_vector.push_back(reader->getHistoFromTree(sample_name,Regions[i].region,var,name,nbins,blow,bup,eventweights[i]));
  }	

  vector <int> colors = {kAzure, kGreen, kRed, kViolet, kOrange, kMagenta};
  vector <int> markers = {22,21,23,24,1,1,1};
  TCanvas *c_normHistos = new TCanvas(name,name,200,10,700,500);
  TLegend *leg = getTLegend();
  TLatex *l2 = getTLatex(sample_name);

  float max = 0;
  for(unsigned int i=0; i<hist_vector.size();i++){

    hist_vector[i]->SetLineColor(colors[i]);
    hist_vector[i]->SetMarkerColor(colors[i]);
    hist_vector[i]->SetMarkerStyle(markers[i]);
    normHisto(hist_vector[i]);
    if((hist_vector[i]->GetMaximum())>max){  
      max = hist_vector[i]->GetMaximum();
    }
  }
  if("log"==option){
    c_normHistos->cd()->SetLogy();
    hist_vector[0]->SetMaximum(max*50);
  }
  else hist_vector[0]->SetMaximum(max*1.5);


  for(unsigned int i=0; i<hist_vector.size();i++){
    if(0==i){
      hist_vector[i]->GetXaxis()->SetTitle(Variable.title);
      hist_vector[i]->GetYaxis()->SetTitle("Normalized to 1");
      hist_vector[i]->Draw();
    }
    else{
      if (i==1) hist_vector[i]->Draw("same");
      else hist_vector[i]->Draw("histsame");
    }
    leg->AddEntry(hist_vector[i],Regions[i].label,"LP");
  }

  leg->Draw();
  l2->Draw();

  gSystem->Exec("mkdir -p plots/compRegion/"+sample_name);
  c_normHistos->SaveAs("plots/compRegion/"+sample_name+"/"+name+".png");

}

void drawDataSignAll(OutputReader* reader, VariableData Variable, RegionData Region, float sys_bgr1 = 0, float sys_bgr2 = 0){
  
  TString region = Region.region;
  TString var = Variable.branch;
  TString bgr_sys = Form("%.2f_%.2f",sys_bgr1,sys_bgr2);
  TString name="sign_"+Variable.name+"_"+Region.name+"_all_signals_"+bgr_sys;

  TString eventweight =buildEventWeight("event_weight",Region.cuts);
  TString eventweightDDZ =buildEventWeight("event_weight*jet0_weight",Region.cuts);

  int nbins=Variable.nbins;
  double blow=Variable.blow;
  double bup =Variable.bup;

  TH1* h_ttbar = reader->getHistoFromTree("ttbar",region,var,name,nbins,blow,bup,eventweight);
  TH1* h_dbz = reader->getHistoFromTree("DDZ",region,var,name,nbins,blow,bup,eventweightDDZ);

  vector <TH1*> hist_vector;
  vector <TGraph*> gr_vector;
  vector <TString> signame_vector = {"a20", "a30", "a40", "a50","a60"};

  for (auto signame:signame_vector){
    hist_vector.push_back(reader->getHistoFromTree(signame,region,var,name,nbins,blow,bup,eventweight));
    gr_vector.push_back(calculateSign(hist_vector.back(), h_dbz, h_ttbar, sys_bgr1, sys_bgr2));
  }


  vector <int> colors = {kGreen+1, kBlue+1, kRed+1, kViolet+1, kOrange+1, kMagenta+1};
  TCanvas *tgraph_sign = new TCanvas(name,name,200,10,700,500);
  TLegend *leg = getTLegend();
  TLatex *l2 = getTLatex(Region.label);

  for (unsigned int i=0; i<gr_vector.size(); i++){

    gr_vector[i]->SetLineColor(colors[i]);
    gr_vector[i]->SetMarkerColor(colors[i]);

    if (0==i){
      gr_vector[i]->GetXaxis()->SetTitle(Variable.title);
      gr_vector[i]->GetYaxis()->SetTitle("Significance");
      gr_vector[i]->Draw();
    }

    else gr_vector[i]->Draw("LPsame");

    leg->AddEntry(gr_vector[i],signame_vector[i],"LP");
  }

  leg->Draw();
  l2->Draw();

  gSystem->Exec("mkdir -p plots/"+Region.name+"/sign/");	
  tgraph_sign->SaveAs("plots/"+Region.name+"/sign/"+name+".png");

}

RegionData getRegionFromMass(int down, int up, int defaultmass=0){

  ostringstream ss;
  if(defaultmass) ss<<"SR"<<defaultmass;
  else ss<<"SR"<<down<<"_"<<up;
  ostringstream ss1;
  ss1<<"dimuon_m>"<<down<<" && dimuon_m<"<<up << "&& klbbmm_m>110 && klbbmm_m<140 && met<60 && klfit_loglikelihood>-8";

  return getRegionData(ss.str().c_str(), "all", ss.str().c_str(),ss1.str().c_str(),2);

}

vector<RegionData> getRegionToCompare(int defaultmass){

  vector<RegionData> regions;
  regions.push_back(getRegionFromMass(defaultmass-1, defaultmass+1, defaultmass));
  regions.push_back(getRegionFromMass(defaultmass-4, defaultmass-1));
  regions.push_back(getRegionFromMass(defaultmass+1, defaultmass+4));
  regions.push_back(getRegionFromMass(defaultmass-4, defaultmass+4));

  return regions;
}

void drawSamples(OutputReader* reader){

  vector<RegionData> regions20 = getRegionToCompare(20);
  vector<RegionData> regions30 = getRegionToCompare(30);
  vector<RegionData> regions40 = getRegionToCompare(40);
  vector<RegionData> regions50 = getRegionToCompare(50);
  vector<RegionData> regions60 = getRegionToCompare(60);

  vector<VariableData> vars;
  vars.push_back(VariableData("dRm1b1","dRm1b1","#DeltaRm_{1}b_{1}",60,0,5));
  vars.push_back(VariableData("dRm2b1","dRm2b1","#DeltaRm_{2}b_{1}",60,0,5));
  vars.push_back(VariableData("dRm1b2","dRm1b2","#DeltaRm_{1}b_{2}",60,0,5));
  vars.push_back(VariableData("dRm2b2","dRm2b2","#DeltaRm_{2}b_{2}",60,0,5));
  vars.push_back(VariableData("dRm1m2","dRm1m2","#DeltaRm_{1}m_{2}",60,0,5));
  vars.push_back(VariableData("dRb1b2","dRb1b2","#DeltaRb_{1}b_{2}",60,0,5));
  vars.push_back(VariableData("dRMmmMbb","dRMmmMbb","#DeltaRM_{mm}M_{bb}",60,0,5));
  vars.push_back(VariableData("dRMmmMbbkl","dRMmmMbbkl","kl #DeltaRM_{mm}M_{bb}",60,0,5));

  vector <TString> BDT_trees = TreeNameVector("BDTTreeNames.txt");
  vector<vector<RegionData>> regionsMatrix = {regions20, regions30, regions40, regions50, regions60};
  //vector <TString> signalRegions = {"a20", "a30", "a40", "a50", "a60"};
  /*
    for(auto tree_name : BDT_trees){
    for(auto var : vars){
    if (tree_name.Index("data") != -1 || tree_name.Index("ttbar") != -1 || tree_name.Index("DDZ") != -1){
    for(auto region : regionsMatrix){
    cout << "---------tree_name--------- " << tree_name << endl;
    drawCompareRegion(reader, var, region, tree_name);
    }
    }

    if (tree_name.Index("a20") != -1){
    cout << "---------tree_name--------- " << tree_name << endl;
    drawCompareRegion(reader, var, regions20, tree_name);
    }
    if (tree_name.Index("a30") != -1){
    cout << "---------tree_name--------- " << tree_name << endl;
    drawCompareRegion(reader, var, regions30, tree_name);
    }
    if (tree_name.Index("a40") != -1){
    cout << "---------tree_name--------- " << tree_name << endl;
    drawCompareRegion(reader, var, regions40, tree_name);
    }
    if (tree_name.Index("a50") != -1){
    cout << "---------tree_name--------- " << tree_name << endl;
    drawCompareRegion(reader, var, regions50, tree_name);
    }
    if (tree_name.Index("a60") != -1){
    cout << "---------tree_name--------- " << tree_name << endl;
    drawCompareRegion(reader, var, regions60, tree_name);
    }
  */
  /*
    drawCompareRegion(reader, var, regions20, "a20");
    drawCompareRegion(reader, var, regions20, "ttbar");
    drawCompareRegion(reader, var, regions20, "DDZ");

    drawCompareRegion(reader, var, regions30, "a30");
    drawCompareRegion(reader, var, regions30, "ttbar");
    drawCompareRegion(reader, var, regions30, "DDZ");

    drawCompareRegion(reader, var, regions40, "a40");
    drawCompareRegion(reader, var, regions40, "ttbar");
    drawCompareRegion(reader, var, regions40, "DDZ");

    drawCompareRegion(reader, var, regions50, "a50");
    drawCompareRegion(reader, var, regions50, "ttbar");
    drawCompareRegion(reader, var, regions50, "DDZ");

    drawCompareRegion(reader, var, regions60, "a60");
    drawCompareRegion(reader, var, regions60, "ttbar");
    drawCompareRegion(reader, var, regions60, "DDZ");
    }
    }*/

}

/*

TString getDimuonCut(int masspoint, int range = 1){ 
  TString dimuonCut = TString("dimuon_m > ") + Form("%d",masspoint - range) + "&& dimuon_m < " + Form("%d",masspoint + range);
  return dimuonCut;
}

void doInterpolation(OutputReader* reader, TGraph *&yields_ggF, TGraph *&yields_VBF, TString BDTcut = "1"){

  TString cuts_ggF = BDTcut + "&& klbbmm_m>110 && klbbmm_m<140 && met<60 && klfit_loglikelihood>-8 && mc_channel_number <= 346075 &&";
  TString cuts_VBF = BDTcut + "&& klbbmm_m>110 && klbbmm_m<140 && met<60 && klfit_loglikelihood>-8 && mc_channel_number > 346075 &&";
  vector<int> masspoints_ggF = {14,16,20,25,30,40,55,60,62}; 
  vector<int> masspoints_VBF = {14,20,62};

  yields_ggF = new TGraph(masspoints_ggF.size());
  yields_VBF = new TGraph(masspoints_VBF.size());

  int i = 0;
  for(auto masspoint : masspoints_ggF){
    i +=1;
    RegionData SR = getRegionData(Form("SR%d",masspoint), "all", Form("SR%d",masspoint), cuts_ggF + getDimuonCut(masspoint,1));
    TH1* h_signal = reader->getHistoFromTree(Form("a%d",masspoint),SR.region,"1",Form("signal%d",masspoint),1,0,2,"event_weight *(" + SR.cuts + ")");
    float yield = h_signal->GetBinContent(1);
    yields_ggF->SetPoint(i-1,masspoint,yield);
  }


  int j = 0;
  for(auto masspoint : masspoints_VBF){
    j +=1;
    RegionData SR = getRegionData(Form("SR%d",masspoint), "all", Form("SR%d",masspoint), cuts_VBF + getDimuonCut(masspoint,1));
    TH1* h_signal = reader->getHistoFromTree(Form("a%d",masspoint),SR.region,"1",Form("signal%d",masspoint),1,0,2,"event_weight *(" + SR.cuts + ")");
    float yield = h_signal->GetBinContent(1);
    yields_VBF->SetPoint(j-1,masspoint,yield);
  }


}

void drawGraph(vector<TGraph*> tgraph_vec, TString prodmode){
  TCanvas *tgraph_interpol = new TCanvas(prodmode,prodmode,200,10,700,500);
  vector<int> colors = {kAzure, kGreen, kRed, kViolet, kOrange, kMagenta};
  for (unsigned int i = 0; i< tgraph_vec.size();++i){
    tgraph_vec[i]->SetLineColor(colors[i]);
    tgraph_vec[i]->SetMarkerColor(colors[i]);
    if(0==i) tgraph_vec[i]->Draw("ALP");
    else tgraph_vec[i]->Draw("LPsame");
  }

  gSystem->Exec("mkdir -p plots/Interpolation/");
  tgraph_interpol->SaveAs("plots/Interpolation/yields_" + prodmode + ".png");
}



void doInterpolation(OutputReader* reader){

  vector<TGraph*> tgraph_ggF(6);
  vector<TGraph*> tgraph_VBF(6);

  doInterpolation(reader, tgraph_ggF[0], tgraph_VBF[0]);
  //doInterpolation(reader, tgraph_ggF[1], tgraph_VBF[1], "BDT_a20_dR_dimuon_m_16_24_config5>0");
  //doInterpolation(reader, tgraph_ggF[2], tgraph_VBF[2], "BDT_a30_dR_dimuon_m_26_34_config5>0");
  //doInterpolation(reader, tgraph_ggF[3], tgraph_VBF[3], "BDT_a40_dR_dimuon_m_36_44_config5>0");
  //doInterpolation(reader, tgraph_ggF[4], tgraph_VBF[4], "BDT_a50_dR_dimuon_m_46_54_config5>0");
  //doInterpolation(reader, tgraph_ggF[5], tgraph_VBF[5], "BDT_a60_dR_dimuon_m_56_64_config5>0");

  drawGraph(tgraph_ggF,"ggF");
  drawGraph(tgraph_VBF,"VBF");
}

*/


void printRatios(OutputReader* reader, RegionData Region, TString sample_name){
  /*
    Definition of isolation cuts
    if( mu->IsoFCLoose ) x[0] = 1;
    if( mu->IsoFCTight ) x[2] = 1;
    if( mu->IsoFixedCutHighPtTrackOnly ) x[3] = 1;
    if( mu->IsoFCTightTrackOnly ) x[4] = 1;
    if( mu->IsoFCLoose_FixedRad ) x[5] = 1;
    if( mu->IsoFCTight_FixedRad ) x[6] = 1;
    if( mu->FCTightTrackOnly_FixedRad ) x[7] = 1;
    if( mu->FixedCutPflowTight ) x[8] = 1;
    if( mu->FixedCutPflowLoose ) x[1] = 1;
  */
  vector<TString> isoNames = {"FCLoose", "FCTight", "FCTighTrackOnly", "FCLoose_FixedRad"};
  vector<TString> isoCuts = {"muIso1&(1<<0)&&muIso2&(1<<0)", "muIso1&(1<<2)&&muIso2&(1<<2)","muIso1&(1<<4)&&muIso2&(1<<4)","muIso1&(1<<5)&&muIso2&(1<<5)"};
  TString eventweight = buildEventWeight("event_weight",Region.cuts);
  TH1* h_all = reader->getHistoFromTree(sample_name,"all","1",sample_name,1,0,2,eventweight);
  
  for(unsigned int i = 0; i<isoNames.size(); ++i){
    TString isocut = buildEventWeight(eventweight, isoCuts[i]);
    TH1* h_iso = reader->getHistoFromTree(sample_name,"all","1",sample_name,1,0,2,isocut);
    h_iso->Divide(h_iso,h_all,1,1,"B");
    cout <<  isoNames[i] << " efficiency(Sample = " << sample_name << ", region = " << Region.label << "): " << setprecision(3) << h_iso->GetBinContent(1) << "+-" << h_iso->GetBinError(1) << endl;
  }
}

void readIso(OutputReader* reader){
  int lumi = 1;
  TString data_dir = getenv("HBBMUMU_DATA_PATH");
  TString directory = data_dir + "/rel21/skimSlimNtuples/FwdJets2Mu/IsoStudy/";

  reader->readTree("data","all",directory +"AllIsoFiles.root","data");
  reader->readTree("ttbar","all",directory+"AllIsoFiles.root","ttbarnonallhad__Nominal");
  reader->readTree("ttbardilep","all",directory+"AllIsoFiles.root","ttbardilep__Nominal");
  reader->readTree("Wjets","all",directory+"AllIsoFiles.root","Wjets__Nominal",lumi);
  reader->readTree("Zjets","all",directory+"AllIsoFiles.root","Zjets__Nominal",lumi);
  reader->readTree("db","all",directory+"AllIsoFiles.root","diBoson__Nominal",lumi);
  reader->readTree("st","all",directory+"AllIsoFiles.root","singleTop__Nominal",lumi);
  reader->readTree("ttV","all",directory+"AllIsoFiles.root","ttV__Nominal",lumi);
  reader->readTree("a14","all",directory+"AllIsoFiles.root","signal14__Nominal",lumi);
  reader->readTree("a18","all",directory+"AllIsoFiles.root","signal18__Nominal",lumi);
  reader->readTree("a20","all",directory+"AllIsoFiles.root","signal20__Nominal",lumi);
  reader->readTree("a30","all",directory+"AllIsoFiles.root","signal30__Nominal",lumi);
  //reader->readTree("a40","all",directory+"AllIsoFiles.root","signal40__Nominal",lumi);
  reader->readTree("a50","all",directory+"AllIsoFiles.root","signal50__Nominal",lumi);
  reader->readTree("a60","all",directory+"AllIsoFiles.root","signal60__Nominal",lumi);
  reader->readTree("a62","all",directory+"AllIsoFiles.root","signal62__Nominal",lumi);
}


void drawIsoRatioTtbar(OutputReader* reader, VariableData Variable, RegionData Region, TString RegionName=""){

  TString region = Region.region;
  TString var = Variable.branch;
  int rebin = Region.rebin;
  TString name="isottbar_"+Variable.name+"_"+Region.name;

  TString eventweight =buildEventWeight("event_weight",Region.cuts);

  TCanvas *c_normHistos = new TCanvas(name,name,200,10,700,500);

  c_normHistos->cd();
  TPad* pad0 = new TPad("pad0","pad0",0,0.2,1,1,0,0,0);
  pad0->SetTickx(false);
  pad0->SetTicky(false);
  pad0->SetTopMargin(0.05);
  pad0->SetBottomMargin(0.102);
  pad0->SetLeftMargin(0.14);
  pad0->SetRightMargin(0.05);
  pad0->SetFrameBorderMode(0);
  pad0->SetFillStyle(0);
  
  TPad* pad1 = new TPad("pad1","pad1",0,0.,1,0.28,0,0,0);
  pad1->SetTickx(false);
  pad1->SetTicky(false);
  pad1->SetTopMargin(0.0);
  pad1->SetBottomMargin(0.4);
  pad1->SetLeftMargin(0.14);
  pad1->SetRightMargin(0.05);
  pad1->SetFrameBorderMode(0);
  pad1->SetFillStyle(0);
  pad0->Draw();
  pad1->Draw();
  TLatex *l2 = getTLatex(Region.label);
  TLegend *leg = getTLegend(0.7, 0.7, 0.93, 0.92);

  int nbins=Variable.nbins/rebin;
  if(nbins<1) nbins = 1;
  double blow=Variable.blow;
  double bup =Variable.bup;

  TH1* h_all = reader->getHistoFromTree("ttbar",region,var,name,nbins,blow,bup,eventweight);
  TH1* h_dilep = reader->getHistoFromTree("ttbardilep",region,var,name,nbins,blow,bup,eventweight);
  TH1* h_ratio = (TH1*)h_all->Clone();
  h_all->SetDirectory(0);
  h_dilep->SetDirectory(0);
  h_ratio->SetDirectory(0);
  h_ratio->Divide(h_dilep, h_all, 1, 1);
  h_ratio->SetXTitle(Variable.title);
  h_all->SetXTitle(Variable.title);
  h_ratio->SetYTitle("Dilep/nonAllHad");
  h_all->SetYTitle("Events");
  h_all->SetLineColor(kRed);
  h_all->SetMarkerColor(kRed);
  h_dilep->SetLineColor(kBlue);
  h_dilep->SetMarkerColor(kBlue);
  leg->AddEntry(h_all, "ttbarnonallhad","LP");
  leg->AddEntry(h_dilep, "ttbardilep","LP");
  pad0->cd();
  h_all->Draw();
  h_all->SetMinimum(0.00001);
  h_all->SetMaximum(h_all->GetMaximum()*1.4);
  if(h_all->GetMaximum() == 0) h_all->SetMaximum(1);
  leg->Draw();
  h_dilep->Draw("same");
  l2->Draw();
  pad1->cd();
  h_ratio->Draw();  
  
  TLine *hline = new TLine(h_ratio->GetXaxis()->GetXmin(),1,h_ratio->GetXaxis()->GetXmax(),1);  
  hline->SetLineColor(kRed);
  hline->SetLineWidth(2);
  hline->SetLineStyle(2);
  hline->Draw();
  h_ratio->GetYaxis()->SetRangeUser(0.5,1.5);
  h_ratio->GetYaxis()->SetTitleOffset(0.4);
  h_ratio->GetYaxis()->SetTitleSize(0.12);
  //h_ratio->GetYaxis()->SetLabelSize(20);
  //h_ratio->GetXaxis()->SetTitleOffset(5);
  //h_ratio->GetXaxis()->SetLabelSize(20);
  h_ratio->GetXaxis()->SetTitleSize(0.12);
  h_ratio->GetYaxis()->SetTitleSize(0.12);
  h_ratio->GetYaxis()->SetLabelSize(0.09);
  h_ratio->GetYaxis()->SetNdivisions(509);
  h_ratio->GetXaxis()->SetLabelSize(0.09);
  h_ratio->GetXaxis()->SetLabelOffset(0.02);

  gSystem->Exec("mkdir -p plots/IsoStudy/ttbarComp/" + RegionName);
  c_normHistos->SaveAs("plots/IsoStudy/ttbarComp/"+ RegionName + "/" + name+".png");

}


void drawIsoRatio(OutputReader* reader, VariableData Variable, RegionData Region, TString sample_name, TString isTrueMu, TString RegionName=""){

  TString region = Region.region;
  int rebin = Region.rebin;
  TString var = Variable.branch;
  TString name="iso_"+Variable.name+"_"+Region.name+"_"+sample_name;

  TString eventweight =buildEventWeight("event_weight",Region.cuts);
  TString eventweightFake =buildEventWeight(eventweight, isTrueMu);

  TCanvas *c_normHistos = new TCanvas(name,name,200,10,700,500);
  TLatex *l2 = getTLatex(Region.label + ", " + sample_name);

  int nbins=Variable.nbins/rebin;
  if(nbins<1) nbins = 1;
  double blow=Variable.blow;
  double bup =Variable.bup;

  TH1* h_all = reader->getHistoFromTree(sample_name,region,var,name,nbins,blow,bup,eventweight);
  TH1* h_fake = reader->getHistoFromTree(sample_name,region,var,name,nbins,blow,bup,eventweightFake);
  TH1* h_ratio = (TH1*)h_all->Clone();
  h_all->SetDirectory(0);
  h_fake->SetDirectory(0);
  h_ratio->SetDirectory(0);
  h_ratio->Divide(h_fake, h_all, 1, 1, "B"); //third argument scaling of histo1, fourth scaling of histo2, "B" binominal error because histos correlated
  h_ratio->SetXTitle(Variable.title);
  h_ratio->SetYTitle("fake fraction");
  h_ratio->Draw();
  //h_all->Draw();
  l2->Draw();

  gSystem->Exec("mkdir -p plots/IsoStudy/fakeFraction/" + RegionName);
  c_normHistos->SaveAs("plots/IsoStudy/fakeFraction/"+ RegionName + "/" +name+".png");

}

void drawIsoFakes(OutputReader* reader, RegionData Region){

  vector<VariableData> vars;
  vars.push_back(VariableData("dimuon_m","dimuon_m","Dimuon mass [GeV]", 24,16,64));
  vars.push_back(VariableData("mu1Pt","mu1Pt","#mu_{1}p_{T} [GeV]",40,27,187));
  vars.push_back(VariableData("mu2Pt","mu2Pt","#mu_{2}p_{T} [GeV]",20,7,87)); 
  vars.push_back(VariableData("yields","1","yields",1,0,2)); 
 
  vector<TString> FCLooseCuts = {"muIso1&(1<<0)&&muIso2&(1<<0)","muIso1&(1<<0)","muIso2&(1<<0)", "muIso1&(1<<0)&&muIso2&(1<<0)"};
  vector<TString> FCTightCuts = {"muIso1&(1<<2)&&muIso2&(1<<2)", "muIso1&(1<<2)", "muIso2&(1<<2)", "muIso1&(1<<2)&&muIso2&(1<<2)"};

  vector<TString> isFakeMu = {"isTrueMu1==0||isTrueMu2==0", "isTrueMu1==0", "isTrueMu2==0", "isTrueMu1==0||isTrueMu2==0"};

  for(unsigned int i = 0; i<isFakeMu.size(); ++i){
    RegionData NoIso = Region;
    RegionData FCLoose = Region;
    RegionData FCTight = Region;
    FCLoose.cuts = buildEventWeight(Region.cuts, FCLooseCuts[i]);
    FCTight.cuts = buildEventWeight(Region.cuts, FCTightCuts[i]);
    FCLoose.label += ", FCLoose";
    FCLoose.name += "_FCLoose";
    FCTight.name += "_FCTight";
    NoIso.name += "_noIsoCuts";
    FCTight.label += ", FCTight";
    NoIso.label += ", noIsoCut";
    drawIsoRatio(reader, vars[i], NoIso, "ttbar", isFakeMu[i], Region.name);
    drawIsoRatio(reader, vars[i], FCLoose, "ttbar", isFakeMu[i], Region.name);
    drawIsoRatio(reader, vars[i], FCTight, "ttbar", isFakeMu[i], Region.name);
    drawIsoRatioTtbar(reader, vars[i], NoIso, Region.name);
    drawIsoRatioTtbar(reader, vars[i], FCLoose, Region.name);
    drawIsoRatioTtbar(reader, vars[i], FCTight, Region.name);
    drawIsoRatio(reader, vars[i], NoIso, "Wjets", isFakeMu[i], Region.name);
    drawIsoRatio(reader, vars[i], FCLoose, "Wjets", isFakeMu[i], Region.name);
    drawIsoRatio(reader, vars[i], FCTight, "Wjets", isFakeMu[i], Region.name);
    drawDataStack(reader, vars[i], NoIso, "None", 0, Region.name);
    drawDataStack(reader, vars[i], FCLoose, "None", 0, Region.name);
    drawDataStack(reader, vars[i], FCTight, "None", 0, Region.name);
  }

}


void makeIsoRegion(OutputReader* reader){

  RegionData SR = getRegionData("SR", "all", "SR","klbbmm_m>110 && klbbmm_m<140 && met<60 && klfit_loglikelihood>-8 && isOS",2);
  RegionData SR20 = getRegionData("SR20", "all", "SR20","klbbmm_m>110 && klbbmm_m<140 && met<60 && dimuon_m>19 && dimuon_m<21 && klfit_loglikelihood>-8 && isOS",4);
  RegionData SR50 = getRegionData("SR50", "all", "SR50","klbbmm_m>110 && klbbmm_m<140 && met<60 && dimuon_m>48 && dimuon_m<52 && klfit_loglikelihood>-8 && isOS",4);
  RegionData SR60 = getRegionData("SR60", "all", "SR60","klbbmm_m>110 && klbbmm_m<140 && met<60 && dimuon_m>58 && dimuon_m<62 && klfit_loglikelihood>-8 && isOS",4);
  RegionData LOOSE = getRegionData("LOOSE", "all", "LOOSE"," klfit_loglikelihood>-8 && isOS",1);
  RegionData TCR = getRegionData("TCR", "all", "TCR","klbbmm_m>110 && klbbmm_m<140 && met>60 && klfit_loglikelihood>-8 && isOS",2);
  RegionData SB1 = getRegionData("SB1", "all", "SB1","klbbmm_m > 80 && klbbmm_m<110  && met<60 && klfit_loglikelihood>-8 && isOS",2);
  RegionData SB2 = getRegionData("SB2", "all", "SB2","klbbmm_m>140 && klbbmm_m<170 && met<60 && klfit_loglikelihood>-8 && isOS",2);

  RegionData SRSS = getRegionData("SRSS", "all", "SRSS","klbbmm_m>110 && klbbmm_m<140 && met<60 && klfit_loglikelihood>-8 && isOS==0",2);
  RegionData SR20SS = getRegionData("SR20SS", "all", "SR20SS","klbbmm_m>110 && klbbmm_m<140 && met<60 && dimuon_m>19 && dimuon_m<21 && klfit_loglikelihood>-8 && isOS==0",4);
  RegionData SR50SS = getRegionData("SR50SS", "all", "SR50SS","klbbmm_m>110 && klbbmm_m<140 && met<60 && dimuon_m>48 && dimuon_m<52 && klfit_loglikelihood>-8 && isOS==0",4);
  RegionData SR60SS = getRegionData("SR60SS", "all", "SR60SS","klbbmm_m>110 && klbbmm_m<140 && met<60 && dimuon_m>58 && dimuon_m<62 && klfit_loglikelihood>-8 && isOS==0",4);
  RegionData LOOSESS = getRegionData("LOOSESS", "all", "LOOSESS"," klfit_loglikelihood>-8 && isOS==0",2);
  RegionData TCRSS = getRegionData("TCRSS", "all", "TCRSS","klbbmm_m>110 && klbbmm_m<140 && met>60 && klfit_loglikelihood>-8 && isOS==0",2);
  RegionData SB1SS = getRegionData("SB1SS", "all", "SB1SS","klbbmm_m > 80 && klbbmm_m<110  && met<60 && klfit_loglikelihood>-8 && isOS==0",2);
  RegionData SB2SS = getRegionData("SB2SS", "all", "SB2SS","klbbmm_m>140 && klbbmm_m<170 && met<60 && klfit_loglikelihood>-8 && isOS==0",2);

  vector<RegionData> OSRegions = {SR, SR20, SR50, SR60, LOOSE, TCR, SB1, SB2};
  vector<RegionData> SSRegions = {SRSS, SR20SS, SR50SS, SR60SS, LOOSESS, TCRSS, SB1SS, SB2SS};

  for(auto osregions : OSRegions){
    //drawIsoFakes(reader, osregions);
  }

  for(auto ssregions : SSRegions){
    //drawIsoFakes(reader, ssregions);
  }

  printRatios(reader, LOOSE, "ttbar");
  printRatios(reader, LOOSE, "ttbardilep");
  printRatios(reader, LOOSE, "a50");
  printRatios(reader, LOOSE, "Zjets");
  printRatios(reader, LOOSE, "Wjets");

  //drawIsoFakes(reader, SR);
  //drawIsoFakes(reader, TCR);
  //drawIsoFakes(reader, LOOSE);
  //drawIsoFakes(reader, SB1);
  //drawIsoFakes(reader, SB2);

}

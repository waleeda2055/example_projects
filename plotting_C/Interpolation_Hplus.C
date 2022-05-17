#include "OutputReader.h"

#include <TSystem.h>
#include <TLegend.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <THStack.h>
#include <TLatex.h>
#include <TSpline.h>
#include <TF1.h>
#include <TFitResultPtr.h>
#include <TFitResult.h>
#include "TMVA/Tools.h"
#include <TMVA/TSpline2.h>
#include <TMVA/TSpline1.h>

#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

void normHisto(TH1* h, bool addflow=true){

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


TLegend *getTLegend(float xlow=0.8, float ylow=0.6, float xup=0.93, float yup=0.92){

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

void readData(OutputReader* reader){

//float lumi = 139/36.; 
float lumi = 1.;
TString data_dir = getenv("HBBMUMU_DATA_PATH");
TString directory = data_dir + "/rel20.7/BDToutput/";

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
  reader->readTree("data","all","/home/admilic/HFbbmumu/rootFiles/rel21/skimSlimNtuples/NewDerivation/Run2_NewDer_1311.root","data");
  //reader->readTree("DDZ","all",directory+"AllNewBDTFiles.root","DDZ__Nominal",lumi*0.0113);
  reader->readTree("DDZ","all","/home/admilic/HFbbmumu/rootFiles/rel21/skimSlimNtuples/NewDerivation/Run2_NewDer_1311.root","DDZ__Nominal",lumi*330);//326 with old ntuples
  //reader->readTree("ttbar","all",directory+"AllNewBDTFiles.root","ttbar__Nominal",lumi*1.264);
  reader->readTree("ttbar","all","/home/admilic/HFbbmumu/rootFiles/rel21/skimSlimNtuples/NewDerivation/Run2_NewDer_1311.root","ttbar__Nominal",lumi*1.07);//1.07 with old ntuples
  reader->readTree("Wjets","all","/home/admilic/HFbbmumu/rootFiles/rel21/skimSlimNtuples/NewDerivation/Run2_NewDer_1311.root","Wjets__Nominal",lumi);
  reader->readTree("db","all","/home/admilic/HFbbmumu/rootFiles/rel21/skimSlimNtuples/NewDerivation/Run2_NewDer_1311.root","diBoson__Nominal",lumi);
  reader->readTree("st","all","/home/admilic/HFbbmumu/rootFiles/rel21/skimSlimNtuples/NewDerivation/Run2_NewDer_1311.root","singleTop__Nominal",lumi);
  reader->readTree("ttV","all","/home/admilic/HFbbmumu/rootFiles/rel21/skimSlimNtuples/NewDerivation/Run2_NewDer_1311.root","ttV__Nominal",lumi);
  reader->readTree("Zjets0","all","/home/admilic/HFbbmumu/rootFiles/rel21/skimSlimNtuples/NewDerivation/Run2_NewDer_ZjetsNom_1512.root","Zjets__0tag",lumi);
  reader->readTree("Zjets2","all","/home/admilic/HFbbmumu/rootFiles/rel21/skimSlimNtuples/NewDerivation/Run2_NewDer_ZjetsNom_1512.root","Zjets__Nominal",lumi);
  /*reader->readTree("a20","all",directory+"AllNewBDTFiles.root","a20__Nominal",lumi*0.2);
  reader->readTree("a30","all",directory+"AllNewBDTFiles.root","a30__Nominal",lumi*0.12);
  reader->readTree("a40","all",directory+"AllNewBDTFiles.root","a40__Nominal",lumi*0.32);
  reader->readTree("a50","all",directory+"AllNewBDTFiles.root","a50__Nominal",lumi*0.53);
  reader->readTree("a60","all",directory+"AllNewBDTFiles.root","a60__Nominal",lumi*0.21);*/



}

TString getDimuonCut(int masspoint, double range = 1){ 
 
TString dimuonCut = TString("dimuon_m >= ") + Form("%f",masspoint - range) + "&& dimuon_m <= " + Form("%f",masspoint + range);
return dimuonCut;
}

float getYields_ggF(OutputReader* reader, int massPoint , int masswindow, TString BDTcut = "1"){// get yield of given mass point obtained from tree
TString cuts_ggF = BDTcut + "&& klbbmm_m>110 && klbbmm_m<140 && met<60 && klfit_loglikelihood>-8 && mc_channel_number <= 346075 &&";
cout << "My cuts are: " << cuts_ggF << endl;

vector<int> masspoints_ggF = {14,16,18,20,25,30,40,50,55,60,62}; 
vector<int>::iterator it = find(masspoints_ggF.begin(), masspoints_ggF.end(),massPoint);

if(it != masspoints_ggF.end()){
  RegionData SR = getRegionData(Form("SR%d", massPoint), "all", Form("SR%d", massPoint), cuts_ggF + getDimuonCut(massPoint,masswindow));
  TH1* h_signal = reader->getHistoFromTree(Form("a%d",massPoint),SR.region,"1",Form("signal%d",massPoint),1,0,2,"event_weight*(" + SR.cuts + ")");
  //cout << "Yield ggF for mass point " <<massPoint << " is " << h_signal->GetBinContent(1) <<endl;
  return h_signal->GetBinContent(1);
}
else cout << "This mass point is not available for ggF!" << endl;
return 0;
}



float getYields_VBF(OutputReader* reader, int massPoint, int masswindow, TString BDTcut = "1"){ // get yield of given mass point obtained from tree
TString cuts_VBF = BDTcut + "&& klbbmm_m>110 && klbbmm_m<140 && met<60 && klfit_loglikelihood>-8 && mc_channel_number > 346075 &&";

vector<int> masspoints_VBF = {14,18,20,30,62};
vector<int>::iterator it = find(masspoints_VBF.begin(), masspoints_VBF.end(),massPoint);

if(it != masspoints_VBF.end()){
  RegionData SR = getRegionData(Form("SR%d",massPoint), "all", Form("SR%d",massPoint), cuts_VBF + getDimuonCut(massPoint,masswindow));
  TH1* h_signal = reader->getHistoFromTree(Form("a%d",massPoint),SR.region,"1",Form("signal%d",massPoint),1,0,2,"event_weight*(" + SR.cuts + ")");
  //cout << "Yield VBF for mass point " <<massPoint << " is " << h_signal->GetBinContent(1) <<endl;
  return h_signal->GetBinContent(1);
}
else cout << "This mass point is not available for VBF!" << endl;
return 0;
}


float computeYield(float massPoint, int massPointDown, int massPointUp, float yieldDown, float yieldUp){ //get yield of given mass point given two yields and assuming linear interpolation
  float k = (yieldDown - yieldUp)/(massPointDown-massPointUp);
  float d = yieldDown - massPointDown * (yieldDown - yieldUp) / (massPointDown - massPointUp);
  float yield = d + k * massPoint;
  if (yield < 0) cout << "WARNING: Yield smaller than 0! (Extrapolated) Yield for masspoint " << massPoint << " is: " << yield << endl;
  return yield;
}


float getAcceptance_ggF(OutputReader* reader, float massPOI, int massPointDown, int massPointUp, int masswindow = 1, TString BDTcut = "1"){ //get the yield of existing mass point obtained by interpolation
float yield_down = -1;
float yield_up = -1;
if(massPointDown != 0){
  yield_down = getYields_ggF(reader, massPointDown, masswindow, BDTcut);
}
else{
  cout << "No lower mass point available -----> Interpolation not possible " << endl;
  return 0;
}
if(massPointUp != 0){
  yield_up = getYields_ggF(reader, massPointUp, masswindow, BDTcut);
}
else{
  cout << "No upper mass point available -----> Interpolation not possible " << endl;
  return 0;
}
  float yieldPOI = computeYield(massPOI, massPointDown, massPointUp, yield_down, yield_up);
  //cout << "Interpolated yield for ggF mass point " << massPOI << " is " << yieldPOI << endl;
  return yieldPOI;
}

float getAcceptance_VBF(OutputReader* reader, float massPOI, int massPointDown, int massPointUp, int masswindow = 1, TString BDTcut = "1"){ //get the yield of existing mass point obtained by interpolation
float yield_down = -1;
float yield_up = -1;
if(massPointDown != 0){
  yield_down = getYields_VBF(reader, massPointDown, masswindow);
}
else{
  cout << "No lower mass point available -----> Interpolation not possible " << endl;
  return 0;
}
if(massPointUp != 0){
  yield_up = getYields_VBF(reader, massPointUp, masswindow);
}
else{
  cout << "No upper mass point available -----> Interpolation not possible " << endl;
  return 0;
}
  float yieldPOI = computeYield(massPOI, massPointDown, massPointUp, yield_down, yield_up);
  //cout << "Interpolated yield for ggF mass point " << massPOI << " is " << yieldPOI << endl;
  return yieldPOI;
}

int getMassDown(int massPOI, int isggF){// get the lower existing mass point in sim next to one given mass point that can exist in sim or not
  TString whichSample;
  vector<int> masspoints;
  if(isggF){ 
    masspoints = {14,16,18,20,25,30,40,50,55,60,62};
    whichSample = "ggF";
  }
  else{ 
    masspoints = {14,18,20,30,62}; 
    whichSample = "VBF";
  }

  int size = masspoints.size();

  vector<int>::iterator it = find(masspoints.begin(), masspoints.end(),massPOI);
  if(it != masspoints.end()){
    int index = distance(masspoints.begin(), it);
    if(0 != index){ 
      //cout << "Mass point down is " << masspoints[index-1] << endl;
      return masspoints[index-1];
    }
    else{ 
     //cout << "Given mass point " << massPOI << " is already at lower edge. Lower mass point will be " << masspoints[0] << endl;
     return 0;
    }
  }
  else {
    cout << "Not simulated mass point for " << whichSample << "." << endl;
    int min = -999;
    int massDown = 0;
    for(int i = 0; i<size; ++i){
      if((min < (masspoints[i] - massPOI)) && (masspoints[i] - massPOI) < 0){
        min = masspoints[i] - massPOI;
        massDown = masspoints[i];
        cout << massDown << endl;
      }
    }
    return massDown;
  }
return 0;
}




vector<int> getMassesLeft(int massPOI, int isggF){// get two lower existing mass point in sim next to one given mass point that can exist in sim or not
  TString whichSample;
  vector<int> masspoints;
  vector<int> myMassPoints;
  if(isggF){ 
    masspoints = {14,16,18,20,25,30,40,50,55,60,62};
    whichSample = "ggF";
  }
  else{ 
    masspoints = {14,18,20,30,62}; 
    whichSample = "VBF";
  }

  vector<int>::iterator it = find(masspoints.begin(), masspoints.end(),massPOI);
  if(it != masspoints.end()){
    int index = distance(masspoints.begin(), it);
    if(index > 1){ 
      myMassPoints.push_back(masspoints[index-1]);
      myMassPoints.push_back(masspoints[index-2]);
      //cout << "Lower mass points are " << myMassPoints[0] << " and " << myMassPoints[1] << endl;
      return myMassPoints;
    }
    else{ 
     //cout << "Given mass point " << massPOI << " is already at lower edge. Lower mass point will be " << masspoints[0] << endl;
     return {0};
    }
  }
  else {
    cout << "Given mass point does not exist for " << whichSample << endl;
    
  }
  return {0};
}

vector<int> getMassesRight(int massPOI, int isggF){// get two higher existing mass point in sim next to one given mass point that can exist in sim or not
  TString whichSample;
  vector<int> masspoints;
  vector<int> myMassPoints;
  if(isggF){ 
    masspoints = {14,16,18,20,25,30,40,50,55,60,62};
    whichSample = "ggF";
  }
  else{ 
    masspoints = {14,18,20,30,62}; 
    whichSample = "VBF";
  }
  int size = masspoints.size();
  vector<int>::iterator it = find(masspoints.begin(), masspoints.end(),massPOI);
  if(it != masspoints.end()){
    int index = distance(masspoints.begin(), it);
    if(fabs(index - size) > 2){ 
      myMassPoints.push_back(masspoints[index+1]);
      myMassPoints.push_back(masspoints[index+2]);
      //cout << "Higher mass points are " << myMassPoints[0] << " and " << myMassPoints[1] << endl;
      return myMassPoints;
    }
    else{ 
     //cout << "Given mass point " << massPOI << " is already at higher edge. Higher mass point will be " << masspoints[0] << endl;
     return {0};
    }
  }
  else {
    cout << "Given mass point does not exist for " << whichSample << endl;
  }
  return {0};
}


int getMassUp(int massPOI, int isggF){ //get the upper existing mass point in sim next to one given mass point that can exist in sim or not
  TString whichSample;
  vector<int> masspoints;
  if(isggF){ 
    masspoints = {14,16,18,20,25,30,40,50,55,60,62};
    whichSample = "ggF";
  }
  else{ 
    masspoints = {14,18,20,30,62}; 
    whichSample = "VBF";
  }
  int size = masspoints.size();
  vector<int>::iterator it = find(masspoints.begin(), masspoints.end(),massPOI);
  if(it != masspoints.end()){
    int index = distance(masspoints.begin(), it);
    if(1 != fabs(index - size)){ 
      //cout << "Mass point up is " << masspoints[index+1] << endl;
      return masspoints[index+1];
    }
    else{ 
     //cout << "Given mass point " << massPOI << " is already at upper edge. Upper mass point will be " << masspoints[index] << endl;
     return 0;
    }
  }
  else {
    cout << "Not simulated mass point for " << whichSample << "." << endl;
    int max = 999;
    int massUp = 0;
    for(int i = 0; i<size; ++i){
      if((max > (masspoints[i] - massPOI)) && (masspoints[i] - massPOI) > 0){
        max = masspoints[i] - massPOI;
        massUp = masspoints[i];
        cout << massUp << endl;
      }
    }
    return massUp;
  }
  return 0;
}

TString getBDTCut(int massPOI, TString myCut, TString mode = "normal",  TString myCrazyBDT = "1"){//mode normal (use BDT trained on that masspoint), mode comb (use comb BDT), mode else (crazy mode, use any BDT for given mass point), get string for BDT cut
  vector<int> masspoints = {14,16,18,20,25,30,40,50,55,60,62};
  vector<TString> BDTs = {
 "BDT_a14_dR_dimuon_m_12_16_configOct",
 "BDT_a16_dR_dimuon_m_12_20_configOct",
 "BDT_a18_dR_dimuon_m_14_22_configOct",
 "BDT_a20_dR_dimuon_m_16_24_configOct",
 "BDT_a25_dR_dimuon_m_21_29_configOct",
 "BDT_a30_dR_dimuon_m_26_34_configOct",
 "BDT_a40_dR_dimuon_m_36_44_configOct",
 "BDT_a50_dR_dimuon_m_46_54_configOct",
 "BDT_a55_dR_dimuon_m_51_59_configOct",
 "BDT_a60_dR_dimuon_m_56_64_configOct",
 "BDT_a62_dR_dimuon_m_58_66_configOct",
 "BDT_a14_16_18_20_dR_dimuon_m_12_24_configOct",
 "BDT_a60_62_dR_dimuon_m_56_66_configOct"};

  TString myBDTCut = "";
  vector<int>::iterator it = find(masspoints.begin(), masspoints.end(),massPOI);
  if(it != masspoints.end()){
    if("normal"==mode){
      int index = distance(masspoints.begin(), it);
      myBDTCut = BDTs[index] + myCut;
    }
    else if("comb"==mode){
        int index = distance(masspoints.begin(), it);
        if(masspoints[index] < 25){
          myBDTCut = BDTs[11] + myCut;
	} 
 	else if(masspoints[index] > 55){
	  myBDTCut = BDTs[11] + myCut;
	}
	else{ 
	  myBDTCut = BDTs[index] + myCut ;
	}
      }
    else if("else"==mode){ 
      myBDTCut = myCrazyBDT + myCut ;
    }
    else {
      cout << "Warning: Mode " << mode << " not supported!" << endl;
    }
  }
  else cout << "Mass point does not exist" << endl;
  cout << "MyBDTcut is " << myBDTCut << endl;
  return myBDTCut;
}


float compareYields(OutputReader* reader, float massPOI, int masswindow = 1, TString BDTcut = "1", int isggF = 1){
  int massUp = getMassUp(massPOI, isggF);
  int massDown = getMassDown(massPOI, isggF);
  float yieldTree = getYields_ggF(reader, massPOI, masswindow, BDTcut);
  float yieldInter = getAcceptance_ggF(reader, massPOI,massDown,massUp, masswindow, BDTcut);
  cout << "Yield for mass point " << massPOI << " is " << yieldTree << endl;
  float yieldComp = 0;
  if(yieldInter != 0){
    cout << "Interpolated yield between " << massDown << " and " << massUp << " for mass point " << massPOI << " is " << yieldInter << endl;
    yieldComp = fabs(yieldInter - yieldTree)/yieldTree;
    cout << "Difference between yield and interpolated yield: " << yieldComp << endl;
  }
  return yieldComp;
}

vector<float> compareExtYields(OutputReader* reader, float massPOI, int masswindow = 1, TString BDTcut = "1", int isggF = 1){
  vector<int> massright = getMassesRight(massPOI, isggF);
  vector<int> massleft = getMassesLeft(massPOI, isggF);
  float yieldTree = getYields_ggF(reader, massPOI, masswindow, BDTcut);
  float yieldExtRight = -999;
  float yieldExtLeft = -999;
  if(massright[0] != 0 && massright[0] != 0){
    yieldExtRight = getAcceptance_ggF(reader, massPOI,massright[0],massright[1], masswindow, BDTcut);
    if(yieldExtRight > 0){
      cout << "Extrapolated yield from mass " << massright[0] << " and " << massright[1] << " for mass point " << massPOI << " is " << yieldExtRight << endl;
    }
  }
  else{
    cout << "Extrapolation from right not possible" << endl;
  }
  if(massleft[0] != 0 && massleft[0] != 0){
    yieldExtLeft = getAcceptance_ggF(reader, massPOI,massleft[0],massleft[1], masswindow, BDTcut);
    if(yieldExtLeft > 0){
      cout << "Extrapolated yield from mass " << massleft[0] << " and " << massleft[1] << " for mass point " << massPOI << " is " << yieldExtLeft << endl;
    }
  }
  else{
    cout << "Extrapolation from left not possible" << endl;
  }

  vector<float> yieldComp = {};
  if(yieldExtRight > 0){
    float yieldCompRight = fabs(yieldExtRight - yieldTree)/yieldTree;
    yieldComp.push_back(yieldCompRight);
    cout << "Difference between yield and extrapolated yield from right is: " << yieldCompRight << endl;
  }
  else if(yieldExtRight < 0  && massright[0] != 0 && massright[0] != 0){ 
    cout << "The extrapolated yield from right is " <<yieldExtRight << ". Not physical, setting it to 0." << endl;
    yieldComp.push_back(0);
  }
  if(yieldExtLeft > 0){
    float yieldCompLeft = fabs(yieldExtLeft - yieldTree)/yieldTree;
    yieldComp.push_back(yieldCompLeft);
    cout << "Difference between yield and extrapolated yield from left is: " << yieldCompLeft << endl;
  }
  else if(yieldExtLeft < 0  && massleft[0] != 0 && massleft[0] != 0){ 
    cout << "The extrapolated yield from left is " << yieldExtLeft << ". Not physical, setting it to 0." << endl;
    yieldComp.push_back(0);
  }
  return yieldComp;
}

void compareYieldsWithCuts(OutputReader* reader, float massPOI, int masswindow, TString myCut, TString mode = "normal",  TString myCrazyBDT = "1", int isggF = 1, TString BDTcut = ""){
  cout << "---------------- Extrapolation/Interpolation for mass point " << massPOI << " ---------------- " <<endl;
  if(BDTcut != "") TString BDTcut = getBDTCut(massPOI, myCut, mode, myCrazyBDT);
  //cout << "Mybdt cut is: " << BDTcut << endl;
  compareYields(reader, massPOI, masswindow, BDTcut, isggF);
  compareExtYields(reader, massPOI, masswindow, BDTcut, isggF);
}


//for getting acceptance: 
//divide for ggF yields by 48.58 * 0.0016 * 139000
//divide for VBF yields by 3.782 * 0.0016 * 139000

vector<int> getMassVector(int exclMass = 0, int isggF = 1){
  vector<int> mp;
  TString whichSample = "";
  if(isggF){
    whichSample = "ggF";
    //mp = {14,16,18,20,25,30,40,50,55,60,62};
    //mp  = {15,30,45,60,75};
    //mp  = {100,120,140,160};
    mp  = {120,140,160};
    //mp  = {140,160};   
  }
  else{
    whichSample = "VBF";
    mp = {14,18,20,30,62};
  }
unsigned int size = mp.size();
vector<int>::iterator it = find(mp.begin(), mp.end(),exclMass);
vector<int> new_mp;
  if((0 != exclMass) && (it != mp.end())){
    unsigned int index = distance(mp.begin(), it);
    for(unsigned int i = 0; i < size; ++i){
      if(i != index){
        //cout << "i: " << i << " mp[i] " << mp[i] <<endl;
        new_mp.push_back(mp[i]);  
      }
    }
   return new_mp;
  }
  else if((0 == exclMass)){
    for(unsigned int i = 0; i < mp.size(); ++i){
        new_mp.push_back(mp[i]);  
    }
    return new_mp;
  }
  else{
    cout << "Mass point does not exist for sample " << whichSample << endl;
    return {0};  
  }

}

void doInterpolation(OutputReader* reader, TGraphErrors *&yields, vector<int> mp, double masswindow, int isggF = 1, TString BDTcut = "1"){

//TString cuts = BDTcut + "&& klbbmm_m>110 && klbbmm_m<140 && met<60 && klfit_loglikelihood>-8 && mc_channel_number <= 346075 &&";
TString cuts = "isOS ==1 && ( mu2Pt/mu1Pt > .2 || Mmu1e_ss > 0 ) && nElectrons_signal == 1 && nMuons_signal ==2 && isOS ==1 && elec1Pt >= 20 && nJet20 >=3 && nBjets_77 >=1 &&";

TString whichSample = "ggF";
if(!isggF){
  cuts = BDTcut + "&& klbbmm_m>110 && klbbmm_m<140 && met<60 && klfit_loglikelihood>-8 && mc_channel_number > 346075 &&";
  whichSample = "VBF";
}

yields = new TGraphErrors(mp.size());

int mA = 30; 
//int mA = 45;
//int mA = 15;

int i = 0;
for(auto masspoint : mp){
  i +=1;
  //RegionData SR = getRegionData(Form("SR%d",masspoint), "all", Form("SR%d",masspoint), cuts + getDimuonCut(masspoint,masswindow));
  RegionData SR = getRegionData(Form("SR%d",mA), "all", Form("SR%d",mA), cuts + getDimuonCut(mA,masswindow));
  //TH1* h_signal = reader->getHistoFromTree(Form("a%d",masspoint),SR.region,"1",Form("signal%d",masspoint),1,0,2,"event_weight/48.58/0.0016/139000*(" + SR.cuts + ")"); //acceptance
  //TH1* h_signal = reader->getHistoFromTree(Form("a%d",masspoint),SR.region,"1",Form("signal%d",masspoint),1,0,2,"event_weight*(" + SR.cuts + ")");
  TH1* h_signal = reader->getHistoFromTree(Form("%d",masspoint),SR.region,"1",Form("signal%d",masspoint),1,0,2,"event_weight*(" + SR.cuts + ")");
  float yield = h_signal->GetBinContent(1);
  yields->SetPoint(i-1,masspoint,yield);
  yields->SetPointError(i-1,0,h_signal->GetBinError(1)); 
  yields->GetXaxis()->SetTitle("Hplus mass [GeV]");
  yields->GetYaxis()->SetTitle("Yields");
}
}



void drawGraph(vector<TGraphErrors*> tgraph_vec, TString prodmode, vector<TString> BDTs){
TCanvas *tgraph_interpol = new TCanvas(prodmode,prodmode,200,10,700,500);
vector<int> colors = {kAzure, kAzure+3,kGreen, kGreen-3, kRed, kRed-3, kViolet,kViolet-3, kOrange+3, kOrange, kOrange-3, kMagenta, kMagenta-3, kBlue, kBlue-7};

for (unsigned int i = 0; i< tgraph_vec.size();++i){
  tgraph_vec[i]->SetLineColor(colors[i]);
  tgraph_vec[i]->SetMarkerColor(colors[i]);
  if(0==i){
    //tgraph_vec[i]->Draw("ALPE");
    tgraph_vec[i]->Draw("ALP");
    //tgraph_vec[i]->GetYaxis()->SetRangeUser(0,50);
    tgraph_vec[i]->GetXaxis()->SetLimits(0,80);
  }
  else tgraph_vec[i]->Draw("LPsame");
}

TLegend *leg = getTLegend(0.2, 0.5, 0.43, 0.92);
leg->AddEntry(tgraph_vec[0], BDTs[0],"LEP");
for (unsigned int i = 1; i< tgraph_vec.size();++i){
leg->AddEntry(tgraph_vec[i], "BDT"+BDTs[i],"LP");
}
leg->Draw();

gSystem->Exec("mkdir -p plots/Interpolation/");
tgraph_interpol->SaveAs("plots/Interpolation/yields_hplus" + prodmode + ".png");
}


void drawGraphSpline3(vector<TGraphErrors*> tgraph_vec, TString prodmode, vector<TString> BDTs, vector<TSpline3*> spline){
TCanvas *tgraph_spline = new TCanvas(prodmode,prodmode,200,10,700,500);
vector<int> colors = {kRed, kAzure+3,kGreen, kGreen-3, kRed, kRed-3, kViolet,kViolet-3, kOrange+3, kOrange, kOrange-3, kMagenta, kMagenta-3, kBlue, kBlue-7};

for (unsigned int i = 0; i< tgraph_vec.size();++i){
  spline[i]->SetLineColor(colors[i]);
  tgraph_vec[i]->SetMarkerColor(colors[i]);
  if(0==i){
    //tgraph_vec[i]->Draw("AP"); //original 
    tgraph_vec[i]->Draw("ALP"); 
    //tgraph_vec[i]->GetYaxis()->SetRangeUser(0,1200);
    tgraph_vec[i]->GetXaxis()->SetLimits(95,170);
    tgraph_vec[i]->SetTitle("A = 15 GeV");

  }
  else tgraph_vec[i]->Draw("LPsame");
  spline[i]->Draw("LPsame");

}

TLegend *leg = getTLegend(0.15, 0.75, 0.35, 0.92);
leg->AddEntry(tgraph_vec[0], BDTs[0],"LEP");
for (unsigned int i = 1; i< tgraph_vec.size();++i){
leg->AddEntry(tgraph_vec[i], "BDT"+BDTs[i],"LP");
}
leg->SetTextSize(0.03);
leg->Draw();

gSystem->Exec("mkdir -p plots/Interpolation/");
tgraph_spline->SaveAs("plots/Interpolation/yields_hplus_spline3" + prodmode + ".png");
}



void drawGraphSpline1(vector<TGraphErrors*> tgraph_vec, TString prodmode, vector<TString> BDTs, vector<TMVA::TSpline1*> spline){
TCanvas *tgraph_spline = new TCanvas(prodmode,prodmode,200,10,700,500);
vector<int> colors = {kRed, kAzure+3,kGreen, kGreen-3, kRed, kRed-3, kViolet,kViolet-3, kOrange+3, kOrange, kOrange-3, kMagenta, kMagenta-3, kBlue, kBlue-7};

for (unsigned int i = 0; i< tgraph_vec.size();++i){
  spline[i]->SetLineColor(colors[i]);
  tgraph_vec[i]->SetMarkerColor(colors[i]);
  if(0==i){
    tgraph_vec[i]->Draw("ALP");
    //tgraph_vec[i]->GetYaxis()->SetRangeUser(0,0.03);
    tgraph_vec[i]->GetXaxis()->SetLimits(95,170);
  }
  else tgraph_vec[i]->Draw("Psame");
  spline[i]->Draw("Psame");
}

TLegend *leg = getTLegend(0.2, 0.5, 0.43, 0.92);
leg->AddEntry(tgraph_vec[0], BDTs[0],"LEP");
for (unsigned int i = 1; i< tgraph_vec.size();++i){
leg->AddEntry(tgraph_vec[i], "BDT"+BDTs[i],"LP");
}
leg->Draw();

gSystem->Exec("mkdir -p plots/Interpolation/");
tgraph_spline->SaveAs("plots/Interpolation/yields_spline1_hplus" + prodmode + ".png");
}



void doInterpolation(OutputReader* reader, double masswindow, TString BDTcut){

vector<int> mp_ggF = getMassVector(0,1); 
vector<int> mp_VBF = getMassVector(0,0);

unsigned int size_ggF = mp_ggF.size();
unsigned int size_VBF = mp_VBF.size();

vector<vector<int>> mp_interggF;
cout<<size_ggF<<endl;
for(unsigned int j = 0; j<size_ggF; ++j){
  mp_interggF.push_back(getMassVector(mp_ggF[j],1));
}

vector<vector<int>> mp_interVBF;
for(unsigned int j = 0; j<size_VBF; ++j){
  mp_interVBF.push_back(getMassVector(mp_VBF[j],0));
}

TString legend = "sim. MP, " + TString(Form("window %.2f",masswindow*2))+ " GeV" ;

//vector<TString> BDTs = {"no BDT cut","lowmasscomb", "14","16","18","20","25","30","40","50","55","60","62", "highmasscomb"};
vector<TString> BDTs = {legend};

/*
vector<TString> BDTNames = {"1", 
 "BDT_a14_16_18_20_dR_dimuon_m_12_24_configOct", 
 "BDT_a14_dR_dimuon_m_12_16_configOct",
 "BDT_a16_dR_dimuon_m_12_20_configOct", 
 "BDT_a18_dR_dimuon_m_14_22_configOct",
 "BDT_a20_dR_dimuon_m_16_24_configOct", 
 "BDT_a25_dR_dimuon_m_21_29_configOct", 
 "BDT_a30_dR_dimuon_m_26_34_configOct", 
 "BDT_a40_dR_dimuon_m_36_44_configOct", 
 "BDT_a50_dR_dimuon_m_46_54_configOct", 
 "BDT_a55_dR_dimuon_m_51_59_configOct", 
 "BDT_a60_dR_dimuon_m_56_64_configOct", 
 "BDT_a62_dR_dimuon_m_58_66_configOct", 
 "BDT_a60_62_dR_dimuon_m_56_66_configOct"};
*/

vector<TString> BDTNames = {"1"};


unsigned int BDTsize = BDTs.size();

vector<TGraphErrors*> tgraph_ggF(BDTsize); 
vector<TGraphErrors*> tgraph_VBF(BDTsize); 

vector<vector<TGraphErrors*>> tgraphinter_ggF(BDTsize); 
vector<vector<TGraphErrors*>> tgraphinter_VBF(BDTsize); 
for(unsigned int i=0; i<BDTsize ; ++i){
  tgraphinter_ggF[i].resize(size_ggF);
  tgraphinter_VBF[i].resize(size_VBF);
}


for(unsigned int i=0; i<BDTs.size(); ++i){
  TString cut = "";
  //if(i != 0){
  //  cut = ">" + BDTcut;
  //}
  doInterpolation(reader, tgraph_ggF[i], mp_ggF, masswindow, 1, BDTNames[i] + cut);
  //doInterpolation(reader, tgraph_VBF[i], mp_VBF, masswindow, 0, BDTNames[i] + cut);
  for(unsigned int j=0; j<size_ggF; ++j){
    doInterpolation(reader, (tgraphinter_ggF[i])[j], mp_interggF[j], masswindow, 1, BDTNames[i] + cut);
  }
  //for(unsigned int j=0; j<size_VBF; ++j){
  //  doInterpolation(reader, (tgraphinter_VBF[i])[j], mp_interVBF[j], masswindow, 0, BDTNames[i] + cut);
  //}
}

drawGraph(tgraph_ggF, TString(Form("ggF_mumuwindow%f",masswindow))+"BDTcut"+BDTcut, BDTs);
//drawGraph(tgraph_VBF, TString(Form("VBF_mumuwindow%d",masswindow))+"BDTcut"+BDTcut, BDTs);

vector<TSpline3*> spline_ggF;
for(unsigned int i = 0; i<BDTsize;++i){
  TSpline3* spline = new TSpline3(BDTs[i],tgraph_ggF[i]);
  spline_ggF.push_back(spline);
}

vector<TMVA::TSpline1*> spline1_ggF;
for(unsigned int i = 0; i<BDTsize;++i){
  TMVA::TSpline1* spline = new TMVA::TSpline1(BDTs[i],tgraph_ggF[i]);
  spline1_ggF.push_back(spline);
}

cout << "Dimuon mass window is: " << masswindow << endl;
//for(unsigned int m = 14; m < 77; ++m){ 
//for(unsigned int m = 100; m < 161; ++m){ //for Hplus
for(unsigned int m = 120; m < 161; ++m){
  for(unsigned int i = 0; i<BDTsize;++i){
    //cout << "Yield at mass for spline3 " << m << " for BDT " << BDTNames[i] << " is: " << spline_ggF[i]->Eval(m) << endl;
    cout << spline_ggF[i]->Eval(m) << endl;
  }
}

cout << "Yield at mass for spline1 " << endl; 

//for(unsigned int m = 14; m < 77; ++m){
//for(unsigned int m = 100; m < 161; ++m){  //for Hplus
for(unsigned int m = 120; m < 161; ++m){  //for Hplus
  for(unsigned int i = 0; i<BDTsize;++i){
  //  cout << "Yield at mass for spline1 " << m << " for BDT " << BDTNames[i] << " is: " << spline1_ggF[i]->Eval(m) << endl;
  cout << spline1_ggF[i]->Eval(m) << endl;
  }
}

drawGraphSpline3(tgraph_ggF, TString(Form("ggF_mumuwindow%f",masswindow))+"BDTcut"+BDTcut, BDTs,spline_ggF);
drawGraphSpline1(tgraph_ggF, TString(Form("ggF_mumuwindow%f",masswindow))+"BDTcut"+BDTcut, BDTs,spline1_ggF);

vector<vector<TSpline3*>> spline_inter_ggF(BDTsize);
for(unsigned int i = 0; i<BDTsize;++i){
  for(unsigned int g = 0; g<size_ggF;++g){
    TSpline3* spline = new TSpline3(BDTs[i],(tgraphinter_ggF[i])[g]);
    spline_inter_ggF[i].push_back(spline);
  }
}

for(unsigned int i = 0; i<BDTsize;++i){
  for(unsigned int g = 0; g<size_ggF;++g){
    float MCyield = spline_ggF[i]->Eval(mp_ggF[g]);
    float remYield = (spline_inter_ggF[i])[g]->Eval(mp_ggF[g]);
    cout << "Mass point: " << mp_ggF[g] << " BDTName: "<< BDTNames[i] << ", MC yield: " << MCyield << ", yield with point removed: " << remYield << ", difference: " << (MCyield - remYield)/MCyield << endl;
  }
}


}

/*
void readSignal(OutputReader* reader){
//double scaleXsec = 48.52 * 1.6e-3 / 14.15; //different xsec in the ntuples, assuming for now that all xsecs in the ntuples are 14.15, next time running put to 1 because xsec now corrected in the skimslim
double scaleXsec = 1.;
//double scaleXsec = 139/36.;
TString data_dir = getenv("HBBMUMU_DATA_PATH");
TString directory = data_dir + "/rel21/skimSlimNtuples/NewDerivation/";
vector<TString> signalvector = {"14", "16", "18", "20", "25", "30", "40", "50", "55", "60", "62"};
vector<float> signalScale = {0.2, 0.2, 0.2, 0.2, 0.15, 0.12, 0.32, 0.53, 0.35, 0.21, 0.21}; //put by hand, interpolation by eye for mass points where we didnt have scaling
for (unsigned int i=0; i<signalvector.size(); ++i){
  TString signal = signalvector.at(i);
  //reader->readTree("a"+signal,"all",directory+"Run2_NewDer_1311.root","signal"+signal+"__Nominal",scaleXsec*signalScale[i]);//remove scaling when drawing efficiency or when doing acceptance studies
  reader->readTree("a"+signal,"all",directory+"Run2_NewDer_1311.root","signal"+signal+"__Nominal",scaleXsec); 
  }
}
*/

void readSignal(OutputReader* reader){
  //double scaleXsec = 48.52 * 1.6e-3 / 14.15;  
  //double scaleXsec = 48.52 * 1.6e-3 / 1; //different xsec in the ntuples, assuming for now that all xsecs in the ntuples are 14.15, next time running put to 1 because xsec now corrected in the skimslim - need to correct this - WA
  //double scaleXsec = 831.76 *1* 1 *3e-4 /2 ; // divide by 2 as it is the default xsec in ntuples for 160 GeV
  //double scaleXsec = 1*100/(831.76*3e-4*1000*139) ; 
  double scaleXsec = 1 ; 

  TString data_dir = getenv("HBBMUMU_DATA_PATH");
  //TString directory =   "/eos/home-w/waahmed/afs_runs/old_ptag/signal/jun2/";
  ///TString rundate = date;
  TString rundate = "sept23";


  //TString directory = "/eos/home-w/waahmed/afs_runs/new_ptag/sig/" + rundate+"/new_xsec/";
  //TString directory = "/eos/home-w/waahmed/afs_runs/new_ptag/sig/2020/" + rundate +"/scaled/";
  TString directory = "/eos/home-w/waahmed/afs_runs/new_ptag/sig/2020/" + rundate +"/"; 

  //vector<TString> signalvector = {"14", "16", "18", "20", "25", "30", "40", "50", "55", "60", "62"};
  //vector<float> signalScale = {0.2, 0.2, 0.2, 0.2, 0.15, 0.12, 0.32, 0.53, 0.35, 0.21, 0.21}; //put by hand, interpolation by eye
  //vector<TString> signalvector = {"30"};
  //vector<TString> signalvector = {"H100a15","H120a15","H120a30","H140a15","H140a30","H140a45","H160a15","H160a30","H160a45","H160a60","H160a75"};
  //vector<TString> signalvector = {"15","30","45","60","75"}; // interpolation along mA 
  vector<TString> signalvector = {"120","140","160"}; // interpolation along H+
  //vector<TString> signalvector = {"140","160"};
  //vector<TString> signalvector = {"100","120","140","160"}; 
  vector<float> signalScale = {1}; // where to get this number from? WA
  
  TString mA = "a30"; 
  //TString mA = "a15";
  for (unsigned int i=0; i<signalvector.size(); ++i){
    TString signal = signalvector.at(i);
    //TString signaltree = "H160a" + signal; //interpolation along mA
    TString signaltree = "H"+ signal + mA; //for interpolation along H+

    //reader->readTree("a"+signal,"all",directory+"SignalBDTFiles.root","signal"+signal+"__Nominal",scaleXsec*signalScale[i]);
    //reader->readTree("a"+signal,"all",directory+"a1"+signal+"/dummyyear/dummysample/tree_NoSys.root","dummysample__Nominal"); 
    //reader->readTree("a"+signal,"all",directory+"/sig30_16d.root","signal30__Nominal",.30); 
    reader->readTree(signal,"all",directory+signaltree+"_16a.root", signaltree +"__Nominal",scaleXsec); //use .1 for Br(t->bH) ==.1 with scaled MC. Use .2 for SR for 160 H+ plots, .015 for MPs for the unscaled to make plots look nice. 
    reader->readTree(signal,"all",directory+signaltree+"_16d.root", signaltree +"__Nominal",scaleXsec);
    reader->readTree(signal,"all",directory+signaltree+"_16e.root", signaltree +"__Nominal",scaleXsec);
  }
}




void Interpolation_Hplus(){

  cout << " start job " << endl;

  OutputReader* reader = new OutputReader();
  //readData(reader);
  readSignal(reader);
  /*compareYieldsWithCuts(reader, 14, 1, ">0","normal", "1",1,"");
  compareYieldsWithCuts(reader, 16, 1, ">0","normal", "1",1,"");
  compareYieldsWithCuts(reader, 18, 1, ">0","normal", "1",1,"");
  compareYieldsWithCuts(reader, 20, 1, ">0","normal", "1",1,"");
  compareYieldsWithCuts(reader, 25, 1, ">0","normal", "1",1,"");
  compareYieldsWithCuts(reader, 30, 1, ">0","normal", "1",1,"");
  compareYieldsWithCuts(reader, 40, 2, ">0","normal", "1",1,"");
  compareYieldsWithCuts(reader, 50, 2, ">0","normal", "1",1,"");
  compareYieldsWithCuts(reader, 55, 2, ">0","normal", "1",1,"");
  compareYieldsWithCuts(reader, 60, 2, ">0","normal", "1",1,"");
  compareYieldsWithCuts(reader, 62, 2, ">0","normal", "1",1,"");*/
  //compareYieldsWithCuts(reader, 33, 1, "","normal", "1",1,"BDT_a30_dR_dimuon_m_26_34_configOct>0.4");
  //doInterpolation(reader,1,"-5");
  //doInterpolation(reader)
  // the mass is window is half the SR window e.g. 1 represents a 2 GeV window
  //doInterpolation(reader,1,"");
  //doInterpolation(reader,.75,"");
  doInterpolation(reader,1,"");
  //doInterpolation(reader,2,"");
  //doInterpolation(reader,1,"0.1");
  //doInterpolation(reader,2,"0.1");
  //doInterpolation(reader,1,"0.2");
  //doInterpolation(reader,2,"0.2");


}


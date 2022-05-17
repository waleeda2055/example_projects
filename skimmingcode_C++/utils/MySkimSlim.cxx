#include "HaaSkimming/SkimSlim.h"
#include "HaaSkimming/Config.h"
#include <TROOT.h>
#include <TChain.h>
#include <TH1F.h>
#include <TSystem.h>
#include <stdlib.h>
#include <map>
#include <ctime>
#include <fstream>

void readCBKFile(TString filename, map<TString, TheoryWeightSyst> &myMap){

std::ifstream inFile(filename.Data());
if (!inFile) {
    std::cout << "Unable to open ttbarTheorySys.txt file";
    abort(); // terminate with error
}

std::string str;
if (inFile.is_open()){
  while(!inFile.eof()){
    getline(inFile, str);
    if(str.size()==0) continue;
    std::size_t found = str.find("#");
    TString tstr = str.c_str();
    if (found == std::string::npos){
      TObjArray* token = tstr.Tokenize(" ");
      TString branchTree = ((TObjString*)token->At(0))->GetString();
      TString branchCBK = ((TObjString*)token->At(1))->GetString();
      TheoryWeightSyst turnip;
      turnip.CBKName = branchCBK;
      turnip.weightName = branchTree;
      if(!myMap.insert(make_pair(branchTree,turnip)).second) std::cout << "WARNNG: Duplicated branch name " << branchTree << " in file " << filename << std::endl;
    }
  }
}
}

void getSOWSyst(TChain *chain, map<TString, TheoryWeightSyst> &myMapFromBefore){
vector<float> partialSOW(myMapFromBefore.size(),0);
int DSID = 0;
chain->SetBranchAddress("DatasetNumber", &DSID);

int i = 0;
for(auto weights : myMapFromBefore){
  chain->SetBranchAddress(weights.second.CBKName, &(partialSOW[i++]));
}

unsigned int nentries = chain->GetEntries();
for(unsigned int entry = 0; entry < nentries; ++entry){
  chain->GetEntry(entry);
  int i = 0;
  for(auto &weights : myMapFromBefore){
    if(weights.second.SOW.find(DSID) == weights.second.SOW.end()){
      weights.second.SOW[DSID] = partialSOW[i];
    }
    else{
      weights.second.SOW[DSID] += partialSOW[i];
    }
    ++i;
  }
}

}

map<int,double> getSOW(TChain *chain, bool isData){

TObjArray *array = chain->GetListOfFiles();
map<int,double> sowmap;
map<int,int> nrFiles;

for(unsigned int a = 0; a < (unsigned int)(array->GetEntries()); ++a){
TString filename = array->At(a)->GetTitle();
TFile *f = TFile::Open(filename);
TH1 *hist = (TH1*)f->Get("weighted__AOD");
//TTree* tree = (TTree*)f->Get("CutBookkeepers");
 if(!hist){
   std::cout << "WARNING: could not find weighted__AOD histo in file " << filename << std::endl;
   std::cout << "---> ignoring, check file especially if you are running on MC" << std::endl;
   continue;
 }

//  if(!tree){
//    std::cout << "WARNING: Could not find CutBookkeepers tree in file" << filename << std::endl;
//    std::cout << "---> ignoring, check file especially if you are running on MC" << std::endl;
//  }

double sow = hist->GetBinContent(1);

TString DSIDStr = hist->GetXaxis()->GetBinLabel(1);
int DSID = DSIDStr.Atoi();
if(isData) DSID = 999;

map<int,int>::iterator j = nrFiles.find(DSID);
if(j == nrFiles.end()) nrFiles[DSID] = 1;
else nrFiles[DSID] += 1;

map<int,double>::iterator i = sowmap.find(DSID);
if (i == sowmap.end()) sowmap[DSID] = sow;
else sowmap[DSID] += sow;
delete f;
}

for (auto n : nrFiles){
  cout << "DSID " << n.first << " has " << n.second << " files and the SOW is " << sowmap[n.first] << endl;
}
return sowmap;

}

TH1* shiftHisto(TH1* hist){
TH1 *h1 = new TH1D(hist->GetName(),"",50,0,50);
int nbins = hist->GetNbinsX();
for (int i=1; i<nbins; ++i){
  h1->SetBinContent(i+1,hist->GetBinContent(i));
  h1->GetXaxis()->SetBinLabel(i+1,hist->GetXaxis()->GetBinLabel(i));
  } 
return h1;
}


void compLabels(TH1* h1, TH1* h2){
  int nbins1 = h1->GetNbinsX();
  int nbins2 = h2->GetNbinsX();

  if(nbins1 != nbins2){
    cout << "ERROR: Number of bins of cutflow histograms not same. Abort!" << endl;
    abort();
  }

  for(int i = 1; i < nbins1 + 1; ++i){
    TString label1 = h1->GetXaxis()->GetBinLabel(i);
    TString label2 = h2->GetXaxis()->GetBinLabel(i);
    if(label1 != label2 && label1 != "" && label2 != ""){
      cout << "ERROR: Bin labels not same. Abort!" << endl;
      abort();
    }
    if(label1 == "" && label2 == ""){
      break;
    }
    if(label1 == "") h1->GetXaxis()->SetBinLabel(i,label2);
    if(label2 == "") h2->GetXaxis()->SetBinLabel(i,label1);
  } 
}

void cutFlow(TChain *chain, map<int,double> SOW, map<int, TH1*> &histmap, map<int, TH1*> &histmap1, bool isData, TString samplePrefix ){

map<int, double> xsecmap;
map<int, int> nEventsMap;
TObjArray *array = chain->GetListOfFiles();
float xsec = 0;
for(unsigned int a = 0; a < (unsigned int)(array->GetEntries()); ++a){
TString filename = array->At(a)->GetTitle();
TFile *f = TFile::Open(filename);
TH1 *hist = (TH1*)f->Get("weighted__AOD");
TH1 *hist1 = (TH1*)f->Get("unweighted__AOD");
 if(!hist1){
   std::cout << "WARNING: could not find unweighted__AOD histo in file " << filename << std::endl;
   std::cout << "---> ignoring, check file especially if you are running on MC" << std::endl;
   continue;
 }
TString DSIDStr = hist->GetXaxis()->GetBinLabel(1);
if(isData) DSIDStr = "999";
TH1 *cutflow = (TH1*)f->Get("cutFlow___weighted")->Clone("cutflow___weighted_" + DSIDStr + samplePrefix );
TH1 *cutflow1 = (TH1*)f->Get("cutFlow___unweighted")->Clone("cutflow___unweighted_" + DSIDStr + samplePrefix );
if(cutflow->GetEntries()==0) continue;
cutflow->SetDirectory(0); //tells to ROOT that this histogram does not belong to file f
cutflow1->SetDirectory(0);
int DSID = DSIDStr.Atoi();

map<int,TH1*>::iterator j = histmap.find(DSID);
map<int,TH1*>::iterator k = histmap1.find(DSID);
map<int, int>::iterator m = nEventsMap.find(DSID);

if(j == histmap.end()) histmap[DSID] = cutflow;
else{ 
  compLabels(cutflow,histmap[DSID]);
  histmap[DSID]->Add(cutflow);
}

if(k == histmap1.end()) histmap1[DSID] = cutflow1;

else{
  compLabels(cutflow1,histmap1[DSID]);
  histmap1[DSID]->Add(cutflow1);
}

if(m == nEventsMap.end()) nEventsMap[DSID] = hist1->GetBinContent(1);
else nEventsMap[DSID] += hist1->GetBinContent(1);

map<int,double>::iterator i = xsecmap.find(DSID);
if (i == xsecmap.end()){
  TTree* tree = (TTree*)f->Get("tree_NoSys");
  tree->SetBranchAddress("xsec", &xsec);
  if (0<tree->GetEntries()){
    tree->GetEntry(0); 
    xsecmap[DSID] = xsec;
  }
}
f->Clear();
f->Close();
delete f;
}

for (pair<const int, TH1*> &histpair : histmap){
map<int, double>::iterator j = xsecmap.find(histpair.first);
if (j == xsecmap.end() && !isData) cout << "WARNING: " << histpair.first << " does not have corresponding cross section!" << endl;
else{
  TH1 *h1 = shiftHisto(histpair.second);
  delete histpair.second;
  histpair.second = h1;
  if(!isData) histpair.second->SetBinContent(0,xsecmap[histpair.first]);
  histpair.second->SetBinContent(1,SOW[histpair.first]);
  }
}

for (auto &histpair : histmap1){
map<int, double>::iterator j = xsecmap.find(histpair.first);
if (j == xsecmap.end() && !isData) cout << "WARNING: " << histpair.first << " does not have corresponding cross section!" << endl;
else{
  TH1 *h1 = shiftHisto(histpair.second);
  delete histpair.second;
  histpair.second = h1;
  if(!isData) histpair.second->SetBinContent(0,xsecmap[histpair.first]);
  histpair.second->SetBinContent(1,nEventsMap[histpair.first]);
  }
}

}
 

void processOutput(TChain *tree, TFile *outputfile, TString newTreeName, bool isData, bool isNominal, Config &myConfig, map<int,double> SOW, map<int, TH1*> histmap, map<int, TH1*> histmap1, bool isSignal, float lumi, map<TString, TheoryWeightSyst> &CBKMap){


SkimSlim *MyNewSkimSlim = new SkimSlim();
MyNewSkimSlim->outputfile = outputfile;
MyNewSkimSlim->newTreeName1 = "DDZ__Nominal";
MyNewSkimSlim->newTreeName2 = "1tag";
MyNewSkimSlim->isNominal = isNominal;
MyNewSkimSlim->newTreeName3 = newTreeName;
MyNewSkimSlim->sumOfWeights = SOW;
MyNewSkimSlim->mapOfHists = histmap;
MyNewSkimSlim->mapOfUnweightedHists = histmap1;
MyNewSkimSlim->lumi = lumi;
MyNewSkimSlim->isData = isData;
MyNewSkimSlim->doIsolation = false;
MyNewSkimSlim->doCMS = false;
MyNewSkimSlim->theoryMap = CBKMap;
MyNewSkimSlim->createRadDown = myConfig.createRadDown;
MyNewSkimSlim->createRadUp = myConfig.createRadUp;
TString inputdir = "/afs/cern.ch/work/a/admilic/HFbbmumu/NewSkimming/skimming-code/HaaSkimming/data/Training/dataset/weights/";
/*
BDTConfig config1(inputdir + "/data/Training/dataset/weights/TMVAClassification_a20_dR_dimuon_m_16_24_config5_", "BDT_a20_dR_dimuon_m_16_24_config5");
BDTConfig config2(inputdir + "/data/Training/dataset/weights/TMVAClassification_a30_dR_dimuon_m_26_34_config5_", "BDT_a30_dR_dimuon_m_26_34_config5");
BDTConfig config3(inputdir + "/data/Training/dataset/weights/TMVAClassification_a40_dR_dimuon_m_36_44_config5_", "BDT_a40_dR_dimuon_m_36_44_config5");
BDTConfig config4(inputdir + "/data/Training/dataset/weights/TMVAClassification_a50_dR_dimuon_m_46_54_config5_", "BDT_a50_dR_dimuon_m_46_54_config5");
BDTConfig config5(inputdir + "/data/Training/dataset/weights/TMVAClassification_a60_dR_dimuon_m_56_64_config5_", "BDT_a60_dR_dimuon_m_56_64_config5");

BDTConfig config1(inputdir + "TMVAClassification_a14_dR_dimuon_m_10_18_config5_", "BDT_a14_dR_dimuon_m_10_18_config5");
BDTConfig config2(inputdir + "TMVAClassification_a16_dR_dimuon_m_12_20_config5_", "BDT_a16_dR_dimuon_m_12_20_config5");
BDTConfig config3(inputdir + "TMVAClassification_a20_dR_dimuon_m_16_24_config5_", "BDT_a20_dR_dimuon_m_16_24_config5");
BDTConfig config4(inputdir + "TMVAClassification_a25_dR_dimuon_m_21_29_config5_", "BDT_a25_dR_dimuon_m_21_29_config5");
BDTConfig config5(inputdir + "TMVAClassification_a30_dR_dimuon_m_26_34_config5_", "BDT_a30_dR_dimuon_m_26_34_config5");
BDTConfig config6(inputdir + "TMVAClassification_a40_dR_dimuon_m_36_44_config5_", "BDT_a40_dR_dimuon_m_36_44_config5");
BDTConfig config7(inputdir + "TMVAClassification_a55_dR_dimuon_m_51_59_config5_", "BDT_a55_dR_dimuon_m_51_59_config5");
BDTConfig config8(inputdir + "TMVAClassification_a60_dR_dimuon_m_56_64_config5_", "BDT_a60_dR_dimuon_m_56_64_config5");
BDTConfig config9(inputdir + "TMVAClassification_a62_dR_dimuon_m_58_66_config5_", "BDT_a62_dR_dimuon_m_58_66_config5");
MyNewSkimSlim->BDT_configs = {config1, config2, config3, config4, config5, config6, config7, config8, config9};
*/
MyNewSkimSlim->BDT_configs = {};
//MyNewSkimSlim->BDT_configs = {config1, config2, config3, config4, config5};
if(isNominal && !isSignal) MyNewSkimSlim->write0Tree = true;
MyNewSkimSlim->maxEvents = myConfig.maxEvents;
MyNewSkimSlim->Init(tree);
MyNewSkimSlim->Loop();
delete MyNewSkimSlim;

}


void getSampleYear(TString inputTextFileName, TString &sample, TString &year){

TString name = inputTextFileName.ReplaceAll(".txt",""); 
TObjArray* tokens1 = name.Tokenize("/");

TString name1 = ((TObjString*)tokens1->Last())->GetString();
cout << name1 << endl;
if (!name1.Contains("_")){
  cout << "ERROR: Text file name format should be of the form: SAMPLE_CAMPAIGN_XXX.txt, for example ttbar_mc16a.txt" << endl;
  abort();
} 
TObjArray* tokens = name1.Tokenize("_");
sample = ((TObjString*)tokens->At(0))->GetString();
year = ((TObjString*)tokens->At(1))->GetString();

}

void coutTime(){
  time_t t = time(0);   // get time now
  struct tm * now = localtime( & t );
  cout <<  (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-'<<  now->tm_mday << '-' << now->tm_hour << '-' << now->tm_min << endl;

}


int main(int argc, char *argv[]){

cout << "running " << argv[0] << endl;

Config myConfig = Config::getInstance();
int status = parseArgs(myConfig, argc, argv);
if(status != 0) return status;
TString sample;
TString year;

cout << "Inputfile is " << myConfig.inputTextFileName << endl;

if (myConfig.inputTextFileName != "") getSampleYear(myConfig.inputTextFileName,sample,year);
else{ 
  sample = myConfig.sample;
  year = myConfig.year;
}


TString yearforlumi = year;
yearforlumi.ToLower();

float lumi = 0;
if(yearforlumi.Contains("16a")) lumi = 36000;
if(yearforlumi.Contains("16d")) lumi = 44307;
if(yearforlumi.Contains("16e")) lumi = 58450;
if(yearforlumi.Contains("all")) lumi = 139000;

if (myConfig.lumi>-1){ 
  lumi = myConfig.lumi;
  cout << "INFO: Luminosity provided by user input! Lumi set to " << myConfig.lumi << endl;
}

if (0==lumi){ 
  cout << "ERROR: Luminosity was not provided! Aborting..." << endl;
  abort();
}

cout << "Running on " << sample << ", " << year << endl;
bool isData = ("data"==sample);
cout << "isData is " << isData << endl;

bool isNominal = false;
TString tmp = myConfig.inputTextFileName;
tmp.ToLower();
bool isSignal = false;  
isSignal = tmp.Contains("signal");
cout << "isSignal is " << isSignal << endl;
 TFile *outputfile= TFile::Open(myConfig.outputDirName,"RECREATE");
gROOT->cd();

TString datadirpath = getenv("DATAPATH");
TObjArray* token = datadirpath.Tokenize(":");
TString datadir = ((TObjString*)token->At(0))->GetString();
//TString CBKFileName = datadir + "/HaaSkimming/ttbarTheorySys.txt";
TString CBKFileName = myConfig.systTxtFileName;
std::cout << "CBKFileName: " << CBKFileName << std::endl;
map<int,double> SOW;


map<TString, TheoryWeightSyst> CBKMap;

if(CBKFileName!=""){
TChain *CutBookKeepers = new TChain("CutBookkeepers");
  for (auto inputFile : myConfig.inputFiles){
   TFile *f = TFile::Open(inputFile);
   if( !f->Get("CutBookkeepers")){
      std::cout << "WARNING: could not find tree CutBookkeepers in file " << inputFile << std::endl;
      std::cout << "---> ignoring, check file especially if you are running on MC" << std::endl;
      continue;
    }
    f->Close();
    CutBookKeepers->Add(inputFile);
  }
readCBKFile(CBKFileName, CBKMap);
getSOWSyst(CutBookKeepers, CBKMap);
}


for (auto treeName : myConfig.treeNames){
  cout << "Running on tree " << treeName << endl;
  TString outputTreeName;
  TString systname = treeName;
  systname.Remove(0,4);
   
  //temp fix to run skim code over minitrees..only for nominal
  //std::size_t pos = treeName.Last('_');      
  //TString str1 = treeName;
  //TString str2 = str1.Remove(0,pos+1);    
  //std::cout <<  str2 << std::endl;
  //treeName = "tree_NoSys"; 
  
  if ("tree_NoSys"==treeName) {
    //isNominal = true;
    //outputTreeName = sample + "__Nominal";
    isNominal = (myConfig.sysSamplePrefix=="");
    outputTreeName = sample + "__" + myConfig.sysSamplePrefix + "Nominal";    
  }	
  else{
    //outputTreeName = sample + "_" + systname;
    outputTreeName = sample + "_" + myConfig.sysSamplePrefix + systname;
    isNominal=false;
  }
  cout << "isNominal is " << isNominal << endl;
  if (isData) outputTreeName = "data";
  TChain *chain = new TChain(treeName);

  for (auto inputFile : myConfig.inputFiles){
    // we have some empty files so do not add them to avoid warnings/errors later
    TFile *f = TFile::Open(inputFile);
    if( !f->Get(treeName) ){
      std::cout << "WARNING: could not find tree " << treeName << " in file " << inputFile << std::endl;
      std::cout << "---> ignoring, check file especially if you are running on MC" << std::endl;
      continue;
    }
    f->Close();
    chain->Add(inputFile);

  }
  if (SOW.size()==0) SOW = getSOW(chain,isData);
  map<int, TH1*> histmap;
  map<int, TH1*> histmap1;
  if ("tree_NoSys"==treeName) cutFlow(chain, SOW, histmap, histmap1, isData,(myConfig.sysSamplePrefix=="") ? "" : ("_" + myConfig.sysSamplePrefix));
  
  cout << "outputTreeName is " << outputTreeName << " and outputDirName is " << myConfig.outputDirName << endl;
  //gSystem->Exec("mkdir -p " + myConfig.outputDirName + "/" + year + "/" + sample);
  coutTime();

  processOutput(chain, outputfile, outputTreeName, isData, isNominal, myConfig, SOW, histmap, histmap1, isSignal, lumi, CBKMap);
  coutTime();
  delete chain;
  cout << "Done running tree " << treeName << endl;
}
outputfile->Close();
cout << "Job finished" << endl;
return 0;
}


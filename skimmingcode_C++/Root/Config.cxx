#include "HaaSkimming/Config.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

#include <TString.h>

// default config
Config::Config() {

  inputTextFileName = "";
  inputFiles.clear();
  outputDirName="Haa_output";

  treeNames = {"tree_NoSys"};

  maxEvents=-1;
  nStart=-1; 
  lumi=-1;
  year = "dummyyear";
  sample = "dummysample";
  isAFII=false;
  systTxtFileName="";
  createRadDown=false;
  createRadUp=false;
  sysSamplePrefix="";

}

void usage(std::string ss){
  std::cout << "Usage: " << ss << " [inputFileList or inputFiles] [outputDirName] [Option1=Value1] [Option2=Value2] ... " << std::endl;
}

// parse command line argument
int parseArgs(Config &config, int argc, char **argv) {

  int status = 0;

  std::cout << " parseArgs ";
  for (int i = 0; i < argc; ++i) {
    std::cout << argv[i] << ", ";
  }
  std::cout << std::endl;

  if(argc<2){
    usage(argv[0]);
    return 1;
  }

  TString input(argv[1]);

  if(input.Contains("=")){
    usage(argv[0]);
    return 1;
  }

  if(input.Contains(".txt")){
    config.inputTextFileName = input;
    status = readInputsFromTextFile(config, input);
    if(status != 0) return status;
  }
  else{
    status = readFromCSList(config.inputFiles, input);
    if(status != 0) return status;
  }


  for(int iarg=2; iarg<argc; iarg++) {
      
    TString token(argv[iarg]);

    if(!token.Contains("=")){
      config.outputDirName=token;
      continue;
    }

    TString parName;
    TString parValue;
    Ssiz_t parPos = 0;
    bool isGood = false;
    if(token.Tokenize(parName, parPos, "=")) {
      if(token.Tokenize(parValue, parPos, "=")) {
	isGood = true;
      }
    }
    if(!isGood) {
      std::cout << "Error in " << __FILE__ << ":" << __LINE__ << ": "
		<< "parsing parameter: " << token << std::endl;
      return 1;
    }



    int ok = setParam(config, parName, parValue);

    if(ok!=0){
      std::cout << "Error in " << __FILE__ << ":" << __LINE__ << ": "
		<< "unknown parameter " << parName << std::endl;
            abort();
    }

  }

  return status;
}

int readFromCSList(std::vector<TString> &items, TString cslist) {

  TString item;
  Ssiz_t pos = 0;
  items.clear();
  while(cslist.Tokenize(item, pos, ",")) {
    items.push_back(item);
  }

  return 0;
}

int readInputsFromTextFile(Config &config, TString flist) {

  std::ifstream ifile(flist.Data(), std::ios::in);
  if(!ifile) {
    std::cout << "Error in " << __FILE__ << ":" << __LINE__ << ": "
	      << "Unable to open file " << flist << std::endl;
    return 1;
  }

  config.inputFiles.clear();
  while(!ifile.eof()) {
    TString fname;
    ifile >> fname;
    if(!ifile.bad() && fname.Length() > 0) {
      config.inputFiles.push_back(fname);
    }
  }

  return 0;
}

std::vector<TString> getTreeNames(TString ss){

  TString item;
  Ssiz_t pos = 0;
  std::vector<TString> trees;
  while(ss.Tokenize(item, pos, ",")) {
    trees.push_back(item);
  }

  return trees;

}

int setParam(Config &config, TString parName, TString parValue){

  if(parName == "treeNames") {
    config.treeNames = getTreeNames(parValue);
    return 0;
  }
    
  if(parName == "maxEvents") {
    config.maxEvents = parValue.Atoi();
    return 0;
  }

  if(parName == "nStart") {
    config.nStart = parValue.Atoi();
    return 0;
  }

  if(parName == "lumi") {
    config.lumi = parValue.Atof();
    return 0;
  }

  if(parName == "year") {
    config.year = parValue;
    return 0;
  }

  if(parName == "sample") {
    config.sample = parValue;
    return 0;
  }



  if(parName == "isAFII") {
    if(parValue=="1" || parValue=="true" || parValue=="True"){
      config.isAFII = true;
    }
    else if(parValue=="0" || parValue=="false" || parValue=="False"){
      config.isAFII = false;
    }
    else{
      std::cout << "Error reading param isAFII value " << parValue << std::endl;
      std::cout << "Supported values are True,true,1/False,false,0 " << parValue << std::endl;
      return 1;
    }
        return 0;
  }
  if(parName == "systTxtFileName"){
    config.systTxtFileName = parValue;
    return 0;
  }
  if(parName == "createRadDown") {
    if(parValue=="1" || parValue=="true" || parValue=="True"){
      config.createRadDown = true;
    }
    else if(parValue=="0" || parValue=="false" || parValue=="False"){
      config.createRadDown = false;
    }
    else{
      std::cout << "Error reading param createRadDown value " << parValue << std::endl;
      std::cout << "Supported values are True,true,1/False,false,0 " << parValue << std::endl;
      return 1;
    }
    return 0;
  }
  if(parName == "createRadUp") {
    if(parValue=="1" || parValue=="true" || parValue=="True"){
      config.createRadUp = true;
    }
    else if(parValue=="0" || parValue=="false" || parValue=="False"){
      config.createRadUp = false;
    }
    else{
      std::cout << "Error reading param createRadUp value " << parValue << std::endl;
      std::cout << "Supported values are True,true,1/False,false,0 " << parValue << std::endl;
      return 1;
    }
    return 0;
  }


   if(parName == "sysSamplePrefix"){
    config.sysSamplePrefix = parValue;
    return 0;
  }


  
  return -1;
  
}

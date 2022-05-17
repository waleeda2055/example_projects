////////////////////////////////////
//// class Config
//// description: holds configuration such as input/output files, events to process, etc...
//// author: Remi Zaidan
/////////////////////////////////////


#ifndef CONFIG_H
#define CONFIG_H

#include <TString.h>
#include <string.h>
#include <vector>
#include <map>

struct Config {
  Config();
  // Make it a singleton
  static Config& getInstance()
  {
    static Config theInstance;
    return theInstance;
  }
  TString inputTextFileName;
  std::vector<TString> inputFiles;
  TString outputFileName;
  TString outputDirName;
  std::vector<TString> treeNames;

  int maxEvents;
  int nStart; 
  double lumi;

  bool isAFII;
  TString year;
  TString sample;
  TString systTxtFileName;
  bool createRadDown;
  bool createRadUp; 
  TString sysSamplePrefix;
};

int parseArgs(Config &config, int argc, char **argv);
int readFromCSList(std::vector<TString> &items, TString cslist);
int readInputsFromTextFile(Config &config, TString flist);
std::vector<TString> getTreeNames(TString);
int parseConfigFile(Config &config, TString filename);
int setParam(Config &config, TString parName, TString parValue);

#endif

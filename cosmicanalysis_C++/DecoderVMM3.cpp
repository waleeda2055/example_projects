//********************************************************************
// Converts the binary mini-DAQ software binary file into a ROOT file
// that contains the high level root TTree. This software decoded the
// binary data, applies the channel mapping, assemble data into events,
// fills the TTree and adds the pedestal values (not yet implemented).
//
// 3 possible modes of operation:
//     1. Manual eLink mapping
//     2. Database
//     3. Raw decoding (no channel mapping)
//
// If the DB and user arguments are in conflict, the later as priority.
//
// Date of first release:   2018-06-19
// Major update:            2018-09-15
// Authors:                 Waleed Ahmed (waleeda@physics.mcgill.ca)
//                          Benoit Lefebvre (lefebben@physics.mcgill.ca)
//********************************************************************

// C++ libs
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>

// ROOT includes
#include <TString.h>

// My includes
#include "Tools.h"
#include "RunInfo.h"
#include "HitData.h"
#include "VMM3FillTreeManager.h"
#include "FEBType.h"
#include "DBManagerVMM3.h"

using namespace std;



void help() {
  cout<<endl;
  cout<<"Standard run settings:" <<endl;
  cout<<"./DecoderVMM3 [--run-name runname] [--detector-type detectortype] [--mapping elinkmap] [-o outputfile.root] [inputfile.bin]"<<endl;
  cout<<endl;
  cout<<"[--run-name runname]: Unique string that differentiates input runs"<<endl;
  cout<<"[--detector-type detectortype]: sTGC detector module type; e.g. QS3P"<<endl;
  cout<<"[--mapping elinkmap]: e-link-FEB-layer mapping in the format elink:p/sFEB:layer"<<endl;
  cout<<"[-o outputfile.root]: Output decoded ROOT file name"<<endl;
  cout<<"[inputfile.bin]: Input raw cosmics data binary file name"<<endl;
  cout<<endl;
  cout<<"Additional options:"<<endl;
  cout<<"[--DB qc-database.db]: Save data into database"<<endl;
  cout<<"[--verbose n]: Possible verbose levels are 0 and 1"<<endl;
  cout<<"[--max-events max]: Specify maximum number of events to decode, otherwise decode all"<<endl;
  cout<<"[--make-plots]: Save raw plots to outputfile.root"<<endl;
  cout<<"[--print-plots]: Save raw plots into separate PDF file"<<endl;
  cout<<"[--plot-out-file outputpdf.pdf]: Output PDF file name"<<endl;
  cout<<"[--detector-name name]: Name of detector for database entry; e.g. QS3P6"<<endl;
  cout<<"[--disable-event-builder]: Disables event building"<<endl;
  cout<<endl;
  cout<<"Example:"<<endl;
  cout<<"./DecoderVMM3 --run-name mcgill_qs3p6 --detector-type QS3P ";
  cout<<"--mapping 0:pFEB:1,2:pFEB:3,3:pFEB:2,5:sFEB:2,4:sFEB:3,6:sFEB:1,7:sFEB:4,1:pFEB:4 ";
  cout<<"-o root/decoded.root data/run_raw.bin"<<endl;
  cout<<endl;
}
map<Int_t, FEBLocation> ParseFEBMapping(const string mapStr);
void PrintMapping(map<Int_t, FEBLocation> mapping);


Int_t main(Int_t argc, char **argv){

  //================
  // Parse arguments
  string inputBinary, outputROOT, outputPDF;
  string runname, detectortype, detectorname;
  string dbFileName;
  string mapStr;
  Int_t  verbose = 0;
  Int_t  maxEvents = -1;
  bool makePlots=false, printPlots=false, makeEvents=true;

  stringstream m_sx;
  
  if(argc<2){
    help();
    return 1;
  }

  Int_t pos=1;
  while(pos<argc){
    if(strcmp(argv[pos], "--run-name")==0){
      runname = string(argv[pos+1]);
      pos+=2;
    }
    else if(strcmp(argv[pos], "--detector-type")==0){
      detectortype = string(argv[pos+1]);
      pos+=2;
    }
    else if(strcmp(argv[pos], "--detector-name")==0){
      detectorname = string(argv[pos+1]);
      pos+=2;
    }
    else if(strcmp(argv[pos], "--mapping")==0){
      mapStr = string(argv[pos+1]);
      pos+=2;
    }
    else if(strcmp(argv[pos], "--DB")==0){
      dbFileName = string(argv[pos+1]);
      pos+=2;
    }
    else if(strcmp(argv[pos], "--verbose")==0){
      verbose = atoi(argv[pos+1]);
      if(verbose<0 || verbose>1){
	help();
	return 1;
      }
      pos+=2;
    }
    else if(strcmp(argv[pos], "--max-events")==0){
      maxEvents = atoi(argv[pos+1]);
      pos+=2;
    }
    else if(strcmp(argv[pos], "--make-plots")==0){
      makePlots=true;
      pos++;
    }
    else if(strcmp(argv[pos], "--print-plots")==0){
      printPlots=true;
      makePlots=true;
      pos++;
    }
    else if(strcmp(argv[pos], "--disable-event-builder")==0){
      makeEvents=false;
      pos++;
    }
    else if(strcmp(argv[pos], "--plot-out-file")==0){
      outputPDF = string(argv[pos+1]);
      pos+=2;
    }
    else if(strcmp(argv[pos], "-o")==0){
      outputROOT = string(argv[pos+1]);
      pos+=2;
    }
    else{
      inputBinary = string(argv[pos]);
      pos++;
    }
  }

  
  cout<<"**** VMM3 decoder ****"<<endl;
  
  //==========================================
  // Get mapping from argument (if applicable)
  map<Int_t, FEBLocation> eLinkMap;
  if(!mapStr.empty())
    eLinkMap = ParseFEBMapping(mapStr);

  
  //=======================================
  // Get info from database (if applicable)
  if(!dbFileName.empty()){
    if(runname.empty()){
      cout<<"ERROR: Run name is a mandatory argument when using a DB."<<endl;
      return 1;
    }

    map<Int_t, FEBLocation> eLinkMapDB;
    string detectornameDB, detectortypeDB;
    
    DBManagerVMM3 db(dbFileName);
    if(!db.IsValid()){
      cout<<"ERROR: The specified database is not valid."<<endl;
      return 1;
    }
    
    if(!db.RunExists(runname)){
      cout<<"ERROR: Cannot find run '"<<runname<<"' in DB."<<endl;
      return 1;
    }
    
    detectornameDB = db.GetDetectorName(runname);
    detectortypeDB = db.GetDetectorType(runname);
    eLinkMapDB = db.GetFEBMap(runname);
    
    

    if(detectorname.empty()){
      detectorname=detectornameDB;
    }
    else if(detectorname!=detectornameDB){
      cout<<"WARNING: Argument and DB don't match for detector name. ";
      cout<<"Arguments have precedence."<<endl;
    }
    
    if(detectortype.empty()){
      detectortype=detectortypeDB;
    }
    else if(detectortype!=detectortypeDB){
      cout<<"WARNING: Argument and DB don't match for detector type. ";
      cout<<"Arguments have precedence."<<endl;
    }
    
    
    if(eLinkMap.empty()){
      eLinkMap = eLinkMapDB;
    }
    else if(eLinkMap != eLinkMapDB){
      cout<<"WARNING: Argument and DB don't match for eLink map. ";
      cout<<"Arguments have precedence."<<endl;
    }
  }
  
  
  //=========================
  // Process output files name
  // The basename is not changed regardless of the run number
  string basename = (Tools::split(inputBinary, '/')).back(); // get file name
  basename = basename.substr(0, basename.size()-4); // remove .bin
  if(outputROOT == ""){
    outputROOT = "root/"+basename+".root";
  }
  if(outputPDF == ""){
    outputPDF = "pdf/"+basename+".pdf";
  }


  
  //=====================
  // Summary of arguments
  cout<<"Input:\t\t"<<inputBinary<<endl;
  cout<<"Output:\t\t"<<outputROOT<<endl;
  cout<<"Run:\t\t"<<runname<<endl;
  cout<<"Det. type:\t"<<detectortype<<endl;
  cout<<"Det. name:\t"<<detectorname<<endl;
  cout<<"Verbose:\t"<<verbose<<endl;
  if(maxEvents>0)
    cout<<"Max evts:\t"<<maxEvents<<endl;
  else
    cout<<"No maximum number of events."<<endl;
  if(makePlots)
    cout<<"Will save raw plots to ROOT file."<<endl;
  if(printPlots)
    cout<<"Will print plots to file: '"<<outputPDF<<"'"<<endl;  
  if(makeEvents){
    cout<<"Will assemble events."<<endl;
  }
  else{
    cout<<"Will not assemble events."<<endl;
  }

  if(eLinkMap.empty()){
    cout<<"Will not apply channel mapping (raw decoding)."<<endl;
  }
  else{
    cout<<"Mapping:"<<endl;
    PrintMapping(eLinkMap);
  }


  //====================
  // Make RunInfo object
  RunInfo *info      = new RunInfo(TString("runinfo"), TString(runname));
  info->detectorname = TString(detectorname);
  info->detectortype = TString(detectortype);
  

  //===============
  // Assemble TTree
  VMM3FillTreeManager decoder(inputBinary, outputROOT,
			      outputPDF, info, eLinkMap);
  decoder.SetMaxEvents(maxEvents);
  decoder.SetVerbose(verbose);
  decoder.makePlots = makePlots;
  decoder.makeEvents = makeEvents;
  
  decoder.InitOutput();
  decoder.Run();
  if(printPlots)
    decoder.PrintPlots();
  decoder.End();

  cout<<"**** Goodbye! ****"<<endl;
  return 0;
}

map<Int_t, FEBLocation> ParseFEBMapping(const string mapStr){
  map<Int_t, FEBLocation> mapping;
  auto febList = Tools::split(mapStr, ',');
  if(febList.empty()) throw runtime_error("Bad eLink mapping format.");

  for(const auto feb : febList){
    auto items = Tools::split(feb, ':');
    if(items.size()!=3) throw runtime_error("Bad eLink mapping format.");
    Int_t eLink = atoi(items.at(0).c_str());
    FEBType t;
    if(items.at(1)=="pFEB")
      t=fPFEB;
    else if(items.at(1)=="sFEB")
      t=fSFEB;
    else throw runtime_error("Bad eLink mapping format.");
    UShort_t l = atoi(items.at(2).c_str());

    mapping[eLink] = {t, l};
  }
  
  return mapping;
}


void PrintMapping(map<Int_t, FEBLocation> mapping){
  if(mapping.empty()){
    cout<<"(EMPTY)"<<endl;
    return;
  }
      
  for(auto it=mapping.begin(); it!=mapping.end(); it++){
    cout<<"eLink "<<it->first<<" -> ("<<it->second.type
	<<", L"<<it->second.layer<<")"<<endl;
  }

}

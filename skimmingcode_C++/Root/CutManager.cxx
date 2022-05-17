#include "HaaSkimming/CutManager.h"
#include <utility>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

CutManager::CutManager(string name){
  m_name=name;
  this->clear();
  m_importance=20;
  m_debugImportance=0;
  m_eventWeight=0;
}

CutManager::~CutManager() {
   this->clear();
}

void CutManager::clear() {
  _counters.clear();
  _order.clear();
}

void CutManager::print() const {
  std::cout<<"CutManager has "<<_counters.size()<<" counters defined."<<std::endl;
}

void CutManager::list(){
  this->orderCounters();
  for(unsigned int ic=0; ic<_order.size(); ++ic) {
    std::cout << m_name<< "-Counter:" << std::setw(3) << ic
	      << " : "  << std::setw(60) << std::setfill('.') << std::left << _order[ic] << " : "
	      << std::setfill(' ') << std::left
	      << " T=" << std::setw(9) << std::setprecision(9) << _counters.find(_order[ic])->second.count
	      << " TW=" << std::setw(9) << std::setprecision(9) << _counters.find(_order[ic])->second.weighted_count
	      << " E=" << std::setw(9) << std::setprecision(9) << _counters.find(_order[ic])->second.event_count
	      << " EW=" << std::setw(9) << std::setprecision(9) << _counters.find(_order[ic])->second.event_weighted_count;
    std::cout << std::endl;
  }
}

//void CutManager::list(){
//  this->orderCounters();
//  for(unsigned int ic=0; ic<_order.size(); ++ic) {
//    std::cout << "cutNames.push_back(\""
//	      << _order[ic]
//              << "\");";
//    std::cout << std::endl;
//  }
//}

void CutManager::dumpToFile(std::string fname) {
  std::ofstream ofile(fname.c_str(),std::ios::out);
  if(!ofile){
    std::cout<<"CutManager::DumpToFile ERROR opening "<<fname<<std::endl;
    return;
  }
  for(unsigned int ic=0; ic<_order.size(); ++ic) {
        ofile << "Counter " << std::setw(3) << ic
	      << " : "  << std::setw(100) << std::setfill('.') << std::left << _order[ic] << " : "
	      << " T=" << std::setw(11) << std::setprecision(10) << _counters.find(_order[ic])->second.count;
	ofile << std::endl;
  }
}


void CutManager::count(std::string key, char importance) {
  if(!checkImportance(importance))return;
  ocount(key,0);
}

void CutManager::ocount(std::string key, int order, char importance) {
  if(!checkImportance(importance))return;

  std::map<std::string,CutManagerUtil::ValInfo>::iterator c;
  c = _counters.find(key);
  if(c != _counters.end()) {
    c->second.count++;
    c->second.order = order;
    c->second.weighted_count+=(m_eventWeight)?*m_eventWeight:1.;
    if(c->second.new_event){
      c->second.event_count++;
      c->second.event_weighted_count+=(m_eventWeight)?*m_eventWeight:1.;
      c->second.new_event=false;
    }
  } 
  else {
    CutManagerUtil::ValInfo vals;
    vals.count=1;
    vals.order= order;
    vals.weighted_count=(m_eventWeight)?*m_eventWeight:1.;
    vals.event_count=1;
    vals.event_weighted_count=(m_eventWeight)?*m_eventWeight:1.;
    vals.new_event=false;
    _counters.insert(std::make_pair(key,vals));
    _order.push_back(key);
  }
  if(importance<m_debugImportance) {
    std::cout  <<  "DBG Counter(-"<<importance<<"-) " <<  key  <<  " : "
	       << " T=" << _counters.find(key)->second.count
	       <<  std::endl;
  }
}


void CutManager::count(std::string key, double val, char importance) {
  if(!checkImportance(importance))return;

  std::ostringstream sval;
  sval << val;
  std::string nkey = key + sval.str();
  this->ocount(nkey, 0);
}


void CutManager::ocount(std::string key, double val, int order, char importance) {
  if(!checkImportance(importance))return;

  std::ostringstream sval;
  sval << val;
  std::string nkey = key + sval.str();
  this->ocount(nkey, order);
}

void CutManager::flagNewEvent(){

  std::map<std::string,CutManagerUtil::ValInfo>::iterator c = _counters.begin();

  for(; c!=_counters.end();++c){
    c->second.new_event=true;
  }

}


double CutManager::getCount(std::string key) const {

  double cnt = 0;
  std::map<std::string,CutManagerUtil::ValInfo>::const_iterator c;
  c = _counters.find(key);
  if(c != _counters.end()) cnt = c->second.count;
  return cnt;
}

double CutManager::getCount(UInt_t ic) {

  double cnt = 0;
  this -> orderCounters();
  if (ic < _order.size()) {
    cnt = _counters.find(_order[ic])->second.count;
  }
  return cnt;
}

string CutManager::getOrder(UInt_t ic) {
  return _order[ic];
}

double CutManager::getWeightCount(std::string key) const {

  double cnt = 0;
  std::map<std::string,CutManagerUtil::ValInfo>::const_iterator c;
  c = _counters.find(key);
  if(c != _counters.end()) cnt = c->second.weighted_count;
  return cnt;
}


void CutManager::orderCounters() {
  for(unsigned int ic=0; ic<_order.size(); ++ic){
    bool pass = true;
    std::string hold = "";
    for(unsigned int jc=0; jc<_order.size()-1; ++jc){
      if(_counters.find(_order[jc])->second.order > _counters.find(_order[jc+1])->second.order){
	hold = _order[jc];
	_order[jc] = _order[jc+1];
	_order[jc+1] = hold;	  
	pass = false;
      }
    }
    if(pass) return;
  }
}

TTree* CutManager::dumpToTree(std::string name){


  TTree* tree = new TTree(name.c_str(),"Cut Manager mirror tree");

  // char thekey[1000];
  //TString* thekey;
  vector<char>* thekey = new vector<char>();
  double thecount;
  double theweighted_count;
  int theorder;
  double theevent_count;
  double theeventweighted_count;

  //tree->Branch("Key", thekey, "Key[1000]/B");
  tree->Branch("Key", "vector<char>", &thekey);
  tree->Branch("Count", &thecount, "Count/D");
  tree->Branch("WeightedCount", &theweighted_count, "WeightedCount/D");
  tree->Branch("EventCount", &theevent_count, "EventCount/D");
  tree->Branch("WeightedEventCount", &theeventweighted_count, "WeightedEventCount/D");
  tree->Branch("Order", &theorder, "Order/I");

  for(unsigned int ic=0; ic<_order.size(); ++ic) {
    thekey->clear();

    for(unsigned int is=0; is<_order[ic].size(); ++is){
      thekey->push_back( (_order[ic])[is] );
    }
    CutManagerUtil::ValInfo vals = _counters.find(_order[ic])->second;
    thecount = vals.count;
    theweighted_count = vals.weighted_count;
    theorder = vals.order;
    theevent_count = vals.event_count;
    theeventweighted_count = vals.event_weighted_count;

    tree->Fill();

  }

  return tree;

}


void CutManager::create(TTree* tree){


  //char thekey[1000];
  //TString* thekey;

  vector<char>* thekey=0;
  double thecount=0;
  double theweighted_count=0;
  double theevent_count=0;
  double theeventweighted_count=0;
  int theorder=0;

  tree->SetBranchAddress("Key", &thekey);
  tree->SetBranchAddress("Order", &theorder);
  tree->SetBranchAddress("Count", &thecount);
  tree->SetBranchAddress("WeightedCount", &theweighted_count);
  if(tree->FindBranch("EventCount")){
    tree->SetBranchAddress("EventCount", &theevent_count);
    tree->SetBranchAddress("WeightedEventCount", &theeventweighted_count);
  }

  Long64_t nentries = tree->GetEntries();


  for(Long64_t ie=0; ie<nentries; ++ie){

    tree->GetEntry(ie);

    std::map<std::string,CutManagerUtil::ValInfo>::iterator c;


    std::string key("");
     for(unsigned int is=0; is<thekey->size(); ++is){
      key.push_back( thekey->at(is) );
    }
    c = _counters.find(key);
    if(c != _counters.end()) {
      c->second.count+=thecount;
      c->second.weighted_count+=theweighted_count;
      c->second.event_count+=theevent_count;
      c->second.event_weighted_count+=theeventweighted_count;
      c->second.new_event=true;
      c->second.order=theorder;
    } 
    else {
      CutManagerUtil::ValInfo vals;
      vals.count=thecount;
      vals.weighted_count=theweighted_count;
      vals.order= theorder;
      vals.event_count=theevent_count;
      vals.event_weighted_count=theeventweighted_count;
      vals.new_event=true;
      _counters.insert(std::make_pair(key,vals));
      _order.push_back(key);

    }


  }

}

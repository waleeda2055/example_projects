
////////////////////////////////////
//// class CutManager
//// description: helper class to count the passage of the algorithm at a specific place in the code.
//// author: Laurent Vacavant
/////////////////////////////////////


#ifndef MYTOOLBOX_CUTMANAGER_H
#define MYTOOLBOX_CUTMANAGER_H

#include <map>
#include <vector>
#include <string>
#include <TTree.h>

using namespace std;

namespace CutManagerUtil{

  typedef struct{

    double count;
    double weighted_count;
    double event_count;
    double event_weighted_count;
    int order; 
    bool new_event;

  } ValInfo;

}


class CutManager {

  //private:


  std::map<std::string,CutManagerUtil::ValInfo>   _counters;
  std::vector<std::string>    _order;
  int m_importance;  
  int m_debugImportance;
  const double *m_eventWeight;
 
  void orderCounters();
  inline bool checkImportance(int a){return a<m_importance;}

  std::string m_name;

public:

  CutManager(string name);
  ~CutManager();

  void clear();
  void print() const;
  void list();
  void dumpToFile(std::string fname);
  TTree* dumpToTree(std::string name);
  void create(TTree* tree);
  void ocount(std::string key, int order, char importance=0);
  void ocount(std::string key, double val, int order, char importance=0);
  
  void count(std::string key, char importance=0);
  void count(std::string key, double val, char importance=0);
  
  double  getCount(std::string key) const;
  double  getCount(UInt_t i);
  string getOrder(UInt_t ic);
  double  getWeightCount(std::string key) const;
  inline void setImportance(int a, int b){ m_importance=a; m_debugImportance=b;}
  inline int counterSize(){return _counters.size();}
  void flagNewEvent();
  inline void setEventWeight(const double& a){ m_eventWeight=&a; }

};

#endif // MYTOOLBOX_CUTMANAGER_H

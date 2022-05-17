//********************************************************************
// Monte Carlo generator for sTGC Quality Control software.
//
// Date of creation:     March 2018
// Main developpers:     Waleed Ahmed (waleed.ahmed2@mail.mcgill.ca)
//                       Benoit Lefebvre (lefebben@physics.mcgill.ca)
//********************************************************************

//C++ libs
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

// ROOT includes
#include <TFile.h>
#include <TTree.h>
#include <TRandom.h>
#include <TRandom3.h>
#include <TGraphErrors.h>
#include <TRandom3.h>
#include <TF1.h>
#include <TF2.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TMath.h>
#include <TString.h>

// My includes
#include "HitData.h"
#include "SimulationInfo.h"
#include "RunInfo.h"
#include "Tools.h"
#include "MyTimer.h"
#include "DetectorGeometryTools.h"
#include "AmplificationSimulation.h"
#include "DetectorDefects.h"

using namespace std;

Double_t randTheta(TRandom *r);
void PrintUsage(const string &usage);
void InitializeSimulation(PlotManager *pm, SimulationInfo &info,
			  DetectorGeometry *g);
void RunSimulation(PlotManager *pm, SimulationInfo &info,
		   DetectorGeometry *g, TRandom *r);
void FinalizeSimulation(PlotManager *pm, SimulationInfo &info,
			DetectorGeometry *g);
RunInfo *SimulateRunInfo(const SimulationInfo &siminfo, DetectorGeometry *g,
			 TRandom *r);

Int_t main(Int_t argc, char **argv){

  
  //================
  // Parse arguments
  string usage(argv[0]);
  usage += " [--config config.xml]";
  usage += " [--seed N]";
  usage += " [--help]";
  usage += " [--allow-overwrite]";
  usage += " [-o output.root]";

  UInt_t pos=1;
  string configFileName, outFileName;
  Bool_t haveSeed=false, allowOverwrite=false;
  Int_t argseed=0;
  while(pos<argc){
    if(string(argv[pos])=="--config"){
      configFileName = string(argv[pos+1]);
      pos+=2;
    }
    else if(string(argv[pos])=="--seed"){
      haveSeed = true;
      argseed = atoi(argv[pos+1]);
      pos+=2;
    }
    else if(string(argv[pos])=="--help"){
      PrintUsage(usage);
      return 0;
    }
    else if(string(argv[pos])=="--allow-overwrite"){
      allowOverwrite=true;
      pos++;
    }
    else if(string(argv[pos])=="-o"){
      outFileName = string(argv[pos+1]);
      pos+=2;
    }
    else{
      PrintUsage(usage);
      return 1;
    }
  }


  cout<<"*** MCgen ***"<<endl;
  
  //==========================
  // Get simulation parameters
  // Default configuration in SimulationInfo.h
  cout<<"Getting configuration..."<<endl;
  SimulationInfo info(configFileName, "simulationinfo");

  if(info.tag==""){   // Must have a tag (and no static default tag exist)
    info.tag = "MC" + Tools::GetTimeStamp();
  }

  if(haveSeed)        // Seed in argument has priority over seed in config file
    info.seed = argseed;  
  
  //=================
  // Validate file I/O
  if(outFileName==""){
    outFileName = "root/"+info.tag+".decoded.root";
  }

  if(!allowOverwrite && Tools::fExists(outFileName)){
    cout<<"ERROR: Cannot overwrite output file: '"<<outFileName<<"'."<<endl;
    cout<<"Use --allow-overwrite flag to override."<<endl;
    return 1;
  }
  
  TFile *fOut = new TFile(outFileName.c_str(), "RECREATE");
  if(!fOut->IsOpen()){
    cout<<"ERROR: Problem opening output file."<<endl;
    return 1;
  }

  //=========================
  // Random number generator
  TRandom3 *r = new TRandom3(info.seed);  

  //=================
  // Initialize ROOT objects
  // All stored in a ROOT object container
  PlotManager *pm = new PlotManager();
  pm->Add(&info, mySimulationInfo);

  //======================
  // Get detector geometry
  DetectorGeometry *g =
    DetectorGeometryTools::GetDetectorGeometry(info.detectortype);
  
  //==============================
  // Summarize (file I/O + config)
  cout<<"\tOutput file:\t"<<outFileName<<endl;
  if(configFileName=="")  cout<<"\tConfig file:\t(NONE)"<<endl;
  else                    cout<<"\tConfig file:\t"<<configFileName<<endl;

  cout<<endl;
  cout<<"**** CONFIGURATION *****"<<endl;
  info.PrintContent(2);
  cout<<endl;
  
  //=================
  // Start simulation
  InitializeSimulation(pm, info, g);
  RunSimulation(pm, info, g, r);
  FinalizeSimulation(pm, info, g);

  //====================
  // Create fake RunInfo
  RunInfo *runinfo = SimulateRunInfo(info, g, r);
  TTree *tgcdata = static_cast<TTree*>(pm->Get("tgcdata"));
  tgcdata->GetUserInfo()->Add(runinfo);
  
  //==========================
  // Save ROOT objects to file
  Int_t mem = pm->Write(fOut);
  fOut->Close();
  delete fOut;

  //==================
  // End of simulation
  cout<<"\tROOT objects written to file '"<<outFileName<<"'."<<endl;
  cout<<"\t"<<mem<<" bytes written to disk."<<endl;
  cout<<endl;
  cout<<"*** Goodbye! ***"<<endl;
  return 0;
}


// Initialize simulation plots
// Does anything that should be done before simulation events
// Plot list:
// TH1F    phi_{raw,accepted}
// TH1F    theta_{raw,accepted}
// TH1F    slopeyz_{raw,accepted}
// TH1F    slopexz_{raw,accepted}
// TH2F    hitmap_hodoscope_{raw,accepted}
// TH2F    hitmap_tgc_L{1..4}_{raw,accepted}
// TH1F    ion_clusters
// TH1F    nelectrons_clusters
// TH1F    primary_ionization
// TH1F    gas_gain
// TH1F    total_ionization
// TH1F    charge
void InitializeSimulation(PlotManager *pm, SimulationInfo &info,
			  DetectorGeometry *g){

  cout<<"Initialize simulation..."<<endl;

  //================================
  // Setup simulated 'tgcdata' TTree
  TTree *tree = new TTree("tgcdata", "Tree for storing TGC data.");
  pm->Add(tree, myTTree);
  //  pm->Add(&info, mySimulationInfo);
  
  if(!info.makePlots)
    return;

  //=====================
  // Intialize histograms
  const vector<string> suffix = {"raw", "accepted"};
  const vector<string> title = {"All tracks", "Accepted tracks"};
  const Double_t PI = TMath::Pi();
  auto  layers = g->GetLayers();

  for(Int_t i=0; i<suffix.size(); i++){
    string s = suffix.at(i);
    string t = title.at(i);
    
    pm->Add("phi_"+s, t+";#phi [rad];Tracks", 300, 0, 2*PI, myTH1F);
    pm->Add("theta_"+s, t+";#theta [rad];Tracks", 300, -PI/2, PI/2, myTH1F);
    pm->Add("slopeyz_"+s, t+";#theta_{yz} [rad];Tracks", 300, -PI/2, PI/2, myTH1F);
    pm->Add("slopexz_"+s, t+";#theta_{xz} [rad];Tracks", 300, -PI/2, PI/2, myTH1F);
    pm->Add("hitmap_hodoscope_"+s,
	    t+" - Hodoscope hitmap;x position [mm];y position [mm]",
	    100, 0, info.wHod, 100, 0, info.dHod, myTH2F);

    for(const auto l : layers){
      string lStr = Tools::CStr(l);
      string tl = t + " - TGC hitmap - L"+lStr;
      auto limitsX = g->GetLimitsChannels({fWIRE, l});
      auto limitsY = g->GetLimitsChannels({fSTRIP, l});
      pm->Add("hitmap_tgc_L"+lStr+"_"+s, tl+";x position [mm];y position [mm]",
	      100, limitsX.first, limitsX.second,
	      100, limitsY.first, limitsY.second, myTH2F);
    } 
  } // End type loop

  // Gas amplification plots
  pm->Add("ion_clusters", ";Number of clusters;Tracks", 100, 0, 100, myTH1F);
  pm->Add("nelectrons_clusters", ";Number of electrons per cluster;Clusters", 100, 0, 100, myTH1F);
  pm->Add("primary_ionization", ";Primary ionization [electrons];Tracks", 100, 0, 500, myTH1F);
  pm->Add("gas_gain", ";Gas gain;Entries", 100, 0, 10*info.meanGain, myTH1F);
  pm->Add("total_ionization", ";Total ionization [electrons];Tracks", 100, 0, 1e3*info.meanGain, myTH1F);
  pm->Add("charge", ";Q [pC];Tracks",  100, 0, info.meanGain/1.602E4, myTH1F);  

}


// Simulate individual events
void RunSimulation(PlotManager *pm, SimulationInfo &info,
		   DetectorGeometry *g, TRandom *r){

  cout<<"Process events..."<<endl;
  
  //=============
  // Set-up TTree
  UInt_t eventnumber=0, nTracks=0, trigCounter=0;
  vector<RawHitData> hits;

  // Vectors actually saved in the TTree
  vector<ElectrodeType>  eTypeV;
  vector<UShort_t>       layerV;
  vector<UShort_t>       idV;
  vector<UInt_t>         pdoV;
  vector<UInt_t>         tdoV;
  vector<UInt_t>         bcidV;
  vector<Bool_t>         flagV;
  vector<UShort_t>       eLinkV;
  vector<UShort_t>       vmmIDV;
  vector<UShort_t>       vmmCHV;

  
  auto tgcdata = (TTree*) pm->Get("tgcdata");
  tgcdata->Branch("eventnumber", &eventnumber, "eventnumber/I");
  tgcdata->Branch("electrode_type", &eTypeV);
  tgcdata->Branch("layer", &layerV);
  tgcdata->Branch("id", &idV);
  tgcdata->Branch("pdo", &pdoV);
  tgcdata->Branch("tdo", &tdoV);
  tgcdata->Branch("bcid", &bcidV);
  tgcdata->Branch("flag", &flagV);
  tgcdata->Branch("elink", &eLinkV);
  tgcdata->Branch("vmmid", &vmmIDV);
  tgcdata->Branch("vmmch", &vmmCHV);

  //=================
  // Detector defects
  DetectorDefects dd(&info, g);

  //=================
  // Begin event loop
  auto layers  = g->GetLayers();
  auto sectors = g->GetSectors();
  MyTimer timer(info.nTrigger, "Simulate sTGC events");
  while(trigCounter < info.nTrigger){
    timer.DisplayProgressBar(trigCounter);
    
    //=========================
    // Prevent infinite runtime
    nTracks++;
    if(nTracks > info.event_timeout){
      //      throw runtime_error("Simulated too many events.");
      cout<<"WARNING: Simulated too many tracks. Simulation timeout."<<endl;
      break;
    }
      
    //=================================
    // Generate random cosmic-ray track
    const Double_t phi     = r->Uniform(0, 2*TMath::Pi());
    const Double_t theta   = randTheta(r);
    const Double_t slopeXZ = tan(theta)*cos(phi);
    const Double_t slopeYZ = tan(theta)*sin(phi);

    // Track offset in hodoscope frame
    const Double_t hodOfstX = r->Uniform(0, info.wHod);
    const Double_t hodOfstY = r->Uniform(0, info.dHod);

    // Track offset in TGC frame
    const Double_t tgcOfstX = hodOfstX - info.xTGC + slopeXZ*info.zTGC;
    const Double_t tgcOfstY = hodOfstY - info.yTGC + slopeYZ*info.zTGC; 

    // Get TGC position on layers
    map<UShort_t, Double_t> xTGCPos, yTGCPos;
    for(const auto l : layers){
      xTGCPos[l] = tgcOfstX + slopeXZ*g->GetZPosition(l);
      yTGCPos[l] = tgcOfstY + slopeYZ*g->GetZPosition(l);
    }
    
    // Fill raw histograms
    if(info.makePlots){
      pm->Fill("phi_raw", phi);
      pm->Fill("theta_raw", theta);
      pm->Fill("slopeyz_raw", atan(slopeXZ));
      pm->Fill("slopexz_raw", atan(slopeYZ));
      pm->Fill("hitmap_hodoscope_raw", hodOfstX, hodOfstY);
      for(const auto l : layers){  
	pm->Fill("hitmap_tgc_L"+Tools::CStr(l)+"_raw",
		 xTGCPos.at(l), yTGCPos.at(l));
      }
    }

    //===========================
    // Check if hodoscope trigger
    const Double_t lowPosHodX = hodOfstX + slopeXZ*info.hHod;
    const Double_t lowPosHodY = hodOfstY + slopeYZ*info.hHod;
    if(lowPosHodX<0 || lowPosHodX>info.wHod ||
       lowPosHodY<0 || lowPosHodY>info.dHod)
      continue;

    trigCounter++;
    
    //=============================
    // Gas amplification simulation
    map<UShort_t, AmplificationSimulation> gasSim;
    map<UShort_t, Double_t> Q;
    
    //==========================
    // Strip spatial resolution
    // varies with YZ angle
    const Double_t yPosRes
      = sqrt( pow(info.resTGC0,2) + pow(slopeYZ*info.resTGCAngle,2) );

    //==================
    // Generate TGC hits
    hits.clear();
    for(const auto s : sectors){
      UShort_t l = s.layer;    ElectrodeType t = s.electrode_type;

      // Check if hit at least one electrode
      if(!g->InFiducialArea(xTGCPos.at(l), yTGCPos.at(l), s))
	continue;

      //==================
      // Gas amplification
      // Same Q value for each layer
      if(gasSim.find(l)==gasSim.end()){
	gasSim[l] = AmplificationSimulation(info);
	gasSim.at(l).Simulate(theta, r, info.fullGasSimulation);
	Q[l] = gasSim.at(l).GetCharge(); // charge in picocoulombs
      }

      //================
      // Process defects
      if(!dd.Evaluate(xTGCPos[l], yTGCPos[l], s, r))
	continue;
      
      // Temporarily store pdo values per channel
      map<UShort_t, Double_t> pdo;
      
      //=======
      // STRIPS
      if(t==fSTRIP){

	// Smear position
	Double_t yPosStrip = r->Gaus(yTGCPos[l], yPosRes);
	Double_t sigma     = info.chargeClusterWidth;
	Double_t A         = 0.5*info.chargeConversion*Q.at(l);
	A /= (sigma*sqrt(2*TMath::Pi()));

	if(g->InFiducialArea(yPosStrip, s)){
	  auto limitsY =  g->GetLimitsChannels(s);

	  // Make 1D gaussian function
	  TF1 *fGaus = new TF1("fGaus", "gaus", limitsY.first, limitsY.second);
	  fGaus->SetParameter(0, A);
	  fGaus->SetParameter(1, yPosStrip);
	  fGaus->SetParameter(2, sigma);

	  // Find strip on the left of central strip then on the right
	  // Assumes first strip is good (it should be at this point)
	  UShort_t curStrip = g->GetID(yPosStrip, s);
	  Bool_t goLeft=true, goRight=false;
	  while(goLeft || goRight){

	    auto limitsStrip = g->PositionEdgesChannel({t,l,curStrip});
	    pdo[curStrip] = fGaus->Integral(limitsStrip.first, limitsStrip.second);

	    // If reached a neighbour (under threshold)
	    if(pdo[curStrip]<info.qDigitalThreshold){
	      // First strip MUST be over threshold
	      if(pdo.size()==1){
		pdo.clear();
		break;
	      }

	      if(goLeft){
		curStrip = pdo.rbegin()->first;
		goLeft=false;
		goRight=true;
	      }
	      else if(goRight){
		goRight=false;
	      }
	    }
	    
	    // Increment
	    if(goLeft){
	      curStrip--;
	      if(!g->ChannelExists({t,l,curStrip})){
		curStrip = pdo.rbegin()->first + 1;
		goLeft=false;
		goRight=true;
	      }
	    }
	    else if(goRight){
	      curStrip++;
	      if(!g->ChannelExists({t,l,curStrip})){
		goRight=false;
	      }
	    }
	    
	  } // End go left/right on cluster
	
	  delete fGaus;
	} // End over a strip
      } // End strip-type

      //=====
      // PADS
      // Integrate a 2D gaussian over the pads area
      // Each integrated bit of the gaussian is associated to the appropriate pad
      else if(t==fPAD){

	// Make 2D gaussian function
	Double_t sigma = info.chargeClusterWidth; // same width in x and y
	Double_t A = 0.5*Q.at(l)*info.chargeConversion/(2*TMath::Pi()*sigma*sigma);
	string fStr = "[0]*TMath::Exp(-0.5/[3]**2*((x-[1])**2+(y-[2])**2))";
	TF2 *fGaus = new TF2("fGaus", fStr.c_str(),
			     xTGCPos[l]-4*sigma, xTGCPos[l]+4*sigma,
			     yTGCPos[l]-4*sigma, yTGCPos[l]+4*sigma);
	fGaus->SetParameter(0, A);
	fGaus->SetParameter(1, xTGCPos[l]);
	fGaus->SetParameter(2, yTGCPos[l]);
	fGaus->SetParameter(3, sigma);
	  

	// Compute integral
	const Double_t n = 2;
	const Double_t ds = 3.*sigma/n;
	for(Int_t ix=-n; ix<n; ix++){
	  for(Int_t iy=-n; iy<n; iy++){
	    Double_t minX=xTGCPos[l]+ix*ds, maxX=minX+ds;
	    Double_t minY=yTGCPos[l]+iy*ds, maxY=minY+ds;
	    Double_t midX=0.5*(minX+maxX), midY=0.5*(minY+maxY);
	    if(g->InFiducialArea(midX, midY, s)){
	      UShort_t id = g->GetID(midX, midY, s);
	      if(pdo.find(id)==pdo.end()){
		pdo[id]=0;
	      }
	      pdo[id] += fGaus->Integral(minX, maxX, minY, maxY);
	    }
	  }
	}
	
	delete fGaus;
      } // End pad-type

      //======
      // WIRES
      // Share charge between two groups if on the edge
      else if(t==fWIRE){
	Double_t qWire = Q.at(l)*info.chargeConversion;
	UShort_t id = g->GetID(xTGCPos[l], s);
	auto limitsGroup = g->PositionEdgesChannel({t,l,id});
	const Double_t xHigh = xTGCPos[l] + 0.5*info.thicknessGap*slopeXZ;
	const Double_t xLow  = xTGCPos[l] - 0.5*info.thicknessGap*slopeXZ;
	const Double_t dx = xHigh-xLow;
	
	if(xLow<limitsGroup.first){
	  pdo[id] = qWire*fabs(xHigh-limitsGroup.first)/dx;
	  if(id>0 && g->ChannelExists({t,l,static_cast<UShort_t>(id-1)})){
	    pdo[id-1] = qWire*fabs(limitsGroup.first-xLow)/dx;
	  }
	}

	else if(xHigh>limitsGroup.second){
	  pdo[id] = qWire*fabs(limitsGroup.second-xLow)/dx;
	  if(g->ChannelExists({t,l,static_cast<UShort_t>(id+1)})){
	    pdo[id+1] = qWire*fabs(xHigh-limitsGroup.second)/dx;
	  }
	}
	else{
	  pdo[id] = qWire;
	}
      } // End wire-type

      //=================================
      // Final hit processing
      for(auto it=pdo.begin(); it!=pdo.end();){
	Bool_t erase=false;

	// Saturation
	if(it->second > info.qDigitalSaturation){
	  it->second = info.qDigitalSaturation;
	}

	// Threshold (strip readout has neighbour triggering)
	if(t!=fSTRIP && it->second<info.qDigitalThreshold){
	  erase=true;
	}

	// erase bad hits 
	if(erase)
	  it = pdo.erase(it);
	else
	  it++;
		  
      }
      
    
      // Copy sector data
      for(auto it : pdo){
	hits.push_back(RawHitData(Channel(t,l,it.first), 
				  static_cast<UInt_t>(it.second), 0, 0, true, 0, 0, 0));
      }
      
    } // End sectors loop (generate TGC hits)
  

    if(hits.size()==0)
      continue;

    //===========
    // Fill TTree
    eTypeV.clear();
    layerV.clear();
    idV.clear();
    pdoV.clear();
    tdoV.clear();
    bcidV.clear();
    flagV.clear();
    eLinkV.clear();
    vmmIDV.clear();
    vmmCHV.clear();

    for(const auto &hit : hits){
      eTypeV.push_back(hit.ch.electrode_type);
      layerV.push_back(hit.ch.layer);
      idV.push_back(hit.ch.electrode_ID);
      pdoV.push_back(hit.pdo);
      tdoV.push_back(hit.tdo);
      bcidV.push_back(hit.BCID);
      flagV.push_back(hit.neighbour_flag);
      eLinkV.push_back(hit.eLink);
      vmmIDV.push_back(hit.vmmID);
      vmmCHV.push_back(hit.vmmCH);
    }

    tgcdata->Fill();
    eventnumber++;

      
    //===========
    // Fill plots
    if(info.makePlots){

      // Tracking
      pm->Fill("phi_accepted", phi);
      pm->Fill("theta_accepted", theta);
      pm->Fill("slopeyz_accepted", atan(slopeXZ));
      pm->Fill("slopexz_accepted", atan(slopeYZ));
      pm->Fill("hitmap_hodoscope_accepted", hodOfstX, hodOfstY);
      for(const auto l : layers){  
	pm->Fill("hitmap_tgc_L"+Tools::CStr(l)+"_accepted",
		 xTGCPos.at(l), yTGCPos.at(l));
      }

      // Gas simulation
      for(const auto l : layers){  
	if(gasSim.find(l)!=gasSim.end()){
	  pm->Fill("ion_clusters", gasSim.at(l).GetNClusters());
	  for(Int_t i=0; i<gasSim.at(l).GetNClusters(); i++)
	    pm->Fill("nelectrons_clusters", (gasSim.at(l).GetClusterSize())[i]);
	  pm->Fill("primary_ionization", gasSim.at(l).GetPrimaryIonization());
	  for(Int_t i=0; i<gasSim.at(l).GetPrimaryIonization(); i++)
	    pm->Fill("gas_gain", (gasSim.at(l).GetGain())[i]);
	  pm->Fill("total_ionization", gasSim.at(l).GetTotalIonization());
	  pm->Fill("charge", gasSim.at(l).GetCharge());
	}
      }
    }
    
  } // end event loop

}

// Finalize plots
// Does anything that should be done after simulating events
void FinalizeSimulation(PlotManager *pm, SimulationInfo &info,
		   DetectorGeometry *g){

  cout<<"Finalize simulation..."<<endl;


  if(!info.makePlots)
    return;
  
  // Fit on histograms
  {
    TH1F *h = pm->GetTH1F("ion_clusters");
    Double_t min=h->GetBinLowEdge(0), max=min+h->GetBinWidth(0);
    TF1 *fPoisson = new TF1("fPoisson", "TMath::Poisson(x,[0])", min, max);
    fPoisson->SetParameter(0,info.nClustersMean);
    h->Fit("fPoisson", "R");
  }

  {
    TH1F *h = pm->GetTH1F("nelectrons_clusters");
    Double_t min=h->GetBinLowEdge(0), max=min+h->GetBinWidth(0);
    TF1 *fPol2 = new TF1("fPol2", "[0]/x**[1]", min, max);
    fPol2->SetParameter(0, h->GetEntries());
    fPol2->SetParameter(1, 2.);
    h->Fit("fPol2", "R");
  }

  {
    TH1F *h = pm->GetTH1F("gas_gain");
    Double_t min=h->GetBinLowEdge(0), max=min+h->GetBinWidth(0);
    string fStr = "[2]/[0]*[1]**[1]/TMath::Gamma([1])*(x/[0])**([1]-1)*TMath::Exp(-x*[1]/[0])";
    TF1 *fPolya = new TF1("fPolya", "[0]/x**[1]", min, max);
    fPolya->SetParameter(0, info.meanGain);
    fPolya->SetParameter(1, info.thetaGain);
    fPolya->SetParameter(2, h->GetEntries()/10);
    h->Fit("fPolya", "R");
  }

  pm->GetTH1F("charge")->Fit("landau");  
}


RunInfo *SimulateRunInfo(const SimulationInfo &siminfo, DetectorGeometry *g,
			 TRandom *r){

  cout<<"Generate 'RunInfo' object..."<<endl;
  RunInfo *runinfo = new RunInfo("runinfo", siminfo.tag);
  runinfo->detectortype = siminfo.detectortype;
  runinfo->detectorname = "mc-simulation";

  auto sectors = g->GetSectors();
  for(const auto s : sectors){
    Channel ch = g->GetFirstChannel(s);
    //    Channel lastCH = g->GetLastChannel(s);
    do{
	runinfo->pedestal[ch] = 0;
	runinfo->pedestal_error[ch] = 0;

	Int_t ID = ch.electrode_ID;
	ch = {ch.electrode_type, ch.layer, static_cast<UShort_t>(ID+1)};
    }while(g->ChannelExists(ch));
  }
  
  return runinfo;
}



// Generates a random number to taken from cos^2 distribution
Double_t randTheta(TRandom *r){ 
  Double_t theta;
  
  do{
    theta=r->Uniform(-TMath::Pi()/2, TMath::Pi()/2);
  }while( r->Uniform(0,1.5) > pow(cos(theta),2) );

  return theta;
}


void PrintUsage(const string &usage){
  cout<<usage<<endl;
  cout<<"Supported detector types: ";
  DetectorGeometryTools::PrintDetectorTypes();
}

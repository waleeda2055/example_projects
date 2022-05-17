"""
***********************************************************************************
 * Package: HistFitter                                                            *   
 * Project: HistFitter - A ROOT-based package for statistical data analysis       *                                                            
 *                                                                                *
 *                                                                                *
 * Description:                                                                   *
 *      Simple example configuration using a single bin and raw numbers           * 
 *                                                                                *
 * Authors:                                                                       *
 *      HistFitter group, CERN, Geneva                                            *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in the file          *
 * LICENSE.                                                                       *
 **********************************************************************************
"""

################################################################
## In principle all you have to setup is defined in this file ##
################################################################

## This configuration performs a trivial one-bin fit
## Only two systematics are considered:
##   -JES (Tree-based)
##   -Alpgen Kt scale (weight-based)
##

from configManager import configMgr
from ROOT import TColor,kBlack,kWhite,kGray,kRed,kPink,kMagenta,kViolet,kBlue,kAzure,kCyan,kTeal,kGreen,kSpring,kYellow,kOrange,kDashed,kSolid,kDotted
from configWriter import fitConfig,Measurement,Channel,Sample
from systematic import Systematic
from math import sqrt

from ROOT import gROOT, TLegend, TLegendEntry, TCanvas, TLatex
#gROOT.LoadMacro("./macros/AtlasStyle.C")
import ROOT
#ROOT.SetAtlasStyle()

#ROOT.TH1.AddDirectory(False) #add this line for memory management

#helper functions
def addWeight(oldList,newWeight):
    newList = deepcopy(oldList)
    newList.append(newWeight)
    return newList

def replaceWeight(oldList,oldWeight,newWeight):
    newList = deepcopy(oldList)
    newList[oldList.index(oldWeight)] = newWeight
    return newList

def appendTo(item, toList):
    for to in toList:
        to.append(item)
    return item

def SetupSamples(samples,systList):
    for sample in samples:
        for syst in systList:
            sample.addSystematic(syst)
    return


def getInterError(massPoint):
  if massPoint > 14 and massPoint < 31: return  0.014
  if massPoint > 30 and massPoint < 46: return  0.0114
  if massPoint > 45 and massPoint < 61: return  0.0066
  if massPoint > 60 and massPoint < 76: return  0.0086
  print "ERROR: Mass point ", massPoint ,"not supported!"
  return 2


#Flags
useStat=True
doValidation= True #False         
#doAllPlots = False 
doSystSmallBkg = True         
doSystDDZtt = False #True #flag to turn on systematics        
doReducedSyst = False #True #flag to run on reduced systematics
doPythia8 = False
doAllSRplots = False #True
haveHadd = False
addSRbinsAsVRs = True
doDefaultTTtheo = False #False = take smoothed syst, True = take from tree
doSmooth = True  

if not 'sigSamples' in dir():
    sigSamples=["H160a15"] 
    sigPointDisc="a15"   


sigPointDisc=sigSamples[0]


print "sigSamples",sigSamples

sigMasses = ["15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51","52","53","54","55","56","57","58","59","60","61","62","63","64","65","66","67","68","69","70","71","72","73","74","75"]


#-------------------------------
# Parameters for hypothesis test
#-------------------------------

if myFitType==FitType.Exclusion:
    configMgr.doExclusion= True # needs to be set when doing hypothesis test: True=exclusion, False=discovery
    configMgr.nTOYs=1500
    configMgr.scanRange = (0.01, 0.2) #for SR20,30,62
    configMgr.calculatorType=2 # 2=asymptotic calculator, 0=frequentist calculator(toys)
elif myFitType==FitType.Discovery:
    configMgr.doExclusion=False # needs to be set when doing hypothesis test: True=exclusion, False=discovery
    configMgr.nTOYs=1500
    configMgr.scanRange = (0., 15)
    configMgr.calculatorType=2 # 2=asymptotic calculator, 0=frequentist calculator(toys)

configMgr.testStatType=3
configMgr.nPoints=50


#-------------------------------------
# Now we start to build the data model
#-------------------------------------

# First define HistFactory attributes
configMgr.analysisName = "MyConfigHplus"
configMgr.histCacheFile = "data/"+configMgr.analysisName+".root"
configMgr.outputFileName = "results/"+configMgr.analysisName+"_Output.root"

# Scaling calculated by outputLumi / inputLumi
configMgr.inputLumi  = 1 # Luminosity of input TTree after weighting
configMgr.outputLumi = 1 # Luminosity of the data
configMgr.setLumiUnits("fb-1")
configMgr.blindSR= False       
#configMgr.useSignalInBlindedData=True 
configMgr.readFromTree = True


#inputDirPath = "/eos/home-w/waahmed/afs_runs/new_ptag/"
inputDirPath = "/eos/home-w/waahmed/afs_runs/conf_2021/"


# Sample definitions (colors, normalization nuisance-parameter, etc.)


# Top background
topSample = Sample("ttbar_",TColor.GetColor("#8bd3fb"))
topSample.setStatConfig(useStat)
topSample.setNormFactor("mu_Top",1.,0.,5.)
topSample.setNormRegions([("CRTop","cuts"),("CRZ","cuts"),("CRttZ","cuts")])

ttVSample = Sample("ttZ_",TColor.GetColor("#ff839b"))
ttVSample.setStatConfig(useStat)
ttVSample.setNormRegions([("CRTop","cuts"),("CRZ","cuts"),("CRttZ","cuts")])
ttVSample.setNormFactor("mu_ttZ",1.,0.,5.)

stSample = Sample("singleTop_",TColor.GetColor("#9559b5"))
stSample.setStatConfig(useStat)
stSample.setNormRegions([("CRTop","cuts"),("CRZ","cuts"),("CRttZ","cuts")])

dbSample = Sample("diBoson_", kGreen-10)
dbSample.setStatConfig(useStat)
dbSample.setNormRegions([("CRTop","cuts"),("CRZ","cuts"),("CRttZ","cuts")])
#dbSample.setNormFactor("mu_db",1.,0.,5.)

WjetsSample = Sample("Wjets_",TColor.GetColor("#785d9f"))
WjetsSample.setStatConfig(useStat)
WjetsSample.setNormRegions([("CRTop","cuts"),("CRZ","cuts"),("CRttZ","cuts")])

ttWSample = Sample("ttW_",kYellow+1)
ttWSample.setStatConfig(useStat)
ttWSample.setNormRegions([("CRTop","cuts"),("CRZ","cuts"),("CRttZ","cuts")])

# Z background
#zSample = Sample("Zjets_",kAzure+1)
zSample = Sample("Zjets_",TColor.GetColor("#ffae32"))
zSample.setStatConfig(useStat)
zSample.setNormFactor("mu_Z",1.,0.,5.)
zSample.setNormRegions([("CRTop","cuts"),("CRZ","cuts"),("CRttZ","cuts")])

#Z + HF study
#zSampleC = Sample("ZjetsC_",kCyan)
#zSampleC.setStatConfig(useStat)
#zSampleC.setNormFactor("mu_Z",1.,0.,5.)
#zSampleC.setNormRegions([("CRTop","cuts"),("CRZ","cuts"),("CRttZ","cuts")])

zSampleO = Sample("ZjetsO_",TColor.GetColor("#357478"))  
zSampleO.setStatConfig(useStat)
zSampleO.setNormRegions([("CRTop","cuts"),("CRZ","cuts"),("CRttZ","cuts")])


#smoothzjets
if doSmooth: 
 
 nZjet = [0.757652,0.762217,0.767083,0.77227,0.777799,0.783693,0.789975,0.796672,0.80381,0.81142,0.819531,0.828177,0.837394,0.847218,0.857691,0.868854,0.880753,0.893437,0.906958,0.921371,0.936734,0.953111,0.970567,0.989176,1.00901,1.03015,1.05269,1.07672,1.10233,1.12963,1.15873,1.18974,1.22281,1.25805,1.29562,1.33567,1.37836,1.42387,1.47237,1.52408,1.57919,1.63795,1.70057,1.76733,1.83849,1.91434,1.9952,2.08139,2.17327,2.2712,2.37559,2.48687,2.60549,2.73194,2.86672,3.01039,3.16354,3.32679,3.50081,3.68631] # jul13 CplusExp 

 nZjetError = [0.33677,0.331178,0.325514,0.319799,0.314058,0.308319,0.302618,0.296992,0.291486,0.28615,0.281041,0.27622,0.271756,0.267722,0.264193,0.26125,0.258971,0.257433,0.256707,0.256854,0.257925,0.259953,0.262956,0.266929,0.271851,0.27768,0.284356,0.291805,0.299937,0.308651,0.317839,0.327385,0.33717,0.347073,0.356973,0.366755,0.376312,0.385552,0.394404,0.402826,0.410824,0.418463,0.425893,0.433377,0.441326,0.450334,0.461225,0.475077,0.493239,0.517307,0.549067,0.590392,0.643142,0.709078,0.789822,0.886879,1.00168,1.13567,1.29032,1.46724] #jul13 CplusExp
 
 nZjetO = [ .33, .33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33,.33] 
   
 nZjetOerror = [.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1]


 i=0
 for Nexp in nZjet:
    zSample.buildHisto([Nexp],"SR%s"%sigMasses[i],"cuts",0.5)
    i += 1

 j=0 
 for Nerror in nZjetError:
    zSample.buildStatErrors( [Nerror] ,"SR%s"%sigMasses[j] ,"cuts" )
    j += 1  
 k=0
 for Nexp in nZjetO:
    zSampleO.buildHisto([Nexp],"SR%s"%sigMasses[k],"cuts",0.5)
    k += 1
 l=0
 for Nerror in nZjetOerror:
    zSampleO.buildStatErrors( [Nerror] ,"SR%s"%sigMasses[l] ,"cuts" )
    l += 1
 
 #1 GeV binning
 #zSample.buildHisto([0.757652,0.762217,0.767083,0.77227,0.777799,0.783693,0.789975,0.796672,0.80381,0.81142,0.819531,0.828177,0.837394,0.847218,0.857691,0.868854,0.880753,0.893437,0.906958,0.921371,0.936734,0.953111,0.970567,0.989176,1.00901,1.03015,1.05269,1.07672,1.10233,1.12963,1.15873,1.18974,1.22281,1.25805,1.29562,1.33567,1.37836,1.42387,1.47237,1.52408,1.57919,1.63795,1.70057,1.76733,1.83849,1.91434,1.9952,2.08139,2.17327,2.2712,2.37559,2.48687,2.60549,2.73194,2.86672,3.01039,3.16354,3.32679,3.50081,3.68631],"SR","dimuon_m",1)
 
 # 3GeV binning 
 zSample.buildHisto([0.757652,0.77227,0.789975,0.81142,0.837394,0.868854,0.906958,0.953111,1.00901,1.07672,1.15873,1.25805,1.37836,1.52408,1.70057,1.91434,2.17327,2.60549,3.16354,3.68631],"SR","dimuon_m",15,3)
 zSampleO.buildHisto([0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33],"SR","dimuon_m",15,3)

 #4GeV binning
 #zSample.buildHisto([0.777799,0.80381,0.837394,0.880753, 0.936734,1.00901,1.10233,1.22281,1.37836,1.57919,1.83849,2.17327,2.60549,3.16354,3.68631],"SR","dimuon_m",15,4)
 #zSampleO.buildHisto([0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33],"SR","dimuon_m",15,4)
 




# Data sample
dataSample = Sample("data",kBlack)
dataSample.setData()

#Dummy data for testing
#ndataBlind = [10,20,20,25,45] #N events data in SR for 2,2,2,4,4 GeV windows --> unblinded
#i=0
#for Nexp in ndataBlind:
#    dataSample.buildHisto([Nexp],"SR%s"%sigMasses[i],"cuts",0.5)
#    i += 1

date = "2020/sept23_sys"
date2= "2020/nov13"
#date3= "2020/nov9"
#date = "2020/jun24"
datettW = "2020/dec10"
dateZHF = "2020/jun23"
datettbar = "2020/jun25"
dateDphi = "2020/jun26_dPhiplots"
dataDphi = "2020/jun26_dPhiplots"


if configMgr.readFromTree: 
      
        #cleanup version 
        #16a
        dataSample.addInput("%smc16a/data/tree_NoSys.root" % (inputDirPath), "data")
        stSample.addInput("%smc16a/singleTop/tree_NoSys.root" % (inputDirPath), "singleTop_")
        ttVSample.addInput("%smc16a/ttZ/ttZ_16a.root" % (inputDirPath), "ttZ_")
        dbSample.addInput("%smc16a/diBoson/tree_NoSys.root" % (inputDirPath), "diBoson_")
        WjetsSample.addInput("%smc16a/Wjets/tree_NoSys.root" % (inputDirPath), "Wjets_")
        ttWSample.addInput("%smc16a/ttW/tree_NoSys.root" % (inputDirPath), "ttW_")
        zSample.addInput("%smc16a/Zjets_HF/Zjets_BplusC_16a.root" % (inputDirPath), "Zjets_")
        zSampleO.addInput("%smc16a/Zjets_split/zjets_other_16a.root" % (inputDirPath), "Zjets_")      
        topSample.addInput("%smc16a/ttbar/ttbar_elecsys_16a.root" % (inputDirPath), "ttbar_")
       
        #16d
        dataSample.addInput("%smc16d/data/tree_NoSys.root" % (inputDirPath), "data")
        stSample.addInput("%smc16d/singleTop/tree_NoSys.root" % (inputDirPath), "singleTop_")
        ttVSample.addInput("%smc16d/ttZ/ttZ_16d.root" % (inputDirPath), "ttZ_")
        dbSample.addInput("%smc16d/diBoson/tree_NoSys.root" % (inputDirPath), "diBoson_")
        WjetsSample.addInput("%smc16d/Wjets/tree_NoSys.root" % (inputDirPath), "Wjets_")
        ttWSample.addInput("%smc16d/ttW/tree_NoSys.root" % (inputDirPath), "ttW_")
        zSample.addInput("%smc16d/Zjets_HF/Zjets_BplusC_16d.root" % (inputDirPath), "Zjets_")
        zSampleO.addInput("%smc16d/Zjets_split/zjets_other_16d.root" % (inputDirPath), "Zjets_")
        topSample.addInput("%smc16d/ttbar/ttbar_elecsys_16d.root" % (inputDirPath), "ttbar_")
    
        #16e
        dataSample.addInput("%smc16e/data/tree_NoSys.root" % (inputDirPath), "data")
        stSample.addInput("%smc16e/singleTop/tree_NoSys.root" % (inputDirPath), "singleTop_")
        ttVSample.addInput("%smc16e/ttZ/ttZ_16e.root" % (inputDirPath), "ttZ_")
        dbSample.addInput("%smc16e/diBoson/tree_NoSys.root" % (inputDirPath), "diBoson_")
        WjetsSample.addInput("%smc16e/Wjets/tree_NoSys.root" % (inputDirPath), "Wjets_")
        ttWSample.addInput("%smc16e/ttW/tree_NoSys.root" % (inputDirPath), "ttW_")
        zSample.addInput("%smc16e/Zjets_HF/Zjets_BplusC_16e.root" % (inputDirPath), "Zjets_")
        zSampleO.addInput("%smc16e/Zjets_split/zjets_other_16e.root" % (inputDirPath), "Zjets_")
        topSample.addInput("%smc16e/ttbar/ttbar_elecsys_16e.root" % (inputDirPath), "ttbar_")

        
        #dPhiplots
   
        """        
        topSample.addInput("%sbkg/%s/mc16a/ttbar/tree_NoSys.root" % (inputDirPath,dateDphi), "ttbar_")
        #topSample.addInput("%sbkg/%s/ttbar_16a_dilep.root" % (inputDirPath,date3), "ttbar_")
        #zSample.addInput("%sbkg/%s/mc16a/Zjets/zjets_16a.root" % (inputDirPath,date), "Zjets_")
        #zSample.addInput("%sbkg/%s/mc16a/Zjets/tree_NoSys.root" % (inputDirPath,date), "Zjets_")
        dataSample.addInput("%sbkg/%s/mc16a/data/tree_NoSys.root" % (inputDirPath,dataDphi), "data")
        #dataSample.addInput("%smc16a/data/tree_NoSys.root" % (inputDirPath2), "data")
        stSample.addInput("%sbkg/%s/mc16a/singleTop/tree_NoSys.root" % (inputDirPath,dateDphi), "singleTop_")
        #ttVSample.addInput("%sbkg/%s/mc16a/ttV/ttV_16a.root" % (inputDirPath,date), "ttV_")
        ttVSample.addInput("%sbkg/%s/mc16a/ttZ/tree_NoSys.root" % (inputDirPath,dateDphi), "ttZ_")
        dbSample.addInput("%sbkg/%s/mc16a/diBoson/tree_NoSys.root" % (inputDirPath,dateDphi), "diBoson_")
        WjetsSample.addInput("%sbkg/%s/mc16a/Wjets/tree_NoSys.root" % (inputDirPath,dateDphi), "Wjets_")
        ttWSample.addInput("%sbkg/%s/mc16a/ttW/tree_NoSys.root" % (inputDirPath,dateDphi), "ttW_")
        #Z+ HF study
        zSample.addInput("%sbkg/%s/mc16a/Zjets/tree_NoSys.root" % (inputDirPath,dateDphi), "Zjets_")
        #zSample.addInput("%sbkg/2020/jun16/zjets_Bfilter_16a.root" % (inputDirPath), "Zjets_")
        #zSampleC.addInput("%sbkg/2020/jun16/zjets_Cfilter_16a.root" % (inputDirPath), "Zjets_")
        #zSampleO.addInput("%sbkg/2020/jun16/zjets_other_16a.root" % (inputDirPath), "Zjets_")

        topSample.addInput("%sbkg/%s/mc16d/ttbar/tree_NoSys.root" % (inputDirPath,dateDphi), "ttbar_")
        #topSample.addInput("%sbkg/%s/ttbar_16d_dilep.root" % (inputDirPath,date3), "ttbar_")
        #zSample.addInput("%sbkg/%s/mc16d/Zjets/zjets_16d.root" % (inputDirPath,date), "Zjets_")
        #zSample.addInput("%sbkg/%s/mc16d/Zjets/tree_NoSys.root" % (inputDirPath,date), "Zjets_")
        dataSample.addInput("%sbkg/%s/mc16d/data/tree_NoSys.root" % (inputDirPath,dataDphi), "data")
        #dataSample.addInput("%smc16d/data/tree_NoSys.root" % (inputDirPath2), "data")
        stSample.addInput("%sbkg/%s/mc16d/singleTop/tree_NoSys.root" % (inputDirPath,dateDphi), "singleTop_")
        #ttVSample.addInput("%sbkg/%s/mc16d/ttV/ttV_16d.root" % (inputDirPath,date), "ttV_")
        ttVSample.addInput("%sbkg/%s/mc16d/ttZ/tree_NoSys.root" % (inputDirPath,dateDphi), "ttZ_")
        dbSample.addInput("%sbkg/%s/mc16d/diBoson/tree_NoSys.root" % (inputDirPath,dateDphi), "diBoson_")
        WjetsSample.addInput("%sbkg/%s/mc16d/Wjets/tree_NoSys.root" % (inputDirPath,dateDphi), "Wjets_")
        ttWSample.addInput("%sbkg/%s/mc16d/ttW/tree_NoSys.root" % (inputDirPath,dateDphi), "ttW_")
        #Z+ HF study
        zSample.addInput("%sbkg/%s/mc16d/Zjets/tree_NoSys.root" % (inputDirPath,dateDphi), "Zjets_")
        #zSample.addInput("%sbkg/2020/jun16/zjets_Bfilter_16d.root" % (inputDirPath), "Zjets_")
        #zSampleC.addInput("%sbkg/2020/jun16/zjets_Cfilter_16d.root" % (inputDirPath), "Zjets_")
        #zSampleO.addInput("%sbkg/2020/jun16/tree_NoSys.root" % (inputDirPath), "Zjets_")
   
        topSample.addInput("%sbkg/%s/mc16e/ttbar/tree_NoSys.root" % (inputDirPath,dateDphi), "ttbar_")
        #topSample.addInput("%sbkg/%s/ttbar_16e_dilep.root" % (inputDirPath,date3), "ttbar_")
        #zSample.addInput("%sbkg/%s/mc16e/Zjets/zjets_16e.root" % (inputDirPath,date), "Zjets_")
        #zSample.addInput("%sbkg/%s/mc16e/Zjets/tree_NoSys.root" % (inputDirPath,date), "Zjets_")
        dataSample.addInput("%sbkg/%s/mc16e/data/tree_NoSys.root" % (inputDirPath,dataDphi), "data")
        #dataSample.addInput("%smc16e/data/tree_NoSys.root" % (inputDirPath2), "data")
        stSample.addInput("%sbkg/%s/mc16e/singleTop/tree_NoSys.root" % (inputDirPath,dateDphi), "singleTop_")
        #ttVSample.addInput("%sbkg/%s/mc16e/ttV/ttV_16e.root" % (inputDirPath,date), "ttV_")
        ttVSample.addInput("%sbkg/%s/mc16e/ttZ/tree_NoSys.root" % (inputDirPath,dateDphi), "ttZ_")
        dbSample.addInput("%sbkg/%s/mc16e/diBoson/tree_NoSys.root" % (inputDirPath,dateDphi), "diBoson_")
        WjetsSample.addInput("%sbkg/%s/mc16e/Wjets/tree_NoSys.root" % (inputDirPath,dateDphi), "Wjets_")
        ttWSample.addInput("%sbkg/%s/mc16e/ttW/tree_NoSys.root" % (inputDirPath,dateDphi), "ttW_")
        #Z+ HF study
        zSample.addInput("%sbkg/%s/mc16e/Zjets/tree_NoSys.root" % (inputDirPath,dateDphi), "Zjets_")
        #zSample.addInput("%sbkg/2020/jun16/zjets_Bfilter_16e.root" % (inputDirPath), "Zjets_")
        #zSampleC.addInput("%sbkg/2020/jun16/zjets_Cfilter_16e.root" % (inputDirPath), "Zjets_")
        #zSampleO.addInput("%sbkg/2020/jun16/zjets_other_16e.root" % (inputDirPath), "Zjets_")
        """

     
        
  

#Dictionnary of cuts for Tree->hist
#configMgr.cutsDict["SR"] = "((lep1Pt < 20 && lep2Pt<10 && met>250 && mt>100 && jet1Pt>130 && jet2Pt>25 && AnalysisType==7) || (lep1Pt < 25 && lep2Pt<10 && met>250 && mt>100 && jet1Pt>130 && jet2Pt>25 && AnalysisType==6))&& met/meff2Jet>0.5"

#CR
configMgr.cutsDict["CRZ"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100 ) && dimuon_m >= 78 && dimuon_m <= 102 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt <= 20))"

configMgr.cutsDict["CRttZ"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100  ) && dimuon_m >= 78 && dimuon_m <= 102 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20))"
configMgr.cutsDict["CRTop"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100  ) && dimuon_m >= 12 && dimuon_m <=77 && isOS ==0 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20))"

#VR

configMgr.cutsDict["VRZ"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100 ) && dimuon_m >= 78 && dimuon_m <= 102 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=0 && elec1Pt <= 20))"

configMgr.cutsDict["VR_Mmu1e"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss >-100) && Mmu1e_os >= 30 && Mmu1e_os <= 110 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20 ))"

configMgr.cutsDict["VR3"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss >-100) && Mmu1e >= 30 && Mmu1e <= 110 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20 ))"
configMgr.cutsDict["VR4"] = "((Mmu2e >= 30 && Mmu2e <= 110 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20 ))"

configMgr.cutsDict["VReSS"] = "((dimuon_m >= 12 && dimuon_m <=77 && isOS ==0 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt <= 20))"
configMgr.cutsDict["VReOS"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss >-100) && dimuon_m >= 12 && dimuon_m <=77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt <= 20))"


#SR
configMgr.cutsDict["SR"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100  ) && dimuon_m >= 12  && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20 ))"
#configMgr.cutsDict["SR"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100  ) && dimuon_m >= 12  && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20 && event_number!= 10396069 && event_number!= 16148384 ))"


configMgr.cutsDict["SR15"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 14.25 && dimuon_m <= 15.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20))"

configMgr.cutsDict["SR16"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 15.25 && dimuon_m <= 16.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20))"
configMgr.cutsDict["SR17"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 16.25 && dimuon_m <= 17.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20))"
configMgr.cutsDict["SR18"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 17.25 && dimuon_m <= 18.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

#configMgr.cutsDict["SR18"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 17.25 && dimuon_m <= 18.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20 && event_number!= 10396069 ))"

configMgr.cutsDict["SR19"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 18.25 && dimuon_m <= 19.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20))"

#configMgr.cutsDict["SR19"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 18.25 && dimuon_m <= 19.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20 && event_number!= 10396069 ))"

configMgr.cutsDict["SR20"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 19.25 && dimuon_m <= 20.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20))"

#configMgr.cutsDict["SR20"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 19.25 && dimuon_m <= 20.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20 && event_number!= 10396069))"


configMgr.cutsDict["SR21"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) &&  dimuon_m >= 20.25 && dimuon_m <= 21.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

#configMgr.cutsDict["SR21"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) &&  dimuon_m >= 20.25 && dimuon_m <= 21.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20 && event_number!= 10396069))"

configMgr.cutsDict["SR22"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) &&  dimuon_m >= 21.25 && dimuon_m <= 22.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR23"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 22.25 && dimuon_m <= 23.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR24"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 23.25 && dimuon_m <= 24.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR25"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 24.25 && dimuon_m <= 25.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR26"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 25.25 && dimuon_m <= 26.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR27"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 26.25 && dimuon_m <= 27.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR28"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 27.25 && dimuon_m <= 28.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR29"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 28.25 && dimuon_m <= 29.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR30"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 29.25 && dimuon_m <= 30.75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20))"
configMgr.cutsDict["SR31"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 30 && dimuon_m <= 32 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR32"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 31 && dimuon_m <= 33 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR33"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 32 && dimuon_m <= 34 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR34"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 33 && dimuon_m <= 35 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR35"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) &&  dimuon_m >=34 && dimuon_m <= 36 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20 ))"
configMgr.cutsDict["SR36"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) &&  dimuon_m >=35 && dimuon_m <= 37 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20 ))"
configMgr.cutsDict["SR37"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 36 && dimuon_m <= 38 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR38"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 37 && dimuon_m <= 39 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR39"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 38 && dimuon_m <= 40 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR40"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 39 && dimuon_m <= 41 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR41"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 40 && dimuon_m <= 42 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR42"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 41 && dimuon_m <= 43 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR43"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 42 && dimuon_m <= 44 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR44"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 43 && dimuon_m <= 45 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"
configMgr.cutsDict["SR45"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 44 && dimuon_m <= 46 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR46"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 44.5 && dimuon_m <= 47.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR47"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 45.5 && dimuon_m <= 48.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR48"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 46.5 && dimuon_m <= 49.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR49"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 47.5 && dimuon_m <= 50.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR50"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 48.5 && dimuon_m <= 51.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR51"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 49.5 && dimuon_m <= 52.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR52"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 50.5 && dimuon_m <= 53.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR53"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 51.5 && dimuon_m <= 54.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR54"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 52.5 && dimuon_m <= 55.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR55"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 53.5 && dimuon_m <= 56.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR56"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 54.5 && dimuon_m <= 57.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR57"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 55.5 && dimuon_m <= 58.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR58"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 56.5 && dimuon_m <= 59.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

#configMgr.cutsDict["SR59"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 57.5 && dimuon_m <= 60.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20 && event_number!= 16148384 ))"

configMgr.cutsDict["SR59"] = "((  (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 57.5 && dimuon_m <= 60.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20 ))"

configMgr.cutsDict["SR60"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 58.5 && dimuon_m <= 61.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >=20))"
#configMgr.cutsDict["SR60"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 58.5 && dimuon_m <= 61.5 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >=20 && event_number!= 16148384 ))"

configMgr.cutsDict["SR61"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 59 && dimuon_m <= 63 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >=20))"

configMgr.cutsDict["SR62"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 60 && dimuon_m <= 64 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >=20  ))"

configMgr.cutsDict["SR63"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 61 && dimuon_m <= 65 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >=20 ))"

configMgr.cutsDict["SR64"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 62 && dimuon_m <= 66 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR65"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 63 && dimuon_m <= 67 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR66"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 64 && dimuon_m <= 68 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR67"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 65 && dimuon_m <= 69 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR68"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 66 && dimuon_m <= 70 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR69"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 67 && dimuon_m <= 71 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR70"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) &&  dimuon_m >= 68 && dimuon_m <= 72 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR71"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) &&  dimuon_m >= 69 && dimuon_m <= 73 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR72"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) &&  dimuon_m >= 70 && dimuon_m <= 74 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR73"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) &&  dimuon_m >= 71 && dimuon_m <= 75 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR74"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) &&  dimuon_m >= 72 && dimuon_m <= 76 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"

configMgr.cutsDict["SR75"] = "(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100) && dimuon_m >= 73 && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >= 1 && elec1Pt >= 20))"



# Tuples of nominal weights without and with b-jet selection
#configMgr.weights = ("genWeight","eventWeight","leptonWeight","triggerWeight","truthWptWeight","bTagWeight2Jet")

# Nominal event weights
#nomWeights = ["event_weight*(event_number!=38395487 && mc_channel_number != 364158)"]  
nomWeights = ["event_weight"]
#configMgr.weights = ["1.","1."]
configMgr.weights = nomWeights

    

#--------------------
# List of systematics
#--------------------


#JES (tree-based)
#jes = Systematic("JES","_NoSys","_JESup","_JESdown","tree","overallSys")
#configMgr.nomName = "_NoSys"
configMgr.nomName = "_Nominal"



##BTag syst, BTagSysCalo vector contains syst weights relative to nominal, i.e. just need to multiply the nom weight by it
leptonWeight_MUON_EFF_BADMUON_STAT__1down = nomWeights + ["leptonWeight_MUON_EFF_BADMUON_STAT__1down"]
leptonWeight_MUON_EFF_BADMUON_STAT__1up = nomWeights + ["leptonWeight_MUON_EFF_BADMUON_STAT__1up"]
leptonWeight_MUON_EFF_BADMUON_SYS__1down = nomWeights + ["leptonWeight_MUON_EFF_BADMUON_SYS__1down"]
leptonWeight_MUON_EFF_BADMUON_SYS__1up = nomWeights + ["leptonWeight_MUON_EFF_BADMUON_SYS__1up"]
leptonWeight_MUON_EFF_ISO_STAT__1down = nomWeights + ["leptonWeight_MUON_EFF_ISO_STAT__1down"]
leptonWeight_MUON_EFF_ISO_STAT__1up = nomWeights + ["leptonWeight_MUON_EFF_ISO_STAT__1up"]
leptonWeight_MUON_EFF_ISO_SYS__1down = nomWeights + ["leptonWeight_MUON_EFF_ISO_SYS__1down"]
leptonWeight_MUON_EFF_ISO_SYS__1up = nomWeights + ["leptonWeight_MUON_EFF_ISO_SYS__1up"]
leptonWeight_MUON_EFF_RECO_STAT__1down = nomWeights + ["leptonWeight_MUON_EFF_RECO_STAT__1down"]
leptonWeight_MUON_EFF_RECO_STAT__1up = nomWeights + ["leptonWeight_MUON_EFF_RECO_STAT__1up"]
leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1down = nomWeights + ["leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1down"]
leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1up = nomWeights + ["leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1up"]
leptonWeight_MUON_EFF_RECO_SYS__1down = nomWeights + ["leptonWeight_MUON_EFF_RECO_SYS__1down"]
leptonWeight_MUON_EFF_RECO_SYS__1up = nomWeights + ["leptonWeight_MUON_EFF_RECO_SYS__1up"]
leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1down = nomWeights + ["leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1down"]
leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1up = nomWeights + ["leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1up"]
leptonWeight_MUON_EFF_TTVA_STAT__1down = nomWeights + ["leptonWeight_MUON_EFF_TTVA_STAT__1down"]
leptonWeight_MUON_EFF_TTVA_STAT__1up = nomWeights + ["leptonWeight_MUON_EFF_TTVA_STAT__1up"]
leptonWeight_MUON_EFF_TTVA_SYS__1down = nomWeights + ["leptonWeight_MUON_EFF_TTVA_SYS__1down"]
leptonWeight_MUON_EFF_TTVA_SYS__1up = nomWeights + ["leptonWeight_MUON_EFF_TTVA_SYS__1up"]
bTagWeight_FT_EFF_B_systematics__1down = nomWeights + ["bTagWeight_FT_EFF_B_systematics__1down"]
bTagWeight_FT_EFF_B_systematics__1up = nomWeights + ["bTagWeight_FT_EFF_B_systematics__1up"]
bTagWeight_FT_EFF_C_systematics__1down = nomWeights + ["bTagWeight_FT_EFF_C_systematics__1down"]
bTagWeight_FT_EFF_C_systematics__1up = nomWeights + ["bTagWeight_FT_EFF_C_systematics__1up"]
bTagWeight_FT_EFF_Light_systematics__1down = nomWeights + ["bTagWeight_FT_EFF_Light_systematics__1down"]
bTagWeight_FT_EFF_Light_systematics__1up = nomWeights + ["bTagWeight_FT_EFF_Light_systematics__1up"]
bTagWeight_FT_EFF_extrapolation__1down = nomWeights + ["bTagWeight_FT_EFF_extrapolation__1down"]
bTagWeight_FT_EFF_extrapolation__1up = nomWeights + ["bTagWeight_FT_EFF_extrapolation__1up"]
bTagWeight_FT_EFF_extrapolation_from_charm__1down = nomWeights + ["bTagWeight_FT_EFF_extrapolation_from_charm__1down"]
bTagWeight_FT_EFF_extrapolation_from_charm__1up = nomWeights + ["bTagWeight_FT_EFF_extrapolation_from_charm__1up"]
jvtWeight_JET_JvtEfficiency__1down = nomWeights + ["jvtWeight_JET_JvtEfficiency__1down"]
jvtWeight_JET_JvtEfficiency__1up = nomWeights + ["jvtWeight_JET_JvtEfficiency__1up"]
pileupWeightUp = nomWeights + ["pileupWeightUp"]
pileupWeightDown = nomWeights + ["pileupWeightDown"]
SF_SLORDLMUON_EFF_TrigStatUncertainty__1down = nomWeights + ["SF_SLORDLMUON_EFF_TrigStatUncertainty__1down"]
SF_SLORDLMUON_EFF_TrigStatUncertainty__1up = nomWeights + ["SF_SLORDLMUON_EFF_TrigStatUncertainty__1up"]
SF_SLORDLMUON_EFF_TrigSystUncertainty__1down = nomWeights + ["SF_SLORDLMUON_EFF_TrigSystUncertainty__1down"]
SF_SLORDLMUON_EFF_TrigSystUncertainty__1up = nomWeights + ["SF_SLORDLMUON_EFF_TrigSystUncertainty__1up"]

leptonWeight_EL_EFF_ID_TOTAL_1NPCOR_PLUS_UNCOR__1down = nomWeights + ["leptonWeight_EL_EFF_ID_TOTAL_1NPCOR_PLUS_UNCOR__1down"]
leptonWeight_EL_EFF_ID_TOTAL_1NPCOR_PLUS_UNCOR__1up = nomWeights + ["leptonWeight_EL_EFF_ID_TOTAL_1NPCOR_PLUS_UNCOR__1up"]
leptonWeight_EL_EFF_Iso_TOTAL_1NPCOR_PLUS_UNCOR__1down = nomWeights + ["leptonWeight_EL_EFF_Iso_TOTAL_1NPCOR_PLUS_UNCOR__1down"]
leptonWeight_EL_EFF_Iso_TOTAL_1NPCOR_PLUS_UNCOR__1up = nomWeights + ["leptonWeight_EL_EFF_Iso_TOTAL_1NPCOR_PLUS_UNCOR__1up"]
leptonWeight_EL_EFF_Reco_TOTAL_1NPCOR_PLUS_UNCOR__1down = nomWeights + ["leptonWeight_EL_EFF_Reco_TOTAL_1NPCOR_PLUS_UNCOR__1down"]
leptonWeight_EL_EFF_Reco_TOTAL_1NPCOR_PLUS_UNCOR__1up = nomWeights + ["leptonWeight_EL_EFF_Reco_TOTAL_1NPCOR_PLUS_UNCOR__1up"]

# PDF weights
LHE3Weight_PDFset90900 = nomWeights + ["LHE3Weight_PDFset90900"]
LHE3Weight_PDFset90901 = nomWeights + ["LHE3Weight_PDFset90901"]
LHE3Weight_PDFset90902 = nomWeights + ["LHE3Weight_PDFset90902"]
LHE3Weight_PDFset90903 = nomWeights + ["LHE3Weight_PDFset90903"]
LHE3Weight_PDFset90904 = nomWeights + ["LHE3Weight_PDFset90904"]
LHE3Weight_PDFset90905 = nomWeights + ["LHE3Weight_PDFset90905"]
LHE3Weight_PDFset90906 = nomWeights + ["LHE3Weight_PDFset90906"]
LHE3Weight_PDFset90907 = nomWeights + ["LHE3Weight_PDFset90907"]
LHE3Weight_PDFset90908 = nomWeights + ["LHE3Weight_PDFset90908"]
LHE3Weight_PDFset90909 = nomWeights + ["LHE3Weight_PDFset90909"]
LHE3Weight_PDFset90910 = nomWeights + ["LHE3Weight_PDFset90910"]
LHE3Weight_PDFset90911 = nomWeights + ["LHE3Weight_PDFset90911"]
LHE3Weight_PDFset90912 = nomWeights + ["LHE3Weight_PDFset90912"]
LHE3Weight_PDFset90913 = nomWeights + ["LHE3Weight_PDFset90913"]
LHE3Weight_PDFset90914 = nomWeights + ["LHE3Weight_PDFset90914"]
LHE3Weight_PDFset90915 = nomWeights + ["LHE3Weight_PDFset90915"]
LHE3Weight_PDFset90916 = nomWeights + ["LHE3Weight_PDFset90916"]
LHE3Weight_PDFset90917 = nomWeights + ["LHE3Weight_PDFset90917"]
LHE3Weight_PDFset90918 = nomWeights + ["LHE3Weight_PDFset90918"]
LHE3Weight_PDFset90919 = nomWeights + ["LHE3Weight_PDFset90919"]
LHE3Weight_PDFset90920 = nomWeights + ["LHE3Weight_PDFset90920"]
LHE3Weight_PDFset90921 = nomWeights + ["LHE3Weight_PDFset90921"]
LHE3Weight_PDFset90922 = nomWeights + ["LHE3Weight_PDFset90922"]
LHE3Weight_PDFset90923 = nomWeights + ["LHE3Weight_PDFset90923"]
LHE3Weight_PDFset90924 = nomWeights + ["LHE3Weight_PDFset90924"]
LHE3Weight_PDFset90925 = nomWeights + ["LHE3Weight_PDFset90925"]
LHE3Weight_PDFset90926 = nomWeights + ["LHE3Weight_PDFset90926"]
LHE3Weight_PDFset90927 = nomWeights + ["LHE3Weight_PDFset90927"]
LHE3Weight_PDFset90928 = nomWeights + ["LHE3Weight_PDFset90928"]
LHE3Weight_PDFset90929 = nomWeights + ["LHE3Weight_PDFset90929"]
LHE3Weight_PDFset90930 = nomWeights + ["LHE3Weight_PDFset90930"]


#####weight systematics
leptonWeight_MUON_EFF_BADMUON_STAT = Systematic("leptonWeight_MUON_EFF_BADMUON_STAT",configMgr.weights, leptonWeight_MUON_EFF_BADMUON_STAT__1up, leptonWeight_MUON_EFF_BADMUON_STAT__1down, "weight", "overallNormSys")
leptonWeight_MUON_EFF_BADMUON_SYS = Systematic("leptonWeight_MUON_EFF_BADMUON_SYS",configMgr.weights, leptonWeight_MUON_EFF_BADMUON_SYS__1up, leptonWeight_MUON_EFF_BADMUON_SYS__1down, "weight", "overallNormHistoSysEnvelopeSym")
leptonWeight_MUON_EFF_ISO_STAT = Systematic("leptonWeight_MUON_EFF_ISO_STAT",configMgr.weights, leptonWeight_MUON_EFF_ISO_STAT__1up, leptonWeight_MUON_EFF_ISO_STAT__1down, "weight", "overallNormHistoSysEnvelopeSym")
leptonWeight_MUON_EFF_ISO_SYS = Systematic("leptonWeight_MUON_EFF_ISO_SYS",configMgr.weights, leptonWeight_MUON_EFF_ISO_SYS__1up, leptonWeight_MUON_EFF_ISO_SYS__1down, "weight", "overallNormSys")
leptonWeight_MUON_EFF_RECO_STAT = Systematic("leptonWeight_MUON_EFF_RECO_STAT",configMgr.weights, leptonWeight_MUON_EFF_RECO_STAT__1up, leptonWeight_MUON_EFF_RECO_STAT__1down, "weight", "overallNormHistoSysEnvelopeSym")
leptonWeight_MUON_EFF_RECO_STAT_LOWPT = Systematic("leptonWeight_MUON_EFF_RECO_STAT_LOWPT",configMgr.weights, leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1up, leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1down, "weight", "overallNormHistoSysEnvelopeSym")
leptonWeight_MUON_EFF_RECO_SYS = Systematic("leptonWeight_MUON_EFF_RECO_SYS",configMgr.weights, leptonWeight_MUON_EFF_RECO_SYS__1up, leptonWeight_MUON_EFF_RECO_SYS__1down, "weight", "overallNormSys")
leptonWeight_MUON_EFF_RECO_SYS_LOWPT = Systematic("leptonWeight_MUON_EFF_RECO_SYS_LOWPT",configMgr.weights, leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1up, leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1down, "weight", "overallNormSys")
leptonWeight_MUON_EFF_TTVA_STAT = Systematic("leptonWeight_MUON_EFF_TTVA_STAT",configMgr.weights, leptonWeight_MUON_EFF_TTVA_STAT__1up, leptonWeight_MUON_EFF_TTVA_STAT__1down, "weight", "overallNormSys")
leptonWeight_MUON_EFF_TTVA_SYS = Systematic("leptonWeight_MUON_EFF_TTVA_SYS",configMgr.weights, leptonWeight_MUON_EFF_TTVA_SYS__1up, leptonWeight_MUON_EFF_TTVA_SYS__1down, "weight", "overallNormSys")
bTagWeight_FT_EFF_B_systematics = Systematic("bTagWeight_FT_EFF_B_systematics",configMgr.weights, bTagWeight_FT_EFF_B_systematics__1up, bTagWeight_FT_EFF_B_systematics__1down, "weight", "overallNormSys")
bTagWeight_FT_EFF_C_systematics = Systematic("bTagWeight_FT_EFF_C_systematics",configMgr.weights, bTagWeight_FT_EFF_C_systematics__1up, bTagWeight_FT_EFF_C_systematics__1down, "weight", "overallNormSys")
bTagWeight_FT_EFF_Light_systematics = Systematic("bTagWeight_FT_EFF_Light_systematics",configMgr.weights, bTagWeight_FT_EFF_Light_systematics__1up, bTagWeight_FT_EFF_Light_systematics__1down, "weight", "overallNormSys")
bTagWeight_FT_EFF_extrapolation = Systematic("bTagWeight_FT_EFF_extrapolation",configMgr.weights, bTagWeight_FT_EFF_extrapolation__1up, bTagWeight_FT_EFF_extrapolation__1down, "weight", "overallNormSys")
bTagWeight_FT_EFF_extrapolation_from_charm = Systematic("bTagWeight_FT_EFF_extrapolation_from_charm",configMgr.weights, bTagWeight_FT_EFF_extrapolation_from_charm__1up, bTagWeight_FT_EFF_extrapolation_from_charm__1down, "weight", "overallNormSys")
jvtWeight_JET_JvtEfficiency = Systematic("jvtWeight_JET_JvtEfficiency",configMgr.weights, jvtWeight_JET_JvtEfficiency__1up, jvtWeight_JET_JvtEfficiency__1down, "weight", "overallNormSys")
pileupWeight = Systematic("pileupWeight", configMgr.weights, pileupWeightUp, pileupWeightDown, "weight", "overallNormSys")
SF_SLORDLMUON_EFF_TrigStatUncertainty = Systematic("SF_SLORDLMUON_EFF_TrigStatUncertainty", configMgr.weights, SF_SLORDLMUON_EFF_TrigStatUncertainty__1up, SF_SLORDLMUON_EFF_TrigStatUncertainty__1down, "weight", "overallNormSys")
SF_SLORDLMUON_EFF_TrigSystUncertainty = Systematic("SF_SLORDLMUON_EFF_TrigSystUncertainty", configMgr.weights, SF_SLORDLMUON_EFF_TrigSystUncertainty__1up, SF_SLORDLMUON_EFF_TrigSystUncertainty__1down, "weight", "overallNormSys")

leptonWeight_EL_EFF_ID_TOTAL_1NPCOR_PLUS_UNCOR = Systematic("leptonWeight_EL_EFF_ID_TOTAL_1NPCOR_PLUS_UNCOR", configMgr.weights,leptonWeight_EL_EFF_ID_TOTAL_1NPCOR_PLUS_UNCOR__1up,leptonWeight_EL_EFF_ID_TOTAL_1NPCOR_PLUS_UNCOR__1down, "weight", "overallNormSys")

leptonWeight_EL_EFF_Iso_TOTAL_1NPCOR_PLUS_UNCOR =  Systematic("leptonWeight_EL_EFF_Iso_TOTAL_1NPCOR_PLUS_UNCOR", configMgr.weights,leptonWeight_EL_EFF_Iso_TOTAL_1NPCOR_PLUS_UNCOR__1up,leptonWeight_EL_EFF_Iso_TOTAL_1NPCOR_PLUS_UNCOR__1down, "weight", "overallNormSys")

leptonWeight_EL_EFF_Reco_TOTAL_1NPCOR_PLUS_UNCOR =  Systematic("leptonWeight_EL_EFF_Reco_TOTAL_1NPCOR_PLUS_UNCOR", configMgr.weights,leptonWeight_EL_EFF_Reco_TOTAL_1NPCOR_PLUS_UNCOR__1up,leptonWeight_EL_EFF_Reco_TOTAL_1NPCOR_PLUS_UNCOR__1down, "weight", "overallNormSys")

Syst_PDFset90901 = Systematic("Syst_PDFset90901",LHE3Weight_PDFset90900, LHE3Weight_PDFset90901, LHE3Weight_PDFset90901, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90901.differentNominalTreeWeight=True
Syst_PDFset90902 = Systematic("Syst_PDFset90902",LHE3Weight_PDFset90900, LHE3Weight_PDFset90902, LHE3Weight_PDFset90902, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90902.differentNominalTreeWeight=True
Syst_PDFset90903 = Systematic("Syst_PDFset90903",LHE3Weight_PDFset90900, LHE3Weight_PDFset90903, LHE3Weight_PDFset90903, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90903.differentNominalTreeWeight=True
Syst_PDFset90904 = Systematic("Syst_PDFset90904",LHE3Weight_PDFset90900, LHE3Weight_PDFset90904, LHE3Weight_PDFset90904, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90904.differentNominalTreeWeight=True
Syst_PDFset90905 = Systematic("Syst_PDFset90905",LHE3Weight_PDFset90900, LHE3Weight_PDFset90905, LHE3Weight_PDFset90905, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90905.differentNominalTreeWeight=True
Syst_PDFset90906 = Systematic("Syst_PDFset90906",LHE3Weight_PDFset90900, LHE3Weight_PDFset90906, LHE3Weight_PDFset90906, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90906.differentNominalTreeWeight=True
Syst_PDFset90907 = Systematic("Syst_PDFset90907",LHE3Weight_PDFset90900, LHE3Weight_PDFset90907, LHE3Weight_PDFset90907, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90907.differentNominalTreeWeight=True
Syst_PDFset90908 = Systematic("Syst_PDFset90908",LHE3Weight_PDFset90900, LHE3Weight_PDFset90908, LHE3Weight_PDFset90908, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90908.differentNominalTreeWeight=True
Syst_PDFset90909 = Systematic("Syst_PDFset90909",LHE3Weight_PDFset90900, LHE3Weight_PDFset90909, LHE3Weight_PDFset90909, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90909.differentNominalTreeWeight=True
Syst_PDFset90910 = Systematic("Syst_PDFset90910",LHE3Weight_PDFset90900, LHE3Weight_PDFset90910, LHE3Weight_PDFset90910, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90910.differentNominalTreeWeight=True
Syst_PDFset90911 = Systematic("Syst_PDFset90911",LHE3Weight_PDFset90900, LHE3Weight_PDFset90911, LHE3Weight_PDFset90911, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90911.differentNominalTreeWeight=True
Syst_PDFset90912 = Systematic("Syst_PDFset90912",LHE3Weight_PDFset90900, LHE3Weight_PDFset90912, LHE3Weight_PDFset90912, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90912.differentNominalTreeWeight=True
Syst_PDFset90913 = Systematic("Syst_PDFset90913",LHE3Weight_PDFset90900, LHE3Weight_PDFset90913, LHE3Weight_PDFset90913, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90913.differentNominalTreeWeight=True
Syst_PDFset90914 = Systematic("Syst_PDFset90914",LHE3Weight_PDFset90900, LHE3Weight_PDFset90914, LHE3Weight_PDFset90914, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90914.differentNominalTreeWeight=True
Syst_PDFset90915 = Systematic("Syst_PDFset90915",LHE3Weight_PDFset90900, LHE3Weight_PDFset90915, LHE3Weight_PDFset90915, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90915.differentNominalTreeWeight=True
Syst_PDFset90916 = Systematic("Syst_PDFset90916",LHE3Weight_PDFset90900, LHE3Weight_PDFset90916, LHE3Weight_PDFset90916, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90916.differentNominalTreeWeight=True
Syst_PDFset90917 = Systematic("Syst_PDFset90917",LHE3Weight_PDFset90900, LHE3Weight_PDFset90917, LHE3Weight_PDFset90917, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90917.differentNominalTreeWeight=True
Syst_PDFset90918 = Systematic("Syst_PDFset90918",LHE3Weight_PDFset90900, LHE3Weight_PDFset90918, LHE3Weight_PDFset90918, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90918.differentNominalTreeWeight=True
Syst_PDFset90919 = Systematic("Syst_PDFset90919",LHE3Weight_PDFset90900, LHE3Weight_PDFset90919, LHE3Weight_PDFset90919, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90919.differentNominalTreeWeight=True
Syst_PDFset90920 = Systematic("Syst_PDFset90920",LHE3Weight_PDFset90900, LHE3Weight_PDFset90920, LHE3Weight_PDFset90920, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90920.differentNominalTreeWeight=True
Syst_PDFset90921 = Systematic("Syst_PDFset90921",LHE3Weight_PDFset90900, LHE3Weight_PDFset90921, LHE3Weight_PDFset90921, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90921.differentNominalTreeWeight=True
Syst_PDFset90922 = Systematic("Syst_PDFset90922",LHE3Weight_PDFset90900, LHE3Weight_PDFset90922, LHE3Weight_PDFset90922, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90922.differentNominalTreeWeight=True
Syst_PDFset90923 = Systematic("Syst_PDFset90923",LHE3Weight_PDFset90900, LHE3Weight_PDFset90923, LHE3Weight_PDFset90923, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90923.differentNominalTreeWeight=True
Syst_PDFset90924 = Systematic("Syst_PDFset90924",LHE3Weight_PDFset90900, LHE3Weight_PDFset90924, LHE3Weight_PDFset90924, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90924.differentNominalTreeWeight=True
Syst_PDFset90925 = Systematic("Syst_PDFset90925",LHE3Weight_PDFset90900, LHE3Weight_PDFset90925, LHE3Weight_PDFset90925, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90925.differentNominalTreeWeight=True
Syst_PDFset90926 = Systematic("Syst_PDFset90926",LHE3Weight_PDFset90900, LHE3Weight_PDFset90926, LHE3Weight_PDFset90926, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90926.differentNominalTreeWeight=True
Syst_PDFset90927 = Systematic("Syst_PDFset90927",LHE3Weight_PDFset90900, LHE3Weight_PDFset90927, LHE3Weight_PDFset90927, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90927.differentNominalTreeWeight=True
Syst_PDFset90928 = Systematic("Syst_PDFset90928",LHE3Weight_PDFset90900, LHE3Weight_PDFset90928, LHE3Weight_PDFset90928, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90928.differentNominalTreeWeight=True
Syst_PDFset90929 = Systematic("Syst_PDFset90929",LHE3Weight_PDFset90900, LHE3Weight_PDFset90929, LHE3Weight_PDFset90929, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90929.differentNominalTreeWeight=True
Syst_PDFset90930 = Systematic("Syst_PDFset90930",LHE3Weight_PDFset90900, LHE3Weight_PDFset90930, LHE3Weight_PDFset90930, "weight", "overallNormHistoSysOneSideSym")
Syst_PDFset90930.differentNominalTreeWeight=True


#add not weight systs
#JET_EtaIntercalibration_NonClosure_highE = Systematic("JET_EtaIntercalibration_NonClosure_highE","_Nominal", "_JET_EtaIntercalibration_NonClosure_highE__1up", "_JET_EtaIntercalibration_NonClosure_highE__1down", "tree", "overallNormSys")

JET_EtaIntercalibration_NonClosure_highE = Systematic("JET_EtaIntercalibration_NonClosure_highE","_Nominal", "_JET_EtaIntercalibration_NonClosure_highE__1up", "_JET_EtaIntercalibration_NonClosure_highE__1down", "tree", "overallNormHistoSysEnvelopeSym")

JET_EtaIntercalibration_NonClosure_negEta = Systematic("JET_EtaIntercalibration_NonClosure_negEta","_Nominal", "_JET_EtaIntercalibration_NonClosure_negEta__1up", "_JET_EtaIntercalibration_NonClosure_negEta__1down", "tree", "overallNormSys")
JET_EtaIntercalibration_NonClosure_posEta = Systematic("JET_EtaIntercalibration_NonClosure_posEta","_Nominal", "_JET_EtaIntercalibration_NonClosure_posEta__1up", "_JET_EtaIntercalibration_NonClosure_posEta__1down", "tree", "overallNormSys")
JET_Flavor_Response = Systematic("JET_Flavor_Response","_Nominal", "_JET_Flavor_Response__1up", "_JET_Flavor_Response__1down", "tree", "overallNormSys")
JET_GroupedNP_1 = Systematic("JET_GroupedNP_1","_Nominal", "_JET_GroupedNP_1__1up", "_JET_GroupedNP_1__1down", "tree", "overallNormSys")
JET_GroupedNP_2 = Systematic("JET_GroupedNP_2","_Nominal", "_JET_GroupedNP_2__1up", "_JET_GroupedNP_2__1down", "tree", "overallNormSys")
JET_GroupedNP_3 = Systematic("JET_GroupedNP_3","_Nominal", "_JET_GroupedNP_3__1up", "_JET_GroupedNP_3__1down", "tree", "overallNormSys")
MUON_SAGITTA_RESBIAS = Systematic("MUON_SAGITTA_RESBIAS","_Nominal", "_MUON_SAGITTA_RESBIAS__1up", "_MUON_SAGITTA_RESBIAS__1down", "tree", "overallNormSys")
MUON_SAGITTA_RHO = Systematic("MUON_SAGITTA_RHO","_Nominal", "_MUON_SAGITTA_RHO__1up", "_MUON_SAGITTA_RHO__1down", "tree", "overallNormHistoSysEnvelopeSym")
JET_JER_DataVsMC = Systematic("JET_JER_DataVsMC_AFII","_Nominal", "_JET_JER_DataVsMC_AFII__1up", "_JET_JER_DataVsMC_AFII__1down", "tree", "overallNormSys")
#JET_JER_EffectiveNP_1 = Systematic("JET_JER_EffectiveNP_1","_Nominal", "_JET_JER_EffectiveNP_1__1up", "_JET_JER_EffectiveNP_1__1down", "tree", "overallNormSys")
JET_JER_EffectiveNP_1 = Systematic("JET_JER_EffectiveNP_1","_Nominal", "_JET_JER_EffectiveNP_1__1up", "_JET_JER_EffectiveNP_1__1down", "tree", "overallNormHistoSysEnvelopeSym")
JET_JER_EffectiveNP_2 = Systematic("JET_JER_EffectiveNP_2","_Nominal", "_JET_JER_EffectiveNP_2__1up", "_JET_JER_EffectiveNP_2__1down", "tree", "overallNormHistoSysEnvelopeSym")
JET_JER_EffectiveNP_3 = Systematic("JET_JER_EffectiveNP_3","_Nominal", "_JET_JER_EffectiveNP_3__1up", "_JET_JER_EffectiveNP_3__1down", "tree", "overallNormHistoSysEnvelopeSym")
JET_JER_EffectiveNP_4 = Systematic("JET_JER_EffectiveNP_4","_Nominal", "_JET_JER_EffectiveNP_4__1up", "_JET_JER_EffectiveNP_4__1down", "tree", "overallNormHistoSysEnvelopeSym")
JET_JER_EffectiveNP_5 = Systematic("JET_JER_EffectiveNP_5","_Nominal", "_JET_JER_EffectiveNP_5__1up", "_JET_JER_EffectiveNP_5__1down", "tree", "overallNormHistoSysEnvelopeSym")
JET_JER_EffectiveNP_6 = Systematic("JET_JER_EffectiveNP_6","_Nominal", "_JET_JER_EffectiveNP_6__1up", "_JET_JER_EffectiveNP_6__1down", "tree", "overallNormHistoSysEnvelopeSym")
JET_JER_EffectiveNP_7restTerm = Systematic("JET_JER_EffectiveNP_7restTerm","_Nominal", "_JET_JER_EffectiveNP_7restTerm__1up", "_JET_JER_EffectiveNP_7restTerm__1down", "tree", "overallNormHistoSysEnvelopeSym")
MET_SoftTrk_ResoPara = Systematic("MET_SoftTrk_ResoPara","_Nominal","_MET_SoftTrk_ResoPara","_MET_SoftTrk_ResoPara","tree","overallNormHistoSysOneSideSym")
MET_SoftTrk_ResoPerp = Systematic("MET_SoftTrk_ResoPerp","_Nominal","_MET_SoftTrk_ResoPerp","_MET_SoftTrk_ResoPerp","tree","overallNormHistoSysOneSideSym")
MET_SoftTrk_Scale = Systematic("MET_SoftTrk_Scale","_Nominal","_MET_SoftTrk_ScaleUp","_MET_SoftTrk_ScaleDown","tree","overallNormHistoSysEnvelopeSym")
ttbar__genSysNominal = Systematic("ttbar__genSysNominal","_AF2Nominal","_genSysNominal","_genSysNominal","tree","overallNormHistoSysOneSideSym")
ttbar__radSysNominal = Systematic("ttbar__radSysNominal","_AF2Nominal","_radUpSysNominal","_radDownSysNominal","tree","overallNormSys")
ttbar__showerSysNominal = Systematic("ttbar__showerSysNominal","_AF2Nominal","_showerSysNominal","_showerSysNominal","tree","overallNormHistoSysOneSideSym")

MUON_ID = Systematic("MUON_ID", "_Nominal","_MUON_ID__1up", "_MUON_ID__1down", "tree", "overallNormHistoSysOneSideSym")
MUON_MS = Systematic("MUON_MS", "_Nominal","_MUON_MS__1up", "_MUON_MS__1down", "tree", "overallNormHistoSysOneSideSym")
MUON_SCALE = Systematic("MUON_SCALE", "_Nominal","_MUON_SCALE__1up", "_MUON_SCALE__1down", "tree", "overallNormHistoSysOneSideSym")
EG_Resolution = Systematic("EG_Resolution", "_Nominal","_EG_RESOLUTION_ALL__1up", "_EG_RESOLUTION_ALL__1down", "tree", "overallNormHistoSysOneSideSym")
EG_Scale = Systematic("EG_Scale", "_Nominal","_EG_SCALE_ALL__1up", "_EG_SCALE_ALL__1down", "tree", "overallNormHistoSysOneSideSym")

#smooth sys
ttbar__genSysUser = Systematic("ttbar__genSysNominal",configMgr.weights, 1.17, 0.83 ,"user","userOverallSys")
ttbar__showerSysUser = Systematic("ttbar__showerSysNominal",configMgr.weights, 1.22, 0.78 ,"user","userOverallSys")
ttbar__radSysUser = Systematic("ttbar__radSysNominal",configMgr.weights, 1.04, 0.97 ,"user","userOverallSys")




# TODO: Below are examples from other analysis. We will have to create one for H->aa->bbmumu analysis.


# Adding Systematics to samples
weightSyst = [bTagWeight_FT_EFF_B_systematics, bTagWeight_FT_EFF_C_systematics, bTagWeight_FT_EFF_Light_systematics, leptonWeight_MUON_EFF_RECO_SYS_LOWPT]#, jvtWeight_JET_JvtEfficiency] #JVT zeza
#weightSyst = [bTagWeight_FT_EFF_B_systematics, bTagWeight_FT_EFF_C_systematics, bTagWeight_FT_EFF_Light_systematics] 

if not doReducedSyst:
    #weightSyst += [leptonWeight_MUON_EFF_BADMUON_STAT , leptonWeight_MUON_EFF_BADMUON_SYS , leptonWeight_MUON_EFF_ISO_STAT , leptonWeight_MUON_EFF_ISO_SYS , leptonWeight_MUON_EFF_RECO_STAT , leptonWeight_MUON_EFF_RECO_STAT_LOWPT , leptonWeight_MUON_EFF_RECO_SYS , leptonWeight_MUON_EFF_TTVA_STAT , leptonWeight_MUON_EFF_TTVA_SYS , SF_SLORDLMUON_EFF_TrigStatUncertainty, SF_SLORDLMUON_EFF_TrigSystUncertainty] #ovdje ne radi pileupWeight
    #weightSyst += [leptonWeight_MUON_EFF_BADMUON_SYS , leptonWeight_MUON_EFF_ISO_STAT , leptonWeight_MUON_EFF_ISO_SYS , leptonWeight_MUON_EFF_RECO_STAT , leptonWeight_MUON_EFF_RECO_STAT_LOWPT , leptonWeight_MUON_EFF_RECO_SYS , leptonWeight_MUON_EFF_TTVA_STAT , leptonWeight_MUON_EFF_TTVA_SYS , SF_SLORDLMUON_EFF_TrigStatUncertainty] #SF_SLORDLMUON_EFF_TrigSystUncertainty screws up Zjets, leptonWeight_MUON_EFF_BADMUON_STAT screws up both zjets and ttV
    weightSyst = [leptonWeight_MUON_EFF_BADMUON_SYS , leptonWeight_MUON_EFF_ISO_STAT , leptonWeight_MUON_EFF_ISO_SYS , leptonWeight_MUON_EFF_RECO_STAT , leptonWeight_MUON_EFF_RECO_STAT_LOWPT , leptonWeight_MUON_EFF_RECO_SYS , leptonWeight_MUON_EFF_RECO_SYS_LOWPT , leptonWeight_MUON_EFF_TTVA_STAT , leptonWeight_MUON_EFF_TTVA_SYS , bTagWeight_FT_EFF_B_systematics , bTagWeight_FT_EFF_C_systematics , bTagWeight_FT_EFF_Light_systematics , bTagWeight_FT_EFF_extrapolation , bTagWeight_FT_EFF_extrapolation_from_charm,jvtWeight_JET_JvtEfficiency, pileupWeight, SF_SLORDLMUON_EFF_TrigStatUncertainty, SF_SLORDLMUON_EFF_TrigSystUncertainty] #leptonWeight_MUON_EFF_BADMUON_STAT --> NE RADI
#^got from bbumum recently, mar 20201


SystList = weightSyst

if not doReducedSyst:
    SystList.append(JET_EtaIntercalibration_NonClosure_highE)
    SystList.append(JET_EtaIntercalibration_NonClosure_posEta)
    SystList.append(JET_EtaIntercalibration_NonClosure_negEta)
    SystList.append(MUON_SAGITTA_RESBIAS)
    SystList.append(MUON_SAGITTA_RHO)
    #SystList.append(JET_JER_DataVsMC) #added with wrong name
    SystList.append(JET_JER_EffectiveNP_1)
    SystList.append(JET_JER_EffectiveNP_2)
    SystList.append(JET_JER_EffectiveNP_3)
    SystList.append(JET_JER_EffectiveNP_4)
    SystList.append(JET_JER_EffectiveNP_5)
    SystList.append(JET_JER_EffectiveNP_6)
    SystList.append(JET_JER_EffectiveNP_7restTerm)


SystList.append(JET_Flavor_Response)
SystZjet = SystList
SystList.append(JET_GroupedNP_1)
SystList.append(JET_GroupedNP_2)
SystList.append(JET_GroupedNP_3)

SystListTTBar = SystList[:]

#if not doReducedSyst:
#   SystListTTBar = SystListTTBar + [Syst_PDFset90901,Syst_PDFset90902,Syst_PDFset90903,Syst_PDFset90904,Syst_PDFset90905,Syst_PDFset90906,Syst_PDFset90907,Syst_PDFset90908,Syst_PDFset90909,Syst_PDFset90910, Syst_PDFset90911,Syst_PDFset90912,Syst_PDFset90913,Syst_PDFset90914,Syst_PDFset90915,Syst_PDFset90916,Syst_PDFset90917,Syst_PDFset90918,Syst_PDFset90919,Syst_PDFset90920,Syst_PDFset90921, Syst_PDFset90922,Syst_PDFset90923,Syst_PDFset90924,Syst_PDFset90925,Syst_PDFset90926,Syst_PDFset90927,Syst_PDFset90928,Syst_PDFset90929,Syst_PDFset90930] #all PDF are here 

   #SystListTTBar += [Syst_PDFset90903,Syst_PDFset90907,Syst_PDFset90909,Syst_PDFset90913,Syst_PDFset90914,Syst_PDFset90915,Syst_PDFset90916,Syst_PDFset90918,Syst_PDFset90920,Syst_PDFset90921, Syst_PDFset90922,Syst_PDFset90923,Syst_PDFset90924,Syst_PDFset90925,Syst_PDFset90926,Syst_PDFset90927,Syst_PDFset90929,Syst_PDFset90930] #PDF with < 0.03% effect in SR
   #SystListTTBar = SystListTTBar + [Syst_PDFset90901,Syst_PDFset90902,Syst_PDFset90904,Syst_PDFset90905,Syst_PDFset90906,Syst_PDFset90908,Syst_PDFset90910, Syst_PDFset90911,Syst_PDFset90912,Syst_PDFset90917,Syst_PDFset90919,Syst_PDFset90928] #PDF with > 0.03% effect in SR


#SystListTTBar.append(ttbar__radSysNominal)
#SystListTTBar.append(ttbar__showerSysNominal)
#do not apply smoothing for theory sys
if doDefaultTTtheo:
  SystListTTBar.append(ttbar__genSysNominal)
  SystListTTBar.append(ttbar__radSysNominal)
  SystListTTBar.append(ttbar__showerSysNominal)


SystListTTBar.append(leptonWeight_EL_EFF_ID_TOTAL_1NPCOR_PLUS_UNCOR)
SystListTTBar.append(leptonWeight_EL_EFF_Iso_TOTAL_1NPCOR_PLUS_UNCOR)
SystListTTBar.append(leptonWeight_EL_EFF_Reco_TOTAL_1NPCOR_PLUS_UNCOR)
SystListTTBar.append(MUON_ID)
SystListTTBar.append(MUON_MS)
SystListTTBar.append(MUON_SCALE)
SystListTTBar.append(EG_Resolution)
SystListTTBar.append(EG_Scale)

if doSystDDZtt:
    SetupSamples([topSample], SystListTTBar)
    #SetupSamples([zSample], SystList)
    SetupSamples([ttVSample], SystList)


smallBgSyst = []#remove systs from small bkgs

if doSystSmallBkg:
    #smallBkgSamples=[WjetsSample,ttVSample,stSample, dbSample]
    #SetupSamples( smallBkgSamples, smallBgSyst )
    
    # Xsec uncertainties
    db_Xsec = Systematic("db_Xsec",configMgr.weights, 1.5, 0.5 ,"user","userOverallSys")
    ttV_Xsec = Systematic("ttW_Xsec",configMgr.weights, 1.13, 0.87 ,"user","userOverallSys")
    W_Xsec = Systematic("W_Xsec",configMgr.weights, 1.5, 0.50 ,"user","userOverallSys")
    st_Xsec = Systematic("st_Xsec",configMgr.weights, 1.05, 0.95 ,"user","userOverallSys")
    zLF_Xsec = Systematic("zLF_Xsec",configMgr.weights, 1.35, 0.65 ,"user","userOverallSys")
  
  
    WjetsSample.addSystematic(W_Xsec)
    dbSample.addSystematic(db_Xsec)
    stSample.addSystematic(st_Xsec)
    ttWSample.addSystematic(ttV_Xsec)
    zSampleO.addSystematic(zLF_Xsec)



#-------------------------------------------
# List of samples and their plotting colours
#-------------------------------------------

#************
#Bkg only fit
#************

bkt = configMgr.addFitConfig("BkgOnly")
if useStat:
    bkt.statErrThreshold=0.001 
else:
    bkt.statErrThreshold=None



#bkt.addSamples([topSample,zSample,stSample,ttVSample,dbSample,WjetsSample,dataSample,ttWSample]) # orig

#bkt.addSamples([topSample,zSample,zSampleO,stSample,ttVSample,dbSample,WjetsSample,dataSample,ttWSample]) # Z + HF split

bkt.addSamples([WjetsSample,ttVSample,ttWSample,zSampleO,stSample,dbSample,zSample,topSample,dataSample]) # Z + HF split


#-------------------------------------------------
# Constraining regions - statistically independent
#-------------------------------------------------

# SBZ using nJet
#nJetWS = bkt.addChannel("dimuon_m",["CRZ"],6,78,102)
nJetWS = bkt.addChannel("cuts",["CRZ"],1,0.5,1.5)
#nJetWS = bkt.addChannel("mu2Pt",["CRZ"],10,0,100)
nJetWS.hasB = False
nJetWS.hasBQCD = False
nJetWS.useOverflowBin = False

# TR using nJet
nJetTS = bkt.addChannel("cuts", ["CRTop"], 1, 0.5, 1.5)

nJetTS.hasB = False
nJetTS.hasBQCD = False
nJetTS.useOverflowBin = False    

#ttV
#nttV = bkt.addChannel("dimuon_m",["CRttZ"],6,78,102)
nttV = bkt.addChannel("cuts",["CRttZ"],1,0.5,1.5)
#nttV = bkt.addChannel("mu2Pt",["CRttV"],10,0,100)
nttV.hasB = False
nttV.hasBQCD = False
nttV.useOverflowBin = False


bkt.addBkgConstrainChannels([nJetWS,nJetTS,nttV])

######################################### #
#    Example new cosmetics                #
#                                         #
##########################################

# Set global plotting colors/styles
bkt.dataColor = dataSample.color
bkt.totalPdfColor = ROOT.TColor.GetColor("#8bd3fb") 
#bkt.XErrorSize = -1.
bkt.errorFillColor = kBlue-5
bkt.errorFillStyle = 3004
bkt.errorLineStyle = kDashed
#bkt.errorLineColor = kBlue-5



# Set Channel titleX, titleY, minY, maxY, logY
#nJetWS.logY = True
nJetWS.ATLASLabelX = 0.15
nJetWS.ATLASLabelY = 0.82
#nJetWS.ATLASLabelText = "Work in progress"
nJetWS.ATLASLabelText = "CRZ, #sqrt{s}=13 TeV, 139 fb^{-1}"
nJetWS.titleX = "M_{#mu#mu} [GeV]"
nJetTS.ATLASLabelX = 0.15
nJetTS.ATLASLabelY = 0.82
nJetTS.ATLASLabelText = "CRt#bar{t}, #sqrt{s}=13 TeV, 139 fb^{-1}"
nJetTS.titleX = "M_{#mu#mu} [GeV]"
nttV.ATLASLabelX = 0.15
nttV.ATLASLabelY = 0.82
nttV.ATLASLabelText = "CRttZ, #sqrt{s}=13 TeV, 139 fb^{-1}"
nttV.titleX = "M_{#mu#mu} [GeV]"


#Measurment settings

meas=bkt.addMeasurement(name="NormalMeasurement",lumi=1.0,lumiErr=0.017) #.032
if myFitType==FitType.Background or myFitType==FitType.Exclusion:
   meas.addPOI("mu_SIG")
if myFitType==FitType.Discovery:
   mA=sigPointDisc.split("a")[1]
   print "adding POI: mu_cuts_SR%s"%(mA)
   meas.addPOI("mu_cuts_SR%s"%(mA)) #this has to be here!!! if it's down there, it fails
   #meas.addPOI("mu_cuts_SR1")

meas.addParamSetting("Lumi",True,1)

#--------------------------------------------------------------
# Validation regions - not necessarily statistically independent
#--------------------------------------------------------------

AllChannels = []

val_channels = []

if doValidation:

    """ 
   
    cuts_SR0 =  bkt.addChannel("cuts",["SR15"],1,1,2)
    AllChannels.append(cuts_SR0 )
    cuts_SR1 =  bkt.addChannel("cuts",["SR31"],1,1,2)
    AllChannels.append(cuts_SR1 )
    cuts_SR2 =  bkt.addChannel("cuts",["SR48"],1,1,2)
    AllChannels.append(cuts_SR2 )
    cuts_SR3 =  bkt.addChannel("cuts",["SR70"],1,1,2)
    AllChannels.append(cuts_SR3 )
         
    cuts_SR59 =  bkt.addChannel("cuts",["SR59"],1,1,2)
    cuts_SR60 =  bkt.addChannel("cuts",["SR60"],1,1,2)    
    cuts_SR61 =  bkt.addChannel("cuts",["SR61"],1,1,2)
    cuts_SR63 =  bkt.addChannel("cuts",["SR63"],1,1,2)
    cuts_SR65 =  bkt.addChannel("cuts",["SR65"],1,1,2)
    cuts_SR67 =  bkt.addChannel("cuts",["SR67"],1,1,2)
    cuts_SR69 =  bkt.addChannel("cuts",["SR69"],1,1,2)
    cuts_SR71 =  bkt.addChannel("cuts",["SR71"],1,1,2)
    cuts_SR73 =  bkt.addChannel("cuts",["SR73"],1,1,2)
    cuts_SR75 =  bkt.addChannel("cuts",["SR75"],1,1,2)
    
   
    CR_ttV = bkt.addChannel("dimuon_m",["CRttZ"],8,78,102)
    CR_Z = bkt.addChannel("dimuon_m",["CRZ"],8,78,102)
    val_channels.append(CR_ttV)
    val_channels.append(CR_Z) 

 

    #for mass in sigMasses:  
    #   SRbins = bkt.addChannel("cuts",["SR%s"%mass],1,0.5,1.5)

    #nBjetCRZ = bkt.addChannel("nBjets_77",["SR"],5,-0.5,4.5)
    #nBjetVRZ = bkt.addChannel("nBjets_77",["VRZ"],5,-0.5,4.5)
   
       
    #bjet1SR  = bkt.addChannel("leadBJetPt", ["SR"] , 11, 35, 200 )
    #val_channels.append(bjet1SR)
    #Cjet_pt = bkt.addChannel("leadCJetPt", ["SR"] , 11, 30, 195 )
    #val_channels.append(Cjet_pt)
    #jet1pt_SR = bkt.addChannel("jet1Pt",["SR"],7,15,120)
    
    """ 
    nMuons = bkt.addChannel("nMuons_signal",["SR"],4,0.5,4.5)
    val_channels.append(nMuons)
    nMuonsCR = bkt.addChannel("nMuons_signal",["CRTop"],4,0.5,4.5)
    val_channels.append(nMuonsCR)
    nMuonsVR = bkt.addChannel("nMuons_signal",["VR_Mmu1e"],4,0.5,4.5)
    val_channels.append(nMuonsVR)
   
   
    nElectrons = bkt.addChannel("nElectrons_signal",["SR"],3,0.5,3.5)
    val_channels.append(nElectrons)
    nElectronsCR = bkt.addChannel("nElectrons_signal",["CRTop"],3,0.5,3.5)
    val_channels.append(nElectronsCR)
    nElectronsVR = bkt.addChannel("nElectrons_signal",["VR_Mmu1e"],3,0.5,3.5)
    val_channels.append(nElectronsVR)
  
  
    njetSR = bkt.addChannel("nJet20",["SR"],6,0.5,6.5)
    val_channels.append(njetSR)
    njetCR = bkt.addChannel("nJet20",["CRTop"],6,0.5,6.5)
    val_channels.append(njetCR)
    njetVR = bkt.addChannel("nJet20",["VR_Mmu1e"],6,0.5,6.5)
    val_channels.append(njetVR)

    
    nBjetCR = bkt.addChannel("nBjets_77",["CRTop"],5,0.5,5.5)
    val_channels.append(nBjetCR)  
    nBjetVR = bkt.addChannel("nBjets_77",["VR_Mmu1e"],5,0.5,5.5)
    val_channels.append(nBjetVR)
    nBjetSR = bkt.addChannel("nBjets_77",["SR"],5,0.5,5.5)
    val_channels.append(nBjetSR)
    nBjetCRZ = bkt.addChannel("nBjets_77",["CRZ"],5,0.5,5.5)
    val_channels.append(nBjetCRZ)
    njetCRZ = bkt.addChannel("nJet20",["CRZ"],6,0.5,6.5)
    val_channels.append(njetCRZ)
       
    #mass variables

    #mmSR_4 = bkt.addChannel("cuts",["SR"],1,1,2)
    #mmSR_4 = bkt.addChannel("dimuon_m",["SR"],15,15,75)
    #mmSR_4 = bkt.addChannel("dimuon_m",["SR"],16,12,76)
    #mmSR_3 = bkt.addChannel("dimuon_m",["SR"],21,13,76)
    mmSR_3 = bkt.addChannel("dimuon_m",["SR"],20,15,75)
    #mmSR_2 = bkt.addChannel("dimuon_m",["SR"],30,15,75)
    #mmSR_1p5 = bkt.addChannel("dimuon_m",["SR"],42,12,75)
    
    val_channels.append(mmSR_3)
    
    #mmCR_2 = bkt.addChannel("dimuon_m",["CRTop"],14,14,42)
    #Mmu2e_CR2 = bkt.addChannel("Mmu2e_os",["CRTop"],14,15,75)  
    
    #val_channels.append(Mmu2e_CR2)

      
    mmCR_4 = bkt.addChannel("dimuon_m",["CRTop"],15,15,75)
    #mCR_3 = bkt.addChannel("dimuon_m",["CRTop"],21,14,74)
    #mmCR_2 = bkt.addChannel("dimuon_m",["CRTop"],30,14,74) 
    #mmCR_1 = bkt.addChannel("dimuon_m",["CRTop"],60,14,74)    
    
    val_channels.append(mmCR_4)
 
    #elecpt_VR = bkt.addChannel("elec1Pt",["VR_Mmu1e"],8,10,130)         
    #CR_ttV = bkt.addChannel("dimuon_m",["CRttZ"],6,78,102)  
    #CR_Z = bkt.addChannel("dimuon_m",["CRZ"],5,80,100)
    #CR_Z = bkt.addChannel("dimuon_m",["CRZ"],6,78,102)
        
    #mmVR_4 = bkt.addChannel("dimuon_m",["VR_Mmu1e"],15,15,75)
    #mmVR_3 = bkt.addChannel("dimuon_m",["VR_Mmu1e"],20,15,75)
    #mmVR_2 = bkt.addChannel("dimuon_m",["VR_Mmu1e"],30,14,74)
    #mmVR_1 = bkt.addChannel("dimuon_m",["VR_Mmu1e"],60,14,74)  
    #mmVR2_3 = bkt.addChannel("dimuon_m",["VR2"],21,13,76)  
    #mmVR2_4 = bkt.addChannel("dimuon_m",["VReOS"],21,13,76)
    
    #val_channels = [mmVR_4,elecpt_VR,dimuoneta_VR,dimuonpt_VR,dR_CR2,Mmu1e_VR4]
    #bkt.addValidationChannels([cuts_VR,cuts_VR2,cuts_SR0,cuts_SR1,cuts_SR2,cuts_SR3])

  
     
    
    #Mmu1eCR = bkt.addChannel("Mmu1e_os",["CRTop"],15,16,76)
    
    Mmu2e_CR4 = bkt.addChannel("Mmu2e_os",["CRTop"],15,15,75)
    #Mmu2e_CR3 = bkt.addChannel("Mmu2e_os",["CRTop"],20,16,76)
    #Mmu2e_CR2 = bkt.addChannel("Mmu2e_os",["CRTop"],30,16,76)
    #Mmu2e_CR1 = bkt.addChannel("Mmu2e_os",["CRTop"],62,14,76)
    

    val_channels.append(Mmu2e_CR4)
    
    Mmu1e_VR4 = bkt.addChannel("Mmu1e_os",["VR_Mmu1e"],15,30,90)
    #Mmu1e_VR3 = bkt.addChannel("Mmu1e_os",["VR_Mmu1e"],20,30,90)
    #Mmu1e_VR2 = bkt.addChannel("Mmu1e_os",["VR_Mmu1e"],30,30,90)
    #Mmu1e_VR1 = bkt.addChannel("Mmu1e_os",["VR_Mmu1e"],60,30,90)

    #Mmu1e_SR = bkt.addChannel("Mmu1e_os",["SR"],15,16,76)
     
    #Mmu2e_VR2 = bkt.addChannel("Mmu2e_os",["VR_Mmu1e"],15,15,75)
    
    
    isOS_VR = bkt.addChannel("isOS",["VR_Mmu1e"],2,-.5,1.5)
 
    val_channels.append(Mmu1e_VR4)
    val_channels.append(isOS_VR)
    
   
          
    #kinematics         
    m1pt_CR = bkt.addChannel("mu1Pt",["CRTop"],7,15,120)
    m1pt_SR = bkt.addChannel("mu1Pt",["SR"],7,15,120) 
    m1pt_VR = bkt.addChannel("mu1Pt",["VR_Mmu1e"],7,15,120)    
 
    val_channels.append(m1pt_CR) 
    val_channels.append(m1pt_SR)
    val_channels.append(m1pt_VR)


    m2pt_CR = bkt.addChannel("mu2Pt",["CRTop"],5,5,80)
    m2pt_VR = bkt.addChannel("mu2Pt",["VR_Mmu1e"],5,5,80) 
    m2pt_SR = bkt.addChannel("mu2Pt",["SR"],5,5,80)

    val_channels.append(m2pt_CR)
    val_channels.append(m2pt_SR)
    val_channels.append(m2pt_VR)
   

    elecpt_SR = bkt.addChannel("elec1Pt",["SR"],7,20,125)
    elecpt_CR = bkt.addChannel("elec1Pt",["CRTop"],7,20,125)
    elecpt_VR = bkt.addChannel("elec1Pt",["VR_Mmu1e"],7,20,125)   
  
    val_channels.append(elecpt_SR)
    val_channels.append(elecpt_CR)
    val_channels.append(elecpt_VR)
    
    dimuonpt_SR =  bkt.addChannel("dimuon_pt",["SR"],8,10,130)
    #dimuonpt_CR = bkt.addChannel("dimuon_pt",["CRTop"],8,10,130)
    dimuonpt_CR = bkt.addChannel("dimuon_pt",["CRTop"],6,15,105)
    dimuonpt_VR = bkt.addChannel("dimuon_pt",["VR_Mmu1e"],8,10,130)
   
    val_channels.append(dimuonpt_SR)
    val_channels.append(dimuonpt_CR)
    val_channels.append(dimuonpt_VR)
    
   
    m1eta_CR = bkt.addChannel("mu1Eta",["CRTop"],10,-3.5,3.5)
    m1eta_VR = bkt.addChannel("mu1Eta",["VR_Mmu1e"],10,-3.5,3.5)
    m1eta_SR = bkt.addChannel("mu1Eta",["SR"],10,-3.5,3.5)


    m2eta_CR = bkt.addChannel("mu2Eta",["CRTop"],10,-3.5,3.5)
    m2eta_VR = bkt.addChannel("mu2Eta",["VR_Mmu1e"],10,-3.5,3.5)
    m2eta_SR = bkt.addChannel("mu2Eta",["SR"],10,-3.5,3.5)
        
    elec1eta_CR = bkt.addChannel("elec1Eta",["CRTop"],10,-3.5,3.5)    
    elec1eta_SR = bkt.addChannel("elec1Eta",["SR"],10,-3.5,3.5)
    elec1eta_VR = bkt.addChannel("elec1Eta",["VR_Mmu1e"],10,-3.5,3.5)
 
  
    val_channels.append(m1eta_CR)
    val_channels.append(m1eta_SR)
    val_channels.append(m1eta_VR)
    val_channels.append(m2eta_CR)
    val_channels.append(m2eta_SR)
    val_channels.append(m2eta_VR)
    val_channels.append(elec1eta_CR)
    val_channels.append(elec1eta_SR)
    val_channels.append(elec1eta_VR)
    
    

    dR_CR = bkt.addChannel("dRm1m2",["CRTop"],10,0,5)
    dR_SR = bkt.addChannel("dRm1m2",["SR"],10,0,5)
    dR_VR2 = bkt.addChannel("dRm1m2",["VR_Mmu1e"],10,0,5)
    
    dR_Mmme1SR = bkt.addChannel("dRMmme1",["SR"],10,0,5)
    dR_Mmme1CR = bkt.addChannel("dRMmme1",["CRTop"],10,0,5)
    
    val_channels.append(dR_CR)
    val_channels.append(dR_SR)
    val_channels.append(dR_VR2) 
    val_channels.append(dR_Mmme1SR)
    val_channels.append(dR_Mmme1CR) 

     
    dimuoneta_SR =  bkt.addChannel("dimuon_eta",["SR"],10,-3.5,3.5)
    dimuoneta_CR = bkt.addChannel("dimuon_eta",["CRTop"],10,-3.5,3.5)
    dimuoneta_VR = bkt.addChannel("dimuon_eta",["VR_Mmu1e"],10,-3.5,3.5)
    
    val_channels.append(dimuoneta_SR)
    val_channels.append(dimuoneta_CR)
    val_channels.append(dimuoneta_VR)
     

    bkt.addValidationChannels (val_channels)    
            
    
    #bkt.addValidationChannels([ mmSR_2, mmCR_2, mmVR_2, Mmu1eCR, Mmu2e_CR2, Mmu1e_VR2  ]) # dilep variables  
    
    
    #jun26 phi plots
    """    
    dR_Mmme1 = bkt.addChannel("dRMmme1",["SR"],10,0,5)
    val_channels.append(dR_Mmme1)
    dPhi_Mmme1 = bkt.addChannel("dPhiMmme1",["SR"],10,-3.5,3.5)
    val_channels.append(dPhi_Mmme1)
    dEta_Mmme1 = bkt.addChannel("dEtaMmme1",["SR"],5,0,3.5)
    val_channels.append(dEta_Mmme1)
    dPhi_mu1mu2 = bkt.addChannel("dPhimu1mu2",["SR"],10,-3.5,3.5)
    val_channels.append(dPhi_mu1mu2)
    
    dR_Mmme1CR = bkt.addChannel("dRMmme1",["CRTop"],10,0,5)
    val_channels.append(dR_Mmme1CR)
    dPhi_Mmme1CR = bkt.addChannel("dPhiMmme1",["CRTop"],10,-3.5,3.5)
    val_channels.append(dPhi_Mmme1CR)
    dEta_Mmme1CR = bkt.addChannel("dEtaMmme1",["CRTop"],5,0,3.5)
    val_channels.append(dEta_Mmme1CR)
    dPhi_mu1mu2CR = bkt.addChannel("dPhimu1mu2",["CRTop"],10,-3.5,3.5)
    val_channels.append(dPhi_mu1mu2CR)
    
    dR_Mmme1VR = bkt.addChannel("dRMmme1",["VR_Mmu1e"],10,0,5)
    val_channels.append(dR_Mmme1VR)
    dPhi_Mmme1VR = bkt.addChannel("dPhiMmme1",["VR_Mmu1e"],10,-3.5,3.5)
    val_channels.append(dPhi_Mmme1VR)
    dEta_Mmme1VR = bkt.addChannel("dEtaMmme1",["VR_Mmu1e"],5,0,3.5)
    val_channels.append(dEta_Mmme1VR)
    dPhi_mu1mu2VR = bkt.addChannel("dPhimu1mu2",["VR_Mmu1e"],10,-3.5,3.5)
    val_channels.append(dPhi_mu1mu2VR)
    """
 

#only works for bkg-only mode up here, thus moved it down
"""
if not doDefaultTTtheo and doSystDDZtt:
     print  AllChannels
     AllChannels.append( nttV )
     AllChannels.append( nJetTS  )
     AllChannels.append( nJetWS  )
     print  AllChannels
     for chan in AllChannels:
        if ("cuts_SR" in chan.name ):
            chan.getSample('ttbar_').addSystematic(ttbar__genSysUser)
            chan.getSample('ttbar_').addSystematic(ttbar__showerSysNominal)
            chan.getSample('ttbar_').addSystematic(ttbar__radSysNominal)
            print "Adding tt Generator User Systematics to (%s)" %chan.name
            #chan.getSample('DDZ_').addSystematic(DDZ_BDTcuteff) #add BDTcuteff only for DDZ sample
            #print "Adding DDZ_BDTcuteff Systematics to (%s)" %chan.name
        else:
            chan.getSample('ttbar_').addSystematic(ttbar__genSysNominal)
            chan.getSample('ttbar_').addSystematic(ttbar__radSysNominal)
            chan.getSample('ttbar_').addSystematic(ttbar__showerSysNominal)
            print "Adding tt Generator Tree Systematics to (%s)" %chan.name

"""        
   
#**************
# Discovery fit
#**************

if myFitType==FitType.Discovery:
 

   #for mA in sigMassesDisc:
   discovery = configMgr.addFitConfigClone(bkt,"Discovery_SR%s"%(mA))
   #discovery = configMgr.addFitConfigClone(bkt,"Discovery")
   #Channel
   SR = discovery.addChannel("cuts",["SR%s"%(mA)],1,0.5,1.5)
   #SR = discovery.addChannel("dimuon_m",["SR"],4,55,65)
   SR.useOverflowBin = False
   AllChannels.append ( SR )
   SR.addDiscoverySamples([SR.name],[1.],[0.],[100.],[kRed])
   print "Adding discovery SR:", SR.name, ", for mA=", mA
   discovery.addSignalChannels(SR)

   
#**************
# Exclusion fit
#**************
if myFitType==FitType.Exclusion:
    # Fit config instance
    for sig in sigSamples:
      mA=sig.split("a")[1]
      print "mA is", mA
      mp = mA 
    

      if mA == "15"  or mA == "30" or mA == "45" or mA  == "60" or mA == "75":
        print "baseline signals: ",mA
    
        #sig = "H160a" + mp + "_"
        sigdir = sig
        sig = sig + "_" 
        sigSample = Sample(sig,kPink)
   	#sigSample.addInputs(["/eos/home-w/waahmed/afs_runs/new_ptag/sig/2020/mar18/mc16a/"+sigdir+"/"+sig+"16a.root"])
        #sigSample.addInputs(["/eos/home-w/waahmed/afs_runs/new_ptag/sig/2020/mar18/mc16d/"+sigdir+"/"+sig+"16d.root"])
        #sigSample.addInputs(["/eos/home-w/waahmed/afs_runs/new_ptag/sig/2020/mar18/mc16e/"+sigdir+"/"+sig+"16e.root"])
        sigSample.addInputs(["/eos/home-w/waahmed/afs_runs/conf_2021/mc16a/sig/"+sigdir+"/"+sig+"16a.root"])
        sigSample.addInputs(["/eos/home-w/waahmed/afs_runs/conf_2021/mc16d/sig/"+sigdir+"/"+sig+"16d.root"])
        sigSample.addInputs(["/eos/home-w/waahmed/afs_runs/conf_2021/mc16e/sig/"+sigdir+"/"+sig+"16e.root"])
    
        sigSample.addSampleSpecificWeight("(.1)")   

        #Hplus sys
        xsecSig = Systematic("sig_scale",configMgr.weights,1.035,.965, "user","userOverallSys") #symmetrized: https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO
        PDFSig = Systematic("sig_PDF",configMgr.weights,1.04,.957, "user","userOverallSys")
        massSig=  Systematic("sig_mass",configMgr.weights,1.03,.97, "user","userOverallSys")    
  
        SetupSamples( [sigSample], SystList + [xsecSig,PDFSig,massSig ] ) ## systematics Hplus
        sigSample.setStatConfig(useStat)
  
      else:  #dummy signals for limit interpolation
        print "==> dummy signal: ",sig, ", mA= ",mp
        from SplinePrediction import *
        splineYield=0.
        for MassYield in SplinePrediction:
        	Mass=MassYield[0]
        	Yield=MassYield[1]
        	if Mass in sig:
                    splineYield=float(Yield)
                    break
        print "The spline predicted yield of the dummy signal is:  ",splineYield
        
        #Hplus sys 
        xsecSig = Systematic("sig_scale",.1,1.035,.965, "user","userOverallSys") #symmetrized: https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO
        PDFSig = Systematic("sig_PDF",.1,1.04,.957, "user","userOverallSys")
        massSig=  Systematic("sig_mass",.1,1.03,.97, "user","userOverallSys")
        myInterpolSys = getInterError(int(mp))
        splineSig= Systematic("sig_interp",.1, 1+myInterpolSys, 1-myInterpolSys ,"user","userOverallSys")
        dummySig= Systematic("sig_exp",.1 ,1.05, 0.95 ,"user","userOverallSys") # 5% error for detector      
        dummyMCstat= Systematic("sig_MC",.1 ,1.015, 0.985 ,"user","userOverallSys")# MC stat error which is average of MC errors        
                 
        sig = sig + "_" 
        sigSample = Sample(sig, kRed)
        sigSample.buildHisto( [splineYield] ,"SR%s"%mp,"cuts",0.5 ) #building histo from interpolation prediction
        SetupSamples( [sigSample], [xsecSig,PDFSig,massSig,dummySig,splineSig,dummyMCstat] )
        

    
    #original config
   
    myTopLvl = configMgr.addFitConfigClone(bkt,"Sig_%s"%sig)
    sigSample.setNormByTheory()
    sigSample.setNormFactor("mu_SIG",1.,0.,5.)                    
    myTopLvl.addSamples(sigSample)
    myTopLvl.setSignalSample(sigSample)
    

    print "Signal name in excl setup:", sig
    SR = myTopLvl.addChannel("cuts",["SR%s"%mp],1,0.5,1.5)
    AllChannels.append( SR )
    SR.useOverflowBin=False
    myTopLvl.addSignalChannels(SR)


#this doesn't seem to work upstairs 
if not doDefaultTTtheo and doSystDDZtt:
     AllChannels.append( nttV )
     AllChannels.append( nJetTS  )
     AllChannels.append( nJetWS  )
     #print  AllChannels
     for chan in AllChannels:
        if ("cuts_SR" in chan.name ):
            chan.getSample('ttbar_').addSystematic(ttbar__genSysUser)
            print "Adding tt Generator User Systematics to (%s)" %chan.name
            chan.getSample('ttbar_').addSystematic(ttbar__showerSysUser)
            print "Adding tt Shower User Systematics to (%s)" %chan.name
            chan.getSample('ttbar_').addSystematic(ttbar__radSysUser)
            print "Adding tt Radiation User Systematics to (%s)" %chan.name

            
        else:
            chan.getSample('ttbar_').addSystematic(ttbar__genSysNominal)
            print "Adding tt Generator Tree Systematics to (%s)" %chan.name
            chan.getSample('ttbar_').addSystematic(ttbar__showerSysNominal)
            print "Adding tt Shower Tree Systematics to (%s)" %chan.name 
            chan.getSample('ttbar_').addSystematic(ttbar__radSysNominal)
            print "Adding tt Radiation Tree Systematics to (%s)" %chan.name





#fix the default labels 
for chan in val_channels:
        chan.ATLASLabelY = 0.8
        chan.ATLASLabelX = 0.14
        if "CRTop" in chan.name:
                chan.ATLASLabelText = "Internal CRt#bar{t}, #sqrt{s}=13 TeV, 139 fb^{-1}"
                #chan.ATLASLabelText = "Internal CRt#bar{t}"
                #chan.showLumi = True 
	elif "CRZ" in chan.name:
                chan.ATLASLabelText = "Internal CRZ, #sqrt{s}=13 TeV, 139 fb^{-1}"
        elif "CRttZ" in chan.name:
                chan.ATLASLabelText = "Internal CRttZ, #sqrt{s}=13 TeV, 139 fb^{-1}"      
        elif "VR" in chan.name:
        	 chan.ATLASLabelText = "Internal VR, #sqrt{s}=13 TeV, 139 fb^{-1} "
        elif "SR" in chan.name:
                chan.ATLASLabelText = "Internal SR, #sqrt{s}=13 TeV, 139 fb^{-1}"
	 	#chan.ATLASLabelText = "Preliminary"
                #chan.showLumi = True #puts the lum and com under ATLAS label 
                #chan.XErrorSize = -1.   

	if "dimuon_m" in chan.name:
        	chan.titleX = "m_{#mu#mu} [GeV]"
                #chan.titleY = "Events / 1.5 GeV"
	elif "dimuon_pt" in chan.name:
	        chan.titleX = "p_{T}^{#mu#mu} [GeV]"
                #chan.titleY = "Events / 15 GeV"
	elif "dRm1m2" in chan.name:
	        chan.titleX = "#Delta R(#mu#mu)"
	elif "Mmu1e_os" in chan.name:
        	chan.titleX = "OS dilepton M(e#mu_{1})[GeV]"
	elif "Mmu2e_os" in chan.name:
                chan.titleX = "OS dilepton M(e#mu_{2})[GeV]"
        elif "mu2Pt" in chan.name:
                chan.titleX = "p_{T} sub-leading #mu [GeV]"
                #chan.titleY = "Events / 15 GeV"
        elif "mu1Pt" in chan.name:
                chan.titleX = "p_{T} leading #mu [GeV]"
                #chan.titleY = "Events / 15 GeV"      
        elif "elec1Pt" in chan.name:
                chan.titleX = "p_{T} electron [GeV]"
                #chan.titleY = "Events / 15 GeV"
        elif "dimuon_eta" in chan.name:
                chan.titleX = "#eta_{#mu#mu}"
        elif "mu1Eta" in chan.name:
                #chan.titleX = "#eta_{#mu1} leading #mu"
                chan.titleX = "#eta leading #mu"  
        elif "mu2Eta" in chan.name:
                #chan.titleX = "#eta_{#mu2} subleading #mu"
                chan.titleX = "#eta sub-leading #mu"
        elif "elec1Eta" in chan.name:
                #chan.titleX = "#eta_{e} electron"
                chan.titleX = "#eta electron"
        elif "nBjets_77" in chan.name:
                chan.titleX = "nBjets"
        elif "nJet20" in chan.name:
                chan.titleX = "nJets"
        elif "nElectrons_signal" in chan.name:
                chan.titleX = "nElectrons"
        elif "nMuons_signal" in chan.name:
                chan.titleX = "nMuons"
        elif "leadBJetPt" in chan.name:
                chan.titleX = "leading B-jet p_{T} [GeV]" 
        elif "leadCJetPt" in chan.name:
               chan.titleX = "leading Central Jet p_{T} [GeV]"
        elif "isOS" in chan.name:
              chan.titleX = "isOS (opposite-sign)"
        elif "dRMmme1" in chan.name:
              chan.titleX = "#Delta R((#mu#mu) e)"
        elif "dPhiMmme1" in  chan.name:
              chan.titleX = "#Delta #phi((#mu#mu) e)" 
        elif "dEtaMmme1" in  chan.name:
              chan.titleX = "#Delta #eta((#mu#mu) e)"
        elif "dPhimu1mu2" in  chan.name:
              chan.titleX = "#Delta #phi(#mu_{1}#mu_{2})"
      
       
 

# Create TLegend (AK: TCanvas is needed for that, but it gets deleted afterwards)
c = TCanvas()
compFillStyle = 1001 # see ROOT for Fill styles
#leg = TLegend(0.62,0.38,0.90,.85,"") #for the pT plots in CONF
#leg = TLegend(0.62,0.50,0.90,.85,"") #from aug2021
leg = TLegend(0.71,0.50,0.94,.85,"") #thesis mar2022
#leg = TLegend(0.71,0.70,0.90,.85,"") #for Mu2e label
#leg = TLegend(0.71,0.64,0.94,.85,"") #for dieta label
#leg = TLegend(0.71,0.72,0.89,.84,"") #for diPhi label

leg.SetFillStyle(0)
leg.SetFillColor(2)
leg.SetBorderSize(0)
#
entry = TLegendEntry()
entry = leg.AddEntry("","Data","p") 
entry.SetMarkerColor(bkt.dataColor)
entry.SetMarkerStyle(20)

#entry = leg.AddEntry("","Total pdf","lf") 
entry = leg.AddEntry("","Uncertainty","lf")
entry.SetLineColor(bkt.totalPdfColor)
entry.SetLineWidth(2)
entry.SetFillColor(bkt.errorFillColor)
entry.SetFillStyle(bkt.errorFillStyle)

entry = leg.AddEntry("","t#bar{t}","lf") 
entry.SetLineColor(topSample.color)
entry.SetFillColor(topSample.color)
entry.SetFillStyle(compFillStyle)

entry = leg.AddEntry("","Z+HF","lf")
entry.SetLineColor(zSample.color)
entry.SetFillColor(zSample.color)
entry.SetFillStyle(compFillStyle)

entry = leg.AddEntry("","Diboson","lf")
entry.SetLineColor(dbSample.color)
entry.SetFillColor(dbSample.color)
entry.SetFillStyle(compFillStyle)

#entry = leg.AddEntry("","W+jets","lf") 
#entry.SetLineColor(WjetsSample.color)
#entry.SetFillColor(WjetsSample.color)
#entry.SetFillStyle(compFillStyle)


#entry = leg.AddEntry("","Single top-quark","lf")
entry = leg.AddEntry("","Single top","lf")
entry.SetLineColor(stSample.color)
entry.SetFillColor(stSample.color)
entry.SetFillStyle(compFillStyle)

entry = leg.AddEntry("","Z+LF","lf")
entry.SetLineColor(zSampleO.color)
entry.SetFillColor(zSampleO.color)
entry.SetFillStyle(compFillStyle)

entry = leg.AddEntry("","t#bar{t}W","lf")
entry.SetLineColor(ttWSample.color)
entry.SetFillColor(ttWSample.color)
entry.SetFillStyle(compFillStyle)

entry = leg.AddEntry("","t#bar{t}Z","lf")
entry.SetLineColor(ttVSample.color)
entry.SetFillColor(ttVSample.color)
entry.SetFillStyle(compFillStyle)

"""
lumiText = TLatex()
lumiText.SetNDC()
lumiText.SetTextAlign( 11 )
lumiText.SetTextFont( 42 )
lumiText.SetTextSize( 0.06 )
lumiText.SetTextColor( 1 )
#lumiText.DrawLatex(0.15, 0.7, "#sqrt{{s}}=13 TeV, {0:0.1f} fb^{{-1}}".format(getattr(MakeHistPullPlot,'luminosity',10.0)))
lumiText.DrawLatex(0.15, 0.7, "#sqrt{{s}} = 13 TeV, 139")
"""


if myFitType==FitType.Exclusion:
    entry = leg.AddEntry("","signal","lf") 
    entry.SetLineColor(kPink)
    entry.SetFillColor(kPink)
    entry.SetFillStyle(compFillStyle)


# Set legend for fitConfig
bkt.tLegend = leg
if myFitType==FitType.Exclusion:
    myTopLvl.tLegend = leg
c.Close()

# Make sure that file is re-made when before HistFactory is executed
if configMgr.executeHistFactory:
    if os.path.isfile("data/%s.root"%configMgr.analysisName):
        os.remove("data/%s.root"%configMgr.analysisName)


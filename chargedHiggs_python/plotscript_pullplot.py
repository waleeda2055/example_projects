"""
 **********************************************************************************
 * Project: HistFitter - A ROOT-based package for statistical data analysis *
 * Package: HistFitter *
 * *
 * Description: *
 * Example pull plot based on the pullPlotUtils module. Adapt to create *
 * your own style of pull plot. Illustrates all functions to redefine to *
 * change labels, colours, etc.  *
 * *
 * Authors: *
 * HistFitter group, CERN, Geneva *
 * *
 * Redistribution and use in source and binary forms, with or without *
 * modification, are permitted according to the terms listed in the file *
 * LICENSE.  *
 ********************************************************************************** """
#!/usr/bin/env python
import ROOT 
from ROOT import * 
ROOT.PyConfig.IgnoreCommandLineOptions = True 
gSystem.Load("libSusyFitter.so")
#gROOT.Reset()
ROOT.gROOT.SetBatch(True) 

import os, pickle, subprocess 
import pullPlotUtils 
from pullPlotUtils import makePullPlot

sigMasses = ["15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51","52","53","54","55","56","57","58","59","60","62","64","66","68","70","72","74","75"]



# Build a dictionary that remaps region names
def renameRegions():
    myRegionDict = {}
    # Remap region names using the old name as index, e.g.:
    myRegionDict["SS_metmeff2Jet"] = "SR exclusion"
    myRegionDict["SLVR2_nJet"] = "VR2"
    for mass in sigMasses:
      myRegionDict["SR%s"%mass] = mass 
      #SRbins = bkt.addChannel("cuts",["SR%s"%mass],1,0.5,1.5)
   
 
    
    return myRegionDict
# Build a list with all the regions you want to use
def makeRegionList():
    regionList=[]
    #regionList = ["SR16_cuts","SR17_cuts","SR18_cuts","SR19_cuts","SR20_cuts","SR21_cuts","SR22_cuts","SR23_cuts","SR24_cuts","SR25_cuts","SR26_cuts","SR27_cuts","SR28_cuts","SR29_cuts","SR30_cuts","SR31_cuts","SR32_cuts","SR33_cuts","SR34_cuts","SR35_cuts","SR36_cuts","SR37_cuts","SR38_cuts","SR39_cuts","SR40_cuts","SR41_cuts","SR42_cuts","SR43_cuts","SR44_cuts","SR45_cuts","SR46_cuts","SR47_cuts","SR48_cuts","SR49_cuts","SR50_cuts","SR51_cuts","SR52_cuts","SR53_cuts","SR54_cuts","SR55_cuts","SR56_cuts","SR57_cuts","SR58_cuts","SR59_cuts","SR60_cuts","SR61_cuts","SR62_cuts"]
    regionList = ["SR15","SR16","SR17","SR18","SR19","SR20","SR21","SR22","SR23","SR24","SR25","SR26","SR27","SR28","SR29","SR30","SR31","SR32","SR33","SR34","SR35","SR36","SR37","SR38","SR39","SR40","SR41","SR42","SR43","SR44","SR45","SR46","SR48","SR50","SR52","SR54","SR56","SR58","SR60","SR62","SR64","SR66","SR68","SR70","SR72","SR74","SR75"]
  

    regionList.sort()
    #print("descendingList",descendingList,"; ",descendingList.sort()) regionList += ["SR50_cuts","SR49_cuts","SR48_cuts","SR47_cuts","SR46_cuts"]
    #    regionList += ["SS_metmeff2Jet"] regionList=descendingList.sort()
    print("regionList:",regionList)
    return regionList
# Define the colors for the pull bars
def getRegionColor(name):
    if name.find("SLWR") != -1: return kBlue+3
    if name.find("SLTR") != -1: return kBlue+3
    if name.find("SR") != -1: return kOrange
    if name.find("SLVR") != -1: return kOrange
    if name.find("SSloose") != -1: return kOrange
    if name.find("SS_") != -1: return kRed
 
    return 1
# Define the colors for the stacked samples
def getSampleColor(sample):
    if sample == "ttbar": return TColor.GetColor("#8bd3fb")
    if sample == "DDZ": return kAzure + 8
    if sample == "singleTop": return TColor.GetColor("#9559b5")
    if sample == "diBoson": return kGreen-10
    if sample == "ttZ": return TColor.GetColor("#ff839b")
    if sample == "Zjets": return TColor.GetColor("#ffae32")
    if sample == "ZjetsO": return TColor.GetColor("#357478")
    if sample == "ttW": return kYellow+1
    if sample == "Wjets": return TColor.GetColor("#785d9f")
    #if sample == "bkg2": return TColor.GetColor("#8bd3fb") 
   
    else:
        print "cannot find color for sample (",sample,")"
    return 1 

def main():
    # Override pullPlotUtils' default colours (which are all black)
    pullPlotUtils.getRegionColor = getRegionColor
    pullPlotUtils.getSampleColor = getSampleColor
    # Where's the workspace file?
    #wsfilename = os.getenv("HISTFITTERUSER")+"/results/BkgOnly_p3983_Unblind_AllSR/BkgOnly_combined_NormalMeasurement_model_afterFit.root" #
    wsfilename = "results/MyConfigHplus_aug4_bkgonly_bbmumuplots/BkgOnly_combined_NormalMeasurement_model_afterFit.root" 
    # Where's the pickle file?
    #pickleFilename = os.getenv("HISTFITTERUSER")+"/Tables/BkgOnly_p3983_Unblind_AllSR_noBDT/YieldsSRbins_ALL_BkgOnly_p3983_Unblind_noBDT.pickle" 
#BkgOnly_p3983_Unblind_AllSR/YieldsSRbins_ALL_BkgOnly_p3983_Unblind.pickle"
    pickleFilename = "results/MyConfigHplus_aug4_bkgonly_bbmumuplots/MyYieldsTable_2.pickle"
    
    # Run blinded?
    doBlind = False
    # Used as plot title region = "Signal_region_bins"
    region = ""
    # Samples to stack on top of eachother in each region
    #samples = "DDZ,ttbar,diBoson,singleTop,ttV"
    samples = "ttbar,Zjets,diBoson,singleTop,ZjetsO,ttW,ttZ,Wjets"
    

    # Which regions do we use?
    regionList = makeRegionList()
    # Regions for which the label gets changed
    renamedRegions = renameRegions()
    if not os.path.exists(pickleFilename):
        print "pickle filename %s does not exist" % pickleFilename
        print "will proceed to run yieldstable again"
        
        # Run YieldsTable.py with all regions and samples requested
        cmd = "YieldsTable.py -c %s -s %s -w %s -o MyYieldsTable.tex" % (",".join(regionList), samples, wsfilename)
        print cmd
        subprocess.call(cmd, shell=True)
    if not os.path.exists(pickleFilename):
        print "pickle filename %s still does not exist" % pickleFilename
        return
    
    # Open the pickle and make the pull plot
    makePullPlot(pickleFilename, regionList, samples, renamedRegions, region, doBlind)

if __name__ == "__main__":
    main()

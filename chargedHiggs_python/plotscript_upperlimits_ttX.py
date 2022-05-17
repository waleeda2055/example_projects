#!/usr/bin/env python2.6
#example usage: python UL_ttX_fromLogs.py may12
import ROOT, os, re, sys
import math
import glob
from math import sqrt, pow
from ROOT import gStyle
from ROOT import gROOT
from ROOT import TStyle
from array import array
from os import listdir
from os.path import isfile, join
ROOT.gSystem.Load("libSusyFitter.so")

c_LightYellow   = ROOT.TColor.GetColor( "#ffe938" )
c_myExp      = ROOT.TColor.GetColor("#28373c")
c_LightRed   = ROOT.TColor.GetColor( "#aa000" )
c_white=ROOT.TColor.GetColor( "#FFFFFF" )

gROOT.SetStyle("Plain")
gStyle.SetPalette(1)
gStyle.SetPaintTextFormat("0.3g")
gStyle.SetOptStat(0)
gROOT.SetBatch()

gStyle.SetPadTickX(1)
gStyle.SetPadTickY(1)



##only ONE needs to be TRUE
doNBSM = False #True
doEffXsec = False
doBRttX = True
doBrHbbmm = False

#other settings
doObs = True #False   # plot observed limits 

calcChi2=True

foldername = sys.argv[1]

if doNBSM or doEffXsec:
   dirname="batchjobs/logs/"
   files = glob.glob(dirname+'out_job*.log')
elif doBRttX or doBrHbbmm:
   dirname="logs/"+ foldername +"/"
   files = glob.glob(dirname+'LimitExclFit_a*.txt') #orig

files.sort()
Npoints = len(files)

print Npoints 

exp=[0 for x in range(Npoints)]
obs=[0 for x in range(Npoints)]
exp2=[0 for x in range(Npoints)]
obs2=[0 for x in range(Npoints)]
expd=[0 for x in range(Npoints)]
expu=[0 for x in range(Npoints)]
exp2d=[0 for x in range(Npoints)]
exp2u=[0 for x in range(Npoints)]
expd2=[0 for x in range(Npoints)]
expu2=[0 for x in range(Npoints)]
exp2d2=[0 for x in range(Npoints)]
exp2u2=[0 for x in range(Npoints)]
xerr=[0 for x in range(Npoints)]
y1d=[0 for x in range(Npoints)]
y1up=[0 for x in range(Npoints)]
y2d=[0 for x in range(Npoints)]
y2up=[0 for x in range(Npoints)]
aMass =[0 for x in range(Npoints)]
signif =[0 for x in range(Npoints)]

i=-1

#content=str(_file.ls())
#print "content: ",content


failedPoints=[]
#for file in os.listdir(dirname):
for file in files:
   print 'using',file
   if ".txt" not in str(file):
      continue
   if "Limit" in str(file):
      if doNBSM or doEffXsec:
         amass =  str(file.split('_a')[1]).split('.log')[0]
      else:
         amass =  str(file.split('_a')[1]).split('.txt')[0] #orig
      print 'opening',file," for amass",amass
      _file = open(file,"r")
      lines = _file.readlines()
      _obs=""
      _exp=""
      _expd=""
      _expu=""
      _exp2d=""
      _exp2u=""
      for line in lines:
         if not ("<INFO> HypoTestTool: The computed upper limit is" in line) and not ("<INFO> HypoTestTool:  expected limit" in line):
            continue
         if "<INFO> HypoTestTool: The computed upper limit is:" in line:
            print line.strip()
            _obs=line.split(' ')[7].strip()
         elif "<INFO> HypoTestTool:  expected limit (median)" in line:
            print line.strip()
            _exp=line.split(') ')[1].strip()
         elif "<INFO> HypoTestTool:  expected limit (-1 sig)" in line:
            print line.strip()
            _expd=line.split(') ')[1].strip()
         elif "<INFO> HypoTestTool:  expected limit (+1 sig)" in line:
            print line.strip()
            _expu=line.split(') ')[1].strip()
         elif "<INFO> HypoTestTool:  expected limit (-2 sig)" in line:
            print line.strip()
            _exp2d=line.split(') ')[1].strip()
         elif "<INFO> HypoTestTool:  expected limit (+2 sig)" in line:
            print line.strip()
            _exp2u=line.split(') ')[1].strip()
         if not _obs=="" and not _exp=="" and not _expd=="" and not _expu=="" and not _exp2d=="" and not _exp2u=="":
            break
      # check here if signal name is correct and one gets limits
      if  _obs=="" and _exp==""  and _expd=="" and _expu=="" and _exp2d==""  and _exp2u=="":
         failedPoints.append(amass)
         continue
      i+=1
      print "i = ",i
      print "The computed upper limit for signal mass ", amass,  " is: ", _obs
      print "expected content", _exp
      print "expected content -1", _expd
      print "expected content +1", _expu
      print "expected content -2", _exp2d
      print "expected content +2", _exp2u

      if doNBSM or doBRttX:
         exp[i]=float(_exp)
         obs[i]=float(_obs)
         expd[i]=float(_expd)
         expu[i]=float(_expu)
         exp2d[i]=float(_exp2d)
         exp2u[i]=float(_exp2u)
         print "check obs", obs[i]
      elif doEffXsec:
         exp[i]=float(_exp)/36.07
         obs[i]=float(_obs)/36.07
         expd[i]=float(_expd)/36.07
         expu[i]=float(_expu)/36.07
         exp2d[i]=float(_exp2d)/36.07
         exp2u[i]=float(_exp2u)/36.07
      elif doBrHbbmm:
         exp[i]=float(_exp)*0.0016*0.1
         obs[i]=float(_obs)*0.0016*0.1
         expd[i]=float(_expd)*0.0016*0.1
         expu[i]=float(_expu)*0.0016*0.1
         exp2d[i]=float(_exp2d)*0.0016*0.1
         exp2u[i]=float(_exp2u)*0.0016*0.1
      print amass
      aMass[i]=float(amass)

      
#####################################


print "aMass",aMass,"\n"
print "obs", obs,"\n"
print "exp", exp,"\n"
#print "obs2", obs2,"\n"
#print "exp2", exp2,"\n"
print "expd", expd,"\n"
print "exp2d", exp2d,"\n"
print "expu", expu,"\n"
print "exp2u", exp2u,"\n"


Nfailed = len(failedPoints)

if Nfailed>0:
   print "!!!!!!!!!!!!!!!!!!!!       WARNING !!!!!!!!!!!!!!!!!!!!!!, some points failed, removing them from the list: ",failedPoints
   aMass=aMass[:-Nfailed]
   obs=obs[:-Nfailed]
   exp=exp[:-Nfailed]
   expd=expd[:-Nfailed]
   exp2d=exp2d[:-Nfailed]
   expu=expu[:-Nfailed]
   exp2u=exp2u[:-Nfailed]
   print "aMass",aMass,"\n"
   print "obs", obs,"\n"
   print "exp", exp,"\n"
   print "expd", expd,"\n"
   print "exp2d", exp2d,"\n"
   print "expu", expu,"\n"
   print "exp2u", exp2u,"\n"


if calcChi2:
   '''
   _file=open("UpperLimitTable_ModelIndependent.tex","r")
   lines = _file.readlines()
   i=0
   for mass in aMass:
      significance=-1.
      for line in lines:
         if "SR"+str(mass).split('.')[0] in line:
            print "for SR:","SR"+str(mass).split('.')[0],"\n line:",line.strip()
            significance=float(line.split('$(')[1].split(')$')[0])
            print "significance=",significance,"\n"
            break
         signif[i]=significance
         i+=1
         if significance<0:
            print "panic, couldn't find the significance for mass", mass
            
   print "signif", signif,"\n"
   print "obs", obs,"\n"
   print "exp", exp,"\n"
   '''
   chi2=0.
   for x in range(Npoints):
      #chi2 += ((obs[x]-exp[x])*(obs[x]-exp[x]))/(signif[x]*signif[x])
      if (obs[x]<exp[x]):
         chi2 += ((obs[x]-exp[x])*(obs[x]-exp[x]))/((exp[x]-expd[x])*(exp[x]-expd[x]))
      else:
         chi2 += ((obs[x]-exp[x])*(obs[x]-exp[x]))/((exp[x]-expu[x])*(exp[x]-expu[x]))
         
   print "chi2 = ", chi2
   print "NDF = ", Npoints 

_hexp = ROOT.TGraph(len(aMass),array('f', aMass),array('f', exp))
_hobs = ROOT.TGraph(len(aMass),array('f', aMass),array('f', obs))
_hexpup = ROOT.TGraph(len(aMass),array('f', aMass),array('f', expu))
_hexpdown = ROOT.TGraph(len(aMass),array('f', aMass),array('f', expd))
_hexp2up = ROOT.TGraph(len(aMass),array('f', aMass),array('f', exp2u))
_hexp2down = ROOT.TGraph(len(aMass),array('f', aMass),array('f', exp2d))


N = len(aMass)
grshade = ROOT.TGraph(2*N)
grshade2 = ROOT.TGraph(2*N)

for g in range(0,N):
   grshade.SetPoint(g,aMass[g],expu[g])
   grshade.SetPoint(N+g,aMass[N-g-1],expd[N-g-1])
   grshade2.SetPoint(g,aMass[g],exp2u[g])
   grshade2.SetPoint(N+g,aMass[N-g-1],exp2d[N-g-1])
   print "g",g," expu[g]",expu[g]," aMass[g]",aMass[g] 


c1 =ROOT.TCanvas( "c", "A scan of m_{a} versus UL on ttX", 0, 0,900, 700)
c1.SetLeftMargin(0.2)
#c1.SetLogy()
frame=ROOT.TH1F() 
if doNBSM:
   frame=c1.DrawFrame(18.,1.,62.,1000.)
   frame.SetYTitle("95% C.L. upper limits on N BSM")
   frame.GetYaxis().SetTitleSize( 0.04 )
elif doEffXsec:
   frame=c1.DrawFrame(18.,0.03,62.,10.)
   frame.SetYTitle("#sigma_{vis}(X) #times B(X #rightarrow bb#mu#mu) [fb]")
   frame.GetYaxis().SetTitleSize( 0.04 )
elif doBRttX:
   frame=c1.DrawFrame(20.,0.0,60.0,0.8) # (xmin,ymin,xmax,ymax) //ttX
   frame.SetYTitle("95% C.L. upper limits on #sigma (ttX) #times B(X #rightarrow #tau#tau) [pb]")
   frame.GetYaxis().SetTitleSize( 0.032 )
elif doBrHbbmm:
   frame=c1.DrawFrame(12.,0.00003,50.,0.0000005)
   frame.SetYTitle("#frac{#sigma_{H}}{#sigma_{SM}}#times B(H #rightarrow aa #rightarrow bb#mu#mu)")
   frame.GetYaxis().SetTitleSize( 0.04 )

if doBRttX or doBrHbbmm:
   frame.SetXTitle("m_{X} [GeV]")
else:
   frame.SetXTitle("m_{#mu#mu} [GeV]")

#frame.SetYTitle("#sigma [pb] * BR")
frame.GetXaxis().SetTitleOffset(1.2)
#frame.GetYaxis().SetTitleOffset(1.0)
frame.GetYaxis().SetTitleOffset(1.2)
frame.GetYaxis().CenterTitle(1)
if doBrHbbmm:
   frame.GetYaxis().SetTitleOffset(1.4)
frame.GetZaxis().SetTitleOffset(0.9)


frame.GetXaxis().SetTitleFont( 42 )
frame.GetYaxis().SetTitleFont( 42 )
frame.GetXaxis().SetLabelFont( 42 )
frame.GetYaxis().SetLabelFont( 42 )

frame.GetXaxis().SetTitleSize( 0.04 )


frame.GetXaxis().SetNdivisions( 410 )
frame.GetYaxis().SetNdivisions( 410 )

frame.GetXaxis().SetLabelSize( 0.035 )
frame.GetYaxis().SetLabelSize( 0.035 )
frame.GetZaxis().SetLabelSize( 0.015 )
frame.Draw()

ROOT.gStyle.SetPaintTextFormat("2.2f")
ROOT.gStyle.SetMarkerColor(14)
ROOT.gStyle.SetTextColor(14)


_hexp.SetLineColor(16)
_hexp.SetLineStyle(9)
_hexp.SetLineWidth(3)
_hexp.SetLineColor(c_myExp)
_hexp.SetLineStyle(9)
_hexp.SetLineWidth(3)
_hexp.SetMarkerStyle(20)
_hobs.SetMarkerStyle(20)
_hobs.SetMarkerSize(1.5)
_hobs.SetMarkerColor(c_LightRed)
_hobs.SetLineColor(c_LightRed)
_hobs.SetLineStyle(1)
_hobs.SetLineWidth(4)


_hexp2up.SetLineColor(c_LightYellow)
_hexp2up.SetFillColor(c_LightYellow)

#_hexp2down.SetFillColor(10)
_hexp2down.SetLineColor(c_LightYellow)
_hexp2down.SetFillColor(c_LightYellow)
#_hexp2down.SetFillColor(ROOT.kWhite )



_hexpup.SetLineColor(ROOT.kGreen)
_hexpup.SetFillColor(ROOT.kGreen)

_hexpdown.SetLineColor(ROOT.kGreen)
_hexpdown.SetFillColor(ROOT.kGreen)
#_hexpdown.SetFillColor(10)

#_hexp2up.Draw("LF:same")
#_hexp2down.Draw("LF:same")

grshade2.SetFillStyle(1001)
grshade2.SetFillColor(c_LightYellow)
grshade2.Draw("F:same")
grshade.SetFillStyle(1001)
grshade.SetFillColor(ROOT.kGreen)
grshade.Draw("F:same")

#this doesn't work, it draws both smoothed and non-smoothed !!???
#_hexpup.Draw("LC:same")
#_hexpdown.Draw("LC:same")


#this works
if doObs:  
  _hobs.Draw("C:same") #don't plot observed when running blinded
  _hobs.Draw("P:same") #toshow points
  #_hobs.Draw("L:same")
#_hexp.SetMarkerColor(2)
#_hexp.SetMarkerStyle(22)
#_hexp.SetMarkerSize(2)
#_hexp.Draw("CP:same")
_hexp.Draw("C:same")
_hexp.Draw("P:same")



Leg0 = ROOT.TLatex()
Leg0.SetNDC()
Leg0.SetTextAlign( 11 )
Leg0.SetTextFont( 42 )
Leg0.SetTextColor(ROOT.kBlack)
Leg0.SetTextSize(0.03)

atlasLabel = ROOT.TLatex()
atlasLabel.SetNDC()
atlasLabel.SetTextFont(72)
atlasLabel.SetTextColor(ROOT.kBlack)
atlasLabel.SetTextSize( 0.05 )
if doBrHbbmm or doEffXsec:
   atlasLabel.DrawLatex(0.23, 0.82,"ATLAS")
else:
   atlasLabel.DrawLatex(0.23, 0.82,"ATLAS")
   #DrawLuminosity(0.55,0.73,"139")
#atlasLabel.Draw("same")


#tex = ROOT.TLatex(0.30,0.85,"Internal")
tex = ROOT.TLatex(0.38,0.82,"Preliminary")
tex.SetNDC()
tex.SetTextFont(42)
tex.SetTextColor(ROOT.kBlack)
tex.SetTextSize(0.038)
tex.SetLineWidth(2)
tex.Draw()

clslimits = ROOT.TLatex()
clslimits.SetNDC()
clslimits.SetTextFont(42 )
clslimits.SetTextSize(0.032)
clslimits.SetTextColor( ROOT.TColor.GetColor(ROOT.kBlack) )
#clslimits.DrawLatex(0.54, 0.65, "All limits at 95% CL")
#clslimits.Draw("same")

Leg1 = ROOT.TLatex()
Leg1.SetNDC()
#Leg1.SetTextFont(72 )
Leg1.SetTextSize( 0.04 )
Leg1.SetTextColor( ROOT.kBlack )
Leg1.DrawLatex(0.23, 0.705, "ttX #rightarrow #tau#tau")
Leg1.Draw("same")

#Legend = ROOT.TLegend(0.5,0.65,0.86,0.88)
Legend = ROOT.TLegend(0.6,0.57,0.89,0.88)
Legend.SetFillColor(0)
Legend.SetLineColor(ROOT.kWhite)
#Legend.SetTextSize( 0.025 )
Legend.SetTextSize( 0.032 )
Legend.SetTextFont( 42 )#
#Legend.AddEntry(_hobs,"observed 95% CL upper limit","l") #orig #comment out for blinded
#Legend.AddEntry(_hexp,"expected 95% CL upper limit","l") #orig 
Legend.AddEntry(_hobs,"Observed ","l") #comment out for blinded
Legend.AddEntry(_hexp,"Expected ","l")
Legend.AddEntry(_hexpup,"Expected #pm 1#sigma","f")
Legend.AddEntry(_hexp2up,"Expected #pm 2#sigma","f")
#Legend.AddEntry(_hexp2,"exp: simplified setup","l")
#Legend.AddEntry(_hobs2,"obs: simplified setup","l")
#Legend.AddEntry(_hexpup2,"expected #pm 1#sigma simplif","l")
#Legend.AddEntry(_hexp2up2,"expected #pm 2#sigma simplif","l")
Legend.Draw("same")



Leg1 = ROOT.TLatex()
Leg1.SetNDC()
Leg1.SetTextFont(42)
Leg1.SetTextSize( 0.038 )
Leg1.SetTextColor( ROOT.kBlack )
if doBrHbbmm or doEffXsec:
   Leg1.DrawLatex(0.23, 0.74, "#sqrt{s}=13 TeV, 139 fb^{-1}")
else:
   Leg1.DrawLatex(0.23, 0.76, "#sqrt{s} = 13 TeV, 44.3 fb^{-1}")
   #Leg1.DrawLatex(0.23, 0.65, "e#mu#mu")
	
Leg1.Draw("same")

frame.Draw( "sameaxis" )
if doNBSM:
   c1.SaveAs("ULresults/ModelIndepUL_NBSMeve.pdf")
elif doEffXsec:
   c1.SaveAs("ULresults/ModelIndepUL_sigmaXbr.pdf")
elif doBrHbbmm:
   c1.SaveAs("FirstLimitPlot.pdf")
elif doBRttX:
   if doObs: 
    c1.SaveAs(dirname + "limitplots_obs.pdf")
   else:
    c1.SaveAs(dirname + "limitplots.pdf")



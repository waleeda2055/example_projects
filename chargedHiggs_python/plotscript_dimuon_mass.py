# To run the new HistFitter I had to change the Makefile
# L34: -std=c++11 -> -std=c++1y
# Then in your HF config file, change all instances of setFileList -> addInputs
# that should be it!

import ROOT
from ROOT import *

#from PythonHelpers import GetHistograms
from AtlasStyle import *
from array import *

#SetAtlasStyle()

# Get signal histogram from file depending on file name
def AddSignal(_file, region="SR",var="dimuon"):

    #masses=["a60","a50","a40","a30","a20"]
    masses=["H160a45"]

    #colors = [ROOT.kBlack,ROOT.kMagenta+1,ROOT.kRed+1,ROOT.kOrange+2,ROOT.kViolet+5]
    colors = [ROOT.kBlack,ROOT.kMagenta+2,ROOT.kRed+1,ROOT.kOrange+4,ROOT.kBlue+2]
    style = [9,2,7,5,1]
    
    if region == "SR":
        cut="(( (mu2Pt/mu1Pt > 0.20 || Mmu1e_ss>-100  ) && dimuon_m >= 12  && dimuon_m <= 77 && isOS ==1 && nElectrons_signal == 1 && nMuons_signal == 2 && nJet20 >=3 && nBjets_77 >=1 && elec1Pt >= 20 ))*event_weight*.03"#0.16%  
    elif region == "TCR":
        cut ="(klbbmm_m>110 && klbbmm_m<140 && met>60 && dimuon_m>16 && dimuon_m<64 && klfit_loglikelihood>-8)*event_weight*36100*12.5"
    elif region == "SB":
        cut ="(klbbmm_m > 80 && (klbbmm_m<110 || klbbmm_m>140) && klbbmm_m<170 && met<60 && dimuon_m>16 && dimuon_m<64 && klfit_loglikelihood>-8)*event_weight*36100*12.5"#*0.02
    elif region == "VRmet":
        #cut ="(klbbmm_m>80  && met>0 && dimuon_m>16 && dimuon_m<64 && klfit_loglikelihood>-8)*event_weight*36100*6.25"#*0.01
        cut ="(met>0 && dimuon_m>16 && dimuon_m<64 && klfit_loglikelihood>-8)*event_weight*36100*6.25*0.5"#*0.01 *0.5 = 0.005
    elif region == "VRMbbmm":
        cut ="(klbbmm_m>80 && klbbmm_m<300 && met<60 && dimuon_m>16 && dimuon_m<64 && klfit_loglikelihood>-8)*event_weight*36100*1* 0.9375" #* 0.9375 = 0.0015
    elif region == "VR1":
        cut ="(klbbmm_m>170 && klbbmm_m<300 && met<60 && dimuon_m>16 && dimuon_m<64 && klfit_loglikelihood>-8)*event_weight*36100*12.5"
    elif region == "VRlog":
        cut ="(klbbmm_m>110 && klbbmm_m<140 && met<60 && dimuon_m>16 && dimuon_m<64 && klfit_loglikelihood>-11 && klfit_loglikelihood<-8)*event_weight*36100*1"

    print region 
    Nbins=1
    Nmin=0
    Nmax=1

    if var=="dimuon":
        var="dimuon_m"
        #Nbins=34
        #Nmin=16
        #Nmax=84
        Nbins=25
        Nmin=15
        Nmax=75
    if var=="met":
        Nbins=20
        Nmin=0
        Nmax=200
    if var=="klbbmm":
        var="klbbmm_m"
        Nbins=22
        Nmin=80
        Nmax=300
    if var=="klbb":
        var="klbb_m"
        Nbins=50
        Nmin=2.5
        Nmax= 102.5
    if var=="klfit":
        var="klfit_loglikelihood"
        Nbins=22
        Nmin=-11
        Nmax=0

    stoKamo=var+">>h"
    h_signal = []
    print _file
    print region
    rootfile = ROOT.TFile.Open(_file)

    cc=4
    for ma in masses:
        tree = rootfile.Get(ma+"__Nominal")
        print ma+"__Nominal"
        print tree
        h = ROOT.TH1F("h","h",Nbins, Nmin,  Nmax)
        tree.Draw(stoKamo,cut)
        h.SetDirectory(0)
        h.SetLineColor(colors[cc])
        #h.SetLineStyle(style[cc])
        h.SetLineWidth(2)
        if cc==4:
            h.SetLineWidth(2)
        cc+=1
        h_signal.append(h)

    return h_signal

# Main plotting function
# The x-axis label is done automatically from the input file name
# my y max and min defaults can be overridden in the argument
def BeautifyPlot(_file, xmin, xmax, ymin=-1, ymax=-1):
    if not ("TCR"  in _file or "VR1" in _file or "SB"  in _file or "VRlog" in _file):
        #h_signal = AddSignal("../MyFiles/v28/Oct3/signal.root",_file.split("/")[2].split("_")[0],_file.split("/")[2].split("_")[1])
        #h_signal = AddSignal("/eos/home-w/waahmed/afs_runs/new_ptag/sig/2020/mar18/mc16a/H140a30/H140a30_16a.root",_file.split("/")[3].split("_")[0],_file.split("/")[3].split("_")[1]) 
        h_signal = AddSignal("/eos/home-w/waahmed/afs_runs/new_ptag/sig/2020/aug7/H160a45_run2Nom.root",_file.split("/")[3].split("_")[0],_file.split("/")[3].split("_")[1])
    
             
    canvas = ROOT.TCanvas("canvas", "canvas", 1000, 800)
    #canvas = ROOT.TCanvas("canvas", "canvas", 800, 800)

    pad1= ROOT.TPad("pad1", "pad1", 0, 0.35, 1, 1.0)
    #pad1.SetBottomMargin(0.005)
    pad1.SetBottomMargin(0.08)
    pad1.SetTicks()
    pad1.Draw()
    pad1.SetLogy(0)
    pad1.cd()
    ROOT.gStyle.SetOptStat(0)
    #ROOT.gStyle.SetLegendTextSize(0.01) # doestn work

    rootfile = ROOT.TFile.Open(_file)
    #histos = GetHistograms(_file)

    stack = ROOT.THStack("stack", "")

    h_total_error_band = rootfile.Get("h_total_error_band")
    h_obsData = rootfile.Get("h_obsData")

    h_SM_total = rootfile.Get("SM_total")
    #h_SM_total.SetLineColor(ROOT.kRed+3)
    h_SM_total.SetLineWidth(2)
    #h_SM_total.SetFillStyle(3005)   
 
    xdata = array('d', [])
    ydata = array('d', [])
    ylow = array('d', [])
    yhigh = array('d', [])
    zeros = array('d', [])

    # Data points are in a tgraph like object, loop through and retrieve them and their errors
    for i in range(h_obsData.GetXaxis().GetNbins()):

        #x=[0 for x in range(h_obsData.GetXaxis().GetNbins())]
        #y=[0 for y in range(h_obsData.GetXaxis().GetNbins())]
        x, y = ROOT.Double(0), ROOT.Double(0)


        h_obsData.GetPoint(i, x, y)

        # just so they don't show up in the plot on the axis
        if y == 0:
            
            y += 10**10

        ylow.append(h_obsData.GetErrorYlow(i))
        yhigh.append(h_obsData.GetErrorYhigh(i))

        xdata.append(x)
        ydata.append(y)
        zeros.append(0)

    data = ROOT.TGraphAsymmErrors(h_obsData.GetXaxis().GetNbins(), xdata, ydata, zeros, zeros, ylow, yhigh)

    errorDict = {}

    # Do the same for the error bands. These are stored as rectangles of points
    # Individually get the x midpoints, x +/- and y +/- and use TGraphErrors
    for i in range(h_total_error_band.GetXaxis().GetNbins()):

        xtmp, ytmp = ROOT.Double(0), ROOT.Double(0)

        h_total_error_band.GetPoint(i, xtmp, ytmp)    

        leq = 0.

        # Get the binning
        for i in range(len(xdata)):            

            #if xdata[i] > 0 and xdata[i+1] > 0:
            if (xdata[i] > 0 and xdata[i+1] > 0) or (xdata[i] < 0 and xdata[i+1] < 0):

                leq = (xdata[i+1] - xdata[i]) / 2.

                break

        # points aren't always arranged logically so find the point that corresponds and store yerror in dict
        for x in xdata:

            if (x > 0 and xtmp > 0) or (x < 0 and xtmp < 0):

                if abs(xtmp - x) < leq:                

                    if not x in errorDict:

                        errorDict[x] = []
                        errorDict[x].append(ytmp)

                    else:

                        if ytmp not in errorDict[x]:

                            errorDict[x].append(ytmp)
                
    xerr = array('d', [])
    yerr = array('d', [])

    # now fill x,y error arrays from dict
    for key in xdata:

        if key == 0.0:
            continue
        xerr.append((xdata[1] - xdata[0])/2)
        if len(errorDict[key]) < 2:
            yerr.append(errorDict[key][0])
        else:
            yerr.append(abs(errorDict[key][1] - errorDict[key][0])/2)

    for i in range(h_SM_total.GetXaxis().GetNbins()):

        ydata[i] = h_SM_total.GetBinContent(i+1)

    #print xdata
    #print ydata
    #print xerr
    #print yerr

    total_unc = ROOT.TGraphErrors(len(xdata), xdata, ydata, xerr, yerr)

    data.SetMarkerStyle(20)
    data.SetMarkerSize(1.5)

    ttV = rootfile.Get("ttZ_")
    st = rootfile.Get("singleTop_")
    db = rootfile.Get("diBoson_")
    tt = rootfile.Get("ttbar_")
    #DDZ = rootfile.Get("DDZ_")
    zjets = rootfile.Get("Zjets_")
    zjetsO = rootfile.Get("ZjetsO_")
    ttW = rootfile.Get("ttW_")
    Wjets = rootfile.Get("Wjets_")

    #if sample == "ttbar": return TColor.GetColor("#8bd3fb")
    #if sample == "DDZ": return kAzure + 8
    #if sample == "singleTop": return TColor.GetColor("#34992f")
    #if sample == "diBoson": return kGreen-10
    #if sample == "ttZ": return TColor.GetColor("#ff839b")
    #if sample == "Zjets": return TColor.GetColor("#ffae32")
    #if sample == "ZjetsO": return TColor.GetColor("#357478")
    #if sample == "ttW": return kYellow+1
    #if sample == "Wjets": return TColor.GetColor("#785d9f")

   
    #DDZ.SetLineColor(ROOT.kCyan+1)
    #DDZ.SetLineColor(ROOT.kAzure+8)
    #DDZ.SetFillColor(ROOT.kAzure+8)
    tt.SetLineColor(TColor.GetColor("#8bd3fb"))
    tt.SetFillColor(TColor.GetColor("#8bd3fb"))
    #tt.SetLineColor(ROOT.kOrange)
    #tt.SetFillColor(ROOT.kOrange)
    #tt.SetLineColor(ROOT.TColor(7005 , 240/255. , 228/255. ,  66/255.))
    #tt.SetFillColor(ROOT.TColor(7005,0.941176,0.894118,0.258824))
    db.SetLineColor(ROOT.kGreen-10)
    db.SetFillColor(ROOT.kGreen-10)
    st.SetLineColor(TColor.GetColor("#9559b5"))
    st.SetFillColor(TColor.GetColor("#9559b5"))
    ttV.SetLineColor(TColor.GetColor("#ff839b"))
    ttV.SetFillColor(TColor.GetColor("#ff839b"))
    zjets.SetLineColor(TColor.GetColor("#ffae32"))
    zjets.SetFillColor(TColor.GetColor("#ffae32")) 
    zjetsO.SetLineColor(TColor.GetColor("#357478"))
    zjetsO.SetFillColor(TColor.GetColor("#357478"))
    ttW.SetLineColor(ROOT.kYellow+1)
    ttW.SetFillColor(ROOT.kYellow+1)
    Wjets.SetLineColor(TColor.GetColor("#785d9f"))
    Wjets.SetFillColor(TColor.GetColor("#785d9f"))

      
    if not "SR" in _file and not "VRLL" in _file and not "VRlog" in _file:
        W = rootfile.Get("Wjets_")
        W.SetLineColor(ROOT.kTeal-6)
        W.SetFillColor(ROOT.kTeal-6)

    
    #if not "SR" in _file and not "VRLL" in _file and not "VRlog" in _file:
    #    stack.Add(W)
    stack.Add(ttV)
    stack.Add(ttW)     
    stack.Add(zjetsO)
    stack.Add(st)
    stack.Add(Wjets)
    stack.Add(db)
    stack.Add(zjets)
    stack.Add(tt)
    


    '''
    # cosmetics
    if not "CR" in _file:

        Other = rootfile.Get("Other")
        Other.SetLineColor(ROOT.kBlue-9)
        Other.SetFillColor(ROOT.kBlue-9)
        stack.Add(Other)        
        
        Diboson = rootfile.Get("Diboson")
        Diboson.SetLineColor(ROOT.kGray+2)
        Diboson.SetFillColor(ROOT.kGray+2)
        stack.Add(Diboson)

        Wmunu = rootfile.Get("Wmunu")
        Wenu = rootfile.Get("Wenu")
        Wtaunu_ISR = rootfile.Get("Wtaunu_ISR")
        Wtaunu_TauFake = rootfile.Get("Wtaunu_TauFake")

        Wjets = Wenu.Clone()
        Wjets.Add(Wmunu)
        Wjets.Add(Wtaunu_ISR)
        Wjets.Add(Wtaunu_TauFake)
        
        ttbar = rootfile.Get("ttbar")
        singletop = rootfile.Get("singletop")
        ttbar.Add(singletop)
        ttbar.SetLineColor(ROOT.kGreen-2)
        ttbar.SetFillColor(ROOT.kGreen-2)
        stack.Add(ttbar)

        Wjets = Wjets
        Wjets.SetLineColor(ROOT.kMagenta-1)
        Wjets.SetFillColor(ROOT.kMagenta-1)
        stack.Add(Wjets)        

        Znunu = rootfile.Get("Znunu")
        Zjets = Znunu.Clone()

        Zjets = Zjets
        Zjets.SetLineColor(ROOT.kOrange-3)
        Zjets.SetFillColor(ROOT.kOrange-3)
        stack.Add(Zjets)        
    '''

    leg = ROOT.TLegend(0.67, 0.31, 0.995, 0.86)
    if "met" in _file:
        leg = ROOT.TLegend(0.64, 0.3, 0.97, 0.86)
    if "SR_" in _file:
        #leg = ROOT.TLegend(0.67, 0.2, 0.995, 0.86)
        #leg = ROOT.TLegend(0.66, 0.18, 0.93, 0.86)
        # leg = ROOT.TLegend(0.68,0.475,0.90,.89,"") #top right
        leg = ROOT.TLegend(0.13,0.43,0.38,.87) #top left
    if "VRLL_" in _file:
        leg = ROOT.TLegend(0.67, 0.31, 0.93, 0.86)
    leg.SetBorderSize(0);
    leg.SetTextSize(0.038);
    leg.SetLineColor(1);
    leg.SetLineStyle(1);
    leg.SetLineWidth(1);
    leg.SetFillColor(0);
    leg.SetFillStyle(0);
    entry = leg.AddEntry("NULL","Data","p");
    entry.SetLineColor(1);
    entry.SetLineStyle(1);
    entry.SetLineWidth(1);
    entry.SetMarkerColor(1);
    entry.SetMarkerStyle(20);
    entry.SetMarkerSize(1);
    entry.SetTextFont(42);
    #h_SM_total.SetLineColor(ROOT.kAzure+1)
    h_SM_total.SetLineColor(TColor.GetColor("#8bd3fb"))
    htmp = h_SM_total.Clone()
    #htmp.SetFillStyle(3354)
    htmp.SetFillStyle(3004)
    htmp.SetFillColor(ROOT.kBlack)
    #entry = leg.AddEntry(htmp,"SM total","fl")
    entry.SetLineColor(ROOT.kRed+3)
    entry.SetLineWidth(2)
    leg.AddEntry(h_signal[0],"H^{+} #rightarrow WA, m_{H^{+}} = 160 GeV, m_{A} = 45 GeV","l")
    entry = leg.AddEntry(htmp,"Uncertainty","fl")
    #leg.AddEntry(DDZ, "DY + jets", "f")        
    leg.AddEntry(tt, "t#bar{t}", "f")
    #if not "SR" in _file and not "VRLL" in _file and not "VRlog" in _file:
        #leg.AddEntry(W, "W+jets", "f")
    leg.AddEntry(zjets, "Z+HF", "f")
    leg.AddEntry(db, "Diboson", "f")
    leg.AddEntry(st, "Single top-quark", "f")
    leg.AddEntry(zjetsO, "Z+LF", "f")
    #leg.AddEntry(ttW, "ttW", "f") 
    #leg.AddEntry(ttV, "t#bar{t}Z", "f")
    #leg.AddEntry(st, "Single top-quark", "f")
    leg.AddEntry(ttW, "t#bar{t}W", "f")
    leg.AddEntry(ttV, "t#bar{t}Z", "f")

 
    #if not ("TCR"  in _file or "VR1" in _file or "SB" in _file  or "VRlog" in _file):
    #    if "SR" in _file:
    #        leg.AddEntry(h_signal[0],"H^{+}160, A45","l")
            #leg.AddEntry(h_signal[1],"m_{a}=50 GeV","l")
        #leg.AddEntry(h_signal[2],"m_{a}=40 GeV","l")
        #if "SR" in _file:
            #leg.AddEntry(h_signal[3],"m_{a}=30 GeV","l")
            #leg.AddEntry(h_signal[4],"m_{a}=20 GeV","l")
        #entry = leg.AddEntry("NULL","Br(H #rightarrow aa #rightarrow bb#mu#mu)=0.08%","")
        #if "met"  in _file:
        #    entry = leg.AddEntry("NULL","(B=0.5%)","")
        #elif "klbbmm" in _file:
        #    entry = leg.AddEntry("NULL","(B=0.15%)","")
        #elif "VRLL" in _file:
        #    entry = leg.AddEntry("NULL","(B=0.1%)","")
        #else:
        #    entry = leg.AddEntry("NULL","(B=0.04%)","")
        
        #if "met"  in _file:
        #    entry = leg.AddEntry("NULL","(x 0.005)","")
        #elif "klbbmm" in _file:
        #    entry = leg.AddEntry("NULL","(x 0.0015)","")
        #elif "VRLL" in _file:
        #    entry = leg.AddEntry("NULL","(x 0.001)","")
        #else:
        #    entry = leg.AddEntry("NULL","(x 0.0004)","")
            
  
    
    stack.Draw("HIST")
    #print "h_SM_total.GetXaxis()->GetXmin()== ",h_SM_total.GetXaxis().GetXmin(), "h_SM_total.GetXaxis()->GetXmax()=",h_SM_total.GetXaxis().GetXmax()

    if "dimuon_m" in _file:
        #xmin=16
        #xmax=84
        xmin=15
        xmax=75
    elif "klbb_m" in _file:
        xmin=12.5
        xmax=87.5
        stack.SetMaximum(38)
    elif "likelihood" in _file:
        xmin=-11
        xmax=-1
    else:
        xmin=h_SM_total.GetXaxis().GetXmin()
        xmax=h_SM_total.GetXaxis().GetXmax()

    stack.GetXaxis().SetRangeUser(xmin, xmax)
    leg.Draw()
    h_SM_total.Draw("HIST SAME")

    # Override these by adding optional argument for ymax
    if ymax > 0:
        stack.SetMaximum(ymax)
    if ymin > 0:
        stack.SetMinimum(ymin)

    stack.SetMaximum(stack.GetMaximum()*2.3)
    if "SB_" in _file or "VR1" in _file or "likelihood" in _file:
        stack.SetMaximum(stack.GetMaximum()*1.4)

    stack.GetXaxis().SetLabelFont(42)
    stack.GetXaxis().SetLabelSize(0)
    stack.GetXaxis().SetTitleSize(0.05)
    stack.GetXaxis().SetTitleFont(42)
    stack.GetYaxis().SetLabelFont(42)
    #stack.GetYaxis().SetLabelSize(0.055)
    stack.GetYaxis().SetLabelSize(0.05)
    stack.GetYaxis().SetLabelOffset(0.009)
    stack.GetYaxis().SetTitleSize(0.05)
    stack.GetYaxis().SetTitleOffset(0.70)
    stack.GetYaxis().SetTitleFont(42)
    if "met" in _file or "VRMbbmm" in _file or "VR1_dimuon_m" in _file or "SB_dimuon_m" in _file:
        stack.GetYaxis().SetLabelSize(0.05)
        stack.GetYaxis().SetLabelOffset(0.005)
    # leq is half the bin width
    if "likelihood" in _file:
        stack.GetYaxis().SetTitle("Events / "+str(2*leq).replace(".0", ""))
    else:
        stack.GetYaxis().SetTitle("Events / "+str(2*leq).replace(".0", "")+" GeV")

    data.Draw("P SAME")
    total_unc.SetFillColor(ROOT.kBlack)
    #total_unc.SetFillStyle(3001)
    #total_unc.SetFillStyle(3354)
    total_unc.SetFillStyle(3004)
    #h_total_error_band.SetFillStyle(3554)
    total_unc.Draw("2 SAME")
    h_total_error_band.Draw("p SAME")

    
    for s in range(0,1):
        if not "SR" in _file and (s==0 or s==1 or s==4 or s==3):
            continue
        if "TCR"  in _file or "VRlog" in _file or "SB"  in _file or "VR1" in _file:
            continue
        h_signal[s].Draw("HIST SAME")

    '''
    # only add signal for signal regions
    if "SR_" in _file:
        h_signal = AddSignal(_file)                
        h_signal.Draw("HIST SAME")
        h_signal.SetLineStyle(2)
        h_signal.SetLineWidth(4)
        if "SR1" in _file or "SR2"  in _file:
            leg.AddEntry(h_signal, "#tilde{t}_{1}("+h_signal.GetTitle().split("_")[0]+"), #tilde{#chi}_{1}^{0}("+h_signal.GetTitle().split("_")[1]+")", "l") 
        else:
            leg.AddEntry(h_signal, "#tilde{c}_{1}("+h_signal.GetTitle().split("_")[0]+"), #tilde{#chi}_{1}^{0}("+h_signal.GetTitle().split("_")[1]+")", "l") 
    '''
    #top left 
    #AtlasLabel(0.15,0.81, ROOT.kBlack)
    #DrawLuminosity(0.15,0.71,"139")
    #to right
    AtlasLabel(0.55,0.80, ROOT.kBlack)
    DrawLuminosity(0.543,0.73,"139")
    #AtlasLumiLabel(0.15,0.8,"36.1")
    if "SR" in _file:            
        #DrawText(0.15, 0.61, "SR")
        #DrawText(0.70, 0.80, "Internal")
        DrawText(0.70, 0.80, "Preliminary")
    if "VRlog" in _file:            
        DrawText(0.15, 0.61, "VR2")
    if "VR1" in _file:            
        DrawText(0.15, 0.61, "VR1")
    if "TCR" in _file:            
        DrawText(0.15, 0.61, "Top CR")
    if "SB" in _file:            
        DrawText(0.15, 0.61, "DYCR")
    if "VRMbbmm" in _file:            
        #DrawText(0.15, 0.65, "DYCR+SR+VR1")
        DrawText(0.12, 0.56, "DYCR")
        DrawText(0.245, 0.56, "SR")
        DrawText(0.33, 0.56, "DYCR")
        DrawText(0.5, 0.56, "VR1")
        line1 = ROOT.TLine(110,0,110,190)
        line1.SetLineWidth(2)
        line1.SetLineStyle(2)
        line1.Draw("same")
        line2 = ROOT.TLine(140,0,140,190)
        line2.SetLineWidth(2)
        line2.SetLineStyle(2)
        line2.Draw("same")
        line3 = ROOT.TLine(170,0,170,190)
        line3.SetLineWidth(2)
        line3.SetLineStyle(2)
        line3.Draw("same")
    if "VRmet" in _file:            
        DrawText(0.15, 0.6, "preselection+ln(#it{L}^{max})>-8")
    if "VRLL" in _file:            
        #DrawText(0.15, 0.65, "SR+VR2")
        DrawText(0.2, 0.56, "VR2")
        DrawText(0.4, 0.56, "SR")
        line1 = ROOT.TLine(-8,0,-8,60)
        line1.SetLineWidth(2)
        line1.SetLineStyle(2)
        line1.Draw("same")



    canvas.cd()
    
    #pad2 = ROOT.TPad("pad2", "pad2", 0, 0.1, 1, 0.34)
    pad2 = ROOT.TPad("pad2", "pad2", 0, 0.075, 1, 0.388)
    #pad2.SetTopMargin(0.035)
    pad2.SetTopMargin(0.03)
    pad2.SetBottomMargin(0.35)
    pad2.SetGridy()
    pad2.SetTicks()
    if "klbbmm" in _file:
        pad2.SetBottomMargin(0.4)
    pad2.Draw()
    pad2.cd()

    h_rel_error_band = rootfile.Get("h_rel_error_band")    
    h_ratio = rootfile.Get("h_ratio")

    xdata = array('d', [])
    ydata = array('d', [])
    ylow = array('d', [])
    yhigh = array('d', [])
    zeros = array('d', [])

    for i in range(h_ratio.GetXaxis().GetNbins()):

        x, y = ROOT.Double(0), ROOT.Double(0)

        h_ratio.GetPoint(i, x, y)    
        #if x==0.0  and ("_met_" in _file or "likelihood" in _file):
        if x==0.0  and "_met_" in _file:
            continue

        ylow.append(h_ratio.GetErrorYlow(i))
        yhigh.append(h_ratio.GetErrorYhigh(i))

        xdata.append(x)
        ydata.append(y)
        zeros.append(0)


    #dataMC = ROOT.TGraphAsymmErrors(h_ratio.GetXaxis().GetNbins(), xdata, ydata, zeros, zeros, ylow, yhigh)
    dataMC = ROOT.TGraphAsymmErrors(len(xdata), xdata, ydata, zeros, zeros, ylow, yhigh)

    # this doesnt work, I dont know wy
    if "_met_" in _file:
        dataMC.RemovePoint(-1)
        print "h_ratio.GetXaxis().GetNbins()",h_ratio.GetXaxis().GetNbins()
        for ii in range(len(xdata)):
            if xdata[ii]==0.0:
                dataMC.RemovePoint(ii)

    xdata = array('d', [])

    for i in range(h_SM_total.GetXaxis().GetNbins()):

        xdata.append(h_SM_total.GetBinCenter(i+1))

    errorDict = {}

    for i in range(h_rel_error_band.GetXaxis().GetNbins()):

        xtmp, ytmp = ROOT.Double(0), ROOT.Double(0)

        h_rel_error_band.GetPoint(i, xtmp, ytmp)
        
        leq = 0.

        for i in range(len(xdata)):            

            if (xdata[i] > 0 and xdata[i+1] > 0) or (xdata[i] < 0 and xdata[i+1] < 0):

                leq = (xdata[i+1] - xdata[i]) / 2.

                break

        for i in range(h_SM_total.GetXaxis().GetNbins()):

            x = h_SM_total.GetBinCenter(i+1)            

            if ytmp > 0:

                if abs(xtmp - x) < leq:
                
                    if not x in errorDict:

                        errorDict[x] = []
                        errorDict[x].append(ytmp)

                    else:

                        if ytmp not in errorDict[x]:

                            errorDict[x].append(ytmp)
                
    yerr = array('d', [])

    upToCut = False

    for i in range(h_SM_total.GetXaxis().GetNbins()):

        if h_SM_total.GetBinContent(i+1) > 0:

            upToCut = True

        if not upToCut:

            yerr.append(0.0)            

    for key in xdata:

        if not key in errorDict:
            continue
        xerr.append((xdata[1] - xdata[0])/2)

        if len(errorDict[key]) > 1:
            yerr.append(abs(errorDict[key][1] - errorDict[key][0])/2)
        else:
            yerr.append(errorDict[key][0])

    #for i in range(h_ratio.GetXaxis().GetNbins()):
    for i in range(len(xdata)):

        ydata[i] = 1.0

    #print "len(yerr)", len(yerr), "len(xerr)",len(xerr)

    for i in range(len(yerr), len(xerr)):

        xerr[i] = 0
    #this is the error band on the ratio
    rel_unc = ROOT.TGraphErrors(len(xdata), xdata, ydata, xerr, yerr)


    #dataMC is the ratio
    dataMC.SetMarkerStyle(20)
    dataMC.SetMarkerSize(1.5)

    # Axis labels depend on input file name
    if "met" in _file:            
        dataMC.GetXaxis().SetTitle("E_{T}^{miss} [GeV]")
    if "klbbmm_m" in _file:            
        dataMC.GetXaxis().SetTitle("m_{bb#mu#mu}^{KL} [GeV]")
    if "klfit_loglikelihood" in _file:            
        dataMC.GetXaxis().SetTitle("ln(#it{L}^{max})")
    if "klbb_m" in _file:            
        dataMC.GetXaxis().SetTitle("m_{bb}^{KL} [GeV]")
    if "dimuon_m" in _file:
        dataMC.GetXaxis().SetTitle("m_{#mu#mu} [GeV]")
        #dataMC.GetXaxis().SetRangeUser(12, 90)

    
    dataMC.GetXaxis().SetLimits(xmin, xmax)
    dataMC.GetXaxis().SetLabelFont(42)
    dataMC.GetXaxis().SetLabelOffset(0.03)
    dataMC.GetXaxis().SetLabelSize(0.15)
    dataMC.GetXaxis().SetTitleSize(0.15)
    dataMC.GetXaxis().SetTitleOffset(1.1)
    dataMC.GetXaxis().SetTickLength(0.06)
    dataMC.GetXaxis().SetTitleFont(42)
    dataMC.GetYaxis().SetTitle("Data / SM")
    dataMC.GetYaxis().CenterTitle(True)
    #dataMC.GetYaxis().SetNdivisions(504)
    #dataMC.GetYaxis().SetNdivisions(506)
    dataMC.GetYaxis().SetNdivisions(504)
    dataMC.GetYaxis().SetLabelFont(42)
    dataMC.GetYaxis().SetLabelOffset(0.01)
    dataMC.GetYaxis().SetLabelSize(0.10)
    #dataMC.GetYaxis().SetTitleSize(0.17)
    dataMC.GetYaxis().SetTitleSize(0.10)
    if "_kl" in _file or "met" in _file:
        dataMC.GetYaxis().SetLabelSize(0.1)
    dataMC.GetYaxis().SetTitleOffset(0.25)
    dataMC.GetYaxis().SetTitleFont(42)
    dataMC.SetMinimum(0)
    dataMC.SetMaximum(2.2)
    dataMC.SetTitle("")
    if "met" in _file or "VRMbbmm" in _file:
        dataMC.GetYaxis().SetNdivisions(505)
        dataMC.SetMinimum(0.5)
        dataMC.SetMaximum(1.5)


    dataMC.Draw()
    dataMC.Draw("AP0")
    if "dimuon_m" in _file or "SR_kl" in _file:
        linea = ROOT.TLine(15,1.5,75,1.5)
        linea.SetLineWidth(1)
        linea.SetLineStyle(3)
        linea.Draw("same")
        lineb = ROOT.TLine(15,0.5,75,0.5)
        lineb.SetLineWidth(1)
        lineb.SetLineStyle(3)
        lineb.Draw("same")

    rel_unc.SetFillColor(ROOT.kBlack)
    #rel_unc.SetFillStyle(3001)
    #rel_unc.SetFillStyle(3354)
    rel_unc.SetFillStyle(3004)
    rel_unc.Draw("2 SAME")
    canvas.SaveAs("tutorial/"+_file.split("/")[3].replace(".root", ".pdf"))


#plotList = ["SR_dimuon_m_afterFit.root","SR_klbb_m_afterFit.root", "TCR_dimuon_m_afterFit.root", "VR1_dimuon_m_afterFit.root", "VRLL_klfit_loglikelihood_afterFit.root", "VRMbbmm_klbbmm_m_afterFit.root", "VRlog_dimuon_m_afterFit.root", "VRmet_klbbmm_m_afterFit.root", "VRmet_met_afterFit.root","SB_dimuon_m_afterFit.root"]
#plotList = ["SR_dimuon_m_afterFit.root","SR_klbb_m_afterFit.root", "VRLL_klfit_loglikelihood_afterFit.root", "VRMbbmm_klbbmm_m_afterFit.root", "VRlog_dimuon_m_afterFit.root", "VRmet_klbbmm_m_afterFit.root", "VRmet_met_afterFit.root"]

plotList = ["SR_dimuon_m_afterFit_3GeV.root"]

for plot in plotList:
    #BeautifyPlot("results/BkgOnly_Oct3_v28_FinalPlots_newHF_removeDDZrew/"+plot, 0, 120)
    BeautifyPlot("tutorial/results/MyConfigHplus_aug4_bkgonly_bbmumuplots/"+plot, 0, 120) 
    #BeautifyPlot("results/BkgOnly_Oct3_v28_FinalPlots_newHF/"+plot, 0, 120)
'''
SRfolders = ["StopCharm_SR1_36.1fb", "StopCharm_SR2_36.1fb", "StopCharm_SR3_36.1fb", "StopCharm_SR4_36.1fb", "StopCharm_SR17_36.1fb"]

SRs = ["SR"+str(i+1) for i in range(4)]
SRs += ["SR17"]

for SR in SRs:

    BeautifyPlot("HF_ConfigAndTrees/results/StopCharm_"+SR+"_36.1fb/CR_Ttbar_met_afterFit.root", 250, 800)
    BeautifyPlot("HF_ConfigAndTrees/results/StopCharm_"+SR+"_36.1fb/CR_Zll_met_afterFit.root", 250, 800)
    BeautifyPlot("HF_ConfigAndTrees/results/StopCharm_"+SR+"_36.1fb/CR_Wlnu_met_afterFit.root", 250, 800)
    if int(SR[2:]) < 3:
        BeautifyPlot("HF_ConfigAndTrees/results/StopCharm_"+SR+"_36.1fb/SR_met_afterFit.root", 500, 1200)
    else:
        BeautifyPlot("HF_ConfigAndTrees/results/StopCharm_"+SR+"_36.1fb/SR_met_afterFit.root", 500, 1000)
'''

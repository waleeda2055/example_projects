#include "HaaSkimming/SkimSlim.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TMVA/Reader.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <iomanip>

void SkimSlim::SetBranchTree(TTree *tree, int ntag){

TString name = tree->GetName();

//any tree that needs to be filled in the mini-ntuples has to be listed here

tree->Branch("event_number", &EventNumber);
tree->Branch("mc_channel_number", &DatasetNumber);
tree->Branch("RandomRunNumber", &RandomRunNumber);

//Trigger
tree->Branch("trigWeight_singleMuonTrig", &trigWeight_singleMuonTrig);
tree->Branch("trigMatch_singleMuonTrig", &trigMatch_singleMuonTrig);
tree->Branch("trigMatch_symdiMuonTrig", &trigMatch_symdiMuonTrig);
tree->Branch("passSingleMuon", &passSingleMuon);
tree->Branch("passDimuon", &passDimuon);
//weight for dimuon SF wrong

//Trigger SF
tree->Branch("SF_SL", &SF_SL);
tree->Branch("SF_DL", &SF_DL);
tree->Branch("SF_SLORDL", &SF_SLORDL);
tree->Branch("SF_SL_MUON_EFF_TrigStatUncertainty__1down", &SF_SL_MUON_EFF_TrigStatUncertainty__1down);
tree->Branch("SF_SL_MUON_EFF_TrigStatUncertainty__1up", &SF_SL_MUON_EFF_TrigStatUncertainty__1up);
tree->Branch("SF_SL_MUON_EFF_TrigSystUncertainty__1down", &SF_SL_MUON_EFF_TrigSystUncertainty__1down);
tree->Branch("SF_SL_MUON_EFF_TrigSystUncertainty__1up", &SF_SL_MUON_EFF_TrigSystUncertainty__1up);
tree->Branch("SF_DL_MUON_EFF_TrigStatUncertainty__1down", &SF_DL_MUON_EFF_TrigStatUncertainty__1down);
tree->Branch("SF_DL_MUON_EFF_TrigStatUncertainty__1up", &SF_DL_MUON_EFF_TrigStatUncertainty__1up);
tree->Branch("SF_DL_MUON_EFF_TrigSystUncertainty__1down", &SF_DL_MUON_EFF_TrigSystUncertainty__1down);
tree->Branch("SF_DL_MUON_EFF_TrigSystUncertainty__1up", &SF_DL_MUON_EFF_TrigSystUncertainty__1up);
tree->Branch("SF_SLORDLMUON_EFF_TrigStatUncertainty__1down", &SF_SLORDLMUON_EFF_TrigStatUncertainty__1down);
tree->Branch("SF_SLORDLMUON_EFF_TrigStatUncertainty__1up", &SF_SLORDLMUON_EFF_TrigStatUncertainty__1up);
tree->Branch("SF_SLORDLMUON_EFF_TrigSystUncertainty__1down", &SF_SLORDLMUON_EFF_TrigSystUncertainty__1down);
tree->Branch("SF_SLORDLMUON_EFF_TrigSystUncertainty__1up", &SF_SLORDLMUON_EFF_TrigSystUncertainty__1up);


//Weights and efficiency
tree->Branch("lumiWeight", &lumiWeight);
tree->Branch("event_weight", &event_weight);
tree->Branch("xsec", &xsec);
tree->Branch("MCweight", &eventWeight);
//tree->Branch("genWeight", &genWeight);
tree->Branch("jvtWeight", &jvtWeight);
tree->Branch("bTagWeight", &bTagWeight);
tree->Branch("leptonWeight", &leptonWeight);
tree->Branch("pileupWeight", &pileupWeight);

tree->Branch("met", &met_met);
tree->Branch("met_px", &met_px);
tree->Branch("met_py", &met_py);
tree->Branch("higgs_pt", &higgs_pt);

//Muons
tree->Branch("mu1Pt", &mu1Pt);
tree->Branch("mu2Pt", &mu2Pt);
tree->Branch("mu1Eta", &mu1Eta);
tree->Branch("mu2Eta", &mu2Eta);
tree->Branch("mu1Phi", &mu1Phi);
tree->Branch("mu2Phi", &mu2Phi);
tree->Branch("mu1E", &mu1E);
tree->Branch("mu2E", &mu2E);
tree->Branch("nMuons_signal", &nMuons_signal);
tree->Branch("nMuons_base", &nMuons_base);
tree->Branch("muIso1", &muIso1);
tree->Branch("muIso2", &muIso2);
tree->Branch("isSignalMu1", &isSignalMu1);
tree->Branch("isSignalMu2", &isSignalMu2);
tree->Branch("isTrueMu1", &isTrueMu1);
tree->Branch("isTrueMu2", &isTrueMu2);
tree->Branch("mu1Charge", &mu1Charge);
tree->Branch("mu2Charge", &mu2Charge);

tree->Branch("isTrueElec", &isTrueElec);
tree->Branch("isFakeMu1", &isFakeMu1);
tree->Branch("isFakeMu2", &isFakeMu2);
tree->Branch("isFakeElec", &isFakeElec);
tree->Branch("isPhoConvMu1", &isPhoConvMu1);
tree->Branch("isPhoConvMu2", &isPhoConvMu2);
tree->Branch("isPhoConvElec", &isPhoConvElec);

tree->Branch("isOS", &isOS);
tree->Branch("isZbb",&isZbb);

tree->Branch("dimuon_pt", &dimuonPt);
tree->Branch("dimuon_eta", &dimuonEta);
tree->Branch("dimuon_phi", &dimuonPhi);
tree->Branch("dimuon_m", &dimuonM);
tree->Branch("dimuon_e", &dimuonE);

//for muon channel 
tree->Branch("dimuonM_mu", &dimuonM_mu);
tree->Branch("mu3Pt", &mu3Pt);
tree->Branch("mu3Eta", &mu3Eta);
tree->Branch("mu3Phi", &mu3Phi);
tree->Branch("mu3E", &mu3E);
tree->Branch("dRm1m3", &dRm1m3);
tree->Branch("dRm2m3", &dRm2m3);
tree->Branch("isCorrect",&isCorrect);
tree->Branch("dRm2m3_tru", &dRm2m3_tru);
tree->Branch("dRm1m3_tru", &dRm1m3_tru);
tree->Branch("dRm1m2_tru", &dRm1m2_tru);
tree->Branch("mT_W", &mT_W);
tree->Branch("mT_W_tru", &mT_W_tru);
tree->Branch("mT_W_e1", &mT_W_e1);


//Electrons
tree->Branch("nElectrons_base", &nElectrons_base);
tree->Branch("nElectrons_signal", &nElectrons_signal);
tree->Branch("elec1Pt", &elec1Pt);
tree->Branch("elec1Eta", &elec1Eta);
tree->Branch("elec1Phi", &elec1Phi);
tree->Branch("elec1E", &elec1E);
tree->Branch("elecCharge", &elecCharge);


//new angle variables plus W and Hplus recon.
tree->Branch("dRMmme1", &dRMmme1);
tree->Branch("dRMmmb1", &dRMmmb1);
tree->Branch("dRMmmb2", &dRMmmb2);
tree->Branch("dRMmmMbb", &dRMmmMbb);
tree->Branch("dRMmmW", &dRMmmW);
tree->Branch("hplus_m", &hplus_m);
tree->Branch("Wmass", &Wmass);
//tree->Branch("dRMmmW2", &dRMmmW2);
//tree->Branch("hplus_m2", &hplus_m2);
//tree->Branch("Wmass2", &Wmass2);
tree->Branch("dRm1e1", &dRm1e1);
tree->Branch("dRm2e1", &dRm2e1);
tree->Branch("m_ct", &m_ct);
tree->Branch("Mmu1e_os", &Mmu1e_os);
tree->Branch("Mmu2e_os", &Mmu2e_os);
tree->Branch("Mmu1e_ss", &Mmu1e_ss);
tree->Branch("Mmu2e_ss", &Mmu2e_ss); 
tree->Branch("Mmu1e", &Mmu1e);
tree->Branch("Mmu2e", &Mmu2e); 
tree->Branch("pTfrac_mu2_mu1",&pTfrac_mu2_mu1 );
tree->Branch("pTfrac_e1_mu1", &pTfrac_e1_mu1 );
tree->Branch("pTfrac_mu2_e1", &pTfrac_mu2_e1 );
tree->Branch("pTfrac_mu2_mu1_m2eOS", &pTfrac_mu2_mu1_m2eOS );
tree->Branch("pTfrac_mu2_mu1_m2eSS", &pTfrac_mu2_mu1_m2eSS );
tree->Branch("pTfrac_e1_mu1_m1eSS", &pTfrac_e1_mu1_m1eSS );
tree->Branch("pTfrac_e1_mu1_m1eOS",&pTfrac_e1_mu1_m1eOS );
tree->Branch("pTfrac_e1_mu1_m2eSS",&pTfrac_e1_mu1_m2eSS );
tree->Branch("pTfrac_e1_mu1_m2eOS",&pTfrac_e1_mu1_m2eOS );

//Angles
tree->Branch("dRm1b1", &dRm1b1);
tree->Branch("dRm2b1", &dRm2b1);
tree->Branch("dRm1b2", &dRm1b2);
tree->Branch("dRm2b2", &dRm2b2);
tree->Branch("dRm1m2", &dRm1m2);
tree->Branch("dRb1b2", &dRb1b2);
tree->Branch("dRMmmMbbkl", &dRMmmMbbkl);
tree->Branch("diffRmRb", &diffRmRb);
tree->Branch("dRmb1", &dRmb1); //biggest angle between a muon and a bjet
tree->Branch("dRmb2", &dRmb2); //second biggest angle between a muon and a bjet
tree->Branch("dRmb3", &dRmb3);
tree->Branch("dRmb4", &dRmb4); //smallest angle between a muon and a bjet
tree->Branch("mdRmb1", &mdRmb1); //invariant mass of the muon and bjet with biggest angle
tree->Branch("mdRmb2", &mdRmb2);
tree->Branch("mdRmb3", &mdRmb3);
tree->Branch("mdRmb4", &mdRmb4);
tree->Branch("centrality", &centrality);
tree->Branch("centralityAll", &centralityAll);
tree->Branch("Ht", &Ht);
tree->Branch("HtAll", &HtAll);
tree->Branch("dPhimumujj",&dPhimumujj);



//BDTs

for(unsigned int i=0; i<BDT_configs.size(); i++){
  tree->Branch(BDT_configs[i].name, &BDT_outputs[i]);
}



//Jets
tree->Branch("nJet20",&nJet20);
tree->Branch("dijetPt", &dijetPt);
tree->Branch("dijetEta", &dijetEta);
tree->Branch("dijetPhi", &dijetPhi);
tree->Branch("dijetM", &dijetM);
tree->Branch("dijetE", &dijetE);

/*
//Bjets
tree->Branch("bjet1Pt", &bjet1Pt);
tree->Branch("bjet2Pt", &bjet2Pt);
tree->Branch("bjet1Eta", &bjet1Eta);
tree->Branch("bjet2Eta", &bjet2Eta);
tree->Branch("bjet1Phi", &bjet1Phi);
tree->Branch("bjet2Phi", &bjet2Phi);
tree->Branch("bjet1E", &bjet1E);
tree->Branch("bjet2E", &bjet2E);
tree->Branch("bjet1M", &bjet1M);
tree->Branch("bjet2M", &bjet2M);
tree->Branch("bjet1Jvt", &bjet1Jvt);
tree->Branch("bjet2Jvt", &bjet2Jvt);
tree->Branch("bjet1Eff", &bjet1Eff);
tree->Branch("bjet2Eff", &bjet2Eff);
, int ntags
tree->Branch("dibjetPt", &dibjetPt);
tree->Branch("dibjetEta", &dibjetEta);
tree->Branch("dibjetPhi", &dibjetPhi);
tree->Branch("dibjetM", &dibjetM);
tree->Branch("dibjetE", &dibjetE);

*/
//Jets
tree->Branch("jet1Pt", &jet1Pt);
tree->Branch("jet1Eta", &jet1Eta);
tree->Branch("jet1Phi", &jet1Phi);
tree->Branch("jet1M", &jet1M);
tree->Branch("jet1E", &jet1E);
tree->Branch("jet1Jvt", &jet1Jvt);
tree->Branch("jet1Eff", &jet1Eff);
tree->Branch("jet2Pt", &jet2Pt);
tree->Branch("jet2Eta", &jet2Eta);
tree->Branch("jet2Phi", &jet2Phi);
tree->Branch("jet2M", &jet2M);
tree->Branch("jet2E", &jet2E);
tree->Branch("jet2Jvt", &jet2Jvt);
tree->Branch("jet2Eff", &jet2Eff);


//////----------------------------------variables filled per # b-tags----------------------------------//////

tree->Branch("nBjets_77", &nBJet20_MV2c10_FixedCutBEff_77);
tree->Branch("nBjets_60", &nBJet20_MV2c10_FixedCutBEff_60);
tree->Branch("njetcentral30",&njetcentral30);
tree->Branch("njetcentral20",&njetcentral20);
tree->Branch("njetfwd30",&njetfwd30);
tree->Branch("njetfwd20",&njetfwd20);
tree->Branch("nbjet30_60",&nbjet30_60);
tree->Branch("nbjet30_77",&nbjet30_77);

//klfit stuff
/*

tree->Branch("klfit_loglikelihood",&klfit_loglikelihood);
tree->Branch("klfit_dijet_pt",&klfit_dijet_pt);
tree->Branch("klfit_dijet_eta",&klfit_dijet_eta);
tree->Branch("klfit_dijet_phi",&klfit_dijet_phi);
tree->Branch("klfit_dijet_e",&klfit_dijet_e);
tree->Branch("klfit_dijet_m",&klfit_dijet_m);

tree->Branch("klfit_bjet1_pt",&klfit_bjet1_pt);
tree->Branch("klfit_bjet1_eta",&klfit_bjet1_eta);
tree->Branch("klfit_bjet1_phi",&klfit_bjet1_phi);
tree->Branch("klfit_bjet1_e",&klfit_bjet1_e);
tree->Branch("klfit_bjet1_m",&klfit_bjet1_m);

tree->Branch("klfit_bjet2_pt",&klfit_bjet2_pt);
tree->Branch("klfit_bjet2_eta",&klfit_bjet2_eta);
tree->Branch("klfit_bjet2_phi",&klfit_bjet2_phi);
tree->Branch("klfit_bjet2_e",&klfit_bjet2_e);
tree->Branch("klfit_bjet2_m",&klfit_bjet2_m);

*/

tree->Branch("bjet1_pt",&noKL_jet1_pt);
tree->Branch("bjet1_eta",&noKL_jet1_eta);
tree->Branch("bjet1_phi",&noKL_jet1_phi);
tree->Branch("bjet1_e",&noKL_jet1_e);
tree->Branch("bjet1_m",&noKL_jet1_m);

tree->Branch("bjet2_pt",&noKL_jet2_pt);
tree->Branch("bjet2_eta",&noKL_jet2_eta);
tree->Branch("bjet2_phi",&noKL_jet2_phi);
tree->Branch("bjet2_e",&noKL_jet2_e);
tree->Branch("bjet2_m",&noKL_jet2_m);

tree->Branch("leadFwdJetPt",&leadFwdJetPt);
tree->Branch("leadFwdJetEta",&leadFwdJetEta);
tree->Branch("leadCJetPt",&leadCJetPt);
tree->Branch("leadCJetEta",&leadCJetEta);
tree->Branch("leadBJetPt",&leadBJetPt);
tree->Branch("leadBJetEta",&leadBJetEta);

/*

tree->Branch("klfit_dimuon_pt",&klfit_dilepton_pt);
tree->Branch("klfit_dimuon_eta",&klfit_dilepton_eta);
tree->Branch("klfit_dimuon_phi",&klfit_dilepton_phi);
tree->Branch("klfit_dimuon_e",&klfit_dilepton_e);
tree->Branch("klfit_dimuon_m",&klfit_dilepton_m);

tree->Branch("klfit_bbmm_pt",&klfit_jjll_pt);
tree->Branch("klfit_bbmm_eta",&klfit_jjll_eta);
tree->Branch("klfit_bbmm_phi",&klfit_jjll_phi);
tree->Branch("klfit_bbmm_e",&klfit_jjll_e);
tree->Branch("klfit_bbmm_m",&klfit_jjll_m);
tree->Branch("klbbmm_m",&klfit_jjll_m);

tree->Branch("dibjet_m",&noKL_dijet_m);
tree->Branch("dibjet_pt",&noKL_dijet_pt);
tree->Branch("dibjet_eta",&noKL_dijet_eta);
tree->Branch("dibjet_phi",&noKL_dijet_phi);

*/

if(ntag==2&&isNominal&&!isData){
tree->Branch("leptonWeight_MUON_EFF_BADMUON_STAT__1down", &leptonWeight_MUON_EFF_BADMUON_STAT__1down);
tree->Branch("leptonWeight_MUON_EFF_BADMUON_STAT__1up", &leptonWeight_MUON_EFF_BADMUON_STAT__1up);
tree->Branch("leptonWeight_MUON_EFF_BADMUON_SYS__1down", &leptonWeight_MUON_EFF_BADMUON_SYS__1down);
tree->Branch("leptonWeight_MUON_EFF_BADMUON_SYS__1up", &leptonWeight_MUON_EFF_BADMUON_SYS__1up);
tree->Branch("leptonWeight_MUON_EFF_ISO_STAT__1down", &leptonWeight_MUON_EFF_ISO_STAT__1down);
tree->Branch("leptonWeight_MUON_EFF_ISO_STAT__1up", &leptonWeight_MUON_EFF_ISO_STAT__1up);
tree->Branch("leptonWeight_MUON_EFF_ISO_SYS__1down", &leptonWeight_MUON_EFF_ISO_SYS__1down);
tree->Branch("leptonWeight_MUON_EFF_ISO_SYS__1up", &leptonWeight_MUON_EFF_ISO_SYS__1up);
tree->Branch("leptonWeight_MUON_EFF_RECO_STAT__1down", &leptonWeight_MUON_EFF_RECO_STAT__1down);
tree->Branch("leptonWeight_MUON_EFF_RECO_STAT__1up", &leptonWeight_MUON_EFF_RECO_STAT__1up);
tree->Branch("leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1down", &leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1down);
tree->Branch("leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1up", &leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1up);
tree->Branch("leptonWeight_MUON_EFF_RECO_SYS__1down", &leptonWeight_MUON_EFF_RECO_SYS__1down);
tree->Branch("leptonWeight_MUON_EFF_RECO_SYS__1up", &leptonWeight_MUON_EFF_RECO_SYS__1up);
tree->Branch("leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1down", &leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1down);
tree->Branch("leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1up", &leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1up);
tree->Branch("leptonWeight_MUON_EFF_TTVA_STAT__1down", &leptonWeight_MUON_EFF_TTVA_STAT__1down);
tree->Branch("leptonWeight_MUON_EFF_TTVA_STAT__1up", &leptonWeight_MUON_EFF_TTVA_STAT__1up);
tree->Branch("leptonWeight_MUON_EFF_TTVA_SYS__1down", &leptonWeight_MUON_EFF_TTVA_SYS__1down);
tree->Branch("leptonWeight_MUON_EFF_TTVA_SYS__1up", &leptonWeight_MUON_EFF_TTVA_SYS__1up);
tree->Branch("bTagWeight_FT_EFF_B_systematics__1down", &bTagWeight_FT_EFF_B_systematics__1down);
tree->Branch("bTagWeight_FT_EFF_B_systematics__1up", &bTagWeight_FT_EFF_B_systematics__1up);
tree->Branch("bTagWeight_FT_EFF_C_systematics__1down", &bTagWeight_FT_EFF_C_systematics__1down);
tree->Branch("bTagWeight_FT_EFF_C_systematics__1up", &bTagWeight_FT_EFF_C_systematics__1up);
tree->Branch("bTagWeight_FT_EFF_Light_systematics__1down", &bTagWeight_FT_EFF_Light_systematics__1down);
tree->Branch("bTagWeight_FT_EFF_Light_systematics__1up", &bTagWeight_FT_EFF_Light_systematics__1up);
tree->Branch("bTagWeight_FT_EFF_extrapolation__1down", &bTagWeight_FT_EFF_extrapolation__1down);
tree->Branch("bTagWeight_FT_EFF_extrapolation__1up", &bTagWeight_FT_EFF_extrapolation__1up);
tree->Branch("bTagWeight_FT_EFF_extrapolation_from_charm__1down", &bTagWeight_FT_EFF_extrapolation_from_charm__1down);
tree->Branch("bTagWeight_FT_EFF_extrapolation_from_charm__1up", &bTagWeight_FT_EFF_extrapolation_from_charm__1up);
tree->Branch("jvtWeight_JET_JvtEfficiency__1down", &jvtWeight_JET_JvtEfficiency__1down);
tree->Branch("jvtWeight_JET_JvtEfficiency__1up", &jvtWeight_JET_JvtEfficiency__1up);
tree->Branch("pileupWeightUp", &pileupWeightUp);
tree->Branch("pileupWeightDown", &pileupWeightDown);

for(auto& weight : theoryMap){
  tree->Branch(weight.second.weightName.Data(), &weight.second.finalWeight);
}

}




/*
tree->Branch("klbb_m",&klbb_m);
tree->Branch("dibjet_m",&dibjet_m);
tree->Branch("bbmm_m",&bbmm_m);
tree->Branch("klbbmm_m",&klbbmm_m);
tree->Branch("jet0_weight",&jet0_weight);
tree->Branch("jet0_weight_up",&jet0_weight_up);
tree->Branch("jet0_weight_down",&jet0_weight_down);
tree->Branch("met_weight",&met_weight);
tree->Branch("event_weight",&event_weight);
tree->Branch("mc_channel_number",&mc_channel_number);
tree->Branch("BTagSysCalo",&BTagSysCalo);
tree->Branch("OS",&OS);
tree->Branch("trackCaloSR",&trackCaloSR);
tree->Branch("massdiff",&massdiff);
tree->Branch("klmassdiff",&klmassdiff);
*/

}

void SkimSlim::fillRightBin(TString cut){
  if(mapOfHists.size()==0) return;
  TH1 *h_weight = mapOfHists[DatasetNumber];
  TH1 *h_unweight = mapOfUnweightedHists[DatasetNumber];
  int nBins = h_weight->GetNbinsX();
  h_weight->GetXaxis()->SetBinLabel(1,"SumOfWeights");
  h_unweight->GetXaxis()->SetBinLabel(1,"SumOfWeights");

  if(cut=="passSkimmingCuts"){
    h_weight->GetXaxis()->SetBinLabel(nBins,cut);
    h_unweight->GetXaxis()->SetBinLabel(nBins,cut);
    h_weight->SetBinContent(nBins,h_weight->GetBinContent(nBins)+eventWeightCuts/lumiWeight);
    h_unweight->SetBinContent(nBins,h_unweight->GetBinContent(nBins)+1);
    return;
  }
  for(int i=2; i < nBins; ++i){
    TString label = h_weight->GetXaxis()->GetBinLabel(i);
    if(cut==label){ 
      h_weight->SetBinContent(i,h_weight->GetBinContent(i)+eventWeightCuts/lumiWeight);
      h_unweight->SetBinContent(i,h_unweight->GetBinContent(i)+1);
      break;
    }
    if(""==label){
      h_weight->GetXaxis()->SetBinLabel(i,cut);
      h_unweight->GetXaxis()->SetBinLabel(i,cut);
      h_weight->SetBinContent(i,h_weight->GetBinContent(i)+eventWeightCuts/lumiWeight);
      h_unweight->SetBinContent(i,h_unweight->GetBinContent(i)+1);
      break;
    }
  }
}


bool SkimSlim::passSelection(){
passSingleMuon = false;
passDimuon = false;
passDilep = false;
passSingleElec = false; 
passAsym = false;


if(doCMS){
  if (jetPt->size()<2) return false;
  if(dimuonM < 12) return false;
  if(muPt->size() < 2) return false;
  if(fabs(muEta->at(0)) > 2.1) return false;
  if(fabs(muEta->at(1)) > 2.1) return false;
  if(muPt->at(0) < 27) return false;
  //if ( !(trigMatch_singleMuonTrig || trigMatch_symdiMuonTrig) )return false;
  if (!trigMatch_singleMuonTrig) return false;
  if(muCharge->at(0)*muCharge->at(1)>0) return false;
}

else{
cutManager->count("passxAODCuts");
fillRightBin("passxAODCuts");

//if(muPt->size() != 2) return false;
//cutManager->count("passNobjectCuts");
//fillRightBin("passNobjectCuts");
//if(muPt->at(0) < 27) return false;
//if(muPt->at(1) < 7) return false;

//if(muPt->at(0) < 27) return false;
//   cutManager->count("passMuonPtCuts");
//   fillRightBin("passMuonPtCuts");

//if(nMuons_signal < 2) return false;
//cutManager->count(">= 2 muons");
//fillRightBin(">= 2 muons");

if(trigMatch_singleMuonTrig && muPt->at(0) > 27 && muPt->at(1) > 5){ 
//if(trigMatch_singleMuonTrig){ 
  passSingleMuon = true; 
  cutManager->count("passSingleMuonCuts");
  fillRightBin("passSingleMuonCuts");
}


if(trigMatch_symdiMuonTrig && muPt->at(0) > 15 && muPt->at(1) > 15){
//if(trigMatch_symdiMuonTrig){
  passDimuon = true;
  cutManager->count("passDimuonCuts");
  fillRightBin("passDimuonCuts");
}

//if(trigMatch_asymdiMuonTrig){
//   passAsym = true;
//   cutManager->count("passAsym");
//   fillRightBin("passAsym");
//}

//if(trigMatch_symdiLepTrig){
//   passDilep = true;
   //cutManager->count("passDilep");
   //fillRightBin("passDilep");
//}

//if(trigMatch_singleElecTrig){
//   passSingleElec = true;
   //cutManager->count("passSingleElec");
   //fillRightBin("passSingleElec");
//}

//if ( !passSingleMuon) return false;

//if ( !passDimuon ) return false;

//if ( !passDilep ) return false;
//cutManager->count("passDilep");
//fillRightBin("passDilep");

//if ( !passSingleElec ) return false;
//cutManager->count("passSingleElec");
//fillRightBin("passSingleElec");


//if ( !passSingleElec && !passDilep ) return false;
//cutManager->count("passSingleElecORDilep");
//fillRightBin("passSingleElecORDilep");

if ( !passSingleMuon && !passDimuon ) return false;
cutManager->count("passSingleORDimuon");
fillRightBin("passSingleORDimuon");

//if ( !passSingleMuon && !passDimuon && !passDilep ) return false;
//cutManager->count("passSingleORDimuonORDilep");
//fillRightBin("passSingleORDimuonORdilep");

//if ( !passSingleMuon && !passDimuon && !passAsym ) return false;
//cutManager->count("passSingleORDimuonORAsym");
//fillRightBin("passSingleORDimuonORAsym");

//if ( !passSingleMuon && !passDimuon && !passSingleElec ) return false;
//cutManager->count("passSingleORDimuonORElec");
//fillRightBin("passSingleORDimuonORElec");

//if ( !passSingleMuon && !passDimuon && !passSingleElec && !passDilep) return false;
//cutManager->count("passSingleORDimuonORElecORDilep");
//fillRightBin("passSingleORDimuonORElecORDilep");

//if(nJet20 < 3) return false;
//cutManager->count(">= 3 Jet, 20GeV");
//fillRightBin(">=3 Jet, 20GeV");

//if(nJet20 < 4) return false;
//cutManager->count(">= 4 Jet, 20GeV");
//fillRightBin(">= 4 Jet, 20GeV");

if(nJet20 < 2) return false;
cutManager->count(">=2 Jet, 20GeV");
fillRightBin(">=2 Jet, 20GeV");

//if(nMuons_signal < 2) return false;
//cutManager->count(">= 2 muons");
//fillRightBin(">= 2 muons");

//if(met_met < 30)
////cutManager->count("passMETCuts");
////fillRightBin("passMETCuts");

//if(nBJet20_MV2c10_FixedCutBEff_77 < 1) return false;
//cutManager->count("passNbjetCuts, >=1 b-jet");
//fillRightBin("passNbjetCuts, >=1 b-jet");

//electron channel cuts

//if(muCharge->at(0)*muCharge->at(1)<0) return false;
////cutManager->count("passSSMuonCuts");
////fillRightBin("passSSMuonCuts");

//if(nMuons_signal != 2) return false;
//cutManager->count("== 2 muons");
//fillRightBin("== 2 muons");

//if(muCharge->at(0)*muCharge->at(1)>0) return false;
//cutManager->count("passOSMuonCuts");
//fillRightBin("passOSMuonCuts");

if(dimuonM < 12) return false; //8
cutManager->count("dimuon_m >= 12");
fillRightBin("dimuon_m >= 12");

//if(dimuonM > 77 ) return false; //8
//cutManager->count("dimuon_m <= 77");
//fillRightBin("dimuon_m <= 77");

if(nElectrons_signal < 1) return false;
cutManager->count(">= 1 electron");
fillRightBin(">= 1 electron");

//if(nElectrons_signal != 1) return false;
//cutManager->count("== 1 electron");
//fillRightBin("== 1 electron");

//if(elecPt->at(0) < 20 ) return false;
//cutManager->count(">= 20 GeV, elec1Pt");
//fillRightBin(">=20 GeV, elec1Pt");

//if(muPt->at(1)< 10) return false;
//cutManager->count(">= 10 GeV, mu2Pt");
//fillRightBin(">=10 GeV, mu2pt");

// muon channel cuts

//if(nMuons_signal != 3) return false;
//cutManager->count("== 3 muons");
//fillRightBin("== 3 muons");

//if(nMuons_signal < 4) return false;
//cutManager->count(">= 4 muons");
//fillRightBin(">= 4 muons");


//if(muPt->at(0) < 25 ) return false;
//cutManager->count(">= 25 GeV mu1Pt");
//fillRightBin(">=25 GeV mut1Pt");


//int n_OS_pair =0;

//for (size_t a = 0; a < nMuons_signal; a++){ 
//  for (size_t b = a+1; b < nMuons_signal; b++){ 
//    if (muCharge->at(a) != muCharge->at(b)) 
//	n_OS_pair ++; 
//  } 
//}

//for (size_t a = 0; a < 3; a++){
//  for (size_t b = a+1; b < 3; b++){
//    if (muCharge->at(a) != muCharge->at(b))
//        n_OS_pair ++;
// }
//}

//if(n_OS_pair < 1) return false;
//cutManager->count(">= 1 OS Mu");
//fillRightBin(">= 1 OS Mu");

//if(nElectrons_signal > 0) return false;
//cutManager->count("<= 0 electron");
//fillRightBin("<= 0 electron");

//if (muOrigin->at(0) == 14 &&  muOrigin->at(1)== 14)
//a_01.push_back(1);
//else if (muOrigin->at(1) == 14 &&  muOrigin->at(2)==14)
//a_12.push_back(1);
//else if (muOrigin->at(0) == 14 &&  muOrigin->at(2)==14)
//a_02.push_back(1);


//cout << muOrigin->at(0) <<","<<  muOrigin->at(1) << ","<<  muOrigin->at(2) <<endl;
//cout << "evt weight " <<event_weight << "dimuon mass " << dimuonM << "evt number "<< EventNumber <<endl;


/*
if((nBJet20_MV2c10_FixedCutBEff_77 != 2)&&(nBJet20_MV2c10_FixedCutBEff_77 != 0)) return false;
cutManager->count("passNbjetCuts ", (double)nBJet20_MV2c10_FixedCutBEff_77 );
if(nBJet20_MV2c10_FixedCutBEff_77 == 2){
  fillRightBin("passNbjetCuts");
  if(met_met < 60){
    cutManager->count("passMETCuts");
    fillRightBin("passMETCuts");
    if(fabs(klfit_jjll_m - 125) < 15){
      cutManager->count("passHiggsMCuts");
      fillRightBin("passHiggsMCuts");
      if(klfit_loglikelihood > -8){
 	cutManager->count("passKlfitCuts");
        fillRightBin("passKlfitCuts");
      }
    }
  }
}
*/

cutManager->count("passSkimmingCuts");
fillRightBin("passSkimmingCuts");

}
return true;
}


void SkimSlim::swapVectorOrder(vector <float>* muStuff, uint j){
float bla = muStuff->at(j);
muStuff->at(j) = muStuff->at(j+1);
muStuff->at(j+1) = bla;

}

void SkimSlim::swapVectorOrder(vector <int>* muStuff, uint j){
int bla = muStuff->at(j);
muStuff->at(j) = muStuff->at(j+1);
muStuff->at(j+1) = bla;

}

bool SkimSlim::isPromptMuon(int type, int orig){
  //if(type != 6) return false;
  //if(orig != 10 && orig != 12 && orig != 13 && orig != 14 && orig != 43 && orig != 15 ) return false;
  if(orig != 10 ) return false;
  return true;
}


bool SkimSlim::isPromptElec(int type, int orig){
  //if(type != 2) return false; //isolated elec
  //if(orig != 10 && orig != 12 && orig != 13 && orig != 14 && orig != 43 && orig != 15) return false;
  if(orig != 10) return false;
  return true;
}


bool SkimSlim::isNonPromptMuon(int type, int orig){
  //if(type != 7) return false; //7 is non isolated muon
  if( orig != 5 && orig != 23 && orig != 24 && orig != 25 && orig != 26 && orig != 27 && orig != 28 && orig != 29 && orig != 30 && orig != 31 && orig != 32 && orig != 33 ) return false; //defs from ttH paper
  return true;
}

bool SkimSlim::isNonPromptElec(int type, int orig){
  //if(type != 3) return false; //7 is non isolated muon
  if( orig !=5 && orig != 23 && orig != 24 && orig != 25 && orig != 26 && orig != 27 && orig != 28 && orig != 29 && orig != 30 && orig != 31 && orig != 32 && orig != 33 ) return false; //defs from ttH paper
  return true;
}

bool SkimSlim::PhoConvMu(int type, int orig){
  //if (orig == 5) cout << type << endl;
  //if(type != 6) return false; //6 is isolated muon
  if(orig != 5) return false; //defs from ttH paper
  return true;  
}

bool SkimSlim::PhoConvElec(int type, int orig){
  //if (orig == 5) cout << type << endl;
  //if(type != 2) return false; //2 is iso muon
  if(orig != 5) return false; //defs from ttH paper
  return true;
}


void SkimSlim::orderMuonPt(){

for(unsigned int i=0; i<muPt->size();++i){
  bool order = true;
  for(unsigned int j=0; j<muPt->size()-1;++j){
    if(muPt->at(j)<muPt->at(j+1)){
      order = false;
      swapVectorOrder(muPt,j);
      swapVectorOrder(muEta,j);
      swapVectorOrder(muPhi,j);
      swapVectorOrder(muM,j);
      swapVectorOrder(muE,j);
      swapVectorOrder(muIso,j);
      swapVectorOrder(muCharge,j);
      swapVectorOrder(muType,j);
      swapVectorOrder(muOrigin,j);
    }
  }
  if(order) break;
}
}

void SkimSlim::orderAngles(vector<float> &dR, vector<float> &dRm){
for(unsigned int i=0; i<dR.size();++i){
  bool order = true;
  for(unsigned int j=0; j<dR.size()-1;++j){
    if(dR.at(j)<dR.at(j+1)){
      order = false;
      swapVectorOrder(&dR,j);
      swapVectorOrder(&dRm,j);
    }
  }
  if(order) break;
}
}

bool SkimSlim::isSampleZbb(){
  if(DatasetNumber == 364102) return true;
  if(DatasetNumber == 364105) return true;
  if(DatasetNumber == 364108) return true;
  if(DatasetNumber == 364111) return true;
  if(DatasetNumber == 364199) return true;
  if(DatasetNumber == 364201) return true;
  if(DatasetNumber == 364203) return true;
  if(DatasetNumber == 364130) return true;
  if(DatasetNumber == 364133) return true;
  if(DatasetNumber == 364136) return true;
  if(DatasetNumber == 364139) return true;
  if(DatasetNumber == 364211) return true;
  if(DatasetNumber == 364213) return true;
  if(DatasetNumber == 364215) return true;
  return false;
}


void SkimSlim::Loop()
{
   if (!outputfile){
     cout << "Outputfile not provided! Abort!" << endl;
     abort();
   }
   outputfile->cd();
   BDT_outputs.resize(BDT_configs.size(), -999);
   TTree *tree0=0, *tree1=0, *tree2=0;
   if (write0Tree && !doCMS){
     tree0 = new TTree(newTreeName1, newTreeName1);
     this->SetBranchTree(tree0,0);
   }
   if (write1Tree && !doCMS){
     tree1 = new TTree(newTreeName2, newTreeName2);
     this->SetBranchTree(tree1,1);
   } 
   tree2 = new TTree(newTreeName3, newTreeName3);
   gROOT->cd();
   this->SetBranchTree(tree2,2);

   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntries();
   if (maxEvents > 0) nentries = maxEvents;
   cout << "nentries: " << nentries << endl;
   Long64_t nbytes = 0, nb = 0;

   TMVA::Reader *reader = new TMVA::Reader();
   reader->AddVariable("dibjet_m", &dibjet_m);
   reader->AddVariable("klfit_loglikelihood", &klfit_loglikelihood);
   reader->AddVariable("dRMmmMbbkl",&dRMmmMbbkl);
   reader->AddVariable("dRm1b1",&dRm1b1);
   reader->AddVariable("dRm2b1",&dRm2b1);
   reader->AddVariable("dRm1b2",&dRm1b2);
   reader->AddVariable("dRm2b2",&dRm2b2);
   reader->AddVariable("dRm1m2",&dRm1m2);
   reader->AddVariable("dRb1b2",&dRb1b2);
    
    
   for(unsigned int i = 0; i<BDT_configs.size(); i++){
     reader->BookMVA(BDT_configs[i].name+"train0", BDT_configs[i].weightfile+"train0.weights.xml");
     reader->BookMVA(BDT_configs[i].name+"train1", BDT_configs[i].weightfile+"train1.weights.xml");
   }

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if(jentry%10000 == 0){
	cout << jentry <<"\r";
	flush(cout);
      }
    
    correctXsec();
    isZbb = isSampleZbb(); 
    computeSF();
    
    //cout << "Event number: " << EventNumber << endl; 
    //Compute event weights for CutManager (needs a double but our event weight is float and also needs to be computed before passSelection
	for(auto &weights : theoryMap){
      weights.second.finalWeight = weights.second.weight * sumOfWeights[DatasetNumber] / weights.second.SOW[DatasetNumber];
	}       
 
	if(!isData)lumiWeight = xsec*lumi/ sumOfWeights[DatasetNumber];//xsec*lumi gives the number of events we expect in data, sumOfWeights is the number of MC events we produced (before any cuts)
        if (isData){ 
	  event_weight = 1;
	  //cout << "if data: " << event_weight << endl;
        }
        else{
	  event_weight = lumiWeight * eventWeight * leptonWeight * bTagWeight * jvtWeight * pileupWeight * SF_SLORDL;
	  //cout << "if not data: " << event_weight << endl;
          //cout << SF_SLORDL << SF_SLORDL << endl;
 	 
	  if(createRadDown){
	    if(theoryMap.find("LHE3Weight_Var3cDown") != theoryMap.end() && theoryMap.find("LHE3Weight_muR2p0_muF2p0") != theoryMap.end() ){
	      event_weight = event_weight * theoryMap["LHE3Weight_Var3cDown"].finalWeight * theoryMap["LHE3Weight_muR2p0_muF2p0"].finalWeight;
	    }
 	    else{ 
 	      std::cout << "ERROR: Could not find radiation down weights!" << std::endl;
	    }
	  }
          if(createRadUp){
            if(theoryMap.find("LHE3Weight_Var3cUp") != theoryMap.end() && theoryMap.find("LHE3Weight_muR0p5_muF0p5") != theoryMap.end() ){
              event_weight = event_weight * theoryMap["LHE3Weight_Var3cUp"].finalWeight * theoryMap["LHE3Weight_muR0p5_muF0p5"].finalWeight;
            }
            else{ 
              std::cout << "ERROR: Could not find radiation up weights!" << std::endl;
            }
          }
 	}

	//eventWeight is MC weight, SF_SLORDL only when using both dimuon and single muon triggers
        eventWeightCuts = event_weight;
        //if (nBJet20_MV2c10_FixedCutBEff_77 == 0 && !doCMS){
        //  event_weight *= 2.772e-5;
        //  if (!isData) event_weight *= -1;
        //}
	
	

    //for(auto &weights : theoryMap){
    //  weights.second.finalWeight = weights.second.weight * sumOfWeights[DatasetNumber] / weights.second.SOW[DatasetNumber];
    //}
    //cout << "after all: " << event_weight << endl;    
    
    njetcentral30 = 0;
    njetcentral20 = 0;
    njetfwd30 = 0;
    njetfwd20 = 0;
    nbjet30_60 = 0;
    nbjet30_77 = 0;
    for(unsigned int i = 0; i < jetPt->size(); ++i){
      if(fabs(jetEta->at(i)) < 2.4){
        if(jetPt->at(i) > 20) ++njetcentral20;
	if(jetPt->at(i) > 30){
	  ++njetcentral30;
	  if(jetMV2c10->at(i) > 0.94) ++nbjet30_60;
          if(jetMV2c10->at(i) > 0.63) ++nbjet30_77;
 	}
      } 
      if(fabs(jetEta->at(i)) > 2.4 && fabs(jetEta->at(i)) < 4.7 ){
        if(jetPt->at(i) > 20) ++njetfwd20;
        if(jetPt->at(i) > 30) ++njetfwd30;
      }
    }
     
    
      if(passSelection()){
divideWeights(leptonWeight_MUON_EFF_BADMUON_STAT__1down,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_BADMUON_STAT__1up,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_BADMUON_SYS__1down,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_BADMUON_SYS__1up,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_ISO_STAT__1down,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_ISO_STAT__1up,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_ISO_SYS__1down,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_ISO_SYS__1up,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_RECO_STAT__1down,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_RECO_STAT__1up,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1down,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1up,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_RECO_SYS__1down,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_RECO_SYS__1up,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1down,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1up,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_TTVA_STAT__1down,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_TTVA_STAT__1up,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_TTVA_SYS__1down,leptonWeight);
divideWeights(leptonWeight_MUON_EFF_TTVA_SYS__1up,leptonWeight);
divideWeights(bTagWeight_FT_EFF_B_systematics__1down,bTagWeight);
divideWeights(bTagWeight_FT_EFF_B_systematics__1up,bTagWeight);
divideWeights(bTagWeight_FT_EFF_C_systematics__1down,bTagWeight);
divideWeights(bTagWeight_FT_EFF_C_systematics__1up,bTagWeight);
divideWeights(bTagWeight_FT_EFF_Light_systematics__1down,bTagWeight);
divideWeights(bTagWeight_FT_EFF_Light_systematics__1up,bTagWeight);
divideWeights(bTagWeight_FT_EFF_extrapolation__1down,bTagWeight);
divideWeights(bTagWeight_FT_EFF_extrapolation__1up,bTagWeight);
divideWeights(bTagWeight_FT_EFF_extrapolation_from_charm__1down,bTagWeight);
divideWeights(bTagWeight_FT_EFF_extrapolation_from_charm__1up,bTagWeight);
divideWeights(jvtWeight_JET_JvtEfficiency__1down,jvtWeight);
divideWeights(jvtWeight_JET_JvtEfficiency__1up,jvtWeight);
divideWeights(pileupWeightUp,pileupWeight);
divideWeights(pileupWeightDown,pileupWeight);
 
        //orderMuonPt();
	//Fill muon variables
	mu1Pt =  muPt->at(0);
	mu2Pt =  muPt->at(1);
	mu1Eta =  muEta->at(0);
	mu2Eta =  muEta->at(1);
	mu1Phi =  muPhi->at(0);
	mu2Phi =  muPhi->at(1);
	mu1E =  muE->at(0);
	mu2E =  muE->at(1);
	muIso1 = muIso->at(0);
        muIso2 = muIso->at(1);     
        isSignalMu1 = isSignalMu->at(0);   
        isSignalMu2 = isSignalMu->at(1);
        isTrueMu1 = isPromptMuon(muType->at(0),muOrigin->at(0));
        isTrueMu2 = isPromptMuon(muType->at(1),muOrigin->at(1));
        //isTrueElec = isPromptElec(elecType->at(0),elecOrigin->at(0));       
        isFakeMu1 = isNonPromptMuon(muType->at(0),muOrigin->at(0));
        isFakeMu2 = isNonPromptMuon(muType->at(1),muOrigin->at(1));
        //isFakeElec = isNonPromptElec(elecType->at(0),elecOrigin->at(0));
        isPhoConvMu1 =  PhoConvMu(muType->at(0),muOrigin->at(0));
        isPhoConvMu2 =  PhoConvMu(muType->at(1),muOrigin->at(1));
        //isPhoConvElec = PhoConvElec(elecType->at(0),elecOrigin->at(0));    
        mu1Charge =  muCharge->at(0);
        mu2Charge = muCharge->at(1);
        pTfrac_mu2_mu1 = mu2Pt/mu1Pt; 
                 
        //cout<<elecType->at(0)<<endl;
        //cout<<muOrigin->at(1)<<endl;      

        //muon channel 
        mu3Pt = -999;
        mu3Eta = -999;
        mu3Phi = -999;
        mu3E = -999;
        isCorrect = -999;
        dRm1m3 = -999; 
        dRm2m3 = -999;
        dimuonM_mu = -999;
        dRm1m3_tru = -2;
        dRm2m3_tru = -2;
        dRm1m2_tru = -2; 

        //elec variables
        elec1Pt =  -999;
	elec1Eta =  -999;
	elec1Phi =  -999;
	elec1E = -999; 
        elec1Charge = -999;      
        pTfrac_e1_mu1 = -999;        
        pTfrac_mu2_e1 = -999;
        pTfrac_mu2_mu1_m2eOS = -999;
        pTfrac_mu2_mu1_m2eSS = -999;
        pTfrac_e1_mu1_m1eSS = -999;
        pTfrac_e1_mu1_m1eOS = -999;
        pTfrac_e1_mu1_m2eSS = -999;
        pTfrac_e1_mu1_m2eOS = -999;

        //jet variables 
        jet1Pt = -999;
        jet2Pt = -999;
        jet1Eta = -999;
        jet2Eta = -999; 
 
        if (jetPt->size() > 1){
         jet1Pt = jetPt->at(0); 
         jet1Eta = jetEta->at(0);
         jet2Pt = jetPt->at(1);
         jet2Eta = jetEta->at(1); 
        }
             
  	//if(this->removeOverlap()) continue;
	isOS = ((muCharge->at(0)*muCharge->at(1))<0);
          bjet1_pt = noKL_jet1_pt;
          bjet1_eta = noKL_jet1_eta;
          bjet1_phi = noKL_jet1_phi;
          bjet1_e = noKL_jet1_e;
          bjet2_pt = noKL_jet2_pt;
          bjet2_eta = noKL_jet2_eta;
          bjet2_phi = noKL_jet2_phi;
          bjet2_e = noKL_jet2_e;
          klfit_bjet1_pt = klfit_jet1_pt;
          klfit_bjet1_eta = klfit_jet1_eta;
          klfit_bjet1_phi = klfit_jet1_phi;
          klfit_bjet1_e = klfit_jet1_e;
          klfit_bjet2_pt = klfit_jet2_pt;
          klfit_bjet2_eta = klfit_jet2_eta;
          klfit_bjet2_phi = klfit_jet2_phi;
          klfit_bjet2_e = klfit_jet2_e;

	dibjet_m = noKL_dijet_m;
        TLorentzVector m1, m2, b1, b2, b1kl, b2kl, amuon, abjet, abjetkl, higgs, higgskl;
        m1.SetPtEtaPhiE(mu1Pt, mu1Eta, mu1Phi, mu1E);
        m2.SetPtEtaPhiE(mu2Pt, mu2Eta, mu2Phi, mu2E);
	b1.SetPtEtaPhiE(bjet1_pt, bjet1_eta, bjet1_phi, bjet1_e);
        b2.SetPtEtaPhiE(bjet2_pt, bjet2_eta, bjet2_phi, bjet2_e);
        b1kl.SetPtEtaPhiE(klfit_bjet1_pt,klfit_bjet1_eta,klfit_bjet1_phi,klfit_bjet1_e);
        b2kl.SetPtEtaPhiE(klfit_bjet2_pt,klfit_bjet2_eta,klfit_bjet2_phi, klfit_bjet2_e);
        amuon = m1 + m2;
	abjet = b1 + b2;
	abjetkl = b1kl + b2kl;
	higgs = amuon + abjet;
	higgskl = amuon + abjetkl;
        
        m_ct = sqrt(pow((mu1E + mu2E),2) - pow((mu1Pt - mu2Pt),2));
	
	int bjetindex = -1;
        int bjetindex2 = -1; //added b-jet index for 2nd bjet
	int centraljetindex = -1;
        int centraljetindex2 = -1; //added another index for 2nd central jet

	for(unsigned int i = 0; i < jetPt->size(); ++i){
	  if(jetPt->at(i) > 30 && fabs(jetEta->at(i)) <= 2.4 && jetMV2c10->at(i) > 0.94){
	    bjetindex = i;
	    break;
	  }
	}
   
        for(unsigned int i = 0; i < jetPt->size(); ++i){
	  if((int)i == bjetindex) continue;
          if(jetPt->at(i) > 30 && fabs(jetEta->at(i)) <= 2.4 && jetMV2c10->at(i) > 0.94){
            bjetindex2 = i;
            break;
          }
        }
        
        for(unsigned int i = 0; i < jetPt->size(); ++i){
	  if((int)i == bjetindex) continue;
          if(jetPt->at(i) > 30 && fabs(jetEta->at(i)) <= 2.4){
            centraljetindex = i;
            break;
          }
        }
   
       for(unsigned int i = 0; i < jetPt->size(); ++i){
	  if((int)i == bjetindex || (int)i == centraljetindex || (int)i == bjetindex2) continue; //enusre the 2nd c-jet is not a b-jet
          if(jetPt->at(i) > 30 && fabs(jetEta->at(i)) <= 2.4){
            centraljetindex2 = i;
            break;
          }
        }

	dPhimumujj = -999;
	if(bjetindex != -1 && centraljetindex != -1){
	  TLorentzVector bjet, cjet, jetjet;
	  bjet.SetPtEtaPhiE(jetPt->at(bjetindex), jetEta->at(bjetindex), jetPhi->at(bjetindex), jetE->at(bjetindex));
          cjet.SetPtEtaPhiE(jetPt->at(centraljetindex), jetEta->at(centraljetindex), jetPhi->at(centraljetindex), jetE->at(centraljetindex));
  	  jetjet = bjet + cjet;
	  dPhimumujj = jetjet.DeltaPhi(amuon);
	}
        
        //angle variables
        dRm1b1 = m1.DeltaR(b1);
        dRm2b1 = m2.DeltaR(b1);
        dRm1b2 = m1.DeltaR(b2);
        dRm2b2 = m2.DeltaR(b2);
        dRm1m2 = m1.DeltaR(m2);
        dRb1b2 = b1.DeltaR(b2);
        dRMmmMbbkl = amuon.DeltaR(abjetkl);
        higgs_pt = higgs.Pt();
	diffRmRb = dRb1b2 - dRm1m2;

        //new angle variables for dimuon system
        dRMmmb1 = amuon.DeltaR(b1);
        dRMmmb2 = amuon.DeltaR(b2);
        dRMmmMbb = amuon.DeltaR(abjet);
        
        isCorrect = 0; 
        if (muOrigin->at(0)==14 && muOrigin->at(1) == 14){
         dRm1m2_tru = m1.DeltaR(m2);
         isCorrect = 1; 
        }
 

       
        //mT transverse reconstruction 
        TLorentzVector met_vector; 
        met_vector.SetPxPyPzE(met_px, met_py ,0,met_met);
        //TLorentzVector Wlep = met_vector+ m1;
        //mT_W = Wlep.Mt();
        mT_W = sqrt(2*mu1Pt*met_met*(1-cos(met_vector.DeltaPhi(m1)))) ;
        float mT_W_m2 = sqrt(2*mu2Pt*met_met*(1-cos(met_vector.DeltaPhi(m2)))) ;
 
        mT_W_tru = -999;
        if (muOrigin->at(0)== 10 || muOrigin->at(0) == 15){
          TLorentzVector met_vector_tru;
          met_vector_tru.SetPxPyPzE(met_px, met_py ,0,met_met);    
          //TLorentzVector Wlep_tru = met_vector_tru + m1;
          //mT_W_tru = Wlep_tru.Mt();
          mT_W_tru = sqrt(2*mu1Pt*met_met*(1-cos(met_vector.DeltaPhi(m1)))) ;   
        }
                
        //for muon channel
        TLorentzVector m3; 
      
        if (nMuons_signal > 2){
         mu3Pt =  muPt->at(2);
         mu3Eta =  muEta->at(2);
         mu3Phi =  muPhi->at(2);
         mu3E =  muE->at(2);
         m3.SetPtEtaPhiE(mu3Pt, mu3Eta, mu3Phi,mu3E);  
       
         dRm1m3 =  m1.DeltaR(m3);
         dRm2m3 = m2.DeltaR(m3);
         float mT_W_m3 = sqrt(2*mu3Pt*met_met*(1-cos(met_vector.DeltaPhi(m3)))) ;

         isCorrect = 0; 
         //cout<<isOS<<endl;

         //algo1 : last two OS sign are signal ones, if last two are not OS then 1 and 3
         /*
         
         if ((muCharge->at(1)*muCharge->at(2))<0){ // if 2nd and 3rd OS, pair them
          dimuonM_mu = (m2 + m3).M();
          if (muOrigin->at(1)==14 && muOrigin->at(2) == 14){
           isCorrect = 1;
           dRm2m3_tru = m2.DeltaR(m3);}
         }
         else if ((muCharge->at(0)*muCharge->at(2))<0){ // if 1st and 3rd OS, pair them
          dimuonM_mu = (m1 + m3).M();
          if (muOrigin->at(0)==14 && muOrigin->at(2) == 14){
           isCorrect = 1;
           dRm1m3_tru =  m1.DeltaR(m3); }
         }//end algo1
         
          */

         //algo 2: low dR algo
         /* 
         if((muCharge->at(0)*muCharge->at(1))<0 ){//if first two are opposite sign, only one choice..pair with m3 with OS one. 
          if ((muCharge->at(1)*muCharge->at(2))<0){ // if 2nd and 3rd OS, pair them
	   dimuonM_mu = (m2 + m3).M();
           if (muOrigin->at(1)==14 && muOrigin->at(2) == 14){
            isCorrect = 1; 
            dRm2m3_tru = m2.DeltaR(m3);}      
          }
          else if ((muCharge->at(0)*muCharge->at(2))<0){ // if 1st and 3rd OS, pair them 
           dimuonM_mu = (m1 + m3).M();	  	
	   if (muOrigin->at(0)==14 && muOrigin->at(2) == 14){
	    isCorrect = 1;  
            dRm1m3_tru =  m1.DeltaR(m3); }
           }
 	 }  
	 else if (dRm2m3 <=  dRm1m3){ // if first 2 are SS, then 3rd could be paired with 1 or 2. We pick one with lower dR
	    dimuonM_mu = (m2 + m3).M();
            if (muOrigin->at(1)==14 && muOrigin->at(2) == 14){
	     isCorrect =1; 
             dRm2m3_tru =  m2.DeltaR(m3);}   	
         }  
         else{
            dimuonM_mu = (m1 + m3).M();
            if (muOrigin->at(0)==14 && muOrigin->at(2) == 14){
             isCorrect = 1;
             dRm1m3_tru =  m1.DeltaR(m3);}   	 
         } */ //end algo 2 
         
         // algo 1b figure out where the OS muon is and assign it as the first candidate. This gives exact same result at algo 1 
         
         /*
         int isM1 = 0;
         int isM2 = 0;
         int isM3 = 0; 
         if((muCharge->at(0)*muCharge->at(1))>0 )//if first two are SS
           isM3 = 1; //if first two SS, then mu3 is OS 
         else if( (muCharge->at(1)*muCharge->at(2))>0) //if mu2 and mu3 are SS then mu1 is OS
           isM1 = 1; 
         else if((muCharge->at(0)*muCharge->at(2))>0) // if mu1 and mu3 are SS then mu2 is OS
           isM2 = 1;  

         if (isM3 > 0){ // if mu3 pair it with m2
           if ((m1 + m3).M() > 140 && (m2 + m3).M() < 140){
               dimuonM_mu = (m2 + m3).M();
               if (muOrigin->at(1)==14 && muOrigin->at(2) == 14)
                isCorrect = 1;}
           else if ((m2 + m3).M() > 140 && (m1 + m3).M() < 140){
               dimuonM_mu = (m1 + m3).M();
               if (muOrigin->at(0)==14 && muOrigin->at(2) == 14)
                isCorrect = 1;}     
           else{
           dimuonM_mu = (m2 + m3).M();
           if (muOrigin->at(1)==14 && muOrigin->at(2) == 14){
            isCorrect = 1;
            dRm2m3_tru = m2.DeltaR(m3);}
           }
         }
         else if (isM1 >0){ // if mu1, then pair it with m3
           if ((m1 + m3).M() > 140 && (m1 + m2).M() < 140){
              dimuonM_mu = (m1+ m2).M();
              if (muOrigin->at(0)==14 && muOrigin->at(1) == 14)
               isCorrect = 1;}
           else if ((m1 + m2).M() > 140 && (m1 + m3).M() < 140){
              dimuonM_mu = (m1 + m3).M();
              if (muOrigin->at(0)==14 && muOrigin->at(2) == 14)
              isCorrect = 1;} 
           else{
           dimuonM_mu = (m1 + m3).M();
           if (muOrigin->at(0)==14 && muOrigin->at(2) == 14){
            isCorrect = 1;
            dRm1m3_tru =  m1.DeltaR(m3); }
           }
         }
         else if(isM2>0 ){ // if mu2, then pair it with m3
            if ((m2 + m3).M() > 140 && (m1 + m2).M() < 140){
              dimuonM_mu = (m1+ m2).M();
              if (muOrigin->at(0)==14 && muOrigin->at(1) == 14)
               isCorrect = 1;}
            else if ((m1 + m2).M() > 140 && (m2 + m3).M() < 140){
              dimuonM_mu = (m2 + m3).M();
              if (muOrigin->at(1)==14 && muOrigin->at(2) == 14)
               isCorrect = 1;} 
            else{
            dimuonM_mu = (m2 + m3).M();
            if (muOrigin->at(1)==14 && muOrigin->at(2) == 14){
             isCorrect =1;
             dRm2m3_tru =  m2.DeltaR(m3);}
           }
         } 
         
         //end algo 1b 
         */

         //algo 3
         /*
   	 int isM1 = 0;
         int isM2 = 0;
         int isM3 = 0;
         if((muCharge->at(0)*muCharge->at(1))>0 )//if first two are SS
           isM3 = 1; //if first two SS, then mu3 is OS
         else if( (muCharge->at(1)*muCharge->at(2))>0) //if mu2 and mu3 are SS then mu1 is OS
           isM1 = 1;
         else if((muCharge->at(0)*muCharge->at(2))>0) // if mu1 and mu3 are SS then mu2 is OS
           isM2 = 1;

         if (isM3 > 0){ //
          if (dRm2m3 <=  dRm1m3){ // 3rd could be paired with 1 or 2. pick one with lower dR
            dimuonM_mu = (m2 + m3).M();
            if (muOrigin->at(1)==14 && muOrigin->at(2) == 14){
             isCorrect =1;
             dRm2m3_tru =  m2.DeltaR(m3);}
          }
          else{
            dimuonM_mu = (m1 + m3).M();
            if (muOrigin->at(0)==14 && muOrigin->at(2) == 14){
             isCorrect =1;
             dRm1m3_tru =  m1.DeltaR(m3);} 
          }
         }//end ifM3    
         else if (isM1 >0){ // if mu1, then pair lowest dR
          if (dRm1m2 <=  dRm1m3){ 
            dimuonM_mu = (m1 + m2).M();
            if (muOrigin->at(0)==14 && muOrigin->at(1) == 14)
             isCorrect =1;
          } 
          else{
            dimuonM_mu = (m1 + m3).M();
            if (muOrigin->at(0)==14 && muOrigin->at(2) == 14){
             isCorrect =1;
             dRm1m3_tru =  m1.DeltaR(m3);}
          }  
         }
         else if(isM2>0){ // if mu2, pair with lowest dR
          if (dRm1m2 <=  dRm2m3){ 
            dimuonM_mu = (m1 + m2).M();
            if (muOrigin->at(0)==14 && muOrigin->at(1) == 14)
             isCorrect =1;
          }
          else{
            dimuonM_mu = (m2 + m3).M();
            if (muOrigin->at(1)==14 && muOrigin->at(2) == 14){
             isCorrect =1;
             dRm2m3_tru =  m2.DeltaR(m3);}
          }         
         } //end algo3 
        */

         //algo4 
         ///*
         int isM1 = 0;
         int isM2 = 0;
         int isM3 = 0;
         if((muCharge->at(0)*muCharge->at(1))>0 )//if first two are SS
           isM3 = 1; //if first two SS, then mu3 is OS
         else if( (muCharge->at(1)*muCharge->at(2))>0) //if mu2 and mu3 are SS then mu1 is OS
           isM1 = 1;
         else if((muCharge->at(0)*muCharge->at(2))>0) // if mu1 and mu3 are SS then mu2 is OS
           isM2 = 1;
         
         int isWcand_m1 = 0;
         int isWcand_m2 =0;
         int isWcand_m3 =0;
  
         if (mT_W > 45 && mT_W < 130)
            isWcand_m1 = 1 ;            
         if (mT_W_m2 > 45 && mT_W_m2 < 130)
            isWcand_m2 = 1;
         if (mT_W_m3 > 45 && mT_W_m3 < 130)
            isWcand_m3 = 1;
                       
         
         if (isM3 > 0){ // if mu3 pair it muon what doesn't give W mass.
           if ((m1 + m3).M() > 140 && (m2 + m3).M() < 120){ // first check if one of two options is giving some crazy high mass 
               dimuonM_mu = (m2 + m3).M();
               if (muOrigin->at(1)==14 && muOrigin->at(2) == 14)
                isCorrect = 1;}				   
           else if ((m2 + m3).M() > 140 && (m1 + m3).M() < 120){
               dimuonM_mu = (m1 + m3).M();
	       if (muOrigin->at(0)==14 && muOrigin->at(2) == 14)
                isCorrect = 1;}	          
           else if (isWcand_m1 == 0 &&  isWcand_m2 == 1){//if only m2 giving W mass,pair m3 with m1 
              dimuonM_mu = (m1 + m3).M();
              if (muOrigin->at(0)==14 && muOrigin->at(2) == 14){
               isCorrect = 1;
               dRm1m3_tru = m1.DeltaR(m3);}
              //if (dimuonM_mu > 140 ) cout << dimuonM_mu << "is correct:" << isCorrect << (m2 + m3).M() <<endl;
           }        
           else { // else pair with m2 
              dimuonM_mu = (m2 + m3).M();
              if (muOrigin->at(1)==14 && muOrigin->at(2) == 14){
               isCorrect = 1;
               dRm2m3_tru = m2.DeltaR(m3);}
              //if (dimuonM_mu > 140 ) cout << dimuonM_mu << "is correct:" << isCorrect << (m1 + m3).M()<< endl;
           }
         }
         else if (isM1 >0){ // if mu1
          if ((m1 + m3).M() > 140 && (m1 + m2).M() < 120){ // first check if one of two options is giving some crazy high mass 
              dimuonM_mu = (m1+ m2).M();
              if (muOrigin->at(0)==14 && muOrigin->at(1) == 14)
               isCorrect = 1;}				   
          else if ((m1 + m2).M() > 140 && (m1 + m3).M() < 120){
              dimuonM_mu = (m1 + m3).M();
	      if (muOrigin->at(0)==14 && muOrigin->at(2) == 14)
               isCorrect = 1;}
          else if (isWcand_m3 == 1 &&  isWcand_m2 == 0){//if only m3 giving W mass,pair m1 with m2
              dimuonM_mu = (m1 + m2).M();
              if (muOrigin->at(0)==14 && muOrigin->at(1) == 14){
               isCorrect = 1;}
              //if (dimuonM_mu > 140) cout << dimuonM_mu << "is correct:" << isCorrect << (m1 + m3).M()<< endl ; 
          }
          else{
           dimuonM_mu = (m1 + m3).M();
           if (muOrigin->at(0)==14 && muOrigin->at(2) == 14){
            isCorrect = 1;
            dRm1m3_tru =  m1.DeltaR(m3); }
           //if (dimuonM_mu > 140) cout << dimuonM_mu << "is correct:" << isCorrect << (m1 + m2).M()<< endl;
          }
         }
         else if(isM2>0){ // if mu2
          if ((m2 + m3).M() > 140 && (m1 + m2).M() < 120){ // first check if one of two options is giving some crazy high mass
              dimuonM_mu = (m1+ m2).M();
              if (muOrigin->at(0)==14 && muOrigin->at(1) == 14)
               isCorrect = 1;}
          else if ((m1 + m2).M() > 140 && (m2 + m3).M() < 120){
              dimuonM_mu = (m2 + m3).M();
              if (muOrigin->at(1)==14 && muOrigin->at(2) == 14)
               isCorrect = 1;} 
          else if (isWcand_m3 == 1 &&  isWcand_m1 == 0){//if only m3 giving W mass,pair m2 with m1
              dimuonM_mu = (m1 + m2).M();
              if (muOrigin->at(0)==14 && muOrigin->at(1) == 14){
               isCorrect = 1;}
              //if (dimuonM_mu > 140) cout << dimuonM_mu << "is correct:" << isCorrect <<(m3 + m2).M()<< endl; 
          }
          else{
            dimuonM_mu = (m2 + m3).M();
            if (muOrigin->at(1)==14 && muOrigin->at(2) == 14){
             isCorrect =1;
             dRm2m3_tru =  m2.DeltaR(m3);}
            //if (dimuonM_mu > 140) cout << dimuonM_mu << "is correct:" << isCorrect << (m1 + m2).M() <<endl; 
           }
         } //end algo4
 
        

        }//if for muon channel 
	

        if (isCorrect ==1)
         n_correctassign.push_back(1);


        //for electron channel 
        dRMmme1 = -999;
        dRm1e1 = -999; 
        dRm2e1 = -999; 
        Mmu1e = -999;
        Mmu2e = -999;
        mT_W_e1 = -999; 
        TLorentzVector e1;  
        Mmu1e_os = -999;
        Mmu2e_os = -999;
        Mmu1e_ss = -999;
        Mmu2e_ss = -999;

             
        //if electron in event, setup the 4 vector and get angle with a1 and muons. plus also get Mmue to get the fake dilepton
        if (nElectrons_signal > 0){ 
	  elec1Pt =  elecPt->at(0);
	  elec1Eta =  elecEta->at(0);
	  elec1Phi =  elecPhi->at(0);
	  elec1E =  elecE->at(0);
          elec1Charge = elecCharge->at(0);       
    
          e1.SetPtEtaPhiE(elec1Pt, elec1Eta, elec1Phi, elec1E);
          dRMmme1 = amuon.DeltaR(e1);
          dRm1e1 = m1.DeltaR(e1);
          dRm2e1 = m2.DeltaR(e1); 
          Mmu1e = (e1 + m1).M();
          Mmu2e = (e1 + m2).M();     
          mT_W_e1 = sqrt(2*elec1Pt*met_met*(1-cos(met_vector.DeltaPhi(e1)))) ;
          pTfrac_e1_mu1 = elec1Pt/mu1Pt; 
          pTfrac_mu2_e1 = mu2Pt/elec1Pt;        


       
          if ((elecCharge->at(0)*muCharge->at(0))<0){ // elec and muon are opposite sign.  
           Mmu1e_os = (e1 + m1).M();
           pTfrac_e1_mu1_m1eOS = elec1Pt/mu1Pt; 
          } 
          else if ((elecCharge->at(0)*muCharge->at(0))>0){ //elec and muon are same sign. 
           Mmu1e_ss = (e1 + m1).M();
           pTfrac_e1_mu1_m1eSS = elec1Pt/mu1Pt; 
          }  
          
          if ((elecCharge->at(0)*muCharge->at(1))<0){
           Mmu2e_os = (e1 + m2).M();
           pTfrac_mu2_mu1_m2eOS = mu2Pt/mu1Pt; 
           pTfrac_e1_mu1_m2eOS = elec1Pt/mu1Pt; 
          }          
          else if ((elecCharge->at(0)*muCharge->at(1))>0){
           Mmu2e_ss = (e1 + m2).M();
           pTfrac_mu2_mu1_m2eSS = mu2Pt/mu1Pt; 
           pTfrac_e1_mu1_m2eSS = elec1Pt/mu1Pt; 
          } 
        }

     
       
        //if 2 central jets, try to reconstruct W and get angle with A. Also reconstruct the H+
        Wmass = -999;
        hplus_m = -999; 
        dRMmmW = -999;   
       	if(centraljetindex != -1 && centraljetindex2 != -1 ){
	  TLorentzVector cjet1, cjet2, di_cjets;
	  cjet1.SetPtEtaPhiE(jetPt->at(centraljetindex), jetEta->at(centraljetindex), jetPhi->at(centraljetindex), jetE->at(centraljetindex));
          cjet2.SetPtEtaPhiE(jetPt->at(centraljetindex2), jetEta->at(centraljetindex2), jetPhi->at(centraljetindex2), jetE->at(centraljetindex2));
  	  di_cjets = cjet1 + cjet2;
          Wmass = di_cjets.M();
          dRMmmW = amuon.DeltaR(di_cjets);
          hplus_m = (di_cjets + amuon).M();  
	} 
        
        //another way to get the W
        //TLorentzVector dijet; 
        //dijet.SetPtEtaPhiE(dijetPt,dijetEta,dijetPhi,dijetE);
        //Wmass2 = dijet.M();
        //dRMmmW2 = amuon.DeltaR(dijet);
        //hplus_m2 = (dijet + amuon).M();

	float massm1b1 = (m1+b1).M();
	float massm2b1 = (m2+b1).M();
	float massm1b2 = (m1+b2).M();
	float massm2b2 = (m2+b2).M();
                
 	vector<float> dR = {dRm1b1, dRm2b1, dRm1b2, dRm2b2};	
	vector<float> dRm = {massm1b1, massm2b1, massm1b2, massm2b2};
	orderAngles(dR,dRm);
	dRmb1 = dR[0];
	dRmb2 = dR[1];
	dRmb3 = dR[2];
	dRmb4 = dR[3];
 	
	mdRmb1 = dRm[0];	
	mdRmb2 = dRm[1];	
	mdRmb3 = dRm[2];	
	mdRmb4 = dRm[3];	

	Ht = m1.Pt() + m2.Pt() + b1.Pt() + b2.Pt();
	centrality = higgs_pt/Ht;	

	TLorentzVector allObjects;
	float alljetPt = 0;
	int leadFwdJetindex = -1;
        int leadJetindex = -1;
        int leadBJetindex = -1;
	for(unsigned int i = 0; i<jetPt->size();++i){
	  if(fabs(jetEta->at(i)) < 2.5){
	    alljetPt += jetPt->at(i);
	    TLorentzVector jet;
	    jet.SetPtEtaPhiM(jetPt->at(i), jetEta->at(i), jetPhi->at(i), jetM->at(i));
	    allObjects += jet;
	  }
	  if(fabs(jetEta->at(i)) > 2.4 && jetPt->at(i)>30 && leadFwdJetindex == -1){
	    leadFwdJetindex = i;
	  }
	  if(fabs(jetEta->at(i)) < 2.4 && jetPt->at(i)>30 && leadJetindex == -1){
            leadJetindex = i;
          }
          if(fabs(jetEta->at(i)) < 2.4 && jetPt->at(i)>30 && jetMV2c10->at(i) > 0.94 &&leadBJetindex == -1){
            leadBJetindex = i;
          }
	}
	leadFwdJetPt = -1;
	leadFwdJetEta = -999;
	if(leadFwdJetindex != -1){
	  leadFwdJetPt = jetPt->at(leadFwdJetindex);
	  leadFwdJetEta = jetEta->at(leadFwdJetindex);
	}
        leadCJetPt = -1;
        leadCJetEta = -999;
        if(leadJetindex != -1){
          leadCJetPt = jetPt->at(leadJetindex);
          leadCJetEta = jetEta->at(leadJetindex);
        }
        leadBJetPt = -1;
        leadBJetEta = -999;
        if(leadBJetindex != -1){
          leadBJetPt = jetPt->at(leadBJetindex);
          leadBJetEta = jetEta->at(leadBJetindex);
        }

	allObjects += (m1 + m2);
        
                  
	HtAll = m1.Pt() + m2.Pt() + alljetPt;
	centralityAll = allObjects.Pt()/HtAll;
	//cout << "before fill: " << event_weight << endl;
        
        for(unsigned int i = 0; i<BDT_configs.size(); i++){
          if(EventNumber%2==1) BDT_outputs[i] = reader->EvaluateMVA(BDT_configs[i].name+"train0");
	  else BDT_outputs[i] = reader->EvaluateMVA(BDT_configs[i].name+"train1");
        }
	if(doCMS){
	  tree2->Fill();
	}
	else{
          //if(nBJet20_MV2c10_FixedCutBEff_77==0&&write0Tree) tree0->Fill();
          //if(nBJet20_MV2c10_FixedCutBEff_77==1&&write1Tree) tree1->Fill();
          //if(nBJet20_MV2c10_FixedCutBEff_77>=2) tree2->Fill();
          tree2->Fill();          
	}
	//cout <<"test8"<<endl;
  	}
  // cout <<"test9"<<endl;
  }
outputfile->cd();
if(!doCMS){
  if(write0Tree) tree0->Write("",TObject::kOverwrite);
  if(write1Tree) tree1->Write("",TObject::kOverwrite);
}
tree2->Write("",TObject::kOverwrite);

for (auto hists : mapOfHists){
  hists.second->Write();
}
for (auto hists : mapOfUnweightedHists){
  hists.second->Write();
}

for(auto hists : mapOfHists){
  
cout << "a_01:" << a_01.size() << endl;
cout << "a_12:" << a_12.size() << endl;
cout << "a_02:" << a_02.size() << endl;
float total_a1 =  a_01.size() + a_12.size() + a_02.size(); 
cout << "toal a1 events:" << total_a1 << endl; 
cout<<"correct assignment: " << n_correctassign.size() << endl;
if (total_a1 > 0) {
cout<<"% correct assign: "<<  n_correctassign.size()/total_a1 << endl;

}
  TH1 *h_weight = hists.second;
  TH1 *h_unweight = mapOfUnweightedHists[hists.first];
  cout << "Printing cutflow for DSID " << hists.first << endl;
  cout.width(50);
  cout << "Cut" << "\t" << "# events" << "\t" << "SOW" << "\t" << "Rel.eff.unw." << "\t" << "Rel.eff.w." << "\t"
  << "Tot.eff.unw." << "\t" << "Tot.eff.w."
  << "\t" << "Rel.skim.eff.unw." << "\t" << "Rel.skim.eff.w." << "\t" 
  << endl;
  float SOW = h_weight->GetBinContent(1);
  float SOE = h_unweight->GetBinContent(1);
  float passXAODW = 0;
  float passXAODU = 0;
  int nBins = h_weight->GetNbinsX(); 
  for(int i=2; i<=nBins; ++i){
    TString label = h_weight->GetXaxis()->GetBinLabel(i);
    if(label=="") continue;
    if("passNobjectCuts"==label){
      passXAODW = h_weight->GetBinContent(i);
      passXAODU = h_unweight->GetBinContent(i);
    }
    float binWeightedB = h_weight->GetBinContent(i-1);
    float binUnweightedB = h_unweight->GetBinContent(i-1);
    float binWeighted = h_weight->GetBinContent(i);
    float binUnweighted = h_unweight->GetBinContent(i);
    cout.width(50);
    cout << setprecision(1) << label << "\t"<< binUnweighted << "\t" << setprecision(2)<<binWeighted << "\t" << binUnweighted/binUnweightedB << "\t" << binWeighted/binWeightedB << "\t" 
    << binUnweighted/SOE << "\t" << binWeighted/SOW << "\t" 
    << binUnweighted/passXAODU << "\t" << binWeighted/passXAODW
    << endl;
  }
}

cutManager->list();
gROOT->cd();
cout << endl;
}


SkimSlim::SkimSlim() : fChain(0) 
{

   outputfile = 0;
   newTreeName1 = "";
   newTreeName2 = "";
   newTreeName3 = "";
   lumiWeight = 1;
   lumi = -1;
   event_weight = -999;
   isData = false;
   maxEvents = -1;
   write0Tree = false;
   write1Tree = false;
   cutManager = new CutManager("cutManager");
   cutManager->setEventWeight(eventWeightCuts);
}

SkimSlim::~SkimSlim()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t SkimSlim::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t SkimSlim::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
   }
   return centry;
}

void SkimSlim::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).
   
   // Set object pointer
   muPt = 0;
   muEta = 0;
   muPhi = 0;
   muM = 0;
   muE = 0;
   muIso = 0;
   muCharge = 0;
   elecPt = 0;
   elecEta = 0;
   elecPhi = 0;
   elecM = 0;
   elecE = 0;
   elecIso = 0;
   elecCharge = 0;
   jetPt = 0;
   jetEta = 0;
   jetPhi = 0;
   jetM = 0;
   jetE = 0;
   jetJvt = 0;
   jetMV2c10 = 0;
   trkjetPt = 0;
   trkjetEta = 0;
   trkjetPhi = 0;
   trkjetM = 0;
   trkjetE = 0;
   trkjetNTrks500 = 0;
   trkjetNTrks1000 = 0;
   trkjetMV2c10 = 0;
   bjetPt = 0;
   bjetEta = 0;
   bjetPhi = 0;
   bjetM = 0;
   bjetE = 0;
   bjetJvt = 0;
   bjetMV2c10 = 0;
   isSignalMu = 0;
   muType = 0;
   muOrigin = 0;
   elecType = 0;
   elecOrigin = 0;  
  
   mutrigMCEff_singleMuonTrig = 0;
   mutrigDataEff_singleMuonTrig = 0;
   mutrigMCEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1down = 0;
   mutrigDataEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1down = 0;
   mutrigMCEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1up = 0;
   mutrigDataEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1up = 0;
   mutrigMCEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1down = 0;
   mutrigDataEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1down = 0;
   mutrigMCEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1up = 0;
   mutrigDataEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1up = 0;
   mutrigMCEff_symdiMuonTrig = 0;
   mutrigDataEff_symdiMuonTrig = 0;
   mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1down = 0;
   mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1down = 0;
   mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1up = 0;
   mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1up = 0;
   mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1down = 0;
   mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1down = 0;
   mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1up = 0;
   mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1up = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   m_branchList = fChain->GetListOfBranches();
   //fChain->SetMakeClass(1);
   fChain->SetBranchStatus("*",0);

for(auto& theoryName : theoryMap){
  setBranchAddress(theoryName.second.weightName.Data(), &theoryName.second.weight);
}

setBranchAddress("mutrigMCEff_singleMuonTrig", &mutrigMCEff_singleMuonTrig);
setBranchAddress("mutrigDataEff_singleMuonTrig", &mutrigDataEff_singleMuonTrig);
setBranchAddress("mutrigMCEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1down", &mutrigMCEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1down);
setBranchAddress("mutrigDataEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1down", &mutrigDataEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1down);
setBranchAddress("mutrigMCEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1up", &mutrigMCEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1up);
setBranchAddress("mutrigDataEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1up", &mutrigDataEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1up);
setBranchAddress("mutrigMCEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1down", &mutrigMCEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1down);
setBranchAddress("mutrigDataEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1down", &mutrigDataEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1down);
setBranchAddress("mutrigMCEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1up", &mutrigMCEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1up);
setBranchAddress("mutrigDataEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1up", &mutrigDataEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1up);
setBranchAddress("mutrigMCEff_symdiMuonTrig", &mutrigMCEff_symdiMuonTrig);
setBranchAddress("mutrigDataEff_symdiMuonTrig", &mutrigDataEff_symdiMuonTrig);
setBranchAddress("mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1down", &mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1down);
setBranchAddress("mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1down", &mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1down);
setBranchAddress("mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1up", &mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1up);
setBranchAddress("mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1up", &mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1up);
setBranchAddress("mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1down", &mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1down);
setBranchAddress("mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1down", &mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1down);
setBranchAddress("mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1up", &mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1up);
setBranchAddress("mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1up", &mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1up);
setBranchAddress("trigWeight_singleMuonTrig",&trigWeight_singleMuonTrig);
setBranchAddress("trigMatch_singleMuonTrig",&trigMatch_singleMuonTrig);
setBranchAddress("trigWeight_singleElecTrig",&trigWeight_singleElecTrig);
setBranchAddress("trigMatch_singleElecTrig",&trigMatch_singleElecTrig);
setBranchAddress("trigWeight_symdiMuonTrig",&trigWeight_symdiMuonTrig);
setBranchAddress("trigMatch_symdiMuonTrig",&trigMatch_symdiMuonTrig);
setBranchAddress("trigWeight_asymdiMuonTrig",&trigWeight_asymdiMuonTrig);
setBranchAddress("trigMatch_asymdiMuonTrig",&trigMatch_asymdiMuonTrig);
setBranchAddress("trigWeight_symdiElecTrig",&trigWeight_symdiElecTrig);
setBranchAddress("trigMatch_symdiElecTrig",&trigMatch_symdiElecTrig);
setBranchAddress("trigWeight_symdiLepTrig",&trigWeight_symdiLepTrig);
setBranchAddress("trigMatch_symdiLepTrig",&trigMatch_symdiLepTrig);
setBranchAddress("trigWeight_asymdiLepTrig",&trigWeight_asymdiLepTrig);
setBranchAddress("trigMatch_asymdiLepTrig",&trigMatch_asymdiLepTrig);
setBranchAddress("HLT_e24_lhmedium_L1EM20VH",&HLT_e24_lhmedium_L1EM20VH);
setBranchAddress("HLT_e24_lhvloose_nod0",&HLT_e24_lhvloose_nod0);
setBranchAddress("HLT_e24_lhtight_nod0_ivarloose",&HLT_e24_lhtight_nod0_ivarloose);
setBranchAddress("HLT_e26_lhtight_nod0_ivarlooseHLT_e60_medium",&HLT_e26_lhtight_nod0_ivarlooseHLT_e60_medium);
setBranchAddress("HLT_e60_lhmedium",&HLT_e60_lhmedium);
setBranchAddress("HLT_e60_lhmedium_nod0",&HLT_e60_lhmedium_nod0);
setBranchAddress("HLT_e120_lhloose",&HLT_e120_lhloose);
setBranchAddress("HLT_e140_lhloose_nod0",&HLT_e140_lhloose_nod0);
setBranchAddress("HLT_e300_etcut",&HLT_e300_etcut);
setBranchAddress("HLT_2e15_lhvloose_nod0_L12EM13VH",&HLT_2e15_lhvloose_nod0_L12EM13VH);
setBranchAddress("HLT_2e17_lhvloose_nod0_L12EM15VHI",&HLT_2e17_lhvloose_nod0_L12EM15VHI);
setBranchAddress("HLT_2e24_lhvloose_nod0",&HLT_2e24_lhvloose_nod0);
setBranchAddress("HLT_e7_lhmedium_mu24",&HLT_e7_lhmedium_mu24);
setBranchAddress("HLT_e7_lhmedium_nod0_mu24",&HLT_e7_lhmedium_nod0_mu24);
setBranchAddress("HLT_e17_lhloose_mu14",&HLT_e17_lhloose_mu14);
setBranchAddress("HLT_e17_lhloose_nod0_mu14",&HLT_e17_lhloose_nod0_mu14);
setBranchAddress("HLT_e24_lhmedium_L1EM20VHI_mu8noL1",&HLT_e24_lhmedium_L1EM20VHI_mu8noL1);
setBranchAddress("HLT_e26_lhmedium_nod0_mu8noL1",&HLT_e26_lhmedium_nod0_mu8noL1);
setBranchAddress("HLT_e26_lhmedium_nod0_L1EM22VHI_mu8noL1",&HLT_e26_lhmedium_nod0_L1EM22VHI_mu8noL1);
setBranchAddress("HLT_mu20_iloose_L1MU15",&HLT_mu20_iloose_L1MU15);
setBranchAddress("HLT_mu24_iloose",&HLT_mu24_iloose);
setBranchAddress("HLT_mu24_iloose_L1MU15",&HLT_mu24_iloose_L1MU15);
setBranchAddress("HLT_mu24_ivarloose_L1MU15",&HLT_mu24_ivarloose_L1MU15);
setBranchAddress("HLT_mu24_imedium",&HLT_mu24_imedium);
setBranchAddress("HLT_mu24_ivarmedium",&HLT_mu24_ivarmedium);
setBranchAddress("HLT_mu26_ivarmedium",&HLT_mu26_ivarmedium);
setBranchAddress("HLT_mu40",&HLT_mu40);
setBranchAddress("HLT_mu50",&HLT_mu50);
setBranchAddress("HLT_mu60",&HLT_mu60);
setBranchAddress("HLT_2mu10",&HLT_2mu10);
setBranchAddress("HLT_2mu14",&HLT_2mu14);
setBranchAddress("HLT_mu18_mu8noL1",&HLT_mu18_mu8noL1);
setBranchAddress("HLT_mu20_mu8noL1",&HLT_mu20_mu8noL1);
setBranchAddress("HLT_mu22_mu8noL1",&HLT_mu22_mu8noL1);
setBranchAddress("TrigCatHaa",&TrigCatHaa);
setBranchAddress("nMuons_base",&nMuons_base);
setBranchAddress("nMuons_signal",&nMuons_signal);
setBranchAddress("nElectrons_base",&nElectrons_base);
setBranchAddress("nElectrons_signal",&nElectrons_signal);
setBranchAddress("isBadMuon",&isBadMuon);
setBranchAddress("nVtx",&nVtx);
setBranchAddress("mu",&mu);
setBranchAddress("isSignalMu",&isSignalMu);
setBranchAddress("muType",&muType);
setBranchAddress("muOrigin",&muOrigin);
setBranchAddress("elecType",&elecType);
setBranchAddress("elecOrigin",&elecOrigin);
setBranchAddress("nJet20",&nJet20);
setBranchAddress("nBJet20_MV2c10_FixedCutBEff_77",&nBJet20_MV2c10_FixedCutBEff_77);
setBranchAddress("nBJet20_MV2c10_FixedCutBEff_60",&nBJet20_MV2c10_FixedCutBEff_60);
setBranchAddress("nBJet20_MV2c10_FixedCutBEff_70",&nBJet20_MV2c10_FixedCutBEff_70);
setBranchAddress("nBJet20_MV2c10_FixedCutBEff_85",&nBJet20_MV2c10_FixedCutBEff_85);
setBranchAddress("muPt",&muPt);
setBranchAddress("muEta",&muEta);
setBranchAddress("muPhi",&muPhi);
setBranchAddress("muM",&muM);
setBranchAddress("muE",&muE);
setBranchAddress("muIso",&muIso);
setBranchAddress("muCharge",&muCharge);
setBranchAddress("dimuonPt",&dimuonPt);
setBranchAddress("dimuonEta",&dimuonEta);
setBranchAddress("dimuonPhi",&dimuonPhi);
setBranchAddress("dimuonM",&dimuonM);
setBranchAddress("dimuonE",&dimuonE);
setBranchAddress("elecPt",&elecPt);
setBranchAddress("elecEta",&elecEta);
setBranchAddress("elecPhi",&elecPhi);
setBranchAddress("elecM",&elecM);
setBranchAddress("elecE",&elecE);
setBranchAddress("elecIso",&elecIso);
setBranchAddress("elecCharge",&elecCharge);
setBranchAddress("dielecPt",&dielecPt);
setBranchAddress("dielecEta",&dielecEta);
setBranchAddress("dielecPhi",&dielecPhi);
setBranchAddress("dielecM",&dielecM);
setBranchAddress("dielecE",&dielecE);
setBranchAddress("emuPt",&emuPt);
setBranchAddress("emuEta",&emuEta);
setBranchAddress("emuPhi",&emuPhi);
setBranchAddress("emuM",&emuM);
setBranchAddress("emuE",&emuE);
setBranchAddress("jetPt",&jetPt);
setBranchAddress("jetEta",&jetEta);
setBranchAddress("jetPhi",&jetPhi);
setBranchAddress("jetM",&jetM);
setBranchAddress("jetE",&jetE);
setBranchAddress("jetJvt",&jetJvt);
setBranchAddress("jetMV2c10",&jetMV2c10);
setBranchAddress("trkjetPt",&trkjetPt);
setBranchAddress("trkjetEta",&trkjetEta);
setBranchAddress("trkjetPhi",&trkjetPhi);
setBranchAddress("trkjetM",&trkjetM);
setBranchAddress("trkjetE",&trkjetE);
setBranchAddress("trkjetNTrks500",&trkjetNTrks500);
setBranchAddress("trkjetNTrks1000",&trkjetNTrks1000);
setBranchAddress("trkjetMV2c10",&trkjetMV2c10);
setBranchAddress("dijetPt",&dijetPt);
setBranchAddress("dijetEta",&dijetEta);
setBranchAddress("dijetPhi",&dijetPhi);
setBranchAddress("dijetM",&dijetM);
setBranchAddress("dijetE",&dijetE);
setBranchAddress("bjetPt",&bjetPt);
setBranchAddress("bjetEta",&bjetEta);
setBranchAddress("bjetPhi",&bjetPhi);
setBranchAddress("bjetM",&bjetM);
setBranchAddress("bjetE",&bjetE);
setBranchAddress("bjetJvt",&bjetJvt);
setBranchAddress("bjetMV2c10",&bjetMV2c10);
setBranchAddress("dibjetPt",&dibjetPt);
setBranchAddress("dibjetEta",&dibjetEta);
setBranchAddress("dibjetPhi",&dibjetPhi);
setBranchAddress("dibjetM",&dibjetM);
setBranchAddress("dibjetE",&dibjetE);
setBranchAddress("met_met",&met_met);
setBranchAddress("met_px",&met_px);
setBranchAddress("met_py",&met_py);
setBranchAddress("met_phi",&met_phi);
setBranchAddress("TST_Et",&TST_Et);
setBranchAddress("TST_Phi",&TST_Phi);
setBranchAddress("met_track_Et",&met_track_Et);
setBranchAddress("met_track_Phi",&met_track_Phi);
setBranchAddress("met_muons_invis_Et",&met_muons_invis_Et);
setBranchAddress("met_muons_invis_Phi",&met_muons_invis_Phi);
setBranchAddress("mu1QOverP",&mu1QOverP);
setBranchAddress("mu1QOverPErr",&mu1QOverPErr);
setBranchAddress("mu2QOverP",&mu2QOverP);
setBranchAddress("mu2QOverPErr",&mu2QOverPErr);
setBranchAddress("klfit_dijet_pt",&klfit_dijet_pt);
setBranchAddress("klfit_dijet_eta",&klfit_dijet_eta);
setBranchAddress("klfit_dijet_phi",&klfit_dijet_phi);
setBranchAddress("klfit_dijet_e",&klfit_dijet_e);
setBranchAddress("klfit_dijet_m",&klfit_dijet_m);
setBranchAddress("klfit_dilepton_pt",&klfit_dilepton_pt);
setBranchAddress("klfit_dilepton_eta",&klfit_dilepton_eta);
setBranchAddress("klfit_dilepton_phi",&klfit_dilepton_phi);
setBranchAddress("klfit_dilepton_e",&klfit_dilepton_e);
setBranchAddress("klfit_dilepton_m",&klfit_dilepton_m);
setBranchAddress("noKL_dijet_pt",&noKL_dijet_pt);
setBranchAddress("noKL_dijet_eta",&noKL_dijet_eta);
setBranchAddress("noKL_dijet_phi",&noKL_dijet_phi);
setBranchAddress("noKL_dijet_e",&noKL_dijet_e);
setBranchAddress("noKL_dijet_m",&noKL_dijet_m);
setBranchAddress("noKL_dilepton_pt",&noKL_dilepton_pt);
setBranchAddress("noKL_dilepton_eta",&noKL_dilepton_eta);
setBranchAddress("noKL_dilepton_phi",&noKL_dilepton_phi);
setBranchAddress("noKL_dilepton_e",&noKL_dilepton_e);
setBranchAddress("noKL_dilepton_m",&noKL_dilepton_m);
setBranchAddress("klfit_loglikelihood",&klfit_loglikelihood);
setBranchAddress("klfit_minuitdidnotconverge",&klfit_minuitdidnotconverge);
setBranchAddress("klfit_leptonid",&klfit_leptonid);
setBranchAddress("klfit_nbtags",&klfit_nbtags);
setBranchAddress("klfit_istrkjet",&klfit_istrkjet);
setBranchAddress("klfit_jet1_pt",&klfit_jet1_pt);
setBranchAddress("klfit_jet1_eta",&klfit_jet1_eta);
setBranchAddress("klfit_jet1_phi",&klfit_jet1_phi);
setBranchAddress("klfit_jet1_e",&klfit_jet1_e);
setBranchAddress("klfit_jet1_m",&klfit_jet1_m);
setBranchAddress("klfit_jet1_isbjet",&klfit_jet1_isbjet);
setBranchAddress("klfit_jet1_istrkjet",&klfit_jet1_istrkjet);
setBranchAddress("noKL_jet1_pt",&noKL_jet1_pt);
setBranchAddress("noKL_jet1_eta",&noKL_jet1_eta);
setBranchAddress("noKL_jet1_phi",&noKL_jet1_phi);
setBranchAddress("noKL_jet1_e",&noKL_jet1_e);
setBranchAddress("noKL_jet1_m",&noKL_jet1_m);
setBranchAddress("noKL_jet1_isbjet",&noKL_jet1_isbjet);
setBranchAddress("noKL_jet1_istrkjet",&noKL_jet1_istrkjet);
setBranchAddress("klfit_jet2_pt",&klfit_jet2_pt);
setBranchAddress("klfit_jet2_eta",&klfit_jet2_eta);
setBranchAddress("klfit_jet2_phi",&klfit_jet2_phi);
setBranchAddress("klfit_jet2_e",&klfit_jet2_e);
setBranchAddress("klfit_jet2_m",&klfit_jet2_m);
setBranchAddress("klfit_jet2_isbjet",&klfit_jet2_isbjet);
setBranchAddress("klfit_jet2_istrkjet",&klfit_jet2_istrkjet);
setBranchAddress("noKL_jet2_pt",&noKL_jet2_pt);
setBranchAddress("noKL_jet2_eta",&noKL_jet2_eta);
setBranchAddress("noKL_jet2_phi",&noKL_jet2_phi);
setBranchAddress("noKL_jet2_e",&noKL_jet2_e);
setBranchAddress("noKL_jet2_m",&noKL_jet2_m);
setBranchAddress("noKL_jet2_isbjet",&noKL_jet2_isbjet);
setBranchAddress("noKL_jet2_istrkjet",&noKL_jet2_istrkjet);
setBranchAddress("klfit_jjll_pt",&klfit_jjll_pt);
setBranchAddress("klfit_jjll_eta",&klfit_jjll_eta);
setBranchAddress("klfit_jjll_phi",&klfit_jjll_phi);
setBranchAddress("klfit_jjll_e",&klfit_jjll_e);
setBranchAddress("klfit_jjll_m",&klfit_jjll_m);
setBranchAddress("noKL_jjll_pt",&noKL_jjll_pt);
setBranchAddress("noKL_jjll_eta",&noKL_jjll_eta);
setBranchAddress("noKL_jjll_phi",&noKL_jjll_phi);
setBranchAddress("noKL_jjll_e",&noKL_jjll_e);
setBranchAddress("noKL_jjll_m",&noKL_jjll_m);
setBranchAddress("pileupWeight",&pileupWeight);
setBranchAddress("leptonWeight",&leptonWeight);
setBranchAddress("eventWeight",&eventWeight);
setBranchAddress("jvtWeight",&jvtWeight);
setBranchAddress("bTagWeight",&bTagWeight);
setBranchAddress("genWeight",&genWeight);
setBranchAddress("genWeightUp",&genWeightUp);
setBranchAddress("genWeightDown",&genWeightDown);
setBranchAddress("SherpaVjetsNjetsWeight",&SherpaVjetsNjetsWeight);
setBranchAddress("ttbarNNLOWeight",&ttbarNNLOWeight);
setBranchAddress("ttbarNNLOWeightUp",&ttbarNNLOWeightUp);
setBranchAddress("ttbarNNLOWeightDown",&ttbarNNLOWeightDown);
setBranchAddress("truthTopPt",&truthTopPt);
setBranchAddress("truthAntiTopPt",&truthAntiTopPt);
setBranchAddress("truthTtbarPt",&truthTtbarPt);
setBranchAddress("truthTtbarM",&truthTtbarM);
setBranchAddress("x1",&x1);
setBranchAddress("x2",&x2);
setBranchAddress("pdf1",&pdf1);
setBranchAddress("pdf2",&pdf2);
setBranchAddress("scalePDF",&scalePDF);
setBranchAddress("id1",&id1);
setBranchAddress("id2",&id2);
setBranchAddress("lep1Type",&lep1Type);
setBranchAddress("lep1Origin",&lep1Origin);
setBranchAddress("lep2Type",&lep2Type);
setBranchAddress("lep2Origin",&lep2Origin);
setBranchAddress("ptcone20_1",&ptcone20_1);
setBranchAddress("IsoTight_1",&IsoTight_1);
setBranchAddress("ptcone20_2",&ptcone20_2);
setBranchAddress("IsoTight_2",&IsoTight_2);
setBranchAddress("GenHt",&GenHt);
setBranchAddress("Gen_ET",&Gen_ET);
setBranchAddress("PRWHash",&PRWHash);
setBranchAddress("EventNumber",&EventNumber);
setBranchAddress("xsec",&xsec);
setBranchAddress("TrueHt",&TrueHt);
setBranchAddress("DatasetNumber",&DatasetNumber);
setBranchAddress("RunNumber",&RunNumber);
setBranchAddress("RandomRunNumber",&RandomRunNumber);
setBranchAddress("FS",&FS);

if(!isData || !isNominal){
setBranchAddress("leptonWeight_MUON_EFF_BADMUON_STAT__1down",&leptonWeight_MUON_EFF_BADMUON_STAT__1down);
setBranchAddress("leptonWeight_MUON_EFF_BADMUON_STAT__1up",&leptonWeight_MUON_EFF_BADMUON_STAT__1up);
setBranchAddress("leptonWeight_MUON_EFF_BADMUON_SYS__1down",&leptonWeight_MUON_EFF_BADMUON_SYS__1down);
setBranchAddress("leptonWeight_MUON_EFF_BADMUON_SYS__1up",&leptonWeight_MUON_EFF_BADMUON_SYS__1up);
setBranchAddress("leptonWeight_MUON_EFF_ISO_STAT__1down",&leptonWeight_MUON_EFF_ISO_STAT__1down);
setBranchAddress("leptonWeight_MUON_EFF_ISO_STAT__1up",&leptonWeight_MUON_EFF_ISO_STAT__1up);
setBranchAddress("leptonWeight_MUON_EFF_ISO_SYS__1down",&leptonWeight_MUON_EFF_ISO_SYS__1down);
setBranchAddress("leptonWeight_MUON_EFF_ISO_SYS__1up",&leptonWeight_MUON_EFF_ISO_SYS__1up);
setBranchAddress("leptonWeight_MUON_EFF_RECO_STAT__1down",&leptonWeight_MUON_EFF_RECO_STAT__1down);
setBranchAddress("leptonWeight_MUON_EFF_RECO_STAT__1up",&leptonWeight_MUON_EFF_RECO_STAT__1up);
setBranchAddress("leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1down",&leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1down);
setBranchAddress("leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1up",&leptonWeight_MUON_EFF_RECO_STAT_LOWPT__1up);
setBranchAddress("leptonWeight_MUON_EFF_RECO_SYS__1down",&leptonWeight_MUON_EFF_RECO_SYS__1down);
setBranchAddress("leptonWeight_MUON_EFF_RECO_SYS__1up",&leptonWeight_MUON_EFF_RECO_SYS__1up);
setBranchAddress("leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1down",&leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1down);
setBranchAddress("leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1up",&leptonWeight_MUON_EFF_RECO_SYS_LOWPT__1up);
setBranchAddress("leptonWeight_MUON_EFF_TTVA_STAT__1down",&leptonWeight_MUON_EFF_TTVA_STAT__1down);
setBranchAddress("leptonWeight_MUON_EFF_TTVA_STAT__1up",&leptonWeight_MUON_EFF_TTVA_STAT__1up);
setBranchAddress("leptonWeight_MUON_EFF_TTVA_SYS__1down",&leptonWeight_MUON_EFF_TTVA_SYS__1down);
setBranchAddress("leptonWeight_MUON_EFF_TTVA_SYS__1up",&leptonWeight_MUON_EFF_TTVA_SYS__1up);
setBranchAddress("bTagWeight_FT_EFF_B_systematics__1down",&bTagWeight_FT_EFF_B_systematics__1down);
setBranchAddress("bTagWeight_FT_EFF_B_systematics__1up",&bTagWeight_FT_EFF_B_systematics__1up);
setBranchAddress("bTagWeight_FT_EFF_C_systematics__1down",&bTagWeight_FT_EFF_C_systematics__1down);
setBranchAddress("bTagWeight_FT_EFF_C_systematics__1up",&bTagWeight_FT_EFF_C_systematics__1up);
setBranchAddress("bTagWeight_FT_EFF_Light_systematics__1down",&bTagWeight_FT_EFF_Light_systematics__1down);
setBranchAddress("bTagWeight_FT_EFF_Light_systematics__1up",&bTagWeight_FT_EFF_Light_systematics__1up);
setBranchAddress("bTagWeight_FT_EFF_extrapolation__1down",&bTagWeight_FT_EFF_extrapolation__1down);
setBranchAddress("bTagWeight_FT_EFF_extrapolation__1up",&bTagWeight_FT_EFF_extrapolation__1up);
setBranchAddress("bTagWeight_FT_EFF_extrapolation_from_charm__1down",&bTagWeight_FT_EFF_extrapolation_from_charm__1down);
setBranchAddress("bTagWeight_FT_EFF_extrapolation_from_charm__1up",&bTagWeight_FT_EFF_extrapolation_from_charm__1up);
setBranchAddress("jvtWeight_JET_JvtEfficiency__1down",&jvtWeight_JET_JvtEfficiency__1down);
setBranchAddress("jvtWeight_JET_JvtEfficiency__1up",&jvtWeight_JET_JvtEfficiency__1up);
setBranchAddress("pileupWeightUp",&pileupWeightUp);
setBranchAddress("pileupWeightDown",&pileupWeightDown);
}
}

void SkimSlim::setBranchAddress(const std::string& bname, void* add){

  if(m_branchList->Contains(bname.c_str())){
    fChain->SetBranchStatus(bname.c_str(), 1);
    fChain->SetBranchAddress(bname.c_str(), add);
  }
  else{
    std::cout << "INFO: branch " << bname << " does not exist, ignoring " << std::endl;
  }
}

void SkimSlim::correctXsec(){
  if(DatasetNumber>346064 && DatasetNumber < 346076){
    xsec = 48.58 * 1.6e-3;
  }
  if(DatasetNumber>346075 && DatasetNumber < 346087){
    xsec = 3.782 * 1.6e-3;
  }
  //if(DatasetNumber > 451025 && DatasetNumber < 451047){ //old
  //    xsec = 831.76*1 * 3.0e-4; // Xsec = sigma (ttbar) x (t ->bH+) x Br (H+->Wa) x BR (a->mumu)  = 832 x 1 x 1 x 3 e-4 . We want to put limits on Br (t->bH), so keep it 1 and let it float. the post-fit value of mu_sig becomes limit on Br (t->bH) 
    //cout<< xsec <<endl; 
  if(DatasetNumber ==  451027){
      xsec = 831.76*2*3.0e-4*0.64; // sigma (ttbar) x 2 Br(t ->bH+) x Br (H+->Wa) x BR (a->mumu) x Eff(Filter) 
      //cout<< xsec <<endl;
  }
 
  if(DatasetNumber == 451031){
      xsec = 831.76*2*3.0e-4*0.64; // sigma (ttbar) x 2 Br(t ->bH+) x Br (H+->Wa) x BR (a->mumu) x Eff(Filter)
      //cout<< xsec <<endl;
  } 
  if(DatasetNumber == 451032){
      xsec = 831.76*2*3.0e-4*0.67; // sigma (ttbar) x 2 Br(t ->bH+) x Br (H+->Wa) x BR (a->mumu) x Eff(Filter)
      //cout<< xsec <<endl;
  }
  if(DatasetNumber == 451036){
      xsec = 831.76*2*3.0e-4*0.62; // sigma (ttbar) x 2 Br(t ->bH+) x Br (H+->Wa) x BR (a->mumu) x Eff(Filter)
      //cout<< xsec <<endl;
  } 
   if(DatasetNumber == 451037){
      xsec = 831.76*2*3.0e-4*0.66; // sigma (ttbar) x 2 Br(t ->bH+) x Br (H+->Wa) x BR (a->mumu) x Eff(Filter)
      //cout<< xsec <<endl;
  }
  if(DatasetNumber == 451038){
      xsec = 831.76*2*3.0e-4*0.66; // sigma (ttbar) x 2 Br(t ->bH+) x Br (H+->Wa) x BR (a->mumu) x Eff(Filter)
      //cout<< xsec <<endl;
  }
  if(DatasetNumber == 451042){
      xsec = 831.76*2*3.0e-4*0.61; // sigma (ttbar) x 2 Br(t ->bH+) x Br (H+->Wa) x BR (a->mumu) x Eff(Filter)
     //cout<< xsec <<endl;
  }
  if(DatasetNumber == 451043){
      xsec = 831.76*2*3.0e-4*0.62; // sigma (ttbar) x 2 Br(t ->bH+) x Br (H+->Wa) x BR (a->mumu) x Eff(Filter)
      //cout<< xsec <<endl;
  }
  if(DatasetNumber == 451044){
      xsec = 831.76*2*3.0e-4*0.63; // sigma (ttbar) x 2 Br(t ->bH+) x Br (H+->Wa) x BR (a->mumu) x Eff(Filter)
      //cout<< xsec <<endl;
  }

  if(DatasetNumber == 451045){
      xsec = 831.76*2*3.0e-4*0.63; // sigma (ttbar) x 2 Br(t ->bH+) x Br (H+->Wa) x BR (a->mumu) x Eff(Filter)
     //cout<< xsec <<endl;
  }

  if(DatasetNumber == 451046){
      xsec = 831.76*2*3.0e-4*0.64; // sigma (ttbar) x 2 Br(t ->bH+) x Br (H+->Wa) x BR (a->mumu) x Eff(Filter)
     // cout<< xsec <<endl;
  }


}


bool SkimSlim::removeOverlap(){
  if(DatasetNumber == 410470 && isTrueMu1 && isTrueMu2){
    cutManager->count("removeOverlapTTbar");
    return true;
  } 
  return false; 
}

float SkimSlim::computeEffAND(float eff1, float eff2){
  return eff1 * eff2;
}

float SkimSlim::computeEffOR(float eff1, float eff2){
  return 1 - (1-eff1)*(1-eff2);
}

float SkimSlim::computeSFSL(vector<float>* effdata, vector<float>* effMC){
  //cout << "effdata: " << effdata << endl;
  if(computeEffOR(effMC->at(0), effMC->at(1)) == 0) return 1;
  return computeEffOR(effdata->at(0), effdata->at(1))/computeEffOR(effMC->at(0), effMC->at(1));
  
}

float SkimSlim::computeSFDL(vector<float>* effdata, vector<float>* effMC){
  if(computeEffAND(effMC->at(0), effMC->at(1)) == 0) return 1;
  return computeEffAND(effdata->at(0), effdata->at(1))/computeEffAND(effMC->at(0), effMC->at(1));
}

float SkimSlim::computeSFSLORDL(vector<float>* effdataSL, vector<float>* effdataDL, vector<float>* effMCSL, vector<float>* effMCDL){
  float effdatasl = computeEffOR(effdataSL->at(0), effdataSL->at(1));
  float effdatadl = computeEffAND(effdataDL->at(0), effdataDL->at(1));
  float effMCsl = computeEffOR(effMCSL->at(0), effMCSL->at(1));
  float effMCdl = computeEffAND(effMCDL->at(0), effMCDL->at(1));
  if(computeEffOR(effMCsl,effMCdl) == 0) return 1;
  return computeEffOR(effdatasl,effdatadl)/computeEffOR(effMCsl,effMCdl);
}



void SkimSlim::computeSF(){
if(isData){
  SF_SL=1;
  SF_DL=1;
  SF_SLORDL=1;
return;
} 

if(!mutrigDataEff_singleMuonTrig){
  SF_SL=-999;
  SF_DL=-999;
  SF_SLORDL=-999;
  return;
}

//if(!mutrigDataEff_symdiMuonTrig){ waleed edit
// SF_SL=-999;
//  SF_DL=-999;
//  SF_SLORDL=-999;
//  return;
//}


if(nMuons_signal<2){ //addd just for the run with no cuts required for signal..to check the selection efficiency
  SF_SL=-999;
  SF_DL=-999;
  SF_SLORDL=-999;
  return;
}

//compute these, add to output tree, normalize to nominal
SF_SL = computeSFSL(mutrigDataEff_singleMuonTrig, mutrigMCEff_singleMuonTrig);
SF_DL = computeSFDL(mutrigDataEff_symdiMuonTrig, mutrigMCEff_symdiMuonTrig); //original 
//SF_DL = computeSFDL(mutrigDataEff_symdiLepTrig, mutrigMCEff_symdiLepTrig); //diLep triggers
SF_SLORDL = computeSFSLORDL(mutrigDataEff_singleMuonTrig, mutrigDataEff_symdiMuonTrig, mutrigMCEff_singleMuonTrig, mutrigMCEff_symdiMuonTrig); //original
//SF_SLORDL = SF_DL;

if(!isNominal || isData || mutrigDataEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1down == 0) return;
SF_SL_MUON_EFF_TrigStatUncertainty__1down = (SF_SL) ? computeSFSL(mutrigDataEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1down, mutrigMCEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1down)/SF_SL : 0;
SF_SL_MUON_EFF_TrigStatUncertainty__1up = (SF_SL) ? computeSFSL(mutrigDataEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1up, mutrigMCEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1up)/SF_SL : 0;
SF_DL_MUON_EFF_TrigStatUncertainty__1down = (SF_DL) ? computeSFDL(mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1down, mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1down)/SF_DL : 0;
SF_DL_MUON_EFF_TrigStatUncertainty__1up = (SF_DL) ? computeSFDL(mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1up, mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1up)/SF_DL : 0;
SF_SLORDLMUON_EFF_TrigStatUncertainty__1down = (SF_SLORDL) ? computeSFSLORDL(mutrigDataEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1down, mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1down, mutrigMCEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1down, mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1down)/SF_SLORDL : 0;
SF_SLORDLMUON_EFF_TrigStatUncertainty__1up = (SF_SLORDL) ? computeSFSLORDL(mutrigDataEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1up, mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1up, mutrigMCEff_singleMuonTrig_MUON_EFF_TrigStatUncertainty__1up, mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigStatUncertainty__1up)/SF_SLORDL : 0;

SF_SL_MUON_EFF_TrigSystUncertainty__1down = (SF_SL) ? computeSFSL(mutrigDataEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1down, mutrigMCEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1down)/SF_SL : 0;
SF_SL_MUON_EFF_TrigSystUncertainty__1up = (SF_SL) ? computeSFSL(mutrigDataEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1up, mutrigMCEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1up)/SF_SL : 0;
SF_DL_MUON_EFF_TrigSystUncertainty__1down = (SF_DL) ? computeSFDL(mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1down, mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1down)/SF_DL : 0;
SF_DL_MUON_EFF_TrigSystUncertainty__1up = (SF_DL) ? computeSFDL(mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1up, mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1up)/SF_DL : 0;
SF_SLORDLMUON_EFF_TrigSystUncertainty__1down = (SF_SLORDL) ? computeSFSLORDL(mutrigDataEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1down, mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1down, mutrigMCEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1down, mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1down)/SF_SLORDL : 0;
SF_SLORDLMUON_EFF_TrigSystUncertainty__1up = (SF_SLORDL) ? computeSFSLORDL(mutrigDataEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1up, mutrigDataEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1up, mutrigMCEff_singleMuonTrig_MUON_EFF_TrigSystUncertainty__1up, mutrigMCEff_symdiMuonTrig_MUON_EFF_TrigSystUncertainty__1up)/SF_SLORDL : 0;


}


void SkimSlim::divideWeights(float &enumerator, float denominator){
if(denominator==0) enumerator=1;
else enumerator/=denominator;
}



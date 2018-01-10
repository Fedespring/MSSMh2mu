// ROOT
#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TCanvas.h"
#include "TFile.h"

    
  //Single Muon quantities
  TH1D *histAllMuons_pt;  
  TH1D *histAllMuons_eta;  
  TH1D *histAllMuons_phi;  
  TH1D *histAllMuons_pv;
  TH1D *histAllMuons_timingVeto;

  TH1D *histTriggerMuons_pt;  
  TH1D *histTriggerMuons_eta;  
  TH1D *histTriggerMuons_phi;  
  TH1D *histTriggerMuons_pv;

  TH1D *histNormChi2;
  TH1D *histMatchSt;
  TH1D *histMuValidHits;
  TH1D *histDxy;
  TH1D *histDz;
  TH1D *histPixelHits;
  TH1D *histTrackerL;
  TH1D *histDeltaPtOverPt;

  ///// n-1
  TH1D *histPt_num;
  TH1D *histEta_num;
  TH1D *histPhi_num;
  TH1D *histPV_num;
  TH1D *histNormChi2_num;
  TH1D *histMatchSt_num;
  TH1D *histMuValidHits_num;
  TH1D *histDxy_num;
  TH1D *histDz_num;
  TH1D *histPixelHits_num;
  TH1D *histTrackerL_num;
  TH1D *histDeltaPtOverPt_num;

  TH1D *histPt_tkl;
  TH1D *histEta_tkl;
  TH1D *histPhi_tkl;
  TH1D *histPV_tkl;
  TH1D *histNormChi2_tkl;
  TH1D *histMatchSt_tkl;
  TH1D *histMuValidHits_tkl;
  TH1D *histDxy_tkl;
  TH1D *histDz_tkl;
  TH1D *histPixelHits_tkl;
  TH1D *histTrackerL_tkl;
  TH1D *histDeltaPtOverPt_tkl;

  TH1D *histPt_pixh;
  TH1D *histEta_pixh;
  TH1D *histPhi_pixh;
  TH1D *histPV_pixh;
  TH1D *histNormChi2_pixh;
  TH1D *histMatchSt_pixh;
  TH1D *histMuValidHits_pixh;
  TH1D *histDxy_pixh;
  TH1D *histDz_pixh;
  TH1D *histPixelHits_pixh;
  TH1D *histTrackerL_pixh;
  TH1D *histDeltaPtOverPt_pixh;

  TH1D *histPt_muh;
  TH1D *histEta_muh;
  TH1D *histPhi_muh;
  TH1D *histPV_muh;
  TH1D *histNormChi2_muh;
  TH1D *histMatchSt_muh;
  TH1D *histMuValidHits_muh;
  TH1D *histDxy_muh;
  TH1D *histDz_muh;
  TH1D *histPixelHits_muh;
  TH1D *histTrackerL_muh;
  TH1D *histDeltaPtOverPt_muh;

  TH1D *histPt_db;
  TH1D *histEta_db;
  TH1D *histPhi_db;
  TH1D *histPV_db;
  TH1D *histNormChi2_db;
  TH1D *histMatchSt_db;
  TH1D *histMuValidHits_db;
  TH1D *histDxy_db;
  TH1D *histDz_db;
  TH1D *histPixelHits_db;
  TH1D *histTrackerL_db;
  TH1D *histDeltaPtOverPt_db;

  TH1D *histPt_dz;
  TH1D *histEta_dz;
  TH1D *histPhi_dz;
  TH1D *histPV_dz;
  TH1D *histNormChi2_dz;
  TH1D *histMatchSt_dz;
  TH1D *histMuValidHits_dz;
  TH1D *histDxy_dz;
  TH1D *histDz_dz;
  TH1D *histPixelHits_dz;
  TH1D *histTrackerL_dz;
  TH1D *histDeltaPtOverPt_dz;

  TH1D *histPt_chi2;
  TH1D *histEta_chi2;
  TH1D *histPhi_chi2;
  TH1D *histPV_chi2;
  TH1D *histNormChi2_chi2;
  TH1D *histMatchSt_chi2;
  TH1D *histMuValidHits_chi2;
  TH1D *histDxy_chi2;
  TH1D *histDz_chi2;
  TH1D *histPixelHits_chi2;
  TH1D *histTrackerL_chi2;
  TH1D *histDeltaPtOverPt_chi2;

  TH1D *histPt_mst;
  TH1D *histEta_mst;
  TH1D *histPhi_mst;
  TH1D *histPV_mst;
  TH1D *histNormChi2_mst;
  TH1D *histMatchSt_mst;
  TH1D *histMuValidHits_mst;
  TH1D *histDxy_mst;
  TH1D *histDz_mst;
  TH1D *histPixelHits_mst;
  TH1D *histTrackerL_mst;
  TH1D *histDeltaPtOverPt_mst;

  TH1D *histPt_iso;
  TH1D *histEta_iso;
  TH1D *histPhi_iso;
  TH1D *histPV_iso;
  TH1D *histNormChi2_iso;
  TH1D *histMatchSt_iso;
  TH1D *histMuValidHits_iso;
  TH1D *histDxy_iso;
  TH1D *histDz_iso;
  TH1D *histPixelHits_iso;
  TH1D *histTrackerL_iso;
  TH1D *histDeltaPtOverPt_iso;

  /////////////////
 
  TH1D *histTightMuons_pt;  
  TH1D *histTightMuons_eta;  
  TH1D *histTightMuons_phi;  
  TH1D *histTightMuons_pv;
  TH1D *histTightMuons_timingVeto;
  TH1D *histTightMuons_pfiso;
  TH1D *histTightMuons_tbiso;

  TH1D *histIsoTightMuons_pt;  
  TH1D *histIsoTightMuons_eta;  
  TH1D *histIsoTightMuons_phi;  
  TH1D *histIsoTightMuons_pv;

  
  TH1D *histMediumMuons_pt;  
  TH1D *histMediumMuons_eta;  
  TH1D *histMediumMuons_phi;  
  TH1D *histMediumMuons_pv;
  TH1D *histMediumMuons_timingVeto;
  TH1D *histMediumMuons_pfiso;
  TH1D *histMediumMuons_tbiso;

  TH1D *histIsoMediumMuons_pt;  
  TH1D *histIsoMediumMuons_eta;  
  TH1D *histIsoMediumMuons_phi;  
  TH1D *histIsoMediumMuons_pv;

  TH1D *histMediumMuons_mass;

  TH1D *histMetMuons_pt;  
  TH1D *histMetMuons_eta;  
  TH1D *histMetMuons_phi;  
  TH1D *histMetMuons_pv;
  TH1D *histMetMuons_mass;

  //MET
  TH1D *histPFMet_pt;
  TH1D *histPFMet_phi;
  TH1D *histPFMet_sumet;
  TH1D *histPFMet_muonEtFraction;
  TH1D *histPFMet_cEMEtFraction;      
  TH1D *histPFMet_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *histPFMet_cHADEtFraction;
  TH1D *histPFMet_nHADEtFraction;
  
  TH1D *histCaloMet_pt;
  TH1D *histCaloMet_phi;
  
  TH1D *histRawMet_phi;
  TH1D *histRawMet_pt;
  TH1D *histRawMet_sumet;
  
  TH1D *histPFMet_70toInf_pt;
  TH1D *histPFMet_70toInf_phi;
  TH1D *histPFMet_70toInf_sumet;
  TH1D *histPFMet_70toInf_muonEtFraction;
  TH1D *histPFMet_70toInf_cEMEtFraction;      
  TH1D *histPFMet_70toInf_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *histPFMet_70toInf_cHADEtFraction;
  TH1D *histPFMet_70toInf_nHADEtFraction;

  TH1D *histPFMet_70to110_pt;
  TH1D *histPFMet_70to110_phi;
  TH1D *histPFMet_70to110_sumet;
  TH1D *histPFMet_70to110_muonEtFraction;
  TH1D *histPFMet_70to110_cEMEtFraction;      
  TH1D *histPFMet_70to110_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *histPFMet_70to110_cHADEtFraction;
  TH1D *histPFMet_70to110_nHADEtFraction;

  TH1D *histPFMet_110to150_pt;
  TH1D *histPFMet_110to150_phi;
  TH1D *histPFMet_110to150_sumet;
  TH1D *histPFMet_110to150_muonEtFraction;
  TH1D *histPFMet_110to150_cEMEtFraction;      
  TH1D *histPFMet_110to150_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *histPFMet_110to150_cHADEtFraction;
  TH1D *histPFMet_110to150_nHADEtFraction;

  TH1D *histPFMet_150toInf_pt;
  TH1D *histPFMet_150toInf_phi;
  TH1D *histPFMet_150toInf_sumet;
  TH1D *histPFMet_150toInf_muonEtFraction;
  TH1D *histPFMet_150toInf_cEMEtFraction;      
  TH1D *histPFMet_150toInf_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *histPFMet_150toInf_cHADEtFraction;
  TH1D *histPFMet_150toInf_nHADEtFraction;


  TH2F *histPFJets_bDisc_pt;
  //TH2F *histPFJets_bDisc_eta;
  TH1D *histPFJets_bDisc;
  TH1D *histPFJets_pt;       
  TH1D *histPFJets_eta;
  TH1D *histPFJets_phi;
  TH1D *histPFJets_deltaPhi;

  TH1D *histbtagPFJets_pt;

  TH1D *histCuttedJets_bDisc;
  TH1D *histCuttedJets_pt;       
  TH1D *histCuttedJets_eta;
  TH1D *histCuttedJets_phi;
  

  TH1D *histBJets_pt;
  TH1D *histBJets_eta;
  TH1D *histBJets_phi;
  TH1D *histBJets_deltaPhi;
  TH1D *histPFJets_size;
  TH1D *histBJets_size;

  TH1D *histCat1Muons_pt;
  TH1D *histCat1Muons_eta;
  TH1D *histCat1Muons_phi;
  TH1D *histCat1Muons_pv;
  TH1D *histCat2Muons_pt;
  TH1D *histCat2Muons_eta;
  TH1D *histCat2Muons_phi;
  TH1D *histCat2Muons_pv;

  
  TH1D *histTightDimuon_mass;
  TH1D *histTightDimuon_pt;
  TH1D *histTightDimuon_eta;
TH1D *histTightDimuon_phi;

TH1D *histMetDimuon_mass;
TH1D *histMetDimuon_pt;
TH1D *histMetDimuon_eta;
TH1D *histMetDimuon_phi;

  TH1D *histCat1_mass;
  TH1D *histCat2_mass;
  TH1D *histCat1_pt;
  TH1D *histCat2_pt;
  TH1D *histCat1_phi;
  TH1D *histCat2_phi;
  TH1D *histCat1_eta;
  TH1D *histCat2_eta;

  TH1D *histCat11_mass;
  TH1D *histCat22_mass;

  /////PUrw histos
  TH1D *rwhistAllMuons_pt;  
  TH1D *rwhistAllMuons_eta;  
  TH1D *rwhistAllMuons_phi;  
  TH1D *rwhistAllMuons_pv;
  
  TH1D *rwhistNormChi2;
  TH1D *rwhistMatchSt;
  TH1D *rwhistMuValidHits;
  TH1D *rwhistDxy;
  TH1D *rwhistDz;
  TH1D *rwhistPixelHits;
  TH1D *rwhistTrackerL;
  TH1D *rwhistDeltaPtOverPt;

  TH1D *rwhistPt_num;
  TH1D *rwhistEta_num;
  TH1D *rwhistPhi_num;
  TH1D *rwhistPV_num;
  TH1D *rwhistNormChi2_num;
  TH1D *rwhistMatchSt_num;
  TH1D *rwhistMuValidHits_num;
  TH1D *rwhistDxy_num;
  TH1D *rwhistDz_num;
  TH1D *rwhistPixelHits_num;
  TH1D *rwhistTrackerL_num;
  TH1D *rwhistDeltaPtOverPt_num;

  TH1D *rwhistTightMuons_pt;  
  TH1D *rwhistTightMuons_eta;  
  TH1D *rwhistTightMuons_phi;  
  TH1D *rwhistTightMuons_pv;
  TH1D *rwhistTightMuons_pfiso;
  TH1D *rwhistTightMuons_tbiso;

  TH1D *rwhistIsoTightMuons_pt;  
  TH1D *rwhistIsoTightMuons_eta;  
  TH1D *rwhistIsoTightMuons_phi;  
  TH1D *rwhistIsoTightMuons_pv;

  TH1D *rwhistTightDimuon_mass;
  TH1D *rwhistTightDimuon_pt;
  TH1D *rwhistTightDimuon_eta;
  TH1D *rwhistTightDimuon_phi;
  TH1D *rwuphistTightDimuon_mass;
  TH1D *rwuphistTightDimuon_pt;
  TH1D *rwuphistTightDimuon_eta;
  TH1D *rwuphistTightDimuon_phi;
  TH1D *rwdownhistTightDimuon_mass;
  TH1D *rwdownhistTightDimuon_pt;
  TH1D *rwdownhistTightDimuon_eta;
  TH1D *rwdownhistTightDimuon_phi;

TH1D *rwhistMetDimuon_mass;
  TH1D *rwhistMetDimuon_pt;
  TH1D *rwhistMetDimuon_eta;
  TH1D *rwhistMetDimuon_phi;
  TH1D *rwuphistMetDimuon_mass;
  TH1D *rwuphistMetDimuon_pt;
  TH1D *rwuphistMetDimuon_eta;
  TH1D *rwuphistMetDimuon_phi;
  TH1D *rwdownhistMetDimuon_mass;
  TH1D *rwdownhistMetDimuon_pt;
  TH1D *rwdownhistMetDimuon_eta;
  TH1D *rwdownhistMetDimuon_phi;

  TH1D *rwhistMediumMuons_pt;  
  TH1D *rwhistMediumMuons_eta;  
  TH1D *rwhistMediumMuons_phi;  
  TH1D *rwhistMediumMuons_pv;
  //TH1D *rwhistMediumMuons_timingVeto;
  TH1D *rwhistMediumMuons_pfiso;
  TH1D *rwhistMediumMuons_tbiso;

  TH1D *rwhistIsoMediumMuons_pt;  
  TH1D *rwhistIsoMediumMuons_eta;  
  TH1D *rwhistIsoMediumMuons_phi;  
  TH1D *rwhistIsoMediumMuons_pv;

  TH1D *rwhistMediumMuons_mass;
  TH1D *rwuphistMediumMuons_mass;
  TH1D *rwdownhistMediumMuons_mass;

  TH1D *rwhistMetMuons_pt;  
  TH1D *rwhistMetMuons_eta;  
  TH1D *rwhistMetMuons_phi;  
  TH1D *rwhistMetMuons_pv;
  
  TH1D *rwhistMetMuons_mass;
  TH1D *rwuphistMetMuons_mass;
  TH1D *rwdownhistMetMuons_mass;
  
  //MET
  TH1D *rwhistPFMet_pt;
  TH1D *rwhistPFMet_phi;
  TH1D *rwhistPFMet_sumet;
  TH1D *rwhistPFMet_muonEtFraction;
  TH1D *rwhistPFMet_cEMEtFraction;      
  TH1D *rwhistPFMet_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *rwhistPFMet_cHADEtFraction;
  TH1D *rwhistPFMet_nHADEtFraction;
  TH1D *rwhistPFMet_70toInf_pt;
  TH1D *rwhistPFMet_70toInf_phi;
  TH1D *rwhistPFMet_70toInf_sumet;
  TH1D *rwhistPFMet_70toInf_muonEtFraction;
  TH1D *rwhistPFMet_70toInf_cEMEtFraction;      
  TH1D *rwhistPFMet_70toInf_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *rwhistPFMet_70toInf_cHADEtFraction;
  TH1D *rwhistPFMet_70toInf_nHADEtFraction;
  TH1D *rwhistPFMet_70to110_pt;
  TH1D *rwhistPFMet_70to110_phi;
  TH1D *rwhistPFMet_70to110_sumet;
  TH1D *rwhistPFMet_70to110_muonEtFraction;
  TH1D *rwhistPFMet_70to110_cEMEtFraction;      
  TH1D *rwhistPFMet_70to110_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *rwhistPFMet_70to110_cHADEtFraction;
  TH1D *rwhistPFMet_70to110_nHADEtFraction;
  TH1D *rwhistPFMet_110to150_pt;
  TH1D *rwhistPFMet_110to150_phi;
  TH1D *rwhistPFMet_110to150_sumet;
  TH1D *rwhistPFMet_110to150_muonEtFraction;
  TH1D *rwhistPFMet_110to150_cEMEtFraction;      
  TH1D *rwhistPFMet_110to150_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *rwhistPFMet_110to150_cHADEtFraction;
  TH1D *rwhistPFMet_110to150_nHADEtFraction;

  TH1D *rwuphistPFMet_pt;
  TH1D *rwuphistPFMet_phi;
  TH1D *rwuphistPFMet_sumet;
  TH1D *rwuphistPFMet_muonEtFraction;
  TH1D *rwuphistPFMet_cEMEtFraction;      
  TH1D *rwuphistPFMet_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *rwuphistPFMet_cHADEtFraction;
  TH1D *rwuphistPFMet_nHADEtFraction;
  TH1D *rwuphistPFMet_70toInf_pt;
  TH1D *rwuphistPFMet_70toInf_phi;
  TH1D *rwuphistPFMet_70toInf_sumet;
  TH1D *rwuphistPFMet_70toInf_muonEtFraction;
  TH1D *rwuphistPFMet_70toInf_cEMEtFraction;      
  TH1D *rwuphistPFMet_70toInf_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *rwuphistPFMet_70toInf_cHADEtFraction;
  TH1D *rwuphistPFMet_70toInf_nHADEtFraction;
  TH1D *rwuphistPFMet_70to110_pt;
  TH1D *rwuphistPFMet_70to110_phi;
  TH1D *rwuphistPFMet_70to110_sumet;
  TH1D *rwuphistPFMet_70to110_muonEtFraction;
  TH1D *rwuphistPFMet_70to110_cEMEtFraction;      
  TH1D *rwuphistPFMet_70to110_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *rwuphistPFMet_70to110_cHADEtFraction;
  TH1D *rwuphistPFMet_70to110_nHADEtFraction;
  TH1D *rwuphistPFMet_110to150_pt;
  TH1D *rwuphistPFMet_110to150_phi;
  TH1D *rwuphistPFMet_110to150_sumet;
  TH1D *rwuphistPFMet_110to150_muonEtFraction;
  TH1D *rwuphistPFMet_110to150_cEMEtFraction;      
  TH1D *rwuphistPFMet_110to150_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *rwuphistPFMet_110to150_cHADEtFraction;
  TH1D *rwuphistPFMet_110to150_nHADEtFraction;

  TH1D *rwdownhistPFMet_pt;
  TH1D *rwdownhistPFMet_phi;
  TH1D *rwdownhistPFMet_sumet;
  TH1D *rwdownhistPFMet_muonEtFraction;
  TH1D *rwdownhistPFMet_cEMEtFraction;      
  TH1D *rwdownhistPFMet_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *rwdownhistPFMet_cHADEtFraction;
  TH1D *rwdownhistPFMet_nHADEtFraction;  
  TH1D *rwdownhistPFMet_70toInf_pt;
  TH1D *rwdownhistPFMet_70toInf_phi;
  TH1D *rwdownhistPFMet_70toInf_sumet;
  TH1D *rwdownhistPFMet_70toInf_muonEtFraction;
  TH1D *rwdownhistPFMet_70toInf_cEMEtFraction;      
  TH1D *rwdownhistPFMet_70toInf_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *rwdownhistPFMet_70toInf_cHADEtFraction;
  TH1D *rwdownhistPFMet_70toInf_nHADEtFraction;
  TH1D *rwdownhistPFMet_70to110_pt;
  TH1D *rwdownhistPFMet_70to110_phi;
  TH1D *rwdownhistPFMet_70to110_sumet;
  TH1D *rwdownhistPFMet_70to110_muonEtFraction;
  TH1D *rwdownhistPFMet_70to110_cEMEtFraction;      
  TH1D *rwdownhistPFMet_70to110_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *rwdownhistPFMet_70to110_cHADEtFraction;
  TH1D *rwdownhistPFMet_70to110_nHADEtFraction;
  TH1D *rwdownhistPFMet_110to150_pt;
  TH1D *rwdownhistPFMet_110to150_phi;
  TH1D *rwdownhistPFMet_110to150_sumet;
  TH1D *rwdownhistPFMet_110to150_muonEtFraction;
  TH1D *rwdownhistPFMet_110to150_cEMEtFraction;      
  TH1D *rwdownhistPFMet_110to150_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *rwdownhistPFMet_110to150_cHADEtFraction;
  TH1D *rwdownhistPFMet_110to150_nHADEtFraction;


TH1D *jetuphistPFMet_pt;
  TH1D *jetuphistPFMet_phi;
  TH1D *jetuphistPFMet_sumet;
  TH1D *jetuphistPFMet_muonEtFraction;
  TH1D *jetuphistPFMet_cEMEtFraction;      
  TH1D *jetuphistPFMet_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *jetuphistPFMet_cHADEtFraction;
  TH1D *jetuphistPFMet_nHADEtFraction;
  TH1D *jetuphistPFMet_70toInf_pt;
  TH1D *jetuphistPFMet_70toInf_phi;
  TH1D *jetuphistPFMet_70toInf_sumet;
  TH1D *jetuphistPFMet_70toInf_muonEtFraction;
  TH1D *jetuphistPFMet_70toInf_cEMEtFraction;      
  TH1D *jetuphistPFMet_70toInf_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *jetuphistPFMet_70toInf_cHADEtFraction;
  TH1D *jetuphistPFMet_70toInf_nHADEtFraction;
  TH1D *jetuphistPFMet_70to110_pt;
  TH1D *jetuphistPFMet_70to110_phi;
  TH1D *jetuphistPFMet_70to110_sumet;
  TH1D *jetuphistPFMet_70to110_muonEtFraction;
  TH1D *jetuphistPFMet_70to110_cEMEtFraction;      
  TH1D *jetuphistPFMet_70to110_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *jetuphistPFMet_70to110_cHADEtFraction;
  TH1D *jetuphistPFMet_70to110_nHADEtFraction;
  TH1D *jetuphistPFMet_110to150_pt;
  TH1D *jetuphistPFMet_110to150_phi;
  TH1D *jetuphistPFMet_110to150_sumet;
  TH1D *jetuphistPFMet_110to150_muonEtFraction;
  TH1D *jetuphistPFMet_110to150_cEMEtFraction;      
  TH1D *jetuphistPFMet_110to150_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *jetuphistPFMet_110to150_cHADEtFraction;
  TH1D *jetuphistPFMet_110to150_nHADEtFraction;

  TH1D *jetdownhistPFMet_pt;
  TH1D *jetdownhistPFMet_phi;
  TH1D *jetdownhistPFMet_sumet;
  TH1D *jetdownhistPFMet_muonEtFraction;
  TH1D *jetdownhistPFMet_cEMEtFraction;      
  TH1D *jetdownhistPFMet_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *jetdownhistPFMet_cHADEtFraction;
  TH1D *jetdownhistPFMet_nHADEtFraction;  
  TH1D *jetdownhistPFMet_70toInf_pt;
  TH1D *jetdownhistPFMet_70toInf_phi;
  TH1D *jetdownhistPFMet_70toInf_sumet;
  TH1D *jetdownhistPFMet_70toInf_muonEtFraction;
  TH1D *jetdownhistPFMet_70toInf_cEMEtFraction;      
  TH1D *jetdownhistPFMet_70toInf_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *jetdownhistPFMet_70toInf_cHADEtFraction;
  TH1D *jetdownhistPFMet_70toInf_nHADEtFraction;
  TH1D *jetdownhistPFMet_70to110_pt;
  TH1D *jetdownhistPFMet_70to110_phi;
  TH1D *jetdownhistPFMet_70to110_sumet;
  TH1D *jetdownhistPFMet_70to110_muonEtFraction;
  TH1D *jetdownhistPFMet_70to110_cEMEtFraction;      
  TH1D *jetdownhistPFMet_70to110_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *jetdownhistPFMet_70to110_cHADEtFraction;
  TH1D *jetdownhistPFMet_70to110_nHADEtFraction;
  TH1D *jetdownhistPFMet_110to150_pt;
  TH1D *jetdownhistPFMet_110to150_phi;
  TH1D *jetdownhistPFMet_110to150_sumet;
  TH1D *jetdownhistPFMet_110to150_muonEtFraction;
  TH1D *jetdownhistPFMet_110to150_cEMEtFraction;      
  TH1D *jetdownhistPFMet_110to150_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente
  TH1D *jetdownhistPFMet_110to150_cHADEtFraction;
  TH1D *jetdownhistPFMet_110to150_nHADEtFraction;


  TH1D *rwhistPFJets_bDisc;
  TH1D *rwhistPFJets_pt;       
  TH1D *rwhistPFJets_eta;
  TH1D *rwhistPFJets_phi;
  TH1D *rwhistPFJets_deltaPhi;
  TH1D *rwhistbtagPFJets_pt;
  TH1D *rwhistCuttedJets_bDisc;
  TH1D *rwhistCuttedJets_pt;       
  TH1D *rwhistCuttedJets_eta;
  TH1D *rwhistCuttedJets_phi;
  TH1D *rwhistBJets_pt;
  TH1D *rwhistBJets_eta;
  TH1D *rwhistBJets_phi;
  TH1D *rwhistBJets_deltaPhi;
  TH1D *rwhistPFJets_size;
  TH1D *rwhistBJets_size;

  TH1D *rwuphistPFJets_bDisc;
  TH1D *rwuphistPFJets_pt;       
  TH1D *rwuphistPFJets_eta;
  TH1D *rwuphistPFJets_phi;
  TH1D *rwuphistPFJets_deltaPhi;
  TH1D *rwuphistbtagPFJets_pt;
  TH1D *rwuphistCuttedJets_bDisc;
  TH1D *rwuphistCuttedJets_pt;       
  TH1D *rwuphistCuttedJets_eta;
  TH1D *rwuphistCuttedJets_phi;
  TH1D *rwuphistBJets_pt;
  TH1D *rwuphistBJets_eta;
  TH1D *rwuphistBJets_phi;
  TH1D *rwuphistBJets_deltaPhi;
  TH1D *rwuphistPFJets_size;
  TH1D *rwuphistBJets_size;

  TH1D *rwdownhistPFJets_bDisc;
  TH1D *rwdownhistPFJets_pt;       
  TH1D *rwdownhistPFJets_eta;
  TH1D *rwdownhistPFJets_phi;
  TH1D *rwdownhistPFJets_deltaPhi;
  TH1D *rwdownhistbtagPFJets_pt;
  TH1D *rwdownhistCuttedJets_bDisc;
  TH1D *rwdownhistCuttedJets_pt;       
  TH1D *rwdownhistCuttedJets_eta;
  TH1D *rwdownhistCuttedJets_phi;
  TH1D *rwdownhistBJets_pt;
  TH1D *rwdownhistBJets_eta;
  TH1D *rwdownhistBJets_phi;
  TH1D *rwdownhistBJets_deltaPhi;
  TH1D *rwdownhistPFJets_size;
  TH1D *rwdownhistBJets_size;

  TH1D *rwhistCat1Muons_pt;
  TH1D *rwhistCat1Muons_eta;
  TH1D *rwhistCat1Muons_phi;
  TH1D *rwhistCat1Muons_pv;
  TH1D *rwhistCat2Muons_pt;
  TH1D *rwhistCat2Muons_eta;
  TH1D *rwhistCat2Muons_phi;
  TH1D *rwhistCat2Muons_pv;

  TH1D *rwhistCat1_phi;
  TH1D *rwhistCat2_phi;
  TH1D *rwhistCat1_eta;
  TH1D *rwhistCat2_eta;
  TH1D *rwhistCat1_pt;
  TH1D *rwhistCat2_pt;
  TH1D *rwhistCat1_mass;
  TH1D *rwhistCat2_mass;
  TH1D *rwhistCat11_mass;
  TH1D *rwhistCat22_mass;

  TH1D *rwuphistCat1Muons_pt;
  TH1D *rwuphistCat1Muons_eta;
  TH1D *rwuphistCat1Muons_phi;
  TH1D *rwuphistCat1Muons_pv;
  TH1D *rwuphistCat2Muons_pt;
  TH1D *rwuphistCat2Muons_eta;
  TH1D *rwuphistCat2Muons_phi;
  TH1D *rwuphistCat2Muons_pv;

  TH1D *rwuphistCat1_phi;
  TH1D *rwuphistCat2_phi;
  TH1D *rwuphistCat1_eta;
  TH1D *rwuphistCat2_eta;
  TH1D *rwuphistCat1_pt;
  TH1D *rwuphistCat2_pt;
  TH1D *rwuphistCat1_mass;
  TH1D *rwuphistCat2_mass;
  TH1D *rwuphistCat11_mass;
  TH1D *rwuphistCat22_mass;

  TH1D *rwdownhistCat1Muons_pt;
  TH1D *rwdownhistCat1Muons_eta;
  TH1D *rwdownhistCat1Muons_phi;
  TH1D *rwdownhistCat1Muons_pv;
  TH1D *rwdownhistCat2Muons_pt;
  TH1D *rwdownhistCat2Muons_eta;
  TH1D *rwdownhistCat2Muons_phi;
  TH1D *rwdownhistCat2Muons_pv;
 
  TH1D *rwdownhistCat1_phi;
  TH1D *rwdownhistCat2_phi;
  TH1D *rwdownhistCat1_eta;
  TH1D *rwdownhistCat2_eta;
  TH1D *rwdownhistCat1_pt;
  TH1D *rwdownhistCat2_pt;
  TH1D *rwdownhistCat1_mass;
  TH1D *rwdownhistCat2_mass;
  TH1D *rwdownhistCat11_mass;
  TH1D *rwdownhistCat22_mass;

 
  TH1D *histZ_massGen;

  

  

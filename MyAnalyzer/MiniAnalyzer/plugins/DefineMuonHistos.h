// ROOT
#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TCanvas.h"
#include "TFile.h"

    
//Single Muon quantities
TH1F *histAllMuons_pt;  
TH1F *histAllMuons_eta;  
TH1F *histAllMuons_phi;  
TH1F *histAllMuons_pv;
TH1F *histAllMuons_timingVeto;

TH1F *histTriggerMuons_pt;  
TH1F *histTriggerMuons_eta;  
TH1F *histTriggerMuons_phi;  
TH1F *histTriggerMuons_pv;

TH1F *histNormChi2;
TH1F *histMatchSt;
TH1F *histMuValidHits;
TH1F *histDxy;
TH1F *histDz;
TH1F *histPixelHits;
TH1F *histTrackerL;
TH1F *histDeltaPtOverPt;

///// n-1
TH1F *histPt_num;
TH1F *histEta_num;
TH1F *histPhi_num;
TH1F *histPV_num;
TH1F *histNormChi2_num;
TH1F *histMatchSt_num;
TH1F *histMuValidHits_num;
TH1F *histDxy_num;
TH1F *histDz_num;
TH1F *histPixelHits_num;
TH1F *histTrackerL_num;
TH1F *histDeltaPtOverPt_num;

TH1F *histPt_tkl;
TH1F *histEta_tkl;
TH1F *histPhi_tkl;
TH1F *histPV_tkl;
TH1F *histNormChi2_tkl;
TH1F *histMatchSt_tkl;
TH1F *histMuValidHits_tkl;
TH1F *histDxy_tkl;
TH1F *histDz_tkl;
TH1F *histPixelHits_tkl;
TH1F *histTrackerL_tkl;
TH1F *histDeltaPtOverPt_tkl;

TH1F *histPt_pixh;
TH1F *histEta_pixh;
TH1F *histPhi_pixh;
TH1F *histPV_pixh;
TH1F *histNormChi2_pixh;
TH1F *histMatchSt_pixh;
TH1F *histMuValidHits_pixh;
TH1F *histDxy_pixh;
TH1F *histDz_pixh;
TH1F *histPixelHits_pixh;
TH1F *histTrackerL_pixh;
TH1F *histDeltaPtOverPt_pixh;

TH1F *histPt_muh;
TH1F *histEta_muh;
TH1F *histPhi_muh;
TH1F *histPV_muh;
TH1F *histNormChi2_muh;
TH1F *histMatchSt_muh;
TH1F *histMuValidHits_muh;
TH1F *histDxy_muh;
TH1F *histDz_muh;
TH1F *histPixelHits_muh;
TH1F *histTrackerL_muh;
TH1F *histDeltaPtOverPt_muh;

TH1F *histPt_db;
TH1F *histEta_db;
TH1F *histPhi_db;
TH1F *histPV_db;
TH1F *histNormChi2_db;
TH1F *histMatchSt_db;
TH1F *histMuValidHits_db;
TH1F *histDxy_db;
TH1F *histDz_db;
TH1F *histPixelHits_db;
TH1F *histTrackerL_db;
TH1F *histDeltaPtOverPt_db;

TH1F *histPt_dz;
TH1F *histEta_dz;
TH1F *histPhi_dz;
TH1F *histPV_dz;
TH1F *histNormChi2_dz;
TH1F *histMatchSt_dz;
TH1F *histMuValidHits_dz;
TH1F *histDxy_dz;
TH1F *histDz_dz;
TH1F *histPixelHits_dz;
TH1F *histTrackerL_dz;
TH1F *histDeltaPtOverPt_dz;

TH1F *histPt_chi2;
TH1F *histEta_chi2;
TH1F *histPhi_chi2;
TH1F *histPV_chi2;
TH1F *histNormChi2_chi2;
TH1F *histMatchSt_chi2;
TH1F *histMuValidHits_chi2;
TH1F *histDxy_chi2;
TH1F *histDz_chi2;
TH1F *histPixelHits_chi2;
TH1F *histTrackerL_chi2;
TH1F *histDeltaPtOverPt_chi2;

TH1F *histPt_mst;
TH1F *histEta_mst;
TH1F *histPhi_mst;
TH1F *histPV_mst;
TH1F *histNormChi2_mst;
TH1F *histMatchSt_mst;
TH1F *histMuValidHits_mst;
TH1F *histDxy_mst;
TH1F *histDz_mst;
TH1F *histPixelHits_mst;
TH1F *histTrackerL_mst;
TH1F *histDeltaPtOverPt_mst;

TH1F *histPt_iso;
TH1F *histEta_iso;
TH1F *histPhi_iso;
TH1F *histPV_iso;
TH1F *histNormChi2_iso;
TH1F *histMatchSt_iso;
TH1F *histMuValidHits_iso;
TH1F *histDxy_iso;
TH1F *histDz_iso;
TH1F *histPixelHits_iso;
TH1F *histTrackerL_iso;
TH1F *histDeltaPtOverPt_iso;

/////////////////
 
TH1F *histTightMuons_pt;  
TH1F *histTightMuons_eta;  
TH1F *histTightMuons_phi;  
TH1F *histTightMuons_pv;
TH1F *histTightMuons_timingVeto;
TH1F *histTightMuons_pfiso;
TH1F *histTightMuons_tbiso;

TH1F *histIsoTightMuons_pt;  
TH1F *histIsoTightMuons_eta;  
TH1F *histIsoTightMuons_phi;  
TH1F *histIsoTightMuons_pv;

TH1F *histDeltaPhi_tightMuons;
TH1F *histDeltaPhi_mediumMuons;

TH1F *histMediumMuons_pt;  
TH1F *histMediumMuons_eta;  
TH1F *histMediumMuons_phi;  
TH1F *histMediumMuons_pv;
TH1F *histMediumMuons_timingVeto;
TH1F *histMediumMuons_pfiso;
TH1F *histMediumMuons_tbiso;

TH1F *histIsoMediumMuons_pt;  
TH1F *histIsoMediumMuons_eta;  
TH1F *histIsoMediumMuons_phi;  
TH1F *histIsoMediumMuons_pv;

TH1F *histMetMuons_pt;  
TH1F *histMetMuons_eta;  
TH1F *histMetMuons_phi;  
TH1F *histMetMuons_pv;

TH1F *histCat1Muons_pt;
TH1F *histCat1Muons_eta;
TH1F *histCat1Muons_phi;
TH1F *histCat1Muons_pv;

TH1F *histCat2Muons_pt;
TH1F *histCat2Muons_eta;
TH1F *histCat2Muons_phi;
TH1F *histCat2Muons_pv;


/////PUrw histos
TH1F *rwhistAllMuons_pt;  
TH1F *rwhistAllMuons_eta;  
TH1F *rwhistAllMuons_phi;  
TH1F *rwhistAllMuons_pv;
  
TH1F *rwhistNormChi2;
TH1F *rwhistMatchSt;
TH1F *rwhistMuValidHits;
TH1F *rwhistDxy;
TH1F *rwhistDz;
TH1F *rwhistPixelHits;
TH1F *rwhistTrackerL;
TH1F *rwhistDeltaPtOverPt;

TH1F *rwhistPt_num;
TH1F *rwhistEta_num;
TH1F *rwhistPhi_num;
TH1F *rwhistPV_num;
TH1F *rwhistNormChi2_num;
TH1F *rwhistMatchSt_num;
TH1F *rwhistMuValidHits_num;
TH1F *rwhistDxy_num;
TH1F *rwhistDz_num;
TH1F *rwhistPixelHits_num;
TH1F *rwhistTrackerL_num;
TH1F *rwhistDeltaPtOverPt_num;

TH1F *rwhistTightMuons_pt;  
TH1F *rwhistTightMuons_eta;  
TH1F *rwhistTightMuons_phi;  
TH1F *rwhistTightMuons_pv;
TH1F *rwhistTightMuons_pfiso;
TH1F *rwhistTightMuons_tbiso;

TH1F *rwhistIsoTightMuons_pt;  
TH1F *rwhistIsoTightMuons_eta;  
TH1F *rwhistIsoTightMuons_phi;  
TH1F *rwhistIsoTightMuons_pv;

TH1F *rwhistMediumMuons_pt;  
TH1F *rwhistMediumMuons_eta;  
TH1F *rwhistMediumMuons_phi;  
TH1F *rwhistMediumMuons_pv;
//TH1F *rwhistMediumMuons_timingVeto;
TH1F *rwhistMediumMuons_pfiso;
TH1F *rwhistMediumMuons_tbiso;

TH1F *rwhistIsoMediumMuons_pt;  
TH1F *rwhistIsoMediumMuons_eta;  
TH1F *rwhistIsoMediumMuons_phi;  
TH1F *rwhistIsoMediumMuons_pv;


TH1F *rwhistMetMuons_pt;  
TH1F *rwhistMetMuons_eta;  
TH1F *rwhistMetMuons_phi;  
TH1F *rwhistMetMuons_pv;
  
TH1F *rwhistCat1Muons_pt;
TH1F *rwhistCat1Muons_eta;
TH1F *rwhistCat1Muons_phi;
TH1F *rwhistCat1Muons_pv;

TH1F *rwhistCat2Muons_pt;
TH1F *rwhistCat2Muons_eta;
TH1F *rwhistCat2Muons_phi;
TH1F *rwhistCat2Muons_pv;

TH1F *rwuphistCat1Muons_pt;
TH1F *rwuphistCat1Muons_eta;
TH1F *rwuphistCat1Muons_phi;
TH1F *rwuphistCat1Muons_pv;

TH1F *rwuphistCat2Muons_pt;
TH1F *rwuphistCat2Muons_eta;
TH1F *rwuphistCat2Muons_phi;
TH1F *rwuphistCat2Muons_pv;

TH1F *rwdownhistCat1Muons_pt;
TH1F *rwdownhistCat1Muons_eta;
TH1F *rwdownhistCat1Muons_phi;
TH1F *rwdownhistCat1Muons_pv;

TH1F *rwdownhistCat2Muons_pt;
TH1F *rwdownhistCat2Muons_eta;
TH1F *rwdownhistCat2Muons_phi;
TH1F *rwdownhistCat2Muons_pv;

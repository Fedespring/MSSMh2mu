//This is the list and types of the variables saved in the TTree;
//New variables must be declared here

#include "TString.h"

//event variables
int runnumber;
int lumiblock;
ULong64_t eventNumber;
//  - l : a 64 bit unsigned integer (ULong64_t)
float timestamp;
int bunchXing;
int orbitNum;



//beam spot
double  BS_x;
double  BS_y;
double  BS_z;
double  BS_xerr;
double  BS_yerr;
double  BS_zerr;
double  BS_Wx;
double  BS_Wy;
double  BS_Wz;
double  BS_Wxerr;
double  BS_Wyerr;
double  BS_Wzerr;
double  BS_dxdz;
double  BS_dxdzerr;
double  BS_dydz;
double  BS_dydzerr;

//PileUp
double  MyWeight, MyWeightUp, MyWeightDown;
double EventWeight;
double mc_weight;
  
//PDF 
double wpdfNNPDF;
double wpdfMSTW;

// HLT
std::vector<TString> hlt_path;

//primary vertex
std::vector<double> PV_x;
std::vector<double> PV_y; 
std::vector<double> PV_z;
std::vector<double> PV_xerr;
std::vector<double> PV_yerr;
std::vector<double> PV_zerr;
   
std::vector<bool> PV_validity;
std::vector<bool> PV_fake;

std::vector<double> PV_normchi2;
std::vector<double> PV_chi2;
std::vector<double> PV_ndof;
std::vector<int> PV_ntracks;

std::vector<double> PV_SumPtTracks;
std::vector<double> PV_position;


//muon variables
std::vector<bool>  Mu_hasTriggeredIso;
std::vector<bool>  Mu_hasTriggeredIsoEr;
std::vector<bool>  Mu_hasTriggeredIsoTk;
std::vector<bool>  Mu_hasTriggeredIsoTkEr;
std::vector<bool>  Mu_hasTriggeredMu50;
std::vector<bool>  Mu_hasTriggeredTkMu50;
std::vector<float>  Mu_roch_correction;

std::vector<short> Mu_isPFMu;
std::vector<short> Mu_isTightMu;
std::vector<short> Mu_isHighPtMu;
std::vector<short> Mu_isMediumMu;
std::vector<short> Mu_isMuGlobal;
std::vector<short> Mu_isMuTracker;
std::vector<short> Mu_isMuStandAlone; 

std::vector<int>   Mu_numberOfChambers;
std::vector<int>   Mu_numberOfMatches;
std::vector<int>   Mu_numberOfMatchedStation;
std::vector<float> Mu_dB;
std::vector<int>   Mu_stationMask;
std::vector<int>   Mu_numberOfMatchedRPCLayers;
std::vector<int>   Mu_timingVeto;

std::vector<float> Mu_pt_mu;
std::vector<float> Mu_px_mu;
std::vector<float> Mu_py_mu;
std::vector<float> Mu_pz_mu;
std::vector<float> Mu_en_mu;
std::vector<float> Mu_phi_mu;
std::vector<float> Mu_eta_mu;
std::vector<short> Mu_charge_mu;

std::vector<float> Mu_triggerSF_BF;
std::vector<float> Mu_triggerSF_GH;
std::vector<float> Mu_idSF_BF;
std::vector<float> Mu_idSF_GH;
std::vector<float> Mu_isoSF_BF;
std::vector<float> Mu_isoSF_GH;

std::vector<float> Mu_hptriggerSF_BF;
std::vector<float> Mu_hptriggerSF_GH;
std::vector<float> Mu_hpidSF_BF;
std::vector<float> Mu_hpidSF_GH;
std::vector<float> Mu_hpisoSF_BF;
std::vector<float> Mu_hpisoSF_GH;

std::vector<float> Mu_vertex_vx;
std::vector<float> Mu_vertex_vy;
std::vector<float> Mu_vertex_vz;

std::vector<float> Mu_sumPtIsoR03;
std::vector<float> Mu_ntkIsoR03;
std::vector<float> Mu_emIsoR03;
std::vector<float> Mu_hadIsoR03;
std::vector<float> Mu_hoEtIsoR03;
std::vector<float> Mu_nJetsIsoR03;
std::vector<float> Mu_sumPtIsoR05;
std::vector<float> Mu_ntkIsoR05;
std::vector<float> Mu_emIsoR05;
std::vector<float> Mu_hadIsoR05;
std::vector<float> Mu_hoEtIsoR05;
std::vector<float> Mu_nJetsIsoR05;
std::vector<float> Mu_calEnergyEm;
std::vector<float> Mu_calEnergyHad;
std::vector<float> Mu_calEnergyHo;
std::vector<float> Mu_calEnergyEmS9;
std::vector<float> Mu_calEnergyHadS9;
std::vector<float> Mu_calEnergyHoS9;

std::vector<float> Mu_sumCHPtPFIsoR04;
std::vector<float> Mu_sumCPPtPFIsoR04;
std::vector<float> Mu_sumNHPtPFIsoR04;
std::vector<float> Mu_sumPhoEtPFIsoR04;
std::vector<float> Mu_sumPUPtPFIsoR04;

std::vector<float> Mu_sumCHPtPFIsoR03;
std::vector<float> Mu_sumCPPtPFIsoR03;
std::vector<float> Mu_sumNHPtPFIsoR03;
std::vector<float> Mu_sumPhoEtPFIsoR03;
std::vector<float> Mu_sumPUPtPFIsoR03;

std::vector<int>   STAMu_numberOfHits;
std::vector<short> STAMu_recHitsSize;
std::vector<float> STAMu_normchi2Mu;
std::vector<float> STAMu_dxyMu;
std::vector<float> STAMu_dzMu;
std::vector<float> STAMu_vertex_vx;
std::vector<float> STAMu_vertex_vy;
std::vector<float> STAMu_vertex_vz;

//global mu
std::vector<float> GLBMu_pt;
std::vector<float> GLBMu_pt_err;
std::vector<float> GLBMu_eta;
std::vector<float> GLBMu_phi;
std::vector<float> GLBMu_chi2;
std::vector<float> GLBMu_ndf;
std::vector<float> GLBMu_qOverPt;
std::vector<float> GLBMu_normchi2Mu;
std::vector<float> GLBMu_dxyMu;
std::vector<float> GLBMu_dzMu;
std::vector<int> GLBMu_numberOfPixelHits;
std::vector<int> GLBMu_numberOfTrackerHits;
std::vector<int> GLBMu_numberOfMuonsHits;
std::vector<float> GLBMu_vertex_vx;
std::vector<float> GLBMu_vertex_vy;
std::vector<float> GLBMu_vertex_vz;

//tracker mu
std::vector<float> TRKMu_pt;
std::vector<float> TRKMu_pt_err;
std::vector<float> TRKMu_eta;
std::vector<float> TRKMu_phi;
std::vector<float> TRKMu_chi2;
std::vector<float> TRKMu_ndf;
std::vector<float> TRKMu_qOverPt;
std::vector<float> TRKMu_normchi2Mu;
std::vector<float> TRKMu_dxyMu;
std::vector<float> TRKMu_dzMu;
std::vector<float> TRKMu_dzPV;
std::vector<int> TRKMu_numberOfPixelHits;
std::vector<int> TRKMu_numberOfTrackerHits;
std::vector<int> TRKMu_trackerLayersWithMeasurement;

//tunep mu
std::vector<float> TPMu_pt;
std::vector<float> TPMu_pt_err;
std::vector<float> TPMu_eta;
std::vector<float> TPMu_phi;
std::vector<float> TPMu_chi2;
std::vector<float> TPMu_ndf;
std::vector<float> TPMu_qOverPt;


//ELECTRONS
/* std::vector<float> Ele_pt_pfele; */
/* std::vector<float> Ele_px_pfele; */
/* std::vector<float> Ele_py_pfele; */
/* std::vector<float> Ele_pz_pfele; */
/* std::vector<float> Ele_en_pfele; */
/* std::vector<float> Ele_phi_pfele;   */
/* std::vector<float> Ele_eta_pfele; */


//JET
std::vector<float> Jet_PUmva_pfjet;
std::vector<bool>  Jet_PULoose_pfjet;
std::vector<bool>  Jet_PUMedium_pfjet;
std::vector<bool>  Jet_PUTight_pfjet;

std::vector<float> Jet_pt_pfjet;
std::vector<float> Jet_ptL5_pfjet;
std::vector<float> Jet_ptL7_pfjet;
std::vector<float> Jet_px_pfjet;
std::vector<float> Jet_py_pfjet;
std::vector<float> Jet_pz_pfjet;
std::vector<float> Jet_en_pfjet;
std::vector<float> Jet_phi_pfjet;  
std::vector<float> Jet_eta_pfjet;
std::vector<float> Jet_Area_pfjet;
std::vector<float> Jet_JECunc_pfjet;

std::vector<bool> Jet_isLoose_pfjet;    
std::vector<bool> Jet_isTight_pfjet;    
std::vector<bool> Jet_isTightLepVeto_pfjet; 

std::vector<float> Jet_vertex_vx;
std::vector<float> Jet_vertex_vy;
std::vector<float> Jet_vertex_vz;

std::vector<float> Jet_ChargedHadEn_pfjet;
std::vector<float> Jet_NeutralHadEn_pfjet;
std::vector<float> Jet_ChargedEmEn_pfjet;
std::vector<float> Jet_ChargedMuEn_pfjet;
std::vector<float> Jet_NeutralEmEn_pfjet;
std::vector<float> Jet_ChargedMultiplicity_pfjet;
std::vector<float> Jet_NeutralMultiplicity_pfjet;
std::vector<float> Jet_MuonMultiplicity_pfjet;   
std::vector<float> Jet_ElectronMultiplicity_pfjet;   

std::vector<float> Jet_discriminatorHE_pfjet;
std::vector<float> Jet_discriminatorHP_pfjet;
std::vector<float> Jet_discriminatorCSV_pfjet;

//MET
std::vector<float> Met_pt_pfmet;
std::vector<float> Met_phi_pfmet;
std::vector<float> Met_eta_pfmet;
std::vector<float> Met_energy_pfmet;
std::vector<float> Met_sumet_pfmet;
std::vector<float> Met_ptsignificance_pfmet;
std::vector<float> Met_etsignificance_pfmet;

std::vector<float> Met_type01smear_pt_pfmet;
std::vector<float> Met_totUp_pt_pfmet;
std::vector<float> Met_totDown_pt_pfmet;
std::vector<float> Met_jetEnUp_pfmet;
std::vector<float> Met_jetEnDown_pfmet;
std::vector<float> Met_jetResUp_pfmet;
std::vector<float> Met_jetResDown_pfmet;
std::vector<float> Met_unclusterUp_pfmet;
std::vector<float> Met_unclusterDown_pfmet;  
std::vector<float> Met_tauUp_pfmet;
std::vector<float> Met_tauDown_pfmet;  
std::vector<float> Met_eleUp_pfmet;
std::vector<float> Met_eleDown_pfmet;  
std::vector<float> Met_photonUp_pfmet;
std::vector<float> Met_photonDown_pfmet;  
std::vector<float> Met_muUp_pfmet;
std::vector<float> Met_muDown_pfmet;
/* std::vector<float> Met_muonEtFraction_pfmet; */
/* std::vector<float> Met_cEMEtFraction_pfmet;       */
/* std::vector<float> Met_nEMEtFraction_pfmet; // mi sa che si sono sbagliati proprio a chiamarla centralmente */
/* std::vector<float> Met_cHADEtFraction_pfmet; */
/* std::vector<float> Met_nHADEtFraction_pfmet; */

/* std::vector<float> Met_pt_calomet; */
/* std::vector<float> Met_phi_calomet; */
      
std::vector<float> Met_phi_umet;
std::vector<float> Met_pt_umet;
std::vector<float> Met_sumet_umet;


/* std::vector<double> MassCat1; */
/* std::vector<double> MassCat2; */
float MassCat1, MassCat11, MassCat111;
float MassCat2, MassCat22, MassCat222;

//genparticles
std::vector<int> Genp_particleId;  
std::vector<int> Genp_status;
std::vector<double> Genp_pt;
std::vector<double> Genp_p;
std::vector<double> Genp_et;
std::vector<double> Genp_e;
std::vector<double> Genp_mt;
std::vector<double> Genp_m;
std::vector<double> Genp_eta;
std::vector<double> Genp_phi;
std::vector<double> Genp_vx;
std::vector<double> Genp_vy;
std::vector<double> Genp_vz;
  
std::vector<int> Genp_nMmothers;
std::vector<int> Genp_nDaughters;
  
std::vector<int> Genp_particleId_mother;
std::vector<int> Genp_status_mother;
std::vector<double> Genp_pt_mother;
std::vector<double> Genp_eta_mother;
std::vector<double> Genp_phi_mother;
std::vector<double> Genp_m_mother;

//////////////////////////////////////////
TH1D* hN_events_count;
TH1D* hN_events_eff;
TH1D* hN_events_count_W;
TH1D* hN_events_eff_W;

double eventi_iniziali_W;
double eventi_Trigger_W;
double eventi_Quality_W;
double eventi_Iso_W;
double eventi_MuonSel_W;
double eventi_Minv_W;
double eventi_MEt_W;
double eventi_finali_W;
double eventi_finali_jetVeto_W;

double eventi_iniziali;
double eventi_MatchMuonTrigger;
double eventi_Trigger;
double eventi_Quality;
double eventi_Iso;
double eventi_MuonSel;
double eventi_Minv;
double eventi_JetSel;
double eventi_finali;
double eventi_finali_cat2;
double eventi_MEt;
double eventi_finali_jetVeto;

double eventi_finali_cat2_W_NNPDF;
double eventi_finali_cat2_W_MSTW;
double eventi_finali_W_NNPDF;
double eventi_finali_W_MSTW;
double eventi_iniziali_W_NNPDF;
double eventi_iniziali_W_MSTW;

double eventi_JEScutDOWN;
double eventi_JEScutBASE;
double eventi_JEScutUP;

TH1F* hMass_gen;
TH1I* hPrimaryVertex;

TH1F* hmu_2;
TH1F* hmu_1;

TH1F* hMass_2mucut;
TH1F* hMass_final_1jet;
TH1F* hMass_final_jetVeto;
TH1F* hMass_MEtcut;
  
TH1F* hMEt_2mucut;
TH1F* hMEt_MEtcut;
TH1F* hMEt_generator;
TH1F *hMEt_generator_stefano;
TH1F *hMu_gen_pt;
TH1F *   hb_gen_pt;
TH1F *   hH_gen_pt;
TH1F *   hAllParticles;

TH1F* hJet_pt_2mucut;
TH1F* hJet_eta_2mucut;
TH1F* hJet_bDisc_2mucut;
TH1F* hJet_pt_MEtcut;
TH1F* hJet_eta_MEtcut;
TH1F* hJet_bDisc_MEtcut;
TH1F* hJet_pt_bcut;
TH1F* hJet_eta_bcut;
TH1F* hJet_bDisc_bcut;

TH1F* hJEC_Unc;

TH1F* hJet_Leadingpt_bcut;
TH1F* hJet_LeadingptL7_bcut;
TH1F* hJet_Leadingeta_bcut;
TH1F* hJet_LeadingbDisc_bcut;
TH1F* hJet_Secondpt_bcut;
TH1F* hJet_SecondptL7_bcut;
TH1F* hJet_Secondeta_bcut;
TH1F* hJet_SecondbDisc_bcut;

TH1F* hNJet;
TH1I* hNJet_MEtcut;
TH1I* hNbJet;
TH1I* hNbJet_Jetcut;

TH1F* hDzVertex;
TH1F* hRho;
TH1F* hDeltaB;


//////VBF Analysis
TH1F* hEta_mu1_cut;
TH1F* hEta_mu2_cut;
TH1F* hPhi_mu1_cut;
TH1F* hPhi_mu2_cut;
TH1F* hPtDiMuon_cut;
TH1F* hEtaDiMuon_cut;
TH1F* hDeltaR_2mu_cut;
TH1F* hDeltaEta_2mu_cut;

TH1F* hMEt_phi_2mucut;
TH1F* hJet_phi_2mucut;

TH1F* hJetVBF_pt;
TH1F* hJetVBF_eta;
TH1F* hJetVBF_phi;
TH1F* hJet1_VBF_DRq;
TH1F* hJet2_VBF_DRq;

TH1F* hDiJet_VBF_pt;
TH1F* hDiJetVBF_phi;
TH1F* hDiJetVBF_eta;
TH1F* hDiJetVBF_mass;

TH1F* hJetVBF_DEtaJets;
TH1F* hJetVBF_DRJets;
TH1F* hDEta_VBF_JetMu;
TH1F* hJetVBF_mass;

TH1F* hJetDPhiJets;

TH1F* hJet1_VBF_pt;
TH1F* hJet1_VBF_eta;
TH1F* hJet1_VBF_phi;
TH1F* hJet2_VBF_pt;
TH1F* hJet2_VBF_eta;
TH1F* hJet2_VBF_phi;
TH1F* hJet_VBF_etaeta;

TH1F* hJet_etaeta;

TH1F* hJet1_pt;
TH1F* hJet1_eta;
TH1F* hJet2_pt;
TH1F* hJet2_eta;
TH1F* hJetDEtaJets;

TH1F* hJetDRJets;
TH1F* hJet_mass;
TH1F* hJet_energy;

/* TH1F*    hDiJetpt; */
/* TH1F*    hJetDEtaJets; */
/* TH1F*    hJetDRJets; */
/* TH1F*    hJetmass; */
TH1F* hDeltaR_DiMu_DiJets;
TH1F* hDeltaPhi_DiMu_DiJets;

TH1F* hDiJetmass;
TH1F* hDiJetenergy;
TH1F* hDiJet_eta;
TH1F* hDiJet_phi;
TH1F* hDiJet_pt;

TH1F* hMinvHiggs;
TH1F* hDEtaJetMu;

TH1F* hDEta_JetMu;   // solo per il segnale: jet che non sono del vbf

TH1F* hJetVeto_pt;
TH1F* hJetVeto_eta;
TH1F* hJetVeto_phi;

//////////////////////////////////////////
//std::vector<float> tMyWeight;

// muons
std::vector<double> tFirstMuon;
std::vector<double> tSecondMuon;

std::vector<double> tDeltaEta_2mu;
std::vector<double> tDeltaR_2mu;
std::vector<double> tDeltaPhi_2mu;

std::vector<double> tMassDiMuon;
std::vector<double> tPtDiMuon;
std::vector<double> tEtaDiMuon;
std::vector<double> tPhiDiMuon;


//jets 
std::vector<double> tFirstJet_pt;   
std::vector<double> tSecondJet_pt;
std::vector<double> tFirstJet_eta;   
std::vector<double> tSecondJet_eta;

std::vector<double> tEtaPerEtaJet;
    
std::vector<double> tDeltaEta_2j;
std::vector<double> tDeltaR_2j;
std::vector<double> tDeltaPhi_2j;
    
std::vector<double> tDiJetmass;
std::vector<double> tDiJetPt;
std::vector<double> tDiJetEta;
std::vector<double> tDiJetPhi;

std::vector<double> tDEtaJetMu;

std::vector<double> tDeltaR_DiMu_DiJets;
std::vector<double> tDeltaPhi_DiMu_DiJets;
std::vector<double> tDeltaEta_DiMu_DiJets;
    
std::vector<double> tMEt_pt; 
std::vector<double> tMEt_phi;

std::vector<double> tmu_1;
std::vector<double> tmu_2;
std::vector<double> tMass_2mucut;
std::vector<double> tMEt_2mucut;
std::vector<double> tMEt_MEtcut;
std::vector<double> tMass_MEtcut;
std::vector<double> tJet_pt_2mucut;
std::vector<double> tJet_eta_2mucut;
std::vector<double> tJet_bDisc_2mucut;
std::vector<double> tJet_pt_MEtcut;
std::vector<double> tJet_eta_MEtcut;
std::vector<double> tJet_bDisc_MEtcut;
std::vector<double> tJet_pt_bcut;
std::vector<double> tJet_eta_bcut;
std::vector<double> tJet_bDisc_bcut;
std::vector<double> tJet_Leadingpt_bcut;
std::vector<double> tJet_LeadingptL7_bcut;
std::vector<double> tJet_Leadingeta_bcut;
std::vector<double> tJet_LeadingbDisc_bcut;
std::vector<double> tJet_Secondpt_bcut;
std::vector<double> tJet_SecondptL7_bcut;
std::vector<double> tJet_Secondeta_bcut;
std::vector<double> tJet_SecondbDisc_bcut;
std::vector<double> tMass_final_1jet;
std::vector<double> tMass_final_jetVeto;


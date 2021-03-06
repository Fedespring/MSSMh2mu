
// -*- C++ -*-
//
// Package:    MyAnalyzer/MiniAnalyzer
// Class:      MiniAnalyzer
// 
/**\class MiniAnalyzer MiniAnalyzer.cc MyAnalyzer/MiniAnalyzer/plugins/MiniAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Federica Primavera
//         Created:  Mon, 20 Oct 2014 08:14:35 GMT
//
//

#ifndef MiniAnalizer_H
#define MiniAnalyzer_H

// system include files
#include <memory>
#include <cmath>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
  
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"
//#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

//PUJetId
#include "RecoJets/JetProducers/interface/PileupJetIdAlgo.h"
//PUreweight
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

//btag scale factors
#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"
#include "MyAnalyzer/MiniAnalyzer/plugins/BTagCalibrationStandalone.h"

// ROOT
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TString.h"
#include "TCanvas.h"
#include "TRandom.h"
#include "TFile.h"
#include "TLegend.h"

#include "MyAnalyzer/MiniAnalyzer/plugins/DefineTreeVariables.h"
//#include "MyAnalyzer/MiniAnalyzer/plugins/DefineHistos.h"
#include "MyAnalyzer/MiniAnalyzer/plugins/DefineMuonHistos.h"
#include "MyAnalyzer/MiniAnalyzer/plugins/DefineMetHistos.h"
#include "MyAnalyzer/MiniAnalyzer/plugins/DefineDimuonHistos.h"
#include "MyAnalyzer/MiniAnalyzer/plugins/DefineJetHistos.h"
#include "MyAnalyzer/Rochester/src/RoccoR.cc"
//#include "RoccoR.cc"

class MiniAnalyzer : public edm::EDAnalyzer {
public:
  explicit MiniAnalyzer(const edm::ParameterSet&);
  ~MiniAnalyzer();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  bool isAncestor(const reco::Candidate * ancestor, const reco::Candidate * particle);
   
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  edm::Service<TFileService> fs; 
 
  // ---------- functions ----------------------------

  void initialize_Tree_variables();
  inline void clear_Arrays();
  
  double getRochester(pat::Muon muon,  bool MCtruth, bool crab, const RoccoR local_rc,  const RoccoR crab_rc);
  TLorentzVector buildDimuonPair(std::vector<pat::Muon> muon_pair, double k1, double k2);
  void selectDimuonPair(std::vector<std::pair <pat::Muon,bool> > tight_muons, std::vector<pat::Muon> &muon_pair, bool &triggerboth, bool &triggerfirst, bool &triggersecond);
  bool isMediumMuonBF(pat::Muon & recoMu);
  double GetScaleFactors(TFile *file, TString histoname, double eta, double pt);
  double GetScaleFactorsFromDir(TFile *file, TString histonameEtaPt, TString histonameEta, double maxPt, double eta, double pt);
  double GetScaleFactorsFromH(TH2F* Bin2D, TH1F* Bin1D, double maxPt, double eta, double pt);

  void CleanJetsFromMuons(std::vector<pat::Jet>& Jets, std::vector<pat::Muon>& Muons, float angle);
  
  bool JetID_isLoose(pat::Jet jet);
  bool JetID_isTight(pat::Jet jet);
  bool JetID_isTightLepVeto(pat::Jet jet);
  
  void fill_beamspot_variables(edm::Handle<reco::BeamSpot> beamspot);
  void fill_vtxs_variables(edm::Handle<reco::VertexCollection> vertices);
  void fill_hlt_variables(edm::Handle<edm::TriggerResults> triggerBits, const edm::Event& iEvent);
 
  void fill_muons_variables(edm::Handle<pat::MuonCollection> muons, const math::XYZPoint &thebs, reco::Vertex PV, const RoccoR local_rc,  const RoccoR crab_rc);
  // void fill_electrons_variables(edm::Handle<pat::ElectronCollection> electrons);
  
  void fill_PFJet_variables(edm::Handle<std::vector<pat::Jet> > jets, edm::Handle<edm::ValueMap<float> > puJetIdMVA, edm::Handle<edm::ValueMap<int> > puJetIdFlag, JetCorrectionUncertainty *jecUnc);
  
  void fill_PFMET_variables(edm::Handle<pat::METCollection> mets);
  // void fill_PileUp_variables(edm::Handle<std::vector<PileupSummaryInfo> > PupInfo);
  // void fill_rho_variables(edm::Handle<double> rho, edm::Handle<double> sigma);

  void fill_gen_variables(edm::Handle<edm::View<reco::GenParticle> > pruned, edm::Handle<edm::View<pat::PackedGenParticle> > packed);

  // ----------member data ---------------------------
  edm::EDGetTokenT<reco::BeamSpot> beamSpotToken_;
  edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
  
  edm::EDGetTokenT<pat::MuonCollection> muonToken_;
  //edm::EDGetTokenT<pat::ElectronCollection> electronToken_; 
  edm::EDGetTokenT<edm::ValueMap<bool> > vidToken_;
  edm::EDGetTokenT<pat::TauCollection> tauToken_;  

  edm::EDGetTokenT<pat::JetCollection> jetToken_;
  edm::EDGetTokenT<edm::ValueMap<float> > puJetIdToken_;
  edm::EDGetTokenT<edm::ValueMap<int> > puJetFlagToken_;
  edm::EDGetTokenT<std::vector<PileupSummaryInfo> > puInfoToken_;  

  edm::EDGetTokenT<pat::METCollection> metToken_;
  // --- met filters
  //edm::EDGetTokenT<edm::TriggerResults> metFilterToken_;
  //edm::EDGetTokenT<bool> BadChCandFilterToken_;
  //edm::EDGetTokenT<bool> BadPFMuonToken_;

  edm::EDGetTokenT<edm::View<reco::GenParticle> > prunedGenToken_;
  edm::EDGetTokenT<edm::View<pat::PackedGenParticle> > packedGenToken_;  //status 1 particles
  
  edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
  edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjects_;
  edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;  

  //edm::EDGetTokenT<LHEEventProduct> lhe_;
  //edm::EDGetTokenT<GenRunInfoProduct> runEventInfo_; //serve per la XS ma noi usiamo FH
  edm::EDGetTokenT<GenEventInfoProduct> genEventInfo_;

  // BTagCalibration calib;
  // BTagCalibrationReader reader; 
  

  edm::LumiReWeighting LumiWeights_; 
  edm::LumiReWeighting LumiWeightsUp_; 
  edm::LumiReWeighting LumiWeightsDown_; 

  TH1F *data; TH1F *mc; TH1F *weights;
  
  JetCorrectionUncertainty *jecUnc;
  JetCorrectionUncertainty *jecUnc_BCD, *jecUnc_EF, *jecUnc_G, *jecUnc_H;

  TFile *file_MuonTRIGrunBF;
  TFile *file_MuonTRIGrunGH;  
  TFile *file_MuonIDrunBF;
  TFile *file_MuonIDrunGH;
  TFile *file_MuonISOrunBF;
  TFile *file_MuonISOrunGH;
  //TFile *file_Btag_;
  
  TH2F *Bin2D_TRIGrunBF;
  TH1F *Bin1D_TRIGrunBF;
  TH2F *Bin2D_TRIGrunGH;
  TH1F *Bin1D_TRIGrunGH;
 
  TH2F *Bin2D_IDrunBF;
  TH1F *Bin1D_IDrunBF;
  TH2F *Bin2D_IDrunGH;
  TH1F *Bin1D_IDrunGH;
 
  TH2F *Bin2D_ISOrunBF;
  TH1F *Bin1D_ISOrunBF;
  TH2F *Bin2D_ISOrunGH;
  TH1F *Bin1D_ISOrunGH;
  
  TH2F *hpBin2D_TRIGrunBF;
  TH1F *hpBin1D_TRIGrunBF;
  TH2F *hpBin2D_TRIGrunGH;
  TH1F *hpBin1D_TRIGrunGH;
 
  TH2F *hpBin2D_IDrunBF;
  TH1F *hpBin1D_IDrunBF;
  TH2F *hpBin2D_IDrunGH;
  TH1F *hpBin1D_IDrunGH;
 
  TH2F *hpBin2D_ISOrunBF;
  TH1F *hpBin1D_ISOrunBF;
  TH2F *hpBin2D_ISOrunGH;
  TH1F *hpBin1D_ISOrunGH;

  //std::vector<std::string> metfilterNames;
  
  std::string pathNames_lowIsoUnpr;     
  //std::string pathNames_lowIsoUnprER;
  // std::string pathNames_lowIsoTkUnpr;     
  // std::string pathNames_lowIsoTkUnprER;
  //std::string pathNames_lowTkUnpr;     
  std::string pathNames_lowUnpr;

  // std::string filterLastName; 
  //std::string datafilepath;
  //Acceptance Cut
  double EtaMuonLoose;
  double PtMuonLoose;
  double EtaMuonTight;
  double PtMuonTight;
  double Isolation;
  double PtMet;
  double PtJet;
  double EtaJet;
  int    Particle;

  //flags
  bool Trigger_debug;
  bool debug;
  bool crab;
  bool MCtruth;
  bool Signal;
  bool checksOnDY;
  bool youwantAnalysis;
  bool youwantNTuples;
  // bool Analyze2012_;

  //counters
  short iBeam;   
  short iGenP;
  short trigger;
  short ihlt;
  short ivtx;
  short imuons;
  short GlobalMuons;
  short TrackerMuons;
  short StandAloneMuons;
  //int ipfelectrons;
  int ipfjets;
  short bTagJetsHE;
  short bTagJetsHP;
  short bTagJetsCSV;
  short ipfmets;
  
  //tree
  TTree *tree_;
  TTree *masstree_;
  TTree *masstree1_, *masstree11_, *masstree111_;
  TTree *masstree2_, *masstree22_, *masstree222_;

  //////////// my checks

  TH1F *histCounter;	
  TH1F *histEff;
  TH1F *histCutEff;
  
  TH1F *histGENWeights;
  TH1F *histEventWeights;
  TH1F *histPUWeights;
  TH1F *histPUWeights2;

  //Histos
  TH1F *histAllVertices;
  TH1F *histAllVerticesRW;
     
  //mctruth bjets
  TH1F *histTrue_bJets;  

  //variables
  double numEvents;
  double numEventsAlPicco;
  double  numEvents_mcweighted;
  double  numEvents_mcpuweighted;
  double  numEvents_mcpuweightedup;
  double  numEvents_mcpuweighteddown;

  TH1F *histEvents;
  TH1F *histEvents_mcweighted;
  TH1F *histEvents_mcpuweighted;
  TH1F *histEvents_mcpuweightedup;
  TH1F *histEvents_mcpuweighteddown;

  double triggerEvents;
  double goodMuonsEvents;
  double triggerMatchedEvents;
  double SamePVEvents;
  
  std::vector<pat::PackedGenParticle> goodGenMuons; 
  std::vector<pat::PackedGenParticle> goodGenMuonsAcc; 
  std::vector<pat::Muon> goodRecoMuons;
  std::vector<pat::Muon> goodRecoMuonsAcc;
  std::vector<pat::Muon> goodMuons;
  std::vector<pat::Muon> mediumMuons;
  std::vector<pat::Muon> tightMuons;

  TLorentzVector mediumZ;
  TLorentzVector tightZ;

  std::vector<pat::PackedGenParticle> genMuons; 
  std::vector<pat::PackedGenParticle> genMuonsAcc; 
  std::vector<pat::PackedGenParticle> genDimuons; 
  std::vector<pat::Muon> theTriggerMuons; 
  std::vector<pat::Muon> theLeadingMuons; 

  double mGenZ;
  double mGenDimuon;

  double  generatedZ;
  double  generatedMuons;
  double  generatedMuons_acc;
  double  matchedMuons_trigger;

  double generatedEvents;
  double generatedEventsAcc;
  double triggeredEvents;
  double triggerMatchEvents;

  double RECO;
  double TRIGGER;
  double TIGHTID;
  double GLOBALPTETA;
  double GLB;
  double PF;
  double CHI2;
  double MATCHST;
  double MUVALIDHITS;
  double DXY;
  double DZ;
  double PIXEL;
  double TRACKER;
  double ISO;
  double MET;
  double CAT1,  CAT1_mcpuweighted,  CAT1_mcpuweightedup, CAT1_mcpuweighteddown;
  double CAT2,  CAT2_mcpuweighted,  CAT2_mcpuweightedup, CAT2_mcpuweighteddown;

  double MEDIUM;
  double METM;
  double CAT1M;
  double CAT2M;


  //temporary
  int GenEvent2mu;
  int GenEventb;
  int GenEventHE;

  int GenMu;
  int GenMuPtTight;

  RoccoR  crab_rc;
  RoccoR  local_rc;

};
#endif 


#include<iostream>

#include "TFile.h"
#include "TTree.h"
#include "TVectorF.h"
#include "TMap.h"
#include "TClonesArray.h"

#include "UserCode/federica/interface/DefineTreeVariables_pat.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "TrackingTools/GeomPropagators/interface/Propagator.h"

#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

//
// class declaration
//
class HmumuNtuple_pat : public edm::EDAnalyzer {
  
 public:
  explicit HmumuNtuple_pat(const edm::ParameterSet&);
  ~HmumuNtuple_pat() {};
  
 private:
  virtual void beginJob() ;
  virtual void beginRun(const edm::Run&, const edm::EventSetup&) {};
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  void initialize_Tree_variables();
  inline void clear_Arrays();
  void fill_beamspot_variables(edm::Handle<reco::BeamSpot> recoBeamSpotHandle);
  void fill_GenParticle_variables (edm::Handle<reco::GenParticleCollection> GenP);
  void fill_hlt_variables(const edm::Event& e, edm::Handle<edm::TriggerResults> hltresults);
  void fill_vtxs_variables(edm::Handle<reco::VertexCollection> privtxs);
  void fill_muons_variables(edm::Handle<pat::MuonCollection> MuList,edm::Handle<reco::VertexCollection> privtxs);
  void fill_PFEle_variables(edm::Handle<pat::ElectronCollection> pfEle);
  void fill_PFJet_variables(edm::Handle<edm::View<pat::Jet> > jets, edm::Handle<edm::ValueMap<float> > puJetIdMVA, edm::Handle<edm::ValueMap<int> > puJetIdFlag); 
  void fill_PFMET_variables(edm::Handle<pat::METCollection> pfMET);
  void fill_PileUp_variables(edm::Handle<std::vector<PileupSummaryInfo> > PupInfo);
  void fill_rho_variables(edm::Handle<double> rho, edm::Handle<double> sigma);

  edm::InputTag hltresults_;
  //  edm::InputTag triggerEvent_;
  edm::InputTag PrimaryVertexTag_;
  edm::InputTag beamSpotTag_;
  edm::InputTag staMuLabel_;
  edm::InputTag PFMETCollection_;
  edm::InputTag PFJetCollection_;
  edm::InputTag PFEleCollection_;
  edm::InputTag GenParticleCollection_;
  edm::InputTag PileupSrc_;
  edm::InputTag TrackerTracks_;
  
  std::string outFile_;
  std::string datafilepath;
  std::string muonMatch_;
  std::string nameTree_;
  
  edm::LumiReWeighting LumiWeights_; 

  std::string fitParametersFile_;

  bool debug_;
  bool MCtruth_;
  bool youwantAnalysis_;
  bool youwantNTuples_;
  bool Analyze2012_;

  short nMU_15;
  short nMU_20;

  //counters
  short ihlt;
  short ivtx;
  short imuons;
  short GlobalMuons;
  short TrackerMuons;
  short StandAloneMuons;
  int ipfelectrons;
  int ipfjets;
  short bTagJetsHE;
  short bTagJetsHP;
  short bTagJetsCSV;
 
  short ipfmets;

  short iGenP;

  short iBeam;
  
  reco::BeamSpot beamspot;

  double RHO;
  double SIGMA;
  double RHO_L2L3;
  double SIGMA_L2L3;

  TFile *outFile;
  TTree *tree_;

};

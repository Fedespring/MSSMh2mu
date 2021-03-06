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


// user include files
#include "MyAnalyzer/MiniAnalyzer/plugins/MiniAnalyzer.h"

using namespace std;
using namespace edm;
using namespace reco;

MiniAnalyzer::MiniAnalyzer(const edm::ParameterSet& pset):

  beamSpotToken_(consumes<reco::BeamSpot>(pset.getParameter<edm::InputTag>("beamspot"))),
  vtxToken_(consumes<reco::VertexCollection>(pset.getParameter<edm::InputTag>("vertices"))),
  
  muonToken_(consumes<pat::MuonCollection>(pset.getParameter<edm::InputTag>("muons"))),
  // electronToken_(consumes<pat::ElectronCollection>(pset.getParameter<edm::InputTag>("electrons"))),
  // vidToken_(consumes<edm::ValueMap<bool> >(pset.getParameter<edm::InputTag>("electronsID"))),
  // tauToken_(consumes<pat::TauCollection>(pset.getParameter<edm::InputTag>("taus"))),

  jetToken_(consumes<pat::JetCollection>(pset.getParameter<edm::InputTag>("jets"))),
  puJetIdToken_(consumes<edm::ValueMap<float> >(pset.getParameter<edm::InputTag>("puJetIdMVA"))),
  puJetFlagToken_(consumes<edm::ValueMap<int> >(pset.getParameter<edm::InputTag>("puJetIdFlag"))),
  puInfoToken_(consumes<std::vector<PileupSummaryInfo> > (pset.getParameter<edm::InputTag>("puInfo"))),
  
  metToken_(consumes<pat::METCollection>(pset.getParameter<edm::InputTag>("mets"))),
 
  //metFilterToken_(consumes<edm::TriggerResults>(pset.getParameter<edm::InputTag> ("metFilterToken"))),
  //BadChCandFilterToken_(consumes<bool>(pset.getParameter < edm::InputTag > ("BadChCandFilterToken"))),
  //BadPFMuonToken_(consumes<bool>(pset.getParameter < edm::InputTag > ("BadPFMuonToken"))),
    
  prunedGenToken_(consumes<edm::View<reco::GenParticle> >(pset.getParameter<edm::InputTag>("pruned"))),
  packedGenToken_(consumes<edm::View<pat::PackedGenParticle> >(pset.getParameter<edm::InputTag>("packed"))),

  triggerBits_(consumes<edm::TriggerResults>(pset.getParameter<edm::InputTag>("bits"))),
  triggerObjects_(consumes<pat::TriggerObjectStandAloneCollection>(pset.getParameter<edm::InputTag>("objects"))),
  triggerPrescales_(consumes<pat::PackedTriggerPrescales>(pset.getParameter<edm::InputTag>("prescales"))),

  // lhe_(consumes<LHEEventProduct>(pset.getParameter<edm::InputTag>("lhe"))),
  //  runEventInfo_(consumes<GenRunInfoProduct,edm::InRun>(pset.getParameter<edm::InputTag>("runEventInfo"))),
  genEventInfo_(consumes<GenEventInfoProduct>(pset.getParameter<edm::InputTag>("genEventInfo"))){
 
  //flags
  crab                = pset.getParameter<bool>("crab"); 
  debug               = pset.getParameter<bool>("debug"); 
  Trigger_debug       = pset.getParameter<bool>("Trigger_debug");  
  MCtruth             = pset.getParameter<bool>("MCtruth");
  Signal              = pset.getParameter<bool>("Signal");
  checksOnDY          = pset.getParameter<bool>("checksOnDY");
  youwantAnalysis     = pset.getParameter<bool>("youwantAnalysis");
  youwantNTuples      = pset.getParameter<bool>("youwantNTuples");
    
  //  metfilterNames = pset.getParameter < std::vector<std::string> > ("metfilterNames");

  //Trigger
  pathNames_lowIsoUnpr     = pset.getParameter<std::string>("pathNames_lowIsoUnpr");
  pathNames_lowUnpr        = pset.getParameter<std::string>("pathNames_lowUnpr");
  
  //Acceptance cuts
  EtaMuonLoose  = pset.getParameter<double>("EtaMuonLoose"); 
  PtMuonLoose   = pset.getParameter<double>("PtMuonLoose"); 
  EtaMuonTight  = pset.getParameter<double>("EtaMuonTight"); 
  PtMuonTight   = pset.getParameter<double>("PtMuonTight");
  Isolation     = pset.getParameter<double>("Isolation");
  PtMet         = pset.getParameter<double>("PtMet");
  PtJet         = pset.getParameter<double>("PtJet");
  EtaJet        = pset.getParameter<double>("EtaJet");
  Particle      = pset.getParameter<int>("Particle"); 
 
  //initialize the event variables
  initialize_Tree_variables();
  
  //counters
  iBeam             = 0;

  iGenP             = 0;

  ihlt              = 0;
  
  ivtx              = 0;

  imuons            = 0;
  GlobalMuons       = 0;
  TrackerMuons      = 0;
  StandAloneMuons   = 0;
  
  ipfjets           = 0;
  bTagJetsHE        = 0;
  bTagJetsHP        = 0;
  bTagJetsCSV       = 0;
  
  ipfmets           = 0;
  
       
  //histos
  edm::Service<TFileService> fs;

  histCounter = fs->make<TH1F>("histCounter", "Number of events; cut; events", 22, 0., 22.);						
  histCutEff = fs->make<TH1F>("histCutEff", "Cut/Acceptance efficiency; efficiency; events", 15, 0., 15.);
  histEff = fs->make<TH1F>("histEff", "Efficiency; efficiency; events", 15, 0., 15.);
 
  histPUWeights = fs->make<TH1F>("histPUWeights", "PU weights; weights; events",1000,0.,100.);
  histPUWeights2 = fs->make<TH1F>("histPUWeights2", "PU weights; #trueInt; weights",400,0.,80.);

  //PV for preselected events (2 muons in the acceptance)
  histAllVertices         = fs->make<TH1F>("histAllVertices", "All Primary Vertices; vertices; events",60,0.,60.);
  histAllVerticesRW       = fs->make<TH1F>("histAllVerticesRW","All Primary Vertices PU reweighted; vertices; events",60,0.,60.);

  // const Int_t EtaBINS = 14;
  // Double_t EtaEDGES[EtaBINS + 1] = {-2.4, -2.2, -1.8, -1.4, -1.1, -0.6, -0.3, 0, 0.3, 0.6, 1.1, 1.4, 1.8, 2.2, 2.4};
  
  //const Int_t PtBINS = 24;
  // Double_t PtEDGES[PtBINS + 1] = {0., 10., 20., 30., 40., 50., 60., 70.,80.,90.,100.,110., 120.,130.,140.,150.,160.,170.,200.,250.,300.,400.,500.,600.,700.};
  
  //   histAllMuons_pt       = fs->make<TH1F>("histAllMuons_pt","All the Global muons: p_{T}", PtBINS, PtEDGES);
  // histAllMuons_eta        = fs->make<TH1F>("histAllMuons_eta","All the Global muons: Eta", EtaBINS, EtaEDGES);

  Minv_Cat1   = fs->make<TH1F>("Minv_Cat1", "Dimuon mass cat1(b jets); mass (GeV/c^{2}); Events", 20000, 0, 2000);
  Minv_Cat2   = fs->make<TH1F>("Minv_Cat2", "Dimuon mass cat2(no b jets); mass (GeV/c^{2}); Events", 20000, 0, 2000);
  Minv_Cat11  = fs->make<TH1F>("Minv_Cat11", "Dimuon mass cat1(1 b jet); mass (GeV/c^{2}); Events", 20000, 0, 2000);
  Minv_Cat22  = fs->make<TH1F>("Minv_Cat22", "Dimuon mass cat2(0,1 light jet); mass (GeV/c^{2}); Events", 20000, 0, 2000);
  Minv_Cat111 = fs->make<TH1F>("Minv_Cat111", "Dimuon mass cat1(2 b jets); mass (GeV/c^{2}); Events", 20000, 0, 2000);
  Minv_Cat222 = fs->make<TH1F>("Minv_Cat222", "Dimuon mass cat2(2 light jets); mass (GeV/c^{2}); Events", 20000, 0, 2000);

  TFileDirectory MUONsubDir = fs->mkdir( "Muons" );
   
  histAllMuons_pt    = MUONsubDir.make<TH1F>("histAllMuons_pt","Preselected(acceptance) muons: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histAllMuons_eta   = MUONsubDir.make<TH1F>("histAllMuons_eta","Preselected(acceptance) muons: #eta; #eta; muons", 50, -2.5, 2.5);
  histAllMuons_phi   = MUONsubDir.make<TH1F>("histAllMuons_phi","Preselected(acceptance) muons: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histAllMuons_pv    = MUONsubDir.make<TH1F>("histAllMuons_pv","Preselected(acceptance) muons: PV; vertices; muons", 60, 0., 60.);
  histAllMuons_timingVeto = MUONsubDir.make<TH1F>("histAllMuons_timingVeto","Preselected(acceptance) muons: is OOT; bool; muons", 2, 0., 2.);

  histTriggerMuons_pt    = MUONsubDir.make<TH1F>("histTriggerMuons_pt","Triggered events: muons p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histTriggerMuons_eta   = MUONsubDir.make<TH1F>("histTriggerMuons_eta","Triggered events: muons #eta; #eta; muons", 50, -2.5, 2.5);
  histTriggerMuons_phi   = MUONsubDir.make<TH1F>("histTriggerMuons_phi","Triggered events: muons #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histTriggerMuons_pv    = MUONsubDir.make<TH1F>("histTriggerMuons_pv","Triggered events: PV; vertices; muons", 60, 0., 60.);

  //histos for muon quantities

  histNormChi2    = MUONsubDir.make<TH1F>("histNormChi2", "Muons passing acceptance & trigger: #chi^{2}/ndof; #chi^{2}/ndof; muons ", 100, 0., 20.);
  histMatchSt     = MUONsubDir.make<TH1F>("histMatchSt", "Muons passing acceptance & trigger: matched stations; # of stations; muons ", 10, 0., 10.);
  histMuValidHits = MUONsubDir.make<TH1F>("histMuValidHits", "Muons passing acceptance & trigger: muon valid hits; # of hits; muons ", 60, 0., 60.);
  histDxy         = MUONsubDir.make<TH1F>("histDxy", "Muons passing acceptance & trigger: dxy; dxy (cm); muons ", 100, -0.5, 0.5);
  histDz          = MUONsubDir.make<TH1F>("histDz", "Muons passing acceptance & trigger: dz; dz (cm); muons ", 100, -0.5, 0.5);
  histPixelHits   = MUONsubDir.make<TH1F>("histPixelHits", "Muons passing acceptance & trigger: pixel hits; # of hits; muons ", 20, 0., 20.);
  histTrackerL    = MUONsubDir.make<TH1F>("histTrackerL", "Muons passing acceptance & trigger: tracker layers; # of layers; muons ", 30, 0., 30.);
  histDeltaPtOverPt = MUONsubDir.make<TH1F>("histDeltaPtOverPt", "Muons passing acceptance & trigger: relative p_{T} error; #Delta p_{T}/p_{T}; muons ", 100, 0., 1.);

  //N-1 histos  ---- no RW is needed -----
  histPt_num    = MUONsubDir.make<TH1F>("histPt_num","Muons passing tightID & Iso: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histEta_num   = MUONsubDir.make<TH1F>("histEta_num","Muons passing tightID & Iso: #eta; #eta; muons", 50, -2.5, 2.5);
  histPhi_num   = MUONsubDir.make<TH1F>("histPhi_num","Muons passing tightID & Iso: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histPV_num    = MUONsubDir.make<TH1F>("histPV_num","Muons passing tightID & Iso: PV; vertices; muons", 60, 0., 60.);
  histNormChi2_num    = MUONsubDir.make<TH1F>("histNormChi2_num", "Muons passing tightID & Iso: #chi^{2}/ndof; #chi^{2}/ndof; muons ", 100, 0., 20.);
  histMatchSt_num     = MUONsubDir.make<TH1F>("histMatchSt_num", "Muons passing tightID & Iso: matched stations; # of stations; muons ", 10, 0., 10.);
  histMuValidHits_num = MUONsubDir.make<TH1F>("histMuValidHits_num", "Muons passing tightID & Iso: muon valid hits; # of hits; muons ", 60, 0., 60.);
  histDxy_num         = MUONsubDir.make<TH1F>("histDxy_num", "Muons passing tightID & Iso: dxy; dxy (cm); muons ", 100, -0.5, 0.5);
  histDz_num          = MUONsubDir.make<TH1F>("histDz_num", "Muons passing tightID & Iso: dz; dz (cm); muons ", 100, -0.5, 0.5);
  histPixelHits_num   = MUONsubDir.make<TH1F>("histPixelHits_num", "Muons passing tightID & Iso: pixel hits; # of hits; muons ", 20, 0., 20.);
  histTrackerL_num    = MUONsubDir.make<TH1F>("histTrackerL_num", "Muons passing tightID & Iso: tracker layers; # of layers; muons ", 30, 0., 30.);
  histDeltaPtOverPt_num = MUONsubDir.make<TH1F>("histDeltaPtOverPt_num", "Muons passing tightID & Iso: relative p_{T} error; #Delta p_{T}/p_{T}; muons ", 100, 0., 1.);

  histPt_tkl    = MUONsubDir.make<TH1F>("histPt_tkl","Muons passing tightID & Iso: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histEta_tkl   = MUONsubDir.make<TH1F>("histEta_tkl","Muons passing tightID & Iso: #eta; #eta; muons", 50, -2.5, 2.5);
  histPhi_tkl   = MUONsubDir.make<TH1F>("histPhi_tkl","Muons passing tightID & Iso: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histPV_tkl    = MUONsubDir.make<TH1F>("histPV_tkl","Muons passing tightID & Iso: PV; vertices; muons", 60, 0., 60.);
  histNormChi2_tkl    = MUONsubDir.make<TH1F>("histNormChi2_tkl", "Muons passing tightID & Iso: #chi^{2}/ndof; #chi^{2}/ndof; muons ", 100, 0., 20.);
  histMatchSt_tkl     = MUONsubDir.make<TH1F>("histMatchSt_tkl", "Muons passing tightID & Iso: matched stations; # of stations; muons ", 10, 0., 10.);
  histMuValidHits_tkl = MUONsubDir.make<TH1F>("histMuValidHits_tkl", "Muons passing tightID & Iso: muon valid hits; # of hits; muons ", 60, 0., 60.);
  histDxy_tkl         = MUONsubDir.make<TH1F>("histDxy_tkl", "Muons passing tightID & Iso: dxy; dxy (cm); muons ", 100, -0.5, 0.5);
  histDz_tkl          = MUONsubDir.make<TH1F>("histDz_tkl", "Muons passing tightID & Iso: dz; dz (cm); muons ", 100, -0.5, 0.5);
  histPixelHits_tkl   = MUONsubDir.make<TH1F>("histPixelHits_tkl", "Muons passing tightID & Iso: pixel hits; # of hits; muons ", 20, 0., 20.);
  histTrackerL_tkl    = MUONsubDir.make<TH1F>("histTrackerL_tkl", "Muons passing tightID & Iso: tracker layers; # of layers; muons ", 30, 0., 30.);
  histDeltaPtOverPt_tkl = MUONsubDir.make<TH1F>("histDeltaPtOverPt_tkl", "Muons passing tightID & Iso: relative p_{T} error; #Delta p_{T}/p_{T}; muons ", 100, 0., 1.);

  histPt_pixh    = MUONsubDir.make<TH1F>("histPt_pixh","Muons passing tightID & Iso: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histEta_pixh   = MUONsubDir.make<TH1F>("histEta_pixh","Muons passing tightID & Iso: #eta; #eta; muons", 50, -2.5, 2.5);
  histPhi_pixh   = MUONsubDir.make<TH1F>("histPhi_pixh","Muons passing tightID & Iso: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histPV_pixh    = MUONsubDir.make<TH1F>("histPV_pixh","Muons passing tightID & Iso: PV; vertices; muons", 60, 0., 60.);
  histNormChi2_pixh    = MUONsubDir.make<TH1F>("histNormChi2_pixh", "Muons passing tightID & Iso: #chi^{2}/ndof; #chi^{2}/ndof; muons ", 100, 0., 20.);
  histMatchSt_pixh     = MUONsubDir.make<TH1F>("histMatchSt_pixh", "Muons passing tightID & Iso: matched stations; # of stations; muons ", 10, 0., 10.);
  histMuValidHits_pixh = MUONsubDir.make<TH1F>("histMuValidHits_pixh", "Muons passing tightID & Iso: muon valid hits; # of hits; muons ", 60, 0., 60.);
  histDxy_pixh         = MUONsubDir.make<TH1F>("histDxy_pixh", "Muons passing tightID & Iso: dxy; dxy (cm); muons ", 100, -0.5, 0.5);
  histDz_pixh          = MUONsubDir.make<TH1F>("histDz_pixh", "Muons passing tightID & Iso: dz; dz (cm); muons ", 100, -0.5, 0.5);
  histPixelHits_pixh   = MUONsubDir.make<TH1F>("histPixelHits_pixh", "Muons passing tightID & Iso: pixel hits; # of hits; muons ", 20, 0., 20.);
  histTrackerL_pixh    = MUONsubDir.make<TH1F>("histTrackerL_pixh", "Muons passing tightID & Iso: tracker layers; # of layers; muons ", 30, 0., 30.);
  histDeltaPtOverPt_pixh = MUONsubDir.make<TH1F>("histDeltaPtOverPt_pixh", "Muons passing tightID & Iso: relative p_{T} error; #Delta p_{T}/p_{T}; muons ", 100, 0., 1.);

  histPt_muh    = MUONsubDir.make<TH1F>("histPt_muh","Muons passing tightID & Iso: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histEta_muh   = MUONsubDir.make<TH1F>("histEta_muh","Muons passing tightID & Iso: #eta; #eta; muons", 50, -2.5, 2.5);
  histPhi_muh   = MUONsubDir.make<TH1F>("histPhi_muh","Muons passing tightID & Iso: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histPV_muh    = MUONsubDir.make<TH1F>("histPV_muh","Muons passing tightID & Iso: PV; vertices; muons", 60, 0., 60.);
  histNormChi2_muh    = MUONsubDir.make<TH1F>("histNormChi2_muh", "Muons passing tightID & Iso: #chi^{2}/ndof; #chi^{2}/ndof; muons ", 100, 0., 20.);
  histMatchSt_muh     = MUONsubDir.make<TH1F>("histMatchSt_muh", "Muons passing tightID & Iso: matched stations; # of stations; muons ", 10, 0., 10.);
  histMuValidHits_muh = MUONsubDir.make<TH1F>("histMuValidHits_muh", "Muons passing tightID & Iso: muon valid hits; # of hits; muons ", 60, 0., 60.);
  histDxy_muh         = MUONsubDir.make<TH1F>("histDxy_muh", "Muons passing tightID & Iso: dxy; dxy (cm); muons ", 100, -0.5, 0.5);
  histDz_muh          = MUONsubDir.make<TH1F>("histDz_muh", "Muons passing tightID & Iso: dz; dz (cm); muons ", 100, -0.5, 0.5);
  histPixelHits_muh   = MUONsubDir.make<TH1F>("histPixelHits_muh", "Muons passing tightID & Iso: pixel hits; # of hits; muons ", 20, 0., 20.);
  histTrackerL_muh    = MUONsubDir.make<TH1F>("histTrackerL_muh", "Muons passing tightID & Iso: tracker layers; # of layers; muons ", 30, 0., 30.);
  histDeltaPtOverPt_muh = MUONsubDir.make<TH1F>("histDeltaPtOverPt_muh", "Muons passing tightID & Iso: relative p_{T} error; #Delta p_{T}/p_{T}; muons ", 100, 0., 1.);

  histPt_db    = MUONsubDir.make<TH1F>("histPt_db","Muons passing tightID & Iso: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histEta_db   = MUONsubDir.make<TH1F>("histEta_db","Muons passing tightID & Iso: #eta; #eta; muons", 50, -2.5, 2.5);
  histPhi_db   = MUONsubDir.make<TH1F>("histPhi_db","Muons passing tightID & Iso: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histPV_db    = MUONsubDir.make<TH1F>("histPV_db","Muons passing tightID & Iso: PV; vertices; muons", 60, 0., 60.);
  histNormChi2_db    = MUONsubDir.make<TH1F>("histNormChi2_db", "Muons passing tightID & Iso: #chi^{2}/ndof; #chi^{2}/ndof; muons ", 100, 0., 20.);
  histMatchSt_db     = MUONsubDir.make<TH1F>("histMatchSt_db", "Muons passing tightID & Iso: matched stations; # of stations; muons ", 10, 0., 10.);
  histMuValidHits_db = MUONsubDir.make<TH1F>("histMuValidHits_db", "Muons passing tightID & Iso: muon valid hits; # of hits; muons ", 60, 0., 60.);
  histDxy_db         = MUONsubDir.make<TH1F>("histDxy_db", "Muons passing tightID & Iso: dxy; dxy (cm); muons ", 100, -0.5, 0.5);
  histDz_db          = MUONsubDir.make<TH1F>("histDz_db", "Muons passing tightID & Iso: dz; dz (cm); muons ", 100, -0.5, 0.5);
  histPixelHits_db   = MUONsubDir.make<TH1F>("histPixelHits_db", "Muons passing tightID & Iso: pixel hits; # of hits; muons ", 20, 0., 20.);
  histTrackerL_db    = MUONsubDir.make<TH1F>("histTrackerL_db", "Muons passing tightID & Iso: tracker layers; # of layers; muons ", 30, 0., 30.);
  histDeltaPtOverPt_db = MUONsubDir.make<TH1F>("histDeltaPtOverPt_db", "Muons passing tightID & Iso: relative p_{T} error; #Delta p_{T}/p_{T}; muons ", 100, 0., 1.);
  
  histPt_dz    = MUONsubDir.make<TH1F>("histPt_dz","Muons passing tightID & Iso: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histEta_dz   = MUONsubDir.make<TH1F>("histEta_dz","Muons passing tightID & Iso: #eta; #eta; muons", 50, -2.5, 2.5);
  histPhi_dz   = MUONsubDir.make<TH1F>("histPhi_dz","Muons passing tightID & Iso: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histPV_dz    = MUONsubDir.make<TH1F>("histPV_dz","Muons passing tightID & Iso: PV; vertices; muons", 60, 0., 60.);
  histNormChi2_dz    = MUONsubDir.make<TH1F>("histNormChi2_dz", "Muons passing tightID & Iso: #chi^{2}/ndof; #chi^{2}/ndof; muons ", 100, 0., 20.);
  histMatchSt_dz     = MUONsubDir.make<TH1F>("histMatchSt_dz", "Muons passing tightID & Iso: matched stations; # of stations; muons ", 10, 0., 10.);
  histMuValidHits_dz = MUONsubDir.make<TH1F>("histMuValidHits_dz", "Muons passing tightID & Iso: muon valid hits; # of hits; muons ", 60, 0., 60.);
  histDxy_dz         = MUONsubDir.make<TH1F>("histDxy_dz", "Muons passing tightID & Iso: dxy; dxy (cm); muons ", 100, -0.5, 0.5);
  histDz_dz          = MUONsubDir.make<TH1F>("histDz_dz", "Muons passing tightID & Iso: dz; dz (cm); muons ", 100, -0.5, 0.5);
  histPixelHits_dz   = MUONsubDir.make<TH1F>("histPixelHits_dz", "Muons passing tightID & Iso: pixel hits; # of hits; muons ", 20, 0., 20.);
  histTrackerL_dz    = MUONsubDir.make<TH1F>("histTrackerL_dz", "Muons passing tightID & Iso: tracker layers; # of layers; muons ", 30, 0., 30.);
  histDeltaPtOverPt_dz = MUONsubDir.make<TH1F>("histDeltaPtOverPt_dz", "Muons passing tightID & Iso: relative p_{T} error; #Delta p_{T}/p_{T}; muons ", 100, 0., 1.);

  histPt_chi2    = MUONsubDir.make<TH1F>("histPt_chi2","Muons passing tightID & Iso: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histEta_chi2   = MUONsubDir.make<TH1F>("histEta_chi2","Muons passing tightID & Iso: #eta; #eta; muons", 50, -2.5, 2.5);
  histPhi_chi2   = MUONsubDir.make<TH1F>("histPhi_chi2","Muons passing tightID & Iso: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histPV_chi2    = MUONsubDir.make<TH1F>("histPV_chi2","Muons passing tightID & Iso: PV; vertices; muons", 60, 0., 60.);
  histNormChi2_chi2    = MUONsubDir.make<TH1F>("histNormChi2_chi2", "Muons passing tightID & Iso: #chi^{2}/ndof; #chi^{2}/ndof; muons ", 100, 0., 20.);
  histMatchSt_chi2     = MUONsubDir.make<TH1F>("histMatchSt_chi2", "Muons passing tightID & Iso: matched stations; # of stations; muons ", 10, 0., 10.);
  histMuValidHits_chi2 = MUONsubDir.make<TH1F>("histMuValidHits_chi2", "Muons passing tightID & Iso: muon valid hits; # of hits; muons ", 60, 0., 60.);
  histDxy_chi2         = MUONsubDir.make<TH1F>("histDxy_chi2", "Muons passing tightID & Iso: dxy; dxy (cm); muons ", 100, -0.5, 0.5);
  histDz_chi2          = MUONsubDir.make<TH1F>("histDz_chi2", "Muons passing tightID & Iso: dz; dz (cm); muons ", 100, -0.5, 0.5);
  histPixelHits_chi2   = MUONsubDir.make<TH1F>("histPixelHits_chi2", "Muons passing tightID & Iso: pixel hits; # of hits; muons ", 20, 0., 20.);
  histTrackerL_chi2    = MUONsubDir.make<TH1F>("histTrackerL_chi2", "Muons passing tightID & Iso: tracker layers; # of layers; muons ", 30, 0., 30.);
  histDeltaPtOverPt_chi2 = MUONsubDir.make<TH1F>("histDeltaPtOverPt_chi2", "Muons passing tightID & Iso: relative p_{T} error; #Delta p_{T}/p_{T}; muons ", 100, 0., 1.);

  histPt_mst    = MUONsubDir.make<TH1F>("histPt_mst","Muons passing tightID & Iso: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histEta_mst   = MUONsubDir.make<TH1F>("histEta_mst","Muons passing tightID & Iso: #eta; #eta; muons", 50, -2.5, 2.5);
  histPhi_mst   = MUONsubDir.make<TH1F>("histPhi_mst","Muons passing tightID & Iso: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histPV_mst    = MUONsubDir.make<TH1F>("histPV_mst","Muons passing tightID & Iso: PV; vertices; muons", 60, 0., 60.);
  histNormChi2_mst    = MUONsubDir.make<TH1F>("histNormChi2_mst", "Muons passing tightID & Iso: #chi^{2}/ndof; #chi^{2}/ndof; muons ", 100, 0., 20.);
  histMatchSt_mst     = MUONsubDir.make<TH1F>("histMatchSt_mst", "Muons passing tightID & Iso: matched stations; # of stations; muons ", 10, 0., 10.);
  histMuValidHits_mst = MUONsubDir.make<TH1F>("histMuValidHits_mst", "Muons passing tightID & Iso: muon valid hits; # of hits; muons ", 60, 0., 60.);
  histDxy_mst         = MUONsubDir.make<TH1F>("histDxy_mst", "Muons passing tightID & Iso: dxy; dxy (cm); muons ", 100, -0.5, 0.5);
  histDz_mst          = MUONsubDir.make<TH1F>("histDz_mst", "Muons passing tightID & Iso: dz; dz (cm); muons ", 100, -0.5, 0.5);
  histPixelHits_mst   = MUONsubDir.make<TH1F>("histPixelHits_mst", "Muons passing tightID & Iso: pixel hits; # of hits; muons ", 20, 0., 20.);
  histTrackerL_mst    = MUONsubDir.make<TH1F>("histTrackerL_mst", "Muons passing tightID & Iso: tracker layers; # of layers; muons ", 30, 0., 30.);
  histDeltaPtOverPt_mst = MUONsubDir.make<TH1F>("histDeltaPtOverPt_mst", "Muons passing tightID & Iso: relative p_{T} error; #Delta p_{T}/p_{T}; muons ", 100, 0., 1.);

  histPt_iso    = MUONsubDir.make<TH1F>("histPt_iso","Muons passing tightID & Iso: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histEta_iso   = MUONsubDir.make<TH1F>("histEta_iso","Muons passing tightID & Iso: #eta; #eta; muons", 50, -2.5, 2.5);
  histPhi_iso   = MUONsubDir.make<TH1F>("histPhi_iso","Muons passing tightID & Iso: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histPV_iso    = MUONsubDir.make<TH1F>("histPV_iso","Muons passing tightID & Iso: PV; vertices; muons", 60, 0., 60.);
  histNormChi2_iso    = MUONsubDir.make<TH1F>("histNormChi2_iso", "Muons passing tightID & Iso: #chi^{2}/ndof; #chi^{2}/ndof; muons ", 100, 0., 20.);
  histMatchSt_iso     = MUONsubDir.make<TH1F>("histMatchSt_iso", "Muons passing tightID & Iso: matched stations; # of stations; muons ", 10, 0., 10.);
  histMuValidHits_iso = MUONsubDir.make<TH1F>("histMuValidHits_iso", "Muons passing tightID & Iso: muon valid hits; # of hits; muons ", 60, 0., 60.);
  histDxy_iso         = MUONsubDir.make<TH1F>("histDxy_iso", "Muons passing tightID & Iso: dxy; dxy (cm); muons ", 100, -0.5, 0.5);
  histDz_iso          = MUONsubDir.make<TH1F>("histDz_iso", "Muons passing tightID & Iso: dz; dz (cm); muons ", 100, -0.5, 0.5);
  histPixelHits_iso   = MUONsubDir.make<TH1F>("histPixelHits_iso", "Muons passing tightID & Iso: pixel hits; # of hits; muons ", 20, 0., 20.);
  histTrackerL_iso    = MUONsubDir.make<TH1F>("histTrackerL_iso", "Muons passing tightID & Iso: tracker layers; # of layers; muons ", 30, 0., 30.);
  histDeltaPtOverPt_iso = MUONsubDir.make<TH1F>("histDeltaPtOverPt_iso", "Muons passing tightID & Iso: relative p_{T} error; #Delta p_{T}/p_{T}; muons ", 100, 0., 1.);

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  histMediumMuons_pt    = MUONsubDir.make<TH1F>("histMediumMuons_pt","Muons passing MediumID: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histMediumMuons_eta   = MUONsubDir.make<TH1F>("histMediumMuons_eta","Muons passing MediumID: #eta; #eta; muons", 50, -2.5, 2.5);
  histMediumMuons_phi   = MUONsubDir.make<TH1F>("histMediumMuons_phi","Muons passing MediumID: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histMediumMuons_pv    = MUONsubDir.make<TH1F>("histMediumMuons_pv","Muons passing MediumID: PV; vertices; muons", 60, 0., 60.);
  histMediumMuons_timingVeto = MUONsubDir.make<TH1F>("histMediumMuons_timingVeto","Muons passing MediumID: is OOT; bool; muons", 2, 0., 2.);
  histMediumMuons_pfiso = MUONsubDir.make<TH1F>("histMediumMuons_pfiso","Thight Muons: PFIso04 + #delta #beta ; rel iso; muons", 200, 0., 10.);
  histMediumMuons_tbiso = MUONsubDir.make<TH1F>("histMediumMuons_tbiso","Thight Muons: TrackerIso03 ; rel iso; muons", 200, 0., 10.);
  histIsoMediumMuons_pt    = MUONsubDir.make<TH1F>("histIsoMediumMuons_pt","Muons passing Isolation: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histIsoMediumMuons_eta   = MUONsubDir.make<TH1F>("histIsoMediumMuons_eta","Muons passing Isolation: #eta; #eta; muons", 50, -2.5, 2.5);
  histIsoMediumMuons_phi   = MUONsubDir.make<TH1F>("histIsoMediumMuons_phi","Muons passing Isolation: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histIsoMediumMuons_pv    = MUONsubDir.make<TH1F>("histIsoMediumMuons_pv","Muons passing Isolation: PV; vertices; muons", 60, 0., 60.);
  histDeltaPhi_mediumMuons =  MUONsubDir.make<TH1F>(" histDeltaPhi_mediumMuons","Muons passing MediumID: #Delta #Phi; #Phi_{1} - #Phi_{2}; muons", 30, 0., 6.3);

  histTightMuons_pt    = MUONsubDir.make<TH1F>("histTightMuons_pt","Muons passing TightID: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histTightMuons_eta   = MUONsubDir.make<TH1F>("histTightMuons_eta","Muons passing TightID: #eta; #eta; muons", 50, -2.5, 2.5);
  histTightMuons_phi   = MUONsubDir.make<TH1F>("histTightMuons_phi","Muons passing TightID: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histTightMuons_pv    = MUONsubDir.make<TH1F>("histTightMuons_pv","Muons passing TightID: PV; vertices; muons", 60, 0., 60.);
  histTightMuons_timingVeto = MUONsubDir.make<TH1F>("histTightMuons_timingVeto","Muons passing TightID: is OOT; bool; muons", 2, 0., 2.);
  histTightMuons_pfiso = MUONsubDir.make<TH1F>("histTightMuons_pfiso","Thight Muons: PFIso04 + #delta #beta ; rel iso; muons", 200, 0., 10.);
  histTightMuons_tbiso = MUONsubDir.make<TH1F>("histTightMuons_tbiso","Thight Muons: TrackerIso03 ; rel iso; muons", 200, 0., 10.);
  histIsoTightMuons_pt    = MUONsubDir.make<TH1F>("histIsoTightMuons_pt","Muons passing Isolation: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histIsoTightMuons_eta   = MUONsubDir.make<TH1F>("histIsoTightMuons_eta","Muons passing Isolation: #eta; #eta; muons", 50, -2.5, 2.5);
  histIsoTightMuons_phi   = MUONsubDir.make<TH1F>("histIsoTightMuons_phi","Muons passing Isolation: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histIsoTightMuons_pv    = MUONsubDir.make<TH1F>("histIsoTightMuons_pv","Muons passing Isolation: PV; vertices; muons", 60, 0., 60.);
  histDeltaPhi_tightMuons =  MUONsubDir.make<TH1F>(" histDeltaPhi_tightMuons","Muons passing TightID: #Delta #Phi; #Phi_{1} - #Phi_{2}; muons", 30, 0., 6.3);

  histMetMuons_pt    = MUONsubDir.make<TH1F>("histMetMuons_pt","Muons of events passing Met cut: p_{T}; p_{T}; events", 1000, 0., 1000.);
  histMetMuons_eta   = MUONsubDir.make<TH1F>("histMetMuons_eta","Muons of events passing Met cut: #eta; #eta; events", 50, -2.5, 2.5);
  histMetMuons_phi   = MUONsubDir.make<TH1F>("histMetMuons_phi","Muons of events passing Met cut: #phi; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  histMetMuons_pv    = MUONsubDir.make<TH1F>("histMetMuons_pv","Muons of events passing Met cut: PV; vertices; events", 60, 0., 60.);

  histCat1Muons_pt    = MUONsubDir.make<TH1F>("histCat1Muons_pt","Muons belonging cat1: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histCat1Muons_eta   = MUONsubDir.make<TH1F>("histCat1Muons_eta","Muons belonging cat1: #eta; #eta; muons", 50, -2.5, 2.5);
  histCat1Muons_phi   = MUONsubDir.make<TH1F>("histCat1Muons_phi","Muons belonging cat1: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histCat1Muons_pv    = MUONsubDir.make<TH1F>("histCat1Muons_pv","Muons belonging cat1: PV; vertices; muons", 60, 0., 60.);

  histCat2Muons_pt    = MUONsubDir.make<TH1F>("histCat2Muons_pt","Muons belonging cat2: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  histCat2Muons_eta   = MUONsubDir.make<TH1F>("histCat2Muons_eta","Muons belonging cat2: #eta; #eta; muons", 50, -2.5, 2.5);
  histCat2Muons_phi   = MUONsubDir.make<TH1F>("histCat2Muons_phi","Muons belonging cat2: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  histCat2Muons_pv    = MUONsubDir.make<TH1F>("histCat2Muons_pv","Muons belonging cat2: PV; vertices; muons", 60, 0., 60.);
  
  TFileDirectory MUONsubDirRW = MUONsubDir.mkdir( "PileUpRw" );
  rwhistAllMuons_pt    = MUONsubDirRW.make<TH1F>("rwhistAllMuons_pt","Preselected(acceptance) muons: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  rwhistAllMuons_eta   = MUONsubDirRW.make<TH1F>("rwhistAllMuons_eta","Preselected(acceptance) muons: #eta; #eta; muons", 50, -2.5, 2.5);
  rwhistAllMuons_phi   = MUONsubDirRW.make<TH1F>("rwhistAllMuons_phi","Preselected(acceptance) muons: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  rwhistAllMuons_pv    = MUONsubDirRW.make<TH1F>("rwhistAllMuons_pv","Preselected(acceptance) muons: PV; vertices; muons", 60, 0., 60.);
  
  rwhistNormChi2    = MUONsubDirRW.make<TH1F>("rwhistNormChi2", "Muons passing acceptance & trigger: #chi^{2}/ndof; #chi^{2}/ndof; muons ", 100, 0., 20.);
  rwhistMatchSt     = MUONsubDirRW.make<TH1F>("rwhistMatchSt", "Muons passing acceptance & trigger: matched stations; # of stations; muons ", 10, 0., 10.);
  rwhistMuValidHits = MUONsubDirRW.make<TH1F>("rwhistMuValidHits", "Muons passing acceptance & trigger: muon valid hits; # of hits; muons ", 60, 0., 60.);
  rwhistDxy         = MUONsubDirRW.make<TH1F>("rwhistDxy", "Muons passing acceptance & trigger: dxy; dxy (cm); muons ", 100, -0.5, 0.5);
  rwhistDz          = MUONsubDirRW.make<TH1F>("rwhistDz", "Muons passing acceptance & trigger: dz; dz (cm); muons ", 100, -0.5, 0.5);
  rwhistPixelHits   = MUONsubDirRW.make<TH1F>("rwhistPixelHits", "Muons passing acceptance & trigger: pixel hits; # of hits; muons ", 20, 0., 20.);
  rwhistTrackerL    = MUONsubDirRW.make<TH1F>("rwhistTrackerL", "Muons passing acceptance & trigger: tracker layers; # of layers; muons ", 30, 0., 30.);
  rwhistDeltaPtOverPt = MUONsubDirRW.make<TH1F>("rwhistDeltaPtOverPt", "Muons passing acceptance & trigger: relative p_{T} error; #Delta p_{T}/p_{T}; muons ", 100, 0., 1.);

  rwhistPt_num    = MUONsubDirRW.make<TH1F>("rwhistPt_num","Muons passing tightID & Iso: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  rwhistEta_num   = MUONsubDirRW.make<TH1F>("rwhistEta_num","Muons passing tightID & Iso: #eta; #eta; muons", 50, -2.5, 2.5);
  rwhistPhi_num   = MUONsubDirRW.make<TH1F>("rwhistPhi_num","Muons passing tightID & Iso: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  rwhistPV_num    = MUONsubDirRW.make<TH1F>("rwhistPV_num","Muons passing tightID & Iso: PV; vertices; muons", 60, 0., 60.);
  rwhistNormChi2_num    = MUONsubDirRW.make<TH1F>("rwhistNormChi2_num", "Muons passing tightID & Iso: #chi^{2}/ndof; #chi^{2}/ndof; muons ", 100, 0., 20.);
  rwhistMatchSt_num     = MUONsubDirRW.make<TH1F>("rwhistMatchSt_num", "Muons passing tightID & Iso: matched stations; # of stations; muons ", 10, 0., 10.);
  rwhistMuValidHits_num = MUONsubDirRW.make<TH1F>("rwhistMuValidHits_num", "Muons passing tightID & Iso: muon valid hits; # of hits; muons ", 60, 0., 60.);
  rwhistDxy_num         = MUONsubDirRW.make<TH1F>("rwhistDxy_num", "Muons passing tightID & Iso: dxy; dxy (cm); muons ", 100, -0.5, 0.5);
  rwhistDz_num          = MUONsubDirRW.make<TH1F>("rwhistDz_num", "Muons passing tightID & Iso: dz; dz (cm); muons ", 100, -0.5, 0.5);
  rwhistPixelHits_num   = MUONsubDirRW.make<TH1F>("rwhistPixelHits_num", "Muons passing tightID & Iso: pixel hits; # of hits; muons ", 20, 0., 20.);
  rwhistTrackerL_num    = MUONsubDirRW.make<TH1F>("rwhistTrackerL_num", "Muons passing tightID & Iso: tracker layers; # of layers; muons ", 30, 0., 30.);
  rwhistDeltaPtOverPt_num = MUONsubDirRW.make<TH1F>("rwhistDeltaPtOverPt_num", "Muons passing tightID & Iso: relative p_{T} error; #Delta p_{T}/p_{T}; muons ", 100, 0., 1.);

  rwhistMediumMuons_pt    = MUONsubDirRW.make<TH1F>("rwhistMediumMuons_pt","Muons passing MediumID: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  rwhistMediumMuons_eta   = MUONsubDirRW.make<TH1F>("rwhistMediumMuons_eta","Muons passing MediumID: #eta; #eta; muons", 50, -2.5, 2.5);
  rwhistMediumMuons_phi   = MUONsubDirRW.make<TH1F>("rwhistMediumMuons_phi","Muons passing MediumID: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  rwhistMediumMuons_pv    = MUONsubDirRW.make<TH1F>("rwhistMediumMuons_pv","Muons passing MediumID: PV; vertices; muons", 60, 0., 60.);
  rwhistMediumMuons_pfiso = MUONsubDirRW.make<TH1F>("rwhistMediumMuons_pfiso","Thight Muons: PFIso04 + #delta #beta ; rel iso; muons", 200, 0., 10.);
  rwhistMediumMuons_tbiso = MUONsubDirRW.make<TH1F>("rwhistMediumMuons_tbiso","Thight Muons: TrackerIso03 ; rel iso; muons", 200, 0., 10.);
  rwhistIsoMediumMuons_pt    = MUONsubDirRW.make<TH1F>("rwhistIsoMediumMuons_pt","Muons passing Isolation: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  rwhistIsoMediumMuons_eta   = MUONsubDirRW.make<TH1F>("rwhistIsoMediumMuons_eta","Muons passing Isolation: #eta; #eta; muons", 50, -2.5, 2.5);
  rwhistIsoMediumMuons_phi   = MUONsubDirRW.make<TH1F>("rwhistIsoMediumMuons_phi","Muons passing Isolation: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  rwhistIsoMediumMuons_pv    = MUONsubDirRW.make<TH1F>("rwhistIsoMediumMuons_pv","Muons passing Isolation: PV; vertices; muons", 60, 0., 60.);
  
  rwhistTightMuons_pt    = MUONsubDirRW.make<TH1F>("rwhistTightMuons_pt","Muons passing TightID: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  rwhistTightMuons_eta   = MUONsubDirRW.make<TH1F>("rwhistTightMuons_eta","Muons passing TightID: #eta; #eta; muons", 50, -2.5, 2.5);
  rwhistTightMuons_phi   = MUONsubDirRW.make<TH1F>("rwhistTightMuons_phi","Muons passing TightID: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  rwhistTightMuons_pv    = MUONsubDirRW.make<TH1F>("rwhistTightMuons_pv","Muons passing TightID: PV; vertices; muons", 60, 0., 60.);
  rwhistTightMuons_pfiso = MUONsubDirRW.make<TH1F>("rwhistTightMuons_pfiso","Thight Muons: PFIso04 + #delta #beta ; rel iso; muons", 200, 0., 10.);
  rwhistTightMuons_tbiso = MUONsubDirRW.make<TH1F>("rwhistTightMuons_tbiso","Thight Muons: TrackerIso03 ; rel iso; muons", 200, 0., 10.);
  rwhistIsoTightMuons_pt    = MUONsubDirRW.make<TH1F>("rwhistIsoTightMuons_pt","Muons passing Isolation: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  rwhistIsoTightMuons_eta   = MUONsubDirRW.make<TH1F>("rwhistIsoTightMuons_eta","Muons passing Isolation: #eta; #eta; muons", 50, -2.5, 2.5);
  rwhistIsoTightMuons_phi   = MUONsubDirRW.make<TH1F>("rwhistIsoTightMuons_phi","Muons passing Isolation: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  rwhistIsoTightMuons_pv    = MUONsubDirRW.make<TH1F>("rwhistIsoTightMuons_pv","Muons passing Isolation: PV; vertices; muons", 60, 0., 60.);

  rwhistMetMuons_pt    = MUONsubDirRW.make<TH1F>("rwhistMetMuons_pt","Muons of events passing Met cut: p_{T}; p_{T}; events", 1000, 0., 1000.);
  rwhistMetMuons_eta   = MUONsubDirRW.make<TH1F>("rwhistMetMuons_eta","Muons of events passing Met cut: #eta; #eta; events", 50, -2.5, 2.5);
  rwhistMetMuons_phi   = MUONsubDirRW.make<TH1F>("rwhistMetMuons_phi","Muons of events passing Met cut: #phi; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  rwhistMetMuons_pv    = MUONsubDirRW.make<TH1F>("rwhistMetMuons_pv","Muons of events passing Met cut: PV; vertices; events", 60, 0., 60.);

  rwhistCat1Muons_pt    = MUONsubDirRW.make<TH1F>("rwhistCat1Muons_pt","Muons belonging cat1: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  rwhistCat1Muons_eta   = MUONsubDirRW.make<TH1F>("rwhistCat1Muons_eta","Muons belonging cat1: #eta; #eta; muons", 50, -2.5, 2.5);
  rwhistCat1Muons_phi   = MUONsubDirRW.make<TH1F>("rwhistCat1Muons_phi","Muons belonging cat1: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  rwhistCat1Muons_pv    = MUONsubDirRW.make<TH1F>("rwhistCat1Muons_pv","Muons belonging cat1: PV; vertices; muons", 60, 0., 60.);

  rwhistCat2Muons_pt    = MUONsubDirRW.make<TH1F>("rwhistCat2Muons_pt","Muons belonging cat2: p_{T}; p_{T}; muons", 1000, 0., 1000.);
  rwhistCat2Muons_eta   = MUONsubDirRW.make<TH1F>("rwhistCat2Muons_eta","Muons belonging cat2: #eta; #eta; muons", 50, -2.5, 2.5);
  rwhistCat2Muons_phi   = MUONsubDirRW.make<TH1F>("rwhistCat2Muons_phi","Muons belonging cat2: #phi; #phi; muons", 50, -TMath::Pi(), TMath::Pi());
  rwhistCat2Muons_pv    = MUONsubDirRW.make<TH1F>("rwhistCat2Muons_pv","Muons belonging cat2: PV; vertices; muons", 60, 0., 60.);
 

  TFileDirectory METsubDir = fs->mkdir( "Met" );
  histPFMet_ptRaw    = METsubDir.make<TH1F>("histPFMet_ptRaw","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  histPFMet_pt       = METsubDir.make<TH1F>("histPFMet_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  histPFMet_phi      = METsubDir.make<TH1F>("histPFMet_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // histPFMet_sumet    = METsubDir.make<TH1F>("histPFMet_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // histPFMet_type01smear_pt = METsubDir.make<TH1F>("histPFMet_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
  histPFMet_totUp_pt = METsubDir.make<TH1F>("histPFMet_totUp_pt","Met + totUncUp*Met of events passing TightID & Iso; p_{T} (GeV); events", 1000, 0., 1000.);
  histPFMet_totDown_pt = METsubDir.make<TH1F>("histPFMet_totDown_pt","Met - totUncDown*Met of events passing TightID & Iso; p_{T} (GeV); events", 1000, 0., 1000.);
  
  // histRawMet_pt       = METsubDir.make<TH1F>("histRawMet_pt","RawMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  // histRawMet_phi      = METsubDir.make<TH1F>("histRawMet_phi","RawMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // histRawMet_sumet    = METsubDir.make<TH1F>("histRawMet_sumet","Sum of Raw Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);

  histPFMet_70toInf_pt       = METsubDir.make<TH1F>("histPFMet_70toInf_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  histPFMet_70toInf_phi      = METsubDir.make<TH1F>("histPFMet_70toInf_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // histPFMet_70toInf_sumet    = METsubDir.make<TH1F>("histPFMet_70toInf_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // histPFMet_70toInf_type01smear_pt = METsubDir.make<TH1F>("histPFMet_70toInf_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
  histPFMet_70toInf_totUp_pt = METsubDir.make<TH1F>("histPFMet_70toInf_totUp_pt","Met + totUncUp*Met of events passing TightID & Iso; p_{T} (GeV); events", 1000, 0., 1000.);
  histPFMet_70toInf_totDown_pt = METsubDir.make<TH1F>("histPFMet_70toInf_totDown_pt","Met - totUncDown*Met of events passing TightID & Iso; p_{T} (GeV); events", 1000, 0., 1000.);
  
  histPFMet_70to110_pt       = METsubDir.make<TH1F>("histPFMet_70to110_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  // histPFMet_70to110_phi      = METsubDir.make<TH1F>("histPFMet_70to110_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // histPFMet_70to110_sumet    = METsubDir.make<TH1F>("histPFMet_70to110_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // histPFMet_70to110_type01smear_pt = METsubDir.make<TH1F>("histPFMet_70to110_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
  // histPFMet_70to110_totUp_pt = METsubDir.make<TH1F>("histPFMet_70to110_totUp_pt","Met + totUncUp*Met of events passing TightID & Iso; p_{T} (GeV); events", 1000, 0., 1000.);
  // histPFMet_70to110_totDown_pt = METsubDir.make<TH1F>("histPFMet_70to110_totDown_pt","Met - totUncDown*Met of events passing TightID & Iso; p_{T} (GeV); events", 1000, 0., 1000.);
  
  histPFMet_110to150_pt       = METsubDir.make<TH1F>("histPFMet_110to150_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  // histPFMet_110to150_phi      = METsubDir.make<TH1F>("histPFMet_110to150_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // histPFMet_110to150_sumet    = METsubDir.make<TH1F>("histPFMet_110to150_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // histPFMet_110to150_type01smear_pt = METsubDir.make<TH1F>("histPFMet_110to150_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
  // histPFMet_110to150_totUp_pt = METsubDir.make<TH1F>("histPFMet_110to150_totUp_pt","Met + totUncUp*Met of events passing TightID & Iso; p_{T} (GeV); events", 1000, 0., 1000.);
  // histPFMet_110to150_totDown_pt = METsubDir.make<TH1F>("histPFMet_110to150_totDown_pt","Met - totUncDown*Met of events passing TightID & Iso; p_{T} (GeV); events", 1000, 0., 1000.);
  
  // histPFMet_150toInf_pt       = METsubDir.make<TH1F>("histPFMet_150toInf_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  // histPFMet_150toInf_phi      = METsubDir.make<TH1F>("histPFMet_150toInf_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // histPFMet_150toInf_sumet    = METsubDir.make<TH1F>("histPFMet_150toInf_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // histPFMet_150toInf_type01smear_pt = METsubDir.make<TH1F>("histPFMet_150toInf_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
  // histPFMet_150toInf_totUp_pt = METsubDir.make<TH1F>("histPFMet_150toInf_totUp_pt","Met + totUncUp*Met of events passing TightID & Iso; p_{T} (GeV); events", 1000, 0., 1000.);
  // histPFMet_150toInf_totDown_pt = METsubDir.make<TH1F>("histPFMet_150toInf_totDown_pt","Met - totUncDown*Met of events passing TightID & Iso; p_{T} (GeV); events", 1000, 0., 1000.);
 
  TFileDirectory METsubDirRW = METsubDir.mkdir( "PileUpRw" );
  rwhistPFMet_pt       = METsubDirRW.make<TH1F>("rwhistPFMet_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  rwhistPFMet_phi      = METsubDirRW.make<TH1F>("rwhistPFMet_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // rwhistPFMet_sumet    = METsubDirRW.make<TH1F>("rwhistPFMet_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // rwhistPFMet_type01smear_pt = METsubDirRW.make<TH1F>("rwhistPFMet_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
  
  rwhistPFMet_70toInf_pt       = METsubDirRW.make<TH1F>("rwhistPFMet_70toInf_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  rwhistPFMet_70toInf_phi      = METsubDirRW.make<TH1F>("rwhistPFMet_70toInf_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // rwhistPFMet_70toInf_sumet    = METsubDirRW.make<TH1F>("rwhistPFMet_70toInf_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // rwhistPFMet_70toInf_type01smear_pt = METsubDirRW.make<TH1F>("rwhistPFMet_70toInf_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
 
  rwhistPFMet_70to110_pt       = METsubDirRW.make<TH1F>("rwhistPFMet_70to110_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  // rwhistPFMet_70to110_phi      = METsubDirRW.make<TH1F>("rwhistPFMet_70to110_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // rwhistPFMet_70to110_sumet    = METsubDirRW.make<TH1F>("rwhistPFMet_70to110_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // rwhistPFMet_70to110_type01smear_pt = METsubDirRW.make<TH1F>("rwhistPFMet_70to110_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
 
  rwhistPFMet_110to150_pt       = METsubDirRW.make<TH1F>("rwhistPFMet_110to150_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  // rwhistPFMet_110to150_phi      = METsubDirRW.make<TH1F>("rwhistPFMet_110to150_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // rwhistPFMet_110to150_sumet    = METsubDirRW.make<TH1F>("rwhistPFMet_110to150_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // rwhistPFMet_110to150_type01smear_pt = METsubDirRW.make<TH1F>("rwhistPFMet_110to150_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
   
  TFileDirectory METsubDirRWUP = METsubDir.mkdir( "PileUpRwUp" );
  rwuphistPFMet_pt       = METsubDirRWUP.make<TH1F>("rwuphistPFMet_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  rwuphistPFMet_phi      = METsubDirRWUP.make<TH1F>("rwuphistPFMet_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // rwuphistPFMet_sumet    = METsubDirRWUP.make<TH1F>("rwuphistPFMet_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // rwuphistPFMet_type01smear_pt = METsubDirRWUP.make<TH1F>("rwuphistPFMet_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
 
  rwuphistPFMet_70toInf_pt       = METsubDirRWUP.make<TH1F>("rwuphistPFMet_70toInf_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  rwuphistPFMet_70toInf_phi      = METsubDirRWUP.make<TH1F>("rwuphistPFMet_70toInf_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // rwuphistPFMet_70toInf_sumet    = METsubDirRWUP.make<TH1F>("rwuphistPFMet_70toInf_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // rwuphistPFMet_70toInf_type01smear_pt = METsubDirRWUP.make<TH1F>("rwuphistPFMet_70toInf_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
  
  rwuphistPFMet_70to110_pt       = METsubDirRWUP.make<TH1F>("rwuphistPFMet_70to110_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  // rwuphistPFMet_70to110_phi      = METsubDirRWUP.make<TH1F>("rwuphistPFMet_70to110_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // rwuphistPFMet_70to110_sumet    = METsubDirRWUP.make<TH1F>("rwuphistPFMet_70to110_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // rwuphistPFMet_70to110_type01smear_pt = METsubDirRWUP.make<TH1F>("rwuphistPFMet_70to110_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
  
  rwuphistPFMet_110to150_pt       = METsubDirRWUP.make<TH1F>("rwuphistPFMet_110to150_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  // rwuphistPFMet_110to150_phi      = METsubDirRWUP.make<TH1F>("rwuphistPFMet_110to150_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // rwuphistPFMet_110to150_sumet    = METsubDirRWUP.make<TH1F>("rwuphistPFMet_110to150_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // rwuphistPFMet_110to150_type01smear_pt = METsubDirRWUP.make<TH1F>("rwuphistPFMet_110to150_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
   
  TFileDirectory METsubDirRWDOWN = METsubDir.mkdir( "PileUpRwDown" );
  rwdownhistPFMet_pt       = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  rwdownhistPFMet_phi      = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // rwdownhistPFMet_sumet    = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // rwdownhistPFMet_type01smear_pt = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
 
  rwdownhistPFMet_70toInf_pt       = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_70toInf_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  rwdownhistPFMet_70toInf_phi      = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_70toInf_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // rwdownhistPFMet_70toInf_sumet    = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_70toInf_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // rwdownhistPFMet_70toInf_type01smear_pt = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_70toInf_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
  
  rwdownhistPFMet_70to110_pt       = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_70to110_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  // rwdownhistPFMet_70to110_phi      = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_70to110_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // rwdownhistPFMet_70to110_sumet    = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_70to110_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // rwdownhistPFMet_70to110_type01smear_pt = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_70to110_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
  
  rwdownhistPFMet_110to150_pt       = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_110to150_pt","PFMet of events passing TightID & Iso; p_{T}; events", 1000, 0., 1000.);
  // rwdownhistPFMet_110to150_phi      = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_110to150_phi","PFMet of events passing TightID & Iso; #phi; events", 50, -TMath::Pi(), TMath::Pi());
  // rwdownhistPFMet_110to150_sumet    = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_110to150_sumet","Sum of Et of events passing TightID & Iso; E_{T}; events", 5000, 0., 5000.);
  // rwdownhistPFMet_110to150_type01smear_pt = METsubDirRWDOWN.make<TH1F>("rwdownhistPFMet_110to150_type01smear_pt","Met + Type01Smear of events passing TightID & Iso; p_{T} (GeV) ; events", 1000, 0., 1000.);
 

  TFileDirectory JETsubDir = fs->mkdir( "Jet" );

  histTrue_bJets = JETsubDir.make<TH1F>("histTrue_bJets", "True bjets", 30, 0.,30.);


  histPFJets_bDisc_pt = JETsubDir.make<TH2F>("histPFJets_bDisc_pt","b Discriminator vs p_{T} of PFJets of events passing TightID, Iso, Met but not the Jet cut; p_{T} (GeV); bDisc", 500, 0., 500.,100, 0.,1. );
  //histPFJets_bDisc_eta = JETsubDir.make<TH2F>("histPFJets_bDisc_eta","b Discriminator vs #eta of PFJets of events passing TightID, Iso, Met but not the Jet cut; #eta (GeV); bDisc", 50, -2.5, 2.5,100, 0.,1. );
  histPFJets_bDisc    = JETsubDir.make<TH1F>("histPFJets_bDisc","b Discriminator of PFJets of events passing TightID, Iso, Met cuts; bDisc; jets", 100, 0., 1.);
  histPFJets_pt       = JETsubDir.make<TH1F>("histPFJets_pt","PFJets of events passing TightID, Iso & Met cuts; p_{T}; jets", 1000, 0., 1000.);
  histPFJets_eta      = JETsubDir.make<TH1F>("histPFJets_eta","PFJets of events passing TightID, Iso & Met cuts; #eta; jets", 200, -5., 5.);
  histPFJets_phi      = JETsubDir.make<TH1F>("histPFJets_phi","PFJets of events passing TightID, Iso & Met cuts; #phi; jets", 50, -TMath::Pi(), TMath::Pi());
  histPFJets_deltaPhi = JETsubDir.make<TH1F>("histPFJets_deltaPhi","PFJets of events passing TightID, Iso & Met cuts; #Phi_{Dimuon} - #Phi_{Jet}; jets", 200, -4*TMath::Pi(), 4*TMath::Pi());
  histPFJets_size     = JETsubDir.make<TH1F>("histPFJets_size","Size of PFJets of events passing TightID, Iso, Met & Jet cuts; size; muons", 10, 0., 10.);
  histCuttedJets_bDisc    = JETsubDir.make<TH1F>("histCuttedJets_bDisc","b Discriminator of PFJets of events passing TightID, Iso, Met and Jet cuts; bDisc; jets", 100, 0., 1. );
  histCuttedJets_pt       = JETsubDir.make<TH1F>("histCuttedJets_pt","PFJets of events passing TightID, Iso, Met and Jetcuts; p_{T}; jets", 1000, 0., 1000.);
  histCuttedJets_eta      = JETsubDir.make<TH1F>("histCuttedJets_eta","PFJets of events passing TightID, Iso, Met and Jet cuts; #eta; jets", 50, -2.5, 2.5);
  histCuttedJets_phi      = JETsubDir.make<TH1F>("histCuttedJets_phi","PFJets of events passing TightID, Iso, Met and Jet cuts; #phi; jets", 50, -TMath::Pi(), TMath::Pi());
  histBJets_size     = JETsubDir.make<TH1F>("histBJets_size","Size of BJets of events passing TightID, Iso, Met & Jet cuts; size; muons", 10, 0., 10.);
  histBJets_pt       = JETsubDir.make<TH1F>("histBJets_pt","BJets of events passing TightID, Iso, Met & Jet cuts; p_{T}; jets", 1000, 0., 1000.);
  histBJets_eta      = JETsubDir.make<TH1F>("histBJets_eta","BJets of events passing TightID, Iso, Met & Jet cuts; #eta; jets", 50, -2.5, 2.5);
  histBJets_phi      = JETsubDir.make<TH1F>("histBJets_phi","BJets of events passing TightID, Iso, Met & Jet cuts; #phi; jets", 50, -TMath::Pi(), TMath::Pi());
  histBJets_deltaPhi = JETsubDir.make<TH1F>("histBJets_deltaPhi","BJets of events passing TightID, Iso, Met & Jet cuts; #Phi_{Dimuon} - #Phi_{Jet}; jets", 200, -4*TMath::Pi(), 4*TMath::Pi());
  histbtagPFJets_pt   = JETsubDir.make<TH1F>("histbtagPFJets_pt","PFJets of events passing TightID, Iso, Met & bdisc cuts; p_{T}; jets", 1000, 0., 1000.);

  histLightJets_size     = JETsubDir.make<TH1F>("histLightJets_size","Size of light Jets of events passing TightID, Iso, Met & Jet cuts; size; muons", 10, 0., 10.);
  histLightJets_pt       = JETsubDir.make<TH1F>("histLightJets_pt","LightJets of events passing TightID, Iso, Met & Jet cuts; p_{T}; jets", 1000, 0., 1000.);
  histLightJets_eta      = JETsubDir.make<TH1F>("histLightJets_eta","LightJets of events passing TightID, Iso, Met & Jet cuts; #eta; jets", 50, -2.5, 2.5);
  histLightJets_phi      = JETsubDir.make<TH1F>("histLightJets_phi","LightJets of events passing TightID, Iso, Met & Jet cuts; #phi; jets", 50, -TMath::Pi(), TMath::Pi());
  histLightJets_deltaPhi = JETsubDir.make<TH1F>("histLightJets_deltaPhi","LightJets of events passing TightID, Iso, Met & Jet cuts; #Phi_{Dimuon} - #Phi_{Jet}; jets", 200, -4*TMath::Pi(), 4*TMath::Pi());

  TFileDirectory JETsubDirRW = JETsubDir.mkdir( "PileUpRw" );
  rwhistPFJets_bDisc    = JETsubDir.make<TH1F>("rwhistPFJets_bDisc","b Discriminator of PFJets of events passing TightID, Iso, Met cuts; bDisc; jets", 100, 0., 1.);
  rwhistPFJets_pt       = JETsubDir.make<TH1F>("rwhistPFJets_pt","PFJets of events passing TightID, Iso & Met cuts; p_{T}; jets", 1000, 0., 1000.);
  rwhistPFJets_eta      = JETsubDir.make<TH1F>("rwhistPFJets_eta","PFJets of events passing TightID, Iso & Met cuts; #eta; jets", 200, -5., 5.);
  rwhistPFJets_phi      = JETsubDir.make<TH1F>("rwhistPFJets_phi","PFJets of events passing TightID, Iso & Met cuts; #phi; jets", 50, -TMath::Pi(), TMath::Pi());
  rwhistPFJets_deltaPhi = JETsubDir.make<TH1F>("rwhistPFJets_deltaPhi","PFJets of events passing TightID, Iso & Met cuts; #Phi_{Dimuon} - #Phi_{Jet}; jets", 200, -4*TMath::Pi(), 4*TMath::Pi());
  rwhistPFJets_size     = JETsubDir.make<TH1F>("rwhistPFJets_size","Size of PFJets of events passing TightID, Iso, Met & Jet cuts; size; muons", 10, 0., 10.);
  rwhistCuttedJets_bDisc    = JETsubDir.make<TH1F>("rwhistCuttedJets_bDisc","b Discriminator of PFJets of events passing TightID, Iso, Met and Jet cuts; bDisc; jets", 100, 0., 1. );
  rwhistCuttedJets_pt       = JETsubDir.make<TH1F>("rwhistCuttedJets_pt","PFJets of events passing TightID, Iso, Met and Jetcuts; p_{T}; jets", 1000, 0., 1000.);
  rwhistCuttedJets_eta      = JETsubDir.make<TH1F>("rwhistCuttedJets_eta","PFJets of events passing TightID, Iso, Met and Jet cuts; #eta; jets", 50, -2.5, 2.5);
  rwhistCuttedJets_phi      = JETsubDir.make<TH1F>("rwhistCuttedJets_phi","PFJets of events passing TightID, Iso, Met and Jet cuts; #phi; jets", 50, -TMath::Pi(), TMath::Pi());
  rwhistBJets_size     = JETsubDir.make<TH1F>("rwhistBJets_size","Size of BJets of events passing TightID, Iso, Met & Jet cuts; size; muons", 10, 0., 10.);
  rwhistBJets_pt       = JETsubDir.make<TH1F>("rwhistBJets_pt","BJets of events passing TightID, Iso, Met & Jet cuts; p_{T}; jets", 1000, 0., 1000.);
  rwhistBJets_eta      = JETsubDir.make<TH1F>("rwhistBJets_eta","BJets of events passing TightID, Iso, Met & Jet cuts; #eta; jets", 50, -2.5, 2.5);
  rwhistBJets_phi      = JETsubDir.make<TH1F>("rwhistBJets_phi","BJets of events passing TightID, Iso, Met & Jet cuts; #phi; jets", 50, -TMath::Pi(), TMath::Pi());
  rwhistBJets_deltaPhi = JETsubDir.make<TH1F>("rwhistBJets_deltaPhi","BJets of events passing TightID, Iso, Met & Jet cuts; #Phi_{Dimuon} - #Phi_{Jet}; jets", 200, -4*TMath::Pi(), 4*TMath::Pi());
  rwhistbtagPFJets_pt   = JETsubDir.make<TH1F>("rwhistbtagPFJets_pt","PFJets of events passing TightID, Iso, Met & bdisc cuts; p_{T}; jets", 1000, 0., 1000.);

  rwhistLightJets_size     = JETsubDir.make<TH1F>("rwhistLightJets_size","Size of light Jets of events passing TightID, Iso, Met & Jet cuts; size; muons", 10, 0., 10.);
  rwhistLightJets_pt       = JETsubDir.make<TH1F>("rwhistLightJets_pt","LightJets of events passing TightID, Iso, Met & Jet cuts; p_{T}; jets", 1000, 0., 1000.);
  rwhistLightJets_eta      = JETsubDir.make<TH1F>("rwhistLightJets_eta","LightJets of events passing TightID, Iso, Met & Jet cuts; #eta; jets", 50, -2.5, 2.5);
  rwhistLightJets_phi      = JETsubDir.make<TH1F>("rwhistLightJets_phi","LightJets of events passing TightID, Iso, Met & Jet cuts; #phi; jets", 50, -TMath::Pi(), TMath::Pi());
  rwhistLightJets_deltaPhi = JETsubDir.make<TH1F>("rwhistLightJets_deltaPhi","LightJets of events passing TightID, Iso, Met & Jet cuts; #Phi_{Dimuon} - #Phi_{Jet}; jets", 200, -4*TMath::Pi(), 4*TMath::Pi());


  TFileDirectory JETsubDirRWUP = JETsubDir.mkdir( "PileUpRwUp" );
  rwuphistPFJets_bDisc    = JETsubDir.make<TH1F>("rwuphistPFJets_bDisc","b Discriminator of PFJets of events passing TightID, Iso, Met cuts; bDisc; jets", 100, 0., 1.);
  rwuphistPFJets_pt       = JETsubDir.make<TH1F>("rwuphistPFJets_pt","PFJets of events passing TightID, Iso & Met cuts; p_{T}; jets", 1000, 0., 1000.);
  rwuphistPFJets_eta      = JETsubDir.make<TH1F>("rwuphistPFJets_eta","PFJets of events passing TightID, Iso & Met cuts; #eta; jets", 200, -5., 5.);
  rwuphistPFJets_phi      = JETsubDir.make<TH1F>("rwuphistPFJets_phi","PFJets of events passing TightID, Iso & Met cuts; #phi; jets", 50, -TMath::Pi(), TMath::Pi());
  rwuphistPFJets_deltaPhi = JETsubDir.make<TH1F>("rwuphistPFJets_deltaPhi","PFJets of events passing TightID, Iso & Met cuts; #Phi_{Dimuon} - #Phi_{Jet}; jets", 200, -4*TMath::Pi(), 4*TMath::Pi());
  rwuphistPFJets_size     = JETsubDir.make<TH1F>("rwuphistPFJets_size","Size of PFJets of events passing TightID, Iso, Met & Jet cuts; size; muons", 10, 0., 10.);
  rwuphistCuttedJets_bDisc    = JETsubDir.make<TH1F>("rwuphistCuttedJets_bDisc","b Discriminator of PFJets of events passing TightID, Iso, Met and Jet cuts; bDisc; jets", 100, 0., 1. );
  rwuphistCuttedJets_pt       = JETsubDir.make<TH1F>("rwuphistCuttedJets_pt","PFJets of events passing TightID, Iso, Met and Jetcuts; p_{T}; jets", 1000, 0., 1000.);
  rwuphistCuttedJets_eta      = JETsubDir.make<TH1F>("rwuphistCuttedJets_eta","PFJets of events passing TightID, Iso, Met and Jet cuts; #eta; jets", 50, -2.5, 2.5);
  rwuphistCuttedJets_phi      = JETsubDir.make<TH1F>("rwuphistCuttedJets_phi","PFJets of events passing TightID, Iso, Met and Jet cuts; #phi; jets", 50, -TMath::Pi(), TMath::Pi());
  rwuphistBJets_size     = JETsubDir.make<TH1F>("rwuphistBJets_size","Size of BJets of events passing TightID, Iso, Met & Jet cuts; size; muons", 10, 0., 10.);
  rwuphistBJets_pt       = JETsubDir.make<TH1F>("rwuphistBJets_pt","BJets of events passing TightID, Iso, Met & Jet cuts; p_{T}; jets", 1000, 0., 1000.);
  rwuphistBJets_eta      = JETsubDir.make<TH1F>("rwuphistBJets_eta","BJets of events passing TightID, Iso, Met & Jet cuts; #eta; jets", 50, -2.5, 2.5);
  rwuphistBJets_phi      = JETsubDir.make<TH1F>("rwuphistBJets_phi","BJets of events passing TightID, Iso, Met & Jet cuts; #phi; jets", 50, -TMath::Pi(), TMath::Pi());
  rwuphistBJets_deltaPhi = JETsubDir.make<TH1F>("rwuphistBJets_deltaPhi","BJets of events passing TightID, Iso, Met & Jet cuts; #Phi_{Dimuon} - #Phi_{Jet}; jets", 200, -4*TMath::Pi(), 4*TMath::Pi());
  rwuphistbtagPFJets_pt   = JETsubDir.make<TH1F>("rwuphistbtagPFJets_pt","PFJets of events passing TightID, Iso, Met & bdisc cuts; p_{T}; jets", 1000, 0., 1000.);

  TFileDirectory JETsubDirRWDOWN = JETsubDir.mkdir( "PileUpRwDown" );
  rwdownhistPFJets_bDisc    = JETsubDir.make<TH1F>("rwdownhistPFJets_bDisc","b Discriminator of PFJets of events passing TightID, Iso, Met cuts; bDisc; jets", 100, 0., 1.);
  rwdownhistPFJets_pt       = JETsubDir.make<TH1F>("rwdownhistPFJets_pt","PFJets of events passing TightID, Iso & Met cuts; p_{T}; jets", 1000, 0., 1000.);
  rwdownhistPFJets_eta      = JETsubDir.make<TH1F>("rwdownhistPFJets_eta","PFJets of events passing TightID, Iso & Met cuts; #eta; jets", 200, -5., 5.);
  rwdownhistPFJets_phi      = JETsubDir.make<TH1F>("rwdownhistPFJets_phi","PFJets of events passing TightID, Iso & Met cuts; #phi; jets", 50, -TMath::Pi(), TMath::Pi());
  rwdownhistPFJets_deltaPhi = JETsubDir.make<TH1F>("rwdownhistPFJets_deltaPhi","PFJets of events passing TightID, Iso & Met cuts; #Phi_{Dimuon} - #Phi_{Jet}; jets", 200, -4*TMath::Pi(), 4*TMath::Pi());
  rwdownhistPFJets_size     = JETsubDir.make<TH1F>("rwdownhistPFJets_size","Size of PFJets of events passing TightID, Iso, Met & Jet cuts; size; muons", 10, 0., 10.);
  rwdownhistCuttedJets_bDisc    = JETsubDir.make<TH1F>("rwdownhistCuttedJets_bDisc","b Discriminator of PFJets of events passing TightID, Iso, Met and Jet cuts; bDisc; jets", 100, 0., 1. );
  rwdownhistCuttedJets_pt       = JETsubDir.make<TH1F>("rwdownhistCuttedJets_pt","PFJets of events passing TightID, Iso, Met and Jetcuts; p_{T}; jets", 1000, 0., 1000.);
  rwdownhistCuttedJets_eta      = JETsubDir.make<TH1F>("rwdownhistCuttedJets_eta","PFJets of events passing TightID, Iso, Met and Jet cuts; #eta; jets", 50, -2.5, 2.5);
  rwdownhistCuttedJets_phi      = JETsubDir.make<TH1F>("rwdownhistCuttedJets_phi","PFJets of events passing TightID, Iso, Met and Jet cuts; #phi; jets", 50, -TMath::Pi(), TMath::Pi());
  rwdownhistBJets_size     = JETsubDir.make<TH1F>("rwdownhistBJets_size","Size of BJets of events passing TightID, Iso, Met & Jet cuts; size; muons", 10, 0., 10.);
  rwdownhistBJets_pt       = JETsubDir.make<TH1F>("rwdownhistBJets_pt","BJets of events passing TightID, Iso, Met & Jet cuts; p_{T}; jets", 1000, 0., 1000.);
  rwdownhistBJets_eta      = JETsubDir.make<TH1F>("rwdownhistBJets_eta","BJets of events passing TightID, Iso, Met & Jet cuts; #eta; jets", 50, -2.5, 2.5);
  rwdownhistBJets_phi      = JETsubDir.make<TH1F>("rwdownhistBJets_phi","BJets of events passing TightID, Iso, Met & Jet cuts; #phi; jets", 50, -TMath::Pi(), TMath::Pi());
  rwdownhistBJets_deltaPhi = JETsubDir.make<TH1F>("rwdownhistBJets_deltaPhi","BJets of events passing TightID, Iso, Met & Jet cuts; #Phi_{Dimuon} - #Phi_{Jet}; jets", 200, -4*TMath::Pi(), 4*TMath::Pi());
  rwdownhistbtagPFJets_pt   = JETsubDir.make<TH1F>("rwdownhistbtagPFJets_pt","PFJets of events passing TightID, Iso, Met & bdisc cuts; p_{T}; jets", 1000, 0., 1000.);


  TFileDirectory DIMUONsubDir = fs->mkdir( "Dimuon" );
  histMediumDimuon_mass     = DIMUONsubDir.make<TH1F>("histMediumDimuon_mass","Dimuon mass of events passing MediumID; mass (GeV/c^{2}); Events", 1000, 0., 1000.); 
  histMediumDimuon_pt        = DIMUONsubDir.make<TH1F>("histMediumDimuon_pt", "Dimuon p_{T} of events passing MediumID; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histMediumDimuon_phi       = DIMUONsubDir.make<TH1F>("histMediumDimuon_phi", "Dimuon #phi of events passing MediumID; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histMediumDimuon_eta       = DIMUONsubDir.make<TH1F>("histMediumDimuon_eta", "Dimuon #eta of events passing MediumID; #eta; Events", 50, -2.5, 2.5);

  histTightDimuon_mass     = DIMUONsubDir.make<TH1F>("histTightDimuon_mass","Dimuon mass of events passing TightID; mass (GeV/c^{2}); Events", 1000, 0., 1000.); 
  histTightDimuon_pt        = DIMUONsubDir.make<TH1F>("histTightDimuon_pt", "Dimuon p_{T} of events passing TightID; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histTightDimuon_phi       = DIMUONsubDir.make<TH1F>("histTightDimuon_phi", "Dimuon #phi of events passing TightID; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histTightDimuon_eta       = DIMUONsubDir.make<TH1F>("histTightDimuon_eta", "Dimuon #eta of events passing TightID; #eta; Events", 50, -2.5, 2.5);

  histTightDimuonRoc_mass     = DIMUONsubDir.make<TH1F>("histTightDimuonRoc_mass","DimuonRoc mass of events passing TightID; mass (GeV/c^{2}); Events", 1000, 0., 1000.); 
  histTightDimuonRoc_pt        = DIMUONsubDir.make<TH1F>("histTightDimuonRoc_pt", "DimuonRoc p_{T} of events passing TightID; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histTightDimuonRoc_phi       = DIMUONsubDir.make<TH1F>("histTightDimuonRoc_phi", "DimuonRoc #phi of events passing TightID; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histTightDimuonRoc_eta       = DIMUONsubDir.make<TH1F>("histTightDimuonRoc_eta", "DimuonRoc #eta of events passing TightID; #eta; Events", 50, -2.5, 2.5);


  histMetDimuonM_mass  = DIMUONsubDir.make<TH1F>("histMetDimuonM_mass","Dimuon mass of events passing MediumID && Met cut; mass (GeV/c^{2}); Events", 1000, 0., 1000.);  

  histMetDimuon_mass  = DIMUONsubDir.make<TH1F>("histMetDimuon_mass","Dimuon mass of events passing Met cut; mass (GeV/c^{2}); Events", 1000, 0., 1000.); 
  histMetDimuon_pt   = DIMUONsubDir.make<TH1F>("histMetDimuon_pt", "Dimuon p_{T} of events passing Met cut; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histMetDimuon_phi  = DIMUONsubDir.make<TH1F>("histMetDimuon_phi", "Dimuon #phi of events passing Met cut; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histMetDimuon_eta  = DIMUONsubDir.make<TH1F>("histMetDimuon_eta", "Dimuon #eta of events passing Met; #eta; Events", 50, -2.5, 2.5);
  
  histCat1_mass      = DIMUONsubDir.make<TH1F>("histCat1_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  histCat2_mass      = DIMUONsubDir.make<TH1F>("histCat2_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  histCat1_pt        = DIMUONsubDir.make<TH1F>("histCat1_pt", "Dimuon p_{T} cat1; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histCat2_pt        = DIMUONsubDir.make<TH1F>("histCat2_pt", "Dimuon p_{T} cat2; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histCat1_phi       = DIMUONsubDir.make<TH1F>("histCat1_phi", "Dimuon #phi cat1; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histCat2_phi       = DIMUONsubDir.make<TH1F>("histCat2_phi", "Dimuon #phi cat2; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histCat1_eta       = DIMUONsubDir.make<TH1F>("histCat1_eta", "Dimuon #eta cat1; #eta; Events", 50, -2.5, 2.5);
  histCat2_eta       = DIMUONsubDir.make<TH1F>("histCat2_eta", "Dimuon #eta cat2; #eta; Events", 50, -2.5, 2.5);

  histCat1M_mass      = DIMUONsubDir.make<TH1F>("histCat1M_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  histCat2M_mass      = DIMUONsubDir.make<TH1F>("histCat2M_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  histCat1M_pt        = DIMUONsubDir.make<TH1F>("histCat1M_pt", "Dimuon p_{T} cat1; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histCat2M_pt        = DIMUONsubDir.make<TH1F>("histCat2M_pt", "Dimuon p_{T} cat2; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histCat1M_phi       = DIMUONsubDir.make<TH1F>("histCat1M_phi", "Dimuon #phi cat1; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histCat2M_phi       = DIMUONsubDir.make<TH1F>("histCat2M_phi", "Dimuon #phi cat2; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histCat1M_eta       = DIMUONsubDir.make<TH1F>("histCat1M_eta", "Dimuon #eta cat1; #eta; Events", 50, -2.5, 2.5);
  histCat2M_eta       = DIMUONsubDir.make<TH1F>("histCat2M_eta", "Dimuon #eta cat2; #eta; Events", 50, -2.5, 2.5);

  histCat11_mass      = DIMUONsubDir.make<TH1F>("histCat11_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  histCat22_mass      = DIMUONsubDir.make<TH1F>("histCat22_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  histCat11_pt        = DIMUONsubDir.make<TH1F>("histCat11_pt", "Dimuon p_{T} cat1; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histCat22_pt        = DIMUONsubDir.make<TH1F>("histCat22_pt", "Dimuon p_{T} cat2; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histCat11_phi       = DIMUONsubDir.make<TH1F>("histCat11_phi", "Dimuon #phi cat1; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histCat22_phi       = DIMUONsubDir.make<TH1F>("histCat22_phi", "Dimuon #phi cat2; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histCat11_eta       = DIMUONsubDir.make<TH1F>("histCat11_eta", "Dimuon #eta cat1; #eta; Events", 50, -2.5, 2.5);
  histCat22_eta       = DIMUONsubDir.make<TH1F>("histCat22_eta", "Dimuon #eta cat2; #eta; Events", 50, -2.5, 2.5);

  histCat11M_mass      = DIMUONsubDir.make<TH1F>("histCat11M_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  histCat22M_mass      = DIMUONsubDir.make<TH1F>("histCat22M_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  histCat11M_pt        = DIMUONsubDir.make<TH1F>("histCat11M_pt", "Dimuon p_{T} cat1; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histCat22M_pt        = DIMUONsubDir.make<TH1F>("histCat22M_pt", "Dimuon p_{T} cat2; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histCat11M_phi       = DIMUONsubDir.make<TH1F>("histCat11M_phi", "Dimuon #phi cat1; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histCat22M_phi       = DIMUONsubDir.make<TH1F>("histCat22M_phi", "Dimuon #phi cat2; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histCat11M_eta       = DIMUONsubDir.make<TH1F>("histCat11M_eta", "Dimuon #eta cat1; #eta; Events", 50, -2.5, 2.5);
  histCat22M_eta       = DIMUONsubDir.make<TH1F>("histCat22M_eta", "Dimuon #eta cat2; #eta; Events", 50, -2.5, 2.5);

 histCat111_mass      = DIMUONsubDir.make<TH1F>("histCat111_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  histCat222_mass      = DIMUONsubDir.make<TH1F>("histCat222_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  histCat111_pt        = DIMUONsubDir.make<TH1F>("histCat111_pt", "Dimuon p_{T} cat1; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histCat222_pt        = DIMUONsubDir.make<TH1F>("histCat222_pt", "Dimuon p_{T} cat2; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histCat111_phi       = DIMUONsubDir.make<TH1F>("histCat111_phi", "Dimuon #phi cat1; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histCat222_phi       = DIMUONsubDir.make<TH1F>("histCat222_phi", "Dimuon #phi cat2; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histCat111_eta       = DIMUONsubDir.make<TH1F>("histCat111_eta", "Dimuon #eta cat1; #eta; Events", 50, -2.5, 2.5);
  histCat222_eta       = DIMUONsubDir.make<TH1F>("histCat222_eta", "Dimuon #eta cat2; #eta; Events", 50, -2.5, 2.5);

  histCat111M_mass      = DIMUONsubDir.make<TH1F>("histCat111M_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  histCat222M_mass      = DIMUONsubDir.make<TH1F>("histCat222M_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  histCat111M_pt        = DIMUONsubDir.make<TH1F>("histCat111M_pt", "Dimuon p_{T} cat1; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histCat222M_pt        = DIMUONsubDir.make<TH1F>("histCat222M_pt", "Dimuon p_{T} cat2; p_{T} (GeV/c); Events", 1000, 0, 1000);
  histCat111M_phi       = DIMUONsubDir.make<TH1F>("histCat111M_phi", "Dimuon #phi cat1; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histCat222M_phi       = DIMUONsubDir.make<TH1F>("histCat222M_phi", "Dimuon #phi cat2; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  histCat111M_eta       = DIMUONsubDir.make<TH1F>("histCat111M_eta", "Dimuon #eta cat1; #eta; Events", 50, -2.5, 2.5);
  histCat222M_eta       = DIMUONsubDir.make<TH1F>("histCat222M_eta", "Dimuon #eta cat2; #eta; Events", 50, -2.5, 2.5);


  histZ_massGen      = DIMUONsubDir.make<TH1F>("histZ_massGen","Gen Higgs(A) mass; mass (GeV/c^{2});events", 300, 0., 1500.);
 
  TFileDirectory DIMUONsubDirRW = DIMUONsubDir.mkdir( "PileUpRw" );
  rwhistMediumDimuon_mass = DIMUONsubDirRW.make<TH1F>("rwhistMediumDimuon_mass","Dimuon mass of events passing MediumID; mass (GeV/c^{2}); Events", 1000, 0., 1000.); 
  rwhistMediumDimuon_pt   = DIMUONsubDirRW.make<TH1F>("rwhistMediumDimuon_pt", "Dimuon p_{T} of events passing MediumID; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistMediumDimuon_phi  = DIMUONsubDirRW.make<TH1F>("rwhistMediumDimuon_phi", "Dimuon #phi of events passing MediumID; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistMediumDimuon_eta  = DIMUONsubDirRW.make<TH1F>("rwhistMediumDimuon_eta", "Dimuon #eta of events passing MediumID; #eta; Events", 50, -2.5, 2.5);

  rwhistTightDimuon_mass = DIMUONsubDirRW.make<TH1F>("rwhistTightDimuon_mass","Dimuon mass of events passing TightID; mass (GeV/c^{2}); Events", 1000, 0., 1000.); 
  rwhistTightDimuon_pt   = DIMUONsubDirRW.make<TH1F>("rwhistTightDimuon_pt", "Dimuon p_{T} of events passing TightID; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistTightDimuon_phi  = DIMUONsubDirRW.make<TH1F>("rwhistTightDimuon_phi", "Dimuon #phi of events passing TightID; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistTightDimuon_eta  = DIMUONsubDirRW.make<TH1F>("rwhistTightDimuon_eta", "Dimuon #eta of events passing TightID; #eta; Events", 50, -2.5, 2.5);

  rwhistTightDimuonRoc_mass = DIMUONsubDirRW.make<TH1F>("rwhistTightDimuonRoc_mass","DimuonRoc mass of events passing TightID; mass (GeV/c^{2}); Events", 1000, 0., 1000.); 
  rwhistTightDimuonRoc_pt   = DIMUONsubDirRW.make<TH1F>("rwhistTightDimuonRoc_pt", "DimuonRoc p_{T} of events passing TightID; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistTightDimuonRoc_phi  = DIMUONsubDirRW.make<TH1F>("rwhistTightDimuonRoc_phi", "DimuonRoc #phi of events passing TightID; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistTightDimuonRoc_eta  = DIMUONsubDirRW.make<TH1F>("rwhistTightDimuonRoc_eta", "DimuonRoc #eta of events passing TightID; #eta; Events", 50, -2.5, 2.5);

  rwhistMetDimuonM_mass    = DIMUONsubDirRW.make<TH1F>("rwhistMetDimuonM_mass","Dimuon mass of events passing MediumID && Met cut; mass (GeV/c^{2}); Events", 1000, 0., 1000.); 
  rwhistMetDimuon_mass    = DIMUONsubDirRW.make<TH1F>("rwhistMetDimuon_mass","Dimuon mass of events passing Met cut; mass (GeV/c^{2}); Events", 1000, 0., 1000.); 
  rwhistMetDimuon_pt   = DIMUONsubDirRW.make<TH1F>("rwhistMetDimuon_pt", "Dimuon p_{T} of events passing Met cut; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistMetDimuon_phi  = DIMUONsubDirRW.make<TH1F>("rwhistMetDimuon_phi", "Dimuon #phi of events passing Met cut; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistMetDimuon_eta  = DIMUONsubDirRW.make<TH1F>("rwhistMetDimuon_eta", "Dimuon #eta of events passing Met cut; #eta; Events", 50, -2.5, 2.5);
    
  rwhistCat1_pt        = DIMUONsubDirRW.make<TH1F>("rwhistCat1_pt", "Dimuon p_{T} cat1; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistCat2_pt        = DIMUONsubDirRW.make<TH1F>("rwhistCat2_pt", "Dimuon p_{T} cat2; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistCat1_mass      = DIMUONsubDirRW.make<TH1F>("rwhistCat1_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwhistCat2_mass      = DIMUONsubDirRW.make<TH1F>("rwhistCat2_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwhistCat1_phi       = DIMUONsubDirRW.make<TH1F>("rwhistCat1_phi", "Dimuon #phi cat1; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistCat2_phi       = DIMUONsubDirRW.make<TH1F>("rwhistCat2_phi", "Dimuon #phi cat2; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistCat1_eta       = DIMUONsubDirRW.make<TH1F>("rwhistCat1_eta", "Dimuon #eta cat1; #eta; Events", 50, -2.5, 2.5);
  rwhistCat2_eta       = DIMUONsubDirRW.make<TH1F>("rwhistCat2_eta", "Dimuon #eta cat2; #eta; Events", 50, -2.5, 2.5);

  rwhistCat1M_pt        = DIMUONsubDirRW.make<TH1F>("rwhistCat1M_pt", "Dimuon p_{T} cat1; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistCat2M_pt        = DIMUONsubDirRW.make<TH1F>("rwhistCat2M_pt", "Dimuon p_{T} cat2; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistCat1M_mass      = DIMUONsubDirRW.make<TH1F>("rwhistCat1M_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwhistCat2M_mass      = DIMUONsubDirRW.make<TH1F>("rwhistCat2M_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwhistCat1M_phi       = DIMUONsubDirRW.make<TH1F>("rwhistCat1M_phi", "Dimuon #phi cat1; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistCat2M_phi       = DIMUONsubDirRW.make<TH1F>("rwhistCat2M_phi", "Dimuon #phi cat2; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistCat1M_eta       = DIMUONsubDirRW.make<TH1F>("rwhistCat1M_eta", "Dimuon #eta cat1; #eta; Events", 50, -2.5, 2.5);
  rwhistCat2M_eta       = DIMUONsubDirRW.make<TH1F>("rwhistCat2M_eta", "Dimuon #eta cat2; #eta; Events", 50, -2.5, 2.5);

  rwhistCat11_pt        = DIMUONsubDirRW.make<TH1F>("rwhistCat11_pt", "Dimuon p_{T} cat1; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistCat22_pt        = DIMUONsubDirRW.make<TH1F>("rwhistCat22_pt", "Dimuon p_{T} cat2; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistCat11_mass      = DIMUONsubDirRW.make<TH1F>("rwhistCat11_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwhistCat22_mass      = DIMUONsubDirRW.make<TH1F>("rwhistCat22_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwhistCat11_phi       = DIMUONsubDirRW.make<TH1F>("rwhistCat11_phi", "Dimuon #phi cat1; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistCat22_phi       = DIMUONsubDirRW.make<TH1F>("rwhistCat22_phi", "Dimuon #phi cat2; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistCat11_eta       = DIMUONsubDirRW.make<TH1F>("rwhistCat11_eta", "Dimuon #eta cat1; #eta; Events", 50, -2.5, 2.5);
  rwhistCat22_eta       = DIMUONsubDirRW.make<TH1F>("rwhistCat22_eta", "Dimuon #eta cat2; #eta; Events", 50, -2.5, 2.5);

  rwhistCat11M_pt        = DIMUONsubDirRW.make<TH1F>("rwhistCat11M_pt", "Dimuon p_{T} cat1; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistCat22M_pt        = DIMUONsubDirRW.make<TH1F>("rwhistCat22M_pt", "Dimuon p_{T} cat2; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistCat11M_mass      = DIMUONsubDirRW.make<TH1F>("rwhistCat11M_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwhistCat22M_mass      = DIMUONsubDirRW.make<TH1F>("rwhistCat22M_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwhistCat11M_phi       = DIMUONsubDirRW.make<TH1F>("rwhistCat11M_phi", "Dimuon #phi cat1; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistCat22M_phi       = DIMUONsubDirRW.make<TH1F>("rwhistCat22M_phi", "Dimuon #phi cat2; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistCat11M_eta       = DIMUONsubDirRW.make<TH1F>("rwhistCat11M_eta", "Dimuon #eta cat1; #eta; Events", 50, -2.5, 2.5);
  rwhistCat22M_eta       = DIMUONsubDirRW.make<TH1F>("rwhistCat22M_eta", "Dimuon #eta cat2; #eta; Events", 50, -2.5, 2.5);

  rwhistCat111_pt        = DIMUONsubDirRW.make<TH1F>("rwhistCat111_pt", "Dimuon p_{T} cat1; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistCat222_pt        = DIMUONsubDirRW.make<TH1F>("rwhistCat222_pt", "Dimuon p_{T} cat2; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistCat111_mass      = DIMUONsubDirRW.make<TH1F>("rwhistCat111_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwhistCat222_mass      = DIMUONsubDirRW.make<TH1F>("rwhistCat222_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwhistCat111_phi       = DIMUONsubDirRW.make<TH1F>("rwhistCat111_phi", "Dimuon #phi cat1; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistCat222_phi       = DIMUONsubDirRW.make<TH1F>("rwhistCat222_phi", "Dimuon #phi cat2; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistCat111_eta       = DIMUONsubDirRW.make<TH1F>("rwhistCat111_eta", "Dimuon #eta cat1; #eta; Events", 50, -2.5, 2.5);
  rwhistCat222_eta       = DIMUONsubDirRW.make<TH1F>("rwhistCat222_eta", "Dimuon #eta cat2; #eta; Events", 50, -2.5, 2.5);

  rwhistCat111M_pt        = DIMUONsubDirRW.make<TH1F>("rwhistCat111M_pt", "Dimuon p_{T} cat1; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistCat222M_pt        = DIMUONsubDirRW.make<TH1F>("rwhistCat222M_pt", "Dimuon p_{T} cat2; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwhistCat111M_mass      = DIMUONsubDirRW.make<TH1F>("rwhistCat111M_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwhistCat222M_mass      = DIMUONsubDirRW.make<TH1F>("rwhistCat222M_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwhistCat111M_phi       = DIMUONsubDirRW.make<TH1F>("rwhistCat111M_phi", "Dimuon #phi cat1; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistCat222M_phi       = DIMUONsubDirRW.make<TH1F>("rwhistCat222M_phi", "Dimuon #phi cat2; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwhistCat111M_eta       = DIMUONsubDirRW.make<TH1F>("rwhistCat111M_eta", "Dimuon #eta cat1; #eta; Events", 50, -2.5, 2.5);
  rwhistCat222M_eta       = DIMUONsubDirRW.make<TH1F>("rwhistCat222M_eta", "Dimuon #eta cat2; #eta; Events", 50, -2.5, 2.5);
 
  

  TFileDirectory DIMUONsubDirRWUP = DIMUONsubDir.mkdir( "PileUpRwUp" );
  rwuphistMediumMuons_mass = DIMUONsubDirRWUP.make<TH1F>("rwuphistMediumMuons_mass","Dimuon mass of events passing TightID; mass (GeV/c^{2}); Events", 1000, 0., 1000.); 
  rwuphistTightDimuon_mass  = DIMUONsubDirRWUP.make<TH1F>("rwuphistTightDimuon_mass","Dimuon mass of events passing TightID; mass (GeV/c^{2}); Events", 1000, 0., 1000.); 
  rwuphistTightDimuon_pt    = DIMUONsubDirRWUP.make<TH1F>("rwuphistTightDimuon_pt", "Dimuon p_{T} of events passing TightID; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwuphistTightDimuon_phi   = DIMUONsubDirRWUP.make<TH1F>("rwuphistTightDimuon_phi", "Dimuon #phi of events passing TightID; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwuphistTightDimuon_eta   = DIMUONsubDirRWUP.make<TH1F>("rwuphistTightDimuon_eta", "Dimuon #eta of events passing TightID; #eta; Events", 50, -2.5, 2.5);

  rwuphistMetDimuon_mass    = DIMUONsubDirRWUP.make<TH1F>("rwuphistMetDimuon_mass","Dimuon mass of events passing Met cut; mass (GeV/c^{2}); Events", 1000, 0., 1000.); 
  rwuphistMetDimuon_pt    = DIMUONsubDirRWUP.make<TH1F>("rwuphistMetDimuon_pt", "Dimuon p_{T} of events passing Met cut; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwuphistMetDimuon_phi   = DIMUONsubDirRWUP.make<TH1F>("rwuphistMetDimuon_phi", "Dimuon #phi of events passing Met cut; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwuphistMetDimuon_eta   = DIMUONsubDirRWUP.make<TH1F>("rwuphistMetDimuon_eta", "Dimuon #eta of events passing Met cut; #eta; Events", 50, -2.5, 2.5);

  rwuphistCat1_phi       = DIMUONsubDirRWUP.make<TH1F>("rwuphistCat1_phi", "Dimuon #phi cat1; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwuphistCat2_phi       = DIMUONsubDirRWUP.make<TH1F>("rwuphistCat2_phi", "Dimuon #phi cat2; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwuphistCat1_eta       = DIMUONsubDirRWUP.make<TH1F>("rwuphistCat1_eta", "Dimuon #eta cat1; #eta; Events", 50, -2.5, 2.5);
  rwuphistCat2_eta       = DIMUONsubDirRWUP.make<TH1F>("rwuphistCat2_eta", "Dimuon #eta cat2; #eta; Events", 50, -2.5, 2.5);
  rwuphistCat1_pt        = DIMUONsubDirRWUP.make<TH1F>("rwuphistCat1_pt", "Dimuon p_{T} cat1; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwuphistCat2_pt        = DIMUONsubDirRWUP.make<TH1F>("rwuphistCat2_pt", "Dimuon p_{T} cat2; p_{T} (GeV/c); Events", 1000, 0, 1000);  
  rwuphistCat1_mass      = DIMUONsubDirRWUP.make<TH1F>("rwuphistCat1_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwuphistCat2_mass      = DIMUONsubDirRWUP.make<TH1F>("rwuphistCat2_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwuphistCat11_mass     = DIMUONsubDirRWUP.make<TH1F>("rwuphistCat11_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwuphistCat22_mass     = DIMUONsubDirRWUP.make<TH1F>("rwuphistCat22_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  
  TFileDirectory DIMUONsubDirRWDOWN = DIMUONsubDir.mkdir( "PileUpRwDown" );
  rwdownhistMediumMuons_mass = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistMediumMuons_mass","Dimuon mass of events passing TightID; mass (GeV/c^{2}); Events", 1000, 0., 1000.); 
  rwdownhistTightDimuon_mass  = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistTightDimuon_mass","Dimuon mass of events passing TightID; mass (GeV/c^{2}); Events", 1000, 0., 1000.); 
  rwdownhistTightDimuon_pt    = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistTightDimuon_pt", "Dimuon p_{T} of events passing TightID; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwdownhistTightDimuon_phi   = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistTightDimuon_phi", "Dimuon #phi of events passing TightID; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwdownhistTightDimuon_eta   = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistTightDimuon_eta", "Dimuon #eta of events passing TightID; #eta; Events", 50, -2.5, 2.5);
 
  rwdownhistMetDimuon_mass    = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistMetDimuon_mass","Dimuon mass of events passing Met cut; mass (GeV/c^{2}); Events", 1000, 0., 1000.); 
  rwdownhistMetDimuon_pt    = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistMetDimuon_pt", "Dimuon p_{T} of events passing Met cut; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwdownhistMetDimuon_phi   = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistMetDimuon_phi", "Dimuon #phi of events passing Met cut; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwdownhistMetDimuon_eta   = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistMetDimuon_eta", "Dimuon #eta of events passing Met cut; #eta; Events", 50, -2.5, 2.5);
 
  rwdownhistCat1_phi       = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistCat1_phi", "Dimuon #phi cat1; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwdownhistCat2_phi       = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistCat2_phi", "Dimuon #phi cat2; #phi; Events", 50, -TMath::Pi(), TMath::Pi());
  rwdownhistCat1_eta       = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistCat1_eta", "Dimuon #eta cat1; #eta; Events", 50, -2.5, 2.5);
  rwdownhistCat2_eta       = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistCat2_eta", "Dimuon #eta cat2; #eta; Events", 50, -2.5, 2.5);    
  rwdownhistCat1_pt        = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistCat1_pt", "Dimuon p_{T} cat1; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwdownhistCat2_pt        = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistCat2_pt", "Dimuon p_{T} cat2; p_{T} (GeV/c); Events", 1000, 0, 1000);
  rwdownhistCat1_mass      = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistCat1_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwdownhistCat2_mass      = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistCat2_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwdownhistCat11_mass     = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistCat11_mass", "Dimuon mass cat1; mass (GeV/c^{2}); Events", 1000, 0, 1000);
  rwdownhistCat22_mass     = DIMUONsubDirRWDOWN.make<TH1F>("rwdownhistCat22_mass", "Dimuon mass cat2; mass (GeV/c^{2}); Events", 1000, 0, 1000);
      
  //rochester correction
  crab_rc.init("src/MyAnalyzer/Rochester/data");
  local_rc.init("/afs/cern.ch/work/f/federica/MSSM/Federica/CMSSW_8_0_26_patch1/src/MyAnalyzer/Rochester/data");

  //bJet SF
  // calib("CSVv2", "/afs/cern.ch/work/f/federica/MSSM/CMSSW_8_0_21/src/MyAnalyzer/MiniAnalyzer/data/CSVv2_Moriond17_B_F.csv"); 
  // reader(&calib, BTagEntry::OP_MEDIUM, "comb", "central");

}



MiniAnalyzer::~MiniAnalyzer(){}


// ------------ method called for each event  ------------
void
MiniAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  using namespace edm;
  using namespace reco;
  using namespace pat;
  using namespace std;
    
  // RoccoR  crab_rc("src/MyAnalyzer/Rochester/data");
  // RoccoR  local_rc("/afs/cern.ch/work/f/federica/MSSM/Federica/CMSSW_8_0_26_patch1/src/MyAnalyzer/Rochester/data");
  
  numEvents++; 
   
  if(MCtruth){ 
    
    Handle<GenEventInfoProduct> gen_ev_info;
    iEvent.getByToken(genEventInfo_, gen_ev_info);  

        
    //madgraph weight
    if (gen_ev_info.isValid()){
      EventWeight = gen_ev_info->weight();
      mc_weight = ( EventWeight > 0 ) ? 1 : -1;
      if(debug)
	std::cout<<" Event weight " << EventWeight << " MC Weight = " << mc_weight <<std::endl;


      if (gen_ev_info->hasPDF()) {
      	const gen::PdfInfo* pdfstuff = gen_ev_info->pdf();
	
      	double Q  = pdfstuff->scalePDF;
	
      	int id1   = pdfstuff->id.first;
      	int id2   = pdfstuff->id.second;
      	double x1 = pdfstuff->x.first;
      	double x2 = pdfstuff->x.second;

      	if(debug)
      	  std::cout<<" PDF: Q= " << Q << " id1 = " << id1  << " id2 = " << id2 << " x1 = " << x1 << " x2 = " << x2<<std::endl;
	
      }

    }
    

    // Handle<LHEEventProduct> lhe;
    // iEvent.getByToken(lhe_, lhe);
    
    // if ( lhe.isValid() and  lhe->weights().size() >=9){
    //   float  r1f2 = double(lhe -> weights() . at(1) . wgt); 
    //   float  r1f5 = double(lhe -> weights() . at(2) . wgt); 
    //   float  r2f1 = double(lhe -> weights() . at(3) . wgt); 
    //   float  r2f2 = double(lhe -> weights() . at(4) . wgt); 
    //   float  r5f1 = double(lhe -> weights() . at(6) . wgt); 
    //   float  r5f5 = double(lhe -> weights() . at(8) . wgt);  
    //   std::cout << "***** " <<r1f2 << std::endl;
    // }

    
    // if (lhe.isValid() and  lhe->weights().size() >109)
    //   for( int pdfw = 9 ; pdfw<109 ;++pdfw)
    //     {
    // 	  // pdfRwgt->push_back( double(lhe -> weights() . at(pdfw) . wgt ) );  
    // 	  std::cout << "***** weights  " << double(lhe -> weights() . at(pdfw) . wgt ) << std::endl;
	  
    //     }
    
    
    //PU Reweight
    Handle<std::vector<PileupSummaryInfo> > puInfo;
    iEvent.getByToken(puInfoToken_, puInfo);
    
    std::vector<PileupSummaryInfo>::const_iterator PVI;
    
    float Tnpv = -1;
    float PUnpv = -1;
    for(PVI = puInfo->begin(); PVI != puInfo->end(); ++PVI) {
      int BX = PVI->getBunchCrossing();
      if(BX == 0) { 
	Tnpv = PVI->getTrueNumInteractions();
	PUnpv = PVI->getPU_NumInteractions();
	if(debug)
	  std::cout << "Tnpv   "<<Tnpv<< " PUnpv   "<<PUnpv<< std::endl;
	continue;
      }
    }
    //std::cout << "Tnpv   "<<Tnpv<< " PUnpv   "<<PUnpv<< std::endl;
    histPUWeights->Fill(LumiWeights_.weight(Tnpv));
    histPUWeights2->Fill(LumiWeights_.weight(PUnpv)); 
    
    //mc and PU weights together
    MyWeight =  mc_weight*LumiWeights_.weight(Tnpv);
    MyWeightUp =  mc_weight*LumiWeightsUp_.weight(Tnpv);
    MyWeightDown =  mc_weight*LumiWeightsDown_.weight(Tnpv);
      
    if(debug){
      std::cout<<" Event weight " << EventWeight << " MC Weight = " << mc_weight <<std::endl;
      std::cout <<" PU weight only = " << LumiWeights_.weight(Tnpv) << " total Weight = "<<MyWeight<< std::endl;
      std::cout <<" PU weight UP = " << LumiWeightsUp_.weight(Tnpv) << std::endl;
      std::cout <<" PU weight DOWN = " << LumiWeightsDown_.weight(Tnpv) << std::endl;
    }
  }//end if mc and not signal   
  //questo non vale più perchè prima il segnale era generato col pileup sbagliato ora no.
  
  numEvents_mcweighted += mc_weight;
  numEvents_mcpuweighted += MyWeight;
  numEvents_mcpuweightedup += MyWeightUp;
  numEvents_mcpuweighteddown += MyWeightDown;

  goodMuons.clear(); //vector of muons in the acceptance matched to MC
  mediumMuons.clear();
  tightMuons.clear();

  if(debug){
    std::cout << " *********************************************************** " << std::endl;
    std::cout << " Number of Events "<< numEvents << std::endl; 
    std::cout << std::endl;
  } 

  Handle<reco::BeamSpot> beamspot; 
  iEvent.getByToken(beamSpotToken_, beamspot);  
 
  Handle<reco::VertexCollection> vertices;    
  iEvent.getByToken(vtxToken_, vertices);
  if (vertices->empty()) return; // skip the event if no PV found
  
  Handle<edm::TriggerResults> triggerBits;
  iEvent.getByToken(triggerBits_, triggerBits);
  Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
  iEvent.getByToken(triggerObjects_, triggerObjects);
  Handle<pat::PackedTriggerPrescales> triggerPrescales;
  iEvent.getByToken(triggerPrescales_, triggerPrescales);

  Handle<pat::MuonCollection> muons;            
  iEvent.getByToken(muonToken_, muons);

  // Handle<pat::ElectronCollection> electrons;            
  // iEvent.getByToken(electronToken_, electrons);  
  
  // Handle<edm::ValueMap<bool> > vid;
  // iEvent.getByToken(vidToken_,vid);
  
  // Handle<pat::TauCollection> taus;            
  // iEvent.getByToken(tauToken_, taus);
  

  Handle<std::vector<pat::Jet> > jets;
  iEvent.getByToken(jetToken_, jets);
  
  Handle<edm::ValueMap<float> > puJetIdMVA;
  iEvent.getByToken(puJetIdToken_, puJetIdMVA);
 
  Handle<edm::ValueMap<int> > puJetIdFlag;
  iEvent.getByToken(puJetFlagToken_, puJetIdFlag);

  // Handle<ValueMap<int> > puJetIdFlag;
  // event.getByLabel("puJetMva","fullId",puJetIdFlag);
  
  // Handle<edm::ValueMap<float> > puJetIdMVA;
  // event.getByLabel("puJetMva","fullDiscriminant",puJetIdMVA);

  Handle<pat::METCollection> mets;
  iEvent.getByToken(metToken_, mets);

  // Handle<edm::TriggerResults> metFilter;
  // iEvent.getByToken(metFilterToken_, metFilter);
 
  // Handle< bool > ifilterbadChCand;
  // iEvent.getByToken(BadChCandFilterToken_, ifilterbadChCand);
   
  // Handle< bool > ifilterbadPFMuon;
  // iEvent.getByToken(BadPFMuonToken_, ifilterbadPFMuon);
  
  Handle<edm::View<reco::GenParticle> > pruned; 
  Handle<edm::View<pat::PackedGenParticle> > packed; 

  if(MCtruth){
    // Packed particles are all the status 1, so usable to remake jets
    // The navigation from status 1 to pruned is possible (the other direction should be made by hand)
    iEvent.getByToken(prunedGenToken_,pruned);
    iEvent.getByToken(packedGenToken_,packed);
  }
  //if(eventNumber%2==0){
  //bool eventMuok = false;
  ////////////////////////////////////////////////////////////////////////
  ///////////////////// MC checks on DY or Signal ////////////////////////
  ////////////////////////////////////////////////////////////////////////
  if(MCtruth && checksOnDY){
    
    bool eventBok = false;
    bool altoEta = false;
    for(size_t i=0; i<pruned->size();i++){
      // if( fabs((*pruned)[i].pdgId()) == 12 || fabs((*pruned)[i].pdgId()) == 14 || fabs((*pruned)[i].pdgId())==16)
      // 	    std::cout << " Pruned PdgID: " << (*pruned)[i].pdgId() << " status: " << (*pruned)[i].status() << " pt " << (*pruned)[i].pt() << " eta: " << (*pruned)[i].eta() << " phi: " << (*pruned)[i].phi() << std::endl;
	 
        
      
      if((*pruned)[i].status() == 62 &&
      	 (*pruned)[i].pdgId() == 36){
      	const Candidate *Z = &(*pruned)[i];
      	generatedZ++;
      	mGenZ = Z->mass();
      	histZ_massGen-> Fill(mGenZ);  // to check that is one
	
	
	//if(mGenZ > 250. && mGenZ < 350.)
	//  eventMuok = true;
	
	//std::cout << "Higgs: PdgID = " << Z->pdgId() << " status = " << Z->status() << " mother = " << Z->mother(0)->pdgId() << ", mass = "<< Z->mass() << ", pt = " << Z->pt() << ", eta = " << Z->eta() << ", phi = " << Z->phi() << std::endl;
	//std::cout <<std::endl;
         
	for(size_t j=0; j<packed->size();j++){
	  // 	get the pointer to the first survied ancestor of a given packed GenParticle in the prunedCollection 
	  const Candidate * motherInPrunedCollection = (*packed)[j].mother(0) ;
	  if(motherInPrunedCollection != nullptr && isAncestor( Z , motherInPrunedCollection)){
	    if(debug) 
	      std::cout << " PdgID: " << (*packed)[j].pdgId() << " pt " << (*packed)[j].pt() << " eta: " << (*packed)[j].eta() << " phi: " << (*packed)[j].phi() << std::endl;
	    
	    // 	  all the GEN muons from Z/Z' 
	    if(abs((*packed)[j].pdgId()) == 13 && (*packed)[j].status() == 1){
	      genMuons.push_back((*packed)[j]);	    
	      generatedMuons++;
	      
	      // 	    cut on the acceptance at GEN level
	      // if(fabs((*packed)[j].eta()) < EtaMuonLoose && (*packed)[j].pt() > PtMuonLoose ){
	      // 	if(debug) std::cout << " Gen Muon in the acceptance (|eta| < 2.4 && pt > 25 GeV): status = " << (*packed)[j].status() << ", pt = " << (*packed)[j].pt() <<", eta = " << (*packed)[j].eta() << std::endl;	
	      // 	//genMuonsAcc.push_back((*packed)[j]);	
	      // 	// 	      generatedMuons_acc++;
	      // 	eventMuok = true;	
	      // 	// 	    }
	    }
	  }
	}
      }
      
	
  
      // if( eventMuok && fabs((*pruned)[i].pdgId())==5 && (*pruned)[i].status() == 71){
      //   std::cout << "quark b: PdgID = " << (*pruned)[i].pdgId() << ", status " << (*pruned)[i].status() << "  pt = " << (*pruned)[i].pt() << ", eta = " << (*pruned)[i].eta() << ", phi = " << (*pruned)[i].phi() << std::endl;
      //   eventBok = true;
      
      //   if(fabs((*pruned)[i].eta())>2.4){
      // 	altoEta = true;
      // 	std::cout << " HIGH ETA JET " << (*pruned)[i].eta() << std::endl;
      //   }
      // }
    }
    
    // if(eventMuok){
    //   GenEvent2mu++;
    //   if(eventBok){
    //     GenEventb++;
    //     if(altoEta)
    // 	GenEventHE++;
    //   }
    // }
    
    
    }//if checks on DY && MCTruth
  
  //  return;
 
  
  //met filters
  unsigned int filter = 1;
  bool filterbadChCandidate = true;// *ifilterbadChCand;
  bool filterbadPFMuon = true; //*ifilterbadPFMuon;
  if(debug)
    std::cout << " Bad Muon = " << filterbadPFMuon << "  Bad Ch = " << filterbadChCandidate << std::endl;
  
  // const edm::TriggerNames &names = iEvent.triggerNames(*metFilter);
  
  // if ( metFilter.isValid() and not metFilter.failedToGet() ) {
  //   for ( unsigned int i = 0; i < names.size(); ++i) {            
  //     if ( std::find( metfilterNames.begin(), metfilterNames.end(), names.triggerName(i) ) != metfilterNames.end() ) {
	
  // 	if(debug)	
  // 	  std::cout << "MetFilter : " << names.triggerName(i) << " is registered " << metFilter->accept(i) << std::endl;
	
  // 	filter *= metFilter->accept(i);
	
  //     }
  //   }
  // }

  if(filter>0 && filterbadChCandidate && filterbadPFMuon ){
  //if(eventMuok && filter>0 && filterbadChCandidate && filterbadPFMuon ){
    numEventsAlPicco++;
    //////////////////////////////////////////////////////////////////////////
    /////////////////////Beam Spot and PV/////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
  
    if (!beamspot.isValid()) {
      cout << "Error! Can't get the product primary Vertex Collection "<< endl;
      return;
    }
    const reco::BeamSpot &BS = *beamspot.product();
    const math::XYZPoint &thebs = BS.position();
  
    int vtxs = vertices->size();
    // histAllVertices->Fill(vtxs);
    
    math::XYZPoint PV_position;
    reco::Vertex PV;
    for(reco::VertexCollection::const_iterator vtx = vertices->begin(); vtx != vertices->end(); ++vtx){
      
      if (!vtx->isFake() && vtx->ndof()>4. && (fabs(vtx->z())<15) ){
	PV_position = vtx->position();
	PV = *vtx;
      
	break;
      }
    }
  
    if(debug){ //compare vertex and beamspot
      cout << "BeamSpot position: x = "<< thebs.x()<< " y = "<< thebs.y() <<" z = "<< thebs.z()<<endl;
      cout << "Primary Vertex:    x = "<< PV_position.x() << " y = " << PV_position.y() << " z = " << PV_position.z() << endl;
    }
  
    //////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Muon PreSelection///////////////////////////
    //////////////////////////////////////////////////////////////////////////
     
    int nMU_10 = 0, nMU_25 = 0, nMU_IsoTrigger = 0, nMU_IsoTkTrigger = 0, nMU_tight = 0, nMU_iso = 0;
    int nMU_globalPtEta = 0;

    std::vector<pair <pat::Muon,double> > presel_muons;
    
    for (pat::Muon mu : *muons) {
      if (mu.pt()>PtMuonLoose && fabs(mu.eta())<EtaMuonLoose && mu.isGlobalMuon()){
	++nMU_10;
	
	//compute the isolation
	double deltaB = mu.pfIsolationR04().sumChargedHadronPt/mu.pt();
	double deltaB_corrected = mu.pfIsolationR04().sumNeutralHadronEt + mu.pfIsolationR04().sumPhotonEt - 0.5*mu.pfIsolationR04().sumPUPt;
	      
	if(deltaB_corrected <0.) deltaB_corrected = deltaB;
	else if(deltaB_corrected >=0.) deltaB_corrected = deltaB_corrected/mu.pt() + deltaB;   
      
	presel_muons.push_back(std::make_pair(mu, deltaB_corrected));
      
	if(mu.pt()>PtMuonTight)	++nMU_25;
      
	//did the muon fire the trigger?
	const pat::TriggerObjectStandAloneCollection muHLTMatches_iso     =  mu.triggerObjectMatchesByPath(pathNames_lowIsoUnpr);
	const pat::TriggerObjectStandAloneCollection muHLTMatches_isoTk   =  mu.triggerObjectMatchesByPath(pathNames_lowIsoUnpr); //uguale a quello sopra
	
	if(muHLTMatches_iso.size() > 0)    
	  ++nMU_IsoTrigger;
      
	if(muHLTMatches_isoTk.size() > 0) 
	  ++nMU_IsoTkTrigger;
      	
	if(mu.pt() > PtMuonTight){
	  ++nMU_globalPtEta;

	  //Tight ID
	  if(mu.isTightMuon(PV) && mu.pt() > PtMuonTight){
	    ++nMU_tight;
	    
	    if(deltaB_corrected < Isolation) 
	      ++nMU_iso;
	  }
	}
	
        if(debug) 
	  printf("muon with pt %4.1f, eta %4.1f, phi %4.1f, dz(PV) %+5.3f, POG loose id %d, medium id %d, tight id %d\n",
		 mu.pt(), mu.eta(), mu.phi(), mu.muonBestTrack()->dz(PV_position), mu.isLooseMuon(), mu.isMediumMuon(), mu.isTightMuon(PV) );    
	  
      }

      if(debug) 
	printf("muon with pt %4.1f, eta %4.1f, phi %4.1f, dz(PV) %+5.3f, POG loose id %d, medium id %d, tight id %d\n",
	       mu.pt(), mu.eta(), mu.phi(), mu.muonBestTrack()->dz(PV_position), mu.isLooseMuon(), mu.isMediumMuon(), mu.isTightMuon(PV) ); 
    }//end loop on pat::muons
    
    //std::cout << std::endl;
  
    /////////////////////////////////////////////////////
    ///////////////////// take the event/////////////////
    /////////////////////////////////////////////////////
    
    if(Signal){
      //Fill the event info block
      runnumber = iEvent.run();
      //lumiblock = iEvent.getLuminosityBlock().luminosityBlock();
      lumiblock = iEvent.eventAuxiliary().luminosityBlock();
      eventNumber = iEvent.eventAuxiliary().event();
      timestamp = iEvent.eventAuxiliary().time().value();
      bunchXing = iEvent.eventAuxiliary().bunchCrossing();
      	
	 
      fill_beamspot_variables(beamspot);
      fill_vtxs_variables(vertices);
      fill_hlt_variables(triggerBits, iEvent);
      
      fill_muons_variables(muons, thebs, PV, local_rc, crab_rc);
      //fill_electrons_variables(electrons);
           
      fill_PFJet_variables(jets, puJetIdMVA, puJetIdFlag, jecUnc);
      
      fill_PFMET_variables(mets);
      
      fill_gen_variables(pruned, packed);
      
      tree_->Fill(); 
    }
    
    else if(nMU_10>1 && nMU_25>0){
      //Fill the event info block
      runnumber = iEvent.run();
      lumiblock = iEvent.eventAuxiliary().luminosityBlock();
      //iEvent.getLuminosityBlock().luminosityBlock();
      eventNumber = iEvent.eventAuxiliary().event();
      timestamp = iEvent.eventAuxiliary().time().value();
      bunchXing = iEvent.eventAuxiliary().bunchCrossing();
      orbitNum = iEvent.eventAuxiliary().orbitNumber();
      
      if(debug) 
	std::cout << " Event in " << eventNumber << std::endl;
      
      //fill PV histos with/wo reweight
      histAllVertices->Fill(vtxs);
      histAllVerticesRW->Fill(vtxs, MyWeight);
      
      
      if(youwantNTuples){
	fill_beamspot_variables(beamspot);
	fill_vtxs_variables(vertices);
	fill_hlt_variables(triggerBits, iEvent);
	
	fill_muons_variables(muons, thebs, PV, local_rc, crab_rc);
	//fill_electrons_variables(electrons);
	
	if(MCtruth)
	  fill_PFJet_variables(jets, puJetIdMVA, puJetIdFlag, jecUnc);
	else if(iEvent.run() <= 276811)
	  fill_PFJet_variables(jets, puJetIdMVA, puJetIdFlag, jecUnc_BCD);
	else if(iEvent.run() <= 278801)
	  fill_PFJet_variables(jets, puJetIdMVA, puJetIdFlag, jecUnc_EF);
	else if(iEvent.run() <= 280385)
	  fill_PFJet_variables(jets, puJetIdMVA, puJetIdFlag, jecUnc_G);
	else
	  fill_PFJet_variables(jets, puJetIdMVA, puJetIdFlag, jecUnc_H);
	
	fill_PFMET_variables(mets);
	
	if(MCtruth)
	  fill_gen_variables(pruned, packed);
	
	tree_->Fill();
      }
      
      //some differences between the ntuples and the histos can occour because:
      //NTuples are taking all the muons in preselected events 
      //Histos are taking preselected muons in preselected events
      
      if(youwantAnalysis){
      
	std::vector<pair <pat::Muon,bool> > tight_muons;
	std::vector<pair <pat::Muon,bool> > medium_muons;
	
	int  nJets = 0, nlightJets = 0, nbJets = 0;
	int  nJetsM = 0, nlightJetsM = 0, nbJetsM = 0;
	std::vector<pat::Muon> muon_pair;
	std::vector<pat::Muon> muon_pairMed;
      
	//////////////////////////////////////////////////////////////////////////
	/////////////////////Muons and Trigger////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
      
	++RECO; // number of reco events in our acceptance
	if(debug)
	  std::cout << " RECO " << RECO << std::endl;  
	
	if(nMU_IsoTrigger > 0 || nMU_IsoTkTrigger > 0){
	  ++TRIGGER;   
	  
	  if(nMU_globalPtEta > 1)
	    ++GLOBALPTETA;     
	  	  
	  if(nMU_tight > 1)
	    ++TIGHTID;
	  
	  if(nMU_iso > 1)
	    ++ISO;
	}
      
	for(unsigned int i = 0; i < presel_muons.size(); i++){
	  pat::Muon muon = presel_muons[i].first;
	  double iso = presel_muons[i].second;
	  
	  histAllMuons_pt  ->Fill(muon.pt(), mc_weight);  
	  histAllMuons_eta ->Fill(muon.eta(), mc_weight);
	  histAllMuons_phi ->Fill(muon.phi(), mc_weight);
	  histAllMuons_pv  ->Fill(vtxs, mc_weight);
	
	  rwhistAllMuons_pt  ->Fill(muon.pt(), MyWeight);  
	  rwhistAllMuons_eta ->Fill(muon.eta(), MyWeight);
	  rwhistAllMuons_phi ->Fill(muon.phi(), MyWeight);
	  rwhistAllMuons_pv  ->Fill(vtxs, MyWeight);

	  //time veto 0 is in time, 1 is OOT
	  int veto = 0; // default is in-time muon      
	
	  reco::MuonTime cmb = muon.time();
	  reco::MuonTime rpc = muon.rpcTime();
	  bool cmbok =(cmb.nDof>7);
	  bool rpcok =(rpc.nDof>1 && rpc.timeAtIpInOutErr==0);
	  if (rpcok){
	    if ((fabs(rpc.timeAtIpInOut)>10) &&    !(cmbok && fabs(cmb.timeAtIpInOut)<10))
	      veto = 1;
	  }
	  else if (cmbok && (cmb.timeAtIpInOut>20 || cmb.timeAtIpInOut<-45))
	    veto = 1;
	  
	  histAllMuons_timingVeto->Fill(veto, mc_weight);
	  
	  //Plot the muon quantites
	  if(muon.pt()>PtMuonTight){
	
	    if(muon.isGlobalMuon()){
	      histNormChi2->Fill(muon.globalTrack()->normalizedChi2(), mc_weight);
	      histMatchSt->Fill(muon.numberOfMatchedStations(), mc_weight);
	      histMuValidHits->Fill(muon.globalTrack()->hitPattern().numberOfValidMuonHits(), mc_weight);
	      histDxy->Fill(muon.muonBestTrack()->dxy(PV_position), mc_weight);
	      histDz->Fill(muon.muonBestTrack()->dz(PV_position), mc_weight);
	      histPixelHits->Fill(muon.innerTrack()->hitPattern().numberOfValidPixelHits(), mc_weight);
	      histTrackerL->Fill(muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), mc_weight);
	      histDeltaPtOverPt->Fill(muon.globalTrack()->ptError()/muon.globalTrack()->pt(), mc_weight);

	      rwhistNormChi2->Fill(muon.globalTrack()->normalizedChi2(), MyWeight);
	      rwhistMatchSt->Fill(muon.numberOfMatchedStations(), MyWeight);
	      rwhistMuValidHits->Fill(muon.globalTrack()->hitPattern().numberOfValidMuonHits(), MyWeight);
	      rwhistDxy->Fill(muon.muonBestTrack()->dxy(PV_position), mc_weight);
	      rwhistDz->Fill(muon.muonBestTrack()->dz(PV_position), mc_weight);
	      rwhistPixelHits->Fill(muon.innerTrack()->hitPattern().numberOfValidPixelHits(), MyWeight);
	      rwhistTrackerL->Fill(muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), MyWeight);
	      rwhistDeltaPtOverPt->Fill(muon.globalTrack()->ptError()/muon.globalTrack()->pt(), MyWeight);
	      
	    }
	    
	    //////// N-1 selection
	    // numerator
	    if( muon.isPFMuon() &&  
		muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5 &&
		muon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0 && 
		muon.globalTrack()->hitPattern().numberOfValidMuonHits() > 0 && 
		fabs(muon.dB()) < 0.2 &&  
		fabs(muon.muonBestTrack()->dz(PV_position)) < 0.5 &&     	  
		muon.globalTrack()->normalizedChi2() < 10 && 
		muon.numberOfMatchedStations() > 1 &&
		//( muon.numberOfMatchedStations() > 1 || (muon.numberOfMatchedStations() == 1 && !(muon.stationMask() == 1 || muon.stationMask() == 16)) || (muon.numberOfMatchedStations() == 1 && (muon.stationMask() == 1 || muon.stationMask() == 16) && muon.numberOfMatchedRPCLayers() > 2)) &&
		iso < Isolation 
		){
	      
	      histPt_num  ->Fill(muon.pt(), mc_weight);  
	      histEta_num ->Fill(muon.eta(), mc_weight);
	      histPhi_num ->Fill(muon.phi(), mc_weight);
	      histPV_num  ->Fill(vtxs, mc_weight);

	      histNormChi2_num->Fill(muon.globalTrack()->normalizedChi2(), mc_weight);
	      histMatchSt_num->Fill(muon.numberOfMatchedStations(), mc_weight);
	      histMuValidHits_num->Fill(muon.globalTrack()->hitPattern().numberOfValidMuonHits(), mc_weight);
	      histDxy_num->Fill(muon.muonBestTrack()->dxy(PV_position), mc_weight);
	      histDz_num->Fill(muon.muonBestTrack()->dz(PV_position), mc_weight);
	      histPixelHits_num->Fill(muon.innerTrack()->hitPattern().numberOfValidPixelHits(), mc_weight);
	      histTrackerL_num->Fill(muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), mc_weight);
	      histDeltaPtOverPt_num->Fill(muon.globalTrack()->ptError()/muon.globalTrack()->pt(), mc_weight);
	   

	      rwhistPt_num  ->Fill(muon.pt(), MyWeight);  
	      rwhistEta_num ->Fill(muon.eta(), MyWeight);
	      rwhistPhi_num ->Fill(muon.phi(), MyWeight);
	      rwhistPV_num  ->Fill(vtxs, MyWeight);

	      rwhistNormChi2_num->Fill(muon.globalTrack()->normalizedChi2(), MyWeight);
	      rwhistMatchSt_num->Fill(muon.numberOfMatchedStations(), MyWeight);
	      rwhistMuValidHits_num->Fill(muon.globalTrack()->hitPattern().numberOfValidMuonHits(), MyWeight);
	      rwhistDxy_num->Fill(muon.muonBestTrack()->dxy(PV_position), mc_weight);
	      rwhistDz_num->Fill(muon.muonBestTrack()->dz(PV_position), mc_weight);
	      rwhistPixelHits_num->Fill(muon.innerTrack()->hitPattern().numberOfValidPixelHits(), MyWeight);
	      rwhistTrackerL_num->Fill(muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), MyWeight);
	      rwhistDeltaPtOverPt_num->Fill(muon.globalTrack()->ptError()/muon.globalTrack()->pt(), MyWeight);

	    }
	    // N- tracker layers
	    if( muon.isPFMuon() &&  
		//muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5 &&
		muon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0 && 
		muon.globalTrack()->hitPattern().numberOfValidMuonHits() > 0 && 
		fabs(muon.dB()) < 0.2 &&  
		fabs(muon.muonBestTrack()->dz(PV_position)) < 0.5 &&     	  
		muon.globalTrack()->normalizedChi2() < 10 && 
		muon.numberOfMatchedStations() > 1 &&
		//( muon.numberOfMatchedStations() > 1 || (muon.numberOfMatchedStations() == 1 && !(muon.stationMask() == 1 || muon.stationMask() == 16)) || (muon.numberOfMatchedStations() == 1 && (muon.stationMask() == 1 || muon.stationMask() == 16) && muon.numberOfMatchedRPCLayers() > 2)) &&
		iso < Isolation 
		){

	      histPt_tkl  ->Fill(muon.pt(), mc_weight);  
	      histEta_tkl ->Fill(muon.eta(), mc_weight);
	      histPhi_tkl ->Fill(muon.phi(), mc_weight);
	      histPV_tkl  ->Fill(vtxs, mc_weight);

	      histNormChi2_tkl->Fill(muon.globalTrack()->normalizedChi2(), mc_weight);
	      histMatchSt_tkl->Fill(muon.numberOfMatchedStations(), mc_weight);
	      histMuValidHits_tkl->Fill(muon.globalTrack()->hitPattern().numberOfValidMuonHits(), mc_weight);
	      histDxy_tkl->Fill(muon.muonBestTrack()->dxy(PV_position), mc_weight);
	      histDz_tkl->Fill(muon.muonBestTrack()->dz(PV_position), mc_weight);
	      histPixelHits_tkl->Fill(muon.innerTrack()->hitPattern().numberOfValidPixelHits(), mc_weight);
	      histTrackerL_tkl->Fill(muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), mc_weight);
	      histDeltaPtOverPt_tkl->Fill(muon.globalTrack()->ptError()/muon.globalTrack()->pt(), mc_weight);
	   
	    }

	    // N- pixel hits
	    if( muon.isPFMuon() &&  
		muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5 &&
		//muon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0 && 
		muon.globalTrack()->hitPattern().numberOfValidMuonHits() > 0 && 
		fabs(muon.dB()) < 0.2 &&  
		fabs(muon.muonBestTrack()->dz(PV_position)) < 0.5 &&     	  
		muon.globalTrack()->normalizedChi2() < 10 && 
		muon.numberOfMatchedStations() > 1 &&
		//( muon.numberOfMatchedStations() > 1 || (muon.numberOfMatchedStations() == 1 && !(muon.stationMask() == 1 || muon.stationMask() == 16)) || (muon.numberOfMatchedStations() == 1 && (muon.stationMask() == 1 || muon.stationMask() == 16) && muon.numberOfMatchedRPCLayers() > 2)) &&
		iso < Isolation 
		){
	      
	      histPt_pixh  ->Fill(muon.pt(), mc_weight);  
	      histEta_pixh ->Fill(muon.eta(), mc_weight);
	      histPhi_pixh ->Fill(muon.phi(), mc_weight);
	      histPV_pixh  ->Fill(vtxs, mc_weight);

	      histNormChi2_pixh->Fill(muon.globalTrack()->normalizedChi2(), mc_weight);
	      histMatchSt_pixh->Fill(muon.numberOfMatchedStations(), mc_weight);
	      histMuValidHits_pixh->Fill(muon.globalTrack()->hitPattern().numberOfValidMuonHits(), mc_weight);
	      histDxy_pixh->Fill(muon.muonBestTrack()->dxy(PV_position), mc_weight);
	      histDz_pixh->Fill(muon.muonBestTrack()->dz(PV_position), mc_weight);
	      histPixelHits_pixh->Fill(muon.innerTrack()->hitPattern().numberOfValidPixelHits(), mc_weight);
	      histTrackerL_pixh->Fill(muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), mc_weight);
	      histDeltaPtOverPt_pixh->Fill(muon.globalTrack()->ptError()/muon.globalTrack()->pt(), mc_weight);
	   
	    }
	    
	    // N- muon hits
	    if( muon.isPFMuon() &&  
		muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5 &&
		muon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0 && 
		//muon.globalTrack()->hitPattern().numberOfValidMuonHits() > 0 && 
		fabs(muon.dB()) < 0.2 &&  
		fabs(muon.muonBestTrack()->dz(PV_position)) < 0.5 &&     	  
		muon.globalTrack()->normalizedChi2() < 10 && 
		muon.numberOfMatchedStations() > 1 &&
		//( muon.numberOfMatchedStations() > 1 || (muon.numberOfMatchedStations() == 1 && !(muon.stationMask() == 1 || muon.stationMask() == 16)) || (muon.numberOfMatchedStations() == 1 && (muon.stationMask() == 1 || muon.stationMask() == 16) && muon.numberOfMatchedRPCLayers() > 2)) &&
		iso < Isolation 
		){

	      histPt_muh  ->Fill(muon.pt(), mc_weight);  
	      histEta_muh ->Fill(muon.eta(), mc_weight);
	      histPhi_muh ->Fill(muon.phi(), mc_weight);
	      histPV_muh  ->Fill(vtxs, mc_weight);
	      
	      histNormChi2_muh->Fill(muon.globalTrack()->normalizedChi2(), mc_weight);
	      histMatchSt_muh->Fill(muon.numberOfMatchedStations(), mc_weight);
	      histMuValidHits_muh->Fill(muon.globalTrack()->hitPattern().numberOfValidMuonHits(), mc_weight);
	      histDxy_muh->Fill(muon.muonBestTrack()->dxy(PV_position), mc_weight);
	      histDz_muh->Fill(muon.muonBestTrack()->dz(PV_position), mc_weight);
	      histPixelHits_muh->Fill(muon.innerTrack()->hitPattern().numberOfValidPixelHits(), mc_weight);
	      histTrackerL_muh->Fill(muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), mc_weight);
	      histDeltaPtOverPt_muh->Fill(muon.globalTrack()->ptError()/muon.globalTrack()->pt(), mc_weight);
	   
	    }

	    // N- dB
	    if( muon.isPFMuon() &&  
		muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5 &&
		muon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0 && 
		muon.globalTrack()->hitPattern().numberOfValidMuonHits() > 0 && 
		//fabs(muon.dB()) < 0.2 &&  
		fabs(muon.muonBestTrack()->dz(PV_position)) < 0.5 &&     	  
		muon.globalTrack()->normalizedChi2() < 10 && 
		muon.numberOfMatchedStations() > 1 &&
		//( muon.numberOfMatchedStations() > 1 || (muon.numberOfMatchedStations() == 1 && !(muon.stationMask() == 1 || muon.stationMask() == 16)) || (muon.numberOfMatchedStations() == 1 && (muon.stationMask() == 1 || muon.stationMask() == 16) && muon.numberOfMatchedRPCLayers() > 2)) &&
		iso < Isolation 
		){

	      histPt_db  ->Fill(muon.pt(), mc_weight);  
	      histEta_db ->Fill(muon.eta(), mc_weight);
	      histPhi_db ->Fill(muon.phi(), mc_weight);
	      histPV_db  ->Fill(vtxs, mc_weight);

	      histNormChi2_db->Fill(muon.globalTrack()->normalizedChi2(), mc_weight);
	      histMatchSt_db->Fill(muon.numberOfMatchedStations(), mc_weight);
	      histMuValidHits_db->Fill(muon.globalTrack()->hitPattern().numberOfValidMuonHits(), mc_weight);
	      histDxy_db->Fill(muon.muonBestTrack()->dxy(PV_position), mc_weight);
	      histDz_db->Fill(muon.muonBestTrack()->dz(PV_position), mc_weight);
	      histPixelHits_db->Fill(muon.innerTrack()->hitPattern().numberOfValidPixelHits(), mc_weight);
	      histTrackerL_db->Fill(muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), mc_weight);
	      histDeltaPtOverPt_db->Fill(muon.globalTrack()->ptError()/muon.globalTrack()->pt(), mc_weight);
	   
	    }

	    // N- dz
	    if( muon.isPFMuon() &&  
		muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5 &&
		muon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0 && 
		muon.globalTrack()->hitPattern().numberOfValidMuonHits() > 0 && 
		fabs(muon.dB()) < 0.2 &&  
		//fabs(muon.muonBestTrack()->dz(PV_position)) < 0.5 &&     	  
		muon.globalTrack()->normalizedChi2() < 10 && 
		muon.numberOfMatchedStations() > 1 &&
		//( muon.numberOfMatchedStations() > 1 || (muon.numberOfMatchedStations() == 1 && !(muon.stationMask() == 1 || muon.stationMask() == 16)) || (muon.numberOfMatchedStations() == 1 && (muon.stationMask() == 1 || muon.stationMask() == 16) && muon.numberOfMatchedRPCLayers() > 2)) &&
		iso < Isolation 
		){

	      histPt_dz  ->Fill(muon.pt(), mc_weight);  
	      histEta_dz ->Fill(muon.eta(), mc_weight);
	      histPhi_dz ->Fill(muon.phi(), mc_weight);
	      histPV_dz  ->Fill(vtxs, mc_weight);

	      histNormChi2_dz->Fill(muon.globalTrack()->normalizedChi2(), mc_weight);
	      histMatchSt_dz->Fill(muon.numberOfMatchedStations(), mc_weight);
	      histMuValidHits_dz->Fill(muon.globalTrack()->hitPattern().numberOfValidMuonHits(), mc_weight);
	      histDxy_dz->Fill(muon.muonBestTrack()->dxy(PV_position), mc_weight);
	      histDz_dz->Fill(muon.muonBestTrack()->dz(PV_position), mc_weight);
	      histPixelHits_dz->Fill(muon.innerTrack()->hitPattern().numberOfValidPixelHits(), mc_weight);
	      histTrackerL_dz->Fill(muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), mc_weight);
	      histDeltaPtOverPt_dz->Fill(muon.globalTrack()->ptError()/muon.globalTrack()->pt(), mc_weight);
	   
	    }

	    // N- chi2
	    if( muon.isPFMuon() &&  
		muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5 &&
		muon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0 && 
		muon.globalTrack()->hitPattern().numberOfValidMuonHits() > 0 && 
		fabs(muon.dB()) < 0.2 &&  
		fabs(muon.muonBestTrack()->dz(PV_position)) < 0.5 &&     	  
		//muon.globalTrack()->normalizedChi2() < 10 && 
		muon.numberOfMatchedStations() > 1 &&
		//( muon.numberOfMatchedStations() > 1 || (muon.numberOfMatchedStations() == 1 && !(muon.stationMask() == 1 || muon.stationMask() == 16)) || (muon.numberOfMatchedStations() == 1 && (muon.stationMask() == 1 || muon.stationMask() == 16) && muon.numberOfMatchedRPCLayers() > 2)) &&
		iso < Isolation 
		){

	      histPt_chi2  ->Fill(muon.pt(), mc_weight);  
	      histEta_chi2 ->Fill(muon.eta(), mc_weight);
	      histPhi_chi2 ->Fill(muon.phi(), mc_weight);
	      histPV_chi2  ->Fill(vtxs, mc_weight);

	      histNormChi2_chi2->Fill(muon.globalTrack()->normalizedChi2(), mc_weight);
	      histMatchSt_chi2->Fill(muon.numberOfMatchedStations(), mc_weight);
	      histMuValidHits_chi2->Fill(muon.globalTrack()->hitPattern().numberOfValidMuonHits(), mc_weight);
	      histDxy_chi2->Fill(muon.muonBestTrack()->dxy(PV_position), mc_weight);
	      histDz_chi2->Fill(muon.muonBestTrack()->dz(PV_position), mc_weight);
	      histPixelHits_chi2->Fill(muon.innerTrack()->hitPattern().numberOfValidPixelHits(), mc_weight);
	      histTrackerL_chi2->Fill(muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), mc_weight);
	      histDeltaPtOverPt_chi2->Fill(muon.globalTrack()->ptError()/muon.globalTrack()->pt(), mc_weight);
	   
	    }

	    // N- mst
	    if( muon.isPFMuon() &&  
		muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5 &&
		muon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0 && 
		muon.globalTrack()->hitPattern().numberOfValidMuonHits() > 0 && 
		fabs(muon.dB()) < 0.2 &&  
		fabs(muon.muonBestTrack()->dz(PV_position)) < 0.5 &&     	  
		muon.globalTrack()->normalizedChi2() < 10 && 
		//muon.numberOfMatchedStations() > 1 &&
		//( muon.numberOfMatchedStations() > 1 || (muon.numberOfMatchedStations() == 1 && !(muon.stationMask() == 1 || muon.stationMask() == 16)) || (muon.numberOfMatchedStations() == 1 && (muon.stationMask() == 1 || muon.stationMask() == 16) && muon.numberOfMatchedRPCLayers() > 2)) &&
		iso < Isolation 
		){

	      histPt_mst  ->Fill(muon.pt(), mc_weight);  
	      histEta_mst ->Fill(muon.eta(), mc_weight);
	      histPhi_mst ->Fill(muon.phi(), mc_weight);
	      histPV_mst  ->Fill(vtxs, mc_weight);

	      histNormChi2_mst->Fill(muon.globalTrack()->normalizedChi2(), mc_weight);
	      histMatchSt_mst->Fill(muon.numberOfMatchedStations(), mc_weight);
	      histMuValidHits_mst->Fill(muon.globalTrack()->hitPattern().numberOfValidMuonHits(), mc_weight);
	      histDxy_mst->Fill(muon.muonBestTrack()->dxy(PV_position), mc_weight);
	      histDz_mst->Fill(muon.muonBestTrack()->dz(PV_position), mc_weight);
	      histPixelHits_mst->Fill(muon.innerTrack()->hitPattern().numberOfValidPixelHits(), mc_weight);
	      histTrackerL_mst->Fill(muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), mc_weight);
	      histDeltaPtOverPt_mst->Fill(muon.globalTrack()->ptError()/muon.globalTrack()->pt(), mc_weight);
	   
	    }

	    // N- iso
	    if( muon.isPFMuon() &&  
		muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5 &&
		muon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0 && 
		muon.globalTrack()->hitPattern().numberOfValidMuonHits() > 0 && 
		fabs(muon.dB()) < 0.2 &&  
		fabs(muon.muonBestTrack()->dz(PV_position)) < 0.5 &&     	  
		muon.globalTrack()->normalizedChi2() < 10 && 
		muon.numberOfMatchedStations() > 1 //&&
		//( muon.numberOfMatchedStations() > 1 || (muon.numberOfMatchedStations() == 1 && !(muon.stationMask() == 1 || muon.stationMask() == 16)) || (muon.numberOfMatchedStations() == 1 && (muon.stationMask() == 1 || muon.stationMask() == 16) && muon.numberOfMatchedRPCLayers() > 2)) //&&
		//iso < Isolation 
		){

	      histPt_iso  ->Fill(muon.pt(), mc_weight);  
	      histEta_iso ->Fill(muon.eta(), mc_weight);
	      histPhi_iso ->Fill(muon.phi(), mc_weight);
	      histPV_iso  ->Fill(vtxs, mc_weight);
	    
	      histNormChi2_iso->Fill(muon.globalTrack()->normalizedChi2(), mc_weight);
	      histMatchSt_iso->Fill(muon.numberOfMatchedStations(), mc_weight);
	      histMuValidHits_iso->Fill(muon.globalTrack()->hitPattern().numberOfValidMuonHits(), mc_weight);
	      histDxy_iso->Fill(muon.muonBestTrack()->dxy(PV_position), mc_weight);
	      histDz_iso->Fill(muon.muonBestTrack()->dz(PV_position), mc_weight);
	      histPixelHits_iso->Fill(muon.innerTrack()->hitPattern().numberOfValidPixelHits(), mc_weight);
	      histTrackerL_iso->Fill(muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), mc_weight);
	      histDeltaPtOverPt_iso->Fill(muon.globalTrack()->ptError()/muon.globalTrack()->pt(), mc_weight);
	    
	    }


	    //tight selection
	    if(muon.isTightMuon(PV)){
	      histTightMuons_pt  ->Fill(muon.pt(), mc_weight);  
	      histTightMuons_eta ->Fill(muon.eta(), mc_weight);
	      histTightMuons_phi ->Fill(muon.phi(), mc_weight);
	      histTightMuons_pv  ->Fill(vtxs, mc_weight);
	      histTightMuons_timingVeto->Fill(veto, mc_weight);
	      //iso study
	      histTightMuons_pfiso->Fill(iso, mc_weight);
	      histTightMuons_tbiso->Fill(muon.isolationR03().sumPt/muon.innerTrack()->pt(), mc_weight);
	      
	      rwhistTightMuons_pt  ->Fill(muon.pt(), MyWeight);  
	      rwhistTightMuons_eta ->Fill(muon.eta(), MyWeight);
	      rwhistTightMuons_phi ->Fill(muon.phi(), MyWeight);
	      rwhistTightMuons_pv  ->Fill(vtxs, MyWeight);
	      //iso study
	      rwhistTightMuons_pfiso->Fill(iso, MyWeight);
	      rwhistTightMuons_tbiso->Fill(muon.isolationR03().sumPt/muon.innerTrack()->pt(), MyWeight);
	    

	      if(iso < Isolation){  //0.25 studiare
		histIsoTightMuons_pt  ->Fill(muon.pt(), mc_weight);  
		histIsoTightMuons_eta ->Fill(muon.eta(), mc_weight);
		histIsoTightMuons_phi ->Fill(muon.phi(), mc_weight);
		histIsoTightMuons_pv  ->Fill(vtxs, mc_weight);
		
		rwhistIsoTightMuons_pt  ->Fill(muon.pt(), MyWeight);  
		rwhistIsoTightMuons_eta ->Fill(muon.eta(), MyWeight);
		rwhistIsoTightMuons_phi ->Fill(muon.phi(), MyWeight);
		rwhistIsoTightMuons_pv  ->Fill(vtxs, MyWeight);

		//did the muon fire the trigger?
		const pat::TriggerObjectStandAloneCollection HLT_iso     =  muon.triggerObjectMatchesByPath(pathNames_lowIsoUnpr);
		const pat::TriggerObjectStandAloneCollection HLT_isoTk   =  muon.triggerObjectMatchesByPath(pathNames_lowIsoUnpr);
		
		bool trigger = false;
		if(HLT_iso.size() > 0 || HLT_isoTk.size() > 0)
		  trigger = true;
	      
		tight_muons.push_back(std::make_pair(muon,trigger));
		
	      }//iso
	    }//tight
	  	 	    
	   
	    //Medium muon selection
	    if( ( (MCtruth || runnumber > 278808) &&  muon.isMediumMuon()) || (!MCtruth && runnumber < 278808 && isMediumMuonBF(muon))){
	      histMediumMuons_pt  ->Fill(muon.pt(), mc_weight);  
	      histMediumMuons_eta ->Fill(muon.eta(), mc_weight);
	      histMediumMuons_phi ->Fill(muon.phi(), mc_weight);
	      histMediumMuons_pv  ->Fill(vtxs, mc_weight);
	      histMediumMuons_timingVeto->Fill(veto, mc_weight);
	      //iso study
	      histMediumMuons_pfiso->Fill(iso, mc_weight);
	      histMediumMuons_tbiso->Fill(muon.isolationR03().sumPt/muon.innerTrack()->pt(), mc_weight);
	      
	      rwhistMediumMuons_pt  ->Fill(muon.pt(), MyWeight);  
	      rwhistMediumMuons_eta ->Fill(muon.eta(), MyWeight);
	      rwhistMediumMuons_phi ->Fill(muon.phi(), MyWeight);
	      rwhistMediumMuons_pv  ->Fill(vtxs, MyWeight);
	      
	      //iso study
	      rwhistMediumMuons_pfiso->Fill(iso, MyWeight);
	      rwhistMediumMuons_tbiso->Fill(muon.isolationR03().sumPt/muon.innerTrack()->pt(), MyWeight);
	      
	      if(iso < Isolation){
		rwhistIsoMediumMuons_pt  ->Fill(muon.pt(), MyWeight);  
		rwhistIsoMediumMuons_eta ->Fill(muon.eta(), MyWeight);
		rwhistIsoMediumMuons_phi ->Fill(muon.phi(), MyWeight);
		rwhistIsoMediumMuons_pv  ->Fill(vtxs, MyWeight);
		
		histIsoMediumMuons_pt  ->Fill(muon.pt(), mc_weight);  
		histIsoMediumMuons_eta ->Fill(muon.eta(), mc_weight);
		histIsoMediumMuons_phi ->Fill(muon.phi(), mc_weight);
		histIsoMediumMuons_pv  ->Fill(vtxs, mc_weight);

		//did the muon fire the trigger?
		const pat::TriggerObjectStandAloneCollection HLT_iso     =  muon.triggerObjectMatchesByPath(pathNames_lowIsoUnpr);
		const pat::TriggerObjectStandAloneCollection HLT_isoTk   =  muon.triggerObjectMatchesByPath(pathNames_lowIsoUnpr);
		
		bool trigger = false;
		if(HLT_iso.size() > 0 || HLT_isoTk.size() > 0)
		  trigger = true;
		
		medium_muons.push_back(std::make_pair(muon,trigger));
		
	      }//iso
	    }//medium
	    
	  }//muon pt cut
	}//end loop on presel muons
	
	if(debug){
	  std::cout << " sel tight muons " << tight_muons.size() << std::endl;  
	  std::cout << " sel medium muons " << medium_muons.size() << std::endl;  
	}
      
	//select dimuon pair among the muons passing tightID
	bool triggerboth = false, triggerfirst = false, triggersecond = false;
	selectDimuonPair(tight_muons, muon_pair, triggerboth, triggerfirst, triggersecond);
	

	//select dimuon pair among the muons passing mediumID
       	bool triggerbothM = false, triggerfirstM = false, triggersecondM = false;
	selectDimuonPair(medium_muons, muon_pairMed, triggerbothM, triggerfirstM, triggersecondM);
	
	if(muon_pairMed.size()>1){
	  MEDIUM++;
	  TLorentzVector dimuonMed = buildDimuonPair(muon_pairMed, 1., 1.);
	  double massM = dimuonMed.M();
	  double ptM = dimuonMed.Pt();
	  double phiM = dimuonMed.Phi();
	  double etaM = dimuonMed.Eta();

	  histDeltaPhi_mediumMuons->Fill(deltaPhi(muon_pairMed[0].phi(), muon_pairMed[1].phi()));

	  if(massM > 50.){
	    histMediumDimuon_mass->Fill(massM, mc_weight);
	    histMediumDimuon_pt->Fill(ptM, mc_weight);
	    histMediumDimuon_eta->Fill(etaM, mc_weight);
	    histMediumDimuon_phi->Fill(phiM, mc_weight);
	    
	    rwhistMediumDimuon_mass->Fill(massM, MyWeight);
	    rwhistMediumDimuon_pt->Fill(ptM, MyWeight);
	    rwhistMediumDimuon_eta->Fill(etaM, MyWeight);
	    rwhistMediumDimuon_phi->Fill(phiM, MyWeight);
	    
	    
	    for(pat::METCollection::const_iterator met = mets->begin(); met != mets->end(); met++){
	      
	      //   if(met->pt() < PtMet){
	      // 	//std::cout << " MEDIUM " << eventNumber << std::endl;
	      // 	METM++;
	      // 	histMetDimuonM_mass->Fill(massM, mc_weight);
	      // 	rwhistMetDimuonM_mass->Fill(massM, MyWeight);
	      

	      std::vector<pat::Jet> cleanedJetsM;
	      for ( unsigned int i=0; i<jets->size(); ++i ) {
		const pat::Jet & jet = jets->at(i);
		
		if(!JetID_isTightLepVeto(jet) || (fabs(jet.eta()) > 2.4 && !JetID_isTight(jet))) 		
		  continue;	
		else
		  cleanedJetsM.push_back(jet);
		
	      }//end loop over jets
	      
	      //do also the muon cleaning by hand	  
	      CleanJetsFromMuons(cleanedJetsM,muon_pairMed,0.4);
	
	      for(size_t i=0; i<cleanedJetsM.size() ;i++){
		pat::Jet jet = cleanedJetsM[i];
		if(jet.pt() > PtJet && fabs(jet.eta()) < EtaJet){ 
		  if(jet.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags") > 0.8484)
		    ++nbJetsM;
		  else
		    ++nlightJetsM;
		}
	      }
	    
	      if(nbJetsM>0){
		if( met->pt() < 40){
		  CAT1M++;
		  
		  if(nbJetsM==1){
		    histCat11M_mass->Fill(massM, mc_weight);    
		    histCat11M_pt->Fill(ptM, mc_weight);
		    histCat11M_eta->Fill(etaM, mc_weight);
		    histCat11M_phi->Fill(phiM, mc_weight);
		    
		    rwhistCat11M_mass->Fill(massM, MyWeight);
		    rwhistCat11M_pt->Fill(ptM, MyWeight);
		    rwhistCat11M_eta->Fill(etaM, MyWeight);
		    rwhistCat11M_phi->Fill(phiM, MyWeight);
		  }

		  else if(nbJetsM>1){
		    histCat111M_mass->Fill(massM, mc_weight);    
		    histCat111M_pt->Fill(ptM, mc_weight);
		    histCat111M_eta->Fill(etaM, mc_weight);
		    histCat111M_phi->Fill(phiM, mc_weight);
		    
		    rwhistCat111M_mass->Fill(massM, MyWeight);
		    rwhistCat111M_pt->Fill(ptM, MyWeight);
		    rwhistCat111M_eta->Fill(etaM, MyWeight);
		    rwhistCat111M_phi->Fill(phiM, MyWeight);
		  }
		  
		}//met cat 1
	      }//jet cat 1
	    
	      else if(met->pt() < 80){
		CAT2M++;
		
		if(nlightJetsM<2){
		  histCat22M_mass->Fill(massM, mc_weight);    
		  histCat22M_pt->Fill(ptM, mc_weight);
		  histCat22M_eta->Fill(etaM, mc_weight);
		  histCat22M_phi->Fill(phiM, mc_weight);
		  
		  rwhistCat22M_mass->Fill(massM, MyWeight);
		  rwhistCat22M_pt->Fill(ptM, MyWeight);
		  rwhistCat22M_eta->Fill(etaM, MyWeight);
		  rwhistCat22M_phi->Fill(phiM, MyWeight);
		}
		else  if(nlightJetsM>1){
		  histCat222M_mass->Fill(massM, mc_weight);    
		  histCat222M_pt->Fill(ptM, mc_weight);
		  histCat222M_eta->Fill(etaM, mc_weight);
		  histCat222M_phi->Fill(phiM, mc_weight);
		  
		  rwhistCat222M_mass->Fill(massM, MyWeight);
		  rwhistCat222M_pt->Fill(ptM, MyWeight);
		  rwhistCat222M_eta->Fill(etaM, MyWeight);
		  rwhistCat222M_phi->Fill(phiM, MyWeight);
		}

	      }//met cat2
	      
	    }//met loop
	  }//mass > 50 
	}//mediumID
	
	
	if(debug)
	  std::cout << " muon pair size " << muon_pair.size() << std::endl;  
	
	//now select met and jet
	if(muon_pair.size()>1){
	  
	  //get muons scale factors
	  double triggerW = 1., idW = 1., isoW = 1.;
	  double iso1 = 1., iso2 = 1., id1 = 1., id2 = 1., tr1 = 1., tr2 = 1.;
	  if(MCtruth){
	    //0.55 del MC lo correggo con BF, il resto con GH
	    
	    // TRandom *R = new TRandom(84855);
	    // create a pointer to a new instance of TRandom in the heap
	    float Rndm = gRandom->Rndm();
	    if(Rndm < 0.55){
	      if(triggerboth){
		tr1 = GetScaleFactorsFromH(Bin2D_TRIGrunBF, Bin1D_TRIGrunBF, 500., muon_pair[0].eta(), muon_pair[0].pt());
		tr2 = GetScaleFactorsFromH(Bin2D_TRIGrunBF, Bin1D_TRIGrunBF, 500., muon_pair[1].eta(), muon_pair[1].pt());
		//tr1 -= tr1*0.005;
		//tr2 -= tr2*0.005;
	      }
	      else if(triggerfirst){
		tr1 = GetScaleFactorsFromH(Bin2D_TRIGrunBF, Bin1D_TRIGrunBF, 500., muon_pair[0].eta(), muon_pair[0].pt());
		//tr1 -= tr1*0.005;
	      }
	      else if(triggersecond){
		tr2 = GetScaleFactorsFromH(Bin2D_TRIGrunBF, Bin1D_TRIGrunBF, 500., muon_pair[1].eta(), muon_pair[1].pt());
		//tr2 -= tr2*0.005;
	      }
	  	  	    
	      id1 = GetScaleFactorsFromH(Bin2D_IDrunBF, Bin1D_IDrunBF, 120., muon_pair[0].eta(), muon_pair[0].pt());
	      id2 = GetScaleFactorsFromH(Bin2D_IDrunBF, Bin1D_IDrunBF, 120., muon_pair[1].eta(), muon_pair[1].pt());
	      
	      iso1 = GetScaleFactorsFromH(Bin2D_ISOrunBF, Bin1D_ISOrunBF, 120., muon_pair[0].eta(), muon_pair[0].pt());
	      iso2 = GetScaleFactorsFromH(Bin2D_ISOrunBF, Bin1D_ISOrunBF, 120., muon_pair[1].eta(), muon_pair[1].pt());
	      
	    } 
	    else { //45% of MC
	      if(triggerboth){
		tr1 = GetScaleFactorsFromH(Bin2D_TRIGrunGH, Bin1D_TRIGrunGH, 500., muon_pair[0].eta(), muon_pair[0].pt());
		tr2 = GetScaleFactorsFromH(Bin2D_TRIGrunGH, Bin1D_TRIGrunGH, 500., muon_pair[1].eta(), muon_pair[1].pt());
		//tr1 -= tr1*0.005;
		//tr2 -= tr2*0.005;
	      }
	      else if(triggerfirst){
		tr1 = GetScaleFactorsFromH(Bin2D_TRIGrunGH, Bin1D_TRIGrunGH, 500., muon_pair[0].eta(), muon_pair[0].pt());
		//tr1 -= tr1*0.005;
	      }
	      else if(triggersecond){
		tr2 = GetScaleFactorsFromH(Bin2D_TRIGrunGH, Bin1D_TRIGrunGH, 500., muon_pair[1].eta(), muon_pair[1].pt());
		//tr2 -= tr1*0.005;
	      }
	      
	      id1 = GetScaleFactorsFromH(Bin2D_IDrunGH, Bin1D_IDrunGH, 120., muon_pair[0].eta(), muon_pair[0].pt());
	      id2 = GetScaleFactorsFromH(Bin2D_IDrunGH, Bin1D_IDrunGH, 120., muon_pair[1].eta(), muon_pair[1].pt());
	      
	      iso1 = GetScaleFactorsFromH(Bin2D_ISOrunGH, Bin1D_ISOrunGH, 120., muon_pair[0].eta(), muon_pair[0].pt());
	      iso2 = GetScaleFactorsFromH(Bin2D_ISOrunGH, Bin1D_ISOrunGH, 120., muon_pair[1].eta(), muon_pair[1].pt());
	    } 
	  }
	    
	  triggerW = tr1*tr2;
	  //triggerW = (tr1+tr1*0.005)*(tr2+tr2*0.005);

	  idW = id1*id2;
	  //idW = (id1 + id1*0.01) * (id2 + id2*0.01);
	  
	  isoW = iso1*iso2;
	  //isoW = (iso1 + iso1*0.005) * (iso2 + iso2*0.005);

	  //update the event SF
	  mc_weight *= (triggerW*idW*isoW);

	  MyWeight *= (triggerW*idW*isoW);
	  MyWeightUp *= (triggerW*idW*isoW);
	  MyWeightDown *= (triggerW*idW*isoW);
	  
	  //get the mass of dimuon pair from tight muons
	  TLorentzVector dimuon = buildDimuonPair(muon_pair, 1., 1.);
	  double mass = dimuon.M();
	  double pt = dimuon.Pt();
	  double phi = dimuon.Phi();
	  double eta = dimuon.Eta();
	  	  
	  histDeltaPhi_tightMuons->Fill(deltaPhi(muon_pair[0].phi(), muon_pair[1].phi()));
	  	  
	  //calculate the rochester muon momentum correction
	  double rochesterSF1 = getRochester(muon_pair[0], MCtruth, crab, local_rc, crab_rc);
	  double rochesterSF2 = getRochester(muon_pair[1], MCtruth, crab, local_rc, crab_rc);
	 
 	
	  //get the mass corrected with rochester
	  TLorentzVector dimuonr = buildDimuonPair(muon_pair, rochesterSF1, rochesterSF2);
	  double massr = dimuonr.M();
	  double ptr = dimuonr.Pt();
	  double phir = dimuonr.Phi();
	  double etar = dimuonr.Eta();
	  

	  if(debug){
	    std::cout << "Dimuon: mass = " << mass << ", pt = " << pt << ", eta = " << eta << ", phi = " << phi << std::endl;
	    std::cout << "Dimuon + roch: mass = " << massr << ", pt = " << ptr << ", eta = " << etar << ", phi = " << phir << std::endl;
	  }

	  
	  if(mass > 50){ //fill histos before the MET cut
	    histTightDimuon_mass->Fill(mass, mc_weight);
	    histTightDimuon_pt->Fill(pt, mc_weight);
	    histTightDimuon_eta->Fill(eta, mc_weight);
	    histTightDimuon_phi->Fill(phi, mc_weight);
	    
	    rwhistTightDimuon_mass->Fill(mass, MyWeight);
	    rwhistTightDimuon_pt->Fill(pt, MyWeight);
	    rwhistTightDimuon_eta->Fill(eta, MyWeight);
	    rwhistTightDimuon_phi->Fill(phi, MyWeight);
	    
	    histTightDimuonRoc_mass->Fill(massr, mc_weight);
	    histTightDimuonRoc_pt->Fill(ptr, mc_weight);
	    histTightDimuonRoc_eta->Fill(etar, mc_weight);
	    histTightDimuonRoc_phi->Fill(phir, mc_weight);
	    
	    rwhistTightDimuonRoc_mass->Fill(massr, MyWeight);
	    rwhistTightDimuonRoc_pt->Fill(ptr, MyWeight);
	    rwhistTightDimuonRoc_eta->Fill(etar, MyWeight);
	    rwhistTightDimuonRoc_phi->Fill(phir, MyWeight);

	    
	    
	    for(pat::METCollection::const_iterator met = mets->begin(); met != mets->end(); met++){
	      //   if(met->pt() < PtMet){
	      // 	MET++;
	      // 	Met = true;
	      //   }
	      
	      
	      
	      histPFMet_pt->Fill(met->pt(), mc_weight);
	      histPFMet_phi->Fill(met->phi(), mc_weight);
	      
	      
	      rwhistPFMet_pt->Fill(met->pt(), MyWeight);
	      rwhistPFMet_phi->Fill(met->phi(), MyWeight);
	      
	      if(mass >= 70){
		histPFMet_70toInf_pt->Fill(met->pt(), mc_weight);
		histPFMet_70toInf_phi->Fill(met->phi(), mc_weight);
		
		
		rwhistPFMet_70toInf_pt->Fill(met->pt(), MyWeight);
		rwhistPFMet_70toInf_phi->Fill(met->phi(), MyWeight);
		
		if(mass <= 110){
		  //syncronization
		  // std::cout << " Run " << iEvent.run() << " Lumi " << iEvent.getLuminosityBlock().luminosityBlock() << " Num " << iEvent.eventAuxiliary().event() << std::endl;
		  // std::cout <<  std::endl;
		  // std::cout<<" MC Weight = " << mc_weight << "  PU Weight = " << MyWeight/mc_weight<< std::endl;
		  // std::cout <<  std::endl;
		  // std::cout << " MET " << met->pt()<< " MET + type1 = " << met->corPt(pat::MET::Type1) << " MET + type01 = " << met->corPt(pat::MET::Type01) << " uncorrected MET = " << met->uncorPt()<< std::endl;  
		  // std::cout << " MET + type1smear = " << met->corPt(pat::MET::Type1Smear) << " MET + type01 smear = " << met->corPt(pat::MET::Type01Smear) << std::endl;  
		  
		  // std::cout <<  std::endl;
		  
		  // std::cout << " MET " << met->pt()<< " met JetPtUp = " << met->shiftedPt(pat::MET::JetEnUp) << " met JetPtDown = " << met->shiftedPt(pat::MET::JetEnDown)<< std::endl;  
		  // std::cout << " MET " << met->pt()<< " met JetResUp = " << met->shiftedPt(pat::MET::JetResUp) << " met JetResDown = " << met->shiftedPt(pat::MET::JetResDown)<< std::endl;  
		  // std::cout << " MET " << met->pt()<< " met UnclusterUp = " << met->shiftedPt(pat::MET::UnclusteredEnUp) << " met UnclusterDown = " << met->shiftedPt(pat::MET::UnclusteredEnDown)<< std::endl;  
		  // std::cout << " MET " << met->pt()<< " met TauUp = " << met->shiftedPt(pat::MET::TauEnUp) << " met TauDown = " << met->shiftedPt(pat::MET::TauEnDown)<< std::endl;  
		  // std::cout << " MET " << met->pt()<< " met EleUp = " << met->shiftedPt(pat::MET::ElectronEnUp) << " met EleDown = " << met->shiftedPt(pat::MET::ElectronEnDown)<< std::endl;  
		  // std::cout << " MET " << met->pt()<< " met PhotonUp = " << met->shiftedPt(pat::MET::PhotonEnUp) << " met PhotonDown = " << met->shiftedPt(pat::MET::PhotonEnDown)<< std::endl;  
		  // std::cout << " MET " << met->pt()<< " met MuUp = " << met->shiftedPt(pat::MET::MuonEnUp) << " met MuDown = " << met->shiftedPt(pat::MET::MuonEnDown)<< std::endl;  
		  // std::cout <<  std::endl;  
		  // std::cout <<  std::endl;
		  
		  histPFMet_70to110_pt->Fill(met->pt(), mc_weight);
		 	  
		  rwhistPFMet_70to110_pt->Fill(met->pt(), MyWeight);
		 	  
		}
		else if(mass <= 150){
		  histPFMet_110to150_pt->Fill(met->pt(), mc_weight);
		 	  
		  rwhistPFMet_110to150_pt->Fill(met->pt(), MyWeight);
		}
		
	      }
	      
	      if(debug){
		std::cout << " MET " << met->pt()<< " correction type1 = " << met->corPt(pat::MET::Type1) << " correction type01 = " << met->corPt(pat::MET::Type01) << " uncorrect pt = " << met->uncorPt()<< std::endl;  
		std::cout << " MET " << met->pt()<< " correction type1smear = " << met->corPt(pat::MET::Type1Smear) << " correction type01 smear = " << met->corPt(pat::MET::Type01Smear) << std::endl;  
		
		std::cout <<  std::endl;
		
		
	      } 
	      
	      
	      	      	     
	      std::vector<pat::Jet> cleanedJets;
	      for ( unsigned int i=0; i<jets->size(); ++i ) {
		const pat::Jet & jet = jets->at(i);
	      	
		if(!JetID_isTightLepVeto(jet) || (fabs(jet.eta()) > 2.4 && !JetID_isTight(jet))) 		
		  continue;	
		else
		  cleanedJets.push_back(jet);
				
	      }//end loop over jets
	      
	      
	      //do also the muon cleaning by hand	  
	      CleanJetsFromMuons(cleanedJets,muon_pair,0.4);
	      	      
	      double True_bJets = 0.;
	      for(size_t i=0; i<cleanedJets.size() ;i++){
		pat::Jet jet = cleanedJets[i];
			      
		//////////////////// fill the jet histos only for those events passing the MET selection
		//if(met->pt() < PtMet){
		
		double bDisc = jet.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"); 
		histPFJets_bDisc->Fill(bDisc, mc_weight);
		histPFJets_pt->Fill(jet.pt(), mc_weight);   
		histPFJets_eta->Fill(jet.eta(), mc_weight);  
		histPFJets_phi->Fill(jet.phi(), mc_weight);
		histPFJets_deltaPhi->Fill(deltaPhi(phi, jet.phi()), mc_weight);
		
		rwhistPFJets_bDisc->Fill(bDisc, MyWeight);
		rwhistPFJets_pt->Fill(jet.pt(), MyWeight);   
		rwhistPFJets_eta->Fill(jet.eta(), MyWeight);  
		rwhistPFJets_phi->Fill(jet.phi(), MyWeight);
		rwhistPFJets_deltaPhi->Fill(deltaPhi(phi, jet.phi()), MyWeight);
		

		jecUnc->setJetEta(jet.eta());
		jecUnc->setJetPt(jet.pt()); // here you must use the CORRECTED jet pt
		float JECunc = jecUnc->getUncertainty(true);
		float jetpt = jet.pt();//+JECunc*jet.pt();	
		//std::cout << JECunc << " pt " <<jet.pt() << "  " << jetpt << std::endl; 	
		//option 1
		if( jetpt  > PtJet && fabs(jet.eta()) < EtaJet){
		  ++nJets;
		  //++Jet;
		  histCuttedJets_bDisc->Fill(bDisc, mc_weight);
		  histCuttedJets_pt->Fill(jet.pt(), mc_weight);   
		  histCuttedJets_eta->Fill(jet.eta(), mc_weight);  
		  histCuttedJets_phi->Fill(jet.phi(), mc_weight);
		  
		  rwhistCuttedJets_bDisc->Fill(bDisc, MyWeight);
		  rwhistCuttedJets_pt->Fill(jet.pt(), MyWeight);   
		  rwhistCuttedJets_eta->Fill(jet.eta(), MyWeight);  
		  rwhistCuttedJets_phi->Fill(jet.phi(), MyWeight);
		    
		  
		  //if(iEvent.getLuminosityBlock().luminosityBlock() == 22 && iEvent.eventAuxiliary().event() == 3859)
		  //std::cout << "**jet " << " pt " << jet.pt() << " b = " <<  bDisc << std::endl;	

		  if(bDisc > 0.8484){
		    ++nbJets;
		  		    
		    if(MCtruth){
		      for(size_t i=0; i<pruned->size();i++){
			if( (*pruned)[i].status()== 71 && fabs((*pruned)[i].pdgId())==5 && deltaR(jet.eta(),jet.phi(), (*pruned)[i].eta(), (*pruned)[i].phi()) < 0.1)			
			  True_bJets++;
			//std::cout << "quark b: PdgID = " << (*pruned)[i].pdgId() << ", status " << (*pruned)[i].status() << "  pt = " << (*pruned)[i].pt() << ", eta = " << (*pruned)[i].eta() << ", phi = " << (*pruned)[i].phi() << std::endl;
		      }	
		    }//if MC
		    
		    // double jet_scalefactor    = reader.eval(BTagEntry::FLAV_B, jet.eta(), jet.pt()); 
		    // double jet_scalefactor_up = 1.;//reader.eval_auto_bounds("up", BTagEntry::FLAV_B, jet.eta(), jet.pt());
		    // double jet_scalefactor_do = 2;//reader.eval_auto_bounds("down", BTagEntry::FLAV_B, jet.eta(), jet.pt()); 
		    // std::cout << " Btag SF " << jet_scalefactor << " Up " << jet_scalefactor_up << " Down " << jet_scalefactor_do << std::endl;
		    
		    histBJets_pt->Fill(jet.pt(), mc_weight);   
		    histBJets_eta->Fill(jet.eta(), mc_weight);  
		    histBJets_phi->Fill(jet.phi(), mc_weight);   
		    histBJets_deltaPhi->Fill(deltaPhi(phi, jet.phi()), mc_weight);
		    
		    rwhistBJets_pt->Fill(jet.pt(), MyWeight);   
		    rwhistBJets_eta->Fill(jet.eta(), MyWeight);  
		    rwhistBJets_phi->Fill(jet.phi(), MyWeight);   
		    rwhistBJets_deltaPhi->Fill(deltaPhi(phi, jet.phi()), MyWeight);
		    
		  }
		  else{
		    ++nlightJets;
		 
		    histLightJets_pt->Fill(jet.pt(), mc_weight);   
		    histLightJets_eta->Fill(jet.eta(), mc_weight);  
		    histLightJets_phi->Fill(jet.phi(), mc_weight);   
		    histLightJets_deltaPhi->Fill(deltaPhi(phi, jet.phi()), mc_weight);
		    
		    rwhistLightJets_pt->Fill(jet.pt(), MyWeight);   
		    rwhistLightJets_eta->Fill(jet.eta(), MyWeight);  
		    rwhistLightJets_phi->Fill(jet.phi(), MyWeight);   
		    rwhistLightJets_deltaPhi->Fill(deltaPhi(phi, jet.phi()), MyWeight);


		  }
		  
		}
		
	      }//end loop over jet
	      
	      histTrue_bJets->Fill(True_bJets);
	      histPFJets_size->Fill(nJets, mc_weight);
	      histLightJets_size->Fill(nlightJets, mc_weight);
	      histBJets_size->Fill(nbJets, mc_weight);
	      
	      rwhistPFJets_size->Fill(nJets, MyWeight);
	      rwhistLightJets_size->Fill(nlightJets, MyWeight);
	      rwhistBJets_size->Fill(nbJets, MyWeight);
	       
	    	      
	      
	      
	      //}//end met loop
	    
	    
	      // if(Met){ //fill dimuon quantities for events passing MET cut
	      //   histMetDimuon_mass->Fill(mass, mc_weight);
	      //   histMetDimuon_pt->Fill(pt, mc_weight);
	      //   histMetDimuon_eta->Fill(eta, mc_weight);
	      //   histMetDimuon_phi->Fill(phi, mc_weight);
	      
	      //   rwhistMetDimuon_mass->Fill(mass, MyWeight);
	      //   rwhistMetDimuon_pt->Fill(pt, MyWeight);
	      //   rwhistMetDimuon_eta->Fill(eta, MyWeight);
	      //   rwhistMetDimuon_phi->Fill(phi, MyWeight);
	     	     
	      
	      if(nbJets > 0){ //cat1
		if(met->pt()<40){
		  CAT1++;	
		  CAT1_mcpuweighted += MyWeight;
		  CAT1_mcpuweightedup += MyWeightUp;
		  CAT1_mcpuweighteddown += MyWeightDown;
		  //std::cout << " Run " << iEvent.run() << " Lumi " << iEvent.getLuminosityBlock().luminosityBlock() << " Num " << iEvent.eventAuxiliary().event() << std::endl;	
		  if(debug)
		    std::cout << " Mass cat 1 = " <<  mass << std::endl;	
		  
		  histCat1_mass->Fill(mass, mc_weight);
		  histCat1_pt->Fill(pt, mc_weight);
		  histCat1_eta->Fill(eta, mc_weight);
		  histCat1_phi->Fill(phi, mc_weight);
		  rwhistCat1_mass->Fill(mass, MyWeight);
		  rwhistCat1_pt->Fill(pt, MyWeight);
		  rwhistCat1_eta->Fill(eta, MyWeight);
		  rwhistCat1_phi->Fill(phi, MyWeight);		  
		  
		  MassCat1=mass;
		  if(mass>100.) masstree1_->Fill(); 
		  Minv_Cat1->Fill(mass, mc_weight);		  
		  // std::cout << " TIGHT Cat1 " << eventNumber << std::endl;
		  
		  //plot the quantities per muon full cat1
		  for(unsigned int i = 0; i < muon_pair.size(); i++){
		    pat::Muon muon = muon_pair[i];
		    
		    histCat1Muons_pt  ->Fill(muon.pt(), mc_weight);  
		    histCat1Muons_eta ->Fill(muon.eta(), mc_weight);
		    histCat1Muons_phi ->Fill(muon.phi(), mc_weight);
		    histCat1Muons_pv  ->Fill(vtxs, mc_weight);
		    
		    rwhistCat1Muons_pt  ->Fill(muon.pt(), MyWeight);  
		    rwhistCat1Muons_eta ->Fill(muon.eta(), MyWeight);
		    rwhistCat1Muons_phi ->Fill(muon.phi(), MyWeight);
		    rwhistCat1Muons_pv  ->Fill(vtxs, MyWeight);
		  }
		  
		  if(nbJets == 1){
		    histCat11_mass->Fill(mass, mc_weight);
		    histCat11_pt->Fill(pt, mc_weight);
		    histCat11_eta->Fill(eta, mc_weight);
		    histCat11_phi->Fill(phi, mc_weight);
		    rwhistCat11_mass->Fill(mass, MyWeight);
		    rwhistCat11_pt->Fill(pt, MyWeight);
		    rwhistCat11_eta->Fill(eta, MyWeight);
		    rwhistCat11_phi->Fill(phi, MyWeight);
		    
		    MassCat11=mass;
		    if(mass>100.) masstree11_->Fill(); 
		    Minv_Cat11->Fill(mass, mc_weight);
		    
		  }
		  else if(nbJets > 1){
		    histCat111_mass->Fill(mass, mc_weight);
		    histCat111_pt->Fill(pt, mc_weight);
		    histCat111_eta->Fill(eta, mc_weight);
		    histCat111_phi->Fill(phi, mc_weight);
		    rwhistCat111_mass->Fill(mass, MyWeight);
		    rwhistCat111_pt->Fill(pt, MyWeight);
		    rwhistCat111_eta->Fill(eta, MyWeight);
		    rwhistCat111_phi->Fill(phi, MyWeight);
		    
		    MassCat111=mass;
		    if(mass>100.) masstree111_->Fill(); 
		    Minv_Cat111->Fill(mass, mc_weight);
		    
		  }
		}//met cut
	      }//bjet
	      else if(met->pt() < 80){ //cat2 no bjet and....
		CAT2++;
		CAT2_mcpuweighted += MyWeight;
		CAT2_mcpuweightedup += MyWeightUp;
		CAT2_mcpuweighteddown += MyWeightDown;
		//std::cout << " TIGHT Cat2 " << eventNumber << std::endl;
		
		histCat2_mass->Fill(mass, mc_weight);    
		histCat2_pt->Fill(pt, mc_weight);
		histCat2_eta->Fill(eta, mc_weight);
		histCat2_phi->Fill(phi, mc_weight);
		
		rwhistCat2_mass->Fill(mass, MyWeight);
		rwhistCat2_pt->Fill(pt, MyWeight);
		rwhistCat2_eta->Fill(eta, MyWeight);
		rwhistCat2_phi->Fill(phi, MyWeight);

		MassCat2=mass;
		if(mass>100.) masstree2_->Fill();
		Minv_Cat2->Fill(mass, mc_weight);

		for(unsigned int i = 0; i < muon_pair.size(); i++){
		  pat::Muon muon = muon_pair[i];
	      
		  histCat2Muons_pt  ->Fill(muon.pt(), mc_weight);  
		  histCat2Muons_eta ->Fill(muon.eta(), mc_weight);
		  histCat2Muons_phi ->Fill(muon.phi(), mc_weight);
		  histCat2Muons_pv  ->Fill(vtxs, mc_weight);
		  	  
		  rwhistCat2Muons_pt  ->Fill(muon.pt(), MyWeight);  
		  rwhistCat2Muons_eta ->Fill(muon.eta(), MyWeight);
		  rwhistCat2Muons_phi ->Fill(muon.phi(), MyWeight);
		  rwhistCat2Muons_pv  ->Fill(vtxs, MyWeight);
		}


		if(nlightJets<2){
		  histCat22_mass->Fill(mass, mc_weight);    
		  histCat22_pt->Fill(pt, mc_weight);
		  histCat22_eta->Fill(eta, mc_weight);
		  histCat22_phi->Fill(phi, mc_weight);
		  
		  rwhistCat22_mass->Fill(mass, MyWeight);
		  rwhistCat22_pt->Fill(pt, MyWeight);
		  rwhistCat22_eta->Fill(eta, MyWeight);
		  rwhistCat22_phi->Fill(phi, MyWeight);
		  
		  MassCat22=mass;
		  if(mass>100.) masstree22_->Fill();
		  Minv_Cat22->Fill(mass, mc_weight);
		  
		}
		else if(nlightJets>1){
		  histCat222_mass->Fill(mass, mc_weight);    
		  histCat222_pt->Fill(pt, mc_weight);
		  histCat222_eta->Fill(eta, mc_weight);
		  histCat222_phi->Fill(phi, mc_weight);
		  
		  rwhistCat222_mass->Fill(mass, MyWeight);
		  rwhistCat222_pt->Fill(pt, MyWeight);
		  rwhistCat222_eta->Fill(eta, MyWeight);
		  rwhistCat222_phi->Fill(phi, MyWeight);
		  
		  MassCat222=mass;
		  if(mass>100.) masstree222_->Fill();
		  Minv_Cat222->Fill(mass, mc_weight);
		}
		
		
		
		if(debug)
		  std::cout << " Mass cat 2 = " <<  mass << std::endl;
	      }
	      
	      
	    }//end met loop
	   
	  }//end cut for m>50
	}//end if dimuon ok
      
	  

	//////////////////////////////////////////////////////////////////////////
	///////////////////////////// Trigger Further Info//////////////////////// 
	//////////////////////////////////////////////////////////////////////////
        
	const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);
	
	if(Trigger_debug)  
	  std::cout << "\n === TRIGGER PATHS === " << std::endl;
	
	for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i) {
	  if(triggerBits->accept(i)){
	    
	    if(Trigger_debug){   
	      std::cout << "Trigger " << names.triggerName(i) << 
		", prescale " << triggerPrescales->getPrescaleForIndex(i) <<
		": " << (triggerBits->accept(i) ? "PASS" : "fail (or not run)") << std::endl;
	      
	      std::cout << "\n === TRIGGER OBJECTS === " << std::endl;
	      std::cout << "\n === TRIGGER Fired === " << std::endl;
	      
	      
	      //unpack the trigger object to use it with muon collection 
	      for (pat::TriggerObjectStandAlone obj : *triggerObjects) { // note: not "const &" since we want to call unpackPathNames
		obj.unpackPathNames(names);
		
		// Print trigger object collection and type
		std::cout << "\tTrigger object:  pt " << obj.pt() << ", eta " << obj.eta() << ", phi " << obj.phi() << std::endl;
		std::cout << "\t   Collection: " << obj.collection() << std::endl;
		
		std::cout << "\t   Type IDs:   ";
		// Print associated trigger filters
		for (unsigned h = 0; h < obj.filterIds().size(); ++h) std::cout << " " << obj.filterIds()[h] ;
		
		std::cout << "\t   Filters:    ";
		for (unsigned h = 0; h < obj.filterLabels().size(); ++h) std::cout << " " << obj.filterLabels()[h];
		std::cout << std::endl;
		std::vector<std::string> pathNamesAll  = obj.pathNames(false);
		std::vector<std::string> pathNamesLast = obj.pathNames(true);
		// Print all trigger paths, for each one record also if the object is associated to a 'l3' filter (always true for the
		// definition used in the PAT trigger producer) and if it's associated to the last filter of a successfull path (which
		// means that this object did cause this trigger to succeed; however, it doesn't work on some multi-object triggers)
		std::cout << "\t   Paths (" << pathNamesAll.size()<<"/"<<pathNamesLast.size()<<"):    ";
		for (unsigned h = 0, n = pathNamesAll.size(); h < n; ++h) {
		  bool isBoth = obj.hasPathName( pathNamesAll[h], true, true ); 
		  bool isL3   = obj.hasPathName( pathNamesAll[h], false, true ); 
		  bool isLF   = obj.hasPathName( pathNamesAll[h], true, false ); 
		  bool isNone = obj.hasPathName( pathNamesAll[h], false, false ); 
		  std::cout << "   " << pathNamesAll[h];
		  if (isBoth) std::cout << "(L,3)";
		  if (isL3 && !isBoth) std::cout << "(*,3)";
		  if (isLF && !isBoth) std::cout << "(L,*)";
		  if (isNone && !isBoth && !isL3 && !isLF) std::cout << "(*,*)";
		}
	      
	      }//Trigger obj
	    }//Trigger debug
	  }//end path trigger selection
	}//end loop on trigger bits
	
	//fill the tree with all the events
	//unselected events have mass = 0 (per gianni)
	masstree_->Fill();
       	
      }//end youwantAnalysis
      
      
      /////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////  
      
    }//end Muon PreSelection 
    
  }//end met filters  
  //vectors cleaning 
  
  clear_Arrays();  
       
}//end of analyze
  


double MiniAnalyzer::getRochester(pat::Muon muon,  bool MCtruth, bool crab, const RoccoR local_rc,  const RoccoR crab_rc){
  
  double rochesterSF = 1.;
  float fRand1 = gRandom->Rndm();
  float fRand2 = gRandom->Rndm();
  
  if(!MCtruth){
    //for each data muon in the loop, use this function to get a scale factor for its momentum:
    if(!crab)
      rochesterSF = local_rc.kScaleDT(muon.charge(), muon.pt(), muon.eta(), muon.phi(),0,0);
    else
      rochesterSF = crab_rc.kScaleDT(muon.charge(), muon.pt(), muon.eta(), muon.phi(),0,0);
  }
  else if(muon.innerTrack().isAvailable()){//MC case
    
    double genPt = -99.;
    std::vector<reco::GenParticleRef> associatedGenParticles = muon.genParticleRefs();
    for ( std::vector<reco::GenParticleRef>::const_iterator it = associatedGenParticles.begin(); it != associatedGenParticles.end(); ++it ) {
      if ( it->isAvailable() ) {
	const reco::GenParticleRef& genParticle = (*it);
	if ( fabs(genParticle->pdgId()) == 13 && genParticle->status() == 1)
	  genPt = genParticle->pt();
      }
    }
    
    if(genPt > 0){ 
      if(!crab)
	rochesterSF = local_rc.kScaleFromGenMC(muon.charge(), muon.pt(), muon.eta(), muon.phi(), muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), genPt, fRand1, 0, 0);
      else
	rochesterSF = crab_rc.kScaleFromGenMC(muon.charge(), muon.pt(), muon.eta(), muon.phi(), muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), genPt, fRand1, 0, 0);
    }
    else{
      if(!crab)
	rochesterSF = local_rc.kScaleAndSmearMC(muon.charge(), muon.pt(), muon.eta(), muon.phi(), muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), fRand1, fRand2, 0, 0);
      else
	rochesterSF = crab_rc.kScaleAndSmearMC(muon.charge(), muon.pt(), muon.eta(), muon.phi(), muon.innerTrack()->hitPattern().trackerLayersWithMeasurement(), fRand1, fRand2, 0, 0);
    }
  }  
  
  return rochesterSF;
}



TLorentzVector MiniAnalyzer::buildDimuonPair(std::vector<pat::Muon> muon_pair,  double k1=1., double k2=1.){
  TLorentzVector mu1, mu2, dimuon;
  float muPDG = 0.105658;
  mu1.SetPtEtaPhiM(muon_pair[0].pt()*k1, muon_pair[0].eta(), muon_pair[0].phi(), muPDG);
  mu2.SetPtEtaPhiM(muon_pair[1].pt()*k2, muon_pair[1].eta(), muon_pair[1].phi(), muPDG);
  
  return  dimuon = mu1+mu2;
}

void MiniAnalyzer::selectDimuonPair(std::vector<std::pair <pat::Muon,bool> > tight_muons, std::vector<pat::Muon> &muon_pair, bool &triggerboth, bool &triggerfirst, bool &triggersecond){
  if(tight_muons.size()==2){
 
    //std::cout << " SELECTED Muon 1 of the pair: charge " << tight_muons[0].first.charge() << "  trigger " << tight_muons[0].second << std::endl; 
    //std::cout << " SELECTED Muon 2 of the pair: charge " << tight_muons[1].first.charge() << "  trigger " << tight_muons[1].second << std::endl; 
    
    if(tight_muons[0].first.charge()*tight_muons[1].first.charge() < 0 && (tight_muons[0].second ||tight_muons[1].second)){
      muon_pair.push_back(tight_muons[0].first);
      muon_pair.push_back(tight_muons[1].first);
      
      

      if(tight_muons[0].second && tight_muons[1].second)
	triggerboth = true;
      else if(tight_muons[0].second)
	triggerfirst = true;
      else if(tight_muons[1].second)
	triggersecond = true;
    }
  }
  else if(tight_muons.size()>2){
    for(size_t s=0; s<tight_muons.size(); s++){
      bool pair = false;
      
      for(size_t t=s+1; t<tight_muons.size(); t++){
	
	int charge = tight_muons[s].first.charge()*tight_muons[t].first.charge();
	bool trigger = false;
	if(tight_muons[s].second ||tight_muons[t].second)
	  trigger = true;
	
	if(charge < 0 && trigger){
	  pair = true;
	  muon_pair.push_back(tight_muons[s].first);
	  muon_pair.push_back(tight_muons[t].first);
	  if(tight_muons[t].second && tight_muons[s].second)
	    triggerboth = true;
	  else if(tight_muons[s].second)
	    triggerfirst = true;
	  else if(tight_muons[t].second)
	    triggersecond = true;	
	  break;
	}
      }
      if(pair)
	break;
    }
  }
  
  return;
}


bool MiniAnalyzer::isMediumMuonBF(pat::Muon & recoMu) {
  bool goodGlob = recoMu.isGlobalMuon() && 
    recoMu.globalTrack()->normalizedChi2() < 3 && 
    recoMu.combinedQuality().chi2LocalPosition < 12 && 
    recoMu.combinedQuality().trkKink < 20; 
  bool isMedium = muon::isLooseMuon(recoMu) && 
    recoMu.innerTrack()->validFraction() > 0.49 && 
    muon::segmentCompatibility(recoMu) > (goodGlob ? 0.303 : 0.451); 
  return isMedium; 
}


double MiniAnalyzer::GetScaleFactorsFromH(TH2F* Bin2D, TH1F* Bin1D, double maxPt, double eta, double pt){
  // TH2F* Bin2D = (TH2F*)file->Get(histonameEtaPt);
  // TH1F* Bin1D = (TH1F*)file->Get(histonameEta);
  double sf = 1.;
  
  if(pt > 26. && pt < maxPt){
    int etaBin = Bin2D->GetXaxis()->FindBin(fabs(eta));
    int ptBin = Bin2D->GetYaxis()->FindBin(pt);
    sf = Bin2D->GetBinContent(etaBin,ptBin);
  }
  else if(fabs(eta) < 2.4){
    int etaBin = Bin1D->GetXaxis()->FindBin(eta);
    sf = Bin1D->GetBinContent(etaBin);
  } 
  // delete Bin2D;
  // delete Bin1D;
  return sf;
}


 
void MiniAnalyzer::CleanJetsFromMuons(std::vector<pat::Jet>& Jets, std::vector<pat::Muon>& Muons, float angle) {
  for(unsigned int m = 0; m < Muons.size(); m++) {
    for(unsigned int j = 0; j < Jets.size(); ) {
      
      if(deltaR(Jets[j], Muons[m]) < angle){ 
	Jets.erase(Jets.begin() + j);
      }
      else j++;
    }
  }
}
  

bool MiniAnalyzer::JetID_isLoose(pat::Jet jet){
 
  double eta = fabs(jet.eta());
  double NHF  = jet.neutralHadronEnergyFraction();
  double NEMF = jet.neutralEmEnergyFraction();
  double CHF  = jet.chargedHadronEnergyFraction();
  //double MUF  = jet.muonEnergyFraction();
  double CEMF = jet.chargedEmEnergyFraction();
  int NumConst = jet.chargedMultiplicity()+jet.neutralMultiplicity();
  int NumNeutralParticles = jet.neutralMultiplicity();
  int CHM      = jet.chargedMultiplicity(); 
    
   bool isLoose = false;
    
   if( ((NHF<0.99 && NEMF<0.99 && NumConst>1) && ((eta<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || eta>2.4) && eta<=2.7) ||
       (NHF<0.98 && NEMF>0.01 && NumNeutralParticles>2 && eta>2.7 && eta<=3.0 ) ||
       (NEMF<0.90 && NumNeutralParticles>10 && eta>3.0 )
       )
     isLoose = true;
    
   return isLoose;
}

bool MiniAnalyzer::JetID_isTight(pat::Jet jet){
 
  double eta = fabs(jet.eta());
  double NHF  = jet.neutralHadronEnergyFraction();
  double NEMF = jet.neutralEmEnergyFraction();
  double CHF  = jet.chargedHadronEnergyFraction();
  //double MUF  = jet.muonEnergyFraction();
  double CEMF = jet.chargedEmEnergyFraction();
  int NumConst = jet.chargedMultiplicity()+jet.neutralMultiplicity();
  int NumNeutralParticles = jet.neutralMultiplicity();
  int CHM      = jet.chargedMultiplicity(); 
    
  bool isTight = false;
  
  if( ((NHF<0.90 && NEMF<0.90 && NumConst>1) && ((eta<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || eta>2.4) && eta<=2.7) ||
      (NHF<0.98 && NEMF>0.01 && NumNeutralParticles>2 && eta>2.7 && eta<=3.0 ) ||
      (NEMF<0.90 && NumNeutralParticles>10 && eta>3.0 )
      )
    isTight = true;
  
  return isTight;
}

bool MiniAnalyzer::JetID_isTightLepVeto(pat::Jet jet){
 
  double eta = fabs(jet.eta());
  double NHF  = jet.neutralHadronEnergyFraction();
  double NEMF = jet.neutralEmEnergyFraction();
  double CHF  = jet.chargedHadronEnergyFraction();
  double MUF  = jet.muonEnergyFraction();
  double CEMF = jet.chargedEmEnergyFraction();
  int NumConst = jet.chargedMultiplicity()+jet.neutralMultiplicity();
  //int NumNeutralParticles = jet.neutralMultiplicity();
  int CHM      = jet.chargedMultiplicity(); 
  
  bool isTightLepVeto = false;  
  
  if((NHF<0.90 && NEMF<0.90 && NumConst>1 && MUF<0.8) && ((eta<=2.4 && CHF>0 && CHM>0 && CEMF<0.90) || eta>2.4) && eta<=2.7)
    isTightLepVeto = true;

  return  isTightLepVeto;
}
  

void MiniAnalyzer::fill_beamspot_variables(edm::Handle<reco::BeamSpot> beamspot){

  if (!beamspot.isValid()) {
    cout << "Error! Can't get the product primary Vertex Collection "<< endl;
    return;
  }
  
  const reco::BeamSpot &BS = *beamspot.product();
  
  BS_x = BS.x0();
  BS_y = BS.y0(); 
  BS_z = BS.z0();
  BS_xerr = BS.x0Error();
  BS_yerr = BS.y0Error();
  BS_zerr = BS.z0Error();
  BS_Wx = BS.BeamWidthX(); 
  BS_Wy = BS.BeamWidthY();
  BS_Wxerr = BS.BeamWidthXError();
  BS_Wyerr = BS.BeamWidthYError();
  BS_dxdz    = BS.dxdz();
  BS_dxdzerr = BS.dxdzError();
  BS_dydz    = BS.dydz();
  BS_dydzerr = BS.dydzError();
  
  iBeam++;
  
  return;
}


void MiniAnalyzer::fill_vtxs_variables(edm::Handle<reco::VertexCollection> vertices){ 
  ivtx = 0;
  
  //math::XYZPoint PV_position;
  reco::Vertex p;
  for(reco::VertexCollection::const_iterator vtx = vertices->begin(); vtx != vertices->end(); ++vtx){
    
    if (!vtx->isFake() && vtx->ndof()>4. && (fabs(vtx->z())<15) ){
      //PV_position = vtx->position();
      p = *vtx;
      ivtx++; 
      
      if(debug) std::cout << "vertex counter " << ivtx << " x " << p.x()<< " y " << p.y()<< " z " << p.z() << std::endl;	
    
      PV_x.push_back(p.x());
      PV_y.push_back(p.y()); 
      PV_z.push_back(p.z());
      PV_xerr.push_back(p.xError());
      PV_yerr.push_back(p.yError());
      PV_zerr.push_back(p.zError());
      
      PV_validity.push_back(p.isValid());
      PV_fake.push_back(p.isFake());
      
      //if (! p.isFake()){
      PV_normchi2.push_back(p.normalizedChi2());
      // }	
      // else
      //PV_normchi2.push_back(999.);
      
      PV_chi2.push_back(p.chi2());
      PV_ndof.push_back(p.ndof());
      PV_ntracks.push_back(p.tracksSize());
      
      double sumpt = 0;
      for (reco::Vertex::trackRef_iterator iTrack = p.tracks_begin(); iTrack!=p.tracks_end(); ++iTrack){
	sumpt += (*iTrack)->pt();
	if(debug) std::cout << "sum pt in vertex " << ivtx << "  = " << sumpt  <<std::endl;
      }
      
      PV_SumPtTracks.push_back(sumpt);
      
      
      //break;
    }
  }
  
  return;
}


void MiniAnalyzer::fill_hlt_variables(edm::Handle<edm::TriggerResults> triggerBits, const edm::Event& iEvent){
  ihlt = 0; 
  const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);
  
  for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i) {
    if(triggerBits->accept(i)){
      hlt_path.push_back(names.triggerName(i));
      ihlt++;
      }
    
  } 
    return;
}


void MiniAnalyzer::fill_muons_variables(edm::Handle<pat::MuonCollection> muons, const math::XYZPoint &thebs, reco::Vertex PV, const RoccoR local_rc,  const RoccoR crab_rc){
  imuons = 0;
  GlobalMuons      = 0;
  TrackerMuons     = 0;
  StandAloneMuons  = 0;

  
  for (pat::Muon mu : *muons) {
     
    imuons++;
    
    if(debug) std::cout << " muon counter " << imuons << ": pt " << mu.pt() << ", eta " << mu.eta() << std::endl;
    
    //did the muon fire the trigger?
    const pat::TriggerObjectStandAloneCollection muHLTMatches_iso     =  mu.triggerObjectMatchesByPath(pathNames_lowIsoUnpr);
    //const pat::TriggerObjectStandAloneCollection muHLTMatches_isoTk   =  mu.triggerObjectMatchesByPath(pathNames_lowIsoTkUnpr);
    const pat::TriggerObjectStandAloneCollection muHLTMatches_        =  mu.triggerObjectMatchesByPath(pathNames_lowUnpr);
    //const pat::TriggerObjectStandAloneCollection muHLTMatches_Tk      =  mu.triggerObjectMatchesByPath(pathNames_lowTkUnpr);
   
    
    
    bool passIso = false, pass = false;
    if(muHLTMatches_iso.size() > 0)     passIso = true;
    if(muHLTMatches_.size() > 0)        pass = true;
        
    if(debug){
      std::cout << "Did it triggered " << pathNames_lowUnpr <<"? " << pass << std::endl; 
      std::cout << "Did it triggered " << pathNames_lowIsoUnpr <<"? " << passIso << std::endl; 
    }
    
    Mu_hasTriggeredIso.push_back(passIso);
    Mu_hasTriggeredMu50.push_back(pass);
    
    Mu_isPFMu.push_back(mu.isPFMuon());
    
    if(MCtruth ||  runnumber > 278808)//runGH
      Mu_isMediumMu.push_back(mu.isMediumMuon());
    else
      Mu_isMediumMu.push_back(isMediumMuonBF(mu));
    
    Mu_isTightMu.push_back(mu.isTightMuon(PV));
    Mu_isMuGlobal.push_back(mu.isGlobalMuon());
    Mu_isMuTracker.push_back(mu.isTrackerMuon());
    Mu_isMuStandAlone.push_back(mu.isStandAloneMuon());
    
    if(mu.isStandAloneMuon()) ++StandAloneMuons;
    if(mu.isGlobalMuon()) ++GlobalMuons;
    if(mu.isTrackerMuon()) ++TrackerMuons;
    
    
    if(mu.tunePMuonBestTrack().isAvailable() && mu.globalTrack().isAvailable() && mu.innerTrack().isAvailable()){
      
      if(mu.isGlobalMuon() && mu.isTrackerMuon() && 
	 mu.tunePMuonBestTrack()->ptError()/mu.tunePMuonBestTrack()->pt() < 0.3 &&
	 (mu.numberOfMatchedStations() > 1 ||
	  (mu.numberOfMatchedStations() == 1 &&  !(mu.stationMask() == 1 || mu.stationMask() == 16)) || 
	  (mu.numberOfMatchedStations() == 1 &&	  (mu.stationMask() == 1 || mu.stationMask() == 16) &&  mu.numberOfMatchedRPCLayers() > 2) ) &&
	 mu.globalTrack()->hitPattern().numberOfValidMuonHits() > 0 &&
	 mu.innerTrack()->hitPattern().numberOfValidPixelHits() > 0 &&
	 mu.innerTrack()->hitPattern().numberOfValidTrackerHits() > 5 &&
	 fabs(mu.dB()) < 0.2 &&
	 fabs(mu.innerTrack()->dz(PV.position()))< 0.5)
	Mu_isHighPtMu.push_back(true);
      else
	Mu_isHighPtMu.push_back(false);
    }
    else
      Mu_isHighPtMu.push_back(false);
    

    //time veto 0 is in time, 1 is OOT
    int veto = 0; // default is in-time muon      
    
    reco::MuonTime cmb = mu.time();
    reco::MuonTime rpc = mu.rpcTime();
    bool cmbok =(cmb.nDof>7);
    bool rpcok =(rpc.nDof>1 && rpc.timeAtIpInOutErr==0);
    if (rpcok){
      if ((fabs(rpc.timeAtIpInOut)>10) &&    !(cmbok && fabs(cmb.timeAtIpInOut)<10))
	veto = 1;
    }
    else if (cmbok && (cmb.timeAtIpInOut>20 || cmb.timeAtIpInOut<-45))
      veto = 1;
    
    Mu_timingVeto.push_back(veto);
    
    Mu_numberOfChambers.push_back(mu.numberOfChambers());
    Mu_numberOfMatches.push_back(mu.numberOfMatches());
    Mu_numberOfMatchedStation.push_back(mu.numberOfMatchedStations());
    Mu_stationMask.push_back(mu.stationMask());
    Mu_numberOfMatchedRPCLayers.push_back(mu.numberOfMatchedRPCLayers());
    Mu_dB.push_back(mu.dB());
    
    Mu_pt_mu.push_back(mu.muonBestTrack()->pt());
    Mu_px_mu.push_back(mu.muonBestTrack()->px());
    Mu_py_mu.push_back(mu.muonBestTrack()->py());
    Mu_pz_mu.push_back(mu.muonBestTrack()->pz());
    Mu_en_mu.push_back(mu.energy());
    Mu_phi_mu.push_back(mu.muonBestTrack()->phi());
    Mu_eta_mu.push_back(mu.muonBestTrack()->eta());
    Mu_charge_mu.push_back(mu.muonBestTrack()->charge());
    
    if(MCtruth){
      Mu_triggerSF_BF.push_back(GetScaleFactorsFromH(Bin2D_TRIGrunBF, Bin1D_TRIGrunBF, 500., mu.eta(), mu.pt()));
      Mu_triggerSF_GH.push_back(GetScaleFactorsFromH(Bin2D_TRIGrunGH, Bin1D_TRIGrunGH, 500., mu.eta(), mu.pt()));
      Mu_idSF_BF.push_back(GetScaleFactorsFromH(Bin2D_IDrunBF, Bin1D_IDrunBF, 120., mu.eta(), mu.pt()));
      Mu_idSF_GH.push_back(GetScaleFactorsFromH(Bin2D_IDrunGH, Bin1D_IDrunGH, 120., mu.eta(), mu.pt()));
      Mu_isoSF_BF.push_back(GetScaleFactorsFromH(Bin2D_ISOrunBF, Bin1D_ISOrunBF, 120., mu.eta(), mu.pt()));
      Mu_isoSF_GH.push_back(GetScaleFactorsFromH(Bin2D_ISOrunGH, Bin1D_ISOrunGH, 120., mu.eta(), mu.pt()));

      Mu_hptriggerSF_BF.push_back(GetScaleFactorsFromH(hpBin2D_TRIGrunBF, hpBin1D_TRIGrunBF, 500., mu.eta(), mu.pt()));
      Mu_hptriggerSF_GH.push_back(GetScaleFactorsFromH(hpBin2D_TRIGrunGH, hpBin1D_TRIGrunGH, 500., mu.eta(), mu.pt()));
      Mu_hpidSF_BF.push_back(GetScaleFactorsFromH(hpBin2D_IDrunBF, hpBin1D_IDrunBF, 120., mu.eta(), mu.pt()));
      Mu_hpidSF_GH.push_back(GetScaleFactorsFromH(hpBin2D_IDrunGH, hpBin1D_IDrunGH, 120., mu.eta(), mu.pt()));
      Mu_hpisoSF_BF.push_back(GetScaleFactorsFromH(hpBin2D_ISOrunBF, hpBin1D_ISOrunBF, 120., mu.eta(), mu.pt()));
      Mu_hpisoSF_GH.push_back(GetScaleFactorsFromH(hpBin2D_ISOrunGH, hpBin1D_ISOrunGH, 120., mu.eta(), mu.pt()));
    }
        
    Mu_roch_correction.push_back(getRochester(mu, MCtruth, crab, local_rc, crab_rc));
    
    //isolation 03 and 05 and 04
    Mu_sumPtIsoR03.push_back(mu.isolationR03().sumPt);
    Mu_ntkIsoR03.push_back(mu.isolationR03().nTracks);
    Mu_emIsoR03.push_back(mu.isolationR03().emEt);
    Mu_hadIsoR03.push_back(mu.isolationR03().hadEt);
    Mu_hoEtIsoR03.push_back(mu.isolationR03().hoEt);
    Mu_nJetsIsoR03.push_back(mu.isolationR03().nJets);
    
    Mu_sumPtIsoR05.push_back(mu.isolationR05().sumPt);
    Mu_ntkIsoR05.push_back(mu.isolationR05().nTracks);
    Mu_emIsoR05.push_back(mu.isolationR05().emEt);
    Mu_hadIsoR05.push_back(mu.isolationR05().hadEt);
    Mu_hoEtIsoR05.push_back(mu.isolationR05().hoEt);
    Mu_nJetsIsoR05.push_back(mu.isolationR05().nJets);
    
    Mu_sumCHPtPFIsoR04.push_back(mu.pfIsolationR04().sumChargedHadronPt);
    Mu_sumCPPtPFIsoR04.push_back(mu.pfIsolationR04().sumChargedParticlePt);
    Mu_sumNHPtPFIsoR04.push_back(mu.pfIsolationR04().sumNeutralHadronEt);
    Mu_sumPhoEtPFIsoR04.push_back(mu.pfIsolationR04().sumPhotonEt);
    Mu_sumPUPtPFIsoR04.push_back(mu.pfIsolationR04().sumPUPt);
    
    Mu_sumCHPtPFIsoR03.push_back(mu.pfIsolationR03().sumChargedHadronPt);
    Mu_sumCPPtPFIsoR03.push_back(mu.pfIsolationR03().sumChargedParticlePt);
    Mu_sumNHPtPFIsoR03.push_back(mu.pfIsolationR03().sumNeutralHadronEt);
    Mu_sumPhoEtPFIsoR03.push_back(mu.pfIsolationR03().sumPhotonEt);
    Mu_sumPUPtPFIsoR03.push_back(mu.pfIsolationR03().sumPUPt);
    
    Mu_calEnergyEm.push_back(mu.calEnergy().em); 
    Mu_calEnergyHad.push_back(mu.calEnergy().had);
    Mu_calEnergyHo.push_back(mu.calEnergy().ho);
    Mu_calEnergyEmS9.push_back(mu.calEnergy().emS9);
    Mu_calEnergyHadS9.push_back(mu.calEnergy().hadS9);
    Mu_calEnergyHoS9.push_back(mu.calEnergy().hoS9);
    
    //point closest to beamspot
    Mu_vertex_vx.push_back(mu.vx());
    Mu_vertex_vy.push_back(mu.vy());
    Mu_vertex_vz.push_back(mu.vz());
    
    if(debug) std::cout << "mu vertex x = " << mu.vx() << "  y = " << mu.vy()  <<"  z = "<<mu.vz()  <<std::endl;
    
    //StandAlone Properties
    if(mu.outerTrack().isAvailable())
      STAMu_numberOfHits.push_back(mu.outerTrack()->numberOfValidHits());   
    else
      STAMu_numberOfHits.push_back(-999.);
        
    //Global Properties 
    if(mu.globalTrack().isAvailable()){
      GLBMu_pt.push_back(mu.globalTrack()->pt());
      GLBMu_pt_err.push_back(mu.globalTrack()->ptError());
      GLBMu_eta.push_back(mu.globalTrack()->eta());
      GLBMu_phi.push_back(mu.globalTrack()->phi());
      GLBMu_chi2.push_back(mu.globalTrack()->chi2());
      GLBMu_ndf.push_back(mu.globalTrack()->ndof());
      GLBMu_qOverPt.push_back(mu.charge()/mu.globalTrack()->pt());
      GLBMu_normchi2Mu.push_back(mu.globalTrack()->normalizedChi2());
      GLBMu_dxyMu.push_back(mu.globalTrack()->dxy(thebs));
      GLBMu_dzMu.push_back(mu.globalTrack()->dz(thebs));
      GLBMu_numberOfPixelHits.push_back(mu.globalTrack()->hitPattern().numberOfValidPixelHits());
      GLBMu_numberOfTrackerHits.push_back(mu.globalTrack()->hitPattern().numberOfValidTrackerHits());
      GLBMu_numberOfMuonsHits.push_back(mu.globalTrack()->hitPattern().numberOfValidMuonHits());
      GLBMu_vertex_vx.push_back(mu.globalTrack()->vx());
      GLBMu_vertex_vy.push_back(mu.globalTrack()->vy());
      GLBMu_vertex_vz.push_back(mu.globalTrack()->vz());
      
      if(debug){
	std::cout << "mu.globalTrack()->vertex()"<< mu.globalTrack()->vertex()<< std::endl;
	std::cout << "mu.vertex()"<< mu.vertex()<< std::endl;
	std::cout <<std::endl;
      }
    }
    
    else{
      GLBMu_pt.push_back(-999.);
      GLBMu_pt_err.push_back(-999);
      GLBMu_eta.push_back(-999);
      GLBMu_phi.push_back(-999);
      GLBMu_chi2.push_back(-999);
      GLBMu_ndf.push_back(-999);
      GLBMu_qOverPt.push_back(-999.);
      GLBMu_normchi2Mu.push_back(-999.);
      GLBMu_dxyMu.push_back(-999.);
      GLBMu_dzMu.push_back(-999.);
      GLBMu_numberOfPixelHits.push_back(-999);
      GLBMu_numberOfTrackerHits.push_back(-999);
      GLBMu_numberOfMuonsHits.push_back(-999);
      GLBMu_vertex_vx.push_back(-999.);
      GLBMu_vertex_vy.push_back(-999.);
      GLBMu_vertex_vz.push_back(-999.);
    }
    
    //Tracker Properties    
    if(mu.innerTrack().isAvailable()){
      TRKMu_pt.push_back(mu.innerTrack()->pt());
      TRKMu_pt_err.push_back(mu.innerTrack()->ptError());
      TRKMu_eta.push_back(mu.innerTrack()->eta());
      TRKMu_phi.push_back(mu.innerTrack()->phi());
      TRKMu_chi2.push_back(mu.innerTrack()->chi2());
      TRKMu_ndf.push_back(mu.innerTrack()->ndof());
      TRKMu_qOverPt.push_back(mu.charge()/mu.innerTrack()->pt());
      TRKMu_normchi2Mu.push_back(mu.innerTrack()->normalizedChi2());
      TRKMu_dxyMu.push_back(mu.innerTrack()->dxy(thebs));
      TRKMu_dzMu.push_back(mu.innerTrack()->dz(thebs));
      TRKMu_numberOfPixelHits.push_back(mu.innerTrack()->hitPattern().numberOfValidPixelHits());
      TRKMu_numberOfTrackerHits.push_back(mu.innerTrack()->hitPattern().numberOfValidTrackerHits());
      TRKMu_trackerLayersWithMeasurement.push_back(mu.innerTrack()->hitPattern().trackerLayersWithMeasurement());
      TRKMu_dzPV.push_back(fabs(mu.innerTrack()->dz(PV.position())));
      
      if(debug){
	std::cout << "mu.innerTrack()->vertex()"<<mu.innerTrack()->vertex()<< std::endl;
	std::cout << "mu.vertex()"<< mu.vertex()<< std::endl;
	std::cout <<std::endl;
      }
    }
    else{
      TRKMu_pt.push_back(-999.);
      TRKMu_pt_err.push_back(-999);
      TRKMu_eta.push_back(-999);
      TRKMu_phi.push_back(-999);
      TRKMu_chi2.push_back(-999);
      TRKMu_ndf.push_back(-999);
      TRKMu_qOverPt.push_back(-999.);
      TRKMu_normchi2Mu.push_back(-999.);
      TRKMu_dxyMu.push_back(-999.);
      TRKMu_dzMu.push_back(-999.);      
      TRKMu_numberOfPixelHits.push_back(-999);
      TRKMu_numberOfTrackerHits.push_back(-999);
      TRKMu_trackerLayersWithMeasurement.push_back(-999);
      TRKMu_dzPV.push_back(-999);
    } 
  
   
    if(mu.tunePMuonBestTrack().isAvailable()){
      TPMu_pt.push_back(mu.tunePMuonBestTrack()->pt());
      TPMu_pt_err.push_back(mu.tunePMuonBestTrack()->ptError());
      TPMu_eta.push_back(mu.tunePMuonBestTrack()->eta());
      TPMu_phi.push_back(mu.tunePMuonBestTrack()->phi());
      TPMu_chi2.push_back(mu.tunePMuonBestTrack()->chi2());
      TPMu_ndf.push_back(mu.tunePMuonBestTrack()->ndof());
      TPMu_qOverPt.push_back(mu.charge()/mu.tunePMuonBestTrack()->pt());
    
    }
    else{
      TPMu_pt.push_back(-999);
      TPMu_pt_err.push_back(-999);
      TPMu_eta.push_back(-999);
      TPMu_phi.push_back(-999);
      TPMu_chi2.push_back(-999);
      TPMu_ndf.push_back(-999);
      TPMu_qOverPt.push_back(-999.);
    }
    
  }
  
  // if(debug){
  //   printf(" good muons size = %lu, medium muons size = %lu, tight muons size = %lu/n",
  // 	   goodMuons.size(), mediumMuons.size(), tightMuons.size() );
  //   // std::cout << " Muon SF " << GetScaleFactorsFromDir(file_MuonTRIGrunBF, "IsoMu24_OR_IsoTkMu24_PtEtaBins/abseta_pt_ratio", mu.eta(), mu.pt()) << std::endl;
  // }
  
  return;
}

  // void MiniAnalyzer::fill_electrons_variables(edm::Handle<pat::ElectronCollection> electrons)
  // {    
  //   for(pat::Electron ele: *electrons){
 
  //     if(debug){
  //       std::cout << "**** E L E C T R O N S ****" <<std::endl;
  //       std::cout << "Pt   default = " <<ele.pt()<< std::endl;
  //       std::cout << "Eta  default = " << ele.eta()<<std::endl;
      
  //     }   
    
  //     Ele_pt_pfele.push_back(ele.pt());
    
  //     Ele_px_pfele.push_back(ele.px());
  //     Ele_py_pfele.push_back(ele.py());
  //     Ele_pz_pfele.push_back(ele.pz());
  //     Ele_en_pfele.push_back(ele.energy());
  //     Ele_phi_pfele.push_back(ele.phi());  
  //     Ele_eta_pfele.push_back(ele.eta());
  
  //   }
  
  //   return;
  // }


void MiniAnalyzer::fill_PFJet_variables(edm::Handle<std::vector<pat::Jet> > jets, edm::Handle<edm::ValueMap<float> > puJetIdMVA, edm::Handle<edm::ValueMap<int> > puJetIdFlag, JetCorrectionUncertainty *jecUnc){    
  
  ipfjets     = 0;
  bTagJetsHE  = 0;
  bTagJetsHP  = 0;
  bTagJetsCSV = 0;
  
  for ( unsigned int i=0; i<jets->size(); ++i ) {
    const pat::Jet & jet = jets->at(i);
    
    ipfjets++;
    
    // float  mva    = (*puJetIdMVA)[jet.originalObjectRef()];
    //int    idflag = (*puJetIdFlag)[jet.originalObjectRef()];
    //bool   passLoose = false;
    // bool   passMedium = false;
    // bool   passTight = false;
    
    //if( PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kLoose ))  passLoose  = true;
    // if( PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kMedium )) passMedium = true;
    // if( PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kTight ))  passTight  = true;
       
    //std::cout << "pileup jet id " << jet.userFloat("pileupJetId:fullDiscriminant") << " pass loose " << passLoose<<std::endl;
    
    double discriminatorHE  =-999.;
    double discriminatorHP  =-999.;
    double discriminatorCSV =-999.;
          
    Jet_pt_pfjet.push_back(jet.pt());
    Jet_px_pfjet.push_back(jet.px());
    Jet_py_pfjet.push_back(jet.py());
    Jet_pz_pfjet.push_back(jet.pz());
    Jet_en_pfjet.push_back(jet.energy());
    Jet_phi_pfjet.push_back(jet.phi());  
    Jet_eta_pfjet.push_back(jet.eta());
    Jet_Area_pfjet.push_back(jet.jetArea());
    
    jecUnc->setJetEta(jet.eta());
    jecUnc->setJetPt(jet.pt()); // here you must use the CORRECTED jet pt
    Jet_JECunc_pfjet.push_back(jecUnc->getUncertainty(true));
    
    Jet_isLoose_pfjet.push_back(JetID_isLoose(jet));
    Jet_isTight_pfjet.push_back(JetID_isTight(jet));
    Jet_isTightLepVeto_pfjet.push_back(JetID_isTightLepVeto(jet));
    
    Jet_ChargedHadEn_pfjet.push_back(jet.chargedHadronEnergy());
    Jet_NeutralHadEn_pfjet.push_back(jet.neutralHadronEnergy());
    Jet_ChargedEmEn_pfjet.push_back(jet.chargedEmEnergy());
    Jet_ChargedMuEn_pfjet.push_back(jet.chargedMuEnergy());
    Jet_NeutralEmEn_pfjet.push_back(jet.neutralEmEnergy());
    Jet_ChargedMultiplicity_pfjet.push_back(jet.chargedMultiplicity());
    Jet_NeutralMultiplicity_pfjet.push_back(jet.neutralMultiplicity());
    Jet_MuonMultiplicity_pfjet.push_back(jet.muonMultiplicity());   
    Jet_ElectronMultiplicity_pfjet.push_back(jet.electronMultiplicity());   
    
    // Jet_PUmva_pfjet.push_back(mva);   
    // Jet_PULoose_pfjet.push_back(passLoose);
    // Jet_PUMedium_pfjet.push_back(passMedium);
    // Jet_PUTight_pfjet.push_back(passTight);
    
    discriminatorHE = jet.bDiscriminator("pfTrackCountingHighEffBJetTags");
    if(discriminatorHE !=-100.) bTagJetsHE++;
    
    discriminatorHP = jet.bDiscriminator("pfTrackCountingHighPurBJetTags");
    if(discriminatorHP !=-100.) bTagJetsHP++;
    
    discriminatorCSV = jet.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
    if(discriminatorCSV !=-100. && discriminatorCSV !=-10.) bTagJetsCSV++;
    
    Jet_discriminatorHE_pfjet.push_back(discriminatorHE);  
    Jet_discriminatorHP_pfjet.push_back(discriminatorHP); 
    Jet_discriminatorCSV_pfjet.push_back(discriminatorCSV); 
    
    if(debug){
      std::cout << "**** J E T ****" <<std::endl;
           
      // std::cout << "btag TCHE = " <<jet.bDiscriminator("pfTrackCountingHighEffBJetTags")<< std::endl;
      // std::cout << "btag TCHP = " <<jet.bDiscriminator("pfTrackCountingHighPurBJetTags")<< std::endl;
      // std::cout << "btag CSV  = " <<jet.bDiscriminator("pfCombinedSecondaryVertexV2BJetTags")<< std::endl;
      
      std::cout << " jet counter " << ipfjets << std::endl;
    }   
  }
  
  return;
}



void MiniAnalyzer::fill_PFMET_variables(edm::Handle<pat::METCollection> mets) {
  ipfmets = 0;
  
  for(pat::METCollection::const_iterator met = mets->begin(); met != mets->end(); met++){
    //for( auto met = mets->begin(); met != mets->end(); ++met ){
    ipfmets++;
    
    if(debug)  std::cout << "MEt = " << met->pt() << " met counter " << ipfmets << std::endl; 
    
    Met_pt_pfmet.push_back(met->pt());
    Met_phi_pfmet.push_back(met->phi());
    Met_eta_pfmet.push_back(met->eta());
    Met_energy_pfmet.push_back(met->energy());
    Met_sumet_pfmet.push_back(met->sumEt());
    Met_ptsignificance_pfmet.push_back(met->metSignificance());
    Met_etsignificance_pfmet.push_back(met->mEtSig());
    
    Met_type01smear_pt_pfmet.push_back(met->corPt(pat::MET::Type01Smear));
    
    // double jetEnUp =  (met->shiftedPt(pat::MET::JetEnUp)- met->pt())/met->pt();
    // double jetEnDown = (met->shiftedPt(pat::MET::JetEnDown)- met->pt())/met->pt();
    // double jetResUp =  (met->shiftedPt(pat::MET::JetResUp)- met->pt())/met->pt();
    // double jetResDown = (met->shiftedPt(pat::MET::JetResDown)- met->pt())/met->pt();
    // double unclusterUp = (met->shiftedPt(pat::MET::UnclusteredEnUp)- met->pt())/met->pt();
    // double unclusterDown = (met->shiftedPt(pat::MET::UnclusteredEnDown)- met->pt())/met->pt();  
    // double tauUp = (met->shiftedPt(pat::MET::TauEnUp)- met->pt())/met->pt();
    // double tauDown =  (met->shiftedPt(pat::MET::TauEnDown)- met->pt())/met->pt();  
    // double eleUp =  (met->shiftedPt(pat::MET::ElectronEnUp)- met->pt())/met->pt();
    // double eleDown = (met->shiftedPt(pat::MET::ElectronEnDown)- met->pt())/met->pt();  
    // double photonUp = (met->shiftedPt(pat::MET::PhotonEnUp)- met->pt())/met->pt();
    // double photonDown = (met->shiftedPt(pat::MET::PhotonEnDown)- met->pt())/met->pt();  
    // double muUp = (met->shiftedPt(pat::MET::MuonEnUp)- met->pt())/met->pt();
    // double muDown = (met->shiftedPt(pat::MET::MuonEnDown)- met->pt())/met->pt();
    
    // double totUp = sqrt(jetEnUp*jetEnUp + jetResUp*jetResUp + unclusterUp*unclusterUp + tauUp*tauUp + eleUp*eleUp + photonUp*photonUp + muUp*muUp);
    // double totDown = sqrt(jetEnDown*jetEnDown + jetResDown*jetResDown + unclusterDown*unclusterDown + tauDown*tauDown + eleDown*eleDown + photonDown*photonDown + muDown*muDown);
    
    // Met_totUp_pt_pfmet.push_back(met->pt() + totUp*met->pt());      
    // Met_totDown_pt_pfmet.push_back(met->pt() - totDown*met->pt()); 


    //std::cout << "MET = " << met->pt() << " jetEnUp = " << met->shiftedPt(pat::MET::JetEnUp) << " jetEnDown = " << met->shiftedPt(pat::MET::JetEnDown) << std::endl;
    
    Met_jetEnUp_pfmet.push_back(met->shiftedPt(pat::MET::JetEnUp));
    Met_jetEnDown_pfmet.push_back(met->shiftedPt(pat::MET::JetEnDown));
    Met_jetResUp_pfmet.push_back(met->shiftedPt(pat::MET::JetResUp));
    Met_jetResDown_pfmet.push_back(met->shiftedPt(pat::MET::JetResDown));
    Met_unclusterUp_pfmet.push_back(met->shiftedPt(pat::MET::UnclusteredEnUp));
    Met_unclusterDown_pfmet.push_back(met->shiftedPt(pat::MET::UnclusteredEnDown));  
    Met_tauUp_pfmet.push_back(met->shiftedPt(pat::MET::TauEnUp));
    Met_tauDown_pfmet.push_back(met->shiftedPt(pat::MET::TauEnDown));  
    Met_eleUp_pfmet.push_back(met->shiftedPt(pat::MET::ElectronEnUp));
    Met_eleDown_pfmet.push_back(met->shiftedPt(pat::MET::ElectronEnDown));  
    Met_photonUp_pfmet.push_back(met->shiftedPt(pat::MET::PhotonEnUp));
    Met_photonDown_pfmet.push_back(met->shiftedPt(pat::MET::PhotonEnDown));  
    Met_muUp_pfmet.push_back(met->shiftedPt(pat::MET::MuonEnUp));
    Met_muDown_pfmet.push_back(met->shiftedPt(pat::MET::MuonEnDown));
    
    // Met_cHADEtFraction_pfmet.push_back(met->ChargedHadEtFraction());
    // Met_nHADEtFraction_pfmet.push_back(met->NeutralHadEtFraction());
    
    // Met_pt_calomet.push_back(met->caloMETPt());
    // Met_phi_calomet.push_back(met->caloMETPhi());
    
    Met_phi_umet.push_back(met->uncorPhi());
    Met_pt_umet.push_back(met->uncorPt());
    Met_sumet_umet.push_back(met->uncorSumEt());
    
  }
  return;
}


void MiniAnalyzer::fill_gen_variables(edm::Handle<edm::View<reco::GenParticle> > pruned, edm::Handle<edm::View<pat::PackedGenParticle> > packed){
  iGenP = 0;

  //std::cout << "***************************************************************" << std::endl;  

  

  for(size_t i=0; i<pruned->size();i++){
   
      // const Candidate *boh = &(*pruned)[i];
      // std::cout << boh->pdgId() << " status " <<boh->status() << ": pt " << boh->pt() << " eta " << boh->eta();
      // if( boh->mother(0)!= nullptr )
      // 	std::cout << "  madre " << boh->mother(0)->pdgId() << "  madre status " << boh->mother(0)->status()<< std::endl;
    
    
    if((*pruned)[i].status() == 62 && //status before decay
       ( (*pruned)[i].pdgId() == 36 || (*pruned)[i].pdgId() == 35 || (*pruned)[i].pdgId() == 25)){
      const Candidate *Higgs = &(*pruned)[i];
      
      for(size_t j=0; j<packed->size();j++){

	//if( fabs((*packed)[j].pdgId()) == 16 || fabs((*packed)[j].pdgId()) == 14 || fabs((*packed)[j].pdgId()) == 12)
	  
	  // std::cout << "**1** PdgID: " << (*packed)[j].pdgId() << " status " << (*packed)[j].status() << " pt " << (*packed)[j].pt() << " eta: " << (*packed)[j].eta() << " phi: " << (*packed)[j].phi() << std::endl;
	
	const Candidate * motherInPrunedCollection = (*packed)[j].mother(0) ;
		
	iGenP++;  
	
	Genp_particleId.push_back((*packed)[j].pdgId());  
	Genp_status.push_back((*packed)[j].status());
	Genp_pt.push_back((*packed)[j].pt());
	Genp_p.push_back((*packed)[j].p());
	Genp_et.push_back((*packed)[j].et());
	Genp_e.push_back((*packed)[j].energy());
	Genp_mt.push_back((*packed)[j].mt());
	Genp_m.push_back((*packed)[j].mass());
	Genp_eta.push_back((*packed)[j].eta());
	Genp_phi.push_back((*packed)[j].phi());
	Genp_vx.push_back((*packed)[j].vx());
	Genp_vy.push_back((*packed)[j].vy());
	Genp_vz.push_back((*packed)[j].vz());
	
	Genp_nMmothers.push_back((*packed)[j].numberOfMothers());
	Genp_nDaughters.push_back((*packed)[j].numberOfDaughters());
	

	if(motherInPrunedCollection != nullptr && isAncestor( Higgs , motherInPrunedCollection)){
	  if(debug) 
	    std::cout << " PdgID: " << (*packed)[j].pdgId() << " pt " << (*packed)[j].pt() << " eta: " << (*packed)[j].eta() << " phi: " << (*packed)[j].phi() << std::endl;
	  
	  Genp_particleId_mother.push_back(Higgs->pdgId());
	  Genp_status_mother.push_back(Higgs->status());
	  Genp_pt_mother.push_back(Higgs->pt());
	  Genp_eta_mother.push_back(Higgs->eta());
	  Genp_phi_mother.push_back(Higgs->phi());
	  Genp_m_mother.push_back(Higgs->mass());	  
	}
	else{
	  Genp_particleId_mother.push_back((*packed)[j].mother(0)->pdgId());
	  Genp_status_mother.push_back((*packed)[j].mother(0)->status());
	  Genp_pt_mother.push_back((*packed)[j].mother(0)->pt());
	  Genp_eta_mother.push_back((*packed)[j].mother(0)->eta());
	  Genp_phi_mother.push_back((*packed)[j].mother(0)->phi());
	  Genp_m_mother.push_back((*packed)[j].mother(0)->mass());
	}
	
      }
    }
    else if((*pruned)[i].status() == 23){ //23 sono solo b e muoni
      //i b diventano 51 e poi 44 poi 52 poi 62 poi 71(li prepara all'adronizzazione)
      const Candidate *quarks = &(*pruned)[i];
      //std::cout<< "**2** PdgID: " << quarks->pdgId() << "  status " <<quarks->status() << ": pt " << quarks->pt() << " eta " << quarks->eta() << std::endl;
      
      //riempi con i quark(o muoni nello stato intermedio) e le rispettive madri che non si sa chi sono
      Genp_particleId.push_back((*pruned)[i].pdgId());  
      Genp_status.push_back((*pruned)[i].status());
      Genp_pt.push_back((*pruned)[i].pt());
      Genp_p.push_back((*pruned)[i].p());
      Genp_et.push_back((*pruned)[i].et());
      Genp_e.push_back((*pruned)[i].energy());
      Genp_mt.push_back((*pruned)[i].mt());
      Genp_m.push_back((*pruned)[i].mass());
      Genp_eta.push_back((*pruned)[i].eta());
      Genp_phi.push_back((*pruned)[i].phi());
      Genp_vx.push_back((*pruned)[i].vx());
      Genp_vy.push_back((*pruned)[i].vy());
      Genp_vz.push_back((*pruned)[i].vz());

      Genp_nMmothers.push_back((*pruned)[i].numberOfMothers());
      Genp_nDaughters.push_back((*pruned)[i].numberOfDaughters());
      
      Genp_particleId_mother.push_back((*pruned)[i].mother(0)->pdgId());
      Genp_status_mother.push_back((*pruned)[i].mother(0)->status());
      Genp_pt_mother.push_back((*pruned)[i].mother(0)->pt());
      Genp_eta_mother.push_back((*pruned)[i].mother(0)->eta());
      Genp_phi_mother.push_back((*pruned)[i].mother(0)->phi());
      Genp_m_mother.push_back((*pruned)[i].mother(0)->mass());
    }

    else if(fabs((*pruned)[i].pdgId())<9 && (*pruned)[i].status() == 62){ //quark che decadono
      //i b diventano 51 e poi 44 poi 52 poi 62 poi 71(li prepara all'adronizzazione)
      const Candidate *quarks = &(*pruned)[i];
            
      //riempi con i figli dei quark 
      for(size_t j=0; j<packed->size();j++){
	
	const Candidate * motherInPrunedCollection = (*packed)[j].mother(0) ;
	if(motherInPrunedCollection != nullptr && isAncestor( quarks , motherInPrunedCollection)){

	  Genp_particleId.push_back((*packed)[j].pdgId());  
	  Genp_status.push_back((*packed)[j].status());
	  Genp_pt.push_back((*packed)[j].pt());
	  Genp_p.push_back((*packed)[j].p());
	  Genp_et.push_back((*packed)[j].et());
	  Genp_e.push_back((*packed)[j].energy());
	  Genp_mt.push_back((*packed)[j].mt());
	  Genp_m.push_back((*packed)[j].mass());
	  Genp_eta.push_back((*packed)[j].eta());
	  Genp_phi.push_back((*packed)[j].phi());
	  Genp_vx.push_back((*packed)[j].vx());
	  Genp_vy.push_back((*packed)[j].vy());
	  Genp_vz.push_back((*packed)[j].vz());
	  
	  Genp_nMmothers.push_back((*packed)[j].numberOfMothers());
	  Genp_nDaughters.push_back((*packed)[j].numberOfDaughters());
	  
	  Genp_particleId_mother.push_back(quarks->pdgId());
	  Genp_status_mother.push_back(quarks->status());
	  Genp_pt_mother.push_back(quarks->pt());
	  Genp_eta_mother.push_back(quarks->eta());
	  Genp_phi_mother.push_back(quarks->phi());
	  Genp_m_mother.push_back(quarks->mass());
	  
	  // std::cout << "**3** PdgID: " << (*packed)[j].pdgId() << " status " << (*packed)[j].status() << " pt " << (*packed)[j].pt() << " eta: " << (*packed)[j].eta() << " phi: " << (*packed)[j].phi() << std::endl;
	}
      }     
      
    } 
       
  } 
  return;
}





// ------------ method called once each job just before starting event loop  ------------
void 
MiniAnalyzer::beginJob(){
  
  
  if(MCtruth){
    ///////////////////////////////////////////////
    ///////// PU Reweight Inizialization //////////
    ///////////////////////////////////////////////
    
    // initializations to compute the pdf systematic error
    //LHAPDF::initPDFSet(1,"MSTW2008nlo68cl.LHgrid");
    //LHAPDF::initPDFSet(2,"NNPDF20_100.LHgrid");  // PdfSetNames 
    //LHAPDF::initPDFSet(3,"CT10.LHgrid"); 
    
    
    std::vector< float > Run_2017 = { 
      259618.,    1.08365e+06,2.08929e+06,3.69081e+06,
      4.14022e+06,5.90848e+06,6.42189e+06,6.83042e+06,9.61582e+06,
      2.30565e+07,4.53003e+07,8.51125e+07,1.32542e+08,1.89197e+08,
      2.67732e+08,3.78454e+08,5.28981e+08,7.0427e+08, 8.77806e+08,
      1.03821e+09,1.17958e+09,1.29358e+09,1.37936e+09,1.44451e+09,
      1.50065e+09,1.55457e+09,1.60086e+09,1.62988e+09,1.63531e+09,
      1.61523e+09,1.57209e+09,1.50998e+09,1.43159e+09,1.33954e+09,
      1.23898e+09,1.13662e+09,1.03797e+09,9.46449e+08,8.64849e+08,
      7.9675e+08, 7.46225e+08,7.16485e+08,7.08561e+08,7.20423e+08,
      7.46478e+08,7.77594e+08,8.02106e+08,8.08162e+08,7.86875e+08,
      7.34652e+08,6.54766e+08,5.56152e+08,4.50267e+08,3.48144e+08,
      2.57938e+08,1.83913e+08,1.26815e+08,8.4997e+07, 5.56524e+07,
      3.57639e+07,2.26539e+07,1.41992e+07,8.83868e+06,5.4831e+06,
      3.40152e+06,2.11725e+06,1.3263e+06, 838220,     535334,
      345702,     225630,     148641,     98653.2,    65824.4,
      44059.6,    29527.4,    19779.1,    13224.2,    8814.72,
      5851.9,     3866.15,    2540.09,    1658.59,    1075.74,
      692.697,    442.64,     280.579,    176.362,    109.891,
      67.8579,    41.5164,    25.1608,    15.1021,    8.97602,
      5.28211,    3.07719,    1.77451,    1.01284,    0.572145};

    std::vector< float > Run_2017_Up = {
      251130,      931342,      1.65874e+06, 3.73954e+06, 
      3.52145e+06, 5.19985e+06, 6.18925e+06, 6.19506e+06, 7.45644e+06, 
      1.45446e+07, 3.14496e+07, 5.79356e+07, 9.9233e+07,  1.436e+08, 
      2.00644e+08, 2.79039e+08, 3.87849e+08, 5.30658e+08, 6.90885e+08, 
      8.47433e+08, 9.92283e+08, 1.12059e+09, 1.22527e+09, 1.30526e+09, 
      1.36633e+09, 1.41795e+09, 1.46743e+09, 1.5124e+09,  1.54484e+09, 
      1.55837e+09, 1.55e+09,    1.52034e+09, 1.47258e+09, 1.40967e+09, 
      1.33345e+09, 1.24671e+09, 1.15437e+09, 1.06192e+09, 9.7345e+08, 
      8.91538e+08, 8.18511e+08, 7.57389e+08, 7.11537e+08, 6.83616e+08, 
      6.74587e+08, 6.8305e+08,  7.04837e+08, 7.32948e+08, 7.5812e+08, 
      7.70356e+08, 7.61208e+08, 7.25894e+08, 6.64567e+08, 5.8273e+08, 
      4.89062e+08, 3.93173e+08, 3.03446e+08, 2.25557e+08, 1.6211e+08, 
      1.13137e+08, 7.7012e+07,  5.13496e+07, 3.36731e+07, 2.17968e+07, 
      1.39741e+07, 8.90105e+06, 5.65008e+06, 3.58473e+06, 2.27985e+06, 
      1.45741e+06, 938596,      609996,      400404,      265454, 
      177601,      119745,      81220.4,     55316.2,     37760.6, 
      25794,       17606.9,     11995.5,     8148.95,     5515.31, 
      3716.38,     2491.65,     0.,          0.,          0.,
      0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.};
    
    std::vector< float > Run_2017_Down = {
      272633, 1.20423e+06, 2.78397e+06, 3.55026e+06, 
      4.81495e+06, 6.61863e+06, 6.73643e+06, 7.79893e+06, 1.47591e+07, 
      3.48147e+07, 6.85352e+07, 1.19617e+08, 1.76711e+08, 2.55102e+08, 
      3.67248e+08, 5.2495e+08,  7.16681e+08, 9.09962e+08, 1.08854e+09, 
      1.24508e+09, 1.36967e+09, 1.46189e+09, 1.53174e+09, 1.59329e+09, 
      1.65174e+09, 1.69795e+09, 1.72099e+09, 1.71506e+09, 1.68019e+09, 
      1.62086e+09, 1.54134e+09, 1.44448e+09, 1.33532e+09, 1.2218e+09, 
      1.11129e+09, 1.00846e+09, 9.1675e+08,  8.40435e+08, 7.84485e+08, 
      7.52812e+08, 7.46487e+08, 7.62569e+08, 7.93483e+08, 8.27231e+08, 
      8.49153e+08, 8.45495e+08, 8.07549e+08, 7.34309e+08, 6.33492e+08, 
      5.18087e+08, 4.02229e+08, 2.97444e+08, 2.10497e+08, 1.43352e+08, 
      9.45041e+07, 6.06637e+07, 3.81267e+07, 2.35792e+07, 1.44144e+07, 
      8.7472e+06,  5.29064e+06, 3.2022e+06,  1.94694e+06, 1.19315e+06, 
      738945,      463167,      293867,      188541,      122088, 
      79598.4,     52120.9,     34195.8,     22433.3,     14690.3, 
      9588.85,     6231.63,     4028.24,     2587.92,     1651.18, 
      1045.61,     656.803,     409.04,      252.445,     154.334, 
      93.433,      55.9947,     33.2113,     0.,          0.,
      0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.};

    std::vector< float > mix_2017_25ns_Moriond18 = {
      3.39597497605e-05,             6.63688402133e-06,              1.39533611284e-05,              3.64963078209e-05,              6.00872171664e-05,
      9.33932578027e-05,             0.000120591524486,              0.000128694546198,              0.000361697233219,              0.000361796847553,
      0.000702474896113,             0.00133766053707,               0.00237817050805,               0.00389825605651,               0.00594546732588,
      0.00856825906255,              0.0116627396044,                0.0148793350787,                0.0179897368379,                0.0208723871946,
      0.0232564170641,               0.0249826433945,                0.0262245860346,                0.0272704617569,                0.0283301107549,
      0.0294006137386,               0.0303026836965,                0.0309692426278,                0.0308818046328,                0.0310566806228,
      0.0309692426278,               0.0310566806228,                0.0310566806228,                0.0310566806228,                0.0307696426944,
      0.0300103336052,               0.0288355370103,                0.0273233309106,                0.0264343533951,                0.0255453758796,
      0.0235877272306,               0.0215627588047,                0.0195825559393,                0.0177296309658,                0.0160560731931,
      0.0146022004183,               0.0134080690078,                0.0129586991411,                0.0125093292745,                0.0124360740539,
      0.0123547104433,               0.0123953922486,                0.0124360740539,                0.0124360740539,                0.0123547104433,
      0.0124360740539,               0.0123387597772,                0.0122414455005,                0.011705203844,                 0.0108187105305,
      0.00963985508986,              0.00827210065136,               0.00683770076341,               0.00545237697118,               0.00420456901556,
      0.00367513566191,              0.00314570230825,               0.0022917978982,                0.00163221454973,               0.00114065309494,
      0.000784838366118,             0.000533204105387,              0.000358474034915,              0.000238881117601,              0.0001984254989,
      0.000157969880198,             0.00010375646169,               6.77366175538e-05,              4.39850477645e-05,              2.84298066026e-05,
      1.83041729561e-05,             1.17473542058e-05,              7.51982735129e-06,              6.16160108867e-06,              4.80337482605e-06,
      3.06235473369e-06,             1.94863396999e-06,              1.23726800704e-06,              7.83538083774e-07,              4.94602064224e-07,
      3.10989480331e-07,             1.94628487765e-07,              1.57888581037e-07,              1.2114867431e-07,               7.49518929908e-08,
      4.6060444984e-08,              2.81008884326e-08,              1.70121486128e-08,              1.02159894812e-08};//,              1.02159894812e-08};



    LumiWeights_ = edm::LumiReWeighting(mix_2017_25ns_Moriond18, Run_2017);
    LumiWeightsUp_ = edm::LumiReWeighting(mix_2017_25ns_Moriond18, Run_2017_Up);
    LumiWeightsDown_ = edm::LumiReWeighting(mix_2017_25ns_Moriond18, Run_2017_Down);
    
       
    if(!crab){
      data = new TH1F("data", "data", Run_2017.size(), 0., Run_2017.size());
      mc = new TH1F("mc", "mc", mix_2017_25ns_Moriond18.size(), 0., mix_2017_25ns_Moriond18.size());
      
      for(unsigned int ibin = 1; ibin < Run_2017.size()+1; ++ibin){
	data->SetBinContent(ibin, Run_2017[ibin-1]);
	mc->SetBinContent(ibin, mix_2017_25ns_Moriond18[ibin-1]);
      }
      
      data->Scale(1./data->Integral());
      mc->Scale(1./mc->Integral());
      weights = (TH1F*)data->Clone();
      weights->Divide(mc);
      
      //gStyle->SetOptStat(kFALSE);
      TLegend *leg = new TLegend(.65,.70,1.,1.);
      TCanvas *c = new TCanvas("c", "Number of True Interactions", 1000,700);
      c->Divide(2,1);
      c->cd(1); 
      data->SetLineColor(1);
      data->SetLineWidth(2);
      data->SetTitle("Number of true interactions");
      data->GetXaxis()->SetTitle("# of int");
      data->GetYaxis()->SetTitle("probability");
      data->Draw();
      mc->SetLineColor(2);
      mc->SetLineWidth(2);
      mc->Draw("same");
      leg->AddEntry(data,"Data2017 All Runs");
      leg->AddEntry(mc,"MC PU_Moriond18");
      leg->Draw();
      
      c->cd(2);
      weights->SetTitle("PU weights");
      weights->GetXaxis()->SetTitle("n. of true int");
      weights->GetYaxis()->SetTitle("data/mc"); 
      weights->Draw();
      c->Update();
      c->SaveAs("pippo.pdf");
      // std::cout << "mia distr. " << std::endl;
      // for(int ibin = 1; ibin<weights->GetNbinsX()+1; ++ibin){
      //   std::cout << weights->GetBinContent(ibin) << std::endl;
      // }
      
    
      file_MuonTRIGrunBF = TFile::Open("/afs/cern.ch/work/f/federica/MSSM/Federica/CMSSW_8_0_26_patch1/src/MyAnalyzer/MiniAnalyzer/data/EfficienciesAndSF_RunBtoF.root");
      file_MuonTRIGrunGH = TFile::Open("/afs/cern.ch/work/f/federica/MSSM/Federica/CMSSW_8_0_26_patch1/src/MyAnalyzer/MiniAnalyzer/data/EfficienciesAndSF_Period4.root");
      file_MuonIDrunBF = TFile::Open("/afs/cern.ch/work/f/federica/MSSM/Federica/CMSSW_8_0_26_patch1/src/MyAnalyzer/MiniAnalyzer/data/EfficienciesAndSF_BCDEF.root");
      file_MuonIDrunGH = TFile::Open("/afs/cern.ch/work/f/federica/MSSM/Federica/CMSSW_8_0_26_patch1/src/MyAnalyzer/MiniAnalyzer/data/EfficienciesAndSF_GH.root");
      file_MuonISOrunBF = TFile::Open("/afs/cern.ch/work/f/federica/MSSM/Federica/CMSSW_8_0_26_patch1/src/MyAnalyzer/MiniAnalyzer/data/IsoEfficienciesAndSF_BCDEF.root");
      file_MuonISOrunGH = TFile::Open("/afs/cern.ch/work/f/federica/MSSM/Federica/CMSSW_8_0_26_patch1/src/MyAnalyzer/MiniAnalyzer/data/IsoEfficienciesAndSF_GH.root");
      
    }
    else{
      file_MuonTRIGrunBF = TFile::Open("src/MyAnalyzer/MiniAnalyzer/data/EfficienciesAndSF_RunBtoF.root");
      file_MuonTRIGrunGH = TFile::Open("src/MyAnalyzer/MiniAnalyzer/data/EfficienciesAndSF_Period4.root");
      file_MuonIDrunBF = TFile::Open("src/MyAnalyzer/MiniAnalyzer/data/EfficienciesAndSF_BCDEF.root");
      file_MuonIDrunGH = TFile::Open("src/MyAnalyzer/MiniAnalyzer/data/EfficienciesAndSF_GH.root");
      file_MuonISOrunBF = TFile::Open("src/MyAnalyzer/MiniAnalyzer/data/IsoEfficienciesAndSF_BCDEF.root");
      file_MuonISOrunGH = TFile::Open("src/MyAnalyzer/MiniAnalyzer/data/IsoEfficienciesAndSF_GH.root");
    }
    
    Bin2D_TRIGrunBF = (TH2F*)file_MuonTRIGrunBF->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/abseta_pt_ratio");
    Bin1D_TRIGrunBF = (TH1F*)file_MuonTRIGrunBF->Get("IsoMu24_OR_IsoTkMu24_EtaBins/eta_ratio");
    Bin2D_TRIGrunGH = (TH2F*)file_MuonTRIGrunGH->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/abseta_pt_ratio");
    Bin1D_TRIGrunGH = (TH1F*)file_MuonTRIGrunGH->Get("IsoMu24_OR_IsoTkMu24_EtaBins/eta_ratio");
    
    Bin2D_TRIGrunBF->SetDirectory(0);
    Bin1D_TRIGrunBF->SetDirectory(0);
    Bin2D_TRIGrunGH->SetDirectory(0);
    Bin1D_TRIGrunGH->SetDirectory(0);

    hpBin2D_TRIGrunBF = (TH2F*)file_MuonTRIGrunBF->Get("Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio");
    hpBin1D_TRIGrunBF = (TH1F*)file_MuonTRIGrunBF->Get("Mu50_OR_TkMu50_EtaBins/eta_ratio");
    hpBin2D_TRIGrunGH = (TH2F*)file_MuonTRIGrunGH->Get("Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio");
    hpBin1D_TRIGrunGH = (TH1F*)file_MuonTRIGrunGH->Get("Mu50_OR_TkMu50_EtaBins/eta_ratio");
    
    hpBin2D_TRIGrunBF->SetDirectory(0);
    hpBin1D_TRIGrunBF->SetDirectory(0);
    hpBin2D_TRIGrunGH->SetDirectory(0);
    hpBin1D_TRIGrunGH->SetDirectory(0);
    delete file_MuonTRIGrunBF;
    
    Bin2D_IDrunBF = (TH2F*)file_MuonIDrunBF->Get("MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/abseta_pt_ratio");
    Bin1D_IDrunBF = (TH1F*)file_MuonIDrunBF->Get("MC_NUM_TightID_DEN_genTracks_PAR_eta/eta_ratio");
    Bin2D_IDrunGH = (TH2F*)file_MuonIDrunGH->Get("MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/abseta_pt_ratio");
    Bin1D_IDrunGH = (TH1F*)file_MuonIDrunGH->Get("MC_NUM_TightID_DEN_genTracks_PAR_eta/eta_ratio");
    
    Bin2D_IDrunBF->SetDirectory(0);
    Bin1D_IDrunBF->SetDirectory(0);
    Bin2D_IDrunGH->SetDirectory(0);
    Bin1D_IDrunGH->SetDirectory(0);

    hpBin2D_IDrunBF = (TH2F*)file_MuonIDrunBF->Get("MC_NUM_HighPtID_DEN_genTracks_PAR_newpt_eta/pair_ne_ratio");
    hpBin1D_IDrunBF = (TH1F*)file_MuonIDrunBF->Get("MC_NUM_HighPtID_DEN_genTracks_PAR_eta/eta_ratio");
    hpBin2D_IDrunGH = (TH2F*)file_MuonIDrunGH->Get("MC_NUM_HighPtID_DEN_genTracks_PAR_newpt_eta/pair_ne_ratio");
    hpBin1D_IDrunGH = (TH1F*)file_MuonIDrunGH->Get("MC_NUM_HighPtID_DEN_genTracks_PAR_eta/eta_ratio");
    
    hpBin2D_IDrunBF->SetDirectory(0);
    hpBin1D_IDrunBF->SetDirectory(0);
    hpBin2D_IDrunGH->SetDirectory(0);
    hpBin1D_IDrunGH->SetDirectory(0);
    delete file_MuonIDrunBF;  
    
    Bin2D_ISOrunBF = (TH2F*)file_MuonISOrunBF->Get("LooseISO_TightID_pt_eta/abseta_pt_ratio");
    Bin1D_ISOrunBF = (TH1F*)file_MuonISOrunBF->Get("LooseISO_TightID_eta/eta_ratio");
    Bin2D_ISOrunGH = (TH2F*)file_MuonISOrunGH->Get("LooseISO_TightID_pt_eta/abseta_pt_ratio");
    Bin1D_ISOrunGH = (TH1F*)file_MuonISOrunGH->Get("LooseISO_TightID_eta/eta_ratio");
    
    Bin2D_ISOrunBF->SetDirectory(0);
    Bin1D_ISOrunBF->SetDirectory(0);
    Bin2D_ISOrunGH->SetDirectory(0);
    Bin1D_ISOrunGH->SetDirectory(0);

    hpBin2D_ISOrunBF = (TH2F*)file_MuonISOrunBF->Get("tkLooseISO_highptID_newpt_eta/abseta_pair_ne_ratio");
    hpBin1D_ISOrunBF = (TH1F*)file_MuonISOrunBF->Get("tkLooseISO_highptID_eta/eta_ratio");
    hpBin2D_ISOrunGH = (TH2F*)file_MuonISOrunGH->Get("tkLooseISO_highptID_newpt_eta/abseta_pair_ne_ratio");
    hpBin1D_ISOrunGH = (TH1F*)file_MuonISOrunGH->Get("tkLooseISO_highptID_eta/eta_ratio");
    
    hpBin2D_ISOrunBF->SetDirectory(0);
    hpBin1D_ISOrunBF->SetDirectory(0);
    hpBin2D_ISOrunGH->SetDirectory(0);
    hpBin1D_ISOrunGH->SetDirectory(0);
    delete file_MuonISOrunBF; 

      
    if(!crab)
      jecUnc = new JetCorrectionUncertainty("/afs/cern.ch/work/f/federica/MSSM/Federica/CMSSW_8_0_26_patch1/src/MyAnalyzer/MiniAnalyzer/data/Summer16_23Sep2016V4_MC/Summer16_23Sep2016V4_MC_Uncertainty_AK4PF.txt");
    else
      jecUnc = new JetCorrectionUncertainty("src/MyAnalyzer/MiniAnalyzer/data/Summer16_23Sep2016V4_MC/Summer16_23Sep2016V4_MC_Uncertainty_AK4PF.txt");
  }
  else{   // per i dati
    if(!crab){ 
      jecUnc_BCD = new JetCorrectionUncertainty("/afs/cern.ch/work/f/federica/MSSM/Federica/CMSSW_8_0_26_patch1/src/MyAnalyzer/MiniAnalyzer/data/Summer16_23Sep2016V4_Data_BCD/Summer16_23Sep2016BCDV4_DATA_Uncertainty_AK4PF.txt");
      jecUnc_EF = new JetCorrectionUncertainty("/afs/cern.ch/work/f/federica/MSSM/Federica/CMSSW_8_0_26_patch1/src/MyAnalyzer/MiniAnalyzer/data/Summer16_23Sep2016V4_Data_EF/Summer16_23Sep2016EFV4_DATA_Uncertainty_AK4PF.txt");
      jecUnc_G = new JetCorrectionUncertainty("/afs/cern.ch/work/f/federica/MSSM/Federica/CMSSW_8_0_26_patch1/src/MyAnalyzer/MiniAnalyzer/data/Summer16_23Sep2016V4_Data_G/Summer16_23Sep2016GV4_DATA_Uncertainty_AK4PF.txt");
      jecUnc_H = new JetCorrectionUncertainty("/afs/cern.ch/work/f/federica/MSSM/Federica/CMSSW_8_0_26_patch1/src/MyAnalyzer/MiniAnalyzer/data/Summer16_23Sep2016V4_Data_H/Summer16_23Sep2016HV4_DATA_Uncertainty_AK4PF.txt");  
    }
    
    else{
      jecUnc_BCD = new JetCorrectionUncertainty("src/MyAnalyzer/MiniAnalyzer/data/Summer16_23Sep2016V4_Data_BCD/Summer16_23Sep2016BCDV4_DATA_Uncertainty_AK4PF.txt");
      jecUnc_EF = new JetCorrectionUncertainty("src/MyAnalyzer/MiniAnalyzer/data/Summer16_23Sep2016V4_Data_EF/Summer16_23Sep2016EFV4_DATA_Uncertainty_AK4PF.txt");
      jecUnc_G = new JetCorrectionUncertainty("src/MyAnalyzer/MiniAnalyzer/data/Summer16_23Sep2016V4_Data_G/Summer16_23Sep2016GV4_DATA_Uncertainty_AK4PF.txt");
      jecUnc_H = new JetCorrectionUncertainty("src/MyAnalyzer/MiniAnalyzer/data/Summer16_23Sep2016V4_Data_H/Summer16_23Sep2016HV4_DATA_Uncertainty_AK4PF.txt");
    }
  }
  
  //variables
  
 
  //they Have to be unitialized to 1
  mc_weight = 1.; 
  MyWeight = 1.;
  MyWeightUp = 1.;
  MyWeightDown = 1.;
  
  numEvents            = 0;
  numEventsAlPicco     = 0;
  numEvents_mcweighted = 0;
  numEvents_mcpuweighted = 0;
  numEvents_mcpuweightedup = 0;
  numEvents_mcpuweighteddown = 0;
  
  triggerEvents        = 0;
  goodMuonsEvents      = 0;
  triggerMatchedEvents = 0;
  SamePVEvents         = 0;
  
  mGenZ                = 0;
  mGenDimuon           = 0;
  
  generatedZ           = 0;
  generatedMuons       = 0;
  generatedMuons_acc   = 0;
  matchedMuons_trigger = 0;
  
  generatedEvents      = 0;
  generatedEventsAcc   = 0;
  triggeredEvents      = 0;
  triggerMatchEvents   = 0;
  
  RECO        = 0;
  TRIGGER     = 0;
  TIGHTID     = 0;
  GLOBALPTETA = 0;
  GLB         = 0;
  PF          = 0;
  CHI2        = 0;
  MATCHST     = 0;
  MUVALIDHITS = 0;
  DXY         = 0;
  DZ          = 0;
  PIXEL       = 0;
  TRACKER     = 0;
  ISO         = 0;
  MET         = 0;
  CAT1        = 0;
  CAT2        = 0;
  CAT1_mcpuweighted     = 0;
  CAT1_mcpuweightedup   = 0;
  CAT1_mcpuweighteddown = 0;
  CAT2_mcpuweighted     = 0;
  CAT2_mcpuweightedup   = 0;
  CAT2_mcpuweighteddown = 0;

  MEDIUM      = 0;
  METM        = 0;
  CAT1M       = 0;
  CAT2M       = 0;  

  /////temporary
  GenEvent2mu = 0;
  GenEventb = 0;
  GenEventHE = 0;
    
  GenMuPtTight = 0;
  GenMu = 0;

  //initialize the tree variables before assign them
  tree_ = fs->make<TTree>("MSSMtree", "H2mu Variables");
  
  masstree_ = fs->make<TTree>("MassTree", "Invariant mass of all the events"); 
  masstree_->Branch("MassCat1",&MassCat1, "MassCat1/F");
  masstree_->Branch("MassCat2",&MassCat2, "MassCat2/F");
  masstree_->Branch("MassCat11",&MassCat11, "MassCat11/F");
  masstree_->Branch("MassCat22",&MassCat22, "MassCat22/F");
  masstree_->Branch("MassCat111",&MassCat111, "MassCat111/F");
  masstree_->Branch("MassCat222",&MassCat222, "MassCat222/F");

  masstree1_ = fs->make<TTree>("MassTree1", "Invariant mass of cat1 events");
  masstree1_->Branch("MassCat1",&MassCat1, "MassCat1/F");
 
  masstree2_ = fs->make<TTree>("MassTree2", "Invariant mass of cat2 events");
  masstree2_->Branch("MassCat2",&MassCat2, "MassCat2/F");
  
  masstree11_ = fs->make<TTree>("MassTree11", "Invariant mass of cat1(only 1 bjet) events");
  masstree11_->Branch("MassCat11",&MassCat11, "MassCat11/F");
 
  masstree22_ = fs->make<TTree>("MassTree22", "Invariant mass of cat2(0,1 light jets) events");
  masstree22_->Branch("MassCat22",&MassCat22, "MassCat22/F");
  
  masstree111_ = fs->make<TTree>("MassTree111", "Invariant mass of cat1(2 bjets) events");
  masstree111_->Branch("MassCat111",&MassCat111, "MassCat111/F");
 
  masstree222_ = fs->make<TTree>("MassTree222", "Invariant mass of cat2(at least 2 light jets) events");
  masstree222_->Branch("MassCat222",&MassCat222, "MassCat222/F");
  
  if (youwantNTuples){  
  
    //Event info
    tree_->Branch("runnumber",&runnumber,"runnumber/I");
    tree_->Branch("lumiblock",&lumiblock,"lumiblock/I");
    tree_->Branch("eventNumber",&eventNumber,"eventNumber/l");
    tree_->Branch("timestamp",&timestamp,"timestamp/F");
    tree_->Branch("bunchXing",&bunchXing,"bunchXing/I");
    tree_->Branch("orbitNum",&orbitNum,"orbitNum/I");
    
    // tree_->Branch("rho", &RHO, "rho/D");
    // tree_->Branch("sigma", &SIGMA, "sigma/D");
    
    //Beamspot
    tree_->Branch("NBeam",&iBeam,"NBeam/S");
    tree_->Branch("BS_x",&BS_x,"BS_x/D");
    tree_->Branch("BS_y",&BS_y,"BS_y/D"); 
    tree_->Branch("BS_z",&BS_z,"BS_z/D");
    tree_->Branch("BS_xerr",&BS_xerr,"BS_xerr/D");
    tree_->Branch("BS_yerr",&BS_yerr,"BS_yerr/D");
    tree_->Branch("BS_zerr",&BS_zerr,"BS_zerr/D");
    tree_->Branch("BS_Wx",&BS_Wx,"BS_Wx/D"); 
    tree_->Branch("BS_Wy",&BS_Wy,"BS_Wy/D");
    tree_->Branch("BS_Wxerr",&BS_Wxerr,"BS_Wxerr/D");
    tree_->Branch("BS_Wyerr",&BS_Wyerr,"BS_Wyerr/D");
    tree_->Branch("BS_dxdz",&BS_dxdz,"BS_dxdz/D");
    tree_->Branch("BS_dxdzerr",&BS_dxdzerr,"BS_dxdzerr/D");
    tree_->Branch("BS_dydz",&BS_dydz,"BS_dydz/D");
    tree_->Branch("BS_dydzerr",&BS_dydzerr,"BS_dydzerr/D");
    
    //genparticle    
    if(MCtruth){  
      tree_->Branch("NGenP",&iGenP,"NGenP/S");
      tree_->Branch("Genp_particleId",&Genp_particleId);  
      tree_->Branch("Genp_status",&Genp_status);
      tree_->Branch("Genp_pt",&Genp_pt);
      tree_->Branch("Genp_p",&Genp_p);
      tree_->Branch("Genp_et",&Genp_et);
      tree_->Branch("Genp_e",&Genp_e);
      tree_->Branch("Genp_mt",&Genp_mt);
      tree_->Branch("Genp_m",&Genp_m);
      tree_->Branch("Genp_eta",&Genp_eta);
      tree_->Branch("Genp_phi",&Genp_phi);
      tree_->Branch("Genp_vx",&Genp_vx);
      tree_->Branch("Genp_vy",&Genp_vy);
      tree_->Branch("Genp_vz",&Genp_vz);
      
      tree_->Branch("Genp_nMmothers",&Genp_nMmothers);
      tree_->Branch("Genp_nDaughters",&Genp_nDaughters);
      
      tree_->Branch("Genp_particleId_mother",&Genp_particleId_mother);
      tree_->Branch("Genp_status_mother",&Genp_status_mother);
      tree_->Branch("Genp_pt_mother",&Genp_pt_mother);
      tree_->Branch("Genp_eta_mother",&Genp_eta_mother);
      tree_->Branch("Genp_phi_mother",&Genp_phi_mother);
      tree_->Branch("Genp_m_mother",&Genp_m_mother);      

      //Pileup
      tree_->Branch("PU_Weight",&MyWeight,"MyWeight/D");
      tree_->Branch("PU_WeightUp",&MyWeightUp,"MyWeightUp/D");
      tree_->Branch("PU_WeightDown",&MyWeightDown,"MyWeightDown/D");
    }
    
    //HLT
    tree_->Branch("Nhlt",&ihlt,"Nhlt/S");
    tree_->Branch("hlt_path",&hlt_path,32000,-1);
    
    //Primary vertex
    tree_->Branch("PV_x",&PV_x);
    tree_->Branch("PV_y",&PV_y);
    tree_->Branch("PV_z",&PV_z);
    tree_->Branch("PV_xerr",&PV_xerr);
    tree_->Branch("PV_yerr",&PV_yerr);
    tree_->Branch("PV_zerr",&PV_zerr);
    
    tree_->Branch("PV_normchi2",&PV_normchi2);
    tree_->Branch("PV_chi2",&PV_chi2);
    tree_->Branch("PV_ndof",&PV_ndof);
    tree_->Branch("PV_ntracks", &PV_ntracks);
    
    tree_->Branch("PV_validity",&PV_validity);
    tree_->Branch("PV_fake",&PV_fake);
    tree_->Branch("PV_SumPtTracks", &PV_SumPtTracks);
    
    tree_->Branch("NPVtx",&ivtx,"NPVtx/S"); 
    
    //MUON variables
    tree_->Branch("Mu_hasTriggeredIso",&Mu_hasTriggeredIso);	
    tree_->Branch("Mu_hasTriggeredIsoTk",&Mu_hasTriggeredIsoTk);	
    tree_->Branch("Mu_hasTriggeredMu50",&Mu_hasTriggeredMu50);
    tree_->Branch("Mu_hasTriggeredTkMu50",&Mu_hasTriggeredTkMu50);
    tree_->Branch("Mu_pt",&Mu_pt_mu);
    tree_->Branch("Mu_px",&Mu_px_mu);
    tree_->Branch("Mu_py",&Mu_py_mu);
    tree_->Branch("Mu_pz",&Mu_pz_mu);
    tree_->Branch("Mu_en",&Mu_en_mu);
    tree_->Branch("Mu_phi",&Mu_phi_mu);
    tree_->Branch("Mu_eta",&Mu_eta_mu);
    tree_->Branch("Mu_charge", &Mu_charge_mu);

    tree_->Branch("Mu_triggerSF_BF",&Mu_triggerSF_BF);
    tree_->Branch("Mu_triggerSF_GH", &Mu_triggerSF_GH);
    tree_->Branch("Mu_idSF_BF",&Mu_idSF_BF);
    tree_->Branch("Mu_idSF_GH",&Mu_idSF_GH);
    tree_->Branch("Mu_isoSF_BF",&Mu_isoSF_BF);
    tree_->Branch("Mu_isoSF_GH",&Mu_isoSF_GH);
    tree_->Branch("Mu_hptriggerSF_BF",&Mu_hptriggerSF_BF);
    tree_->Branch("Mu_hptriggerSF_GH", &Mu_hptriggerSF_GH);
    tree_->Branch("Mu_hpidSF_BF",&Mu_hpidSF_BF);
    tree_->Branch("Mu_hpidSF_GH",&Mu_hpidSF_GH);
    tree_->Branch("Mu_hpisoSF_BF",&Mu_hpisoSF_BF);
    tree_->Branch("Mu_hpisoSF_GH",&Mu_hpisoSF_GH);

    tree_->Branch("Mu_roch_correction", &Mu_roch_correction);
    
    tree_->Branch("Mu_vertex_vx",&Mu_vertex_vx);
    tree_->Branch("Mu_vertex_vy",&Mu_vertex_vy);
    tree_->Branch("Mu_vertex_vz",&Mu_vertex_vz);
    
    tree_->Branch("Mu_isPFMu",&Mu_isPFMu);
    tree_->Branch("Mu_isMediumMu",&Mu_isMediumMu);
    tree_->Branch("Mu_isTightMu",&Mu_isTightMu);
    tree_->Branch("Mu_isHighPtMu",&Mu_isHighPtMu);

    tree_->Branch("Mu_isMuGlobal",&Mu_isMuGlobal);
    tree_->Branch("Mu_isMuTracker",&Mu_isMuTracker);
    tree_->Branch("Mu_isMuStandAlone",&Mu_isMuStandAlone);
    tree_->Branch("Mu_numberOfChambers",&Mu_numberOfChambers);
    tree_->Branch("Mu_numberOfMatches",&Mu_numberOfMatches);
    tree_->Branch("Mu_numberOfMatchedStation",&Mu_numberOfMatchedStation);
    tree_->Branch("Mu_dB",&Mu_dB);
    tree_->Branch("Mu_stationMask",&Mu_stationMask);
    tree_->Branch("Mu_numberOfMatchedRPCLayers",&Mu_numberOfMatchedRPCLayers);

    tree_->Branch("Mu_timingVeto",&Mu_timingVeto);    

    tree_->Branch("Mu_sumPtIsoR03",&Mu_sumPtIsoR03);
    tree_->Branch("Mu_ntkIsoR03",&Mu_ntkIsoR03);
    tree_->Branch("Mu_emIsoR03",&Mu_emIsoR03);
    tree_->Branch("Mu_hadIsoR03",&Mu_hadIsoR03);
    tree_->Branch("Mu_hoEtIsoR03",&Mu_hoEtIsoR03);
    tree_->Branch("Mu_nJetsIsoR03", &Mu_nJetsIsoR03);
    
    tree_->Branch("Mu_sumPtIsoR05",&Mu_sumPtIsoR05);
    tree_->Branch("Mu_ntkIsoR05",&Mu_ntkIsoR05);
    tree_->Branch("Mu_emIsoR05",&Mu_emIsoR05);
    tree_->Branch("Mu_hadIsoR05",&Mu_hadIsoR05);
    tree_->Branch("Mu_hoEtIsoR05",&Mu_hoEtIsoR05);
    tree_->Branch("Mu_nJetsIsoR05", &Mu_nJetsIsoR05);
    
    tree_->Branch("Mu_sumCHPtPFIsoR04",&Mu_sumCHPtPFIsoR04);
    tree_->Branch("Mu_sumCPPtPFIsoR04",&Mu_sumCPPtPFIsoR04);
    tree_->Branch("Mu_sumNHPtPFIsoR04",&Mu_sumNHPtPFIsoR04);
    tree_->Branch("Mu_sumPhoEtPFIsoR04",&Mu_sumPhoEtPFIsoR04);
    tree_->Branch("Mu_sumPUPtPFIsoR04",&Mu_sumPUPtPFIsoR04);

    tree_->Branch("Mu_sumCHPtPFIsoR03",&Mu_sumCHPtPFIsoR03);
    tree_->Branch("Mu_sumCPPtPFIsoR03",&Mu_sumCPPtPFIsoR03);
    tree_->Branch("Mu_sumNHPtPFIsoR03",&Mu_sumNHPtPFIsoR03);
    tree_->Branch("Mu_sumPhoEtPFIsoR03",&Mu_sumPhoEtPFIsoR03);
    tree_->Branch("Mu_sumPUPtPFIsoR03",&Mu_sumPUPtPFIsoR03);
    
    tree_->Branch("Mu_calEnergyEm", &Mu_calEnergyEm);
    tree_->Branch("Mu_calEnergyHad",&Mu_calEnergyHad);
    tree_->Branch("Mu_calEnergyHo",&Mu_calEnergyHo);
    tree_->Branch("Mu_calEnergyEmS9",&Mu_calEnergyEmS9);
    tree_->Branch("Mu_calEnergyHadS9",&Mu_calEnergyHadS9);
    tree_->Branch("Mu_calEnergyHoS9",&Mu_calEnergyHoS9);
    
    //standalone
    tree_->Branch("Mu_numberOfHits_sta",&STAMu_numberOfHits);
    tree_->Branch("Mu_recHitsSize",&STAMu_recHitsSize);
    tree_->Branch("Mu_normchi2_sta",&STAMu_normchi2Mu);
    tree_->Branch("Mu_dxy_sta",&STAMu_dxyMu);
    tree_->Branch("Mu_dz_sta",&STAMu_dzMu);
    tree_->Branch("Mu_vx_sta",&STAMu_vertex_vx);
    tree_->Branch("Mu_vy_sta",&STAMu_vertex_vy);
    tree_->Branch("Mu_vz_sta",&STAMu_vertex_vz);
  
    //global
    tree_->Branch("GLBMu_pt", &GLBMu_pt);
    tree_->Branch("GLBMu_pt_err", &GLBMu_pt_err);
    tree_->Branch("GLBMu_eta", &GLBMu_eta);
    tree_->Branch("GLBMu_phi", &GLBMu_phi);
    tree_->Branch("GLBMu_chi2", &GLBMu_chi2);
    tree_->Branch("GLBMu_ndf", &GLBMu_ndf);
    tree_->Branch("GLBMu_qOverPt", &GLBMu_qOverPt);
    tree_->Branch("Mu_normchi2_glb",&GLBMu_normchi2Mu);
    tree_->Branch("Mu_dxy_glb",&GLBMu_dxyMu);
    tree_->Branch("Mu_dz_glb",&GLBMu_dzMu);
    tree_->Branch("Mu_numberOfPixelHits_glb",&GLBMu_numberOfPixelHits);
    tree_->Branch("Mu_numberOfTrackerHits_glb",&GLBMu_numberOfTrackerHits);
    tree_->Branch("Mu_numberOfMuonsHits_glb",&GLBMu_numberOfMuonsHits);
    tree_->Branch("Mu_vx_glb",&GLBMu_vertex_vx);
    tree_->Branch("Mu_vy_glb",&GLBMu_vertex_vy);
    tree_->Branch("Mu_vz_glb",&GLBMu_vertex_vz);
    

    //tracker
    tree_->Branch("TRKMu_pt", &TRKMu_pt);
    tree_->Branch("TRKMu_pt_err", &TRKMu_pt_err);
    tree_->Branch("TRKMu_eta", &TRKMu_eta);
    tree_->Branch("TRKMu_phi", &TRKMu_phi);
    tree_->Branch("TRKMu_chi2", &TRKMu_chi2);
    tree_->Branch("TRKMu_ndf", &TRKMu_ndf);
    tree_->Branch("TRKMu_qOverPt", &TRKMu_qOverPt);
    tree_->Branch("Mu_normchi2_trk",&TRKMu_normchi2Mu);
    tree_->Branch("Mu_dxy_trk",&TRKMu_dxyMu);
    tree_->Branch("Mu_dz_trk",&TRKMu_dzMu);
    tree_->Branch("Mu_numberOfPixelHits_trk",&TRKMu_numberOfPixelHits);
    tree_->Branch("Mu_numberOfTrackerHits_trk",&TRKMu_numberOfTrackerHits);
    tree_->Branch("Mu_dzPV_trk",&TRKMu_dzPV);
    tree_->Branch("Mu_trackerLayersWithMeasurement_trk",&TRKMu_trackerLayersWithMeasurement);

    tree_->Branch("Nmuons",&imuons,"Nmuons/S");
    tree_->Branch("NGlobalMuons",&GlobalMuons,"NGlobalMuons/S");
    tree_->Branch("NTrackerMuons",&TrackerMuons,"NTrackerMuons/S");
    tree_->Branch("NStandAloneMuons",&StandAloneMuons,"NStandAloneMuons/S");
    
    //tunep
    tree_->Branch("TPMu_pt", &TPMu_pt);
    tree_->Branch("TPMu_pt_err", &TPMu_pt_err);
    tree_->Branch("TPMu_eta", &TPMu_eta);
    tree_->Branch("TPMu_phi", &TPMu_phi);
    tree_->Branch("TPMu_chi2", &TPMu_chi2);
    tree_->Branch("TPMu_ndf", &TPMu_ndf);
    tree_->Branch("TPMu_qOverPt", &TPMu_qOverPt);

    // PFELECTRONS
    // tree_->Branch("Ele_pt_pfele",&Ele_pt_pfele);
    
    // tree_->Branch("Ele_px_pfele",&Ele_px_pfele);
    // tree_->Branch("Ele_py_pfele",&Ele_py_pfele);
    // tree_->Branch("Ele_pz_pfele",&Ele_pz_pfele);
    // tree_->Branch("Ele_en_pfele",&Ele_en_pfele);
    // tree_->Branch("Ele_phi_pfele",&Ele_phi_pfele);  
    // tree_->Branch("Ele_eta_pfele",&Ele_eta_pfele);
   

    // PFJET
    tree_->Branch("NpfJets",&ipfjets,"NpfJets/S");
    tree_->Branch("NbTagHE_pfJets",&bTagJetsHP,"NbTagHE_pfJets/S");
    tree_->Branch("NbTagHP_pfJets",&bTagJetsHP,"NbTagHP_pfJets/S");
    
    tree_->Branch("Jet_PUmva_pfjet",&Jet_PUmva_pfjet);
    tree_->Branch("Jet_PULoose_pfjet",&Jet_PULoose_pfjet);
    tree_->Branch("Jet_PUMedium_pfjet",&Jet_PUMedium_pfjet);
    tree_->Branch("Jet_PUTight_pfjet",&Jet_PUTight_pfjet);

    tree_->Branch("Jet_pt_pfjet",&Jet_pt_pfjet);
    tree_->Branch("Jet_ptL5_pfjet",&Jet_ptL5_pfjet);
    tree_->Branch("Jet_ptL7_pfjet",&Jet_ptL7_pfjet);  
    tree_->Branch("Jet_px_pfjet",&Jet_px_pfjet);
    tree_->Branch("Jet_py_pfjet",&Jet_py_pfjet);
    tree_->Branch("Jet_pz_pfjet",&Jet_pz_pfjet);
    tree_->Branch("Jet_en_pfjet",&Jet_en_pfjet);
    tree_->Branch("Jet_phi_pfjet",&Jet_phi_pfjet);  
    tree_->Branch("Jet_eta_pfjet",&Jet_eta_pfjet);
    tree_->Branch("Jet_Area_pfjet",&Jet_Area_pfjet);
    tree_->Branch("Jet_JECunc_pfjet", &Jet_JECunc_pfjet);

    tree_->Branch("Jet_isLoose_pfjet",&Jet_isLoose_pfjet);    
    tree_->Branch("Jet_isTight_pfjet",&Jet_isTight_pfjet);    
    tree_->Branch("Jet_isTightLepVeto_pfjet",&Jet_isTightLepVeto_pfjet);    

    tree_->Branch("Jet_ChargedHadEn_pfjet",&Jet_ChargedHadEn_pfjet);
    tree_->Branch("Jet_NeutralHadEn_pfjet",&Jet_NeutralHadEn_pfjet);
    tree_->Branch("Jet_ChargedEmEn_pfjet",&Jet_ChargedEmEn_pfjet);
    tree_->Branch("Jet_ChargedMuEn_pfjet",&Jet_ChargedMuEn_pfjet);
    tree_->Branch("Jet_NeutralEmEn_pfjet",&Jet_NeutralEmEn_pfjet);
    tree_->Branch("Jet_ChargedMultiplicity_pfjet",&Jet_ChargedMultiplicity_pfjet);
    tree_->Branch("Jet_NeutralMultiplicity_pfjet",&Jet_NeutralMultiplicity_pfjet);
    tree_->Branch("Jet_MuonMultiplicity_pfjet",&Jet_MuonMultiplicity_pfjet);   
    tree_->Branch("Jet_ElectronMultiplicity_pfjet",&Jet_ElectronMultiplicity_pfjet);  
    
    tree_->Branch("Jet_discriminatorHE_pfjet",&Jet_discriminatorHE_pfjet);
    tree_->Branch("Jet_discriminatorHP_pfjet",&Jet_discriminatorHP_pfjet);
    tree_->Branch("Jet_discriminatorCSV_pfjet",&Jet_discriminatorCSV_pfjet);
           
    //PFMET
    tree_->Branch("NpfMet",&ipfmets,"NpfMet/S");
    tree_->Branch("Met_pt_pfmet",&Met_pt_pfmet);
    tree_->Branch("Met_phi_pfmet",&Met_phi_pfmet);
    tree_->Branch("Met_eta_pfmet", &Met_eta_pfmet);
    tree_->Branch("Met_energy_pfmet", &Met_energy_pfmet);
    tree_->Branch("Met_sumet_pfmet", &Met_sumet_pfmet);
    tree_->Branch("Met_ptsignificance_pfmet", &Met_ptsignificance_pfmet);
    tree_->Branch("Met_etsignificance_pfmet", &Met_etsignificance_pfmet);
    
    tree_->Branch("Met_type01smear_pt_pfmet", &Met_type01smear_pt_pfmet);
    tree_->Branch("Met_totUp_pt_pfmet", &Met_totUp_pt_pfmet);      
    tree_->Branch("Met_totDown_pt_pfmet", &Met_totDown_pt_pfmet); // mi sa che si sono sbagliati proprio a chiamarla centralmente


    tree_->Branch("Met_jetEnUp_pfmet", &Met_jetEnUp_pfmet);
    tree_->Branch("Met_jetEnDown_pfmet", &Met_jetEnDown_pfmet);
    tree_->Branch("Met_jetResUp_pfmet", &Met_jetResUp_pfmet);
    tree_->Branch("Met_jetResDown_pfmet", &Met_jetResDown_pfmet);
    tree_->Branch("Met_unclusterUp_pfmet", &Met_unclusterUp_pfmet);
    tree_->Branch("Met_unclusterDown_pfmet", &Met_unclusterDown_pfmet);  
    tree_->Branch("Met_tauUp_pfmet", &Met_tauUp_pfmet);
    tree_->Branch("Met_tauDown_pfmet", &Met_tauDown_pfmet);  
    tree_->Branch("Met_eleUp_pfmet", &Met_eleUp_pfmet);
    tree_->Branch("Met_eleDown_pfmet", &Met_eleDown_pfmet);  
    tree_->Branch("Met_photonUp_pfmet", &Met_photonUp_pfmet);
    tree_->Branch("Met_photonDown_pfmet", &Met_photonDown_pfmet);  
    tree_->Branch("Met_muUp_pfmet", &Met_muUp_pfmet);
    tree_->Branch("Met_muDown_pfmet", &Met_muDown_pfmet);


    // tree_->Branch("Met_cHADEtFraction_pfmet", &Met_cHADEtFraction_pfmet);
    // tree_->Branch("Met_nHADEtFraction_pfmet", &Met_nHADEtFraction_pfmet);
    
    // tree_->Branch("Met_pt_calomet", &Met_pt_calomet);
    // tree_->Branch("Met_phi_calomet", &Met_phi_calomet);
    
    tree_->Branch("Met_phi_umet", &Met_phi_umet);
    tree_->Branch("Met_pt_umet", &Met_pt_umet);
    tree_->Branch("Met_sumet_umet", &Met_sumet_umet);
    
  }

  
}

  // ------------ method called once each job just after ending the event loop  ------------
  void 
    MiniAnalyzer::endJob() {

    histCutEff->SetBinContent(2,TRIGGER/RECO);
    histCutEff->SetBinContent(3,TIGHTID/RECO);
    histCutEff->SetBinContent(4,ISO/RECO);
    histCutEff->SetBinContent(5,MET/RECO);
    histCutEff->SetBinContent(6,CAT1/RECO);
    histCutEff->SetBinContent(7,CAT2/RECO);
    histCutEff->SetBinContent(8,0.);
    histCutEff->SetBinContent(9,MEDIUM/RECO);
    histCutEff->SetBinContent(10,METM/RECO);
    histCutEff->SetBinContent(11,CAT1M/RECO);
    histCutEff->SetBinContent(12,CAT2M/RECO);    

    histEff->SetBinContent(2,TRIGGER/RECO);
    histEff->SetBinContent(3,TIGHTID/TRIGGER);
    histEff->SetBinContent(4,ISO/TIGHTID);
    histEff->SetBinContent(5,MET/ISO);
    histEff->SetBinContent(6,CAT1/MET);
    histEff->SetBinContent(7,CAT2/MET);
    histEff->SetBinContent(8,0.);
    histEff->SetBinContent(9,MEDIUM/RECO);
    histEff->SetBinContent(10,METM/MEDIUM);
    histEff->SetBinContent(11,CAT1M/METM);
    histEff->SetBinContent(12,CAT2M/METM);  


    std::cout << " *********** F I N A L ** R E P O R T ************ " << std::endl;

    std::cout << "  Eventi non ripesati: "  << numEvents << " Al picco " << numEventsAlPicco++ << "   CAT1 = " << CAT1 << " CAT2 = " << CAT2 << "  tot = " << CAT1+CAT2<<std::endl;
    std::cout << "   numEvents_mcweighted " <<  numEvents_mcweighted << std::endl;
    std::cout << "   numEvents_mcpuweighted " <<  numEvents_mcpuweighted << "  Cat1+Cat2 " <<  CAT1_mcpuweighted+CAT2_mcpuweighted << "  Eff = " << (CAT1_mcpuweighted+CAT2_mcpuweighted)/numEvents_mcpuweighted <<std::endl;
    std::cout << "   numEvents_mcpuweightedup "   <<numEvents_mcpuweightedup << "  Cat1+Cat2 " <<  CAT1_mcpuweightedup+CAT2_mcpuweightedup << "  Eff = " << (CAT1_mcpuweightedup+CAT2_mcpuweightedup)/numEvents_mcpuweightedup<< std::endl;
    std::cout << "   numEvents_mcpuweighteddown "  << numEvents_mcpuweighteddown << "  Cat1+Cat2 " <<  CAT1_mcpuweighteddown+CAT2_mcpuweighteddown << "  Eff = " << (CAT1_mcpuweighteddown+CAT2_mcpuweighteddown)/numEvents_mcpuweighteddown<< std::endl;


    histCounter->SetBinContent(1, numEvents);
    histCounter->SetBinContent(2, numEvents_mcweighted);
    histCounter->SetBinContent(3, numEvents_mcpuweighted);
    histCounter->SetBinContent(4, numEvents_mcpuweightedup);
    histCounter->SetBinContent(5, numEvents_mcpuweighteddown);

    histCounter->SetBinContent(6,0.);
    histCounter->SetBinContent(7,0.);
    histCounter->SetBinContent(8,RECO);
    histCounter->SetBinContent(9,TRIGGER);
    histCounter->SetBinContent(10,GLOBALPTETA);
    histCounter->SetBinContent(11,TIGHTID);
    histCounter->SetBinContent(12,ISO);
    histCounter->SetBinContent(13,MET);
    histCounter->SetBinContent(14,CAT1);
    histCounter->SetBinContent(15,CAT2);
    histCounter->SetBinContent(16,0.);
    histCounter->SetBinContent(17,MEDIUM);
    histCounter->SetBinContent(18,METM);
    histCounter->SetBinContent(19,CAT1M);
    histCounter->SetBinContent(20,CAT2M);  


    std::cout << "Eventi con 2 mu = " << RECO << " Triggered  = " << TRIGGER << " Tight = " << TIGHTID << " Isolated = " << ISO << " After MET cut " << MET << " Cat1 " << CAT1 << " Cat2 " << CAT2 << std::endl;
    std::cout << "Eventi con 2 Iso Medium = " << MEDIUM << " After MET cut " << METM << " Cat1 " << CAT1M << " Cat2 " << CAT2M << std::endl;

    std::cout << " Found " << generatedZ << " generated Z giving " << generatedMuons <<
      " status 1 muons. " << generatedMuons_acc << " are within our acceptance: " << generatedMuons_acc/generatedMuons << std::endl; 

    std::cout << " Eventi con 2 mu " << GenEvent2mu << " con anche i b " << GenEventb << " con b ad eta > 2.4 " << GenEventHE << std::endl; 
     
  }


  inline void MiniAnalyzer::clear_Arrays(){

    MyWeight = 1.;
    MyWeightUp = 1.;
    MyWeightDown = 1.;
    EventWeight = 1.;
    mc_weight = 1.;
  
    if(youwantAnalysis){  
      tmu_1             .clear();
      tmu_2             .clear();
    
      tMass_2mucut      .clear();
      tMass_MEtcut      .clear();
      tMass_final_1jet  .clear();
      tMass_final_jetVeto.clear();
    
      tMEt_2mucut       .clear();
      tMEt_MEtcut       .clear();
    
      tJet_pt_2mucut    .clear();
      tJet_eta_2mucut   .clear();
      tJet_bDisc_2mucut .clear();
      tJet_pt_MEtcut    .clear();
      tJet_eta_MEtcut   .clear();
      tJet_bDisc_MEtcut .clear();
      tJet_pt_bcut      .clear();
      tJet_eta_bcut     .clear();
      tJet_bDisc_bcut   .clear();
    
      tJet_Leadingpt_bcut    .clear();
      tJet_LeadingptL7_bcut  .clear();
      tJet_Leadingeta_bcut   .clear();
      tJet_LeadingbDisc_bcut .clear();
      tJet_Secondpt_bcut    .clear();
      tJet_SecondptL7_bcut  .clear();
      tJet_Secondeta_bcut   .clear();
      tJet_SecondbDisc_bcut .clear();
    } 
  
    if(youwantNTuples){  
      //HLT
      hlt_path.clear();
    
      //primary vertex
      PV_x.clear();
      PV_y.clear(); 
      PV_z.clear();
      PV_xerr.clear();
      PV_yerr.clear();
      PV_zerr.clear();
      PV_validity.clear();
      PV_fake.clear();
      PV_normchi2.clear();
      PV_chi2.clear();
      PV_ndof.clear();
      PV_ntracks.clear();
      PV_SumPtTracks.clear();
    
      //muon variables
      Mu_hasTriggeredIso.clear();
      Mu_hasTriggeredIsoTk.clear();
      Mu_hasTriggeredMu50.clear();
      Mu_hasTriggeredTkMu50.clear();
      Mu_isPFMu.clear();
      Mu_isTightMu.clear();
      Mu_isHighPtMu.clear();
      Mu_isMediumMu.clear();
      Mu_isMuGlobal.clear();
      Mu_isMuTracker.clear();
      Mu_isMuStandAlone.clear();
      Mu_numberOfChambers.clear();
      Mu_numberOfMatches.clear();
      Mu_numberOfMatchedStation.clear();
      Mu_dB.clear();
      Mu_stationMask.clear();
      Mu_numberOfMatchedRPCLayers.clear(); 
      Mu_timingVeto.clear();   
    
      Mu_pt_mu.clear();
      Mu_px_mu.clear();
      Mu_py_mu.clear();
      Mu_pz_mu.clear();
      Mu_en_mu.clear();
      Mu_phi_mu.clear();
      Mu_eta_mu.clear();
      Mu_charge_mu.clear();
    
      Mu_vertex_vx.clear();
      Mu_vertex_vy.clear();
      Mu_vertex_vz.clear();
    
      Mu_sumPtIsoR03.clear();
      Mu_ntkIsoR03.clear();
      Mu_emIsoR03.clear();
      Mu_hadIsoR03.clear();
      Mu_hoEtIsoR03.clear();
      Mu_nJetsIsoR03.clear();
      Mu_sumPtIsoR05.clear();
      Mu_ntkIsoR05.clear();
      Mu_emIsoR05.clear();
      Mu_hadIsoR05.clear();
      Mu_hoEtIsoR05.clear();
      Mu_nJetsIsoR05.clear();
    
      Mu_sumCHPtPFIsoR04.clear();
      Mu_sumCPPtPFIsoR04.clear();
      Mu_sumNHPtPFIsoR04.clear();
      Mu_sumPhoEtPFIsoR04.clear();
      Mu_sumPUPtPFIsoR04.clear();

      Mu_sumCHPtPFIsoR03.clear();
      Mu_sumCPPtPFIsoR03.clear();
      Mu_sumNHPtPFIsoR03.clear();
      Mu_sumPhoEtPFIsoR03.clear();
      Mu_sumPUPtPFIsoR03.clear();    

      Mu_calEnergyEm.clear();
      Mu_calEnergyHad.clear();
      Mu_calEnergyHo.clear();
      Mu_calEnergyEmS9.clear();
      Mu_calEnergyHadS9.clear();
      Mu_calEnergyHoS9.clear();
    
      STAMu_numberOfHits.clear();
      STAMu_recHitsSize.clear();
      STAMu_normchi2Mu.clear();
      STAMu_dxyMu.clear();
      STAMu_dzMu.clear();
      STAMu_vertex_vx.clear();
      STAMu_vertex_vy.clear();
      STAMu_vertex_vz.clear();
    
      GLBMu_pt.clear();
      GLBMu_pt_err.clear();
      GLBMu_eta.clear();
      GLBMu_phi.clear();
      GLBMu_chi2.clear();
      GLBMu_ndf.clear();
      GLBMu_qOverPt.clear();
      GLBMu_normchi2Mu.clear();
      GLBMu_dxyMu.clear();
      GLBMu_dzMu.clear();
      GLBMu_numberOfPixelHits.clear();
      GLBMu_numberOfTrackerHits.clear();
      GLBMu_numberOfMuonsHits.clear();
      GLBMu_vertex_vx.clear();
      GLBMu_vertex_vy.clear();
      GLBMu_vertex_vz.clear();
    
      TRKMu_pt.clear();
      TRKMu_pt_err.clear();
      TRKMu_eta.clear();
      TRKMu_phi.clear();
      TRKMu_chi2.clear();
      TRKMu_ndf.clear();
      TRKMu_qOverPt.clear();
      TRKMu_normchi2Mu.clear();
      TRKMu_dxyMu.clear();
      TRKMu_dzMu.clear();
      TRKMu_numberOfPixelHits.clear();
      TRKMu_numberOfTrackerHits.clear();
      TRKMu_dzPV.clear();
      TRKMu_trackerLayersWithMeasurement.clear();
    
      TPMu_pt.clear();
      TPMu_pt_err.clear();
      TPMu_eta.clear();
      TPMu_phi.clear();
      TPMu_chi2.clear();
      TPMu_ndf.clear();
      TPMu_qOverPt.clear();

      Mu_triggerSF_BF.clear();
      Mu_triggerSF_GH.clear();
      Mu_idSF_BF.clear();
      Mu_idSF_GH.clear();
      Mu_isoSF_BF.clear();
      Mu_isoSF_GH.clear();
      Mu_hptriggerSF_BF.clear();
      Mu_hptriggerSF_GH.clear();
      Mu_hpidSF_BF.clear();
      Mu_hpidSF_GH.clear();
      Mu_hpisoSF_BF.clear();
      Mu_hpisoSF_GH.clear();

      Mu_roch_correction.clear();

      //PFELECTRONS
      // Ele_pt_pfele.clear();
      // Ele_px_pfele.clear();
      // Ele_py_pfele.clear();
      // Ele_pz_pfele.clear();
      // Ele_en_pfele.clear();
      // Ele_phi_pfele.clear();  
      // Ele_eta_pfele.clear();

      //PFJET
      Jet_PUmva_pfjet.clear();
      Jet_PULoose_pfjet.clear();
      Jet_PUMedium_pfjet.clear();
      Jet_PUTight_pfjet.clear();

      Jet_pt_pfjet.clear();
      Jet_ptL5_pfjet.clear();
      Jet_ptL7_pfjet.clear();
      Jet_px_pfjet.clear();
      Jet_py_pfjet.clear();
      Jet_pz_pfjet.clear();
      Jet_en_pfjet.clear();
      Jet_phi_pfjet.clear();  
      Jet_eta_pfjet.clear();
      Jet_Area_pfjet.clear();
      Jet_JECunc_pfjet.clear();

      Jet_isLoose_pfjet.clear();    
      Jet_isTight_pfjet.clear();    
      Jet_isTightLepVeto_pfjet.clear();    
    
      Jet_ChargedHadEn_pfjet.clear();
      Jet_NeutralHadEn_pfjet.clear();
      Jet_ChargedEmEn_pfjet.clear();
      Jet_ChargedMuEn_pfjet.clear();
      Jet_NeutralEmEn_pfjet.clear();
      Jet_ChargedMultiplicity_pfjet.clear();
      Jet_NeutralMultiplicity_pfjet.clear();
      Jet_MuonMultiplicity_pfjet.clear();   
      Jet_ElectronMultiplicity_pfjet.clear();   
    
      Jet_discriminatorHE_pfjet.clear();
      Jet_discriminatorHP_pfjet.clear();
      Jet_discriminatorCSV_pfjet.clear();
         
      //PFMET
      Met_pt_pfmet.clear();
      Met_phi_pfmet.clear();
      Met_eta_pfmet.clear();
      Met_energy_pfmet.clear();
      Met_sumet_pfmet.clear();
      Met_ptsignificance_pfmet.clear();
      Met_etsignificance_pfmet.clear();
    
      Met_type01smear_pt_pfmet.clear();
      Met_totUp_pt_pfmet.clear();      
      Met_totDown_pt_pfmet.clear();

      Met_jetEnUp_pfmet.clear();
      Met_jetEnDown_pfmet.clear();
      Met_jetResUp_pfmet.clear();
      Met_jetResDown_pfmet.clear();
      Met_unclusterUp_pfmet.clear();
      Met_unclusterDown_pfmet.clear();  
      Met_tauUp_pfmet.clear();
      Met_tauDown_pfmet.clear();  
      Met_eleUp_pfmet.clear();
      Met_eleDown_pfmet.clear();  
      Met_photonUp_pfmet.clear();
      Met_photonDown_pfmet.clear();  
      Met_muUp_pfmet.clear();
      Met_muDown_pfmet.clear();


      // mi sa che si sono sbagliati proprio a chiamarla centralmente
      // Met_cHADEtFraction_pfmet.clear();
      // Met_nHADEtFraction_pfmet.clear();
    
      // Met_pt_calomet.clear();
      // Met_phi_calomet.clear();
     
      Met_phi_umet.clear();
      Met_pt_umet.clear();
      Met_sumet_umet.clear();
    
    
      //genparticle
      if(MCtruth){
	Genp_particleId.clear();  
	Genp_status.clear();
	Genp_pt.clear();
	Genp_p.clear();
	Genp_et.clear();
	Genp_e.clear();
	Genp_mt.clear();
	Genp_m.clear();
	Genp_eta.clear();
	Genp_phi.clear();
	Genp_vx.clear();
	Genp_vy.clear();
	Genp_vz.clear();  
	Genp_nMmothers.clear();
	Genp_nDaughters.clear();
      
	Genp_particleId_mother.clear();
	Genp_status_mother.clear();
	Genp_pt_mother.clear();
	Genp_eta_mother.clear();
	Genp_phi_mother.clear();
	Genp_m_mother.clear();
      }
    }
    return;
  }



  void MiniAnalyzer::initialize_Tree_variables(){
 
    if(youwantNTuples){  
      //Event variables
      runnumber   = 0;
      lumiblock   = 0;
      eventNumber = 0;
      timestamp   = 0.;
      bunchXing   = 0;
      orbitNum    = 0;  
    
      BS_x        = 0;
      BS_y        = 0;
      BS_z        = 0;
      BS_xerr     = 0;
      BS_yerr     = 0;
      BS_zerr     = 0;
      BS_Wx       = 0;
      BS_Wxerr    = 0;
      BS_Wy       = 0;
      BS_Wyerr    = 0;
      BS_dxdz     = 0;
      BS_dxdzerr  = 0;
      BS_dydz     = 0;
      BS_dydzerr  = 0;

      MassCat1    = 0;
      MassCat2    = 0;
      MassCat11   = 0;
      MassCat22   = 0;
      MassCat111  = 0;
      MassCat222  = 0;
      //  RHO         = 0;
      //SIGMA       = 0;
    }

  }

  // ------------ method called when starting to processes a run  ------------
  /*
    void 
    MiniAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
    {
    }
  */

  // ------------ method called when ending the processing of a run  ------------
  /*
    void 
    MiniAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
    {
    }
  */

  // ------------ method called when starting to processes a luminosity block  ------------
  /*
    void 
    MiniAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
    {
    }
  */

  // ------------ method called when ending the processing of a luminosity block  ------------
  /*
    void 
    MiniAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
    {
    }
  */

  // ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
  void
    MiniAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
  }

  bool MiniAnalyzer::isAncestor(const reco::Candidate* ancestor, const reco::Candidate * particle){
    //particle is already the ancestor
    if(ancestor == particle ) return true;
  
    //otherwise loop on mothers, if any and return true if the ancestor is found
    for(size_t i=0;i< particle->numberOfMothers();i++){
      if(isAncestor(ancestor,particle->mother(i))) return true;
    }
    //if we did not return yet, then particle and ancestor are not relatives
    return false;
  }

  //define this as a plug-in
  DEFINE_FWK_MODULE(MiniAnalyzer);

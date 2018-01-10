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
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

// ROOT
#include "TH1.h"
#include "TMath.h"
#include "TLorentzVector.h"

//
// class declaration
//


//
// class declaration
//

class MiniMiniAnalyzer : public edm::EDAnalyzer {
public:
  explicit MiniMiniAnalyzer(const edm::ParameterSet&);
  ~MiniMiniAnalyzer();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  bool isAncestor(const reco::Candidate * ancestor, const reco::Candidate * particle);
   
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  // ----------member data ---------------------------
  edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
  edm::EDGetTokenT<pat::MuonCollection> muonToken_;
  // edm::EDGetTokenT<pat::ElectronCollection> electronToken_;
  // edm::EDGetTokenT<pat::PhotonCollection> photonToken_;
  // edm::EDGetTokenT<pat::JetCollection> jetToken_;
  // edm::EDGetTokenT<pat::JetCollection> fatjetToken_;
  // edm::EDGetTokenT<pat::METCollection> metToken_;

  edm::EDGetTokenT<edm::View<reco::GenParticle> > prunedGenToken_;
  edm::EDGetTokenT<edm::View<pat::PackedGenParticle> > packedGenToken_;  //status 1 particles
  
  edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
  edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjects_;
  edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;  

  std::string pathNames; 
  std::string filterLastName; 

  bool customizedMINIAOD;
  bool debug;

  //histos
  TH1D *histAllVertZPos;
  TH1D *histAllMuons_pt;
  TH1D *histTunePMuons_pt;
  TH1D *histBestMuons_pt;
  TH1D *histTrackerMuons_pt;
  TH1D *histGlobalMuons_pt;

  TH1D *histAllMuons_eta;
  TH1D *histAllMuons_phi;
  TH1D *histOver45Muons_eta;
  TH1D *histDoubleMuons_pt;
  TH1D *histDoubleMuons_eta;
  TH1D *histDoubleMuons_phi;
 
  TH1D *histZprime_mass;
  TH1D *histZprime_mass_TuneP;
  TH1D *histZprime_mass_PF;
  TH1D *histZprime_mass_global;
  TH1D *histZprime_mass_inner;
  TH1D *histZprime_mass_tpfms;
  TH1D *histZprime_mass_picky;
  TH1D *histZprime_mass_dyt;

  TH1D *histZprime_res;
  TH1D *histZprime_res_TuneP;
  TH1D *histZprime_res_PF;
  TH1D *histZprime_res_global;
  TH1D *histZprime_res_inner;
  TH1D *histZprime_res_tpfms;
  TH1D *histZprime_res_picky;
  TH1D *histZprime_res_dyt;

  TH1D *histZprime_massGen;
  TH1D *histDimuon_massGen;

  TH1D *hN_events_counts;  
  TH1D *hN_events_eff;

  //variables
  double numEvents;
  double triggerEvents;
  double goodMuonsEvents;
  double triggerMatchedEvents;
  double SamePVEvents;
  
  std::vector<pat::Muon> goodMuons;
 
  TLorentzVector mZprime;
  TLorentzVector mZprimeBestMuons_TuneP;
  TLorentzVector mZprimeBestMuons_PF;
  TLorentzVector mZprimeGlobalMuons;
  TLorentzVector mZprimeInnerMuons;
  TLorentzVector mZprimeTPFMSMuons;
  TLorentzVector mZprimePickyMuons;
  TLorentzVector mZprimeDYTMuons;

  // double mZprime;
  
  std::vector<pat::PackedGenParticle> genMuons; 
  // TLorentzVector  mGenZprime;
  // TLorentzVector  mGenDimuon;
  double mGenZprime;
  double mGenDimuon;
    
};

MiniMiniAnalyzer::MiniMiniAnalyzer(const edm::ParameterSet& iConfig):
  muonToken_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))){
  
  //histos
  edm::Service<TFileService> fs;
  
  histAllVertZPos     = fs->make<TH1D>("histAllVertZPos","All Primary Vertices",500,-250.0,250.0);
  histAllMuons_pt     = fs->make<TH1D>("histAllMuons_pt","All the good quality Muons: pt", 500, 0., 5000.);
  histTunePMuons_pt   = fs->make<TH1D>("histTunePMuons_pt","TuneP Muons: pt", 500, 0., 5000.);
  histBestMuons_pt    = fs->make<TH1D>("histBestMuons_pt","Best  Muons: pt", 500, 0., 5000.);
  histTrackerMuons_pt = fs->make<TH1D>("histTrackerMuons_pt","Tracker Muons: pt", 500, 0., 5000.);
  histGlobalMuons_pt  = fs->make<TH1D>("histGlobalMuons_pt","Global Muons: pt", 500, 0., 5000.);

  histAllMuons_eta    = fs->make<TH1D>("histAllMuons_eta","All the good quality Muons: eta", 30, -5., 5.);
  histAllMuons_phi    = fs->make<TH1D>("histAllMuons_phi","All the good quality Muons: phi", 30, -20., 20.);
  histOver45Muons_eta = fs->make<TH1D>("histOver45Muons_eta","Muons after pt cut", 30, -5., 5.);

  histDoubleMuons_pt  = fs->make<TH1D>("histDoubleMuons_pt","Muons from selected pair: pt", 100, 0., 500.);
  histDoubleMuons_eta = fs->make<TH1D>("histDoubleMuons_eta","Muons from selected pair: eta", 30, -5., 5.);
  histDoubleMuons_phi = fs->make<TH1D>("histDoubleMuons_phi","Muons from selected pair: phi", 30, -20., 20.);

  histZprime_mass       = fs->make<TH1D>("histZprime_mass","Reconstructed mass of the dimuons", 350, 0., 3500.);
  histZprime_mass_TuneP = fs->make<TH1D>("histZprime_mass_TuneP","Reconstructed mass of the TuneP dimuons", 350, 0., 3500.);
  histZprime_mass_PF    = fs->make<TH1D>("histZprime_mass_PF","Reconstructed mass of the PF dimuons", 350, 0., 3500.);
  histZprime_mass_global= fs->make<TH1D>("histZprime_mass_global","Reconstructed mass of the Global dimuons", 350, 0., 3500.);
  histZprime_mass_inner = fs->make<TH1D>("histZprime_mass_inner","Reconstructed mass of the Inner dimuons", 350, 0., 3500.);
  histZprime_mass_tpfms = fs->make<TH1D>("histZprime_mass_tpfms","Reconstructed mass of the TPFMS dimuons", 350, 0., 3500.);
  histZprime_mass_picky = fs->make<TH1D>("histZprime_mass_picky","Reconstructed mass of the Picky dimuons", 350, 0., 3500.);
  histZprime_mass_dyt   = fs->make<TH1D>("histZprime_mass_dyt","Reconstructed mass of the DYT dimuons", 350, 0., 3500.);

  histZprime_massGen  = fs->make<TH1D>("histZprime_massGen","Generated mass of the Z'", 350, 0., 3500.);;
  histDimuon_massGen  = fs->make<TH1D>("histDimuon_massGen","Reconstructed mass of the Generated dimuons", 350, 0., 3500.);;

  histZprime_res       = fs->make<TH1D>("histZprime_res","Mass resolution of the dimuons", 120, -0.3, 0.3);
  histZprime_res_TuneP = fs->make<TH1D>("histZprime_res_TuneP","Mass resolution of the TuneP dimuons", 120, -0.3, 0.3);
  histZprime_res_PF    = fs->make<TH1D>("histZprime_res_PF","Mass resolution of the PF dimuons", 120, -0.3, 0.3);
  histZprime_res_global= fs->make<TH1D>("histZprime_res_global","Mass resolution of the Global dimuons", 120, -0.3, 0.3);
  histZprime_res_inner = fs->make<TH1D>("histZprime_res_inner","Mass resolution of the Inner dimuons", 120, -0.3, 0.3);
  histZprime_res_tpfms = fs->make<TH1D>("histZprime_res_tpfms","Mass resolution of the TPFMS dimuons", 120, -0.3, 0.3);
  histZprime_res_picky = fs->make<TH1D>("histZprime_res_picky","Mass resolution of the Picky dimuons", 120, -0.3, 0.3);
  histZprime_res_dyt   = fs->make<TH1D>("histZprime_res_dyt","Mass resolution of the DYT dimuons", 120, -0.3, 0.3);


  hN_events_counts    = fs->make<TH1D>("N_events_counts","Number of events",11,0.,11.);
  hN_events_eff       = fs->make<TH1D>("N_events_eff","Efficiency",11,0.,11.);

  //variables
  numEvents            = 0;
  // triggerEvents        = 0;
  // goodMuonsEvents      = 0;
  // triggerMatchedEvents = 0;
  // SamePVEvents         = 0;
   
  // mGenZprime           = 0;
  // mGenDimuon           = 0;
  
}



MiniMiniAnalyzer::~MiniMiniAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MiniMiniAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  using namespace edm;
  using namespace reco;
  using namespace pat;
  using namespace std;
  
  //float muPDG = 0.105658;
  
  numEvents++;  
  //std::cout << " Event number "<< numEvents << std::endl; 
  
  
  Handle<pat::MuonCollection> muons;                             
  iEvent.getByToken(muonToken_, muons);
  
  for (pat::Muon mu : *muons) {
    if (mu.isGlobalMuon() && mu.pt()>20){
      histAllMuons_pt    ->Fill(mu.pt());
      if((mu.tunePMuonBestTrack().refCore()).isAvailable()){
	histTunePMuons_pt  ->Fill(mu.tunePMuonBestTrack()->pt());
	
	// if(iEvent.id().event()==5083 || iEvent.id().event()==5136 || iEvent.id().event()==5167 || iEvent.id().event()==5181 || iEvent.id().event()==5203 || iEvent.id().event()==5207 || iEvent.id().event()==5214 || iEvent.id().event()==5230 || iEvent.id().event()==5268 || iEvent.id().event()==5286 || iEvent.id().event()==5372)
	//	std::cout << " Event Number " << iEvent.id().event() << " TuneP pt = " << mu.tunePMuonBestTrack()->pt() << " Type: " << mu.tunePMuonBestTrackType() << std::endl;
      }
      else{
	
	// if(iEvent.id().event()==5083 || iEvent.id().event()==5136 || iEvent.id().event()==5167 || iEvent.id().event()==5181 || iEvent.id().event()==5203 || iEvent.id().event()==5207 || iEvent.id().event()==5214 || iEvent.id().event()==5230 || iEvent.id().event()==5268 || iEvent.id().event()==5286 || iEvent.id().event()==5372)
	std::cout << " Event: id " << iEvent.id()<< "  /run " << iEvent.id().run() << " /number " << iEvent.id().event() << " Muon pt = " << mu.pt() << " Inner pt = " << mu.innerTrack()->pt() << " Global pt = " << mu.globalTrack()->pt() << " Best Track pt = " << mu.muonBestTrack()->pt() <<" Type: " << mu.muonBestTrackType() << std::endl;
	
	histTunePMuons_pt  ->Fill(mu.globalTrack()->pt());
      }
      

      histBestMuons_pt   ->Fill(mu.muonBestTrack()->pt());
      histTrackerMuons_pt->Fill(mu.innerTrack()->pt());
      histGlobalMuons_pt ->Fill(mu.globalTrack()->pt());
    }
    
  }

}//end of analyze

// ------------ method called once each job just before starting event loop  ------------
void 
MiniMiniAnalyzer::beginJob()
{

  //inizializza la roba
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MiniMiniAnalyzer::endJob() {
  hN_events_counts->Fill(1,numEvents);
  // hN_events_eff->Fill(1,numEvents/numEvents);
  // hN_events_counts->Fill(2,triggerEvents);
  // hN_events_eff->Fill(2,triggerEvents/numEvents); 
  // hN_events_counts->Fill(3,goodMuonsEvents);
  // hN_events_eff->Fill(3,goodMuonsEvents/numEvents); 
  // hN_events_counts->Fill(4,triggerMatchedEvents);
  // hN_events_eff->Fill(4,triggerMatchedEvents/numEvents); 
  // hN_events_counts->Fill(5,SamePVEvents);
  // hN_events_eff->Fill(5,SamePVEvents/numEvents); 
}

// ------------ method called when starting to processes a run  ------------
/*
void 
MiniMiniAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
MiniMiniAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
MiniMiniAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
MiniMiniAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MiniMiniAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

bool MiniMiniAnalyzer::isAncestor(const reco::Candidate* ancestor, const reco::Candidate * particle){
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
DEFINE_FWK_MODULE(MiniMiniAnalyzer);

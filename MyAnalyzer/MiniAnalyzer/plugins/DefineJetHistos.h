// ROOT
#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TCanvas.h"
#include "TFile.h"


TH2F *histPFJets_bDisc_pt;
  //TH2F *histPFJets_bDisc_eta;
TH1F *histPFJets_bDisc;
TH1F *histPFJets_pt;       
TH1F *histPFJets_eta;
TH1F *histPFJets_phi;
TH1F *histPFJets_deltaPhi;

TH1F *histbtagPFJets_pt;

TH1F *histCuttedJets_bDisc;
TH1F *histCuttedJets_pt;       
TH1F *histCuttedJets_eta;
TH1F *histCuttedJets_phi;


TH1F *histBJets_pt;
TH1F *histBJets_eta;
TH1F *histBJets_phi;
TH1F *histBJets_deltaPhi;
TH1F *histPFJets_size;
TH1F *histBJets_size;
TH1F *histLightJets_size;
TH1F *histLightJets_pt;
TH1F *histLightJets_eta;
TH1F *histLightJets_phi;
TH1F *histLightJets_deltaPhi;

TH1F *rwhistPFJets_bDisc;
TH1F *rwhistPFJets_pt;       
TH1F *rwhistPFJets_eta;
TH1F *rwhistPFJets_phi;
TH1F *rwhistPFJets_deltaPhi;
TH1F *rwhistbtagPFJets_pt;
TH1F *rwhistCuttedJets_bDisc;
TH1F *rwhistCuttedJets_pt;       
TH1F *rwhistCuttedJets_eta;
TH1F *rwhistCuttedJets_phi;
TH1F *rwhistBJets_pt;
TH1F *rwhistBJets_eta;
TH1F *rwhistBJets_phi;
TH1F *rwhistBJets_deltaPhi;
TH1F *rwhistPFJets_size;
TH1F *rwhistBJets_size;
TH1F *rwhistLightJets_size;
TH1F *rwhistLightJets_pt;
TH1F *rwhistLightJets_eta;
TH1F *rwhistLightJets_phi;
TH1F *rwhistLightJets_deltaPhi;

TH1F *rwuphistPFJets_bDisc;
TH1F *rwuphistPFJets_pt;       
TH1F *rwuphistPFJets_eta;
TH1F *rwuphistPFJets_phi;
TH1F *rwuphistPFJets_deltaPhi;
TH1F *rwuphistbtagPFJets_pt;
TH1F *rwuphistCuttedJets_bDisc;
TH1F *rwuphistCuttedJets_pt;       
TH1F *rwuphistCuttedJets_eta;
TH1F *rwuphistCuttedJets_phi;
TH1F *rwuphistBJets_pt;
TH1F *rwuphistBJets_eta;
TH1F *rwuphistBJets_phi;
TH1F *rwuphistBJets_deltaPhi;
TH1F *rwuphistPFJets_size;
TH1F *rwuphistBJets_size;

TH1F *rwdownhistPFJets_bDisc;
TH1F *rwdownhistPFJets_pt;       
TH1F *rwdownhistPFJets_eta;
TH1F *rwdownhistPFJets_phi;
TH1F *rwdownhistPFJets_deltaPhi;
TH1F *rwdownhistbtagPFJets_pt;
TH1F *rwdownhistCuttedJets_bDisc;
TH1F *rwdownhistCuttedJets_pt;       
TH1F *rwdownhistCuttedJets_eta;
TH1F *rwdownhistCuttedJets_phi;
TH1F *rwdownhistBJets_pt;
TH1F *rwdownhistBJets_eta;
TH1F *rwdownhistBJets_phi;
TH1F *rwdownhistBJets_deltaPhi;
TH1F *rwdownhistPFJets_size;
TH1F *rwdownhistBJets_size;


  

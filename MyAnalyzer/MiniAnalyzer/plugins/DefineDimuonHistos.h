// ROOT
#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TCanvas.h"
#include "TFile.h"


TH1F *Minv_Cat1, *Minv_Cat2;
TH1F *Minv_Cat11, *Minv_Cat22;
TH1F *Minv_Cat111, *Minv_Cat222;

TH1F *histMediumDimuon_mass;
TH1F *histMediumDimuon_pt;
TH1F *histMediumDimuon_eta;
TH1F *histMediumDimuon_phi;

TH1F *rwhistMediumDimuon_mass;
TH1F *rwhistMediumDimuon_pt;
TH1F *rwhistMediumDimuon_eta;
TH1F *rwhistMediumDimuon_phi;

TH1F *rwuphistMediumMuons_mass;
TH1F *rwdownhistMediumMuons_mass;


TH1F *histMetDimuonM_mass;
TH1F *rwhistMetDimuonM_mass;
/* TH1F *rwuphistMetMuons_mass; */
/* TH1F *rwdownhistMetMuons_mass; */
  
TH1F *histTightDimuon_mass;
TH1F *histTightDimuon_pt;
TH1F *histTightDimuon_eta;
TH1F *histTightDimuon_phi;

TH1F *histTightDimuonRoc_mass;
TH1F *histTightDimuonRoc_pt;
TH1F *histTightDimuonRoc_eta;
TH1F *histTightDimuonRoc_phi;

TH1F *histMetDimuon_mass;
TH1F *histMetDimuon_pt;
TH1F *histMetDimuon_eta;
TH1F *histMetDimuon_phi;
 
TH1F *histCat1_mass;
TH1F *histCat2_mass;
TH1F *histCat1_pt;
TH1F *histCat2_pt;
TH1F *histCat1_phi;
TH1F *histCat2_phi;
TH1F *histCat1_eta;
TH1F *histCat2_eta;

TH1F *histCat11_mass;
TH1F *histCat11_pt;
TH1F *histCat11_eta;
TH1F *histCat11_phi;
TH1F *histCat22_mass;
TH1F *histCat22_pt;
TH1F *histCat22_eta;
TH1F *histCat22_phi;

TH1F *histCat111_mass;
TH1F *histCat111_pt;
TH1F *histCat111_eta;
TH1F *histCat111_phi;
TH1F *histCat222_mass;
TH1F *histCat222_pt;
TH1F *histCat222_eta;
TH1F *histCat222_phi;

TH1F *histCat1M_mass;
TH1F *histCat2M_mass;
TH1F *histCat1M_pt;
TH1F *histCat2M_pt;
TH1F *histCat1M_phi;
TH1F *histCat2M_phi;
TH1F *histCat1M_eta;
TH1F *histCat2M_eta;

TH1F *histCat11M_mass;
TH1F *histCat11M_pt;
TH1F *histCat11M_eta;
TH1F *histCat11M_phi;
TH1F *histCat22M_mass;
TH1F *histCat22M_pt;
TH1F *histCat22M_eta;
TH1F *histCat22M_phi;

TH1F *histCat111M_mass;
TH1F *histCat111M_pt;
TH1F *histCat111M_eta;
TH1F *histCat111M_phi;
TH1F *histCat222M_mass;
TH1F *histCat222M_pt;
TH1F *histCat222M_eta;
TH1F *histCat222M_phi;

TH1F *rwhistTightDimuon_mass;
TH1F *rwhistTightDimuon_pt;
TH1F *rwhistTightDimuon_eta;
TH1F *rwhistTightDimuon_phi;

TH1F *rwhistTightDimuonRoc_mass;
TH1F *rwhistTightDimuonRoc_pt;
TH1F *rwhistTightDimuonRoc_eta;
TH1F *rwhistTightDimuonRoc_phi;

TH1F *rwuphistTightDimuon_mass;
TH1F *rwuphistTightDimuon_pt;
TH1F *rwuphistTightDimuon_eta;
TH1F *rwuphistTightDimuon_phi;

TH1F *rwdownhistTightDimuon_mass;
TH1F *rwdownhistTightDimuon_pt;
TH1F *rwdownhistTightDimuon_eta;
TH1F *rwdownhistTightDimuon_phi;

TH1F *rwhistMetDimuon_mass;
TH1F *rwhistMetDimuon_pt;
TH1F *rwhistMetDimuon_eta;
TH1F *rwhistMetDimuon_phi;
TH1F *rwuphistMetDimuon_mass;
TH1F *rwuphistMetDimuon_pt;
TH1F *rwuphistMetDimuon_eta;
TH1F *rwuphistMetDimuon_phi;
TH1F *rwdownhistMetDimuon_mass;
TH1F *rwdownhistMetDimuon_pt;
TH1F *rwdownhistMetDimuon_eta;
TH1F *rwdownhistMetDimuon_phi;

TH1F *rwhistCat1_phi;
TH1F *rwhistCat2_phi;
TH1F *rwhistCat1_eta;
TH1F *rwhistCat2_eta;
TH1F *rwhistCat1_pt;
TH1F *rwhistCat2_pt;
TH1F *rwhistCat1_mass;
TH1F *rwhistCat2_mass;

TH1F *rwhistCat11_mass;
TH1F *rwhistCat22_mass;
TH1F *rwhistCat11_pt;
TH1F *rwhistCat22_pt;
TH1F *rwhistCat11_eta;
TH1F *rwhistCat22_eta;
TH1F *rwhistCat11_phi;
TH1F *rwhistCat22_phi;

TH1F *rwhistCat111_mass;
TH1F *rwhistCat222_mass;
TH1F *rwhistCat111_pt;
TH1F *rwhistCat222_pt;
TH1F *rwhistCat111_eta;
TH1F *rwhistCat222_eta;
TH1F *rwhistCat111_phi;
TH1F *rwhistCat222_phi;

TH1F *rwhistCat1M_mass;
TH1F *rwhistCat2M_mass;
TH1F *rwhistCat1M_phi;
TH1F *rwhistCat2M_phi;
TH1F *rwhistCat1M_eta;
TH1F *rwhistCat2M_eta;
TH1F *rwhistCat1M_pt;
TH1F *rwhistCat2M_pt;

TH1F *rwhistCat11M_mass;
TH1F *rwhistCat22M_mass;
TH1F *rwhistCat11M_pt;
TH1F *rwhistCat22M_pt;
TH1F *rwhistCat11M_eta;
TH1F *rwhistCat22M_eta;
TH1F *rwhistCat11M_phi;
TH1F *rwhistCat22M_phi;

TH1F *rwhistCat111M_mass;
TH1F *rwhistCat222M_mass;
TH1F *rwhistCat111M_pt;
TH1F *rwhistCat222M_pt;
TH1F *rwhistCat111M_eta;
TH1F *rwhistCat222M_eta;
TH1F *rwhistCat111M_phi;
TH1F *rwhistCat222M_phi;


TH1F *rwuphistCat1_phi;
TH1F *rwuphistCat2_phi;
TH1F *rwuphistCat1_eta;
TH1F *rwuphistCat2_eta;
TH1F *rwuphistCat1_pt;
TH1F *rwuphistCat2_pt;

TH1F *rwuphistCat1_mass;
TH1F *rwuphistCat2_mass;
TH1F *rwuphistCat11_mass;
TH1F *rwuphistCat22_mass;
 
TH1F *rwdownhistCat1_phi;
TH1F *rwdownhistCat2_phi;
TH1F *rwdownhistCat1_eta;
TH1F *rwdownhistCat2_eta;
TH1F *rwdownhistCat1_pt;
TH1F *rwdownhistCat2_pt;

TH1F *rwdownhistCat1_mass;
TH1F *rwdownhistCat2_mass;
TH1F *rwdownhistCat11_mass;
TH1F *rwdownhistCat22_mass;


TH1F *histZ_massGen;

  

  

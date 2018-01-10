// ROOT
#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TCanvas.h"
#include "TFile.h"

    
//MET
TH1F *histPFMet_ptRaw;
TH1F *histPFMet_pt;
TH1F *histPFMet_phi;
TH1F *histPFMet_sumet;
TH1F *histPFMet_type01smear_pt; 
TH1F *histPFMet_totUp_pt;
TH1F *histPFMet_totDown_pt; // mi sa che si sono sbagliati proprio a chiamarla centralmente
/* TH1F *histPFMet_cHADEtFraction; */
/* TH1F *histPFMet_nHADEtFraction; */
  
/* TH1F *histCaloMet_pt; */
/* TH1F *histCaloMet_phi; */
  
TH1F *histRawMet_phi;
TH1F *histRawMet_pt;
TH1F *histRawMet_sumet;
  
TH1F *histPFMet_70toInf_pt;
TH1F *histPFMet_70toInf_phi;
TH1F *histPFMet_70toInf_sumet;
TH1F *histPFMet_70toInf_type01smear_pt;
TH1F *histPFMet_70toInf_totUp_pt;
TH1F *histPFMet_70toInf_totDown_pt; // mi sa che si sono sbagliati proprio a chiamarla centralmente
/* TH1F *histPFMet_70toInf_cHADEtFraction; */
/* TH1F *histPFMet_70toInf_nHADEtFraction; */

TH1F *histPFMet_70to110_pt;
TH1F *histPFMet_70to110_phi;
TH1F *histPFMet_70to110_sumet;
TH1F *histPFMet_70to110_type01smear_pt; 
TH1F *histPFMet_70to110_totUp_pt;
TH1F *histPFMet_70to110_totDown_pt; // mi sa che si sono sbagliati proprio a chiamarla centralmente
/* TH1F *histPFMet_70to110_cHADEtFraction; */
/* TH1F *histPFMet_70to110_nHADEtFraction; */

TH1F *histPFMet_110to150_pt;
TH1F *histPFMet_110to150_phi;
TH1F *histPFMet_110to150_sumet;
TH1F *histPFMet_110to150_type01smear_pt;
TH1F *histPFMet_110to150_totUp_pt;
TH1F *histPFMet_110to150_totDown_pt; // mi sa che si sono sbagliati proprio a chiamarla centralmente
/* TH1F *histPFMet_110to150_cHADEtFraction; */
/* TH1F *histPFMet_110to150_nHADEtFraction; */

TH1F *histPFMet_150toInf_pt;
TH1F *histPFMet_150toInf_phi;
TH1F *histPFMet_150toInf_sumet;
TH1F *histPFMet_150toInf_type01smear_pt; 
TH1F *histPFMet_150toInf_totUp_pt;
TH1F *histPFMet_150toInf_totDown_pt; // mi sa che si sono sbagliati proprio a chiamarla centralmente
/* TH1F *histPFMet_150toInf_cHADEtFraction; */
/* TH1F *histPFMet_150toInf_nHADEtFraction; */


  
//MET
TH1F *rwhistPFMet_pt;
TH1F *rwhistPFMet_phi;
TH1F *rwhistPFMet_sumet;
TH1F *rwhistPFMet_type01smear_pt; 
/* TH1F *rwhistPFMet_totUp_pt;       */
/* TH1F *rwhistPFMet_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente */
/* TH1F *rwhistPFMet_cHADEtFraction; */
/* TH1F *rwhistPFMet_nHADEtFraction; */
TH1F *rwhistPFMet_70toInf_pt;
TH1F *rwhistPFMet_70toInf_phi;
TH1F *rwhistPFMet_70toInf_sumet;
TH1F *rwhistPFMet_70toInf_type01smear_pt; 
/* TH1F *rwhistPFMet_70toInf_totUp_pt;       */
/* TH1F *rwhistPFMet_70toInf_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente */
/* TH1F *rwhistPFMet_70toInf_cHADEtFraction; */
/* TH1F *rwhistPFMet_70toInf_nHADEtFraction; */
TH1F *rwhistPFMet_70to110_pt;
TH1F *rwhistPFMet_70to110_phi;
TH1F *rwhistPFMet_70to110_sumet;
TH1F *rwhistPFMet_70to110_type01smear_pt;
/* TH1F *rwhistPFMet_70to110_cEMEtFraction;       */
/* TH1F *rwhistPFMet_70to110_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente */
/* TH1F *rwhistPFMet_70to110_cHADEtFraction; */
/* TH1F *rwhistPFMet_70to110_nHADEtFraction; */
TH1F *rwhistPFMet_110to150_pt;
TH1F *rwhistPFMet_110to150_phi;
TH1F *rwhistPFMet_110to150_sumet;
TH1F *rwhistPFMet_110to150_type01smear_pt; 
/* TH1F *rwhistPFMet_110to150_cEMEtFraction;       */
/* TH1F *rwhistPFMet_110to150_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente */
/* TH1F *rwhistPFMet_110to150_cHADEtFraction; */
/* TH1F *rwhistPFMet_110to150_nHADEtFraction; */

TH1F *rwuphistPFMet_pt;
TH1F *rwuphistPFMet_phi;
TH1F *rwuphistPFMet_sumet;
TH1F *rwuphistPFMet_type01smear_pt; 
/* TH1F *rwuphistPFMet_cEMEtFraction;       */
/* TH1F *rwuphistPFMet_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente */
/* TH1F *rwuphistPFMet_cHADEtFraction; */
/* TH1F *rwuphistPFMet_nHADEtFraction; */
TH1F *rwuphistPFMet_70toInf_pt;
TH1F *rwuphistPFMet_70toInf_phi;
TH1F *rwuphistPFMet_70toInf_sumet;
TH1F *rwuphistPFMet_70toInf_type01smear_pt; 
/* TH1F *rwuphistPFMet_70toInf_cEMEtFraction;       */
/* TH1F *rwuphistPFMet_70toInf_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente */
/* TH1F *rwuphistPFMet_70toInf_cHADEtFraction; */
/* TH1F *rwuphistPFMet_70toInf_nHADEtFraction; */
TH1F *rwuphistPFMet_70to110_pt;
TH1F *rwuphistPFMet_70to110_phi;
TH1F *rwuphistPFMet_70to110_sumet;
TH1F *rwuphistPFMet_70to110_type01smear_pt; 
/* TH1F *rwuphistPFMet_70to110_cEMEtFraction;       */
/* TH1F *rwuphistPFMet_70to110_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente */
/* TH1F *rwuphistPFMet_70to110_cHADEtFraction; */
/* TH1F *rwuphistPFMet_70to110_nHADEtFraction; */
TH1F *rwuphistPFMet_110to150_pt;
TH1F *rwuphistPFMet_110to150_phi;
TH1F *rwuphistPFMet_110to150_sumet;
TH1F *rwuphistPFMet_110to150_type01smear_pt; 
/* TH1F *rwuphistPFMet_110to150_cEMEtFraction;       */
/* TH1F *rwuphistPFMet_110to150_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente */
/* TH1F *rwuphistPFMet_110to150_cHADEtFraction; */
/* TH1F *rwuphistPFMet_110to150_nHADEtFraction; */

TH1F *rwdownhistPFMet_pt;
TH1F *rwdownhistPFMet_phi;
TH1F *rwdownhistPFMet_sumet;
TH1F *rwdownhistPFMet_type01smear_pt; 
/* TH1F *rwdownhistPFMet_cEMEtFraction;       */
/* TH1F *rwdownhistPFMet_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente */
/* TH1F *rwdownhistPFMet_cHADEtFraction; */
/* TH1F *rwdownhistPFMet_nHADEtFraction;  */ 
TH1F *rwdownhistPFMet_70toInf_pt;
TH1F *rwdownhistPFMet_70toInf_phi;
TH1F *rwdownhistPFMet_70toInf_sumet;
TH1F *rwdownhistPFMet_70toInf_type01smear_pt; 
/* TH1F *rwdownhistPFMet_70toInf_cEMEtFraction;       */
/* TH1F *rwdownhistPFMet_70toInf_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente */
/* TH1F *rwdownhistPFMet_70toInf_cHADEtFraction; */
/* TH1F *rwdownhistPFMet_70toInf_nHADEtFraction; */
TH1F *rwdownhistPFMet_70to110_pt;
TH1F *rwdownhistPFMet_70to110_phi;
TH1F *rwdownhistPFMet_70to110_sumet;
TH1F *rwdownhistPFMet_70to110_type01smear_pt;
/* TH1F *rwdownhistPFMet_70to110_cEMEtFraction;       */
/* TH1F *rwdownhistPFMet_70to110_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente */
/* TH1F *rwdownhistPFMet_70to110_cHADEtFraction; */
/* TH1F *rwdownhistPFMet_70to110_nHADEtFraction; */
TH1F *rwdownhistPFMet_110to150_pt;
TH1F *rwdownhistPFMet_110to150_phi;
TH1F *rwdownhistPFMet_110to150_sumet;
TH1F *rwdownhistPFMet_110to150_type01smear_pt; 
/* TH1F *rwdownhistPFMet_110to150_cEMEtFraction;       */
/* TH1F *rwdownhistPFMet_110to150_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente */
/* TH1F *rwdownhistPFMet_110to150_cHADEtFraction; */
/* TH1F *rwdownhistPFMet_110to150_nHADEtFraction; */


/* TH1F *jetuphistPFMet_pt; */
/* TH1F *jetuphistPFMet_phi; */
/* TH1F *jetuphistPFMet_sumet; */
/* /\* TH1F *jetuphistPFMet_type01smear_pt; *\/ */
/* /\* TH1F *jetuphistPFMet_cEMEtFraction;       *\/ */
/* /\* TH1F *jetuphistPFMet_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente *\/ */
/* /\* TH1F *jetuphistPFMet_cHADEtFraction; *\/ */
/* /\* TH1F *jetuphistPFMet_nHADEtFraction; *\/ */
/* TH1F *jetuphistPFMet_70toInf_pt; */
/* TH1F *jetuphistPFMet_70toInf_phi; */
/* TH1F *jetuphistPFMet_70toInf_sumet; */
/* /\* TH1F *jetuphistPFMet_70toInf_type01smear_pt; *\/ */
/* /\* TH1F *jetuphistPFMet_70toInf_cEMEtFraction;       *\/ */
/* /\* TH1F *jetuphistPFMet_70toInf_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente *\/ */
/* /\* TH1F *jetuphistPFMet_70toInf_cHADEtFraction; *\/ */
/* /\* TH1F *jetuphistPFMet_70toInf_nHADEtFraction; *\/ */
/* TH1F *jetuphistPFMet_70to110_pt; */
/* TH1F *jetuphistPFMet_70to110_phi; */
/* TH1F *jetuphistPFMet_70to110_sumet; */
/* /\* TH1F *jetuphistPFMet_70to110_type01smear_pt; *\/ */
/* /\* TH1F *jetuphistPFMet_70to110_cEMEtFraction;       *\/ */
/* /\* TH1F *jetuphistPFMet_70to110_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente *\/ */
/* /\* TH1F *jetuphistPFMet_70to110_cHADEtFraction; *\/ */
/* /\* TH1F *jetuphistPFMet_70to110_nHADEtFraction; *\/ */
/* TH1F *jetuphistPFMet_110to150_pt; */
/* TH1F *jetuphistPFMet_110to150_phi; */
/* TH1F *jetuphistPFMet_110to150_sumet; */
/* /\* TH1F *jetuphistPFMet_110to150_type01smear_pt; *\/ */
/* /\* TH1F *jetuphistPFMet_110to150_cEMEtFraction;       *\/ */
/* /\* TH1F *jetuphistPFMet_110to150_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente *\/ */
/* /\* TH1F *jetuphistPFMet_110to150_cHADEtFraction; *\/ */
/* /\* TH1F *jetuphistPFMet_110to150_nHADEtFraction; *\/ */

/* TH1F *jetdownhistPFMet_pt; */
/* TH1F *jetdownhistPFMet_phi; */
/* TH1F *jetdownhistPFMet_sumet; */
/* /\* TH1F *jetdownhistPFMet_type01smear_pt; *\/ */
/* /\* TH1F *jetdownhistPFMet_cEMEtFraction;       *\/ */
/* /\* TH1F *jetdownhistPFMet_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente *\/ */
/* /\* TH1F *jetdownhistPFMet_cHADEtFraction; *\/ */
/* /\* TH1F *jetdownhistPFMet_nHADEtFraction;  *\/  */
/* TH1F *jetdownhistPFMet_70toInf_pt; */
/* TH1F *jetdownhistPFMet_70toInf_phi; */
/* TH1F *jetdownhistPFMet_70toInf_sumet; */
/* /\* TH1F *jetdownhistPFMet_70toInf_type01smear_pt; *\/ */
/* /\* TH1F *jetdownhistPFMet_70toInf_cEMEtFraction;       *\/ */
/* /\* TH1F *jetdownhistPFMet_70toInf_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente *\/ */
/* /\* TH1F *jetdownhistPFMet_70toInf_cHADEtFraction; *\/ */
/* /\* TH1F *jetdownhistPFMet_70toInf_nHADEtFraction; *\/ */
/* TH1F *jetdownhistPFMet_70to110_pt; */
/* TH1F *jetdownhistPFMet_70to110_phi; */
/* TH1F *jetdownhistPFMet_70to110_sumet; */
/* /\* TH1F *jetdownhistPFMet_70to110_type01smear_pt; *\/ */
/* /\* TH1F *jetdownhistPFMet_70to110_cEMEtFraction;       *\/ */
/* /\* TH1F *jetdownhistPFMet_70to110_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente *\/ */
/* /\* TH1F *jetdownhistPFMet_70to110_cHADEtFraction; *\/ */
/* /\* TH1F *jetdownhistPFMet_70to110_nHADEtFraction; *\/ */
/* TH1F *jetdownhistPFMet_110to150_pt; */
/* TH1F *jetdownhistPFMet_110to150_phi; */
/* TH1F *jetdownhistPFMet_110to150_sumet; */
/* /\* TH1F *jetdownhistPFMet_110to150_type01smear_pt; *\/ */
/* /\* TH1F *jetdownhistPFMet_110to150_cEMEtFraction;       *\/ */
/* /\* TH1F *jetdownhistPFMet_110to150_nEMEtFraction; // mi sa che si sono sbagliati proprio a chiamarla centralmente *\/ */
/* /\* TH1F *jetdownhistPFMet_110to150_cHADEtFraction; *\/ */
/* /\* TH1Fyyyyyyyyyyyy *jetdownhistPFMet_110to150_nHADEtFraction; *\/ */



  

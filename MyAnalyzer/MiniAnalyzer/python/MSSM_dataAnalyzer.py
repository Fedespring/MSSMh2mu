

import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

mc = cms.bool(False)
signal = cms.bool(False)
useCrab = cms.bool(False)


globaltag_data  = cms.string('80X_dataRun2_2016SeptRepro_v7') ##8_0_20
globaltag_mc = cms.string('80X_mcRun2_asymptotic_2016_TrancheIV_v8') #moriond17


process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load('Configuration.StandardSequences.Services_cff')

if mc:
    process.GlobalTag.globaltag = globaltag_mc    
else:
    process.GlobalTag.globaltag = globaltag_data    


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                            #fileNames = cms.untracked.vstring('file:./../test/crab_pickevents_20170515_102914/crab_pickEvents/results/pickevents_merged.root')
       



        #'/store/mc/RunIISummer16MiniAODv2/GluGlu_HToMuMu_M125_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/36967CD0-3CC1-E611-A615-D8D385FF1996.root',
        #'/store/mc/RunIISummer16MiniAODv2/GluGlu_HToMuMu_M125_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/42101C49-85C1-E611-AEE6-D8D385AF8B02.root',
        #'/store/mc/RunIISummer16MiniAODv2/GluGlu_HToMuMu_M125_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/56023300-57C1-E611-9D9A-0025905C6448.root',
        #'/store/mc/RunIISummer16MiniAODv2/GluGlu_HToMuMu_M125_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/6C16044D-4BC1-E611-8477-00266CFCC490.root',
        #'/store/mc/RunIISummer16MiniAODv2/GluGlu_HToMuMu_M125_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/78D8A043-85C1-E611-AB19-00266CFCCD94.root',
        #'/store/mc/RunIISummer16MiniAODv2/GluGlu_HToMuMu_M125_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/9ACE9491-85C1-E611-8173-001E67E6F8E6.root',
        #'/store/mc/RunIISummer16MiniAODv2/GluGlu_HToMuMu_M125_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/C0801715-85C0-E611-97A8-001E67396A18.root',
        #'/store/mc/RunIISummer16MiniAODv2/GluGlu_HToMuMu_M125_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/D2BDCC9A-2CC1-E611-8B9B-44A842CF05B2.root',
        #'/store/mc/RunIISummer16MiniAODv2/GluGlu_HToMuMu_M125_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/E017474B-85C1-E611-AF99-B499BAAC03BA.root',
        #'/store/mc/RunIISummer16MiniAODv2/GluGlu_HToMuMu_M125_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/E027D81F-70C0-E611-B47C-001E67E71408.root')
                            

                            #'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/120000/02A210D6-F5C3-E611-B570-008CFA197BD4.root')
                            #fileNames = cms.untracked.vstring('/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_HCALDebug_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/207B2E7F-01BE-E611-A7D7-002590AB3A70.root')
                            
                            fileNames = cms.untracked.vstring('/store/data/Run2016H/SingleMuon/MINIAOD/03Feb2017_ver2-v1/110000/00633FF0-85EA-E611-811C-001E674FB25C.root')

                          
                            
                            )

## ==== Trigger Matching ====
filter_name = cms.string("hltL3crIsoL1sMu16L1f0L2f10QL3f20QL3trkIsoFiltered0p09") # not used
path_name = cms.string("HLT_IsoMu24_v*")
path_nameER = cms.string("HLT_IsoMu22_eta2p1_v*")

bits = cms.InputTag("TriggerResults","","HLT")
triggerResults = cms.InputTag( 'TriggerResults::HLT')
if mc:
    bits = cms.InputTag("TriggerResults","","HLT")
    triggerResults = cms.InputTag( 'TriggerResults::HLT')


process.unpackedPatTrigger = cms.EDProducer("PATTriggerObjectStandAloneUnpacker", 
                                            patTriggerObjectsStandAlone = cms.InputTag( 'selectedPatTrigger' ), 
                                            triggerResults = triggerResults
                                            )

process.muonMatchHLTL3 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                        src = cms.InputTag( 'slimmedMuons' ),
                                        matched = cms.InputTag( 'unpackedPatTrigger' ),
                                        matchedCuts = cms.string( 'path("HLT_IsoMu24_v*")  || path("HLT_IsoMu22_eta2p1_v*") || path("HLT_IsoTkMu22_eta2p1_v*") || path("HLT_IsoTkMu24_v*")' ),
                                        maxDPtRel   = cms.double( 0.5 ), # no effect here
                                        maxDeltaR   = cms.double( 0.5 ),
                                        maxDeltaEta = cms.double( 0.2 ), # no effect here
                                        resolveAmbiguities    = cms.bool( True ),
                                        resolveByMatchQuality = cms.bool( True )
                                        )

## ==== Embed ====
from MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff import patMuonsWithTrigger
process.MuonsWithTrigger = patMuonsWithTrigger.clone(src = cms.InputTag(  "slimmedMuons" ),
                                                     matches = cms.VInputTag('muonMatchHLTL3')
                                                     )
## ==== Trigger Sequence ====
process.patTriggerMatching = cms.Sequence(process.unpackedPatTrigger * process.muonMatchHLTL3 *  process.MuonsWithTrigger)


#####################################################################
############# E L E C T R O N  -  S E L E C T O R ###################
# defined a proces to call VID Ele Selector using miniAOD 
# the proces is called in test/DataMCSpectraComparison/histos.py
# and in test/NMinus1Effs/nminus1effs.py

# to use it you have to do from CMSSW_X_X_X/src/:
# git cms-merge-topic Sam-Harper:HEEPV70VID
# compile it, and pass to "my_id_modules" the version that you merged 

#from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
#switchOnVIDElectronIdProducer(process, DataFormat.MiniAOD)
#my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring16_GeneralPurpose_V1_cff']
#for idmod in my_id_modules:
 #   setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)


#####################################################################
############# J E T    E N E R G Y   C O R R E C T I O N S ##########
#from CondCore.DBCommon.CondDBSetup_cfi import CondDBSetup
from CondCore.CondDB.CondDB_cfi import CondDB
if mc:
    if useCrab:
        connectString = cms.string('sqlite:src/MyAnalyzer/MiniAnalyzer/data/Summer16_23Sep2016V4_MC.db')
    else:
        connectString = cms.string('sqlite:./../data/Summer16_23Sep2016V4_MC.db')
    tagName = 'Summer16_23Sep2016V4_MC_AK4PFchs'
    tagNamePuppi = 'Summer16_23Sep2016V4_MC_AK4PFPuppi'
else:
    if useCrab:
        connectString = cms.string('sqlite:src/MyAnalyzer/MiniAnalyzer/data/Summer16_23Sep2016AllV4_DATA.db')
    else:
        connectString = cms.string('sqlite:./../data/Summer16_23Sep2016AllV4_DATA.db')
    tagName = 'Summer16_23Sep2016AllV4_DATA_AK4PFchs'
    tagNamePuppi = 'Summer16_23Sep2016AllV4_DATA_AK4PFPuppi'
    

process.jec = cms.ESSource("PoolDBESSource",
                           DBParameters = cms.PSet(authenticationPath = cms.untracked.string(''),
                                                   authenticationSystem = cms.untracked.int32(0),
                                                   messageLevel = cms.untracked.int32(0),
                                                   security = cms.untracked.string('')
                                                   ),
                           connect = connectString,
                           timetype = cms.string('runnumber'),
                           toGet = cms.VPSet(cms.PSet(record = cms.string('JetCorrectionsRecord'),
                                                      tag    = cms.string('JetCorrectorParametersCollection_%s'%tagName),
                                                      label  = cms.untracked.string('AK4PFchs'))
                                             ## here you add as many jet types as you need
                                             ## note that the tag name is specific for the particular sqlite file 
                                             )
                           )
## add an es_prefer statement to resolve a possible conflict from simultaneous connection to a global tag
process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')

#### RECOMPUTE JEC From GT ###
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
 
if mc:
    jecLevels= ['L1FastJet',  'L2Relative', 'L3Absolute']
else:
    jecLevels =['L1FastJet',  'L2Relative', 'L3Absolute', 'L2L3Residual']
 
updateJetCollection(process,
                    jetSource = cms.InputTag("slimmedJets"),
                    labelName = 'UpdatedJEC',
                    jetCorrections = ('AK4PFchs', cms.vstring(jecLevels), 'None')  # Do not forget 'L2L3Residual' on data!
                    )


print "-> Updating the jets collection to run on to 'updatedPatJetsUpdatedJEC' with the new jec in the GT"

process.jecSequence = cms.Sequence( process.patJetCorrFactorsUpdatedJEC* process.updatedPatJetsUpdatedJEC)

#####################################################################
#################### M E T   F i l t e r ############################
process.load('RecoMET.METFilters.BadPFMuonFilter_cfi')
process.BadPFMuonFilter.muons = cms.InputTag("slimmedMuons")
process.BadPFMuonFilter.PFCandidates = cms.InputTag("packedPFCandidates")
process.BadPFMuonFilter.taggingMode = cms.bool(True)

process.load('RecoMET.METFilters.BadChargedCandidateFilter_cfi')
process.BadChargedCandidateFilter.muons = cms.InputTag("slimmedMuons")
process.BadChargedCandidateFilter.PFCandidates = cms.InputTag("packedPFCandidates")
process.BadChargedCandidateFilter.taggingMode = cms.bool(True)


from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD

if not mc:
    runMetCorAndUncFromMiniAOD(process,
                               isData = True,
                               )
    
    process.load('RecoMET.METFilters.badGlobalMuonTaggersMiniAOD_cff')
    process.badGlobalMuonTaggerMAOD.taggingMode = cms.bool(True)
    process.cloneGlobalMuonTaggerMAOD.taggingMode = cms.bool(True)

    process.mucorMET = cms.Sequence()
                                    
    met = cms.InputTag("slimmedMETs")
 
else:
    runMetCorAndUncFromMiniAOD(process,
                               isData = False,
                               )
    
    process.load('RecoMET.METFilters.badGlobalMuonTaggersMiniAOD_cff')
    process.badGlobalMuonTaggerMAOD.taggingMode = cms.bool(True)
    process.cloneGlobalMuonTaggerMAOD.taggingMode = cms.bool(True)

    from PhysicsTools.PatUtils.tools.muonRecoMitigation import muonRecoMitigation

    muonRecoMitigation(process,
                       pfCandCollection="packedPFCandidates",
                       runOnMiniAOD=True,
                       muonCollection="cleanMuonsPFCandidates",
                       selection="",
                       cleaningScheme="all",
                       postfix="")

    runMetCorAndUncFromMiniAOD(process,
                               isData = False,
                               pfCandColl="cleanMuonsPFCandidates",
                               recoMetFromPFCs=True,
                               postfix="MuClean"
                               )

    process.mucorMET = cms.Sequence(process.badGlobalMuonTaggerMAOD *
                                    process.cloneGlobalMuonTaggerMAOD *
                                    process.badMuons * #if you are using the option "all" 
                                    process.cleanMuonsPFCandidates *
                                    process.fullPatMetSequenceMuClean *
                                    process.fullPatMetSequence
                                    )
   
    met = cms.InputTag("slimmedMETsMuClean")


    
## ==== Analyzer ====
process.analysis = cms.EDAnalyzer("MiniAnalyzer",
                                  beamspot = cms.InputTag("offlineBeamSpot"),
                                  vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                  #electrons = cms.InputTag("slimmedElectrons"),
                                  #electronsID = cms.InputTag('egmGsfElectronIDs:mvaEleID-Spring16-GeneralPurpose-V1-wp90'),
                                  muons = cms.InputTag("MuonsWithTrigger"),
                                  #taus= cms.InputTag("slimmedTaus"),
                                  
                                  jets = cms.InputTag("updatedPatJetsUpdatedJEC"),
                                  puJetIdMVA = cms.InputTag("puJetMva","fullDiscriminant"),
                                  puJetIdFlag = cms.InputTag("puJetMva","fullId"),
                                  puInfo = cms.InputTag("slimmedAddPileupInfo"), 
                                  
                                  mets = met,
                                 
                                  ## MET Filters
                                  metfilterNames = cms.vstring(['Flag_HBHENoiseFilter', 
                                                                'Flag_HBHENoiseIsoFilter', 
                                                                'Flag_EcalDeadCellTriggerPrimitiveFilter', 
                                                                'Flag_goodVertices', 
                                                                'Flag_globalTightHalo2016Filter'
                                                                ]),
                                  metFilterToken = cms.InputTag("TriggerResults"),   
                                  BadChCandFilterToken = cms.InputTag("BadChargedCandidateFilter"),     
                                  BadPFMuonToken = cms.InputTag("BadPFMuonFilter"),

                                  pruned = cms.InputTag("prunedGenParticles"),
                                  packed = cms.InputTag("packedGenParticles"),
                                  bits = bits,
                                  prescales = cms.InputTag("patTrigger"),
                                  objects = cms.InputTag("selectedPatTrigger"),

                                  genEventInfo = cms.InputTag("generator"),

                                  #Trigger_path = path_name,
                                  Trigger_filter = filter_name,
                                                       
                                  pathNames_lowIsoUnpr = cms.string("HLT_IsoMu24_v*"), 
                                  pathNames_lowIsoUnprER = cms.string("HLT_IsoMu22_eta2p1_v*"), 
                                  pathNames_lowIsoTkUnpr = cms.string("HLT_IsoTkMu24_v*"), 
                                  pathNames_lowIsoTkUnprER = cms.string("HLT_IsoTkMu22_eta2p1_v*"), 
                                  
                                  crab = useCrab,
                                  debug = cms.bool(False),
                                  Trigger_debug = cms.bool(False),
                                  
                                  MCtruth = mc,
                                  Signal = signal,
                                  youwantAnalysis= cms.bool(True),
                                  youwantNTuples  = cms.bool(False),
                                  checksOnDY  = cms.bool(False),
                                  
                                  EtaMuonLoose = cms.double(2.4),
                                  PtMuonLoose = cms.double(10.),
                                  EtaMuonTight = cms.double(2.4),
                                  PtMuonTight = cms.double(26.),
                                  Isolation = cms.double(.25),
                                  PtMet = cms.double(40.),
                                  PtJet = cms.double(30.),
                                  EtaJet = cms.double(2.4),
                                  #Particle = cms.int32(23),
                                  Particle = cms.int32(25),
                                  )

process.TFileService = cms.Service('TFileService', 
                                   fileName=cms.string('MSSM_NTupleMC6.root'),
                                   closeFileFast = cms.untracked.bool(False)
                                   )

process.p = cms.Path(process.patTriggerMatching*  process.jecSequence *process.mucorMET *process.fullPatMetSequence * process.BadPFMuonFilter * process.BadChargedCandidateFilter * process.analysis)
#process.p = cms.Path(process.patTriggerMatching*  process.jecSequence * process.BadPFMuonFilter * process.BadChargedCandidateFilter * process.analysis)

#process.p = cms.Path(process.patTriggerMatching* process.analysis)


#f = file('outfile9', 'w')
#f.write(process.dumpPython())
#f.close()

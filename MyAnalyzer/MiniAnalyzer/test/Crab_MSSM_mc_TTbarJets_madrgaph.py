from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'NTuple_mc_TTbarJets_madgraph'
config.General.workArea = 'crab'
#config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../python/MSSM_Analyzer.py'   
#config.JobType.priority = 1
config.JobType.allowUndistributedCMSSW = True

config.Data.inputDataset = '/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'
#'/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'
config.Data.inputDBS = 'global'
#job_control
config.Data.publication = False
config.Data.outputDatasetTag = 'NTuple_mc_TTbarJets_madgraph'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 2000
config.Data.outLFNDirBase = '/store/user/federica'
#config.Data.ignoreLocality = True

#config.Site.whitelist = ["T2_CH_CERN"]
#config.Site.storageSite = 'T2_CH_CERN'
#config.Site.storageSite = 'T2_IT_Legnaro'
config.Site.storageSite = 'T2_CH_CERN'


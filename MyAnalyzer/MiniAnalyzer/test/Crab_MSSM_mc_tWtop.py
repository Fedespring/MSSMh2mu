from CRABClient.UserUtilities import config
config = config()

#config.General.requestName = 'TriggerCheck_mc_DYJetsToLL_M-50_asymptotic'
config.General.requestName = 'NTuple_mc_tWtop_asymptotic_Moriond17_pow'
config.General.workArea = 'crab'
#config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../python/MSSM_Analyzer.py'   
#config.JobType.priority = 1
config.JobType.allowUndistributedCMSSW = True
config.JobType.inputFiles = ['./../data']

config.Data.inputDataset ='/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
config.Data.inputDBS = 'global'
#job_control
config.Data.publication = False
#config.Data.outputDatasetTag = 'TriggerCheck_mc_DYJetsToLL_M-50_asymptotic'
config.Data.outputDatasetTag =  'NTuple_mc_tWtop_asymptotic_Moriond17_pow'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 2000
config.Data.outLFNDirBase = '/store/user/federica'
#config.Data.ignoreLocality = True

#config.Site.whitelist = ["T2_CH_CERN"]
#config.Site.storageSite = 'T2_CH_CERN'
#config.Site.storageSite = 'T2_IT_Legnaro'
config.Site.storageSite = 'T2_CH_CERN'


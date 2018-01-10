
from CRABClient.UserUtilities import config
config = config()

config.General.requestName = "MSSMggA-HiggsToMuMu_MA-350_Tanb-35_13TeV_pythia8"
config.General.workArea = 'crab'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../python/MSSM_Analyzer.py'   
config.JobType.allowUndistributedCMSSW = True

config.Data.inputDataset = "/MSSMggA-HiggsToMuMu_MA-350_Tanb-35_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
config.Data.inputDBS = 'global'
config.Data.publication = False
config.Data.outputDatasetTag = "MSSMggA-HiggsToMuMu_MA-350_Tanb-35_13TeV_pythia8"
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 20000
config.Data.outLFNDirBase = '/store/user/federica/XXX'

config.Site.storageSite = 'T2_CH_CERN'

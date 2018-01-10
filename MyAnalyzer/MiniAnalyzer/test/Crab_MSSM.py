from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'NTuple_data_2016G_ReReco'
config.General.workArea = 'crab'
#config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../python/MSSM_Analyzer.py'   
#config.JobType.priority = 1
config.JobType.allowUndistributedCMSSW = True

#config.Data.inputDataset = '/SingleMuon/Run2016B-23Sep2016-v1/MINIAOD'
#config.Data.inputDataset = '/SingleMuon/Run2016B-23Sep2016-v3/MINIAOD'
#config.Data.inputDataset = '/SingleMuon/Run2016C-23Sep2016-v1/MINIAOD'
#config.Data.inputDataset = '/SingleMuon/Run2016D-23Sep2016-v1/MINIAOD'
#config.Data.inputDataset = '/SingleMuon/Run2016E-23Sep2016-v1/MINIAOD'
#config.Data.inputDataset = '/SingleMuon/Run2016F-23Sep2016-v1/MINIAOD'
config.Data.inputDataset = '/SingleMuon/Run2016G-23Sep2016-v1/MINIAOD'

config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 100
config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_MuonPhys.txt'
#'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/Cert_271036-280385_13TeV_PromptReco_Collisions16_JSON_NoL1T_v2.txt'
#config.Data.runRange = '193093-193999' # '193093-194075'
config.Data.outLFNDirBase = '/store/user/federica'
#config.Data.ignoreLocality = True
config.Data.publication = False
config.Data.outputDatasetTag = 'NTuple_data_2016G_ReReco'


#config.Site.whitelist = ["T2_CH_CERN"]
config.Site.storageSite = 'T2_IT_Legnaro'
#config.Site.storageSite = 'T2_CH_CERN'

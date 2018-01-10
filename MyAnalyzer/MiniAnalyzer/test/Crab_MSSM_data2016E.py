from CRABClient.UserUtilities import config
config = config()
CUTmst = '_Histos'
#period = 'Run2016B-03Feb2017_ver1-v1'
#period = 'Run2016B-03Feb2017_ver2-v2'
#period = 'Run2016C-03Feb2017-v1'
#period = 'Run2016D-03Feb2017-v1'
period = 'Run2016E-03Feb2017-v1'
#period = 'Run2016F-03Feb2017-v1'
#period = 'Run2016G-03Feb2017-v1'
#period = 'Run2016H-03Feb2017_ver2-v1'
#period = 'Run2016H-03Feb2017_ver3-v1'

config.General.requestName = 'NTuple_data_' + period #+ CUTmst
config.General.workArea = 'crab'
#config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../python/MSSM_Analyzer.py'   
#config.JobType.priority = 1
config.JobType.allowUndistributedCMSSW = True

config.Data.inputDataset = '/SingleMuon/'+period+'/MINIAOD'

config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 500
config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
#'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/Cert_271036-280385_13TeV_PromptReco_Collisions16_JSON_NoL1T_v2.txt'
#config.Data.runRange = '193093-193999' # '193093-194075'
config.Data.outLFNDirBase = '/store/user/federica'
#config.Data.ignoreLocality = True
config.Data.publication = False
config.Data.outputDatasetTag = 'NTuple_data_' + period #+ CUTmst


#config.Site.whitelist = ["T2_CH_CERN"]
#config.Site.storageSite = 'T2_IT_Legnaro'
config.Site.storageSite = 'T2_CH_CERN'

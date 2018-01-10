#!/usr/bin/env python
import os
import re
import sys
import urllib
import re
import popen2



#f = open('MSSM_13TeV_Signal_Samples3.txt', 'r')
f = open('MSSM_13TeV_Signal_Samples.txt', 'r')
#f = open('MSSM_13TeV_Signal_Samples2.txt', 'r')

for line in f:
    line = line.replace('\n', '')   
    signal = "/"+line+"/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
   
    print signal
   
    crab_cfg = '''
from CRABClient.UserUtilities import config
config = config()

config.General.requestName = "'''+line+'''"
config.General.workArea = 'crab'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../python/MSSM_Analyzer.py'   
config.JobType.allowUndistributedCMSSW = True

config.Data.inputDataset = "'''+signal+'''"
config.Data.inputDBS = 'global'
config.Data.publication = False
config.Data.outputDatasetTag = "'''+line+'''"
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 20000
config.Data.outLFNDirBase = '/store/user/federica'

#config.Site.storageSite = 'T3_IT_Bologna'
config.Site.storageSite = 'T2_CH_CERN'
'''
    #print crab_cfg
    open('Crab_MSSM_mc_Signal.py', 'w').write(crab_cfg)

    os.system('crab submit -c Crab_MSSM_mc_Signal.py')
    os.system('rm Crab_MSSM_mc_Signal.py')

##/MSSMbbA-HiggsToMuMu_MA-150_Tanb-30_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM

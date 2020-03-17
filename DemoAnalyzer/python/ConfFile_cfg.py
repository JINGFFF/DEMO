import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.MessageLogger.cerr.FwkReport.reportEvery = 10

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        '/store/mc/RunIISummer16MiniAODv3/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/120000/44B0FBC3-45DF-E811-8A71-842B2B6AE6F0.root'
        #'/store/mc/RunIIFall17MiniAODv2/DY1JetsToLL_M-50_LHEZpT_250-400_TuneCP5_13TeV-amcnloFXFX-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/10000/4AA090F1-0AB2-E811-A684-A0369F5BE308.root'
	#'/store/mc/RunIIFall17MiniAODv2/DY1JetsToLL_M-50_LHEZpT_150-250_TuneCP5_13TeV-amcnloFXFX-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/80000/F01104EF-2064-E811-B385-008CFAC94150.root'
	#'/store/mc/RunIIFall17MiniAODv2/DY1JetsToLL_M-50_LHEZpT_50-150_TuneCP5_13TeV-amcnloFXFX-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/80000/EACAC817-5B61-E811-869A-0CC47AA98F98.root'
	#'/store/mc/RunIIFall17MiniAODv2/DY1JetsToLL_M-50_LHEZpT_50-150_TuneCP5_13TeV-amcnloFXFX-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/80000/EACAC817-5B61-E811-869A-0CC47AA98F98.root'
    )
)

process.TFileService = cms.Service('TFileService',
    fileName = cms.string("eff.root")
)

process.demo = cms.EDAnalyzer('DemoAnalyzer',
    gentag = cms.InputTag('generator'),
    lhetag = cms.InputTag('externalLHEProducer'),
)


process.p = cms.Path(process.demo)

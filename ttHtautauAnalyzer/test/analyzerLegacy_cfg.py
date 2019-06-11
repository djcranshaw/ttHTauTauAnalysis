### CMSSW_9_4_4

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("ttH")

### initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.option = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

### Run in unscheduled mode
#process.options = cms.untracked.PSet( allowUnscheduled = cms.untracked.bool(True)

###
options = VarParsing.VarParsing('analysis')

options.register('Debug', False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Debug mode")

options.register('isData', False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Run on collision data or not")

options.register('SampleName','',  #'ttH'
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Sample name")

#options.register('TurnOffEvtSel', False,
options.register('TurnOffEvtSel', True,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Turn off event selection")

options.register('doCutFlow', False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Fill Cutflow histogram or not")

options.register('doSync', False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "For Synchronization")

###
options.maxEvents = 5000
#options.inputFiles='file:/uscms/home/ztao/nobackup/datasample/ttH_94X/ttHJetToNonbb.root'
#options.inputFiles='/store/mc/RunIIAutumn18MiniAOD/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v2/270000/FFCEE41A-EB86-6642-A812-80F9FDDFA2CA.root'
#options.inputFiles='/store/mc/RunIISummer16MiniAODv3/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_94X_mcRun2_asymptotic_v3-v2/120000/F24F2D5E-DDEC-E811-AF50-90B11C08AD7D.root'
options.inputFiles='file:/uscms/home/dcransha/nobackup/RunIISummer16MiniAODv3_F24F2D5E-DDEC-E811-AF50-90B11C08AD7D.root'
#options.inputFiles='file:~/work/CMSSW_10_2_13/src/ttHTauTauAnalysis/ttHtautauAnalyzer/test/pickEventsOutput.root'
#/store/mc/RunIIFall17MiniAOD/ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8/MINIAODSIM/94X_mc2017_realistic_v10-v1/20000/0CF65340-0200-E811-ABB7-0025905C53F0.root

# get and parse the command line arguments
options.parseArguments()

process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Geometry.CaloEventSetup.CaloTowerConstituents_cfi")

### Global tag
process.load('Configuration.StandardSequences.Services_cff')
process.load( "Configuration.StandardSequences.FrontierConditions_GlobalTag_cff" )

#MCGT = '94X_mc2017_realistic_v13'
#MCGT = '94X_mcRun2_asymptotic_v3' #Newer
MCGT = '102X_mcRun2_asymptotic_v6' #Newest
DATAGT = '94X_dataRun2_v6'

process.GlobalTag.globaltag = DATAGT if options.isData else MCGT

process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(options.maxEvents)
)

############################
### Filters for running on data
#if options.isData:
if True:
    # primary vertex filter
    process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                               vertexCollection = cms.InputTag('offlineSlimmedPrimaryVertices'),
                                               minimumNDOF = cms.uint32(4) ,
                                               maxAbsZ = cms.double(24), 
                                               maxd0 = cms.double(2) 
    )
##############################

### Inputs
process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(options.inputFiles)
)

### JEC
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection

JECLevel = cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute'])
if options.isData:
    JECLevel.append('L2L3Residual')

updateJetCollection(
    process,
    jetSource = cms.InputTag('slimmedJets'),
    labelName = 'UpdatedJEC',
    jetCorrections = ('AK4PFchs', JECLevel, 'None')
)

## Quark-gluon likelihood
# add the database object
qgDatabaseVersion = 'cmssw8020_v2'

from CondCore.CondDB.CondDB_cfi import *
#CondDB.connect = cms.string('frontier://FrontierProd/CMS_COND_PAT_000')
CondDB.connect = cms.string('sqlite:qg/QGL_'+qgDatabaseVersion+'.db')
process.QGPoolDBESSource = cms.ESSource("PoolDBESSource", CondDB,
                                toGet = cms.VPSet(),
)

for type in ['AK4PFchs','AK4PFchs_antib']:
    process.QGPoolDBESSource.toGet.extend(cms.VPSet(cms.PSet(
        record = cms.string('QGLikelihoodRcd'),
        tag    = cms.string('QGLikelihoodObject_'+qgDatabaseVersion+'_'+type),
        label  = cms.untracked.string('QGL_'+type)
    )))

process.es_prefer_QGL = cms.ESPrefer("PoolDBESSource","QGPoolDBESSource")
  
# load QGTagger
process.load('RecoJets.JetProducers.QGTagger_cfi')
process.QGTagger.srcJets = cms.InputTag("updatedPatJetsUpdatedJEC")
process.QGTagger.srcVertexCollection=cms.InputTag("offlineSlimmedPrimaryVertices")
process.QGTagger.jetsLabel = cms.string('QGL_AK4PFchs')

### Electron scale and smearing + MVA IDs
from RecoEgamma.EgammaTools.EgammaPostRecoTools import setupEgammaPostRecoSeq
#setupEgammaPostRecoSeq(process,applyEnergyCorrections=not options.doSync,
#                       applyVIDOnCorrectedEgamma=not options.doSync,
#                       isMiniAOD=True,
#                       era='2017-Nov17ReReco')
setupEgammaPostRecoSeq(process,applyEnergyCorrections=not options.doSync,
                       applyVIDOnCorrectedEgamma=not options.doSync,
                       isMiniAOD=True,
                       era='2016-Legacy')


### Electron MVA VID-based receipe
#from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
#switchOnVIDElectronIdProducer(process, DataFormat.MiniAOD)

#my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_noIso_V1_cff', 'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V1_cff']
#for idmod in my_id_modules:
#    setupAllVIDIdsInModule(process, idmod, setupVIDElectronSelection)

# ##################################################
# # rerun tau ID
from ttHTauTauAnalysis.ttHtautauAnalyzer.runTauIdMVA import *
na = TauIDEmbedder(process, cms, 
                   debug=True,
                   toKeep = ["dR0p32017v2"] # pick the one you need: ["2017v1", "2017v2", "newDM2017v2", "dR0p32017v2", "2016v1", "newDM2016v1"]
)
na.runTauID()

### MET correction 
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
runMetCorAndUncFromMiniAOD(process,
                           isData=options.isData,
                           )

### load the analysis
# LeptonID producer from ttH Multi-lepton group
process.load("ttH.LeptonID.ttHLeptons_cfi")
# Analyzer
process.load("ttHTauTauAnalysis.ttHtautauAnalyzer.ttHtaus_cfi")

### update parameter sets
process.ttHLeptons.rhoParam = "fixedGridRhoFastjetAll"
# if rerun tauID
#process.ttHLeptons.electrons = cms.InputTag("slimmedElectrons","","ttH")
process.ttHLeptons.taus = cms.InputTag("NewTauIDsEmbedded")
#
process.ttHLeptons.jets = cms.InputTag("updatedPatJetsUpdatedJEC")
#process.ttHLeptons.JECTag = cms.string("patJetCorrFactorsUpdatedJEC")
process.ttHLeptons.LooseCSVWP = cms.double(0.1522)  # DeepCSV WP
process.ttHLeptons.MediumCSVWP = cms.double(0.4941) # DeepCSV WP
process.ttHLeptons.mvaValuesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Fall17NoIsoV1Values")
process.ttHLeptons.mvaCategoriesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Fall17NoIsoV1Categories")
#process.ttHLeptons.IsHIPSafe = cms.bool(options.HIPSafeMediumMuon)

process.ttHtaus.electrons = cms.InputTag("ttHLeptons")
process.ttHtaus.muons = cms.InputTag("ttHLeptons")
process.ttHtaus.taus = cms.InputTag("ttHLeptons")
process.ttHtaus.jets = cms.InputTag("updatedPatJetsUpdatedJEC")
process.ttHtaus.mets = cms.InputTag("slimmedMETs","","ttH")
process.ttHtaus.rho = cms.InputTag("fixedGridRhoFastjetAll")
#process.ttHtaus.do_systematics = cms.bool(options.doSystematics)
process.ttHtaus.turn_off_event_sel = cms.bool(options.TurnOffEvtSel)
process.ttHtaus.sample_name = cms.string(options.SampleName)
process.ttHtaus.using_collision_data = cms.bool(options.isData)
process.ttHtaus.store_gen_objects = cms.bool(True)
process.ttHtaus.debug_mode = cms.bool(options.Debug)
process.ttHtaus.do_sync = cms.bool(options.doSync)
process.ttHtaus.doCutFlow = cms.bool(options.doCutFlow)
process.ttHtaus.verbosity = cms.int32(1)
# DeepCSV WPs 
process.ttHtaus.csv_loose_wp = cms.double(0.1522)
process.ttHtaus.csv_medium_wp = cms.double(0.4941)
process.ttHtaus.csv_tight_wp = cms.double(0.8001)


### Output
out_file = 'output_' + options.SampleName + '.root'

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(out_file)
)

### Performance
process.Timing = cms.Service("Timing",
                             summaryOnly = cms.untracked.bool(True),
                             useJobReport = cms.untracked.bool(True)
)
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        oncePerEventMode=cms.untracked.bool(True)
#                                        )
#process.Tracer = cms.Service('Tracer')

#Path
process.p = cms.Path(
    process.primaryVertexFilter *
    process.patJetCorrFactorsUpdatedJEC * process.updatedPatJetsUpdatedJEC *     
    #process.egmGsfElectronIDSequence *
    process.egammaPostRecoSeq *
    process.rerunMvaIsolationSequence * process.NewTauIDsEmbedded * # *getattr(process, "NewTauIDsEmbedded")
    process.fullPatMetSequence *
    process.ttHLeptons *
    process.QGTagger *
    process.ttHtaus
)

### CMSSW_9_4_4

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("ttH")

### initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
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

options.register('TauESType', 'NA',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Tau energy scale: NA, tauESUp, tauESDown")

options.register('JECType', 'NA',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "JEC type: NA, JESUp, JESDown, JERUp, JERDown")

options.register("doJERSmearing", False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "apply jet energy smearing for MC or not")

options.register('TurnOffEvtSel', False, 
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Turn off event selection")

options.register('TurnOffHLTCut', True,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Turn off HLT path check in event selection")

options.register('AnalysisType', '2lss1tau',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Analysis type currently supported: 1l2tau, 2lss1tau, 3l1tau")

options.register('SelectionRegion', 'signal_2lss1tau',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Which selection region to apply: signal_2lss1tau, control_2los1tau, control_fake_2lss1tau, signal_1l2tau, control_fake_1l2tau, signal_3l1tau, control_fake_3l1tau, loose_2lss1tau, loose_1l2tau")

options.register('doSystematics', True,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Include systematics or not")

options.register('doCutFlow', False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Fill Cutflow histogram or not")

options.register('doSync', False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "For Synchronization")

###
options.maxEvents = -1
options.inputFiles='file:/uscms/home/ztao/nobackup/datasample/ttH_94X/ttHJetToNonbb.root'
#/store/mc/RunIIFall17MiniAOD/ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8/MINIAODSIM/94X_mc2017_realistic_v10-v1/20000/0CF65340-0200-E811-ABB7-0025905C53F0.root

# get and parse the command line arguments
options.parseArguments()

### Global tag
process.load('Configuration.StandardSequences.Services_cff')
process.load( "Configuration.StandardSequences.FrontierConditions_GlobalTag_cff" )

MCGT = '94X_mc2017_realistic_v13'
DATAGT = '94X_dataRun2_v6'

process.GlobalTag.globaltag = DATAGT if options.isData else MCGT

process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(options.maxEvents)
)

############################
### Filters for running on data
if options.isData:
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


### Electron MVA VID-based receipe
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
switchOnVIDElectronIdProducer(process, DataFormat.MiniAOD)

my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V1_cff']
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process, idmod, setupVIDElectronSelection)

##################################################
# rerun tau ID
from RecoTauTag.RecoTau.TauDiscriminatorTools import noPrediscriminants
process.load('RecoTauTag.Configuration.loadRecoTauTagMVAsFromPrepDB_cfi')
from RecoTauTag.RecoTau.PATTauDiscriminationByMVAIsolationRun2_cff import *

tauIdDiscrMVA_trainings_run2_2017 = {
  'tauIdMVAIsoDBoldDMwLT2017' : "tauIdMVAIsoDBoldDMwLT2017",
  }
tauIdDiscrMVA_WPs_run2_2017 = {
  'tauIdMVAIsoDBoldDMwLT2017' : {
    'Eff95' : "DBoldDMwLTEff95",
    'Eff90' : "DBoldDMwLTEff90",
    'Eff80' : "DBoldDMwLTEff80",
    'Eff70' : "DBoldDMwLTEff70",
    'Eff60' : "DBoldDMwLTEff60",
    'Eff50' : "DBoldDMwLTEff50",
    'Eff40' : "DBoldDMwLTEff40"
    }
  }
tauIdDiscrMVA_2017_version = "v1"

def loadMVA_WPs_run2_2017(process):
                
      for training, gbrForestName in tauIdDiscrMVA_trainings_run2_2017.items():

         process.loadRecoTauTagMVAsFromPrepDB.toGet.append(
            cms.PSet(
               record = cms.string('GBRWrapperRcd'),
               tag = cms.string("RecoTauTag_%s%s" % (gbrForestName, tauIdDiscrMVA_2017_version)),
               label = cms.untracked.string("RecoTauTag_%s%s" % (gbrForestName, tauIdDiscrMVA_2017_version))
            )
         )

         for WP in tauIdDiscrMVA_WPs_run2_2017[training].keys():
            process.loadRecoTauTagMVAsFromPrepDB.toGet.append(
               cms.PSet(
                  record = cms.string('PhysicsTGraphPayloadRcd'),
                  tag = cms.string("RecoTauTag_%s%s_WP%s" % (gbrForestName, tauIdDiscrMVA_2017_version, WP)),
                  label = cms.untracked.string("RecoTauTag_%s%s_WP%s" % (gbrForestName, tauIdDiscrMVA_2017_version, WP))
               )
            )

         process.loadRecoTauTagMVAsFromPrepDB.toGet.append(
            cms.PSet(
               record = cms.string('PhysicsTFormulaPayloadRcd'),
               tag = cms.string("RecoTauTag_%s%s_mvaOutput_normalization" % (gbrForestName, tauIdDiscrMVA_2017_version)),
               label = cms.untracked.string("RecoTauTag_%s%s_mvaOutput_normalization" % (gbrForestName, tauIdDiscrMVA_2017_version))
            )
)

loadMVA_WPs_run2_2017(process)

process.rerunDiscriminationByIsolationMVArun2v1raw = patDiscriminationByIsolationMVArun2v1raw.clone(
   PATTauProducer = cms.InputTag('slimmedTaus'),
   Prediscriminants = noPrediscriminants,
   loadMVAfromDB = cms.bool(True),
   mvaName = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v1"),  # name of the training you want to use: training with 2017 MC_v1 for oldDM
   mvaOpt = cms.string("DBoldDMwLTwGJ"), # option you want to use for your training (i.e., which variables are used to compute the BDT score)
   requireDecayMode = cms.bool(True),
   verbosity = cms.int32(0)
)

process.rerunDiscriminationByIsolationMVArun2v1VLoose = patDiscriminationByIsolationMVArun2v1VLoose.clone(
   PATTauProducer = cms.InputTag('slimmedTaus'),    
   Prediscriminants = noPrediscriminants,
   toMultiplex = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1raw'),
   key = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1raw:category'),
   loadMVAfromDB = cms.bool(True),
   mvaOutput_normalization = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v1_mvaOutput_normalization"), # normalization fo the training you want to use
   mapping = cms.VPSet(
      cms.PSet(
         category = cms.uint32(0),
         cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v1_WPEff90"), # this is the name of the working point you want to use
         variable = cms.string("pt"),
      )
   )
)

process.rerunDiscriminationByIsolationMVArun2v1VVLoose = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1VVLoose.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v1_WPEff95")
process.rerunDiscriminationByIsolationMVArun2v1Loose = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1Loose.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v1_WPEff80")
process.rerunDiscriminationByIsolationMVArun2v1Medium = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1Medium.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v1_WPEff70")
process.rerunDiscriminationByIsolationMVArun2v1Tight = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1Tight.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v1_WPEff60")
process.rerunDiscriminationByIsolationMVArun2v1VTight = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1VTight.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v1_WPEff50")
process.rerunDiscriminationByIsolationMVArun2v1VVTight = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1VVTight.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v1_WPEff40")

process.rerunMvaIsolation2SeqRun2 = cms.Sequence(
   process.rerunDiscriminationByIsolationMVArun2v1raw
   *process.rerunDiscriminationByIsolationMVArun2v1VLoose
   *process.rerunDiscriminationByIsolationMVArun2v1VVLoose
   *process.rerunDiscriminationByIsolationMVArun2v1Loose
   *process.rerunDiscriminationByIsolationMVArun2v1Medium
   *process.rerunDiscriminationByIsolationMVArun2v1Tight
   *process.rerunDiscriminationByIsolationMVArun2v1VTight
   *process.rerunDiscriminationByIsolationMVArun2v1VVTight
)

# embed new id's into new tau collection
embedID = cms.EDProducer("PATTauIDEmbedder",
   src = cms.InputTag('slimmedTaus'),
   tauIDSources = cms.PSet(
      byIsolationMVArun2v1DBoldDMwLTrawNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1raw'),
      byVLooseIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1VLoose'),
      byVVLooseIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1VVLoose'),
      byLooseIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1Loose'),
      byMediumIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1Medium'),
      byTightIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1Tight'),
      byVTightIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1VTight'),
      byVVTightIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1VVTight'),
   ),
)

setattr(process, "NewTauIDsEmbedded", embedID)

##################################################
    
### load the analysis
# LeptonID producer from ttH Multi-lepton group
process.load("ttH.LeptonID.ttHLeptons_cfi")
# Analyzer
process.load("ttHTauTauAnalysis.ttHtautauAnalyzer.ttHtaus_cfi")

### update parameter sets
process.ttHLeptons.rhoParam = "fixedGridRhoFastjetCentralNeutral"
# if rerun tauID
#process.ttHLeptons.taus = cms.InputTag("NewTauIDsEmbedded")
#
process.ttHLeptons.jets = cms.InputTag("updatedPatJetsUpdatedJEC")
process.ttHLeptons.LooseCSVWP = cms.double(0.5803)
process.ttHLeptons.MediumCSVWP = cms.double(0.8838)
process.ttHLeptons.mvaValuesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Fall17IsoV1Values")
process.ttHLeptons.mvaCategoriesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Fall17IsoV1Categories")
#process.ttHLeptons.IsHIPSafe = cms.bool(options.HIPSafeMediumMuon)

process.ttHtaus.electrons = cms.InputTag("ttHLeptons")
process.ttHtaus.muons = cms.InputTag("ttHLeptons")
process.ttHtaus.taus = cms.InputTag("ttHLeptons")
process.ttHtaus.jets = cms.InputTag("updatedPatJetsUpdatedJEC")
process.ttHtaus.mets = cms.InputTag("slimmedMETs")
process.ttHtaus.rho = cms.InputTag("fixedGridRhoFastjetCentralNeutral")
process.ttHtaus.do_systematics = cms.bool(options.doSystematics)
process.ttHtaus.turn_off_event_sel = cms.bool(options.TurnOffEvtSel)
process.ttHtaus.sample_name = cms.string(options.SampleName)
process.ttHtaus.TauESType = cms.string(options.TauESType)
process.ttHtaus.JECType = cms.string(options.JECType)
process.ttHtaus.using_real_data = cms.bool(options.isData)
process.ttHtaus.analysis_type = cms.string(options.AnalysisType)
process.ttHtaus.selection_region = cms.string(options.SelectionRegion)
process.ttHtaus.turn_off_HLT_cut = cms.bool(options.TurnOffHLTCut)
process.ttHtaus.debug_mode = cms.bool(options.Debug)
process.ttHtaus.do_sync = cms.bool(options.doSync)
process.ttHtaus.doCutFlow = cms.bool(options.doCutFlow)
process.ttHtaus.doJERsmear = cms.bool(options.doJERSmearing)
# CSV WPs 
process.ttHtaus.csv_loose_wp = cms.double(0.5803)
process.ttHtaus.csv_medium_wp = cms.double(0.8838)
process.ttHtaus.csv_tight_wp = cms.double(0.9693)


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
if options.isData:
    process.p = cms.Path(
        process.primaryVertexFilter *
        process.patJetCorrFactorsUpdatedJEC * process.updatedPatJetsUpdatedJEC *
        process.egmGsfElectronIDSequence *
        #process.rerunMvaIsolation2SeqRun2 * getattr(process, "NewTauIDsEmbedded") *
        process.ttHLeptons *
        process.ttHtaus
    )
else:
    process.p = cms.Path(
        process.patJetCorrFactorsUpdatedJEC * process.updatedPatJetsUpdatedJEC *
        process.egmGsfElectronIDSequence *
        #process.rerunMvaIsolation2SeqRun2 * getattr(process, "NewTauIDsEmbedded") *
        process.ttHLeptons *
        process.ttHtaus
    )
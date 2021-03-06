#!/usr/bin/env python
import os
from ROOT import TFile, TH1D, TTree, gROOT
import math
gROOT.SetBatch(True)
import ttHTauTauAnalysis.ttHtautauAnalyzer.plotting as myplt
import ttHTauTauAnalysis.ttHtautauAnalyzer.datacards as dc
from ttHTauTauAnalysis.ttHtautauAnalyzer.crab_utils import getDatasetDict

import argparse

def getNtupleFileName(sample,selection,channel=None,correction=None,eosprefix=''):

    filename = 'mvaNtuple_'+sample+'_'
    
    if 'data' in sample:  # data_e, data_mu, data_dieg, data_dimu, data_mueg
        assert(channel is not None)

        if 'signal' in selection:
            if 'fakes' in channel:
                filename += selection.replace('signal','application_fake')+'.root'
            elif 'flips' in channel:
                filename += selection.replace('signal','application_flip')+'.root'
            else:  # data_obs
                filename += selection+'.root'
        elif 'control' in selection:
            if 'fakes' in channel:
                filename += selection.replace('control','control_fakeAR')+'.root'
            elif 'flips' in channel:
                filename += selection.replace('control','control_flipAR')+'.root'
            else:  # data_obs
                filename += selection+'.root'

    else:  # MC sample
        filename += selection+'.root'
        
        if correction is not None:  # jesup, jesdown, tesup, tesdown
            filename = filename.replace('.root','_'+correction.lower()+'.root')

    filename = eosprefix + filename
            
    return filename

    
def getHistogramfromTrees(trees, sample_label, variable, xmin, xmax, nbins,
                          selection, weight_name='event_weight', transform=False,
                          isPhotonConvs=False, applySF=False, invertGenMatch=False):

    if len(trees)<1:
        print "getHistogramfromTrees: No input trees!"
        return None

    # extract temporary histogram from tree
    #htmp = myplt.DrawHistfromTree(trees[0], variable)
    #nbin = htmp.GetNbinsX()
    #xmin = htmp.GetXaxis().GetXmin()
    #xmax = htmp.GetXaxis().GetXmax()
    #print "nbin, xmin, xmax = ", nbin, xmin, xmax

    # setup output histogram
    histname = variable+'_'+sample_label
    
    h = TH1D(histname, "", nbins, xmin, xmax)
    h.Sumw2()
    
    eventIDlist = []
    eventListAux = []
    nEntries = 0
    sumweight = 0.
    
    singletree = len(trees)==1
    
    for tree in trees:
        if tree.GetEntries()==0:
            if args.verbose>3:
                print "WARNING: empty tree for", sample_label
            continue
        
        for ev in tree:
            # if there are more than one trees, need to merge the entries by
            # removing duplicate events based on evnet ID: (run, lumi, nEvent)
            if not singletree:
                eventID = (ev.run, ev.lumi, ev.nEvent)
                #eventID = (ev.run, int(ev.GetLeaf('ls').GetValue()), ev.nEvent)
                if eventID in eventIDlist:
                    continue
                eventIDlist.append(eventID)
                nEntries += 1

            ##########
            # gen matching
            if not 'data' in sample_label:
                genmatched=False
                if isPhotonConvs:
                    genmatched = ev.isGenPhotonMatched
                elif '1l2tau' in selection or '2l2tau' in selection:
                    genmatched = ev.isGenMatchedLep and ev.isGenMatchedTau
                else: # 2lss1tau or 3l1tau
                    genmatched = ev.isGenMatchedLep

                if invertGenMatch:  # for fakes_mc
                    genmatched = not genmatched
                    
                if not genmatched:
                    continue
                
            ##########
                
            # gen match and higgs decay mode filters if necessary
            if 'gentau' in sample_label.lower() and not ev.isGenMatchedTau:
                continue
            if 'faketau' in sample_label.lower() and ev.isGenMatchedTau:
                continue
            if '_htt' in sample_label.lower() and abs(ev.HiggsDecayType)!=15:
                continue
            if '_hww' in sample_label.lower() and abs(ev.HiggsDecayType)!=24:
                continue
            if '_hzz' in sample_label.lower() and abs(ev.HiggsDecayType)!=23:
                continue

            value = ev.GetLeaf(variable).GetValue()
            # modify here if dealing with special case, e.g. 2D mapping
            if transform and 'mvaOutput' in variable:
                value = transform_mva(value)
            
            evtweight = ev.GetLeaf(weight_name).GetValue()
            if applySF:  # special case e.g. MC sample in fake AR
                assert('data' not in sample_label)
                evtweight *= (ev.pu_weight * ev.mc_weight * ev.btag_sf *
                              ev.lepid_sf * ev.tauid_sf * ev.hlt_sf)
            
            sumweight += evtweight

            #eventListAux.append((ev.run, ev.lumi, ev.nEvent, evtweight))

            # handle under and overflow bins
            value = max(xmin, value)
            value = min(xmax-1e-5, value)
            
            h.Fill(value, evtweight)

    #eventListAux.sort()
    #print "event list with weight: "
    #for id in eventListAux:
    #    print id
    #print "sumweight =", sumweight
    #print "nEntries =", nEntries
            
    return h

def transform_mva(mva):
    newmva = 1. / (1. + math.sqrt((1. - mva) / (1. + mva)))
    return newmva

def getHistogramFromMCNtuple(variable, sample, selection, luminosity, xmin, xmax,
                             nbins, eosPrefix='', sample_suffix='',
                             evtweight='event_weight', treename='mva',
                             makeBinPositive=False, transform=False,
                             datasetlist_dict=None, isPhotonConvs=False,
                             applySF=False, invertGenMatch=False):
    # sample_suffix = _gentau, _faketau, _htt, _hww, _hzz, _<systematics>, ...

    jec=None
    for ec in ['jesup','jesdown','tesup','tesdown']:
        if ec in sample_suffix.lower():
            jec = ec
            assert(ec in evtweight.lower())
            evtweight = 'event_weight'
    
    # open ntuple file and get tree
    ntuplename = getNtupleFileName(sample, selection, correction=jec,
                                   eosprefix=eosPrefix)
    #print "opening ntuple file :", ntuplename
    
    infile = TFile.Open(ntuplename)
    tree = infile.Get(treename)

    # get sample cross section and SumGenWeight from tree user info
    SumGenWeight = tree.GetUserInfo().FindObject('SumGenWeight').GetVal()  
    xsection = tree.GetUserInfo().FindObject('xsection').GetVal() if datasetlist_dict is None else float(datasetlist_dict[sample]['xsection'])
    if xsection < 0.:
        print "WARNING: cross section for sample", sample, " is not valid!"

    # get histogram
    label = sample.lower()+sample_suffix
    hist = getHistogramfromTrees([tree],label, variable, xmin, xmax, nbins, selection,
                                 weight_name=evtweight, transform=transform,
                                 isPhotonConvs=isPhotonConvs,
                                 applySF=applySF, invertGenMatch=invertGenMatch)

    # scale
    lumi = luminosity*1000  # convert from 1/fb to 1/pb
    hist.Scale(lumi * xsection / SumGenWeight)

    # make bin contents positive if needed 
    if makeBinPositive:
        dc.makeBinContentsPositive(hist)

    hist.SetDirectory(0)
    
    return hist

def getHistogramFromDataNtuples(variable, channel, selection, xmin, xmax, nbins,
                                eosPrefix='', channel_suffix='',
                                evtweight='event_weight', treename='mva',
                                makeBinPositive=False, transform=False):
    # determine event weight string based on histname

    
    # channel_suffix = _<systematics>
    
    # open ntuple files and get list of trees
    listOfntupleNames = [getNtupleFileName(sample, selection, channel=channel, eosprefix=eosPrefix) for sample in getSampleListByChannel(channel)]
    #print "opening ntuple files :", listOfntupleNames
    
    infiles = [TFile.Open(ntuplename) for ntuplename in listOfntupleNames]
    trees = [fin.Get(treename) for fin in infiles]

    # get histogram
    label = channel+channel_suffix
    hist = getHistogramfromTrees(trees, label, variable, xmin, xmax, nbins, selection,
                                 weight_name=evtweight,transform=transform)

    # make bin contents positive if needed
    if makeBinPositive:
        dc.makeBinContentsPositive(hist)

    hist.SetDirectory(0)

    return hist


#def getSampleListByChannel(channel, datasetCSV): # datacard.py   
def getSampleListByChannel(channel):
    return dc.SamplesInChannel2017[channel]



def getHistSuffixandWeightNames_data(channel, selection, addSyst,
                                     syst_label='_CMS_ttHl_'):
    hnamelist=[''] # nominal
    wnamelist=['event_weight'] # nominal weight

    if not (addSyst and 'fakes' in channel):
        return hnamelist, wnamelist

    # fake rate systematics
    systlist = [frl for frl in dc.FakeRateLepSysts]
    if '1l2tau' in selection or '2l2tau' in selection:
        systlist += [frjt for frjt in dc.FakeTauSysts]

    hnamelist += [syst_label+syst for syst in systlist]
    wnamelist += ['event_weight_'+syst for syst in systlist]

    return hnamelist, wnamelist
    
    
def getHistSuffixandWeightNames_mc(sample, selection, addSyst, matchGenTau=False,
                                   splitHiggsDecay=False, syst_label='_CMS_ttHl_',
                                   lowercase=True):
    s = '_'+sample.lower() if lowercase else '_'+sample

    hnamelist = [''] # nominal
    wnamelist = ['event_weight']  # nominal weight
    
    if matchGenTau:
        hnamelist += ['_gentau', '_faketau']
        wnamelist += ['event_weight', 'event_weight']
    if 'ttH' in sample and splitHiggsDecay:
        hnamelist += ['_htt', '_hww', '_hzz']
        wnamelist += ['event_weight']*3
        if matchGenTau:
            hnamelist += ['_htt_gentau', '_htt_faketau', '_hww_gentau',
                         '_hww_faketau', '_hzz_gentau', '_hzz_faketau']
            wnamelist += ['event_weight']*6

    if not addSyst:
        return hnamelist, wnamelist

    systlist = ['JESUp','JESDown','TESUp','TESDown']
    #systlist = []

    if '2lss' in selection or '2l2tau' in selection:
        systlist += ['triggerUp','triggerDown']

    systlist += ['lepEff_mulooseUp','lepEff_mulooseDown',
                 'lepEff_ellooseUp','lepEff_ellooseDown',
                 'lepEff_mutightUp','lepEff_mutightDown',
                 'lepEff_eltightUp','lepEff_eltightDown']

    for btsys in dc.BTagSysts:
        systlist.append('btag_'+btsys)
    for thu in dc.ThSysts:
        systlist.append('thu_shape_'+thu)
    for frjt in dc.FakeTauSysts:  # ?
        systlist.append(frjt)

    hnamelist += [name+syst_label+syst for name in hnamelist for syst in systlist if not(syst in dc.FakeTauSysts and ('1l2tau' in selection or '2l2tau' in selection or 'gentau' in name))]
    wnamelist += ['event_weight_'+syst for name in hnamelist for syst in systlist if not(syst in dc.FakeTauSysts and ('1l2tau' in selection or '2l2tau' in selection or 'gentau' in name))]

    return hnamelist, wnamelist 

def getHistogramPromptContamination(variable, anatype, luminosity, xmin,
                                    xmax, nbins, eosPrefix='',
                                    evtweight='event_weight', treename='mva',
                                    transform=False, datasetlist_dict=None,
                                    verbosity=0, control=False):
    seltype = 'application_fake_'+anatype
    if control:
        seltype = 'control_fakeAR_'+anatype

    samples = []
    channels = ['TTW','TTWW','TTZ','EWK','Rares','TT','tH','VH']
    for ch in channels:
        for s in dc.SamplesInChannel2017[ch]:
            samples.append(s)

    if verbosity>2 and evtweight=='event_weight':
        print "prompt contamination in fake background:"

    histograms_MCfakeAR = []
    for sample in samples:
        h_fakeAR = getHistogramFromMCNtuple(variable, sample, seltype, luminosity,
                                            xmin, xmax, nbins, eosPrefix=eosPrefix,
                                            evtweight=evtweight, treename=treename,
                                            transform=transform,
                                            sample_suffix='_fakeAR',
                                            datasetlist_dict=datasetlist_dict,
                                            isPhotonConvs=False, applySF=True)
        if verbosity>3:
            print sample, ":", h_fakeAR.Integral()

        histograms_MCfakeAR.append(h_fakeAR)

        h_fakeAR_conv = getHistogramFromMCNtuple(variable, sample, seltype, luminosity,
                                            xmin, xmax, nbins, eosPrefix=eosPrefix,
                                            evtweight=evtweight, treename=treename,
                                            transform=transform,
                                            sample_suffix='_fakeAR',
                                            datasetlist_dict=datasetlist_dict,
                                            isPhotonConvs=True, applySF=True)

        if verbosity>3:
            print sample, "(conversion):", h_fakeAR_conv.Integral()

        histograms_MCfakeAR.append(h_fakeAR_conv)

    assert(len(histograms_MCfakeAR)>0)

    h_fakes_prompt = histograms_MCfakeAR[0].Clone(variable+"_fakes_prompt")
    h_fakes_prompt.SetDirectory(0)
    
    first=True  
    for hist in histograms_MCfakeAR:
        if first:
            first = False
            continue

        h_fakes_prompt.Add(hist)

    if verbosity>2 and evtweight=='event_weight':
        print "fakes_prompt :", h_fakes_prompt.Integral()

    return h_fakes_prompt  


if __name__ == "__main__":

    Selections=['signal_1l2tau', 'signal_2lss1tau', 'signal_3l1tau', 'signal_2l2tau',
                'control_1l2tau', 'control_2lss1tau', 'control_3l1tau',
                'control_2l2tau', 'control_ttW', 'control_ttZ', 'control_WZ']
    
    parser = argparse.ArgumentParser()
    parser.add_argument('selection', choices=Selections, help="Types of analysis")
    parser.add_argument('var', type=str, help="Variable to plot")
    parser.add_argument('xmin', type=float, help="lower limit of histograms")
    parser.add_argument('xmax', type=float, help="upper limit of histograms")
    parser.add_argument('-b','--nbins', type=int, default=100, help="Number of bins")
    parser.add_argument('-c','--channels', nargs='+', choices=dc.Channels,
                        default=['ttH','TTW','TTZ','EWK','Rares','fakes_data',
                                 'flips_data','data_obs'],
                        help="List of channels to make histograms")
    parser.add_argument('-o','--outname', type=str, default='./histograms.root')
    parser.add_argument('-l','--luminosity', type=float, default=1.,
                        help="Integrated luminosity (1/fb)")
    parser.add_argument('--treename', type=str, default="mva", help="Input tree name")
    parser.add_argument('--closureFile', type=str,
                        help="Auxiliary file for closure systematic uncertainty")
    parser.add_argument('-s','--systematics', action='store_true',
                        help="Include systematics")
    parser.add_argument('--sys_coname', type=str, default='_CMS_ttHl_',
                        help="Common string in histogram names for systematics")
    parser.add_argument('-v', '--verbose', action='count', #action='store_true',
                        help='Verbosity')
    parser.add_argument('-d','--datasetlist', type=str, default="DatasetList.csv")
    #parser.add_argument('-m','--genmatch', action='store_true')
    parser.add_argument('-p','--posbins', action='store_true',
                        help="make histogram bins positive")
    parser.add_argument('--genTauSplit', action='store_true',
                        help="split mc datacards further into _gentau and _faketau")
    parser.add_argument('--hDecaySplit', action='store_true',
                        help="split signal datacards further into higgs decay modes")
    parser.add_argument('--eostopdir', type=str,
                        default="/store/user/ztao/Condor/mvaNtuples/",
                        help="EOS directory to store mva ntuples")
    parser.add_argument('--version', type=str, default="may2018",
                        help="mva ntuple version")
    parser.add_argument('--transform', action='store_true',
                        help="Transform mva output")
    #parser.add_argument('--transfer_inputs', action='store_true',
    #                    help="Copy input files locally before processing")

    args = parser.parse_args()

    ######

    eosDirectoryString = 'root://cmsxrootd.fnal.gov/'+args.eostopdir+args.version+'/'
    
    histograms_var = []
    var = args.var

    # dataset list dictionary
    datalist_dict = getDatasetDict(args.datasetlist)

    print 
    if args.verbose>=1:
        print "Making histograms with variable :", var
    
    for channel in args.channels:

        if 'flip' in channel and '2lss' not in args.selection:
            continue
        
        print "======================================="
        print "processing channel:", channel
        print "---------------------------------------"
            
        if 'data' in channel:  # Collision data

            hnames, weightnames = getHistSuffixandWeightNames_data(channel, args.selection, args.systematics, syst_label=args.sys_coname)
            for hsuffix, wname in zip(hnames, weightnames):
                
                h = getHistogramFromDataNtuples(var, channel, args.selection,
                                                args.xmin, args.xmax, args.nbins,
                                                eosPrefix=eosDirectoryString,
                                                channel_suffix=hsuffix,
                                                evtweight=wname,
                                                treename=args.treename,
                                                makeBinPositive=args.posbins,
                                                transform=args.transform)
                
                if 'fakes_data' in h.GetName():
                    anatype = args.selection.split('_')[-1]
                    h_fakes_prompt = getHistogramPromptContamination(var, anatype, args.luminosity, args.xmin, args.xmax, args.nbins, eosPrefix=eosDirectoryString, evtweight=wname, treename=args.treename,transform=args.transform, datasetlist_dict=datalist_dict, verbosity=args.verbose, control="control" in args.selection)
                    h.Add(h_fakes_prompt, -1.)
                     
                histograms_var.append(h)

                if args.verbose>=1 and wname=='event_weight':#histname=='x_'+channel:
                    yields = h.Integral()
                    print channel, "\t", '%.5f'%yields
                    
                # Closure test
                if args.systematics and 'fakes_data' in h.GetName() and wname=='event_weight' and args.closureFile is not None and ('2lss1tau' in args.selection or '3l1tau' in args.selection):
                    for clos in dc.ClosureTests:
                        h_clos = dc.getClosureTestShape(h, clos, args.closureFile,
                                                        syst_coname=args.sys_coname)
                        histograms_var.append(h_clos)
                             
        else:  # MC
            histograms_mcch = []
            samples = getSampleListByChannel(channel)
            if args.verbose>=1:
                print '\tyields',
                if args.genTauSplit:
                    print '\tyields(gentau)\tyields(faketau)'
                else:
                    print
                    
            firstsample = True
            
            for sample in samples:

                histograms_sample = []
                    
                hnames, weightnames = getHistSuffixandWeightNames_mc(sample, args.selection, args.systematics, args.genTauSplit, args.hDecaySplit, syst_label=args.sys_coname)
                for hsuffix, wname in zip(hnames, weightnames):
                    #print hsuffix, wname
                    
                    h = getHistogramFromMCNtuple(var, sample, args.selection,
                                                 args.luminosity,
                                                 args.xmin, args.xmax, args.nbins,
                                                 eosPrefix=eosDirectoryString,
                                                 sample_suffix=hsuffix,
                                                 evtweight=wname,
                                                 treename=args.treename,
                                                 makeBinPositive=args.posbins,
                                                 transform=args.transform,
                                                 datasetlist_dict=datalist_dict,
                                                 isPhotonConvs='conv' in channel.lower(),
                                                 invertGenMatch='fakes_mc' in channel.lower())
                    histograms_sample.append(h)
                    #print h.Integral()

                if args.verbose>=2 and len(samples)>1:
                    dc.printYields(sample.lower(), histograms_sample, var,
                                   args.genTauSplit)
                        

                if firstsample:
                    histograms_mcch = [h.Clone(h.GetName().replace(sample.lower(),channel)) for h in histograms_sample]
                    #print histograms_mcch[0].GetName()
                else:
                    for hc, hs in zip(histograms_mcch, histograms_sample):
                        hc.Add(hs)

                firstsample = False
                if len(samples)>1:
                    histograms_var += histograms_sample   

            assert(histograms_mcch!=[])
            histograms_var += histograms_mcch

            if args.verbose>=1:
                if channel=='ttH' and args.hDecaySplit:
                    dc.printYields('ttH_htt', histograms_mcch, var, args.genTauSplit)
                    dc.printYields('ttH_hww', histograms_mcch, var, args.genTauSplit)
                    dc.printYields('ttH_hzz', histograms_mcch, var, args.genTauSplit)

                print "---------------------------------------"
                dc.printYields(channel, histograms_mcch, var, args.genTauSplit)

    #if args.verbose>=2:
    #    print "Done making histograms for variable", var

    # write datacards to file
    print "======================================="
    print
    outfile = TFile(args.outname, 'recreate')

    for histogram in histograms_var:
        # rename and write histograms to disk
        histname = histogram.GetName()
        if 'mvaOutput' in histname:
            histname = histname.replace(var,'x')
        histogram.SetName(histname)
        histogram.Write()

    print "Output file:", args.outname
    print

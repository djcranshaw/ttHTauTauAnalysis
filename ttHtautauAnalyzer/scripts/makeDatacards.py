#!/usr/bin/env python

from ROOT import TFile, TH1D
import ttHTauTauAnalysis.ttHtautauAnalyzer.datacards as dc
from ttHTauTauAnalysis.ttHtautauAnalyzer.cross_section import CrossSection
import copy
import argparse

syst_coname = '_CMS_ttHl_'
 
def getClosureTestShapes(h_nominal, infile,
                         hname_ele = 'x_TT_DL_FR_TT_MC_minus_FR_QCD_MC_ele',
                         hname_mu = 'x_TT_DL_FR_TT_MC_minus_FR_QCD_MC_mu'):
    
    assert(h_nominal.GetName()=='x_fakes_data')

    # get number of bins and x range from h_nominal
    nbin = h_nominal.GetNbinsX()
    xmin = h_nominal.GetBinLowEdge(1)
    xmax = h_nominal.GetBinLowEdge(nbin)+h_nominal.GetBinWidth(nbin)
    
    hists_clos = []

    # open file and get histograms
    f_clos = TFile(infile, 'read')
    h_ttbar_minus_qcd_fr_ele = f_clos.Get(hname_ele)
    h_ttbar_minus_qcd_fr_mu = f_clos.Get(hname_mu)

    h_clos_e_shape_up = TH1D("x_fakes_data_"+sys_coname+"Clos_e_shapeUp", '',
                             nbin, xmin, xmax)
    h_clos_e_shape_down = TH1D("x_fakes_data_"+sys_coname+"Clos_e_shapeDown", '',
                               nbin, xmin, xmax)
    h_clos_m_shape_up = TH1D("x_fakes_data_"+sys_coname+"Clos_m_shapeUp", '',
                             nbin, xmin, xmax)
    h_clos_m_shape_down = TH1D("x_fakes_data_"+sys_coname+"Clos_m_shapeDown", '',
                               nbin, xmin, xmax)

    h_clos_e_shape_up.Sumw2()
    h_clos_e_shape_down.Sumw2()
    h_clos_m_shape_up.Sumw2()
    h_clos_m_shape_down.Sumw2()

    h_clos_e_shape_up.Add(h_nominal, h_ttbar_minus_qcd_fr_ele, 1, 1)
    h_clos_e_shape_down.Add(h_nominal, h_ttbar_minus_qcd_fr_ele, 1, -1)
    h_clos_m_shape_up.Add(h_nominal, h_ttbar_minus_qcd_fr_mu, 1, 1)
    h_clos_m_shape_down.Add(h_nominal, h_ttbar_minus_qcd_fr_mu, 1, -1)

    return [h_clos_e_shape_up, h_clos_e_shape_down, h_clos_m_shape_up,
            h_clos_m_shape_down]
        

def getShapesfromSample_mc(anaType, channel, sample, tree_name, nbin, xmin, xmax,
                           ntuplelist, binningMap, lumi, addSyst=True, correction=None):
    cards = []
    
    # setup
    namelist = [sample, sample+'_gentau', sample+'_faketau']
    if sample=='ttH':
        namelist += [sample+'_htt',sample+'_htt_gentau',sample+'_htt_faketau',
                     sample+'_hww',sample+'_hww_gentau',sample+'_hww_faketau',
                     sample+'_hzz',sample+'_hzz_gentau',sample+'_hzz_faketau']

    systlist = []
    if correction is None:
        if addSyst:
            for btsys in dc.BTagSysts:
                systlist.append('btag_'+btsys)
            for thu in dc.ThSysts:
                systlist.append('thu_shape_'+thu)
            for frjt in dc.FakeTauSysts:
                systlist.append(frjt)
                
    else:
        assert(correction in ['JESUp','JESDown','TESUp','TESDown'])
        systlist = [correction]
        
    # open ntuple and get tree
    infile = dc.getNtupleFileName_mc(ntuplelist, anaType, sample, correction)
    f = TFile(infile,'read')
    tree = f.Get(tree_name)
    
    inverseSumGenWeight = tree.GetWeight()
    xsection = CrossSection[sample]

    # make data cards
    # change sample name to all lower case first
    namelist = [name.lower() for name in namelist]
    for name in namelist:

        if correction is None:
            histname = 'x_'+name
            h = dc.getShapeFromTree(tree, histname, nbin, xmin, xmax, binningMap)
            # scale histogram
            h.Scale(lumi*xsection*inverseSumGenWeight)
            dc.makeBinContentsPositive(h, False)
            cards.append(h)
            
        for syst in systlist:
            if syst in dc.FakeTauSysts and (args.anaType=='1l2tau' or 'gentau' in name):
                continue
            histname = 'x_'+name+syst_coname+syst
            h = dc.getShapeFromTree(tree, histname, nbin, xmin, xmax, binningMap)
            # scale histogram
            h.Scale(lumi*xsection*inverseSumGenWeight)
            dc.makeBinContentsPositive(h, False)
            cards.append(h)

    cardscopy = copy.deepcopy(cards) # better way than doing this?
            
    return cardscopy
    

def makeDatacards_mc(anaType, channel, treename, nbin, xmin, xmax, ntuplelist,
                     binningMap, lumi, addSystematics):
    
    datacards = []
    datacards_channel = []

    samples = dc.SamplesInChannel[channel]

    for sample in samples:
        datacards_sample = []
        
        shapes = getShapesfromSample_mc(anaType, channel, sample, treename,
                                        nbin, xmin, xmax, ntuplelist, binningMap,
                                        lumi, addSystematics, correction=None)  
        datacards_sample += shapes
        print "datacards_sample : ",datacards_sample
        
        if addSystematics:
            shape_jesup = getShapesfromSample_mc(anaType, channel, sample, treename, nbin, xmin, xmax, ntuplelist, binningMap, lumi, addSyst=False, correction='JESUp')
            shape_jesdo = getShapesfromSample_mc(anaType, channel, sample, treename, nbin, xmin, xmax, ntuplelist, binningMap, lumi, addSyst=False, correction='JESDown')
            shape_tesup = getShapesfromSample_mc(anaType, channel, sample, treename, nbin, xmin, xmax, ntuplelist, binningMap, lumi, addSyst=False, correction='TESUp')
            shape_tesdo = getShapesfromSample_mc(anaType, channel, sample, treename, nbin, xmin, xmax, ntuplelist, binningMap, lumi, addSyst=False, correction='TESDown')
            datacards_sample += shape_jesup
            datacards_sample += shape_jesdo
            datacards_sample += shape_tesup
            datacards_sample += shape_tesdo

        # add datacards 
        if datacards_channel==[]:
            datacards_channel = copy.deepcopy(datacards_sample)
            # rename histograms
            for h in datacards_channel:
                hname = h.GetName()
                h.SetName(hname.replace(sample.lower(), channel))
        else:
            for card_channel, card_sample in zip(datacards_channel,datacards_sample):
                card_channel.Add(card_sample)

        print "datacards_channel : ", datacards_channel
        datacards += datacards_sample
        print "datacards : ", datacards
                
    datacards += datacards_channel
    print "datacards outofloop : ", datacards

    datacards_copy = copy.deepcopy(datacards) # better way?
    
    return datacards_copy

            
def makeDatacards_data(anaType, channel, treename, nbin, xmin, xmax, ntuplelist,
                       binningMap, addSystematics):
    
    datacards = []
    samples = dc.SamplesInChannel[channel]
    
    # open root file and get tree for each of the samples
    inputfiles = [TFile(dc.getNtupleFileName_data(ntuplelist, anaType, channel, sample),'read') for sample in samples]
    trees = [fin.Get(treename) for fin in inputfiles]
        
    histname = 'x_'+channel
    
    shape = dc.getShapeFromMergingTrees(trees, histname, nbin, xmin, xmax, binningMap)
    
    # make bin contents positive
    dc.makeBinContentsPositive(shape, False)
    datacards.append(shape)

    # systematics
    if 'fakes' in channel and addSystematics:
        # fake rate systematics
        systlist = []
        if args.anaType=='1l2tau':
            for frjt in dc.FakeTauSysts:
                systlist.append(frjt)
            # lepton fake rate systematics?
        elif args.anaType=='2lss1tau' or args.anaType=='3l1tau':
            for frl in dc.FakeRateLepSysts:
                systlist.append(frl)

        for syst in systlist:
            hname = 'x_'+channel+syst_coname+syst
            h = dc.getShapeFromMergingTrees(trees, hname, nbin, xmin,xmax, binningMap)
            dc.makeBinContentsPositive(h, False)
            datacards.append(h)
            
        # closure test systematics of the lepton fake rate
        if args.anaType=='2lss1tau' or args.anaType=='3l1tau':
            datacards += getClosureTestShapes(shape)

    datacards_copy = copy.deepcopy(datacards) # better way?
            
    return datacards_copy


if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument('anaType', choices=['1l2tau','2lss1tau','3l1tau'],
                        help="Analysis type")
    parser.add_argument('channels', nargs='+',choices=['ttH','TTW','TTZ','EWK','Rares','fakes_data','flips_data','data_obs'],
                        help="List of channels to make data cards")
    parser.add_argument('nbin', type=int, help="number of bins")
    parser.add_argument('--xmin', type=float)
    parser.add_argument('--xmax', type=float)
    parser.add_argument('-n','--ntuplelist', type=str, default='mvaNtuplelist.txt')
    parser.add_argument('-b','--binmap', type=str, default='binning.root',
                        help="Binning Map from 2D to 1D. Default histogram name: 'hTargetBinning'")
    parser.add_argument('-o','--outname', type=str, default='./datacards.root',
                        help="Output file name")
    parser.add_argument('-l', '--luminosity', type=float, default=1.,
                        help="Integrated luminosity")
    parser.add_argument('--treename', type=str, default="mva",
                        help="Name of tree")
    parser.add_argument('-s','--systematics', action='store_true',
                        help="Include systematics")
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='verbosity')
    
    args = parser.parse_args()

    # get binning Map 2D histogram
    fMap = TFile(args.binmap)
    hBinningMap = fMap.Get("hTargetBinning")

    # determine xmin and xmax if not specified
    xMin = 0. if args.xmin is None else args.xmin
    xMax = float(args.nbin) if args.xmax is None else args.xmax
    
    datacards = []
    
    for channel in args.channels:
        
        print "======================================="
        print "processing channel:", channel
        print "---------------------------------------"
        
        if 'data' in channel: # Collision data
            datacards += makeDatacards_data(args.anaType, channel, args.treename,
                                            args.nbin, xMin, xMax,
                                            args.ntuplelist, hBinningMap,
                                            args.systematics)
        else: # Monte Carlo
            datacards += makeDatacards_mc(args.anaType, channel, args.treename,
                                          args.nbin, xMin, xMax,
                                          args.ntuplelist, hBinningMap,
                                          args.luminosity, args.systematics)

    # write datacards to file
    outfile = TFile(args.outname, 'recreate')

    for datacard in datacards:
        datacard.Write()

    print "Output file:", args.outname

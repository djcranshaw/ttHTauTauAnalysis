#!/usr/bin/env python

from ROOT import TFile, TH1D, TTree, gROOT, TF1, TFitResultPtr, Double
from ROOT import TRatioPlot, TCanvas, TLegend, TPaveText
from ttHTauTauAnalysis.ttHtautauAnalyzer.crab_utils import getDatasetDict
from ttHTauTauAnalysis.ttHtautauAnalyzer.datacards import getBinEdges
import argparse
gROOT.SetBatch(True)

parser = argparse.ArgumentParser()
parser.add_argument('analysis', choices=['1l2tau','2lss1tau','3l1tau','2l2tau'])
parser.add_argument('-s','--samples', nargs='+', choices=['TTToDiLep_psw','TTToSemiLep_psw'], default=['TTToDiLep_psw','TTToSemiLep_psw'])
parser.add_argument('-l','--luminosity', type=float, default=41.53,
                    help="Integrated luminosity in fb^-1")
parser.add_argument('--variable', type=str, default="mvaOutput",
                    help="Variable to make the histograms")
parser.add_argument('-o','--output', type=str, default="Closure_FR_syst.root")
parser.add_argument('-d','--datasetlist', type=str)#, default="DatasetList.csv")
parser.add_argument('--treename', type=str, default="mva", help="Input tree name")
parser.add_argument('--prefix', type=str, default="mvaNtuple_",
                    help="Common prefix of mva ntuple names")
parser.add_argument('--version', type=str, default="jun2018v2",
                    help="mvaNtuple version")
parser.add_argument('--eostopdir', type=str,
                    default="/store/user/ztao/Condor/mvaNtuples/",
                    help="EOS directory where mva ntuples are stored")
parser.add_argument('-p','--plot', action='store_true')
parser.add_argument('--nbin', type=int, help="Number of bins")
parser.add_argument('--xmin', type=float)
parser.add_argument('--xmax', type=float)

args = parser.parse_args()

def getShapeFromTree_SR(tree, histname, anatype, clos_type='clos'):
    # non-prompt/fakes contributions in the signal region

    xbins = getBinEdges(anatype) 
    nbin = len(xbins)-1
    h = TH1D(histname, "", nbin, xbins) if args.nbin is None else TH1D(histname, "", args.nbin, args.xmin, args.xmax)
    h.Sumw2()

    for ev in tree:
        # select events that are NOT matched to generator level prompt objects
        # for the purpose of estimating fakes in the SR

        # double check the selected objects all pass tight selection
        if anatype == '1l2tau':
            assert(ev.lep1_isTight and ev.tau1_isTight and ev.tau2_isTight)
        elif anatype == '2lss1tau':
            assert(ev.lep1_isTight and ev.lep2_isTight and ev.tau1_isTight)
        elif anatype == '3l1tau':
            assert(ev.lep1_isTight and ev.lep2_isTight and ev.lep3_isTight and ev.tau1_isTight)
        elif anatype == '2l2tau':
            assert(ev.lep1_isTight and ev.lep2_isTight and ev.tau1_isTight and ev.tau2_isTight)
        
        isfake = False
        
        if clos_type.lower()=='clos':
            if anatype in ['2lss1tau', '3l1tau']:
                # only e and mu are used in the FF method
                isfake = not ev.isGenMatchedLep
            elif anatype in ['1l2tau', '2l2tau']:
                # e, mu and tau are used in the FF method
                isfake = not (ev.isGenMatchedTau and ev.isGenMatchedLep)
            else:
                raise ValueError("Unknown analysis type "+anatype)

        elif clos_type.lower()=='clos_e': # contribution from electrons only
            if anatype == '2lss1tau' or anatype == '2l2tau':
                # only consider electrons
                lep1isFakeEle = abs(ev.lep1_pdgId)==11 and (not ev.lep1_isGenMatched)
                lep2isFakeEle = abs(ev.lep2_pdgId)==11 and (not ev.lep2_isGenMatched)
                isfake = lep1isFakeEle or lep2isFakeEle
            elif anatype == '3l1tau':
                lep1isFakeEle = abs(ev.lep1_pdgId)==11 and (not ev.lep1_isGenMatched)
                lep2isFakeEle = abs(ev.lep2_pdgId)==11 and (not ev.lep2_isGenMatched)
                lep3isFakeEle = abs(ev.lep3_pdgId)==11 and (not ev.lep3_isGenMatched)
                isfake = lep1isFakeEle or lep2isFakeEle or lep3isFakeEle
            elif anatype == '1l2tau':
                isfake = abs(ev.lep1_pdgId)==11 and (not ev.lep1_isGenMatched)
            else:
                raise ValueError("Unknown analysis type "+anatype)

        elif clos_type.lower()=='clos_m': # contribution from muons only
            if anatype == '2lss1tau' or anatype == '2l2tau':
                # only consider electrons
                lep1isFakeMu = abs(ev.lep1_pdgId)==13 and (not ev.lep1_isGenMatched)
                lep2isFakeMu = abs(ev.lep2_pdgId)==13 and (not ev.lep2_isGenMatched)
                isfake = lep1isFakeMu or lep2isFakeMu
            elif anatype == '3l1tau':
                lep1isFakeMu = abs(ev.lep1_pdgId)==13 and (not ev.lep1_isGenMatched)
                lep2isFakeMu = abs(ev.lep2_pdgId)==13 and (not ev.lep2_isGenMatched)
                lep3isFakeMu = abs(ev.lep3_pdgId)==13 and (not ev.lep3_isGenMatched)
                isfake = lep1isFakeMu or lep2isFakeMu or lep3isFakeMu
            elif anatype == '1l2tau':
                isfake = abs(ev.lep1_pdgId)==13 and (not ev.lep1_isGenMatched)
            else:
                raise ValueError("Unknown analysis type "+anatype)
                
        elif clos_type.lower()=='clos_t': # contribution from taus only
            assert(anatype in ['1l2tau', '2l2tau'])
            isFakeTau1 = not ev.tau1_isGenMatched
            isFakeTau2 = not ev.tau2_isGenMatched
            isfake = isFakeTau1 or isFakeTau2
            # require gen matched lepton?

        else:
            raise ValueError("Unknown closure test "+clos_type)


        if not isfake:
            continue

        value = ev.GetLeaf(args.variable).GetValue()
        weight = ev.GetLeaf("event_weight").GetValue()

        h.Fill(value, weight)

    return h
    

def getShapeFromTree_FakeAR(tree, event_weight, histname, anatype, clos_type='clos'):
    # application region
    
    xbins = getBinEdges(anatype) if args.nbin is None else [(args.xmin+i*float(args.xmax-args.xmin)/args.nbin) for i in xrange(0, args.nbin+1)]
    nbin = len(xbins)-1
    h = TH1D(histname, "", nbin, xbins) if args.nbin is None else TH1D(histname, "", args.nbin, args.xmin, args.xmax)
    h.Sumw2()

    for ev in tree:

        # TODO: double check it is fake application region
        
        # additional selections if necessary
        pass_selection = True

        # TODO: subtract prompt contamination
        # need to store Fake Factor of each fakeable objects in the ntuple
        # and recompute the weights of events in application region that are
        # gen matched. Then subtract these from the total estimation
        
        if clos_type.lower() == 'clos_t': # extrapolation from tau fake rate only
            assert(anatype in ['1l2tau', '2l2tau'])
            # only use events with tight lepton(s)
            pass_selection = ev.lep1_isTight
            if anatype == '2l2tau':
                pass_selection = pass_selection and ev.lep2_isTight

        elif clos_type.lower() == 'clos_e': # from electron fake rate only
            if anatype == '2lss1tau':
                assert(ev.tau1_isTight)
                lep1IsFakeableMu = abs(ev.lep1_pdgId)==13 and (not ev.lep1_isTight)
                lep2IsFakeableMu = abs(ev.lep2_pdgId)==13 and (not ev.lep2_isTight)
                # no fakeable muons, i.e. only electron fake rate is applied
                pass_selection =  not (lep1IsFakeableMu or lep2IsFakeableMu)
            elif anatype == '3l1tau':
                assert(ev.tau1_isTight)
                lep1IsFakeableMu = abs(ev.lep1_pdgId)==13 and (not ev.lep1_isTight)
                lep2IsFakeableMu = abs(ev.lep2_pdgId)==13 and (not ev.lep2_isTight)
                lep3IsFakeableMu = abs(ev.lep3_pdgId)==13 and (not ev.lep3_isTight)
                # no fakeable muons, i.e. only electron fake rate is applied
                pass_selection = not (lep1IsFakeableMu or lep2IsFakeableMu or lep3IsFakeableMu)
            elif anatype == '1l2tau':
                lep1IsFakeableEle = abs(ev.lep1_pdgId)==11 and (not ev.lep1_isTight)
                tausAreTight = ev.tau1_isTight and ev.tau2_isTight
                pass_selection = lep1IsFakeableEle and tausAreTight
            elif anatype == '2l2tau':
                lep1IsFakeableMu = abs(ev.lep1_pdgId)==13 and (not ev.lep1_isTight)
                lep2IsFakeableMu = abs(ev.lep2_pdgId)==13 and (not ev.lep2_isTight)
                tausAreTight = ev.tau1_isTight and ev.tau2_isTight
                # no fakeable muons and all taus are tight
                pass_selection = not (lep1IsFakeableMu or lep2IsFakeableMu) and tausAreTight
            else:
                raise ValueError("Unknown analysis type "+anatype)
            
        elif clos_type.lower() == 'clos_m': # from muon fake rate only
            if anatype == '2lss1tau':
                assert(ev.tau1_isTight)
                lep1IsFakeableEle = abs(ev.lep1_pdgId)==11 and (not ev.lep1_isTight)
                lep2IsFakeableEle = abs(ev.lep2_pdgId)==11 and (not ev.lep2_isTight)
                # no fakeable electrons, i.e. only muon fake rate is applied
                pass_selection =  not (lep1IsFakeableEle or lep2IsFakeableEle)
            elif anatype == '3l1tau':
                assert(ev.tau1_isTight)
                lep1IsFakeableEle = abs(ev.lep1_pdgId)==11 and (not ev.lep1_isTight)
                lep2IsFakeableEle = abs(ev.lep2_pdgId)==11 and (not ev.lep2_isTight)
                lep3IsFakeableEle = abs(ev.lep3_pdgId)==11 and (not ev.lep3_isTight)
                # no fakeable electrons, i.e. only muon fake rate is applied
                pass_selection = not (lep1IsFakeableEle or lep2IsFakeableEle or lep3IsFakeableEle)
            elif anatype == '1l2tau':
                lep1IsFakeableMu = abs(ev.lep1_pdgId)==13 and (not ev.lep1_isTight)
                tausAreTight = ev.tau1_isTight and ev.tau2_isTight
                pass_selection = lep1IsFakeableMu and tausAreTight
            elif anatype == '2l2tau':
                lep1IsFakeableEle = abs(ev.lep1_pdgId)==11 and (not ev.lep1_isTight)
                lep2IsFakeableEle = abs(ev.lep2_pdgId)==11 and (not ev.lep2_isTight)
                tausAreTight = ev.tau1_isTight and ev.tau2_isTight
                # no fakeable muons and all taus are tight
                pass_selection = not (lep1IsFakeableEle or lep2IsFakeableEle) and tausAreTight
            else:
                raise ValueError("Unknown analysis type "+anatype)
            
        if not pass_selection:
            continue
            
        value = ev.GetLeaf(args.variable).GetValue()
        weight = ev.GetLeaf(event_weight).GetValue()

        for sf in ['pu_weight','mc_weight','btag_sf','lepid_sf','tauid_sf','hlt_sf']:
            assert(ev.GetLeaf(sf).GetValue() > -9998.)
            weight *= ev.GetLeaf(sf).GetValue()

        h.Fill(value, weight)

    return h

def plotClosure(outname, hist_fakes_mc, hist_fakes_fr, fitfunc):

    canvas = TCanvas()

    hist_fakes_mc.SetLineColor(1)
    hist_fakes_mc.SetMarkerColor(1)
    hist_fakes_mc.SetMarkerStyle(25)
    hist_fakes_mc.SetMarkerSize(1)

    hist_fakes_fr.SetLineColor(4)
    hist_fakes_fr.SetMarkerColor(4)
    hist_fakes_fr.SetMarkerStyle(25)
    hist_fakes_fr.SetMarkerSize(1)

    ymax = max(hist_fakes_mc.GetMaximum(), hist_fakes_fr.GetMaximum())
    ymax *= 2.0

    hist_fakes_mc.SetMaximum(ymax)
    hist_fakes_mc.SetStats(0)
    hist_fakes_mc.GetXaxis().SetTitle(args.variable)
    
    rp = TRatioPlot(hist_fakes_mc, hist_fakes_fr, "divsym")
    rp.SetH1DrawOpt("e1p")
    rp.SetH2DrawOpt("e1p")
    rp.Draw()

    #ymax_bot = max(1.50, (fitfunc.GetMaximum(0,1)-1.)*1.2+1)
    #ymin_bot = min(0.50, (fitfunc.GetMinimum(0,1)-1)*1.2+1)
    
    rp.GetUpperRefYaxis().SetTitle("Events")
    rp.GetLowerRefYaxis().SetTitle("Fakes_MC/TT_FR")
    rp.GetLowerRefYaxis().CenterTitle()
    #rp.GetLowerRefGraph().SetMinimum(ymin_bot)
    #rp.GetLowerRefGraph().SetMaximum(ymax_bot)
    rp.GetLowYaxis().SetNdivisions(505)

    leg = TLegend(0.40,0.80,0.61,0.90)
    leg.AddEntry(hist_fakes_mc, "Fakes_MC","p")
    leg.AddEntry(hist_fakes_fr, "TT(FR)", "p")
    leg.AddEntry(fitfunc, "Fit to ratio", "l")

    leg.Draw("same")

    textbox = TPaveText(0.65,0.68,0.86,0.78,"NDC")
    textbox.AddText("Fit to ratio")
    textbox.AddText("Slope: "+"%.3f"%fitfunc.GetParameter(0)+" +/- "+"%.3f"%fitfunc.GetParError(0))
    textbox.AddText("Offset: "+"%.3f"%fitfunc.GetParameter(1)+" +/- "+"%.3f"%fitfunc.GetParError(1))
    textbox.Draw("same")

    textbox2 = TPaveText(0.65,0.80,0.86,0.90,"NDC")
    textbox2.AddText(str(args.luminosity)+" fb^{-1} (13 TeV)")
    binx1 = hist_fakes_mc.GetXaxis().GetFirst()
    binx2 = hist_fakes_mc.GetXaxis().GetLast()
    err_mc = Double(0.)
    yields_mc = hist_fakes_mc.IntegralAndError(binx1, binx2, err_mc)
    textbox2.AddText("Fakes_MC: "+"%.2f"%yields_mc+" +/- "+"%.2f"%err_mc)
    err_fr = Double(0.)
    yields_fr = hist_fakes_fr.IntegralAndError(binx1, binx2, err_fr)
    textbox2.AddText("TT(FR): "+"%.2f"%yields_fr+" +/- "+"%.2f"%err_fr)
    textbox2.Draw("same")
    
    rp.GetLowerPad().cd()

    fitfunc.Draw("same")

    canvas.SaveAs(outname)

##################
eosDirectoryString = 'root://cmsxrootd.fnal.gov/'+args.eostopdir+args.version+'/'

# dataset list dictionary
datalist_dict = getDatasetDict(args.datasetlist)

clos_types = ['clos']
if args.analysis in ['1l2tau', '2l2tau']:
    clos_types += ['clos_t','clos_e','clos_m']
elif args.analysis in ['2lss1tau', '3l1tau']:
    clos_types += ['clos_e', 'clos_m']

firstsample=True
hist_tt_mc = []
hist_tt_fr = []
hist_allsample_mc = []
hist_allsample_fr = []

for sample in args.samples:

    # get mva ntuple
    # SR
    ntuplefilename_SR = eosDirectoryString+args.prefix+sample+'_signal_'+args.analysis + '.root'
    print "Open file :", ntuplefilename_SR
    infile_sr = TFile.Open(ntuplefilename_SR)
    tree_sr = infile_sr.Get(args.treename)

    # for scaling histograms
    lumi = args.luminosity*1000  # convert from 1/fb to 1/pb
    SumGenWeight = tree_sr.GetUserInfo().FindObject('SumGenWeight').GetVal()
    xsection = tree_sr.GetUserInfo().FindObject('xsection').GetVal() if datalist_dict is None else float(datalist_dict[sample]['xsection'])
    
    hist_sample_mc = []
    for clos in clos_types:
        htmp = getShapeFromTree_SR(tree_sr, "x_"+sample+"_mc_"+clos, args.analysis,
                                   clos)
        htmp.SetDirectory(0)
        htmp.Scale(lumi * xsection / SumGenWeight)      
        hist_sample_mc.append(htmp)
        
    # get mva ntuple
    # Fake AR
    ntuplefilename_fakeAR = eosDirectoryString+args.prefix+sample+'_application_fake_'+args.analysis + '.root'
    print "Open file :", ntuplefilename_fakeAR
    infile_ar = TFile.Open(ntuplefilename_fakeAR)
    tree_ar = infile_ar.Get(args.treename)

    evweight="event_weight"
    #evweight='event_weight_FR_TT' # default
    #if args.analysis in ['2lss1tau', '3l1tau']:
    #    evweight = 'event_weight_FR_QCD'
    
    hist_sample_fr = []
    for clos in clos_types:
        htmp = getShapeFromTree_FakeAR(tree_ar, evweight, "x_"+sample+"_fr_"+clos,
                                       args.analysis, clos)
        htmp.SetDirectory(0)
        htmp.Scale(lumi * xsection / SumGenWeight)
        hist_sample_fr.append(htmp)

    if firstsample:
        hist_tt_mc = [h.Clone(h.GetName().replace(sample,'TT')) for h in hist_sample_mc]
        hist_tt_fr = [h.Clone(h.GetName().replace(sample,'TT')) for h in hist_sample_fr]
        for hmc, hfr in zip(hist_tt_mc, hist_tt_fr):
            hmc.SetDirectory(0)
            hfr.SetDirectory(0)
    else:
        for htmc, hsmc in zip(hist_tt_mc, hist_sample_mc):
            htmc.Add(hsmc)
        for htfr, hsfr in zip(hist_tt_fr, hist_sample_fr):
            htfr.Add(hsfr)

    hist_allsample_mc += hist_sample_mc
    hist_allsample_fr += hist_sample_fr
        
    firstsample=False

# ratio, diff histograms
hist_tt_ratio = []
hist_tt_diff = []
tf1_fit = []
for h_mc, h_fr in zip(hist_tt_mc, hist_tt_fr):
    h_diff = h_mc.Clone(h_mc.GetName().replace('mc','diff'))
    h_diff.Add(h_fr, -1.)
    hist_tt_diff.append(h_diff)
    
    h_ratio = h_mc.Clone(h_mc.GetName().replace('mc','ratio'))
    h_ratio.Divide(h_fr)
    hist_tt_ratio.append(h_ratio)

    # fit ratio with straight line
    fitname = h_ratio.GetName().replace('x_TT','fit')
    fitfunc = TF1(fitname,"[0]*x+[1]", 0, 1)
    fitresult = h_ratio.Fit(fitfunc, "WLS0")
    # get fit results
    fit_nominal = h_ratio.GetFunction(fitname)
    k = fitresult.Parameter(0) #fit_nominal.GetParameter(0)
    kerr = fitresult.ParError(0) #fit_nominal.GetParError(0)
    m = fitresult.Parameter(1) #fit_nominal.GetParameter(1)
    merr = fitresult.ParError(1) #fit_nominal.GetParError(1)

    # uncertainties
    # TODO: uncorrelate two parameters
    fit_shapeup = TF1(fitname+"_shapeUp","[0]*x+[1]", 0, 1)
    fit_shapeup.SetParameter(0, k+kerr)
    fit_shapeup.SetParameter(1, m)
    fit_shapedo = TF1(fitname+"_shapeDown","[0]*x+[1]", 0, 1)
    fit_shapedo.SetParameter(0, k-kerr)
    fit_shapedo.SetParameter(1, m)
    fit_normup = TF1(fitname+"_normUp","[0]*x+[1]", 0, 1)
    fit_normup.SetParameter(0, k)
    fit_normup.SetParameter(1, m+merr)
    fit_normdo = TF1(fitname+"_normDown","[0]*x+[1]", 0, 1)
    fit_normdo.SetParameter(0, k)
    fit_normdo.SetParameter(1, m-merr)
    
    tf1_fit.append(fit_nominal)
    tf1_fit.append(fit_shapeup)
    tf1_fit.append(fit_shapedo)
    tf1_fit.append(fit_normup)
    tf1_fit.append(fit_normdo)

    # plot
    if args.plot:     
        clostype = h_ratio.GetName().replace('x_TT_ratio_','')
        pname = args.output.replace('.root','_'+clostype+'.pdf')
        plotClosure(pname, h_mc, h_fr, fit_nominal)
    

outfile = TFile(args.output, 'recreate')

for h_mc, h_fr in zip(hist_tt_mc, hist_tt_fr):
    h_mc.Write()
    h_fr.Write()
for hdiff in hist_tt_diff:
    hdiff.Write()
for hratio in hist_tt_ratio:
    hratio.Write()
for fit in tf1_fit:
    fit.Write()
    

print "Output file :", args.output

#ifndef eventNtuple_cc
#define eventNtuple_cc

#include "../interface/eventNtuple.h"

std::vector<miniLepton> eventNtuple::buildLeptons(bool loose) const
{
	char wp = loose ? 'L' : 'F';
	return buildLeptons(wp);
}

std::vector<miniLepton> eventNtuple::buildLeptons(char WP) const
{
	assert(WP=='L' or WP=='F' or WP=='T');
	
	std::vector<miniLepton> leptons;

	for (unsigned int l = 0; l<mu_pt->size(); ++l) {
		
		bool passID = (WP=='L');
		if (WP=='F') passID = mu_isfakeablesel->at(l);
		if (WP=='T') passID = mu_ismvasel->at(l);
		if (not passID) continue;
		
		TLorentzVector mu;
		mu.SetPtEtaPhiE(mu_pt->at(l),mu_eta->at(l),mu_phi->at(l),mu_E->at(l));
		miniLepton lep(mu, mu_conept->at(l), -13*mu_charge->at(l),
					   mu_charge->at(l), true, mu_isfakeablesel->at(l),
					   mu_ismvasel->at(l), mu_istightcharge->at(l));
		if (mu_mcMatchType->size()>0) lep.set_MCMatchType(mu_mcMatchType->at(l));
		leptons.push_back(lep);
	}

	for (unsigned int l = 0; l<ele_pt->size(); ++l) {

		bool passID = (WP=='L');
		if (WP=='F') passID = ele_isfakeablesel->at(l);
		if (WP=='T') passID = ele_ismvasel->at(l);
		if (not passID) continue;

		TLorentzVector ele;
		ele.SetPtEtaPhiE(ele_pt->at(l),ele_eta->at(l),ele_phi->at(l),ele_E->at(l));
		miniLepton lep(ele, ele_conept->at(l), -11*ele_charge->at(l),
					   ele_charge->at(l), true, ele_isfakeablesel->at(l),
					   ele_ismvasel->at(l), ele_istightcharge->at(l));
		if (ele_mcMatchType->size()>0) lep.set_MCMatchType(ele_mcMatchType->at(l));
		if (ele_isPromptGenPhotonMatched->size()>0)
			lep.set_isGenPhotonMatched(ele_isPromptGenPhotonMatched->at(l));
		//if (ele_isGenPhotonMatched)
		//	lep.set_isGenPhotonMatched(ele_isGenPhotonMatched->at(l));
		leptons.push_back(lep);
	}
	// sort by conept
	sort(leptons.begin(), leptons.end(), [] (miniLepton l1, miniLepton l2)
		 {return l1.conept()>l2.conept();} );

	return leptons;
}

std::vector<TLorentzVector> eventNtuple::buildFourVectorLeps(bool loose) const
{
	auto leptons = buildLeptons(loose);
	
	std::vector<TLorentzVector> lepsP4;
	for (const auto & lep : leptons)
		lepsP4.push_back(lep.p4());

	return lepsP4;
}

/*std::vector<miniTau> eventNtuple::buildTaus(std::string WP) const
{
	// explicitly selecting taus passing WP
	// passTightSel() is meaningless for the tau collection returned by this method
	return buildTaus(false, WP);
}
*/

std::vector<miniTau> eventNtuple::buildTaus(bool loose, Analysis_types anatype,
											const TString& tec, bool isdata,
											float minPt) const
{
	std::string selTauWP;
	if (anatype==Analyze_1l2tau or anatype==Analyze_2l2tau)
		selTauWP = "M";
	else
		selTauWP = "L";

	return buildTaus(loose, selTauWP, tec, isdata, minPt);
}

std::vector<miniTau> eventNtuple::buildTaus(bool loose, std::string tightWPDef,
											const TString& tec, bool isdata,
											float minPt) const
{
	std::vector<miniTau> taus;
		
	for (unsigned int t = 0; t < tau_pt->size(); ++t) {
		
		TLorentzVector tauP4;
		tauP4.SetPtEtaPhiE(tau_pt->at(t),tau_eta->at(t),tau_phi->at(t),tau_E->at(t));
		
		// tauES and uncertainties
		// https://twiki.cern.ch/twiki/bin/view/CMS/TauIDRecommendation13TeV#Tau_energy_scale
		// AN-18-098 Table 14
		if (not isdata) {
			float tauES_sf = 1.;
			if ( tau_mcMatchType->at(t)>=1 and tau_mcMatchType->at(t)<=5 ) {
				// gen matched tau only ?
				if (tau_decayMode->at(t)==0) tauES_sf = 0.97;
				else if (tau_decayMode->at(t)==1) tauES_sf = 0.98;
				else if (tau_decayMode->at(t)==10) tauES_sf = 0.99;
			}

			float unc = 0.03;
			float shift = 0.;
			if (tec.EqualTo("tesup", TString::ECaseCompare::kIgnoreCase))
				shift = 1.;
			else if (tec.EqualTo("tesdown", TString::ECaseCompare::kIgnoreCase))
				shift = -1.;

			tauP4 *= (tauES_sf+shift*unc);
		}
		
		if (tauP4.Pt() < minPt) continue;
		
		miniTau tau(tauP4,tau_charge->at(t),tau_decayMode->at(t),
					tau_idPreselection->at(t), tau_idSelection->at(t));
		if (tau_mcMatchType->size()>0)
			tau.set_MCMatchType(tau_mcMatchType->at(t));
		
		tau.set_tauIDWPindex(tau_byVLooseIsolationMVArun2v1DBdR03oldDMwLT->at(t)>0,
							 tau_byLooseIsolationMVArun2v1DBdR03oldDMwLT->at(t)>0,
							 tau_byMediumIsolationMVArun2v1DBdR03oldDMwLT->at(t)>0,
							 tau_byTightIsolationMVArun2v1DBdR03oldDMwLT->at(t)>0,
							 tau_byVTightIsolationMVArun2v1DBdR03oldDMwLT->at(t)>0);

		//Update istight_ according to tight WP definition (based on on analysis type)
		tau.set_IsTight(tau.passMVAID(tightWPDef));

		assert(tau.passMVAID("VL"));
		if (!loose and !tau.passTightSel()) continue;
		/*
		if (WP!="-") {
			if (not tau.passMVAID(WP)) continue;
		}
		else {// if WP is not explicitly set
			if (!loose and !(tau_idSelection->at(t))) continue;
		}
		*/	
		
		// Tau decay products
		// charged hadron
		std::vector<TLorentzVector> chP4;
        std::vector<int> charges;
		for (unsigned int ch = 0; ch<(tau_signalChargedHadrCands_pt->at(t)).size();++ch) {
			TLorentzVector chargedhadr;
			chargedhadr.SetPtEtaPhiE((tau_signalChargedHadrCands_pt->at(t))[ch],
									 (tau_signalChargedHadrCands_eta->at(t))[ch],
									 (tau_signalChargedHadrCands_phi->at(t))[ch],
									 (tau_signalChargedHadrCands_E->at(t))[ch]);
			chP4.push_back(chargedhadr);

            charges.push_back((tau_signalChargedHadrCands_charge->at(t))[ch]);
		}			
		tau.set_signalChargedHadrCands(chP4);
        tau.set_signalChargedHadrCands_charge(charges);

		// gamma (pi0)
		std::vector<TLorentzVector> gmP4;
		for (unsigned int g=0; g<(tau_signalGammaCands_pt->at(t)).size(); ++g) {
			TLorentzVector gamma;
			gamma.SetPtEtaPhiE((tau_signalGammaCands_pt->at(t))[g],
							   (tau_signalGammaCands_eta->at(t))[g],
							   (tau_signalGammaCands_phi->at(t))[g],
							   (tau_signalGammaCands_E->at(t))[g]);
			gmP4.push_back(gamma);
		}
		tau.set_signalGammaCands(gmP4);

		// neutral hadron
		std::vector<TLorentzVector> nhP4;
		for (unsigned int nh=0; nh<(tau_signalNeutrHadrCands_pt->at(t)).size(); ++nh) {
			TLorentzVector neutralhadr;
			neutralhadr.SetPtEtaPhiE((tau_signalNeutrHadrCands_pt->at(t))[nh],
									 (tau_signalNeutrHadrCands_eta->at(t))[nh],
									 (tau_signalNeutrHadrCands_phi->at(t))[nh],
									 (tau_signalNeutrHadrCands_E->at(t))[nh]);
		    nhP4.push_back(neutralhadr);
		}
		tau.set_signalNeutrHadrCands(nhP4);

		taus.push_back(tau);
	}
	// should be already sorted by pt
	return taus;
}

std::vector<TLorentzVector> eventNtuple::buildFourVectorTaus(std::vector<int>& decaymode, bool loose) const
{
	
	std::vector<TLorentzVector> tausP4;

	decaymode.clear();
	
	for (unsigned int t = 0; t < tau_pt->size(); ++t) {
		// require tight tau if not loose selection
		if (!loose and !(tau_idSelection->at(t))) continue;

		TLorentzVector tau;
		tau.SetPtEtaPhiE(tau_pt->at(t),tau_eta->at(t),tau_phi->at(t),tau_E->at(t));
		tausP4.push_back(tau);
		decaymode.push_back(tau_decayMode->at(t));
	}
	// should be already sorted by pt
	return tausP4;
}

std::vector<TLorentzVector> eventNtuple::buildFourVectorTaus(bool loose) const
{
	std::vector<int> dummy;
	return buildFourVectorTaus(dummy, loose);
}

std::vector<TLorentzVector> eventNtuple::buildFourVectorTauDaugsCharged(bool loose) const
{
	std::vector<TLorentzVector> tauDaugsChargedP4;

	for (unsigned int t = 0; t < tau_pt->size(); ++t) {
		// require tight tau if not loose selection
		if (!loose and !(tau_idSelection->at(t))) continue;

		TLorentzVector chargedDaugsP4;
		
		// loop over tau charged daughters and add up four vectors
		for (unsigned int ch=0; ch<(tau_signalChargedHadrCands_pt->at(t)).size(); ++ch) {
			TLorentzVector chargedhadr;
			chargedhadr.SetPtEtaPhiE((tau_signalChargedHadrCands_pt->at(t))[ch],
									 (tau_signalChargedHadrCands_eta->at(t))[ch],
									 (tau_signalChargedHadrCands_phi->at(t))[ch],
									 (tau_signalChargedHadrCands_E->at(t))[ch]);
			chargedDaugsP4 += chargedhadr;
		}

		tauDaugsChargedP4.push_back(chargedDaugsP4);
	}

	return tauDaugsChargedP4;
}

std::vector<TLorentzVector> eventNtuple::buildFourVectorTauDaugsNeutral(bool loose) const
{
	std::vector<TLorentzVector> tauDaugsNeutralP4;

	for (unsigned int t = 0; t < tau_pt->size(); ++t) {
		// require tight tau if not loose selection
		if (!loose and !(tau_idSelection->at(t))) continue;

		TLorentzVector neutralDaugsP4;

		// loop over tau neutral daughters and add up four vectors
		for (unsigned int g=0; g<(tau_signalGammaCands_pt->at(t)).size(); ++g) {
			TLorentzVector gamma;
			gamma.SetPtEtaPhiE((tau_signalGammaCands_pt->at(t))[g],
							   (tau_signalGammaCands_eta->at(t))[g],
							   (tau_signalGammaCands_phi->at(t))[g],
							   (tau_signalGammaCands_E->at(t))[g]);
			neutralDaugsP4 += gamma;
		}

		for (unsigned int nh=0; nh<(tau_signalNeutrHadrCands_pt->at(t)).size(); ++nh) {
			TLorentzVector neutralhadr;
			neutralhadr.SetPtEtaPhiE((tau_signalNeutrHadrCands_pt->at(t))[nh],
									 (tau_signalNeutrHadrCands_eta->at(t))[nh],
									 (tau_signalNeutrHadrCands_phi->at(t))[nh],
									 (tau_signalNeutrHadrCands_E->at(t))[nh]);
			neutralDaugsP4 += neutralhadr;
		}
		

		tauDaugsNeutralP4.push_back(neutralDaugsP4);
	}

	return tauDaugsNeutralP4;
}

std::vector<miniJet> eventNtuple::buildJets(const TString& jec, bool doSmear,
                                            float minPt) const
{
	std::vector<miniJet> jets;

	for (unsigned int j = 0; j < jet_pt->size(); ++j) {

		TLorentzVector jp4;
		jp4.SetPtEtaPhiE(jet_pt->at(j),jet_eta->at(j),jet_phi->at(j),jet_E->at(j));

        if (doSmear) {
          float smearfactor = jet_smearFactor->at(j);
          jp4 *= smearfactor;
        }

		// JEC uncertainties
        float scalefactor = 1.;
        
        if (jec.EqualTo("jesup", TString::ECaseCompare::kIgnoreCase))
          scalefactor = 1. + jet_jesUnc->at(j);
        else if (jec.EqualTo("jesdown", TString::ECaseCompare::kIgnoreCase))
          scalefactor = 1. - jet_jesUnc->at(j);
        else if (jec.EqualTo("jerup", TString::ECaseCompare::kIgnoreCase)) {
          assert(doSmear);
          scalefactor = jet_smearFactor_up->at(j) / jet_smearFactor->at(j);
        }
        else if (jec.EqualTo("jerdown", TString::ECaseCompare::kIgnoreCase)) {
          assert(doSmear);
          scalefactor = jet_smearFactor_down->at(j) / jet_smearFactor->at(j);
        }

        if (scalefactor != 1.)
          jp4 *= scalefactor;
		
		if (jp4.Pt() < minPt) continue;
		
		miniJet jet(jp4, jet_csv->at(j), jet_flavor->at(j), jet_qgLikelihood->at(j));
		jets.push_back(jet);
	}
	
	return jets;
}

std::vector<miniJet> eventNtuple::buildCleanedJets(
	float cleaningdR, Analysis_types anatype, Selection_types seltype,
	std::vector<miniLepton> const * const fakeableLeps,
	std::vector<miniTau> const * const fakeableTaus,
	const TString& jec, bool doSmear, float minPt) const
{
	// Remove overlap jets by dR w.r.t. the first N leptons and first M taus
	// in pT descending order based on analysis type: (N)l(M)tau
	int NLeps = Types_enum::getNnominalLeptons(anatype);
	int MTaus = Types_enum::getNnominalTaus(anatype, seltype);
	
	std::vector<miniJet> jets_raw = buildJets(jec,doSmear,minPt);
	
	std::vector<miniJet> jets_clean;
	for (const auto & jet : jets_raw) {
		bool keep = true;
		
		int ilep = 0;
		for (const auto & lep : (*fakeableLeps)) {
			if (ilep >= NLeps) break;
			if ( jet.p4().DeltaR(lep.p4()) < cleaningdR ) keep = false;
			ilep++;
		}

		int itau = 0;
		for (const auto & tau : (*fakeableTaus)) {
			if (itau >= MTaus) break;
			if ( jet.p4().DeltaR(tau.p4()) < cleaningdR ) keep = false;
			itau++;
		}

		if (keep) jets_clean.push_back(jet);
	}

	return jets_clean;
}

std::tuple<int, int> eventNtuple::count_btags(const std::vector<miniJet>& jets) const
{
	// FIXME: hard-coded csv WPs for now
	// should move them to some better place and pass as arguments
	float csv_loose_wp = 0.1522;  // DeepCSV loose wp
	float csv_medium_wp = 0.4941; // DeepCSV medium wp
	
	int nbtags_loose = 0;
	int nbtags_medium = 0;

	for (const auto & jet : jets) {
		if (jet.csv() > csv_loose_wp) nbtags_loose++;
		if (jet.csv() > csv_medium_wp) nbtags_medium++;
	}

	assert(nbtags_loose >= nbtags_medium);
	
	return std::make_tuple(nbtags_loose, nbtags_medium);
}

std::vector<TLorentzVector> eventNtuple::buildFourVectorJets() const
{
	std::cerr << "Method deprecated" << std::endl;
	assert(0);
	
	std::vector<TLorentzVector> jetsP4;
	
	for (unsigned int j = 0; j < jet_pt->size(); ++j) {
		TLorentzVector jet;
		jet.SetPtEtaPhiE(jet_pt->at(j),jet_eta->at(j),jet_phi->at(j),jet_E->at(j));
		jetsP4.push_back(jet);
	}
	// should be already sorted by pt
	return jetsP4;
}

std::vector<TLorentzVector> eventNtuple::buildFourVectorBtagJets() const
{
	std::cerr << "Method deprecated" << std::endl;
	assert(0);
	
	// return two jets with the highest csv score
	
	std::vector<TLorentzVector> btagsP4;
	btagsP4.reserve(2);

	int icsv0 = -1; int icsv1 = -1;
	float csv0 = -233.; float csv1 = -233;
	
	for (unsigned int j = 0; j < jet_pt->size(); ++j) {
		if (jet_csv->at(j) > csv0) {
			csv1 = csv0;
			icsv1 = icsv0;
			csv0 = jet_csv->at(j);
			icsv0 = j;
		}
		else if (jet_csv->at(j) > csv1) {
			csv1 = jet_csv->at(j);
			icsv1 = j;
		}
	}
	
	TLorentzVector bj0, bj1;
	bj0.SetPtEtaPhiE(jet_pt->at(icsv0),jet_eta->at(icsv0),jet_phi->at(icsv0),
					 jet_E->at(icsv0));
	assert(jet_csv->at(icsv0)==csv0);
	bj1.SetPtEtaPhiE(jet_pt->at(icsv1),jet_eta->at(icsv1),jet_phi->at(icsv1),
					 jet_E->at(icsv1));
	assert(jet_csv->at(icsv1)==csv1);
	btagsP4.push_back(bj0);
	btagsP4.push_back(bj1);

	return btagsP4;
}

TLorentzVector eventNtuple::buildFourVectorMET(const TString& correction)
{
  // FIXME 
  
	TLorentzVector met;
	
	if (correction.EqualTo("jesup", TString::ECaseCompare::kIgnoreCase))
		met.SetPtEtaPhiM(PFMET_jesup, 0., PFMETphi_jesup, 0.);
	else if (correction.EqualTo("jesdown", TString::ECaseCompare::kIgnoreCase))
		met.SetPtEtaPhiM(PFMET_jesdown, 0., PFMETphi_jesdown, 0.);
	else if (correction.EqualTo("tesup", TString::ECaseCompare::kIgnoreCase))
		met.SetPtEtaPhiM(PFMET_tesup, 0., PFMETphi_tesup, 0.);
	else if (correction.EqualTo("tesdown", TString::ECaseCompare::kIgnoreCase))
		met.SetPtEtaPhiM(PFMET_tesdown, 0., PFMETphi_tesdown, 0.);
	else
		met.SetPtEtaPhiM(PFMET, 0., PFMETphi, 0.);
	
	return met;
}

float eventNtuple::computeMHT(const std::vector<miniLepton>& leptons,
							  const std::vector<miniTau>& taus,
							  const std::vector<miniJet>& jets)
{
	TLorentzVector mht;

	for (const auto & l : leptons)
		mht -= l.p4();
	for (const auto & t : taus)
		mht -= t.p4();
	for (const auto & j : jets)
		mht -= j.p4();

	return mht.Pt();
}

float eventNtuple::computeMHT()
{
	std::cerr << "Method deprecated." << std::endl;
	assert(0);
	
	// FIXME: use fakeable leptons
	
	TLorentzVector mht;

	auto fakeableleptons = buildLeptons();
	auto loosetaus = buildTaus(true);
	auto loosejets = buildFourVectorJets();

	for (const auto l: fakeableleptons)
		mht -= l.p4();
	for (const auto t: loosetaus)
		mht -= t.p4();
	for (const auto j: loosejets)
		mht -= j;

	return mht.Pt();
}

void eventNtuple::initialize()
{
	// event variables
	//std::cout << "eventNtuple::initialize(): event ids" << std::endl;
	run = 0;
	ls = 0;
    nEvent = 0;

	//std::cout << "eventNtuple::initialize(): weights" << std::endl;
	event_weight = -9999.;
	PU_weight = -9999.;
	MC_weight = -9999.;
	bTagSF_weight = -9999.;
	leptonSF_weight = -9999.;
	tauSF_weight = -9999.;
	triggerSF_weight = -9999.;
	FR_weight = -9999.;

	/////////////////////////////
	// systematics
	MC_weight_scale_muF0p5 = -9999.;
	MC_weight_scale_muF2 = -9999.;
	MC_weight_scale_muR0p5 = -9999.;
	MC_weight_scale_muR2 = -9999.;
	btagSF_weight_LFUp = -9999.;
	btagSF_weight_LFDown = -9999.;
	btagSF_weight_HFUp = -9999.;
	btagSF_weight_HFDown = -9999.;
	btagSF_weight_HFStats1Up = -9999.;
	btagSF_weight_HFStats1Down = -9999.;
	btagSF_weight_HFStats2Up = -9999.;
	btagSF_weight_HFStats2Down = -9999.;
	btagSF_weight_LFStats1Up = -9999.;
	btagSF_weight_LFStats1Down = -9999.;
	btagSF_weight_LFStats2Up = -9999.;
	btagSF_weight_LFStats2Down = -9999.;
	btagSF_weight_cErr1Up = -9999.;
	btagSF_weight_cErr1Down = -9999.;
	btagSF_weight_cErr2Up = -9999.;
	btagSF_weight_cErr2Down = -9999.;
	/////////////////////////////

	HiggsDecayType = -9999;  // Higgs decay product pdgId

    lepCategory = -9999;    // 0: mumu; 1: ee; 2:emu
    btagCategory = -9999;   // 0: loose; 1: medium (>=2 medium btags)

	// pileup
	npuTrue = -9999.;
	npuInTime = -9999.;

	pvx = -9999.;
	pvy = -9999.;
	pvz = -9999.;
	
	// triggers
	// triggerBits_single_e = 0;
	// triggerBits_single_mu = 0;
	// triggerBits_double_e = 0;
	// triggerBits_double_mu = 0;
	// triggerBits_elemu = 0;
	triggerBits = 0;
	
	filterBits = 0;

	nBadMuons = -9999;

	// event selection flag
	passTauCharge = -9999;
	isGenMatchedLep = -9999;
	isGenMatchedTau  = -9999;
	
	// event level MVA variables
	//MVA_2lss_ttV;
	//MVA_2lss_ttbar;
	//MT_met_lep0;
	//mindr_lep0_jet;
	//mindr_lep1_jet;
	//lep0_conept;
	//lep1_conept;
	//avg_dr_jet;

	//ibin;  // bin index in 1D BDT shape template

    n_presel_mu = -9999;
    n_fakeable_mu = -9999;
    n_mvasel_mu = -9999;
    n_presel_ele = -9999;
	n_fakeable_ele = -9999;
	n_mvasel_ele = -9999;
	n_presel_tau = -9999;
	n_tau = -9999;
	n_presel_jet = -9999;
	n_jet = -9999;
	n_btag_medium = -9999;
	n_btag_loose = -9999;

	// muons
	//std::cout << "eventNtuple::initialize(): muons" << std::endl;
	mu_pt->clear();
	mu_conept->clear();
	mu_eta->clear();
	mu_phi->clear();
	mu_E->clear();
	mu_charge->clear();
	mu_dxy->clear();
	mu_dz->clear();
	mu_isfakeablesel->clear();
	mu_ismvasel->clear();
	mu_jetNDauChargedMVASel->clear();
	mu_miniRelIso->clear();
	mu_miniIsoCharged->clear();
	mu_miniIsoNeutral->clear();
	mu_jetPtRel->clear();
	mu_jetPtRatio->clear();
	mu_jetCSV->clear();
	mu_sip3D->clear();
	mu_segmentCompatibility->clear();
	mu_leptonMVA->clear();
	mu_pfRelIso04->clear();
	mu_istightcharge->clear();
	mu_mediumID->clear();
	mu_dpt_div_pt->clear();
	mu_mcMatchType->clear();
	mu_isPFMuon->clear();

	// electrons
	//std::cout << "eventNtuple::initialize(): electrons" << std::endl;
	ele_pt->clear();
	ele_conept->clear();
	ele_eta->clear();
	ele_phi->clear();
	ele_E->clear();
	ele_charge->clear();
	ele_dxy->clear();
	ele_dz->clear();
	ele_isfakeablesel->clear();
	ele_ismvasel->clear();
	ele_jetNDauChargedMVASel->clear();
	ele_miniRelIso->clear();
	ele_miniIsoCharged->clear();
	ele_miniIsoNeutral->clear();
	ele_jetPtRel->clear();
	ele_jetPtRatio->clear();
	ele_jetCSV->clear();
	ele_sip3D->clear();
	ele_ntMVAeleID->clear();
	ele_leptonMVA->clear();
	ele_pfRelIso04->clear();
	ele_istightcharge->clear();
	ele_isChargeConsistent->clear();
	ele_passesConversionVeto->clear();
	ele_nMissingHits->clear();
	ele_mcMatchType->clear();
	ele_isGenPhotonMatched->clear();
	ele_isPromptGenPhotonMatched->clear();
	ele_ecalTrkEnergyPreCorr->clear();
	ele_ecalTrkEnergyPostCorr->clear();
	ele_energyScaleValue->clear();
	ele_energySigmaValue->clear();
	ele_energySmearNrSigma->clear();
    ele_energyScaleUp->clear();
    ele_energyScaleDown->clear();
    ele_energySigmaUp->clear();
    ele_energySigmaDown->clear();
	
	// taus
	//std::cout << "eventNtuple::initialize(): taus" << std::endl;
	tau_pt->clear();
	tau_eta->clear();
	tau_phi->clear();
	tau_E->clear();
	tau_charge->clear();
	tau_dxy->clear();
	tau_dz->clear();
	tau_decayMode->clear();
	tau_decayModeFindingOldDMs->clear();
	tau_decayModeFindingNewDMs->clear();
	tau_byCombinedIsolationDeltaBetaCorr3Hits->clear();
	tau_byLooseCombinedIsolationDeltaBetaCorr3Hits->clear();
	tau_byMediumCombinedIsolationDeltaBetaCorr3Hits->clear();
	tau_byTightCombinedIsolationDeltaBetaCorr3Hits->clear();
	//tau_byLooseCombinedIsolationDeltaBetaCorr3HitsdR03->clear();
	//tau_byMediumCombinedIsolationDeltaBetaCorr3HitsdR03->clear();
	//tau_byTightCombinedIsolationDeltaBetaCorr3HitsdR03->clear();
	tau_byVLooseIsolationMVArun2v1DBdR03oldDMwLT->clear();
	tau_byLooseIsolationMVArun2v1DBdR03oldDMwLT->clear();
	tau_byMediumIsolationMVArun2v1DBdR03oldDMwLT->clear();
	tau_byTightIsolationMVArun2v1DBdR03oldDMwLT->clear();
	tau_byVTightIsolationMVArun2v1DBdR03oldDMwLT->clear();
	tau_byIsolationMVArun2v1DBdR03oldDMwLTraw->clear();
	tau_againstMuonLoose3->clear();
	tau_againstMuonTight3->clear();
	tau_againstElectronVLooseMVA6->clear();
	tau_againstElectronLooseMVA6->clear();
	tau_againstElectronMediumMVA6->clear();
	tau_againstElectronTightMVA6->clear();
	tau_idPreselection->clear();
	tau_idSelection->clear();
	tau_mcMatchType->clear();
	tau_ecalEnergy->clear();
	tau_hcalEnergy->clear();
	//tau_isPFTau->clear();
	//tau_isCaloTau->clear();
	///
	tau_signalChargedHadrCands_pt->clear();
	tau_signalChargedHadrCands_eta->clear();
	tau_signalChargedHadrCands_phi->clear();
	tau_signalChargedHadrCands_E->clear();
	tau_signalChargedHadrCands_charge->clear();
	tau_signalNeutrHadrCands_pt->clear();
	tau_signalNeutrHadrCands_eta->clear();
	tau_signalNeutrHadrCands_phi->clear();
	tau_signalNeutrHadrCands_E->clear();
	tau_signalGammaCands_pt->clear();
	tau_signalGammaCands_eta->clear();
	tau_signalGammaCands_phi->clear();
	tau_signalGammaCands_E->clear();
	
	// jets
	//std::cout << "eventNtuple::initialize(): jets" << std::endl;
	jet_pt->clear();
	jet_eta->clear();
	jet_phi->clear();
	jet_E->clear();
	jet_csv->clear();
	jet_deepcsv->clear();
	jet_deepCvsB->clear();
	jet_deepCvsL->clear();
	jet_csvv2->clear();
	jet_ctagCvsB->clear();
	jet_ctagCvsL->clear();
	jet_flavor->clear();
	jet_qgLikelihood->clear();
	jet_axis2->clear();
	jet_ptD->clear();
	jet_mult->clear();
	jet_jesUnc->clear();
    jet_smearFactor->clear();
    jet_smearFactor_up->clear();
    jet_smearFactor_down->clear();

	// met
	//std::cout << "eventNtuple::initialize(): met" << std::endl;
	PFMET = -9999.;
	PFMETphi = -9999.;
	MHT = -9999.;
	metLD = -9999.;
	METSignificance = -9999.;
	METCov00 = -9999.;
	METCov10 = -9999.;
	METCov01 = -9999.;
	METCov11 = -9999.;
	
	PFMET_jesup = -9999.;
	PFMETphi_jesup = -9999.;
	PFMET_jesdown = -9999.;
	PFMETphi_jesdown = -9999.;
	PFMET_tesup = -9999.;
	PFMETphi_tesup = -9999.;
	PFMET_tesdown = -9999.;
	PFMETphi_tesdown = -9999.;

    // gen objects
    matchedGenMuons_pt->clear();
    matchedGenMuons_eta->clear();
    matchedGenMuons_phi->clear();
    matchedGenMuons_E->clear();
    matchedGenMuons_status->clear();
    matchedGenMuons_pdgid->clear();

    matchedGenElectrons_pt->clear();
    matchedGenElectrons_eta->clear();
    matchedGenElectrons_phi->clear();
    matchedGenElectrons_E->clear();
    matchedGenElectrons_status->clear();
    matchedGenElectrons_pdgid->clear();

    matchedGenTaus_pt->clear();
    matchedGenTaus_eta->clear();
    matchedGenTaus_phi->clear();
    matchedGenTaus_E->clear();
    matchedGenTaus_status->clear();
    matchedGenTaus_pdgid->clear();

    genjet_pt->clear();
    genjet_eta->clear();
    genjet_phi->clear();
    genjet_E->clear();
}

void eventNtuple::setup_branches(TTree* tree)
{
	tree->Branch("run", &run);
	tree->Branch("ls", &ls);
	tree->Branch("nEvent", &nEvent);
	tree->Branch("event_weight", &event_weight);
	tree->Branch("PU_weight", &PU_weight);
	tree->Branch("MC_weight", &MC_weight);
	tree->Branch("bTagSF_weight", &bTagSF_weight);
	tree->Branch("leptonSF_weight", &leptonSF_weight);
	tree->Branch("tauSF_weight", &tauSF_weight);
	tree->Branch("triggerSF_weight", &triggerSF_weight);
	tree->Branch("FR_weight", &FR_weight);
	tree->Branch("MC_weight_scale_muF0p5", &MC_weight_scale_muF0p5);
	tree->Branch("MC_weight_scale_muF2", &MC_weight_scale_muF2);
	tree->Branch("MC_weight_scale_muR0p5", &MC_weight_scale_muR0p5);
	tree->Branch("MC_weight_scale_muR2", &MC_weight_scale_muR2);
	tree->Branch("btagSF_weight_LFUp", &btagSF_weight_LFUp);
	tree->Branch("btagSF_weight_LFDown", &btagSF_weight_LFDown);
	tree->Branch("btagSF_weight_HFUp", &btagSF_weight_HFUp);
	tree->Branch("btagSF_weight_HFDown", &btagSF_weight_HFDown);
	tree->Branch("btagSF_weight_HFStats1Up", &btagSF_weight_HFStats1Up);
	tree->Branch("btagSF_weight_HFStats1Down", &btagSF_weight_HFStats1Down);
	tree->Branch("btagSF_weight_HFStats2Up", &btagSF_weight_HFStats2Up);
	tree->Branch("btagSF_weight_HFStats2Down", &btagSF_weight_HFStats2Down);
	tree->Branch("btagSF_weight_LFStats1Up", &btagSF_weight_LFStats1Up);
	tree->Branch("btagSF_weight_LFStats1Down", &btagSF_weight_LFStats1Down);
	tree->Branch("btagSF_weight_LFStats2Up", &btagSF_weight_LFStats2Up);
	tree->Branch("btagSF_weight_LFStats2Down", &btagSF_weight_LFStats2Down);
	tree->Branch("btagSF_weight_cErr1Up", &btagSF_weight_cErr1Up);
	tree->Branch("btagSF_weight_cErr1Down", &btagSF_weight_cErr1Down);
	tree->Branch("btagSF_weight_cErr2Up", &btagSF_weight_cErr2Up);
	tree->Branch("btagSF_weight_cErr2Down", &btagSF_weight_cErr2Down);
	tree->Branch("HiggsDecayType", &HiggsDecayType);
	tree->Branch("lepCategory", &lepCategory);
	tree->Branch("btagCategory", &btagCategory);
	tree->Branch("npuTrue", &npuTrue);
	tree->Branch("npuInTime", &npuInTime);
	tree->Branch("pvx", &pvx);
	tree->Branch("pvy", &pvy);
	tree->Branch("pvz", &pvz);
	//tree->Branch("triggerBits_single_e", &triggerBits_single_e);
	//tree->Branch("triggerBits_single_mu", &triggerBits_single_mu);
	//tree->Branch("triggerBits_double_e", &triggerBits_double_e);
	//tree->Branch("triggerBits_double_mu", &triggerBits_double_mu);
	//tree->Branch("triggerBits_elemu", &triggerBits_elemu);
	tree->Branch("triggerBits", &triggerBits);
	tree->Branch("filterBits", &filterBits);
	tree->Branch("nBadMuons", &nBadMuons);
	
	tree->Branch("passTauCharge", &passTauCharge);
	tree->Branch("isGenMatchedLep", &isGenMatchedLep);
	tree->Branch("isGenMatchedTau", &isGenMatchedTau);

	tree->Branch("n_presel_mu", &n_presel_mu);
	tree->Branch("n_fakeable_mu", &n_fakeable_mu);
	tree->Branch("n_mvasel_mu", &n_mvasel_mu);
	tree->Branch("n_presel_ele", &n_presel_ele);
	tree->Branch("n_fakeable_ele", &n_fakeable_ele);
	tree->Branch("n_mvasel_ele", &n_mvasel_ele);
	tree->Branch("n_presel_tau", &n_presel_tau);
	tree->Branch("n_tau", &n_tau);
	tree->Branch("n_presel_jet", &n_presel_jet);
	tree->Branch("n_jet", &n_jet);
	tree->Branch("n_btag_loose", &n_btag_loose);
	tree->Branch("n_btag_medium", &n_btag_medium);

	tree->Branch("mu_pt",                   &mu_pt);
	tree->Branch("mu_conept",               &mu_conept);
	tree->Branch("mu_eta",                  &mu_eta);
	tree->Branch("mu_phi",                  &mu_phi);
	tree->Branch("mu_E",                    &mu_E);
	tree->Branch("mu_charge",               &mu_charge);
	tree->Branch("mu_jetNDauChargedMVASel", &mu_jetNDauChargedMVASel);
	tree->Branch("mu_miniRelIso",           &mu_miniRelIso);
	tree->Branch("mu_miniIsoCharged",       &mu_miniIsoCharged);
	tree->Branch("mu_miniIsoNeutral",       &mu_miniIsoNeutral);
	tree->Branch("mu_jetPtRel",             &mu_jetPtRel);
	tree->Branch("mu_jetPtRatio",           &mu_jetPtRatio);
	tree->Branch("mu_jetCSV",               &mu_jetCSV);
	tree->Branch("mu_sip3D",                &mu_sip3D);
	tree->Branch("mu_dxy",                  &mu_dxy);
	tree->Branch("mu_dz",                   &mu_dz);
	tree->Branch("mu_segmentCompatibility", &mu_segmentCompatibility);
	tree->Branch("mu_leptonMVA",            &mu_leptonMVA);
	tree->Branch("mu_pfRelIso04",           &mu_pfRelIso04);
	tree->Branch("mu_istightcharge",        &mu_istightcharge);
	tree->Branch("mu_mediumID",             &mu_mediumID);
	tree->Branch("mu_dpt_div_pt",           &mu_dpt_div_pt);
	tree->Branch("mu_ismvasel",             &mu_ismvasel);
	tree->Branch("mu_isfakeablesel",        &mu_isfakeablesel);
	tree->Branch("mu_mcMatchType",          &mu_mcMatchType);
	tree->Branch("mu_isPFMuon",             &mu_isPFMuon);

	tree->Branch("ele_pt",                   &ele_pt);
	tree->Branch("ele_conept",               &ele_conept);
	tree->Branch("ele_eta",                  &ele_eta);
	tree->Branch("ele_phi",                  &ele_phi);
	tree->Branch("ele_E",                    &ele_E);
	tree->Branch("ele_charge",               &ele_charge);
	tree->Branch("ele_jetNDauChargedMVASel", &ele_jetNDauChargedMVASel);
	tree->Branch("ele_miniRelIso",           &ele_miniRelIso);
	tree->Branch("ele_miniIsoCharged",       &ele_miniIsoCharged);
	tree->Branch("ele_miniIsoNeutral",       &ele_miniIsoNeutral);
	tree->Branch("ele_jetPtRel",             &ele_jetPtRel);
	tree->Branch("ele_jetPtRatio",           &ele_jetPtRatio);
	tree->Branch("ele_jetCSV",               &ele_jetCSV);
	tree->Branch("ele_sip3D",                &ele_sip3D);
	tree->Branch("ele_dxy",                  &ele_dxy);
	tree->Branch("ele_dz",                   &ele_dz);
	tree->Branch("ele_ntMVAeleID",           &ele_ntMVAeleID);
	tree->Branch("ele_leptonMVA",            &ele_leptonMVA);
	tree->Branch("ele_pfRelIso04",           &ele_pfRelIso04);
	tree->Branch("ele_istightcharge",        &ele_istightcharge);
	tree->Branch("ele_isChargeConsistent",   &ele_isChargeConsistent);
	tree->Branch("ele_passesConversionVeto", &ele_passesConversionVeto);
	tree->Branch("ele_nMissingHits",         &ele_nMissingHits);
	tree->Branch("ele_ismvasel",             &ele_ismvasel);
	tree->Branch("ele_isfakeablesel",        &ele_isfakeablesel);
	tree->Branch("ele_mcMatchType",          &ele_mcMatchType);
	tree->Branch("ele_isGenPhotonMatched",   &ele_isGenPhotonMatched);
	tree->Branch("ele_isPromptGenPhotonMatched", &ele_isPromptGenPhotonMatched);
	tree->Branch("ele_ecalTrkEnergyPreCorr",  &ele_ecalTrkEnergyPreCorr);
	tree->Branch("ele_ecalTrkEnergyPostCorr", &ele_ecalTrkEnergyPostCorr);
	tree->Branch("ele_energyScaleValue",      &ele_energyScaleValue);
	tree->Branch("ele_energySigmaValue",      &ele_energySigmaValue);
	tree->Branch("ele_energySmearNrSigma",    &ele_energySmearNrSigma);
	tree->Branch("ele_energyScaleUp",         &ele_energyScaleUp);
	tree->Branch("ele_energyScaleDown",       &ele_energyScaleDown);
	tree->Branch("ele_energySigmaUp",         &ele_energySigmaUp);
	tree->Branch("ele_energySigmaDown",       &ele_energySigmaDown);

	tree->Branch("tau_pt", &tau_pt);
	tree->Branch("tau_eta", &tau_eta);
	tree->Branch("tau_phi", &tau_phi);
	tree->Branch("tau_E", &tau_E);
	tree->Branch("tau_charge", &tau_charge);
	tree->Branch("tau_dxy", &tau_dxy);
	tree->Branch("tau_dz", &tau_dz);
	tree->Branch("tau_decayMode", &tau_decayMode);
	tree->Branch("tau_decayModeFindingOldDMs", &tau_decayModeFindingOldDMs);
	tree->Branch("tau_decayModeFindingNewDMs", &tau_decayModeFindingNewDMs);
	tree->Branch("tau_byCombinedIsolationDeltaBetaCorr3Hits", &tau_byCombinedIsolationDeltaBetaCorr3Hits);
	tree->Branch("tau_byLooseCombinedIsolationDeltaBetaCorr3Hits", &tau_byLooseCombinedIsolationDeltaBetaCorr3Hits);
	tree->Branch("tau_byMediumCombinedIsolationDeltaBetaCorr3Hits", &tau_byMediumCombinedIsolationDeltaBetaCorr3Hits);
	tree->Branch("tau_byTightCombinedIsolationDeltaBetaCorr3Hits", &tau_byTightCombinedIsolationDeltaBetaCorr3Hits);
	//tree->Branch("tau_byLooseCombinedIsolationDeltaBetaCorr3HitsdR03", &tau_byLooseCombinedIsolationDeltaBetaCorr3HitsdR03);
	//tree->Branch("tau_byMediumCombinedIsolationDeltaBetaCorr3HitsdR03", &tau_byMediumCombinedIsolationDeltaBetaCorr3HitsdR03);
	//tree->Branch("tau_byTightCombinedIsolationDeltaBetaCorr3HitsdR03", &tau_byTightCombinedIsolationDeltaBetaCorr3HitsdR03);
	tree->Branch("tau_byVLooseIsolationMVArun2v1DBdR03oldDMwLT", &tau_byVLooseIsolationMVArun2v1DBdR03oldDMwLT);
	tree->Branch("tau_byLooseIsolationMVArun2v1DBdR03oldDMwLT", &tau_byLooseIsolationMVArun2v1DBdR03oldDMwLT);
	tree->Branch("tau_byMediumIsolationMVArun2v1DBdR03oldDMwLT", &tau_byMediumIsolationMVArun2v1DBdR03oldDMwLT);
	tree->Branch("tau_byTightIsolationMVArun2v1DBdR03oldDMwLT", &tau_byTightIsolationMVArun2v1DBdR03oldDMwLT);
	tree->Branch("tau_byVTightIsolationMVArun2v1DBdR03oldDMwLT", &tau_byVTightIsolationMVArun2v1DBdR03oldDMwLT);
	tree->Branch("tau_byIsolationMVArun2v1DBdR03oldDMwLTraw", &tau_byIsolationMVArun2v1DBdR03oldDMwLTraw);
	tree->Branch("tau_againstMuonLoose3", &tau_againstMuonLoose3);
	tree->Branch("tau_againstMuonTight3", &tau_againstMuonTight3);
	tree->Branch("tau_againstElectronVLooseMVA6", &tau_againstElectronVLooseMVA6);
	tree->Branch("tau_againstElectronLooseMVA6", &tau_againstElectronLooseMVA6);
	tree->Branch("tau_againstElectronMediumMVA6", &tau_againstElectronMediumMVA6);
	tree->Branch("tau_againstElectronTightMVA6", &tau_againstElectronTightMVA6);
	tree->Branch("tau_idPreselection", &tau_idPreselection);
	tree->Branch("tau_idSelection", &tau_idSelection);
	tree->Branch("tau_mcMatchType", &tau_mcMatchType);
	tree->Branch("tau_ecalEnergy", &tau_ecalEnergy);
	tree->Branch("tau_hcalEnergy", &tau_hcalEnergy);
	//tree->Branch("tau_isPFTau", &tau_isPFTau);
	//tree->Branch("tau_isCaloTau", &tau_isCaloTau);
	///
	tree->Branch("tau_signalChargedHadrCands_pt", &tau_signalChargedHadrCands_pt);
	tree->Branch("tau_signalChargedHadrCands_eta", &tau_signalChargedHadrCands_eta);
	tree->Branch("tau_signalChargedHadrCands_phi", &tau_signalChargedHadrCands_phi);
	tree->Branch("tau_signalChargedHadrCands_E", &tau_signalChargedHadrCands_E);
	tree->Branch("tau_signalChargedHadrCands_charge", &tau_signalChargedHadrCands_charge);
	tree->Branch("tau_signalNeutrHadrCands_pt", &tau_signalNeutrHadrCands_pt);
	tree->Branch("tau_signalNeutrHadrCands_eta", &tau_signalNeutrHadrCands_eta);
	tree->Branch("tau_signalNeutrHadrCands_phi", &tau_signalNeutrHadrCands_phi);
	tree->Branch("tau_signalNeutrHadrCands_E", &tau_signalNeutrHadrCands_E);
	tree->Branch("tau_signalGammaCands_pt", &tau_signalGammaCands_pt);
	tree->Branch("tau_signalGammaCands_eta", &tau_signalGammaCands_eta);
	tree->Branch("tau_signalGammaCands_phi", &tau_signalGammaCands_phi);
	tree->Branch("tau_signalGammaCands_E", &tau_signalGammaCands_E);
	///
	
	tree->Branch("jet_pt",           &jet_pt);
	tree->Branch("jet_eta",          &jet_eta);
	tree->Branch("jet_phi",          &jet_phi);
	tree->Branch("jet_E",            &jet_E);
	tree->Branch("jet_csv",          &jet_csv);
	tree->Branch("jet_deepcsv",      &jet_deepcsv);
	tree->Branch("jet_deepCvsB",     &jet_deepCvsB);
	tree->Branch("jet_deepCvsL",     &jet_deepCvsL);
	tree->Branch("jet_csvv2",        &jet_csvv2);
	tree->Branch("jet_ctagCvsB",     &jet_ctagCvsB);
	tree->Branch("jet_ctagCvsL",     &jet_ctagCvsL);
	tree->Branch("jet_flavor",       &jet_flavor);
	tree->Branch("jet_qgLikelihood", &jet_qgLikelihood);
	tree->Branch("jet_axis2",        &jet_axis2);
	tree->Branch("jet_ptD",          &jet_ptD);
	tree->Branch("jet_mult",         &jet_mult);
	tree->Branch("jet_jesUnc",       &jet_jesUnc);
    tree->Branch("jet_smearFactor",  &jet_smearFactor);
    tree->Branch("jet_smearFactor_up", &jet_smearFactor_up);
    tree->Branch("jet_smearFactor_down", &jet_smearFactor_down);

	tree->Branch("PFMET", &PFMET);
	tree->Branch("PFMETphi", &PFMETphi);
	tree->Branch("METSignificance", &METSignificance);
	tree->Branch("METCov00", &METCov00);
	tree->Branch("METCov01", &METCov01);
	tree->Branch("METCov10", &METCov10);
	tree->Branch("METCov11", &METCov11);
	tree->Branch("MHT", &MHT);
	tree->Branch("metLD", &metLD);
	tree->Branch("PFMET_jesup", &PFMET_jesup);
	tree->Branch("PFMETphi_jesup", &PFMETphi_jesup);
	tree->Branch("PFMET_jesdown", &PFMET_jesdown);
	tree->Branch("PFMETphi_jesdown", &PFMETphi_jesdown);
	tree->Branch("PFMET_tesup", &PFMET_tesup);
	tree->Branch("PFMETphi_tesup", &PFMETphi_tesup);
	tree->Branch("PFMET_tesdown", &PFMET_tesdown);
	tree->Branch("PFMETphi_tesdown", &PFMETphi_tesdown);

    tree->Branch("matchedGenMuons_pt",     &matchedGenMuons_pt);
    tree->Branch("matchedGenMuons_eta",    &matchedGenMuons_eta);
    tree->Branch("matchedGenMuons_phi",    &matchedGenMuons_phi);
    tree->Branch("matchedGenMuons_E",      &matchedGenMuons_E);
    tree->Branch("matchedGenMuons_status", &matchedGenMuons_status);
    tree->Branch("matchedGenMuons_pdgid",  &matchedGenMuons_pdgid);

    tree->Branch("matchedGenElectrons_pt",     &matchedGenElectrons_pt);
    tree->Branch("matchedGenElectrons_eta",    &matchedGenElectrons_eta);
    tree->Branch("matchedGenElectrons_phi",    &matchedGenElectrons_phi);
    tree->Branch("matchedGenElectrons_E",      &matchedGenElectrons_E);
    tree->Branch("matchedGenElectrons_status", &matchedGenElectrons_status);
    tree->Branch("matchedGenElectrons_pdgid",  &matchedGenElectrons_pdgid);

    tree->Branch("matchedGenTaus_pt",     &matchedGenTaus_pt);
    tree->Branch("matchedGenTaus_eta",    &matchedGenTaus_eta);
    tree->Branch("matchedGenTaus_phi",    &matchedGenTaus_phi);
    tree->Branch("matchedGenTaus_E",      &matchedGenTaus_E);
    tree->Branch("matchedGenTaus_status", &matchedGenTaus_status);
    tree->Branch("matchedGenTaus_pdgid",  &matchedGenTaus_pdgid);

    tree->Branch("genjet_pt",  &genjet_pt);
    tree->Branch("genjet_eta", &genjet_eta);
    tree->Branch("genjet_phi", &genjet_phi);
    tree->Branch("genjet_E",   &genjet_E);
}

void eventNtuple::set_branch_address(TTree* tree)
{
	tree->SetBranchAddress("run", &run);
	tree->SetBranchAddress("ls", &ls);
	tree->SetBranchAddress("nEvent", &nEvent);
	tree->SetBranchAddress("event_weight", &event_weight);
	tree->SetBranchAddress("PU_weight", &PU_weight);
	tree->SetBranchAddress("MC_weight", &MC_weight);
	tree->SetBranchAddress("bTagSF_weight", &bTagSF_weight);
	tree->SetBranchAddress("leptonSF_weight", &leptonSF_weight);
	tree->SetBranchAddress("tauSF_weight", &tauSF_weight);
	tree->SetBranchAddress("triggerSF_weight", &triggerSF_weight);
	tree->SetBranchAddress("FR_weight", &FR_weight);
	tree->SetBranchAddress("MC_weight_scale_muF0p5", &MC_weight_scale_muF0p5);
	tree->SetBranchAddress("MC_weight_scale_muF2", &MC_weight_scale_muF2);
	tree->SetBranchAddress("MC_weight_scale_muR0p5", &MC_weight_scale_muR0p5);
	tree->SetBranchAddress("MC_weight_scale_muR2", &MC_weight_scale_muR2);
	tree->SetBranchAddress("btagSF_weight_LFUp", &btagSF_weight_LFUp);
	tree->SetBranchAddress("btagSF_weight_LFDown", &btagSF_weight_LFDown);
	tree->SetBranchAddress("btagSF_weight_HFUp", &btagSF_weight_HFUp);
	tree->SetBranchAddress("btagSF_weight_HFDown", &btagSF_weight_HFDown);
	tree->SetBranchAddress("btagSF_weight_HFStats1Up", &btagSF_weight_HFStats1Up);
	tree->SetBranchAddress("btagSF_weight_HFStats1Down", &btagSF_weight_HFStats1Down);
	tree->SetBranchAddress("btagSF_weight_HFStats2Up", &btagSF_weight_HFStats2Up);
	tree->SetBranchAddress("btagSF_weight_HFStats2Down", &btagSF_weight_HFStats2Down);
	tree->SetBranchAddress("btagSF_weight_LFStats1Up", &btagSF_weight_LFStats1Up);
	tree->SetBranchAddress("btagSF_weight_LFStats1Down", &btagSF_weight_LFStats1Down);
	tree->SetBranchAddress("btagSF_weight_LFStats2Up", &btagSF_weight_LFStats2Up);
	tree->SetBranchAddress("btagSF_weight_LFStats2Down", &btagSF_weight_LFStats2Down);
	tree->SetBranchAddress("btagSF_weight_cErr1Up", &btagSF_weight_cErr1Up);
	tree->SetBranchAddress("btagSF_weight_cErr1Down", &btagSF_weight_cErr1Down);
	tree->SetBranchAddress("btagSF_weight_cErr2Up", &btagSF_weight_cErr2Up);
	tree->SetBranchAddress("btagSF_weight_cErr2Down", &btagSF_weight_cErr2Down);
	tree->SetBranchAddress("HiggsDecayType", &HiggsDecayType);
	tree->SetBranchAddress("lepCategory", &lepCategory);
	tree->SetBranchAddress("btagCategory", &btagCategory);
	tree->SetBranchAddress("npuTrue", &npuTrue);
	tree->SetBranchAddress("npuInTime", &npuInTime);
	tree->SetBranchAddress("pvx", &pvx);
	tree->SetBranchAddress("pvy", &pvy);
	tree->SetBranchAddress("pvz", &pvz);
	//tree->SetBranchAddress("triggerBits_single_e", &triggerBits_single_e);
	//tree->SetBranchAddress("triggerBits_single_mu", &triggerBits_single_mu);
	//tree->SetBranchAddress("triggerBits_double_e", &triggerBits_double_e);
	//tree->SetBranchAddress("triggerBits_double_mu", &triggerBits_double_mu);
	//tree->SetBranchAddress("triggerBits_elemu", &triggerBits_elemu);
	tree->SetBranchAddress("triggerBits", &triggerBits);
	tree->SetBranchAddress("filterBits", &filterBits);
	tree->SetBranchAddress("nBadMuons", &nBadMuons);

	tree->SetBranchAddress("passTauCharge", &passTauCharge);
	tree->SetBranchAddress("isGenMatchedLep", &isGenMatchedLep);
	tree->SetBranchAddress("isGenMatchedTau", &isGenMatchedTau);
	
	tree->SetBranchAddress("n_presel_mu", &n_presel_mu);
	tree->SetBranchAddress("n_fakeable_mu", &n_fakeable_mu);
	tree->SetBranchAddress("n_mvasel_mu", &n_mvasel_mu);
	tree->SetBranchAddress("n_presel_ele", &n_presel_ele);
	tree->SetBranchAddress("n_fakeable_ele", &n_fakeable_ele);
	tree->SetBranchAddress("n_mvasel_ele", &n_mvasel_ele);
	tree->SetBranchAddress("n_presel_tau", &n_presel_tau);
	tree->SetBranchAddress("n_tau", &n_tau);
	tree->SetBranchAddress("n_presel_jet", &n_presel_jet);
	tree->SetBranchAddress("n_jet", &n_jet);
	tree->SetBranchAddress("n_btag_loose", &n_btag_loose);
	tree->SetBranchAddress("n_btag_medium", &n_btag_medium);

	tree->SetBranchAddress("mu_pt",                   &mu_pt);
	tree->SetBranchAddress("mu_conept",               &mu_conept);
	tree->SetBranchAddress("mu_eta",                  &mu_eta);
	tree->SetBranchAddress("mu_phi",                  &mu_phi);
	tree->SetBranchAddress("mu_E",                    &mu_E);
	tree->SetBranchAddress("mu_charge",               &mu_charge);
	tree->SetBranchAddress("mu_jetNDauChargedMVASel", &mu_jetNDauChargedMVASel);
	tree->SetBranchAddress("mu_miniRelIso",           &mu_miniRelIso);
	tree->SetBranchAddress("mu_miniIsoCharged",       &mu_miniIsoCharged);
	tree->SetBranchAddress("mu_miniIsoNeutral",       &mu_miniIsoNeutral);
	tree->SetBranchAddress("mu_jetPtRel",             &mu_jetPtRel);
	tree->SetBranchAddress("mu_jetPtRatio",           &mu_jetPtRatio);
	tree->SetBranchAddress("mu_jetCSV",               &mu_jetCSV);
	tree->SetBranchAddress("mu_sip3D",                &mu_sip3D);
	tree->SetBranchAddress("mu_dxy",                  &mu_dxy);
	tree->SetBranchAddress("mu_dz",                   &mu_dz);
	tree->SetBranchAddress("mu_segmentCompatibility", &mu_segmentCompatibility);
	tree->SetBranchAddress("mu_leptonMVA",            &mu_leptonMVA);
	tree->SetBranchAddress("mu_pfRelIso04",           &mu_pfRelIso04);
	tree->SetBranchAddress("mu_istightcharge",        &mu_istightcharge);
	tree->SetBranchAddress("mu_mediumID",             &mu_mediumID);
	tree->SetBranchAddress("mu_dpt_div_pt",           &mu_dpt_div_pt);
	tree->SetBranchAddress("mu_ismvasel",             &mu_ismvasel);
	tree->SetBranchAddress("mu_isfakeablesel",        &mu_isfakeablesel);
	tree->SetBranchAddress("mu_mcMatchType",          &mu_mcMatchType);
	tree->SetBranchAddress("mu_isPFMuon",             &mu_isPFMuon);

	tree->SetBranchAddress("ele_pt",                   &ele_pt);
	tree->SetBranchAddress("ele_conept",               &ele_conept);
	tree->SetBranchAddress("ele_eta",                  &ele_eta);
	tree->SetBranchAddress("ele_phi",                  &ele_phi);
	tree->SetBranchAddress("ele_E",                    &ele_E);
	tree->SetBranchAddress("ele_charge",               &ele_charge);
	tree->SetBranchAddress("ele_jetNDauChargedMVASel", &ele_jetNDauChargedMVASel);
	tree->SetBranchAddress("ele_miniRelIso",           &ele_miniRelIso);
	tree->SetBranchAddress("ele_miniIsoCharged",       &ele_miniIsoCharged);
	tree->SetBranchAddress("ele_miniIsoNeutral",       &ele_miniIsoNeutral);
	tree->SetBranchAddress("ele_jetPtRel",             &ele_jetPtRel);
	tree->SetBranchAddress("ele_jetPtRatio",           &ele_jetPtRatio);
	tree->SetBranchAddress("ele_jetCSV",               &ele_jetCSV);
	tree->SetBranchAddress("ele_sip3D",                &ele_sip3D);
	tree->SetBranchAddress("ele_dxy",                  &ele_dxy);
	tree->SetBranchAddress("ele_dz",                   &ele_dz);
	tree->SetBranchAddress("ele_ntMVAeleID",           &ele_ntMVAeleID);
	tree->SetBranchAddress("ele_leptonMVA",            &ele_leptonMVA);
	tree->SetBranchAddress("ele_pfRelIso04",           &ele_pfRelIso04);
	tree->SetBranchAddress("ele_istightcharge",        &ele_istightcharge);
	tree->SetBranchAddress("ele_isChargeConsistent",   &ele_isChargeConsistent);
	tree->SetBranchAddress("ele_passesConversionVeto", &ele_passesConversionVeto);
	tree->SetBranchAddress("ele_nMissingHits",         &ele_nMissingHits);
	tree->SetBranchAddress("ele_ismvasel",             &ele_ismvasel);
	tree->SetBranchAddress("ele_isfakeablesel",        &ele_isfakeablesel);
	tree->SetBranchAddress("ele_mcMatchType",          &ele_mcMatchType);
	tree->SetBranchAddress("ele_isGenPhotonMatched",   &ele_isGenPhotonMatched);
	tree->SetBranchAddress("ele_isPromptGenPhotonMatched", &ele_isPromptGenPhotonMatched);
	tree->SetBranchAddress("ele_ecalTrkEnergyPreCorr", &ele_ecalTrkEnergyPreCorr);
	tree->SetBranchAddress("ele_ecalTrkEnergyPostCorr",&ele_ecalTrkEnergyPostCorr);
	tree->SetBranchAddress("ele_energyScaleValue",     &ele_energyScaleValue);
	tree->SetBranchAddress("ele_energySigmaValue",     &ele_energySigmaValue);
	tree->SetBranchAddress("ele_energySmearNrSigma",   &ele_energySmearNrSigma);
	tree->SetBranchAddress("ele_energyScaleUp",        &ele_energyScaleUp);
	tree->SetBranchAddress("ele_energyScaleDown",      &ele_energyScaleDown);
	tree->SetBranchAddress("ele_energySigmaUp",        &ele_energySigmaUp);
	tree->SetBranchAddress("ele_energySigmaDown",      &ele_energySigmaDown);

	tree->SetBranchAddress("tau_pt", &tau_pt);
	tree->SetBranchAddress("tau_eta", &tau_eta);
	tree->SetBranchAddress("tau_phi", &tau_phi);
	tree->SetBranchAddress("tau_E", &tau_E);
	tree->SetBranchAddress("tau_charge", &tau_charge);
	tree->SetBranchAddress("tau_dxy", &tau_dxy);
	tree->SetBranchAddress("tau_dz", &tau_dz);
	tree->SetBranchAddress("tau_decayMode", &tau_decayMode);
	tree->SetBranchAddress("tau_decayModeFindingOldDMs", &tau_decayModeFindingOldDMs);
	tree->SetBranchAddress("tau_decayModeFindingNewDMs", &tau_decayModeFindingNewDMs);
	tree->SetBranchAddress("tau_byCombinedIsolationDeltaBetaCorr3Hits", &tau_byCombinedIsolationDeltaBetaCorr3Hits);
	tree->SetBranchAddress("tau_byLooseCombinedIsolationDeltaBetaCorr3Hits", &tau_byLooseCombinedIsolationDeltaBetaCorr3Hits);
	tree->SetBranchAddress("tau_byMediumCombinedIsolationDeltaBetaCorr3Hits", &tau_byMediumCombinedIsolationDeltaBetaCorr3Hits);
	tree->SetBranchAddress("tau_byTightCombinedIsolationDeltaBetaCorr3Hits", &tau_byTightCombinedIsolationDeltaBetaCorr3Hits);
	//tree->SetBranchAddress("tau_byLooseCombinedIsolationDeltaBetaCorr3HitsdR03", &tau_byLooseCombinedIsolationDeltaBetaCorr3HitsdR03);
	//tree->SetBranchAddress("tau_byMediumCombinedIsolationDeltaBetaCorr3HitsdR03", &tau_byMediumCombinedIsolationDeltaBetaCorr3HitsdR03);
	//tree->SetBranchAddress("tau_byTightCombinedIsolationDeltaBetaCorr3HitsdR03", &tau_byTightCombinedIsolationDeltaBetaCorr3HitsdR03);
	tree->SetBranchAddress("tau_byVLooseIsolationMVArun2v1DBdR03oldDMwLT", &tau_byVLooseIsolationMVArun2v1DBdR03oldDMwLT);
	tree->SetBranchAddress("tau_byLooseIsolationMVArun2v1DBdR03oldDMwLT", &tau_byLooseIsolationMVArun2v1DBdR03oldDMwLT);
	tree->SetBranchAddress("tau_byMediumIsolationMVArun2v1DBdR03oldDMwLT", &tau_byMediumIsolationMVArun2v1DBdR03oldDMwLT);
	tree->SetBranchAddress("tau_byTightIsolationMVArun2v1DBdR03oldDMwLT", &tau_byTightIsolationMVArun2v1DBdR03oldDMwLT);
	tree->SetBranchAddress("tau_byVTightIsolationMVArun2v1DBdR03oldDMwLT", &tau_byVTightIsolationMVArun2v1DBdR03oldDMwLT);
	tree->SetBranchAddress("tau_byIsolationMVArun2v1DBdR03oldDMwLTraw", &tau_byIsolationMVArun2v1DBdR03oldDMwLTraw);
	tree->SetBranchAddress("tau_againstMuonLoose3", &tau_againstMuonLoose3);
	tree->SetBranchAddress("tau_againstMuonTight3", &tau_againstMuonTight3);
	tree->SetBranchAddress("tau_againstElectronVLooseMVA6", &tau_againstElectronVLooseMVA6);
	tree->SetBranchAddress("tau_againstElectronLooseMVA6", &tau_againstElectronLooseMVA6);
	tree->SetBranchAddress("tau_againstElectronMediumMVA6", &tau_againstElectronMediumMVA6);
	tree->SetBranchAddress("tau_againstElectronTightMVA6", &tau_againstElectronTightMVA6);
	tree->SetBranchAddress("tau_idPreselection", &tau_idPreselection);
	tree->SetBranchAddress("tau_idSelection", &tau_idSelection);
	tree->SetBranchAddress("tau_mcMatchType", &tau_mcMatchType);
	tree->SetBranchAddress("tau_ecalEnergy", &tau_ecalEnergy);
	tree->SetBranchAddress("tau_hcalEnergy", &tau_hcalEnergy);
	//tree->SetBranchAddress("tau_isPFTau", &tau_isPFTau);
	//tree->SetBranchAddress("tau_isCaloTau", &tau_isCaloTau);
	
	tree->SetBranchAddress("tau_signalChargedHadrCands_pt", &tau_signalChargedHadrCands_pt);
	tree->SetBranchAddress("tau_signalChargedHadrCands_eta", &tau_signalChargedHadrCands_eta);
	tree->SetBranchAddress("tau_signalChargedHadrCands_phi", &tau_signalChargedHadrCands_phi);
	tree->SetBranchAddress("tau_signalChargedHadrCands_E", &tau_signalChargedHadrCands_E);
	tree->SetBranchAddress("tau_signalChargedHadrCands_charge", &tau_signalChargedHadrCands_charge);
	tree->SetBranchAddress("tau_signalNeutrHadrCands_pt", &tau_signalNeutrHadrCands_pt);
	tree->SetBranchAddress("tau_signalNeutrHadrCands_eta", &tau_signalNeutrHadrCands_eta);
	tree->SetBranchAddress("tau_signalNeutrHadrCands_phi", &tau_signalNeutrHadrCands_phi);
	tree->SetBranchAddress("tau_signalNeutrHadrCands_E", &tau_signalNeutrHadrCands_E);
	tree->SetBranchAddress("tau_signalGammaCands_pt", &tau_signalGammaCands_pt);
	tree->SetBranchAddress("tau_signalGammaCands_eta", &tau_signalGammaCands_eta);
	tree->SetBranchAddress("tau_signalGammaCands_phi", &tau_signalGammaCands_phi);
	tree->SetBranchAddress("tau_signalGammaCands_E", &tau_signalGammaCands_E);
	
	tree->SetBranchAddress("jet_pt",           &jet_pt);
	tree->SetBranchAddress("jet_eta",          &jet_eta);
	tree->SetBranchAddress("jet_phi",          &jet_phi);
	tree->SetBranchAddress("jet_E",            &jet_E);
	tree->SetBranchAddress("jet_csv",          &jet_csv);
	tree->SetBranchAddress("jet_deepcsv",      &jet_deepcsv);
	tree->SetBranchAddress("jet_deepCvsB",     &jet_deepCvsB);
	tree->SetBranchAddress("jet_deepCvsL",     &jet_deepCvsL);
	tree->SetBranchAddress("jet_csvv2",        &jet_csvv2);
	tree->SetBranchAddress("jet_ctagCvsB",     &jet_ctagCvsB);
	tree->SetBranchAddress("jet_ctagCvsL",     &jet_ctagCvsL);
	tree->SetBranchAddress("jet_flavor",       &jet_flavor);
	tree->SetBranchAddress("jet_qgLikelihood", &jet_qgLikelihood);
	tree->SetBranchAddress("jet_axis2",        &jet_axis2);
	tree->SetBranchAddress("jet_ptD",          &jet_ptD);
	tree->SetBranchAddress("jet_mult",         &jet_mult);
	tree->SetBranchAddress("jet_jesUnc",       &jet_jesUnc);
    tree->SetBranchAddress("jet_smearFactor",  &jet_smearFactor);
    tree->SetBranchAddress("jet_smearFactor_up", &jet_smearFactor_up);
    tree->SetBranchAddress("jet_smearFactor_down", &jet_smearFactor_down);

	tree->SetBranchAddress("PFMET", &PFMET);
	tree->SetBranchAddress("PFMETphi", &PFMETphi);
	tree->SetBranchAddress("METSignificance", &METSignificance);
	tree->SetBranchAddress("METCov00", &METCov00);
	tree->SetBranchAddress("METCov01", &METCov01);
	tree->SetBranchAddress("METCov10", &METCov10);
	tree->SetBranchAddress("METCov11", &METCov11);
	tree->SetBranchAddress("MHT", &MHT);
	tree->SetBranchAddress("metLD", &metLD);
	tree->SetBranchAddress("PFMET_jesup", &PFMET_jesup);
	tree->SetBranchAddress("PFMETphi_jesup", &PFMETphi_jesup);
	tree->SetBranchAddress("PFMET_jesdown", &PFMET_jesdown);
	tree->SetBranchAddress("PFMETphi_jesdown", &PFMETphi_jesdown);
	tree->SetBranchAddress("PFMET_tesup", &PFMET_tesup);
	tree->SetBranchAddress("PFMETphi_tesup", &PFMETphi_tesup);
	tree->SetBranchAddress("PFMET_tesdown", &PFMET_tesdown);
	tree->SetBranchAddress("PFMETphi_tesdown", &PFMETphi_tesdown);

    tree->SetBranchAddress("matchedGenMuons_pt",     &matchedGenMuons_pt);
    tree->SetBranchAddress("matchedGenMuons_eta",    &matchedGenMuons_eta);
    tree->SetBranchAddress("matchedGenMuons_phi",    &matchedGenMuons_phi);
    tree->SetBranchAddress("matchedGenMuons_E",      &matchedGenMuons_E);
    tree->SetBranchAddress("matchedGenMuons_status", &matchedGenMuons_status);
    tree->SetBranchAddress("matchedGenMuons_pdgid",  &matchedGenMuons_pdgid);

    tree->SetBranchAddress("matchedGenElectrons_pt",     &matchedGenElectrons_pt);
    tree->SetBranchAddress("matchedGenElectrons_eta",    &matchedGenElectrons_eta);
    tree->SetBranchAddress("matchedGenElectrons_phi",    &matchedGenElectrons_phi);
    tree->SetBranchAddress("matchedGenElectrons_E",      &matchedGenElectrons_E);
    tree->SetBranchAddress("matchedGenElectrons_status", &matchedGenElectrons_status);
    tree->SetBranchAddress("matchedGenElectrons_pdgid",  &matchedGenElectrons_pdgid);

    tree->SetBranchAddress("matchedGenTaus_pt",     &matchedGenTaus_pt);
    tree->SetBranchAddress("matchedGenTaus_eta",    &matchedGenTaus_eta);
    tree->SetBranchAddress("matchedGenTaus_phi",    &matchedGenTaus_phi);
    tree->SetBranchAddress("matchedGenTaus_E",      &matchedGenTaus_E);
    tree->SetBranchAddress("matchedGenTaus_status", &matchedGenTaus_status);
    tree->SetBranchAddress("matchedGenTaus_pdgid",  &matchedGenTaus_pdgid);

    tree->SetBranchAddress("genjet_pt",  &genjet_pt);
    tree->SetBranchAddress("genjet_eta", &genjet_eta);
    tree->SetBranchAddress("genjet_phi", &genjet_phi);
    tree->SetBranchAddress("genjet_E",   &genjet_E);
}

#endif

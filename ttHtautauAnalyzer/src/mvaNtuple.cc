#ifndef mvaNtuple_cc
#define mvaNtuple_cc

#include "../interface/mvaNtuple.h"

void mvaNtuple::setup_branches(TTree* tree)
{
	tree->Branch("run", &run);
	tree->Branch("lumi", &lumi);
	tree->Branch("nEvent", &nEvent);
	
	tree->Branch("event_weight", &event_weight);
	tree->Branch("xsection_weight", &xsection_weight);
	tree->Branch("xsection_weight_gen", &xsection_weight);

	tree->Branch("pu_weight", &pu_weight);
	tree->Branch("mc_weight", &mc_weight);
	tree->Branch("btag_sf", &btag_sf);
	tree->Branch("lepid_sf", &lepid_sf);
	tree->Branch("tauid_sf", &tauid_sf);
	tree->Branch("hlt_sf", &hlt_sf);

	tree->Branch("isGenMatchedLep", &isGenMatchedLep);
	tree->Branch("isGenMatchedTau", &isGenMatchedTau);
	tree->Branch("HiggsDecayType", &HiggsDecayType);
	tree->Branch("isGenPhotonMatched", &isGenPhotonMatched);
	
	if (dosystematics_) {
		tree->Branch("event_weight_thu_shape_x1Up",&event_weight_thu_shape_x1Up);
		tree->Branch("event_weight_thu_shape_x1Down",&event_weight_thu_shape_x1Down);
		tree->Branch("event_weight_thu_shape_y1Up",&event_weight_thu_shape_y1Up);
		tree->Branch("event_weight_thu_shape_y1Down",&event_weight_thu_shape_y1Down);
		tree->Branch("event_weight_btag_LFUp",&event_weight_btag_LFUp);
		tree->Branch("event_weight_btag_LFDown",&event_weight_btag_LFDown);
		tree->Branch("event_weight_btag_HFUp",&event_weight_btag_HFUp);
		tree->Branch("event_weight_btag_HFDown",&event_weight_btag_HFDown);
		tree->Branch("event_weight_btag_HFStats1Up",&event_weight_btag_HFStats1Up);
		tree->Branch("event_weight_btag_HFStats1Down",&event_weight_btag_HFStats1Down);
		tree->Branch("event_weight_btag_HFStats2Up",&event_weight_btag_HFStats2Up);
		tree->Branch("event_weight_btag_HFStats2Down",&event_weight_btag_HFStats2Down);
		tree->Branch("event_weight_btag_LFStats1Up",&event_weight_btag_LFStats1Up);
		tree->Branch("event_weight_btag_LFStats1Down",&event_weight_btag_LFStats1Down);
		tree->Branch("event_weight_btag_LFStats2Up",&event_weight_btag_LFStats2Up);
		tree->Branch("event_weight_btag_LFStats2Down",&event_weight_btag_LFStats2Down);
		tree->Branch("event_weight_btag_cErr1Up",&event_weight_btag_cErr1Up);
		tree->Branch("event_weight_btag_cErr1Down",&event_weight_btag_cErr1Down);
		tree->Branch("event_weight_btag_cErr2Up",&event_weight_btag_cErr2Up);
		tree->Branch("event_weight_btag_cErr2Down",&event_weight_btag_cErr2Down);
		tree->Branch("event_weight_FRjt_normUp",&event_weight_FRjt_normUp);
		tree->Branch("event_weight_FRjt_normDown",&event_weight_FRjt_normDown);
		tree->Branch("event_weight_FRjt_shapeUp",&event_weight_FRjt_shapeUp);
		tree->Branch("event_weight_FRjt_shapeDown",&event_weight_FRjt_shapeDown);
		tree->Branch("event_weight_FRe_normUp",&event_weight_FRe_normUp);
		tree->Branch("event_weight_FRe_normDown",&event_weight_FRe_normDown);
		tree->Branch("event_weight_FRe_ptUp",&event_weight_FRe_ptUp);
		tree->Branch("event_weight_FRe_ptDown",&event_weight_FRe_ptDown);
		tree->Branch("event_weight_FRe_beUp",&event_weight_FRe_beUp);
		tree->Branch("event_weight_FRe_beDown",&event_weight_FRe_beDown);
		//tree->Branch("event_weight_FRe_bUp",&event_weight_FRe_bUp);
		//tree->Branch("event_weight_FRe_bDown",&event_weight_FRe_bDown);
		//tree->Branch("event_weight_FRe_ecUp",&event_weight_FRe_ecUp);
		//tree->Branch("event_weight_FRe_ecDown",&event_weight_FRe_ecDown);
		tree->Branch("event_weight_FRm_normUp",&event_weight_FRm_normUp);
		tree->Branch("event_weight_FRm_normDown",&event_weight_FRm_normDown);
		tree->Branch("event_weight_FRm_ptUp",&event_weight_FRm_ptUp);
		tree->Branch("event_weight_FRm_ptDown",&event_weight_FRm_ptDown);
		tree->Branch("event_weight_FRm_beUp",&event_weight_FRm_beUp);
		tree->Branch("event_weight_FRm_beDown",&event_weight_FRm_beDown);
		//tree->Branch("event_weight_FRm_bUp",&event_weight_FRm_bUp);
		//tree->Branch("event_weight_FRm_bDown",&event_weight_FRm_bDown);
		//tree->Branch("event_weight_FRm_ecUp",&event_weight_FRm_ecUp);
		//tree->Branch("event_weight_FRm_ecDown",&event_weight_FRm_ecDown);
		tree->Branch("event_weight_triggerUp", &event_weight_triggerUp);
		tree->Branch("event_weight_triggerDown", &event_weight_triggerDown);
		tree->Branch("event_weight_lepEff_mulooseUp",&event_weight_lepEff_mulooseUp);
		tree->Branch("event_weight_lepEff_mulooseDown",&event_weight_lepEff_mulooseDown);
		tree->Branch("event_weight_lepEff_ellooseUp",&event_weight_lepEff_ellooseUp);
		tree->Branch("event_weight_lepEff_ellooseDown",&event_weight_lepEff_ellooseDown);
		tree->Branch("event_weight_lepEff_mutightUp",&event_weight_lepEff_mutightUp);
		tree->Branch("event_weight_lepEff_mutightDown",&event_weight_lepEff_mutightDown);
		tree->Branch("event_weight_lepEff_eltightUp",&event_weight_lepEff_eltightUp);
		tree->Branch("event_weight_lepEff_eltightDown",&event_weight_lepEff_eltightDown);	
		tree->Branch("event_weight_FR_TT", &event_weight_FR_TT);
		tree->Branch("event_weight_FR_QCD", &event_weight_FR_QCD);
		tree->Branch("event_weight_FR_el_QCD_mu_TT", &event_weight_FR_el_QCD_mu_TT);
		tree->Branch("event_weight_FR_el_TT_mu_QCD", &event_weight_FR_el_TT_mu_QCD);
	}		

	// generic variables
	tree->Branch("nEle", &nEle);
	tree->Branch("nMu", &nMu);
	tree->Branch("nTau", &nTau);
	tree->Branch("nJet", &nJet);
	tree->Branch("nBTags_loose", &nbtags_loose);
	tree->Branch("nBTags_medium", &nbtags_medium);
	tree->Branch("ptmiss", &met);
	tree->Branch("metLD", &metLD);

	tree->Branch("mvaOutput", &mva_output);

    // kinematics and ID flags
    // electrons and muons
    tree->Branch("lep1_pt", &lep0_pt);
    tree->Branch("lep1_conePt", &lep0_conept);
    tree->Branch("lep1_eta", &lep0_eta);
    tree->Branch("lep1_phi", &lep0_phi);
    tree->Branch("lep1_E", &lep0_E);
    tree->Branch("lep1_charge", &lep0_charge);
    tree->Branch("lep1_pdgId", &lep0_pdgId);
    tree->Branch("lep1_isTight", &lep0_isTight);
    tree->Branch("lep1_isGenMatched", &lep0_isGenMatched);
    tree->Branch("lep2_pt", &lep1_pt);
    tree->Branch("lep2_conePt", &lep1_conept);
    tree->Branch("lep2_eta", &lep1_eta);
    tree->Branch("lep2_phi", &lep1_phi);
    tree->Branch("lep2_E", &lep1_E);
    tree->Branch("lep2_charge", &lep1_charge);
    tree->Branch("lep2_pdgId", &lep1_pdgId);
    tree->Branch("lep2_isTight", &lep1_isTight);
    tree->Branch("lep2_isGenMatched", &lep1_isGenMatched);
    tree->Branch("lep3_pt", &lep2_pt);
    tree->Branch("lep3_conePt", &lep2_conept);
    tree->Branch("lep3_eta", &lep2_eta);
    tree->Branch("lep3_phi", &lep2_phi);
    tree->Branch("lep3_E", &lep2_E);
    tree->Branch("lep3_charge", &lep2_charge);
    tree->Branch("lep3_pdgId", &lep2_pdgId);
    tree->Branch("lep3_isTight", &lep2_isTight);
    tree->Branch("lep3_isGenMatched", &lep2_isGenMatched);

    tree->Branch("mll", &mll);

    // taus
    tree->Branch("tau1_pt", &tau0_pt);
    tree->Branch("tau1_eta", &tau0_eta);
    tree->Branch("tau1_phi", &tau0_phi);
    tree->Branch("tau1_E", &tau0_E);
    tree->Branch("tau1_charge", &tau0_charge);
    tree->Branch("tau1_isTight", &tau0_isTight);
    tree->Branch("tau1_isGenMatched", &tau0_isGenMatched);
    tree->Branch("tau2_pt", &tau1_pt);
    tree->Branch("tau2_eta", &tau1_eta);
    tree->Branch("tau2_phi", &tau1_phi);
    tree->Branch("tau2_E", &tau1_E);
    tree->Branch("tau2_charge", &tau1_charge);
    tree->Branch("tau2_isTight", &tau1_isTight);
    tree->Branch("tau2_isGenMatched", &tau1_isGenMatched);
    // tau decays
    tree->Branch("tau1_decaymode", &tau0_decaymode);
    tree->Branch("tau2_decaymode", &tau1_decaymode);
    tree->Branch("tau1_ldgtrkpt", &tau0_ldgtrkpt);
    tree->Branch("tau2_ldgtrkpt", &tau1_ldgtrkpt);
    tree->Branch("tau1_ldgtrkE", &tau0_ldgtrkE);
    tree->Branch("tau2_ldgtrkE", &tau1_ldgtrkE);
    tree->Branch("tau1_chargedE", &tau0_chargedE);
    tree->Branch("tau2_chargedE", &tau1_chargedE);
    tree->Branch("tau1_neutralE", &tau0_neutralE);
    tree->Branch("tau2_neutralE", &tau1_neutralE);
    tree->Branch("tau1_chargedE_ss", &tau0_chargedE_ss);
    tree->Branch("tau2_chargedE_ss", &tau1_chargedE_ss);

    // jets
    tree->Branch("jet1_pt", &jet0_pt);
    tree->Branch("jet2_pt", &jet1_pt);
    tree->Branch("jet3_pt", &jet2_pt);
    tree->Branch("jet4_pt", &jet3_pt);
    tree->Branch("jet1_eta", &jet0_eta);
    tree->Branch("jet2_eta", &jet1_eta);
    tree->Branch("jet3_eta", &jet2_eta);
    tree->Branch("jet4_eta", &jet3_eta);
    tree->Branch("jet1_phi", &jet0_phi);
    tree->Branch("jet2_phi", &jet1_phi);
    tree->Branch("jet3_phi", &jet2_phi);
    tree->Branch("jet4_phi", &jet3_phi);
    tree->Branch("jet1_E", &jet0_E);
    tree->Branch("jet2_E", &jet1_E);
    tree->Branch("jet3_E", &jet2_E);
    tree->Branch("jet4_E", &jet3_E);

    // MVA variables
	if (anatype_ == Analyze_2lss1tau) {
		if (version_=="2016") { // 2016 analysis variables
			tree->Branch("mindr_lep0_jet", &mindr_lep0_jet);
			tree->Branch("mindr_lep1_jet", &mindr_lep1_jet);		
			tree->Branch("max_lep_eta", &max_lep_eta);
			tree->Branch("met", &met);
			tree->Branch("mht", &mht);
			tree->Branch("mT_met_lep0", &mT_met_lep0);
			tree->Branch("lep0_conept", &lep0_conept);
			tree->Branch("lep1_conept", &lep1_conept);
			tree->Branch("dr_leps", &dr_leps);
			tree->Branch("tau_pt", &tau0_pt);
			tree->Branch("dr_lep0_tau", &dr_lep0_tau);
			tree->Branch("dr_lep1_tau", &dr_lep1_tau);
			tree->Branch("mvis_lep0_tau", &mvis_lep0_tau);
			tree->Branch("mvis_lep1_tau", &mvis_lep1_tau);
			tree->Branch("tau0_decaymode", &tau0_decaymode);
			tree->Branch("tau0_E", &tau0_E);
			tree->Branch("tau0_easym", &tau0_easym);
			tree->Branch("avg_dr_jet", &avg_dr_jet);
		}
		else if (version_=="2017") {
			tree->Branch("avg_dr_jet", &avg_dr_jet);
			tree->Branch("dr_lep1_tau", &dr_lep0_tau);
			tree->Branch("dr_lep2_tau", &dr_lep1_tau);
			tree->Branch("dr_leps", &dr_leps);
			tree->Branch("mT_lep1", &mT_met_lep0);
			tree->Branch("mT_lep2", &mT_met_lep1);
			tree->Branch("mTauTauVis1", &mvis_lep0_tau);
			tree->Branch("mTauTauVis2", &mvis_lep1_tau);
			tree->Branch("max_lep_eta", &max_lep_eta);
			tree->Branch("mbb", &mbb);
			tree->Branch("mindr_lep1_jet", &mindr_lep0_jet);
			tree->Branch("mindr_lep2_jet", &mindr_lep1_jet);
			tree->Branch("mindr_tau_jet", &mindr_tau0_jet);
			tree->Branch("Hj_tagger", &Hj_tagger);
			tree->Branch("HTT", &HTT);
			tree->Branch("HadTop_pt", &HadTop_pt);
			tree->Branch("Hj_tagger", &Hj_tagger);
			tree->Branch("memOutput_LR", &mem_LR);

			tree->Branch("mvaOutput_2lss_1tau_plainKin_ttV", &mva_2lss1tau_BDT1);
			tree->Branch("mvaOutput_2lss_1tau_plainKin_ttbar", &mva_2lss1tau_BDT2);
			tree->Branch("mvaOutput_2lss_1tau_plainKin_SUM_M", &mva_2lss1tau_BDT3);
			tree->Branch("mvaOutput_2lss_1tau_HTT_SUM_M", &mva_2lss1tau_BDT4);
			tree->Branch("mvaOutput_2lss_1tau_HTTMEM_SUM_M", &mva_2lss1tau_BDT5);
			tree->Branch("mvaOutput_2lss_1tau_plainKin_1B_M", &mva_2lss1tau_BDT6);
		}
		else {
			std::cout << "WARNING: Version " << version_
					  << " for 2lss1tau category is unknown. Ntuple will be empty."
					  << std::endl;
		}
	}
	else if (anatype_ == Analyze_1l2tau) {
		if (version_=="2016") { // 2016 analysis variables
			tree->Branch("ht", &HT);
			tree->Branch("tt_deltaR", &dr_taus);
			tree->Branch("tt_mvis", &mTauTauVis);
			tree->Branch("tt_sumpt", &tt_pt);
			tree->Branch("max_dr_jet", &max_dr_jet);
			tree->Branch("tau0_pt", &tau0_pt);
			tree->Branch("tau1_pt", &tau1_pt);
			tree->Branch("nbtags", &nbtags_medium);
			tree->Branch("nbtags_loose", &nbtags_loose);
			tree->Branch("avg_dr_jet", &avg_dr_jet);
		}
		else if (version_=="2017") {
			tree->Branch("avg_dr_jet", &avg_dr_jet);
			tree->Branch("dr_taus", &dr_taus);
			tree->Branch("mT_lep", &mT_met_lep0);
			tree->Branch("mTauTauVis", &mTauTauVis);
			tree->Branch("mindr_lep_jet", &mindr_lep0_jet);
			tree->Branch("mindr_tau1_jet", &mindr_tau0_jet);
			tree->Branch("mindr_tau2_jet", &mindr_tau1_jet);
			tree->Branch("dr_lep_tau_lead", &dr_lep_tau_lead);
			tree->Branch("nBjetLoose", &nbtags_loose);
			tree->Branch("dr_lep_tau_ss", &dr_lep_tau_ss);
			tree->Branch("costS_tau", &costS_tau);
			tree->Branch("HTT", &HTT);
			tree->Branch("HadTop_pt", &HadTop_pt);
			
			tree->Branch("mvaOutput_plainKin_ttbar", &mva_1l2tau_BDT1);
			tree->Branch("mvaOutput_1l_2tau_HTT_SUM_VT", &mva_1l2tau_BDT2);
			
			/*
			tree->Branch("tau0_tightWP", &tau0_tightWP);
			tree->Branch("tau1_tightWP", &tau1_tightWP);
			tree->Branch("tau0_ldgtrkpt", &tau0_ldgtrkpt);
			tree->Branch("tau1_ldgtrkpt", &tau1_ldgtrkpt);
			tree->Branch("tau0_ldgtrkE", &tau0_ldgtrkE);
			tree->Branch("tau1_ldgtrkE", &tau1_ldgtrkE);
			
			tree->Branch("taup_decaymode", &taup_decaymode);
			tree->Branch("taum_decaymode", &taum_decaymode);
			tree->Branch("taup_easym", &taup_easym);
			tree->Branch("taum_easym", &taum_easym);
			tree->Branch("taup_cosPsi", &taup_cosPsi);
			tree->Branch("taum_cosPsi", &taum_cosPsi);
			tree->Branch("evisTaus_diff", &evisTaus_diff);
			tree->Branch("evisTaus_sum", &evisTaus_sum);
			tree->Branch("evisTausAsym", &evisTausAsym);
			
			tree->Branch("taup_pt", &taup_pt);
			tree->Branch("taum_pt", &taum_pt);
			tree->Branch("taup_ldgtrkpt", &taup_ldgtrkpt);
			tree->Branch("taum_ldgtrkpt", &taum_ldgtrkpt);
			tree->Branch("taup_ldgtrkE", &taup_ldgtrkE);
			tree->Branch("taum_ldgtrkE", &taum_ldgtrkE);
			tree->Branch("taup_tightWP", &taup_tightWP);
			tree->Branch("taum_tightWP", &taum_tightWP);
			tree->Branch("taup_upsilon", &taup_upsilon);
			tree->Branch("taum_upsilon", &taum_upsilon);
			*/
		}
		else if (version_=="test") {
			tree->Branch("taup_decaymode", &taup_decaymode);
			tree->Branch("taum_decaymode", &taum_decaymode);
			tree->Branch("taup_easym", &taup_easym);
			tree->Branch("taum_easym", &taum_easym);
			tree->Branch("taup_cosPsi", &taup_cosPsi);
			tree->Branch("taum_cosPsi", &taum_cosPsi);
			tree->Branch("evisTaus_diff", &evisTaus_diff);
			tree->Branch("evisTaus_sum", &evisTaus_sum);
			tree->Branch("evisTausAsym", &evisTausAsym);
			
			tree->Branch("pp1_pp1", &pp1_pp1);
			tree->Branch("pp1_pp2", &pp1_pp2);
			tree->Branch("pp1_pp3", &pp1_pp3);
			tree->Branch("pp1_pm1", &pp1_pm1);
			tree->Branch("pp1_pm2", &pp1_pm2);
			tree->Branch("pp1_pm3", &pp1_pm3);
			tree->Branch("pp2_pp2", &pp2_pp2);
			tree->Branch("pp2_pp3", &pp2_pp3);
			tree->Branch("pp2_pm1", &pp2_pm1);
			tree->Branch("pp2_pm2", &pp2_pm2);
			tree->Branch("pp2_pm3", &pp2_pm3);
			tree->Branch("pp3_pp3", &pp3_pp3);
			tree->Branch("pp3_pm1", &pp3_pm1);
			tree->Branch("pp3_pm2", &pp3_pm2);
			tree->Branch("pp3_pm3", &pp3_pm3);
			tree->Branch("pm1_pm1", &pm1_pm1);
			tree->Branch("pm1_pm2", &pm1_pm2);
			tree->Branch("pm1_pm3", &pm1_pm3);
			tree->Branch("pm2_pm2", &pm2_pm2);
			tree->Branch("pm2_pm3", &pm2_pm3);
			tree->Branch("pm3_pm3", &pm3_pm3);
		}
		else {
			std::cout << "WARNING: Version " << version_
					  << " for 1l2tau category is unknown. Ntuple will be empty."
					  << std::endl;
		}
	}
	else if (anatype_ == Analyze_3l1tau) {
		if (version_=="2016") {
			tree->Branch("max_lep_eta", &max_lep_eta);
			tree->Branch("mindr_lep0_jet", &mindr_lep0_jet);
			tree->Branch("mindr_lep1_jet", &mindr_lep1_jet);
			tree->Branch("mindr_lep2_jet", &mindr_lep2_jet);
			tree->Branch("mT_met_lep0", &mT_met_lep0);
			tree->Branch("lep0_conept", &lep0_conept);
			tree->Branch("lep1_conept", &lep1_conept);
			tree->Branch("lep2_conept", &lep2_conept);
			tree->Branch("avg_dr_jet", &avg_dr_jet);
			//tree->Branch("tau0_decaymode", &tau0_decaymode);
			//tree->Branch("tau0_E", &tau0_E);
			//tree->Branch("tau0_easym", &tau0_easym);
		}
		else if (version_=="2017") {
			tree->Branch("mTauTauVis1", &mvis_lep0_tau);
			tree->Branch("mTauTauVis2", &mvis_lep1_tau);
			tree->Branch("max_lep_eta", &max_lep_eta);
			tree->Branch("dr_leps", &dr_leps);
			tree->Branch("mindr_lep1_jet", &mindr_lep0_jet);
			tree->Branch("mindr_lep2_jet", &mindr_lep1_jet);
			tree->Branch("mT_lep1", &mT_met_lep0);
			tree->Branch("mT_lep2", &mT_met_lep1);
			tree->Branch("avg_dr_jet", &avg_dr_jet);
			tree->Branch("mindr_tau_jet", &mindr_tau0_jet);
			tree->Branch("mbb_loose", &mbb);
			tree->Branch("mindr_lep3_jet", &mindr_lep2_jet);

			tree->Branch("mvaOutput_plainKin_ttV", &mva_3l1tau_BDT1);
			tree->Branch("mvaOutput_plainKin_ttbar", &mva_3l1tau_BDT2);
			tree->Branch("mvaOutput_3l_1tau_plainKin_SUM_M", &mva_3l1tau_BDT3);
			tree->Branch("mvaOutput_3l_1tau_plainKin_1B_M", &mva_3l1tau_BDT4);
		}
		else {
			std::cout << "WARNING: Version " << version_
					  << " for 3l1tau category is unknown. Ntuple will be empty."
					  << std::endl;
		}
	}
	else if (anatype_ == Analyze_2l2tau) {
		if (version_=="2017") {
			tree->Branch("mTauTauVis", &mTauTauVis);
			tree->Branch("cosThetaS_hadTau", &costS_tau);
			tree->Branch("is_OS", &is_OS);
			tree->Branch("min_dr_lep_jet", &min_dr_lep_jet);
			tree->Branch("mindr_tau_jet", &mindr_tau_jet);
			tree->Branch("mT_lep1", &mT_met_lep0);
			tree->Branch("mT_lep2", &mT_met_lep1);
			tree->Branch("mindr_lep1_jet", &mindr_lep0_jet);
			tree->Branch("max_dr_lep_tau", &max_dr_lep_tau);
			tree->Branch("nBJetLoose", &nbtags_loose);
			tree->Branch("dr_taus", &dr_taus);
			tree->Branch("avg_dr_jet", &avg_dr_jet);
			tree->Branch("min_dr_lep_tau", &min_dr_lep_tau);
			tree->Branch("mindr_tau1_jet", &mindr_tau0_jet);
			tree->Branch("avg_dr_lep_tau", &avg_dr_lep_tau);

			tree->Branch("mvaOutput_plainKin_ttV", &mva_2l2tau_BDT1);
			tree->Branch("mvaOutput_plainKin_ttbar", &mva_2l2tau_BDT2);
			tree->Branch("mvaOutput_2l_2tau_plainKin_SUM_VT", &mva_2l2tau_BDT3);
			tree->Branch("mvaOutput_2l_2tau_plainKin_1B_VT", &mva_2l2tau_BDT4);
		}
		else {
			std::cout << "WARNING: Version " << version_
					  << " for 2l2tau category is unknown. Ntuple will be empty."
					  << std::endl;
		}
	}
}

void mvaNtuple::get_object_kinematics(const std::vector<miniLepton>& leptons,
                                      const std::vector<miniTau>& taus,
                                      const std::vector<TLorentzVector>& jets)
{
	assert(leptons.size()>0);
	lep0_conept = leptons[0].conept();
    lep0_pt = leptons[0].pt();
	lep0_eta = leptons[0].eta();
	lep0_phi = leptons[0].phi();
	lep0_E = leptons[0].p4().E();
    lep0_charge = leptons[0].charge();
	lep0_pdgId = leptons[0].pdgId();
	lep0_isTight = leptons[0].passTightSel();
    lep0_isGenMatched = leptons[0].isGenMatched();
	if (leptons.size() > 1) {
		lep1_conept = leptons[1].conept();
        lep1_pt = leptons[1].pt();
		lep1_eta = leptons[1].eta();
		lep1_phi = leptons[1].phi();
		lep1_E = leptons[1].p4().E();
        lep1_charge = leptons[1].charge();
		lep1_pdgId = leptons[1].pdgId();
		lep1_isTight = leptons[1].passTightSel();
        lep1_isGenMatched = leptons[1].isGenMatched();
	}
	if (leptons.size() > 2) {
		lep2_conept = leptons[2].conept();
        lep2_pt = leptons[2].pt();
		lep2_eta = leptons[2].eta();
		lep2_phi = leptons[2].phi();
		lep2_E = leptons[2].p4().E();
        lep2_charge = leptons[2].charge();
		lep2_pdgId = leptons[2].pdgId();
		lep2_isTight = leptons[2].passTightSel();
        lep2_isGenMatched = leptons[2].isGenMatched();
	}

	if (taus.size()>0) {
		tau0_pt = taus[0].pt();
		tau0_eta = taus[0].eta();
		tau0_phi = taus[0].phi();
		tau0_E = taus[0].p4().E();
        tau0_charge = taus[0].charge();
		tau0_isTight = taus[0].passTightSel();
        tau0_isGenMatched = taus[0].isGenMatched();
	}
	if (taus.size()>1) {
		tau1_pt = taus[1].pt();
		tau1_eta = taus[1].eta();
		tau1_phi = taus[1].phi();
		tau1_E = taus[1].p4().E();
        tau1_charge = taus[1].charge();
		tau1_isTight = taus[1].passTightSel();
        tau1_isGenMatched = taus[1].isGenMatched();
	}

    if (jets.size()>0) {
      jet0_pt = jets[0].Pt();
      jet0_eta = jets[0].Eta();
      jet0_phi = jets[0].Phi();
      jet0_E = jets[0].E();
    }
    if (jets.size()>1) {
      jet1_pt = jets[1].Pt();
      jet1_eta = jets[1].Eta();
      jet1_phi = jets[1].Phi();
      jet1_E = jets[1].E();
    }
    if (jets.size()>2) {
      jet2_pt = jets[2].Pt();
      jet2_eta = jets[2].Eta();
      jet2_phi = jets[2].Phi();
      jet2_E = jets[2].E();
    }
    if (jets.size()>3) {
      jet3_pt = jets[3].Pt();
      jet3_eta = jets[3].Eta();
      jet3_phi = jets[3].Phi();
      jet3_E = jets[3].E();
    }
}

int mvaNtuple::count_leptons(const std::vector<miniLepton>& leptons, int pdgid)
{
	int count = 0;
	for (const auto & lep : leptons) {
		if (abs(lep.pdgId())==abs(pdgid)) count++;
	}
	return count;
}

int mvaNtuple::count_electrons(const std::vector<miniLepton>& leptons)
{
	return count_leptons(leptons, 11);
}

int mvaNtuple::count_muons(const std::vector<miniLepton>& leptons)
{
	return count_leptons(leptons, 13);
}

void mvaNtuple::compute_all_variables(const std::vector<miniLepton>& leptons,
									  const std::vector<miniTau>& taus,
									  const std::vector<miniJet>& jets,
									  float MET, float METphi, float MHT,
									  int nbtagsloose, int nbtagsmedium)
{
	compute_mva_variables(leptons, taus, jets, MET, METphi, MHT, nbtagsloose,
						  nbtagsmedium);

	if (control_) return;  // for now
	
	if (doHTT_ and (anatype_==Analyze_1l2tau or anatype_==Analyze_2lss1tau))
		compute_HTT(jets);
	if (evalMVA_) evaluate_BDTs();
}

void mvaNtuple::compute_mva_variables(const std::vector<miniLepton>& leptons,
									  const std::vector<miniTau>& taus,
									  const std::vector<TLorentzVector>& jets,
									  float MET, float METphi, float MHT,
									  int nbtagsloose, int nbtagsmedium)
{
	assert(version_=="2016");
	std::vector<TLorentzVector> dummy;
	compute_mva_variables(leptons, taus, jets, MET, METphi, MHT, nbtagsloose,
						  nbtagsmedium, dummy);
}

void mvaNtuple::compute_mva_variables(const std::vector<miniLepton>& leptons,
									  const std::vector<miniTau>& taus,
									  const std::vector<miniJet>& jets,
									  float MET, float METphi, float MHT,
									  int nbtagsloose, int nbtagsmedium)
{
	std::vector<TLorentzVector> jetsp4;
	for (const auto & jet : jets) {
		jetsp4.push_back(jet.p4());
	}

	std::vector<miniJet> jets_csv = jets;
	// sort by csv 
	std::sort(jets_csv.begin(),jets_csv.end(), [] (miniJet& j1, miniJet& j2) {return j1.csv() > j2.csv();});
	assert(jets_csv.size()>=2);
	std::vector<TLorentzVector> bjetsp4 = {jets_csv[0].p4(), jets_csv[1].p4()};

	compute_mva_variables(leptons, taus, jetsp4, MET, METphi, MHT, nbtagsloose,
						  nbtagsmedium, bjetsp4);
}

void mvaNtuple::compute_mva_variables(const std::vector<miniLepton>& leptons,
									  const std::vector<miniTau>& taus,
									  const std::vector<TLorentzVector>& jets,
									  float MET, float METphi, float MHT,
									  int nbtagsloose, int nbtagsmedium,
									  const std::vector<TLorentzVector>& bjets)
{
	nEle =  count_electrons(leptons);
	nMu = count_muons(leptons);
	nTau = taus.size();
	nJet = jets.size();
	nbtags_medium = nbtagsloose;
	nbtags_loose = nbtagsmedium;
	met = MET;
	mht = MHT;
	metLD = 0.00397 * MET + 0.00265 * MHT;

    get_object_kinematics(leptons, taus, jets);
	mll = compute_mll(leptons);

    // tau decay variables
    compute_tauDecay_variables(taus);
	
	// common variables in all categories
	avg_dr_jet = compute_average_dr(jets);

    // For TTW, TTZ, WZ control regions, some variables below are not well defined
    // FIXME?
	if (control_) return; 
	
	switch(anatype_){
	case Analyze_1l2tau:
		assert(leptons.size()>0);
		assert(taus.size()>1);
		
		if (version_=="2016") {
			HT = mht;  // FIXME
			dr_taus = taus[0].p4().DeltaR(taus[1].p4());;
			mTauTauVis = (taus[0].p4()+taus[1].p4()).M();
			tt_pt = (taus[0].p4()+taus[1].p4()).Pt();
			max_dr_jet = compute_max_dr(jets);
		}
		else if (version_=="2017") {
			costS_tau = compute_cosThetaS(taus[0].p4());
			dr_taus = taus[0].p4().DeltaR(taus[1].p4());
			mT_met_lep0 = compute_mT_lep(leptons[0], MET, METphi);
			mTauTauVis = (taus[0].p4()+taus[1].p4()).M();
			mindr_lep0_jet = compute_min_dr(leptons[0].p4(),jets);
			mindr_tau0_jet = compute_min_dr(taus[0].p4(),jets);
			mindr_tau1_jet = compute_min_dr(taus[1].p4(),jets);		
			dr_lep_tau_lead = leptons[0].p4().DeltaR(taus[0].p4());
			dr_lep_tau_sublead = leptons[0].p4().DeltaR(taus[1].p4());
			dr_lep_tau_ss = (leptons[0].charge()==taus[0].charge()) ? dr_lep_tau_lead : dr_lep_tau_sublead;
		}
		else if (version_=="test") {
			compute_tauDecay_variables(taus, true);
		}
		break;
	case Analyze_2lss1tau:
		assert(leptons.size()>1);
		assert(taus.size()>0);
		
		if (version_=="2016") {
			mindr_lep0_jet = compute_min_dr(leptons[0].p4(),jets);
			mindr_lep1_jet = compute_min_dr(leptons[1].p4(),jets);
			max_lep_eta = compute_max_lep_eta(leptons);
			mT_met_lep0 = compute_mT_lep(leptons[0], MET, METphi);
			dr_leps = leptons[0].p4().DeltaR(leptons[1].p4());
			dr_lep0_tau = leptons[0].p4().DeltaR(taus[0].p4());
			dr_lep1_tau = leptons[1].p4().DeltaR(taus[0].p4());
			mvis_lep0_tau = (leptons[0].p4()+taus[0].p4()).M();
			mvis_lep1_tau = (leptons[1].p4()+taus[0].p4()).M();
			tau0_decaymode = taus[0].decaymode();
			tau0_easym = compute_upsilon(taus[0]);
		}
		else if (version_=="2017") {
			dr_lep0_tau = leptons[0].p4().DeltaR(taus[0].p4());
			dr_lep1_tau = leptons[1].p4().DeltaR(taus[0].p4());
			dr_leps = leptons[0].p4().DeltaR(leptons[1].p4());
			mT_met_lep0 = compute_mT_lep(leptons[0], MET, METphi);
			mT_met_lep1 = compute_mT_lep(leptons[1], MET, METphi);
			mvis_lep0_tau = (leptons[0].p4()+taus[0].p4()).M();
			mvis_lep1_tau = (leptons[1].p4()+taus[0].p4()).M();
			max_lep_eta = compute_max_lep_eta(leptons);
			mbb = nbtagsmedium>1 ? (bjets[0]+bjets[1]).M() : -1.;
			mindr_lep0_jet = compute_min_dr(leptons[0].p4(),jets);
			mindr_lep1_jet = compute_min_dr(leptons[1].p4(),jets);
			mindr_tau0_jet = compute_min_dr(taus[0].p4(),jets);
		}
		break;
	case Analyze_3l1tau:
		assert(leptons.size()>2);
		assert(taus.size()>0);
		assert(nbtagsloose>1 or nbtagsmedium>0);
		
		if (version_=="2016") {
			max_lep_eta = compute_max_lep_eta(leptons);
			mindr_lep0_jet = compute_min_dr(leptons[0].p4(),jets);
			mindr_lep1_jet = compute_min_dr(leptons[1].p4(),jets);
			mindr_lep2_jet = compute_min_dr(leptons[2].p4(),jets);
			mT_met_lep0 = compute_mT_lep(leptons[0], MET, METphi);
		}
		else if (version_=="2017") {
			// index of two leading leptons OS with tau
			std::vector<int> ioslep;
			for (int i = 0; i < 3; ++i) {
				if (leptons[i].charge()*taus[0].charge()<0) {
					ioslep.push_back(i);
				}
			}

			if (ioslep.size()>0)
				mvis_lep0_tau = (leptons.at(ioslep[0]).p4()+taus[0].p4()).M();
			else
				mvis_lep0_tau = -1.;

			if (ioslep.size()>1)
				mvis_lep1_tau = (leptons.at(ioslep[1]).p4()+taus[0].p4()).M();
			else
				mvis_lep1_tau = -1.;
			//
			max_lep_eta = compute_max_lep_eta(leptons);
			dr_leps = leptons[0].p4().DeltaR(leptons[1].p4());
			mindr_lep0_jet = compute_min_dr(leptons[0].p4(),jets);
			mindr_lep1_jet = compute_min_dr(leptons[1].p4(),jets);
			mT_met_lep0 = compute_mT_lep(leptons[0], MET, METphi);
			mT_met_lep1 = compute_mT_lep(leptons[1], MET, METphi);
			mindr_tau0_jet = compute_min_dr(taus[0].p4(),jets);;
			mbb = (nbtagsloose>1) ? (bjets[0]+bjets[1]).M() : -1.;
			mindr_lep2_jet = compute_min_dr(leptons[2].p4(),jets);
		}
		break;
	case Analyze_2l2tau:
		assert(taus.size()>1);
		assert(leptons.size()>1);
		if (version_=="2017") {
			mTauTauVis = (taus[0].p4()+taus[1].p4()).M();
			costS_tau = compute_cosThetaS(taus[0].p4());
			is_OS = (taus[0].charge() * taus[1].charge() < 0);  // double check this
			min_dr_lep_jet = std::min(compute_min_dr(leptons[0].p4(),jets),
									  compute_min_dr(leptons[1].p4(),jets));
			mindr_tau_jet = std::min(compute_min_dr(taus[0].p4(),jets),
									 compute_min_dr(taus[1].p4(),jets));
			mT_met_lep0 = compute_mT_lep(leptons[0], MET, METphi);
			mT_met_lep1 = compute_mT_lep(leptons[1], MET, METphi);
			mindr_lep0_jet = compute_min_dr(leptons[0].p4(),jets);
			max_dr_lep_tau = compute_max_dr({leptons[0].p4(),leptons[1].p4()},
											{taus[0].p4(),taus[1].p4()});
			dr_taus = taus[0].p4().DeltaR(taus[1].p4());
			min_dr_lep_tau = compute_min_dr({leptons[0].p4(),leptons[1].p4()},
											{taus[0].p4(),taus[1].p4()});
			mindr_tau0_jet = compute_min_dr(taus[0].p4(),jets);
			avg_dr_lep_tau = compute_average_dr({leptons[0].p4(),leptons[1].p4()},
												{taus[0].p4(),taus[1].p4()});
		}
		break;
	default:
		std::cout << "WARNING analysis type is not supported. Nothing is computed."
				  << std::endl;
		break;
	}
}

void mvaNtuple::compute_tauDecay_variables(const std::vector<miniTau>& taus,bool test)
{
  if (taus.size()>0) {
    tau0_decaymode = taus[0].decaymode();
    tau0_ldgtrkpt = taus[0].leadtrackP4().Pt();
    tau0_ldgtrkE = taus[0].leadtrackP4().Energy();
    tau0_chargedE = taus[0].chargedDaughtersP4().Energy();
    tau0_neutralE = taus[0].neutralDaughtersP4().Energy();
    tau0_chargedE_ss = taus[0].sameSignChargedDaughtersP4().Energy();
  }
  if (taus.size()>1) {
    tau1_decaymode = taus[1].decaymode();
    tau1_ldgtrkpt = taus[1].leadtrackP4().Pt();
    tau1_ldgtrkE = taus[1].leadtrackP4().Energy();
    tau1_chargedE = taus[1].chargedDaughtersP4().Energy();
    tau1_neutralE = taus[1].neutralDaughtersP4().Energy();
    tau1_chargedE_ss = taus[1].sameSignChargedDaughtersP4().Energy();
  }
  
  /*
	assert(taus.size()>1);
	//assert(taus[0].charge()!=taus[1].charge());
	int ip, im;
	if (taus[0].charge()>0) {ip=0; im=1;}
	else {ip=1; im=0;}
	
	taup_decaymode = taus[ip].decaymode();
	taum_decaymode = taus[im].decaymode();
	taup_easym = compute_upsilon(taus[ip]);
	taum_easym = compute_upsilon(taus[im]);
	taup_cosPsi = compute_cosPsi(taus[ip]);
	taum_cosPsi= compute_cosPsi(taus[im]);
	evisTaus_diff = (taus[im].p4().E()-taus[ip].p4().E());
	evisTaus_sum = (taus[im].p4()+taus[ip].p4()).E();
	evisTausAsym = evisTaus_diff / evisTaus_sum;
	
	taup_tightWP = taus[ip].passTightSel();
	taum_tightWP = taus[im].passTightSel();
	taup_pt = taus[ip].pt();
	taum_pt = taus[im].pt();
	taup_ldgtrkpt = taus[ip].leadtrackP4().Pt();
	taum_ldgtrkpt = taus[im].leadtrackP4().Pt();
	taup_ldgtrkE = taus[ip].leadtrackP4().Energy();
	taum_ldgtrkE = taus[im].leadtrackP4().Energy();
	taup_upsilon = compute_upsilon(taus[ip]);
	taum_upsilon = compute_upsilon(taus[im]);
  */
  /*
	if (test) {
		// LorentzVectors of tau decay products
		TLorentzVector pp1, pp2, pp3;
		TLorentzVector pm1, pm2, pm3;

		std::vector<TLorentzVector> taup_chargedHadrons =
			taus[ip].get_signalChargedHadrCands();
		std::vector<TLorentzVector> taum_chargedHadrons =
			taus[im].get_signalChargedHadrCands();
		
		assert(taup_chargedHadrons.size()>0);
		assert(taum_chargedHadrons.size()>0);
		
		if (taup_decaymode == 0) { // 1prong
			pp1 = taup_chargedHadrons[0];
		}
		else if (taup_decaymode == 1) { // 1prong1pi0
			pp1 = taup_chargedHadrons[0];
			pp2 = taus[ip].p4()-taup_chargedHadrons[0];
		}
		else if (taup_decaymode == 10) { // 3prongs
			assert(taup_chargedHadrons.size()>2);
			pp1 = taup_chargedHadrons[0];
			pp2 = taup_chargedHadrons[1];
			pp3 = taup_chargedHadrons[2];
		}

		if (taum_decaymode == 0) { // 1prong
			pm1 = taum_chargedHadrons[0];
		}
		else if (taum_decaymode == 1) { // 1prong1pi0
			pm1 = taum_chargedHadrons[0];
			pm2 = taus[im].p4()-taum_chargedHadrons[0];
		}
		else if (taum_decaymode == 10) { // 3prongs
			assert(taum_chargedHadrons.size()>2);
			pm1 = taum_chargedHadrons[0];
			pm2 = taum_chargedHadrons[1];
			pm3 = taum_chargedHadrons[2];
		}
		
        
		pp1_pp1 = pp1*pp1; //std::cout << pp1_pp1 << std::endl;
		pp1_pp2 = pp1*pp2; //std::cout << pp1_pp2 << std::endl;
		pp1_pp3 = pp1*pp3; //std::cout << pp1_pp3 << std::endl;
		pp1_pm1 = pp1*pm1; //std::cout << pp1_pm1 << std::endl;
		pp1_pm2 = pp1*pm2; //std::cout << pp1_pm2 << std::endl;
		pp1_pm3 = pp1*pm3; //std::cout << pp1_pm3 << std::endl;
		pp2_pp2 = pp2*pp2; //std::cout << pp2_pp2 << std::endl;
		pp2_pp3 = pp2*pp3; //std::cout << pp2_pp3 << std::endl;
		pp2_pm1 = pp2*pm1; //std::cout << pp2_pm1 << std::endl;
		pp2_pm2 = pp2*pm2; //std::cout << pp2_pm2 << std::endl;
		pp2_pm3 = pp2*pm3; //std::cout << pp2_pm3 << std::endl;
		pp3_pp3 = pp3*pp3; //std::cout << pp3_pp3 << std::endl;
		pp3_pm1 = pp3*pm1; //std::cout << pp3_pm1 << std::endl;
		pp3_pm2 = pp3*pm2; //std::cout << pp3_pm2 << std::endl;
		pp3_pm3 = pp3*pm3; //std::cout << pp3_pm3 << std::endl;
		pm1_pm1 = pm1*pm1; //std::cout << pm1_pm1 << std::endl;
		pm1_pm2 = pm1*pm2; //std::cout << pm1_pm2 << std::endl;
		pm1_pm3 = pm1*pm3; //std::cout << pm1_pm3 << std::endl;
		pm2_pm2 = pm2*pm2; //std::cout << pm2_pm2 << std::endl;
		pm2_pm3 = pm2*pm3; //std::cout << pm2_pm3 << std::endl;
		pm3_pm3 = pm3*pm3; //std::cout << pm3_pm3 << std::endl;
	}*/
}

float mvaNtuple::compute_average_dr(const std::vector<TLorentzVector>& lvs)
{
	if (lvs.size()<2) return -9999.;

	float sum_dr = 0.;
	int ncomb = 0;

	for (auto i = lvs.begin(); i != lvs.end()-1; ++i) {
		for (auto j = i+1; j != lvs.end(); ++j) {
			++ncomb;
			sum_dr += i->DeltaR(*j);
		}
	}

	return sum_dr / ncomb;
}

float mvaNtuple::compute_average_dr(const std::vector<TLorentzVector>& lvs1,
									const std::vector<TLorentzVector>& lvs2)
{
	float sum_dr = 0.;
	int count = 0;
	
	for (const auto & i : lvs1) {
		for (const auto & j : lvs2) {
			count++;
			sum_dr += i.DeltaR(j);
		}
	}

	if (count == 0)
		return 0;
	else
		return sum_dr / count;
}

float mvaNtuple::compute_max_dr(const std::vector<TLorentzVector>& lvs)
{
	float max_dr = 0.;

	for (auto i = lvs.begin(); i != lvs.end()-1; ++i) {
		for (auto j = i+1; j != lvs.end(); ++j) {
		    float dr = i->DeltaR(*j);
			if (dr > max_dr) max_dr = dr;
		}
	}

	return max_dr;	
}

float mvaNtuple::compute_max_dr(const std::vector<TLorentzVector>& lvs1,
								const std::vector<TLorentzVector>& lvs2)
{
	float max_dr = 0.;
	for (const auto & i : lvs1) {
		for (const auto & j : lvs2) {
			float dr = i.DeltaR(j);
			if (dr > max_dr) max_dr = dr;
		}
	}

	return max_dr;
}

float mvaNtuple::compute_min_dr(const TLorentzVector& l,
								const std::vector<TLorentzVector>& vjs)
{
	if (vjs.size() < 1) return -9999.;

	float mindr = 666.;

	for (auto & j : vjs) {
		float dr = l.DeltaR(j);
		if (dr < mindr) mindr = dr;
	}

	return mindr;
}

float mvaNtuple::compute_min_dr(const std::vector<TLorentzVector>& lvs1,
								const std::vector<TLorentzVector>& lvs2)
{
	if (lvs1.size()<1 or lvs2.size()<1) return -9999.;
	
	float mindr = 666.;
	for (const auto & i : lvs1) {
		for (const auto & j : lvs2) {
			float dr = i.DeltaR(j);
			if (dr < mindr) mindr = dr;
		}
	}
	
	return mindr;
}

float mvaNtuple::compute_cosThetaS(const TLorentzVector& taup4_lead
								   /*,const TLorentzVector& taup4_sublead*/)
{
	// ????
	// https://github.com/HEP-KBFI/tth-htt/blob/master/bin/analyze_2l_2tau.cc#L104-L115
	
	//TLorentzVector ditaup4 = taup4_lead + taup4_sublead;
	//TLorentzVector tauboost = taup4_lead;

	return std::abs(taup4_lead.CosTheta());	
}

float mvaNtuple::compute_mT_lep(const miniLepton& lepton, float MET, float METphi)
{
	return sqrt(2*lepton.conept()*MET*(1-cos(lepton.phi()-METphi)));
}

float mvaNtuple::compute_max_lep_eta(const std::vector<miniLepton>& leptons)
{
	float max_eta = 0.;

	for (const auto & lep : leptons) {
		if (fabs(lep.eta())>max_eta) max_eta = fabs(lep.eta());
	}

	return max_eta;
}

float mvaNtuple::compute_upsilon(const miniTau& tau)
{
	// (2*E_ldgtrk - E_vis) / E_vis
	auto p4_ldgtrk = tau.leadtrackP4();
	auto p4_visible = tau.p4();
	return (2.*p4_ldgtrk.Energy()/p4_visible.Energy() - 1);
}

float mvaNtuple::compute_upsilon_pt(const miniTau& tau)
{
	// (2*pt_ldgtrk - pt_vis) / pt_vis
	auto p4_ldgtrk = tau.leadtrackP4();
	auto p4_visible = tau.p4();
	return (2.*p4_ldgtrk.Pt()/p4_visible.Pt() - 1);
}

float mvaNtuple::compute_cosPsi(const miniTau& tau, float mass)
{
	std::vector<TLorentzVector> chargedhadrons = tau.get_signalChargedHadrCands();
	if (chargedhadrons.size()<3)
		return 1.;
	else
		// FIXME: directly compute from TLorentzVector?
		return compute_cosPsi(chargedhadrons[0], chargedhadrons[1],
							  chargedhadrons[2], mass);	
}

float mvaNtuple::compute_cosPsi(const TLorentzVector& p1, const TLorentzVector& p2,
							  const TLorentzVector& p3, float mass)
// lab frame, 3 prongs mode only
// angle between the direction of the normal to the plane defined by the three pions and the direction of flight of a1
{
	float s = (p1+p2+p3).M2();
	float s12 = (p1+p2).M2();
	float s23 = (p2+p3).M2();
	float s13 = (p1+p3).M2();

	TVector3 v1 = p1.Vect();
	TVector3 v2 = p2.Vect();
	TVector3 v3 = p3.Vect();

	float m2 = mass*mass;

	float numerator = v1.Dot(v2.Cross(v3)) / (v1+v2+v3).Mag();
	float denominator = TMath::Sqrt(-lam(lam(s,s12,m2),lam(s,s13,m2),
										  lam(s,s23,m2)));

	return 8*s*numerator/denominator;
}

float mvaNtuple::compute_mll(const std::vector<miniLepton>& leptons)
{
	if (anatype_==Analyze_1l2tau) {
		return 0;
	}
	else if (anatype_==Analyze_2lss1tau or anatype_==Analyze_2l2tau) {
		assert(leptons.size()>1);
		return (leptons[0].p4()+leptons[1].p4()).M();
	}
	else {
		// return the mass of lepton pair cloest to Z peak (91.2 GeV)
		float bestmll = 0;
		for (auto l = leptons.begin(); l != leptons.end()-1; ++l) {
			for (auto l2 = l+1; l2 != leptons.end(); ++l2) {
				float newdiff = fabs((l->p4()+l2->p4()).M() - 91.2);
				float bestdiff = fabs(bestmll - 91.2);
				if (newdiff < bestdiff) bestmll = (l->p4()+l2->p4()).M();
			}
		}
		return bestmll;
	}
}

float mvaNtuple::lam(float a, float b, float c)
{
	return a*a + b*b + c*c - 2*a*b - 2*b*c - 2*a*c;
}

void mvaNtuple::compute_HTT_input_variables(const miniJet& bjet,
											const miniJet& wjet1,
											const miniJet& wjet2)
{
	assert(doHTT_);
	
	CSV_b = bjet.csv();
	qg_Wj2 = wjet2.qgLikelihood();
	pT_bWj1Wj2 = (bjet.p4()+wjet1.p4()+wjet2.p4()).Pt();
	pT_Wj2 = wjet2.pt();
	m_Wj1Wj2 = (wjet1.p4()+wjet2.p4()).M();

	//kinFit_->fit(bjet.p4(), wjet1.p4(), wjet2.p4());
	kinFit_->fit(bjet.lorentzvector(), wjet1.lorentzvector(), wjet2.lorentzvector());
	nllKinFit = kinFit_->nll();
	pT_b_o_kinFit_pT_b = (bjet.pt()) / (kinFit_->fittedBJet().Pt());

	/*
	std::cout << "HTT input variables: " << std::endl;
	std::cout << "CSV_b = " << CSV_b << std::endl;
	std::cout << "qg_Wj2 = " << qg_Wj2 << std::endl;
	std::cout << "pT_bWj1Wj2 = " << pT_bWj1Wj2 << std::endl;
	std::cout << "pT_Wj2 = " << pT_Wj2 << std::endl;
	std::cout << "m_Wj1Wj2 = " << m_Wj1Wj2 << std::endl;
	std::cout << "nllKinFit = " << nllKinFit << std::endl;
	std::cout << "pT_b = " << bjet.pt() << std::endl;
	std::cout << "kinFit_pT_b = " << kinFit_->fittedBJet().Pt() << std::endl;
	std::cout << "pT_b_o_kinFit_pT_b = " << pT_b_o_kinFit_pT_b << std::endl;
	*/
}

void mvaNtuple::compute_HTT(const std::vector<miniJet>& jets)
{
	assert(doHTT_);
	
	HTT = -9999.;
	HadTop_pt = -9999.;

	//assert(jets.size()>=3);
	if (jets.size()<3)
		return;
	
	for (auto bjet = jets.begin(); bjet != jets.end(); ++bjet) {
		for (auto wjet1 = jets.begin(); wjet1 != jets.end(); ++wjet1) {
			if (wjet1 == bjet) continue;
			for (auto wjet2 = wjet1+1; wjet2 != jets.end(); ++wjet2) {
				if (wjet2 == bjet) continue;
				if (wjet2 == wjet1) continue;
				/*
				std::cout << "start computing HTT with jet triplet: " << std::endl;
				std::cout << "bjet : "; bjet->dump();
				std::cout << "wjet1 : "; wjet1->dump();
				std::cout << "wjet2 : " ; wjet2->dump();
				*/
				compute_HTT_input_variables(*bjet, *wjet1, *wjet2);

				float vars[7] = {CSV_b, qg_Wj2, pT_bWj1Wj2, m_Wj1Wj2, nllKinFit,
								 pT_b_o_kinFit_pT_b, pT_Wj2};
				
				float htt = mva_eval_->evaluate_bdt_HTT(vars);
				//std::cout << "htt for this tripet : " << htt << std::endl;
				
				if (htt > HTT) {
					HTT = htt;
					HadTop_pt = (bjet->p4() + wjet1->p4() + wjet2->p4()).Pt();
				}
				/*
				std::cout << "best HTT/HadTop_pt so far : " << HTT << " " << HadTop_pt << std::endl;
				std::cout << "----------" << std::endl;
				*/
			}
		}
	}
	/*
	std::cout << "Best HTT : " << HTT << std::endl;
	std::cout << "HadTop_pt : " << HadTop_pt << std::endl;
	std::cout << "===========" << std::endl;
	*/
}

void mvaNtuple::evaluate_BDTs()
{
	assert(evalMVA_);
	if (anatype_ == Analyze_1l2tau) {
		float vars1_1l2tau[13] =
			{avg_dr_jet, dr_taus, met, mT_met_lep0, nJet, mTauTauVis, mindr_lep0_jet,
			 mindr_tau0_jet, mindr_tau1_jet, dr_lep_tau_lead, nbtags_loose, tau0_pt,
			 tau1_pt};
		mva_1l2tau_BDT1 = mva_eval_->evaluate_bdt_1l2tau_BDT1(vars1_1l2tau);
		
		float vars2_1l2tau[17] =
			{avg_dr_jet, dr_taus, met, lep0_conept, mT_met_lep0, mTauTauVis,
			 mindr_lep0_jet, mindr_tau0_jet, mindr_tau1_jet, dr_lep_tau_ss,
			 dr_lep_tau_lead, costS_tau, nbtags_loose, tau0_pt, tau1_pt, HTT,
			 HadTop_pt};
		mva_1l2tau_BDT2 = mva_eval_->evaluate_bdt_1l2tau_BDT2(vars2_1l2tau);

		mva_output = mva_1l2tau_BDT2;
	}
	else if (anatype_ == Analyze_2lss1tau) {
		float vars1_2l1tau[15] =
			{avg_dr_jet, dr_lep0_tau, dr_leps, lep0_conept, lep1_conept, mT_met_lep0,
			 mT_met_lep1, mvis_lep0_tau, mvis_lep1_tau, mindr_lep0_jet,
			 mindr_lep1_jet, mindr_tau0_jet, met, max_lep_eta, tau0_pt};
		mva_2lss1tau_BDT1 =  mva_eval_->evaluate_bdt_2lss1tau_BDT1(vars1_2l1tau);

		float vars2_2l1tau[16] =
			{avg_dr_jet, dr_lep0_tau, dr_lep1_tau, dr_leps, lep0_conept, lep1_conept,
			 mT_met_lep1, mvis_lep0_tau, mvis_lep1_tau, mbb, mindr_lep0_jet,
			 mindr_lep1_jet, mindr_tau0_jet, nJet, met, tau0_pt};
		mva_2lss1tau_BDT2 = mva_eval_->evaluate_bdt_2lss1tau_BDT2(vars2_2l1tau);
		
		float vars3_2l1tau[18] =
			{avg_dr_jet, dr_lep0_tau, dr_lep1_tau, dr_leps, lep0_conept, lep1_conept,
			 mT_met_lep0, mT_met_lep1, mvis_lep0_tau, mvis_lep1_tau, max_lep_eta, mbb,
			 mindr_lep0_jet, mindr_lep1_jet, mindr_tau0_jet, nJet, met, tau0_pt};
		mva_2lss1tau_BDT3 = mva_eval_->evaluate_bdt_2lss1tau_BDT3(vars3_2l1tau);
		
		float vars4_2l1tau[18] =
			{avg_dr_jet, dr_lep0_tau, dr_lep1_tau, dr_leps, lep1_conept, mT_met_lep0,
			 mT_met_lep1, mvis_lep1_tau, max_lep_eta, mbb, mindr_lep0_jet,
			 mindr_lep1_jet, mindr_tau0_jet, nJet, met, tau0_pt, HTT, HadTop_pt};
		mva_2lss1tau_BDT4 = mva_eval_->evaluate_bdt_2lss1tau_BDT4(vars4_2l1tau);

		//float vars5_2l1tau[20] =
		//	{avg_dr_jet, dr_lep0_tau, dr_lep1_tau, dr_leps, lep1_conept, mT_met_lep0,
		//	 mT_met_lep1, mvis_lep1_tau, max_lep_eta, mbb, mindr_lep0_jet,
		//	 mindr_lep1_jet, mindr_tau0_jet, nJet, met, tau0_pt, mem_LR, HTT,
		//	 Hj_tagger, HadTop_pt};
		//mva_2lss1tau_BDT5 = mva_eval_->evaluate_bdt_2lss1tau_BDT5(vars5_2l1tau);

		float vars6_2l1tau[2] = {mva_2lss1tau_BDT2, mva_2lss1tau_BDT1};
		mva_2lss1tau_BDT6 = mva_eval_->evaluate_bdt_2lss1tau_BDT6(vars6_2l1tau);

		mva_output = mva_2lss1tau_BDT4;
    }
	else if (anatype_ == Analyze_3l1tau) {
		float vars1_3l1tau[13] =
			{lep0_conept, lep1_conept, mindr_lep0_jet, mindr_lep1_jet, mT_met_lep1,
			 mT_met_lep0, max_lep_eta, avg_dr_jet, met, tau0_pt, dr_leps,
			 mvis_lep0_tau, mvis_lep1_tau};
		mva_3l1tau_BDT1 = mva_eval_->evaluate_bdt_3l1tau_BDT1(vars1_3l1tau);

		float vars2_3l1tau[15] =
			{mindr_lep0_jet, mindr_lep1_jet, mT_met_lep1, mT_met_lep0, max_lep_eta,
			 lep2_conept, mindr_lep2_jet, mindr_tau0_jet, avg_dr_jet, met, tau0_pt,
			 dr_leps, mvis_lep0_tau, mvis_lep1_tau, mbb};
		mva_3l1tau_BDT2 = mva_eval_->evaluate_bdt_3l1tau_BDT2(vars2_3l1tau);

		float vars3_3l1tau[12] =
			{lep0_conept, lep1_conept, mindr_lep0_jet, max_lep_eta, mindr_tau0_jet,
			 met, tau0_pt, dr_leps, mvis_lep0_tau, mvis_lep1_tau, mbb, nJet};
		mva_3l1tau_BDT3 = mva_eval_->evaluate_bdt_3l1tau_BDT3(vars3_3l1tau);

		float vars4_3l1tau[2] = {mva_3l1tau_BDT2, mva_3l1tau_BDT1};
		mva_3l1tau_BDT4 = mva_eval_->evaluate_bdt_3l1tau_BDT4(vars4_3l1tau);

		mva_output = mva_3l1tau_BDT3;
	}
	else if (anatype_ == Analyze_2l2tau) {
		float vars1_2l2tau[14] =
			{mTauTauVis, costS_tau, lep0_conept, lep1_conept, mT_met_lep0,
			 mT_met_lep1, dr_taus, min_dr_lep_jet, mindr_tau0_jet, avg_dr_jet,
			 min_dr_lep_tau, max_dr_lep_tau, is_OS, nJet};
		mva_2l2tau_BDT1 = mva_eval_->evaluate_bdt_2l2tau_BDT1(vars1_2l2tau);

		float vars2_2l2tau[11] =
			{mTauTauVis, costS_tau, tau0_pt, tau1_pt, tau1_eta, mindr_lep0_jet,
			 mT_met_lep0, mindr_tau_jet, max_dr_lep_tau, is_OS, nbtags_loose};
		mva_2l2tau_BDT2 = mva_eval_->evaluate_bdt_2l2tau_BDT2(vars2_2l2tau);

		float vars3_2l2tau[13] =
			{mTauTauVis, costS_tau, tau0_pt, tau1_pt, lep1_conept, mindr_lep0_jet,
			 mT_met_lep0, mindr_tau_jet, avg_dr_jet, avg_dr_lep_tau, dr_taus, is_OS,
			 nbtags_loose};
		mva_2l2tau_BDT3 = mva_eval_->evaluate_bdt_2l2tau_BDT3(vars3_2l2tau);

		float vars4_2l2tau[2] = {mva_2l2tau_BDT2, mva_2l2tau_BDT1};
		mva_2l2tau_BDT4 = mva_eval_->evaluate_bdt_2l2tau_BDT4(vars4_2l2tau);

		mva_output = mva_2l2tau_BDT3;
	}
}

#endif

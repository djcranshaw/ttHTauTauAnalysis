#ifndef eventNtuple_h
#define eventNtuple_h

#include "TTree.h"
#include "TLorentzVector.h"
#include "TString.h"

#include "miniLepton.h"
#include "miniTau.h"
#include "miniJet.h"

#include "Types_enum.h"

#include <iostream>
#include <vector>
#include <string>
#include <tuple>

class eventNtuple
{
 public:

	eventNtuple(){};
	~eventNtuple(){};

	void set_branch_address(TTree*);
	void setup_branches(TTree*);
	void initialize();
	std::vector<miniLepton> buildLeptons(bool loose=false) const;
	std::vector<miniLepton> buildLeptons(char) const;
	std::vector<TLorentzVector> buildFourVectorLeps(bool loose=false) const;
	std::vector<TLorentzVector> buildFourVectorTaus(std::vector<int>&, bool loose=false) const;
	//std::vector<miniTau> buildTaus(std::string) const;
	std::vector<miniTau> buildTaus(bool loose=false, std::string tightWPDef="L",
								   const TString& tec="NA", bool isdata=false,
								   float minPt=20.) const;
	std::vector<miniTau> buildTaus(bool, Analysis_types, const TString& tec="NA",
								   bool isdata=false, float minPt=20.) const;
	std::vector<TLorentzVector> buildFourVectorTaus(bool loose=false) const;
	std::vector<TLorentzVector> buildFourVectorTauDaugsCharged(bool loose=false) const;
	std::vector<TLorentzVector> buildFourVectorTauDaugsNeutral(bool loose=false) const;
	std::vector<miniJet> buildJets(const TString& jec="NA", bool doSmear=false,
                                   float minPt=25.) const;
	std::vector<miniJet> buildCleanedJets(float,Analysis_types,Selection_types,
										  std::vector<miniLepton> const * const,
										  std::vector<miniTau> const * const,
										  const TString& jec="NA",
                                          bool doSmear=false,
                                          float minPt=25.) const;
	std::tuple<int, int> count_btags(const std::vector<miniJet>&) const ;
	std::vector<TLorentzVector> buildFourVectorJets() const;
	std::vector<TLorentzVector> buildFourVectorBtagJets() const;
	TLorentzVector buildFourVectorMET(const TString& correction="na");
	float computeMHT();  // deprecated
	float computeMHT(const std::vector<miniLepton>&, const std::vector<miniTau>&,
					 const std::vector<miniJet>&);

	template <typename T>
		std::vector<TLorentzVector> buildFourVectors(const std::vector<T>&);
	
	////////////////////////
	//// variables
	////////////////////////
	
	// event variables
	unsigned int run = 0;
	unsigned int ls = 0;
	unsigned long long nEvent = 0;

	float event_weight = -9999.;
	float PU_weight = -9999.;
	float MC_weight = -9999.;
	float bTagSF_weight = -9999.;
	float leptonSF_weight = -9999.;
	float tauSF_weight = -9999.;
	float triggerSF_weight = -9999.;
	float FR_weight = -9999.;

	/////////////////////////////
	// systematics
	float MC_weight_scale_muF0p5 = -9999.;
	float MC_weight_scale_muF2 = -9999.;
	float MC_weight_scale_muR0p5 = -9999.;
	float MC_weight_scale_muR2 = -9999.;
	float btagSF_weight_LFUp = -9999.;
	float btagSF_weight_LFDown = -9999.;
	float btagSF_weight_HFUp = -9999.;
	float btagSF_weight_HFDown = -9999.;
	float btagSF_weight_HFStats1Up = -9999.;
	float btagSF_weight_HFStats1Down = -9999.;
	float btagSF_weight_HFStats2Up = -9999.;
	float btagSF_weight_HFStats2Down = -9999.;
	float btagSF_weight_LFStats1Up = -9999.;
	float btagSF_weight_LFStats1Down = -9999.;
	float btagSF_weight_LFStats2Up = -9999.;
	float btagSF_weight_LFStats2Down = -9999.;
	float btagSF_weight_cErr1Up = -9999.;
	float btagSF_weight_cErr1Down = -9999.;
	float btagSF_weight_cErr2Up = -9999.;
	float btagSF_weight_cErr2Down = -9999.;
	/////////////////////////////

	int HiggsDecayType = -9999;  // Higgs decay product pdgId

    int lepCategory = -9999;    // 0: mumu; 1: ee; 2:emu
    int btagCategory = -9999;   // 0: loose; 1: medium (>=2 medium btags)

	// pileup
	float npuTrue = -9999.;
	float npuInTime = -9999.;

	// pv
	float pvx = -9999.;
	float pvy = -9999.;
	float pvz = -9999.;
			
	// triggers
	//unsigned int triggerBits_single_e = 0;
	//unsigned int triggerBits_single_mu = 0;
	//unsigned int triggerBits_double_e = 0;
	//unsigned int triggerBits_double_mu = 0;
	//unsigned int triggerBits_elemu = 0;
	unsigned int triggerBits = 0;

	unsigned int filterBits = 0;

	int nBadMuons = -9999;

	// event selection flag
	int passTauCharge = -9999;
	int isGenMatchedLep = -9999;
	int isGenMatchedTau  = -9999;

    int n_presel_mu = -9999;
    int n_fakeable_mu = -9999;
    int n_mvasel_mu = -9999;
    int n_presel_ele = -9999;
	int n_fakeable_ele = -9999;
	int n_mvasel_ele = -9999;
	int n_presel_tau = -9999;
	int n_tau = -9999;
	int n_presel_jet = -9999;
	int n_jet = -9999;
	int n_btag_medium = -9999;
	int n_btag_loose = -9999;

	// muons
	std::vector<float> *mu_pt = 0;
	std::vector<float> *mu_conept = 0;
	std::vector<float> *mu_eta = 0;
	std::vector<float> *mu_phi = 0;
	std::vector<float> *mu_E = 0;
	std::vector<int>   *mu_charge = 0;
	std::vector<float> *mu_dxy = 0;
	std::vector<float> *mu_dz = 0;
	std::vector<int>   *mu_isfakeablesel = 0;
	std::vector<int>   *mu_ismvasel = 0;
	std::vector<int>   *mu_jetNDauChargedMVASel = 0;
	std::vector<float> *mu_miniRelIso = 0;
	std::vector<float> *mu_miniIsoCharged = 0;
	std::vector<float> *mu_miniIsoNeutral = 0;
	std::vector<float> *mu_jetPtRel = 0;
	std::vector<float> *mu_jetPtRatio = 0;
	std::vector<float> *mu_jetCSV = 0;
	std::vector<float> *mu_sip3D = 0;
	std::vector<float> *mu_segmentCompatibility = 0;
	std::vector<float> *mu_leptonMVA = 0;
	std::vector<float> *mu_pfRelIso04 = 0;
	std::vector<int>   *mu_istightcharge = 0;
	std::vector<float> *mu_mediumID = 0;
	std::vector<float> *mu_dpt_div_pt = 0;
	std::vector<int>   *mu_mcMatchType = 0;
	std::vector<int>   *mu_isPFMuon = 0;

	// electrons
	std::vector<float> *ele_pt = 0;
	std::vector<float> *ele_conept = 0;
	std::vector<float> *ele_eta = 0;
	std::vector<float> *ele_phi = 0;
	std::vector<float> *ele_E = 0;
	std::vector<int>   *ele_charge = 0;
	std::vector<float> *ele_dxy = 0;
	std::vector<float> *ele_dz = 0;
	std::vector<int>   *ele_isfakeablesel = 0;
	std::vector<int>   *ele_ismvasel = 0;
	std::vector<int>   *ele_jetNDauChargedMVASel = 0;
	std::vector<float> *ele_miniRelIso = 0;
	std::vector<float> *ele_miniIsoCharged = 0;
	std::vector<float> *ele_miniIsoNeutral = 0;
	std::vector<float> *ele_jetPtRel = 0;
	std::vector<float> *ele_jetPtRatio = 0;
	std::vector<float> *ele_jetCSV = 0;
	std::vector<float> *ele_sip3D = 0;
	std::vector<float> *ele_ntMVAeleID = 0;
	std::vector<float> *ele_leptonMVA = 0;
	std::vector<float> *ele_pfRelIso04 = 0;
	std::vector<int>   *ele_istightcharge = 0;
	std::vector<int>   *ele_isChargeConsistent = 0;
	std::vector<int>   *ele_passesConversionVeto = 0;
	std::vector<int>   *ele_nMissingHits = 0;
	std::vector<int>   *ele_mcMatchType = 0;
	std::vector<int>   *ele_isGenPhotonMatched = 0;
	std::vector<int>   *ele_isPromptGenPhotonMatched = 0;
	std::vector<float> *ele_ecalTrkEnergyPreCorr = 0;
	std::vector<float> *ele_ecalTrkEnergyPostCorr = 0;
	std::vector<float> *ele_energyScaleValue = 0;
	std::vector<float> *ele_energySigmaValue = 0;
	std::vector<float> *ele_energySmearNrSigma = 0;
	std::vector<float> *ele_energyScaleUp = 0;
	std::vector<float> *ele_energyScaleDown = 0;
	std::vector<float> *ele_energySigmaUp = 0;
	std::vector<float> *ele_energySigmaDown = 0;
	
	// taus
	std::vector<float> *tau_pt = 0;
	std::vector<float> *tau_eta = 0;
	std::vector<float> *tau_phi = 0;
	std::vector<float> *tau_E = 0;
	std::vector<int>   *tau_charge = 0;
	std::vector<float> *tau_dxy = 0;
	std::vector<float> *tau_dz = 0;
	std::vector<int>   *tau_decayMode = 0;
	std::vector<int>   *tau_decayModeFindingOldDMs = 0;
	std::vector<int>   *tau_decayModeFindingNewDMs = 0;
	std::vector<int>   *tau_byCombinedIsolationDeltaBetaCorr3Hits = 0;
	std::vector<int>   *tau_byLooseCombinedIsolationDeltaBetaCorr3Hits = 0;
	std::vector<int>   *tau_byMediumCombinedIsolationDeltaBetaCorr3Hits = 0;
	std::vector<int>   *tau_byTightCombinedIsolationDeltaBetaCorr3Hits = 0;
	//std::vector<int>   *tau_byLooseCombinedIsolationDeltaBetaCorr3HitsdR03 = 0;
	//std::vector<int>   *tau_byMediumCombinedIsolationDeltaBetaCorr3HitsdR03 = 0;
	//std::vector<int>   *tau_byTightCombinedIsolationDeltaBetaCorr3HitsdR03 = 0;
	std::vector<int>   *tau_byVLooseIsolationMVArun2v1DBdR03oldDMwLT = 0;
	std::vector<int>   *tau_byLooseIsolationMVArun2v1DBdR03oldDMwLT = 0;
	std::vector<int>   *tau_byMediumIsolationMVArun2v1DBdR03oldDMwLT = 0;
	std::vector<int>   *tau_byTightIsolationMVArun2v1DBdR03oldDMwLT = 0;
	std::vector<int>   *tau_byVTightIsolationMVArun2v1DBdR03oldDMwLT = 0;
	std::vector<float> *tau_byIsolationMVArun2v1DBdR03oldDMwLTraw = 0;
	std::vector<int>   *tau_againstMuonLoose3 = 0;
	std::vector<int>   *tau_againstMuonTight3 = 0;
	std::vector<int>   *tau_againstElectronVLooseMVA6 = 0;
	std::vector<int>   *tau_againstElectronLooseMVA6 = 0;
	std::vector<int>   *tau_againstElectronMediumMVA6 = 0;
	std::vector<int>   *tau_againstElectronTightMVA6 = 0;
	std::vector<float> *tau_idPreselection = 0;
	std::vector<float> *tau_idSelection = 0;
	std::vector<int>   *tau_mcMatchType = 0;
	std::vector<float> *tau_ecalEnergy = 0;
	std::vector<float> *tau_hcalEnergy = 0;
	//std::vector<int> *tau_isPFTau = 0;
	//std::vector<int> *tau_isCaloTau = 0;
	// decay substructure
	std::vector<std::vector<float>> *tau_signalChargedHadrCands_pt = 0;
	std::vector<std::vector<float>> *tau_signalChargedHadrCands_eta = 0;
	std::vector<std::vector<float>> *tau_signalChargedHadrCands_phi = 0;
	std::vector<std::vector<float>> *tau_signalChargedHadrCands_E = 0;
	std::vector<std::vector<int>>   *tau_signalChargedHadrCands_charge = 0;
	std::vector<std::vector<float>> *tau_signalNeutrHadrCands_pt = 0;
	std::vector<std::vector<float>> *tau_signalNeutrHadrCands_eta = 0;
	std::vector<std::vector<float>> *tau_signalNeutrHadrCands_phi = 0;
	std::vector<std::vector<float>> *tau_signalNeutrHadrCands_E = 0;
	std::vector<std::vector<float>> *tau_signalGammaCands_pt = 0;
	std::vector<std::vector<float>> *tau_signalGammaCands_eta = 0;
	std::vector<std::vector<float>> *tau_signalGammaCands_phi = 0;
	std::vector<std::vector<float>> *tau_signalGammaCands_E = 0;
	
	// jets
	std::vector<float> *jet_pt = 0;
	std::vector<float> *jet_eta = 0;
	std::vector<float> *jet_phi = 0;
	std::vector<float> *jet_E = 0;
	std::vector<float> *jet_csv = 0; // Store DeepCSV; for backward compactibility
	std::vector<float> *jet_deepcsv = 0;  
	std::vector<float> *jet_deepCvsB = 0;
	std::vector<float> *jet_deepCvsL = 0;
	std::vector<float> *jet_csvv2 = 0;
	std::vector<float> *jet_ctagCvsB = 0;
	std::vector<float> *jet_ctagCvsL = 0;
	std::vector<float> *jet_flavor = 0;
	std::vector<float> *jet_qgLikelihood = 0;
	std::vector<float> *jet_axis2 = 0;
	std::vector<float> *jet_ptD = 0;
	std::vector<int>   *jet_mult = 0;
	std::vector<float> *jet_jesUnc = 0;
    std::vector<float> *jet_smearFactor = 0;
    std::vector<float> *jet_smearFactor_up = 0;
    std::vector<float> *jet_smearFactor_down = 0;

	// met
	float PFMET = -9999.;
	float PFMETphi = -9999.;
	float MHT = -9999.;
	float metLD = -9999.;
	float METSignificance = -9999.;
	float METCov00 = -9999.;
	float METCov10 = -9999.;
	float METCov01 = -9999.;
	float METCov11 = -9999.;

	float PFMET_jesup = -9999.;
	float PFMETphi_jesup = -9999.;
	float PFMET_jesdown = -9999.;
	float PFMETphi_jesdown = -9999.;
	float PFMET_tesup = -9999.;
	float PFMETphi_tesup = -9999.;
	float PFMET_tesdown = -9999.;
	float PFMETphi_tesdown = -9999.;

    // Gen Objects
    std::vector<float> *matchedGenMuons_pt = 0;
    std::vector<float> *matchedGenMuons_eta = 0;
    std::vector<float> *matchedGenMuons_phi = 0;
    std::vector<float> *matchedGenMuons_E = 0;
    std::vector<int> *matchedGenMuons_status = 0;
    std::vector<int> *matchedGenMuons_pdgid = 0;

    std::vector<float> *matchedGenElectrons_pt = 0;
    std::vector<float> *matchedGenElectrons_eta = 0;
    std::vector<float> *matchedGenElectrons_phi = 0;
    std::vector<float> *matchedGenElectrons_E = 0;
    std::vector<int> *matchedGenElectrons_status = 0;
    std::vector<int> *matchedGenElectrons_pdgid = 0;

    std::vector<float> *matchedGenTaus_pt = 0;
    std::vector<float> *matchedGenTaus_eta = 0;
    std::vector<float> *matchedGenTaus_phi = 0;
    std::vector<float> *matchedGenTaus_E = 0;
    std::vector<int> *matchedGenTaus_status = 0;
    std::vector<int> *matchedGenTaus_pdgid = 0;

    std::vector<float> *genjet_pt = 0;
    std::vector<float> *genjet_eta = 0;
    std::vector<float> *genjet_phi = 0;
    std::vector<float> *genjet_E = 0;
};

template <typename T> std::vector<TLorentzVector>
eventNtuple::buildFourVectors(const std::vector<T>& miniObjects)
{
	std::vector<TLorentzVector> vecp4;
	for (const auto & obj : miniObjects) {
		vecp4.push_back(obj.p4());
	}
	return vecp4;
}

#endif

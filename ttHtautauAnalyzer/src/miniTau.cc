#include "ttHTauTauAnalysis/ttHtautauAnalyzer/interface/miniTau.h"

//#if !defined(__ACLIC__) && !defined(__ROOTCLING__)
miniTau::miniTau(const pat::Tau& tau, bool addDaughters)
{
	pt_ = tau.pt();
	eta_ = tau.eta();
	phi_ = tau.phi();
	mass_ = tau.mass();
	charge_ = tau.charge();
	//pdgid_ = tau.pdgId();

	if (tau.hasUserInt("isLoose"))
		isloose_ = tau.hasUserInt("isLoose");

	if (tau.hasUserInt("isTight"))
		istight_ = tau.userInt("isTight");

	if (tau.hasUserInt("MCMatchType"))
		mcmatchtype_ = tau.userInt("MCMatchType");
	else
		mcmatchtype_ = -9999;

	// assign tauID MVA work point index
	// tau ID MVA work point:
	// 0='VLoose', 1='Loose', 2='Medium', 3='Tight', 4='VTight'
	set_tauIDWPindex(tau.tauID("byVLooseIsolationMVArun2017v2DBoldDMdR0p3wLT2017")>0.5,
					 tau.tauID("byLooseIsolationMVArun2017v2DBoldDMdR0p3wLT2017")>0.5,
					 tau.tauID("byMediumIsolationMVArun2017v2DBoldDMdR0p3wLT2017")>0.5,
					 tau.tauID("byTightIsolationMVArun2017v2DBoldDMdR0p3wLT2017")>0.5,
					 tau.tauID("byVTightIsolationMVArun2017v2DBoldDMdR0p3wLT2017")>0.5);

	if (addDaughters) {
		// TODO
		assert(0);
	}
}
//#endif

miniTau::miniTau(const TLorentzVector& t, int charge, int decaymode,
				 bool isloose, bool istight, int mcmatchtype)
{
	pt_ = t.Pt();
	eta_ = t.Eta();
	phi_ = t.Phi();
	mass_ = t.M();
	charge_ = charge;
	decaymode_ = decaymode;
	isloose_ = isloose;
	istight_ = istight;
	mcmatchtype_ = mcmatchtype;
	mvawp_set_ = false;
}

miniTau::miniTau(const TLorentzVector& t, int charge, int decaymode,
				 bool isloose, bool istight,
				 const std::vector<TLorentzVector>& signalChargedHadrCands,
				 const std::vector<TLorentzVector>& signalGammaCands,
				 const std::vector<TLorentzVector>& signalNeutrHadrCands,
				 int mcmatchtype)
{
	pt_ = t.Pt();
	eta_ = t.Eta();
	phi_ = t.Phi();
	mass_ = t.M();
	charge_ = charge;
	decaymode_ = decaymode;
	isloose_ = isloose;
	istight_ = istight;
	mcmatchtype_ = mcmatchtype;
	signalChargedHadrCands_ = signalChargedHadrCands;
	signalGammaCands_ = signalGammaCands;
	signalNeutrHadrCands_ = signalNeutrHadrCands;
	mvawp_set_ = false;
}

void miniTau::set_tauIDWPindex(bool passVLooseID, bool passLooseID,
							   bool passMediumID, bool passTightID,
							   bool passVTightID)
{
	// tau ID MVA work point:
	// 0='VLoose', 1='Loose', 2='Medium', 3='Tight', 4='VTight'
	tauIDMVAWP_ = -1;
	if (passVLooseID) tauIDMVAWP_++;
	if (passLooseID)  tauIDMVAWP_++;
	if (passMediumID) tauIDMVAWP_++;
	if (passTightID)  tauIDMVAWP_++;
	if (passVTightID) tauIDMVAWP_++;
	mvawp_set_ = true;
}

bool miniTau::passMVAID(std::string WP) const 
{
	assert(mvawp_set_);
	if (WP=="VL")  // VLoose
		return tauIDMVAWP_ >= 0;
	else if (WP=="L") // Loose
		return tauIDMVAWP_ >= 1;
	else if (WP=="M")  // Medium
		return tauIDMVAWP_ >= 2;
	else if (WP=="T")  // Tight
		return tauIDMVAWP_ >= 3;
	else if (WP=="VT")  // VTight
		return tauIDMVAWP_ >= 4;
	else {
		std::cout << "Unknown tauID WP : " << WP << std::endl;
		return false;
	}
}

TLorentzVector miniTau::p4() const
{
	TLorentzVector t;
	t.SetPtEtaPhiM(pt_,eta_,phi_,mass_);
	return t;
}

TLorentzVector miniTau::chargedDaughtersP4() const
{
	TLorentzVector ChargedP4;

	for (const TLorentzVector & ch : signalChargedHadrCands_)
		ChargedP4 += ch;

	return ChargedP4;
}


TLorentzVector miniTau::sameSignChargedDaughtersP4() const
{
  // Sum up only the charged daughters with the same sign as the tau
  TLorentzVector ssChargedP4;

  assert(signalChargedHadrCands_.size()==signalChargedHadrCands_charge_.size());
  for (unsigned int ich = 0; ich < signalChargedHadrCands_.size(); ++ich) {
    if (charge_ * signalChargedHadrCands_charge_[ich] > 0) // ss
      ssChargedP4 += signalChargedHadrCands_[ich];
  }

  return ssChargedP4;
}
TLorentzVector miniTau::neutralDaughtersP4() const
{
	TLorentzVector NeutralP4;

	for (const TLorentzVector & g : signalGammaCands_)
		NeutralP4 += g;
	for (const TLorentzVector & nh : signalNeutrHadrCands_)
		NeutralP4 += nh;

	return NeutralP4;
}

TLorentzVector miniTau::leadtrackP4() const
{
	TLorentzVector ldgtrkP4(0.,0.,0.,0.);
	for (const TLorentzVector& ch : signalChargedHadrCands_) {
		if (ch.Pt()>ldgtrkP4.Pt())
			ldgtrkP4 = ch;
	}

	return ldgtrkP4;
}

bool miniTau::isGenMatched() const
{
	return (mcmatchtype_==1 or mcmatchtype_==2 or mcmatchtype_==3 or
			mcmatchtype_==4 or mcmatchtype_==5);
}

void miniTau::dump() const
{
    std::cout << " pt: " << pt() << " eta: " << eta() << " phi: " << phi()
			  << " mass: " << mass() << " charge: " << charge()
			  << " decaymode: " << decaymode() << " passLoose: " << passLooseSel()
			  << " passTight: " << passTightSel()
			  << " mcMatchType: " << MCMatchType() << std::endl;
}

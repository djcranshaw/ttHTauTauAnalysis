#ifndef EventSelector_h
#define EventSelector_h

#if !defined(__ACLIC__) && !defined(__ROOTCLING__)
//#include "DataFormats/PatCandidates/interface/Electron.h"
//#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
//#include "DataFormats/PatCandidates/interface/MET.h"
//#include "DataFormats/PatCandidates/interface/Jet.h"
#endif

#include "Types_enum.h"
#include "miniLepton.h"
#include "miniTau.h"
#include "TriggerHelper.h"

class EventSelector
{
 public:
	// constructor and destructor
	EventSelector(Analysis_types anatype, Selection_types seltype, bool verbose,
				  bool isMC=true) {
		anaType_ = anatype;
		selType_ = seltype;
		verbose_ = verbose;
		isMC_ = isMC;
	}

	EventSelector(bool verbose, bool isMC, bool genMatching=true,
				  bool looseSelection=false) {
		verbose_ = verbose;
		isMC_ = isMC;
		looseselection_ = looseSelection;
		genMatching_ = genMatching;
		anaType_ = Analyze_NA;
		selType_ = Selection_NA;
	}
	
	~EventSelector(){};

	// member functions
	void fill_cutflow(TH1*, int ibin, const char*);
	bool pass_hlt_paths(Analysis_types, TriggerHelper * const, unsigned int);
	bool pass_hlt_match(Analysis_types, TriggerHelper * const, unsigned int,int,int);
	bool pass_hlt_and_filters(Analysis_types, TriggerHelper * const, unsigned int,
							  int, int, unsigned int, bool);
	bool pass_ttH_ltau_inclusive_selection(const std::vector<miniLepton>&,
										   const std::vector<miniTau>&,
										   int, TH1* h_cutflow=0);
	bool pass_full_event_selection(Analysis_types, Selection_types,
								   const std::vector<miniLepton>&,
								   const std::vector<miniLepton>&,
								   const std::vector<miniLepton>&,
								   const std::vector<miniTau>&,
								   const std::vector<miniTau>&,
								   int, int, int, float, TH1* h_cutflow=0);
	
	bool pass_1l2tau_inclusive_selection(const std::vector<miniLepton>&,
										 const std::vector<miniLepton>&,
										 const std::vector<miniLepton>&,
										 const std::vector<miniTau>&,
										 int, int, int, TH1* h_cutflow=0);
	bool pass_1l2tau_tightID(const std::vector<miniLepton>&,
							 const std::vector<miniTau>&);
	bool pass_1l2tau_charge(const std::vector<miniTau>&);
	bool pass_1l2tau_selection(Selection_types, const std::vector<miniLepton>&,
							   const std::vector<miniTau>&);
	
	bool pass_2l_generic_selection(const std::vector<miniLepton>&,
								   const std::vector<miniLepton>&,
								   const std::vector<miniLepton>&,
								   int, int, int, float, int&, TH1* h_cutflow=0);
	bool pass_2ltight_ss_selection(const std::vector<miniLepton>&, int);
	bool pass_2ltight_ss_selection(const std::vector<miniLepton>&, int,
								   int&, TH1* h_cutflow=0);
	bool pass_2l1tau_inclusive_selection(const std::vector<miniLepton>&,
										 const std::vector<miniLepton>&,
										 const std::vector<miniLepton>&,
										 const std::vector<miniTau>&,
										 int, int, int, float, TH1* h_cutflow=0);
	bool pass_2l_tightLepID(const std::vector<miniLepton>&);
	bool pass_2l_2lSS(const std::vector<miniLepton>&);
	bool pass_2l1tau_leptauOS(const miniTau&, const miniLepton&);
	bool pass_2l1tau_tauNumber(const std::vector<miniTau>&);
	bool pass_2lss1tau_selection(Selection_types, const std::vector<miniLepton>&,
								 const std::vector<miniTau>&);
	
	bool pass_3l_generic_selection(const std::vector<miniLepton>&,
								   const std::vector<miniLepton>&,
								   int, float, int&, TH1* h_cutflow=0);
	bool pass_3l1tau_inclusive_selection(const std::vector<miniLepton>&,
										 const std::vector<miniLepton>&,
										 const std::vector<miniTau>&,
										 int, int, int, float, TH1* h_cutflow=0);
	bool pass_3l_tightLepID(const std::vector<miniLepton>&);
	bool pass_3l1tau_charge(const std::vector<miniLepton>&,
							const std::vector<miniTau>&);
	bool pass_3l1tau_tauNumber(const std::vector<miniTau>&);
	bool pass_3l1tau_selection(Selection_types, const std::vector<miniLepton>&,
							   const std::vector<miniTau>&);

	bool pass_2l2tau_inclusive_selection(const std::vector<miniLepton>&,
										 const std::vector<miniLepton>&,
										 const std::vector<miniTau>&,
										 int, int, int, float, TH1* h_cutflow=0);
	bool pass_2l2tau_tightID(const std::vector<miniLepton>&,
							 const std::vector<miniTau>&);
	bool pass_2l2tau_charge(const std::vector<miniLepton>&,
							const std::vector<miniTau>&);
	bool pass_2l2tau_selection(Selection_types, const std::vector<miniLepton>&,
							   const std::vector<miniTau>&);

	// control region
	bool pass_ttW_CR_selection(Selection_types, const std::vector<miniLepton>&,
							   const std::vector<miniLepton>&,
							   const std::vector<miniLepton>&,
							   const std::vector<miniTau>&,
							   int, int, int, float, TH1* h_cutflow=0);
	bool pass_ttZ_CR_selection(Selection_types, const std::vector<miniLepton>&,
							   const std::vector<miniLepton>&,
							   const std::vector<miniLepton>&,
							   const std::vector<miniTau>&,
							   int, int, int, float, TH1* h_cutflow=0);
	
	bool pass_pairMass_veto(const std::vector<miniLepton>&);
	bool pass_Zmass_veto(const std::vector<miniLepton>&, bool, bool);
	bool pass_metLD_3l(float, const std::vector<miniLepton>&, int);
	bool pass_lepton_pT_2l(const std::vector<miniLepton>&);
	bool pass_3l_charge(const std::vector<miniLepton>&);
	bool pass_HZZ4l_veto(const std::vector<miniLepton>&);
	bool pass_MCMatch(const std::vector<miniLepton>&, int,
					  const std::vector<miniTau>&, int);
	bool pass_MCMatch(Analysis_types, const std::vector<miniLepton>&,
					  const std::vector<miniTau>&);
	bool pass_MCMatch_Leps(Analysis_types, const std::vector<miniLepton>&);
	bool pass_MCMatch_Taus(Analysis_types, const std::vector<miniTau>&);
	bool is_MCMatch_Photon(Analysis_types, const std::vector<miniLepton>&);
	
 protected:
	
	Analysis_types  anaType_;
	Selection_types selType_;
	bool verbose_;
	bool isMC_;
	bool genMatching_;
	bool looseselection_;
};

#endif

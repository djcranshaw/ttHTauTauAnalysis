#ifndef ttHtautauAnalyzer_Setup_cc
#define ttHtautauAnalyzer_Setup_cc

#include "ttHTauTauAnalysis/ttHtautauAnalyzer/plugins/ttHtautauAnalyzer.h"

/* Deprecated 
ttHtautauAnalyzer now selects an inclusive sample covering all regions

void ttHtautauAnalyzer::Set_up_AnaType(const std::string& anatype)
{
	std::cout << anatype << std::endl;
	anaType_ = Types_enum::getAnaType(anatype);
	
	assert(AnaTypeMap.count(anatype)>0);
}

void ttHtautauAnalyzer::Set_up_SelType(const std::string& seltype)
{
	std::cout << seltype << std::endl;
	selType_ = Types_enum::getSelType(seltype);

	assert(SelTypeMap.count(seltype)>0);
}
*/

void ttHtautauAnalyzer::Set_up_histograms()
{
	h_nProcessed_ = fs_->make<TH1I>("h_nProcessed","",1,0.5,1.5);
	h_SumGenWeight_ = fs_->make<TH1D>("h_SumGenWeight","",1,0.5,1.5);
	//h_SumGenWeightxPU_ = fs_->make<TH1D>("h_SumGenWeightxPU","",1,0.5,1.5);

	// cut flow
	h_CutFlow_ = fs_->make<TH1D>("h_CutFlow","",15,0,15);

	// pileup
	h_npuTrue_ = fs_->make<TH1D>("h_npuTrue","",200,0,200);
	h_npuInTime_ = fs_->make<TH1D>("h_npuInTime","",200,0,200);
	h_npuTrue_genweight_ = fs_->make<TH1D>("h_npuTree_genweight","",200,0,200);
	h_npuInTime_genweight_ = fs_->make<TH1D>("h_npuInTime_genweight","",200,0,200);

	// HLT and filters
	h_hlt_ = fs_->make<TH1I>("h_hlt","",30,0,30);
	h_flt_ = fs_->make<TH1I>("h_filter","",20,0,20);

	if (trig_helper_) {
		std::vector<std::string> hltpaths = trig_helper_->get_hlt_paths_wo_version();
		std::vector<std::string> fltpaths = trig_helper_->get_flt_paths();
		
		TAxis *axis = h_hlt_->GetXaxis();
		for (unsigned int i=0; i<hltpaths.size(); ++i) {
			if (i >= (unsigned int)axis->GetNbins()) {
				std::cerr << "HLT histogram does not have enough bins!" << std::endl;
				break;
			}
			axis->SetBinLabel(i+1, hltpaths[i].c_str());
		}

		axis = h_flt_->GetXaxis();
		for (unsigned int i=0; i<fltpaths.size(); ++i) {
			if (i >= (unsigned int)axis->GetNbins()) {
				std::cerr << "Filter histogram does not have enough bins!" << std::endl;
				break;
			}
			axis->SetBinLabel(i+1, fltpaths[i].c_str());
		}
	}
	
}

void ttHtautauAnalyzer::Set_up_tree()
{
	eventTree_ = fs_->make<TTree>("eventTree", "Event tree");

	evNtuple_.setup_branches(eventTree_);
}

#endif

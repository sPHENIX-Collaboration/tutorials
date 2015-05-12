#ifndef PHFLOWJETMAKER_H__
#define PHFLOWJETMAKER_H__

#include <fun4all/SubsysReco.h>
#include <map>
#include <set>
#include <string>
#include <vector>

class PHCompositeNode;
class RawClusterContainer;
class SvtxTrackMap;
class TF1;

namespace fastjet {
  class PseudoJet;
  class JetDefinition;
  class SISConePlugin;    
  class ClusterSequence;
}


class PHFlowJetMaker: public SubsysReco
{
 public:

  PHFlowJetMaker(const std::string &name = "PHFlowJetMaker");

  virtual ~PHFlowJetMaker();

  int Init(PHCompositeNode *);

  int process_event(PHCompositeNode *);

  void create_calo_pseudojets(std::vector<fastjet::PseudoJet>& particles, RawClusterContainer* emc_clusters, RawClusterContainer* hci_clusters, RawClusterContainer* hco_clusters);

  void run_particle_flow(std::vector<fastjet::PseudoJet>& flow_particles, RawClusterContainer* emc_clusters, RawClusterContainer* hci_clusters, RawClusterContainer* hco_clusters, SvtxTrackMap* reco_tracks);

  int get_matched(double clus_energy, double track_energy);

			 //void save_jets_to_nodetree();

  private:
			 
  //Sampling Fractions
  static const float sfEMCAL = 0.03;
  static const float sfHCALIN = 0.071;
  static const float sfHCALOUT = 0.04;
			 
  //Tolerance for cluster-track matching
  TF1 *match_tolerance_low;
  TF1 *match_tolerance_high;

  fastjet::JetDefinition* fJetAlgorithm;
  double min_jet_pT;

			 // PHPyJetContainerV2 *flow_jet_container;

};

#endif

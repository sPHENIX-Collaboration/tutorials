#ifndef ANATUTORIAL_H__
#define ANATUTORIAL_H__

#include <fun4all/SubsysReco.h>

/// Class declarations for use in the analysis module
class Fun4AllHistoManager;
class PHCompositeNode;
class TFile;
class TTree;
class TH1;
class TH2;
class PHCompositeNode;
class RawClusterContainer;
class RawCluster;
class SvtxTrackMap;
class JetMap;
class GlobalVertex;
class PHHepMCGenEventMap;
class JetEvalStack;
class JetRecoEval;
class SvtxTrackEval;
class PHG4TruthInfoContainer;
class PHHepMCGenEvent;
class CaloTriggerInfo;
class JetTruthEval;
class SvtxEvalStack;
class JetEvalStack;

/// Definition of this analysis module class
class AnaTutorial : public SubsysReco
{
 public:
  /// Constructor
  AnaTutorial(const std::string &name = "AnaTutorial",
              const std::string &fname = "AnaTutorial.root");

  // Destructor
  virtual ~AnaTutorial();

  /// SubsysReco initialize processing method
  int Init(PHCompositeNode *);

  /// SubsysReco event processing method
  int process_event(PHCompositeNode *);

  /// SubsysReco end processing method
  int End(PHCompositeNode *);

  /// Set the minimum jet pT to cut on
  void setMinJetPt(float minjetpt) { m_minjetpt = minjetpt; }

  /// Set the minimum cluster pT to cut on
  void setMinClusPt(float mincluspt) { m_mincluspt = mincluspt; }

  /// Set things to analyze
  void analyzeTracks(bool analyzeTracks) { m_analyzeTracks = analyzeTracks; }
  void analyzeClusters(bool analyzeClusters) { m_analyzeClusters = analyzeClusters; }
  void analyzeJets(bool analyzeJets) { m_analyzeJets = analyzeJets; }
  void analyzeTruth(bool analyzeTruth) { m_analyzeTruth = analyzeTruth; }

 private:
  /// String to contain the outfile name containing the trees
  std::string m_outfilename;

  /// Fun4All Histogram Manager tool
  Fun4AllHistoManager *m_hm;

  /// A float for cutting on jet pt
  float m_minjetpt;

  /// A float for cutting on cluster pt
  float m_mincluspt;

  /// A boolean for running over tracks
  bool m_analyzeTracks;

  /// A boolean for running over clusters
  bool m_analyzeClusters;

  /// A boolean for running over jets
  bool m_analyzeJets;

  /// A boolean for collecting hepmc information
  bool m_analyzeTruth;

  /// TFile to hold the following TTrees and histograms
  TFile *m_outfile;
  TTree *m_clustertree;
  TTree *m_tracktree;
  TTree *m_hepmctree;
  TTree *m_truthtree;
  TTree *m_recojettree;
  TTree *m_truthjettree;
  TH1 *m_phi_h;
  TH2 *m_eta_phi_h;

  SvtxEvalStack *m_svtxEvalStack = nullptr;
  JetEvalStack *m_jetEvalStack = nullptr;

  /// Methods for grabbing the data
  void getTracks(PHCompositeNode *topNode);
  void getTruthJets(PHCompositeNode *topNode);
  void getReconstructedJets(PHCompositeNode *topNode);
  void getEMCalClusters(PHCompositeNode *topNode);
  void getHEPMCTruth(PHCompositeNode *topNode);
  void getPHG4Truth(PHCompositeNode *topNode);

  void initializeVariables();
  void initializeTrees();

  /**
   * Make variables for the relevant trees
   */

  /// HEPMC Tree variables
  int m_partid1;
  int m_partid2;
  double m_x1;
  double m_x2;
  int m_mpi;
  int m_process_id;
  double m_truthenergy;
  double m_trutheta;
  double m_truthphi;
  double m_truthpx;
  double m_truthpy;
  double m_truthpz;
  double m_truthpt;
  double m_truthp;
  int m_numparticlesinevent;
  int m_truthpid;

  /// Track variables
  double m_tr_px;
  double m_tr_py;
  double m_tr_pz;
  double m_tr_p;
  double m_tr_pt;
  double m_tr_phi;
  double m_tr_eta;
  int m_charge;
  double m_chisq;
  int m_ndf;
  double m_dca;
  double m_tr_x;
  double m_tr_y;
  double m_tr_z;
  int m_truth_is_primary;
  double m_truthtrackpx;
  double m_truthtrackpy;
  double m_truthtrackpz;
  double m_truthtrackp;
  double m_truthtracke;
  double m_truthtrackpt;
  double m_truthtrackphi;
  double m_truthtracketa;
  int m_truthtrackpid;

  /// Reconstructed jet variables
  double m_recojetpt;
  int m_recojetid;
  double m_recojetpx;
  double m_recojetpy;
  double m_recojetpz;
  double m_recojetphi;
  double m_recojetp;
  double m_recojetenergy;
  double m_recojeteta;
  int m_truthjetid;
  double m_truthjetp;
  double m_truthjetphi;
  double m_truthjeteta;
  double m_truthjetpt;
  double m_truthjetenergy;
  double m_truthjetpx;
  double m_truthjetpy;
  double m_truthjetpz;
  double m_dR;

  /// Cluster variables
  double m_clusenergy;
  double m_cluseta;
  double m_clustheta;
  double m_cluspt;
  double m_clusphi;
  double m_cluspx;
  double m_cluspy;
  double m_cluspz;
  double m_E_4x4;
};

#endif

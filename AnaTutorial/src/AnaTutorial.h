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
  void setMinJetPt(float minjetpt){_minjetpt = minjetpt;}
  
  /// Set the minimum cluster pT to cut on
  void setMinClusPt(float mincluspt){_mincluspt = mincluspt;}
  
  /// Set things to analyze
  void analyzeTracks(bool analyzeTracks){_analyzeTracks = analyzeTracks;}
  void analyzeClusters(bool analyzeClusters){_analyzeClusters = analyzeClusters;}
  void analyzeJets(bool analyzeJets){_analyzeJets = analyzeJets;}
  void analyzeTruth(bool analyzeTruth){_analyzeTruth = analyzeTruth;}
  
 protected:

  /// String to contain the outfile name containing the trees
  std::string outfilename;

  /// Fun4All Histogram Manager tool
  Fun4AllHistoManager *hm;

  /// A float for cutting on jet pt
  float _minjetpt;

  /// A float for cutting on cluster pt
  float _mincluspt;

  /// A boolean for running over tracks
  bool _analyzeTracks;

  /// A boolean for running over clusters
  bool _analyzeClusters;

  /// A boolean for running over jets
  bool _analyzeJets;

  /// A boolean for collecting hepmc information
  bool _analyzeTruth;

  /// TFile to hold the following TTrees and histograms
  TFile *outfile;
  TTree *clustertree;
  TTree *tracktree;
  TTree *hepmctree;
  TTree *truthtree;
  TTree *recojettree;
  TTree *truthjettree;
  TH1 *phi_h;
  TH2 *eta_phi_h;

  /// Methods for grabbing the data
  void getTracks(PHCompositeNode *topNode);
  void getTruthJets(PHCompositeNode *topNode);
  void getReconstructedJets(PHCompositeNode *topNode);
  void getEMCalClusters(PHCompositeNode *topNode);
  void getHEPMCTruth(PHCompositeNode *topNode);
  void getPHG4Truth(PHCompositeNode *topNode);
  void getTriggerInfo(PHCompositeNode *topNode);
  void getIHCalClusters(PHCompositeNode *topNode);

  void initializeVariables();
  void initializeTrees();

  /**
   * Make variables for the relevant trees
   */
  
  /// HEPMC Tree variables
  int partid1;
  int partid2;
  double x1;
  double x2;
  int mpi;
  int process_id;
  double truthenergy;
  double trutheta;
  double truthphi;
  double truthpx;
  double truthpy;
  double truthpz;
  double truthpt;
  double truthp;
  int numparticlesinevent;
  int truthpid;


  /// Track variables
  double tr_px;
  double tr_py;
  double tr_pz;
  double tr_p;
  double tr_pt;
  double tr_phi;
  double tr_eta;
  int charge;
  double chisq;
  int ndf;
  double dca;
  double tr_x;
  double tr_y;
  double tr_z;
  int truth_is_primary;
  double truthtrackpx;
  double truthtrackpy;
  double truthtrackpz;
  double truthtrackp;
  double truthtracke;
  double truthtrackpt;
  double truthtrackphi;
  double truthtracketa;
  int truthtrackpid;

  /// Reconstructed jet variables
  double recojetpt;
  int recojetid;
  double recojetpx;
  double recojetpy;
  double recojetpz;
  double recojetphi;
  double recojetp;
  double recojetenergy;
  double recojeteta;
  int truthjetid;
  double truthjetp;
  double truthjetphi;
  double truthjeteta;
  double truthjetpt;
  double truthjetenergy;
  double truthjetpx;
  double truthjetpy;
  double truthjetpz;
  double dR;

  /// Cluster variables
  double clusenergy;
  double cluseta;
  double clustheta;
  double cluspt;
  double clusphi;
  double cluspx;
  double cluspy;
  double cluspz;
  

};

#endif

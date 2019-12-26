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

/// Definition of this analysis module class
class AnaTutorial : public SubsysReco
{
 public:
  /// Constructor
  AnaTutorial(const std::string &name = "AnaTutorial", const std::string &fname = "AnaTutorial.root");

  // Destructor
  virtual ~AnaTutorial();

  /// SubsysReco initialize processing method
  int Init(PHCompositeNode *);

  /// SubsysReco event processing method
  int process_event(PHCompositeNode *);

  /// SubsysReco end processing method
  int End(PHCompositeNode *);


 protected:
  /// String to contain the outfile name containing the trees
  std::string outfilename;
  /// Fun4All Histogram Manager tool
  Fun4AllHistoManager *hm;

  /// TFile to hold the following TTrees and histograms
  TFile *outfile;
  TTree *clustertree;
  TTree *jettree;
  TTree *tracktree;
  TH1 *phi_h;
  TH2 *eta_phi_h;

};

#endif

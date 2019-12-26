#include "AnaTutorial.h"

/// Cluster includes
#include <calobase/RawCluster.h>
#include <calobase/RawClusterContainer.h>

/// Fun4All includes
#include <fun4all/Fun4AllHistoManager.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/getClass.h>

/// ROOT includes
#include <TFile.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>

/// C++ includes
#include <cassert>
#include <sstream>
#include <string>

using namespace std;

/**
 * This class demonstrates the construction and use of an analysis module 
 * within the sPHENIX Fun4All framework. It is intended to show how to 
 * obtain physics objects from the analysis tree, and then write them out
 * to a ROOT tree and file for further offline analysis.  
 */

/**
 * Constructor of module
 */
AnaTutorial::AnaTutorial(const std::string& name, const std::string& filename)
  : SubsysReco(name)
  , outfilename(filename)
  , hm(nullptr)
{
}

/**
 * Destructor of module
 */
AnaTutorial::~AnaTutorial()
{
  delete hm;
  delete clustertree;
  delete jettree;
  delete tracktree;
}

/**
 * Initialize the module and prepare looping over events
 */
int AnaTutorial::Init(PHCompositeNode*)
{
  hm = new Fun4AllHistoManager(Name());
  // create and register your histos (all types) here
  // TH1 *h1 = new TH1F("h1",....)
  // hm->registerHisto(h1);
  outfile = new TFile(outfilename.c_str(), "RECREATE");
  
  phi_h = new TH1D("phi_h",";Counts;#phi [rad]",50,-6,6);
  hm->registerHisto(phi_h);
  eta_phi_h = new TH2F("phi_eta_h",";#eta;#phi [rad]",10,-1,1,50,-6,6);
  hm->registerHisto(eta_phi_h);

  return 0;
}

/**
 * Main workhorse function where each event is looped over and 
 * data from each event is collected for analysis
 */
int AnaTutorial::process_event(PHCompositeNode* topNode)
{
  
  return Fun4AllReturnCodes::EVENT_OK;
}


/**
 * End the module and finish any data collection. Clean up any remaining
 * loose ends
 */
int AnaTutorial::End(PHCompositeNode* topNode)
{
  outfile->cd();
  tracktree->Write();
  clustertree->Write();
  jettree->Write();
  phi_h->Write();
  eta_phi_h->Write();
  outfile->Write();
  outfile->Close();

  delete outfile;
  /// Let the histogram manager deal with dumping the histogram memory
  hm->dumpHistos(outfilename, "UPDATE");
  return 0;
}

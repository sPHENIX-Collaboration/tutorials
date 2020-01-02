#include "AnaTutorial.h"

/// Cluster/Calorimeter includes
#include <calobase/RawCluster.h>
#include <calobase/RawClusterContainer.h>
#include <calotrigger/CaloTriggerInfo.h>
#include <calobase/RawClusterUtility.h>
#include <calobase/RawTower.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawTowerGeomContainer.h>

/// Jet includes
#include <g4jets/Jet.h>
#include <g4jets/JetMap.h>

/// Tracking includes
#include <g4vertex/GlobalVertex.h>
#include <g4vertex/GlobalVertexMap.h>
#include <trackbase_historic/SvtxTrack.h>
#include <trackbase_historic/SvtxTrackMap.h>
#include <trackbase_historic/SvtxVertex.h>
#include <trackbase_historic/SvtxVertexMap.h>

/// Truth evaluation includes
#include <g4eval/JetEvalStack.h>
#include <g4eval/SvtxEvalStack.h>

/// HEPMC truth includes
#include <HepMC/GenEvent.h>
#include <HepMC/GenVertex.h>
#include <phhepmc/PHHepMCGenEvent.h>
#include <phhepmc/PHHepMCGenEventMap.h>

/// Fun4All includes
#include <fun4all/Fun4AllHistoManager.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/getClass.h>
#include <g4main/PHG4Hit.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <phool/PHCompositeNode.h>

/// ROOT includes
#include <TFile.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TMath.h>
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
  , _minjetpt(5.0)
  , _mincluspt(0.25)
  , _analyzeTracks(true)
  , _analyzeClusters(true)
  , _analyzeJets(true)
  , _analyzeTruth(false)
{
}

/**
 * Destructor of module
 */
AnaTutorial::~AnaTutorial()
{
  delete hm;
  delete hepmctree;
  delete truthjettree;
  delete recojettree;
  delete tracktree;
}

/**
 * Initialize the module and prepare looping over events
 */
int AnaTutorial::Init(PHCompositeNode *topNode)
{
  initializeVariables();
  initializeTrees();

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
 * data from each event is collected from the node tree for analysis
 */
int AnaTutorial::process_event(PHCompositeNode* topNode)
{
  /// Get the truth information 
  if( _analyzeTruth ) {
    getHEPMCTruth(topNode);
    getPHG4Truth(topNode);
  }
  
  /// Get the tracks
  if (_analyzeTracks)
    getTracks(topNode);
  
  /// Get the truth and reconstructed jets
  if (_analyzeJets) {
    getTruthJets(topNode);
    getReconstructedJets(topNode);
  }
  
  /// Get calorimeter information
  if(_analyzeClusters){
    getEMCalClusters(topNode);
    getIHCalClusters(topNode);
    getTriggerInfo(topNode);
  }

  return Fun4AllReturnCodes::EVENT_OK;
}


/**
 * End the module and finish any data collection. Clean up any remaining
 * loose ends
 */
int AnaTutorial::End(PHCompositeNode* topNode)
{

  if(Verbosity() > 1){
    cout<< "Ending AnaTutorial analysis package"<<endl;
  }
  /// Change to the outfile
  outfile->cd();
  
  /// If we analyzed the tracks, write the tree out
  if(_analyzeTracks)
    tracktree->Write();
  
  /// If we analyzed the jets, write them out
  if(_analyzeJets){
    truthjettree->Write();
    recojettree->Write();
  }
  
  /// If we analyzed the truth particles, write them out
  if(_analyzeTruth){
    hepmctree->Write();
    truthtree->Write();
  }

  /// If we analyzed the clusters, write them out
  if(_analyzeClusters)
    {
      clustertree->Write();
    }
  
  /// Write out any other histograms
  phi_h->Write();
  eta_phi_h->Write();

  /// Write and close the outfile
  outfile->Write();
  outfile->Close();
  
  delete outfile;
  /// Let the histogram manager deal with dumping the histogram memory
  hm->dumpHistos(outfilename, "UPDATE");

    if(Verbosity() > 1){
    cout<< "Finished AnaTutorial analysis package"<<endl;
  }
 
  return 0;
}


/**
 * This method gets all of the HEPMC truth particles from the node tree
 * and stores them in a ROOT TTree. The HEPMC truth particles are what, 
 * for example, directly comes out of PYTHIA and thus gives you all of
 * the associated parton information
 */
void AnaTutorial::getHEPMCTruth(PHCompositeNode *topNode)
{
  
  /// Get the node from the node tree
  PHHepMCGenEventMap *hepmceventmap = findNode::getClass<PHHepMCGenEventMap>(topNode, "PHHepMCGenEventMap");
  
  /// If the node was not properly put on the tree, return
  if (!hepmceventmap)
    {
      cout << PHWHERE 
	   << "HEPMC event map node is missing, can't collected HEPMC truth particles"
	   << endl;
      return;
    }
  
  /// Could have some print statements for debugging with verbosity
  if (Verbosity() > 1)
    {
      cout << "Getting HEPMC truth particles " << endl;
    }
  
  /// You can iterate over the number of events in a hepmc event
  /// for pile up events where you have multiple hard scatterings per bunch crossing
  for (PHHepMCGenEventMap::ConstIter eventIter = hepmceventmap->begin();
       eventIter != hepmceventmap->end();
       ++eventIter)
    {
      /// Get the event
      PHHepMCGenEvent *hepmcevent = eventIter->second;
      
      if (hepmcevent)
	{
	  /// Get the event characteristics, inherited from HepMC classes
	  HepMC::GenEvent *truthevent = hepmcevent->getEvent();
	  if (!truthevent)
	    {
	      cout << PHWHERE 
		   << "no evt pointer under phhepmvgeneventmap found " 
		   << endl;
	      return;
	    }
	  
	  /// Get the parton info
	  HepMC::PdfInfo *pdfinfo = truthevent->pdf_info();
	  
	  /// Get the parton info as determined from HEPMC
	  partid1 = pdfinfo->id1();
	  partid2 = pdfinfo->id2();
	  x1 = pdfinfo->x1();
	  x2 = pdfinfo->x2();
	  
	  /// Are there multiple partonic intercations in a p+p event
	  mpi = truthevent->mpi();
	  
	  /// Get the PYTHIA signal process id identifying the 2-to-2 hard process
	  process_id = truthevent->signal_process_id();
	  
	  if(Verbosity() > 2)
	    {
	      cout<<" Iterating over an event"<<endl;
	    }
	  /// Loop over all the truth particles and get their information
	  for (HepMC::GenEvent::particle_const_iterator iter = truthevent->particles_begin();
	       iter != truthevent->particles_end();
	       ++iter)
	    {
	      /// Get each pythia particle characteristics
	      truthenergy = (*iter)->momentum().e();
	      truthpid = (*iter)->pdg_id();
	      
	      trutheta = (*iter)->momentum().pseudoRapidity();
	      truthphi = (*iter)->momentum().phi();
	      truthpx = (*iter)->momentum().px();
	      truthpy = (*iter)->momentum().py();
	      truthpz = (*iter)->momentum().pz();
	      truthpt = sqrt(truthpx * truthpx + truthpy * truthpy);
	      
	      //fill the truth tree
	      hepmctree->Fill();
	      numparticlesinevent++;
	    }
	}
    } 
  
}

/**
 * This function collects the truth PHG4 stable particles that
 * are produced from PYTHIA, or some other event generator. These
 * are the stable particles, e.g. there are not any (for example)
 * partons here.
 */
void AnaTutorial::getPHG4Truth(PHCompositeNode *topNode)
{

  /// G4 truth particle node
  PHG4TruthInfoContainer *truthinfo = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");
     
  if( !truthinfo ){
      cout << PHWHERE 
	   << "PHG4TruthInfoContainer node is missing, can't collect G4 truth particles"
	   << endl;
      return;
  }

  /// Get the primary particle range
  PHG4TruthInfoContainer::Range range = truthinfo->GetPrimaryParticleRange();

  /// Loop over the G4 truth (stable) particles
  for (PHG4TruthInfoContainer::ConstIterator iter = range.first;
       iter != range.second;
       ++iter)
    {
      /// Get this truth particle
      PHG4Particle *truth = iter->second;
      
      /// Get this particles momentum, etc.
      truthpx = truth->get_px();
      truthpy = truth->get_py();
      truthpz = truth->get_pz();
      truthp = sqrt(truthpx * truthpx + truthpy * truthpy + truthpz * truthpz);
      truthenergy = truth->get_e();
      
      truthpt = sqrt(truthpx * truthpx + truthpy * truthpy);
      
      truthphi = atan(truthpy / truthpx);
      
      trutheta = atanh(truthpz / truthenergy);
      /// Check for nans
      if (trutheta != trutheta)
	trutheta = -9999;
      truthpid = truth->get_pid();
      
      /// Fill the g4 truth tree
      truthtree->Fill();
    }
  
}



/**
 * This method gets the tracks as reconstructed from the tracker. It also
 * compares the reconstructed track to its truth track counterpart as determined
 * by the 
 */
void AnaTutorial::getTracks(PHCompositeNode *topNode)
{
  
  /// SVTX tracks node
  SvtxTrackMap *trackmap = findNode::getClass<SvtxTrackMap>(topNode, "SvtxTrackMap");

  if( !trackmap ){
      cout << PHWHERE 
	   << "SvtxTrackMap node is missing, can't collect tracks"
	   << endl;
      return;
  }
  
  /// EvalStack for truth track matching
  SvtxEvalStack *svtxevalstack = new SvtxEvalStack(topNode);
  svtxevalstack->next_event(topNode);

  /// Get the track evaluator
  SvtxTrackEval *trackeval = svtxevalstack->get_track_eval();
  
  /// Get the range for primary tracks
  PHG4TruthInfoContainer *truthinfo = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");
  
  if (Verbosity() > 1)
    {
      cout << "Get the SVTX tracks" << endl;
    }
  for (SvtxTrackMap::Iter iter = trackmap->begin();
       iter != trackmap->end();
       ++iter)
    {
      SvtxTrack *track = iter->second;
      
      /// Get the reconstructed track info
      tr_px = track->get_px();
      tr_py = track->get_py();
      tr_pz = track->get_pz();
      tr_p = sqrt(tr_px * tr_px + tr_py * tr_py + tr_pz * tr_pz);
      
      tr_pt = sqrt(tr_px * tr_px + tr_py * tr_py);
      
      // Make some cuts on the track to clean up sample
      if (tr_pt < 0.5)
	continue;
      
      tr_phi = track->get_phi();
      tr_eta = track->get_eta();
   
      charge = track->get_charge();
      chisq = track->get_chisq();
      ndf = track->get_ndf();
      dca = track->get_dca();
      tr_x = track->get_x();
      tr_y = track->get_y();
      tr_z = track->get_z();
      
      /// Get truth track info that matches this reconstructed track
      PHG4Particle *truthtrack = trackeval->max_truth_particle_by_nclusters(track);
      truth_is_primary = truthinfo->is_primary(truthtrack);
      
      truthtrackpx = truthtrack->get_px();
      truthtrackpy = truthtrack->get_py();
      truthtrackpz = truthtrack->get_pz();
      truthtrackp = sqrt(truthtrackpx * truthtrackpx + truthtrackpy * truthtrackpy + truthtrackpz * truthtrackpz);
      
      truthtracke = truthtrack->get_e();
      
      truthtrackpt = sqrt(truthtrackpx * truthtrackpx + truthtrackpy * truthtrackpy);
      truthtrackphi = atan(truthtrackpy / truthtrackpx);
      truthtracketa = atanh(truthtrackpz / truthtrackp);
      truthtrackpid = truthtrack->get_pid();
      
      tracktree->Fill();
    }
  
}

/**
 * Method that gets the truth jets and stores them in a tree
 */
void AnaTutorial::getTruthJets(PHCompositeNode *topNode)
{
  if (Verbosity() > 1)
     {
       cout << "get the truth jets" << endl;
     }
   
  /// Get the truth jet node
  JetMap *truth_jets = findNode::getClass<JetMap>(topNode, "AntiKt_Truth_r04");
  
  if (!truth_jets){
      cout << PHWHERE 
	   << "Truth jet node is missing, can't collect truth jets"
	   << endl;
      return;
  }


  /// Iterate over the truth jets
  for (JetMap::Iter iter = truth_jets->begin();
       iter != truth_jets->end();
       ++iter)
    {
      Jet *jet = iter->second;
      
      truthjetpt = jet->get_pt();
      
      /// Only collect truthjets above the _minjetpt cut
      if (truthjetpt < _minjetpt)
	continue;
      
      truthjeteta = jet->get_eta();   
      truthjetpx = jet->get_px();
      truthjetpy = jet->get_py();
      truthjetpz = jet->get_pz();
      truthjetphi = jet->get_phi();
      truthjetp = jet->get_p();
      truthjetenergy = jet->get_e();
      
      /// Fill the truthjet tree
      truthjettree->Fill();
    }
  
}

/**
 * Get the reconstructed jets and store them in a tree
 */
void AnaTutorial::getReconstructedJets(PHCompositeNode *topNode)
{
  /// Get the reconstructed tower jets
  JetMap *reco_jets = findNode::getClass<JetMap>(topNode, "AntiKt_Tower_r04");
  /// Get the truth jets
  JetMap *truth_jets = findNode::getClass<JetMap>(topNode, "AntiKt_Truth_r04");
  
  if(!reco_jets){
    cout << PHWHERE 
	 << "Reconstructed jet node is missing, can't collect reconstructed jets"
	 << endl;
    return;
  }

  if (Verbosity() > 1)
    {
       cout << "Get all Reco Jets" << endl;
    }
  
  /// Iterate over the reconstructed jets
  for (JetMap::Iter recoIter = reco_jets->begin();
       recoIter != reco_jets->end();
       ++recoIter)
    {
      Jet *jet = recoIter->second;
      recojetpt = jet->get_pt();
      if (recojetpt < _minjetpt)
	continue;
      
      recojeteta = jet->get_eta();
      
      // Get reco jet characteristics
      recojetid = jet->get_id();
      recojetpx = jet->get_px();
      recojetpy = jet->get_py();
      recojetpz = jet->get_pz();
      recojetphi = jet->get_phi();
      recojetp = jet->get_p();
      recojetenergy = jet->get_e();
      
      if (Verbosity() > 1)
	{
	  cout << "matching by distance jet" << endl;
	}
      
      /// Set the matched truth jet characteristics to 0
      truthjetid = 0;
      truthjetp = 0;
      truthjetphi = 0;
      truthjeteta = 0;
      truthjetpt = 0;
      truthjetenergy = 0;
      truthjetpx = 0;
      truthjetpy = 0;
      truthjetpz = 0;
      
      /// Check to make sure the truth jet node is available
      if(truth_jets) {
	/// Match the reconstructed jet to the closest truth jet in delta R space
	/// Iterate over the truth jets
	float closestjet = 9999;
	for (JetMap::Iter truthIter = truth_jets->begin();
	     truthIter != truth_jets->end();
	     ++truthIter)
	  {
	    Jet *jet = truthIter->second;
	    
	    float thisjetpt = jet->get_pt();
	    if (thisjetpt < _minjetpt)
	      continue;
	    
	    float thisjeteta = jet->get_eta();
	    float thisjetphi = jet->get_phi();
	    
	    float dphi = recojetphi - thisjetphi;
	    if (dphi > 3. * TMath::Pi() / 2.)
	      dphi -= TMath::Pi() * 2.;
	    if (dphi < -1. * TMath::Pi() / 2.)
	      dphi += TMath::Pi() * 2.;
	    
	    float deta = recojeteta - thisjeteta;
	    /// Determine the distance in eta phi space between the reconstructed
	    /// and truth jets
	    dR = sqrt(pow(dphi, 2.) + pow(deta, 2.));
	    
	    /// If this truth jet is closer than the previous truth jet, it is 
	    /// closer and thus should be considered the truth jet
	    if (dR < reco_jets->get_par() && dR < closestjet)
	      {
		truthjetid = -9999;  
		truthjetp = jet->get_p();
		truthjetphi = jet->get_phi();
		truthjeteta = jet->get_eta();
		truthjetpt = jet->get_pt();
		truthjetenergy = jet->get_e();
		truthjetpx = jet->get_px();
		truthjetpy = jet->get_py();
		truthjetpz = jet->get_pz();
		closestjet = dR;
	      }
	  }
      }
      recojettree->Fill();
      
    }
}


void AnaTutorial::getEMCalClusters(PHCompositeNode *topNode)
{
  /// Get the raw cluster container
  /// Note: other cluster containers exist as well. Check out the node tree when
  /// you run a simulation
  RawClusterContainer *clusters = findNode::getClass<RawClusterContainer>(topNode, "CLUSTER_CEMC");
  
  if(!clusters){
    cout << PHWHERE 
	 << "EMCal cluster node is missing, can't collect EMCal clusters"
	 << endl;
    return;
  }
  
  /// Get the global vertex to determine the appropriate pseudorapidity of the clusters
  GlobalVertexMap *vertexmap = findNode::getClass<GlobalVertexMap>(topNode, "GlobalVertexMap");
  if (!vertexmap)
    {
      cout << "AnaTutorial::getEmcalClusters - Fatal Error - GlobalVertexMap node is missing. Please turn on the do_global flag in the main macro in order to reconstruct the global vertex." << endl;
      assert(vertexmap);  // force quit
      
      return;
    }
  
  if (vertexmap->empty())
    {
      cout << "AnaTutorial::getEmcalClusters - Fatal Error - GlobalVertexMap node is empty. Please turn on the do_global flag in the main macro in order to reconstruct the global vertex." << endl;
      return;
    }
  
  GlobalVertex *vtx = vertexmap->begin()->second;
  if (vtx == nullptr) 
    return;
  
  RawClusterContainer::ConstRange begin_end = clusters->getClusters();
  RawClusterContainer::ConstIterator clusiter;
  
  /// Loop over the EMCal clusters
  for (clusiter = begin_end.first;
       clusiter != begin_end.second;
       ++clusiter)
    {
      /// Get this cluster
      RawCluster *cluster = clusiter->second;
      
      /// Get cluster characteristics
      /// This helper class determines the photon characteristics
      /// depending on the vertex position
      /// This is important for e.g. eta determination and E_T determination
      CLHEP::Hep3Vector vertex(vtx->get_x(), vtx->get_y(), vtx->get_z());
      CLHEP::Hep3Vector E_vec_cluster = RawClusterUtility::GetECoreVec(*cluster, vertex);
      clusenergy = E_vec_cluster.mag();
      cluseta = E_vec_cluster.pseudoRapidity();
      clustheta = E_vec_cluster.getTheta();
      cluspt = E_vec_cluster.perp();
      clusphi = E_vec_cluster.getPhi();
      
      if (cluspt < _mincluspt)
	continue;
   
      cluspx = cluspt * cos(clusphi);
      cluspy = cluspt * sin(clusphi);
      cluspz = sqrt(clusenergy * clusenergy - cluspx * cluspx - cluspy * cluspy);
      
      //fill the cluster tree with all emcal clusters
      clustertree->Fill();
      
      
    }
}

void AnaTutorial::getIHCalClusters(PHCompositeNode *topNode)
{

}

void AnaTutorial::getTriggerInfo(PHCompositeNode *topNode)
{

  /// Trigger emulator
  ///CaloTriggerInfo *trigger = findNode::getClass<CaloTriggerInfo>(topNode, "CaloTriggerInfo");

}

/**
 * This function puts all of the tree branch assignments in one place so as to not
 * clutter up the AnaTutorial::Init function.
 */
void AnaTutorial::initializeTrees()
{
  recojettree = new TTree("jettree","A tree with reconstructed jets");
  recojettree->Branch("recojetpt",&recojetpt,"recojetpt/D");
  recojettree->Branch("recojetid",&recojetid,"recojetid/I");
  recojettree->Branch("recojetpx",&recojetpx,"recojetpx/D");
  recojettree->Branch("recojetpy",&recojetpy,"recojetpy/D");
  recojettree->Branch("recojetpz",&recojetpz,"recojetpz/D");
  recojettree->Branch("recojetphi",&recojetphi,"recojetphi/D");
  recojettree->Branch("recojeteta",&recojeteta,"recojeteta/D");
  recojettree->Branch("recojetenergy",&recojetenergy,"recojetenergy/D");
  recojettree->Branch("truthjetid",&truthjetid,"truthjetid/I");
  recojettree->Branch("truthjetp",&truthjetp,"truthjetp/D");
  recojettree->Branch("truthjetphi",&truthjetphi,"truthjetphi/D");
  recojettree->Branch("truthjeteta",&truthjeteta,"truthjeteta/D");
  recojettree->Branch("truthjetpt",&truthjetpt,"truthjetpt/D");
  recojettree->Branch("truthjetenergy",&truthjetenergy,"truthjetenergy/D");
  recojettree->Branch("truthjetpx",&truthjetpx,"truthjetpx/D");
  recojettree->Branch("truthjetpy",&truthjetpy,"truthjyetpy/D");
  recojettree->Branch("truthjetpz",&truthjetpz,"truthjetpz/D");
  recojettree->Branch("dR",&dR,"dR/D");


  truthjettree = new TTree ("truthjettree","A tree with truth jets");
  truthjettree->Branch("truthjetid",&truthjetid,"truthjetid/I");
  truthjettree->Branch("truthjetp",&truthjetp,"truthjetp/D");
  truthjettree->Branch("truthjetphi",&truthjetphi,"truthjetphi/D");
  truthjettree->Branch("truthjeteta",&truthjeteta,"truthjeteta/D");
  truthjettree->Branch("truthjetpt",&truthjetpt,"truthjetpt/D");
  truthjettree->Branch("truthjetenergy",&truthjetenergy,"truthjetenergy/D");
  truthjettree->Branch("truthjetpx",&truthjetpx,"truthjetpx/D");
  truthjettree->Branch("truthjetpy",&truthjetpy,"truthjetpy/D");
  truthjettree->Branch("truthjetpz",&truthjetpz,"truthjetpz/D");

  tracktree = new TTree("tracktree", "A tree with svtx tracks");
  tracktree->Branch("tr_px",&tr_px,"tr_px/D");
  tracktree->Branch("tr_py",&tr_py,"tr_py/D");
  tracktree->Branch("tr_pz",&tr_pz,"tr_pz/D");
  tracktree->Branch("tr_p",&tr_p,"tr_p/D");
  tracktree->Branch("tr_pt",&tr_pt,"tr_pt/D");
  tracktree->Branch("tr_phi",&tr_phi,"tr_phi/D");
  tracktree->Branch("tr_eta",&tr_eta,"tr_eta/D");
  tracktree->Branch("charge",&charge,"charge/I");
  tracktree->Branch("chisq",&chisq,"chisq/D");
  tracktree->Branch("ndf",&ndf,"ndf/I");
  tracktree->Branch("dca",&dca,"dca/D");
  tracktree->Branch("tr_x",&tr_x,"tr_x/D");
  tracktree->Branch("tr_y",&tr_y,"tr_y/D");
  tracktree->Branch("tr_z",&tr_z,"tr_z/D");
  tracktree->Branch("truth_is_primary",&truth_is_primary,"truth_is_primary/I");
  tracktree->Branch("truthtrackpx",&truthtrackpx,"truthtrackpx/D");
  tracktree->Branch("truthtrackpy",&truthtrackpy,"truthtrackpy/D");
  tracktree->Branch("truthtrackpz",&truthtrackpz,"truthtrackpz/D");
  tracktree->Branch("truthtrackp",&truthtrackp,"truthtrackp/D");
  tracktree->Branch("truthtracke",&truthtracke,"truthtracke/D");
  tracktree->Branch("truthtrackpt",&truthtrackpt,"truthtrackpt/D");
  tracktree->Branch("truthtrackphi",&truthtrackphi,"truthtrackphi/D");
  tracktree->Branch("truthtracketa",&truthtracketa,"truthtracketa/D");
  tracktree->Branch("truthtrackpid",&truthtrackpid,"truthtrackpid/I");

  hepmctree = new TTree("hepmctree","A tree with hepmc truth particles");
  hepmctree->Branch("partid1",&partid1,"partid1/I");
  hepmctree->Branch("partid2",&partid2,"partid2/I");
  hepmctree->Branch("x1",&x1,"x1/D");
  hepmctree->Branch("x2",&x2,"x2/D");
  hepmctree->Branch("mpi",&mpi,"mpi/I");
  hepmctree->Branch("process_id",&process_id,"process_id/I");
  hepmctree->Branch("truthenergy",&truthenergy,"truthenergy/D");
  hepmctree->Branch("trutheta",&trutheta,"trutheta/D");
  hepmctree->Branch("truthphi",&truthphi,"truthphi/D");
  hepmctree->Branch("truthpx",&truthpx,"truthpx/D");
  hepmctree->Branch("truthpy",&truthpy,"truthpy/D");
  hepmctree->Branch("truthpz",&truthpz,"truthpz/D");
  hepmctree->Branch("truthpt",&truthpt,"truthpt/D");
  hepmctree->Branch("numparticlesinevent",&numparticlesinevent,"numparticlesinevent/I");
  hepmctree->Branch("truthpid",&truthpid,"truthpid/I");
  

  truthtree = new TTree("truthg4tree","A tree with truth g4 particles");
  truthtree->Branch("truthenergy", &truthenergy, "truthenergy/D");
  truthtree->Branch("truthp", &truthp, "truthp/D");
  truthtree->Branch("truthpx", &truthpx, "truthpx/D");
  truthtree->Branch("truthpy", &truthpy, "truthpy/D");
  truthtree->Branch("truthpz", &truthpz, "truthpz/D");
  truthtree->Branch("truthpt", &truthpt, "truthpt/D");
  truthtree->Branch("truthphi", &truthphi, "truthphi/D");
  truthtree->Branch("trutheta", &trutheta, "trutheta/D");
  truthtree->Branch("truthpid", &truthpid, "truthpid/I");

  clustertree = new TTree("clustertree", "A tree with emcal clusters");
  clustertree->Branch("clusenergy",&clusenergy,"clusenergy/D");
  clustertree->Branch("cluseta",&cluseta,"cluseta/D");
  clustertree->Branch("clustheta",&clustheta,"clustheta/D");
  clustertree->Branch("cluspt",&cluspt,"cluspt/D");
  clustertree->Branch("clusphi",&clusphi,"clusphi/D");
  clustertree->Branch("cluspx",&cluspx,"cluspx/D");
  clustertree->Branch("cluspy",&cluspy,"cluspy/D");
  clustertree->Branch("cluspz",&cluspz,"cluspz/D");

}

/**
 * This function initializes all of the member variables in this class so that there
 * are no variables that might not be set before e.g. writing them to the output
 * trees. 
 */
void AnaTutorial::initializeVariables() 
{
  partid1 = -99;
  partid2 = -99;
  x1 = -99;
  x2 = -99;
  mpi = -99;
  process_id = -99;
  truthenergy = -99;
  trutheta = -99;
  truthphi = -99;
  truthp = -99;
  truthpx = -99;
  truthpy = -99;
  truthpz = -99;
  truthpt = -99;
  numparticlesinevent = -99;
  truthpid = -99;
  
  tr_px = -99;
  tr_py = -99;
  tr_pz = -99;
  tr_p = -99;
  tr_pt = -99;
  tr_phi = -99;
  tr_eta = -99;
  charge = -99;
  chisq = -99;
  ndf = -99;
  dca = -99;
  tr_x = -99;
  tr_y = -99;
  tr_z = -99;
  truth_is_primary = -99;
  truthtrackpx = -99;
  truthtrackpy = -99;
  truthtrackpz = -99;
  truthtrackp = -99;
  truthtracke = -99;
  truthtrackpt = -99;
  truthtrackphi = -99;
  truthtracketa = -99;
  truthtrackpid = -99;
  
  recojetpt = -99;
  recojetid = -99;
  recojetpx = -99;
  recojetpy = -99;
  recojetpz = -99;
  recojetphi = -99;
  recojetp = -99;
  recojetenergy = -99;
  recojeteta = -99;
  truthjetid = -99;
  truthjetp = -99;
  truthjetphi = -99;
  truthjeteta = -99;
  truthjetpt = -99;
  truthjetenergy = -99;
  truthjetpx = -99;
  truthjetpy = -99;
  truthjetpz = -99;
  dR = -99;
  
  
  

}

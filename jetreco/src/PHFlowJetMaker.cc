//----------------------------------------------------------
// Module for flow jet reconstruction in sPHENIX
// J. Orjuela-Koop
// May 5 2015
//----------------------------------------------------------

#include<PHFlowJetMaker.h>
#include<g4cemc/RawCluster.h>
#include<g4cemc/RawClusterContainer.h>

#include <g4hough/SvtxTrackMap.h>
#include <g4hough/SvtxTrack.h>

#include<phool/PHCompositeNode.h>
#include<phool/PHNodeIterator.h>
#include<phool/PHNodeReset.h>
#include<fun4all/getClass.h>

#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequence.hh>
#include <fastjet/SISConePlugin.hh>

//#include <PHPyJetContainerV2.h>

#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TNtuple.h>
#include <TFile.h>
#include <TMath.h>

using namespace std;

typedef std::map<int,TLorentzVector*> tlvmap;

/*
 * Constructor
 */
PHFlowJetMaker::PHFlowJetMaker(char *outputfile, const std::string &name):SubsysReco(name)
{
  outfile = outputfile;

  //Define parameters for jet reconstruction
   min_jet_pT = 1.0;
   double r_param = 0.3;
   fastjet::Strategy strategy = fastjet::Best;
   fJetAlgorithm = new fastjet::JetDefinition (fastjet::antikt_algorithm,r_param,strategy);

   //Define tolerance limits for track-cluster matching
   match_tolerance_low = new TF1("match_tolerance_low","pol4");
   match_tolerance_low->SetParameter(0,-0.470354);
   match_tolerance_low->SetParameter(1,0.928888);
   match_tolerance_low->SetParameter(2,-0.0958367);
   match_tolerance_low->SetParameter(3,0.00748122);
   match_tolerance_low->SetParameter(4,-0.000177858);
   
   match_tolerance_high = new TF1("match_tolerance_high","pol2");
   match_tolerance_high->SetParameter(0,0.457184);
   match_tolerance_high->SetParameter(1,1.24821);
   match_tolerance_high->SetParameter(2,-0.00848157);
}

/*
 * Empty destructor
 */
PHFlowJetMaker::~PHFlowJetMaker()
{
}

/*
 * Initialize module
 */
int PHFlowJetMaker::Init(PHCompositeNode* topNode)
{
  cout << "------PHFlowJetMaker::Init(PHCompositeNode*)------" << endl;
  return 0;
}

/*
 * Process event
 */
int PHFlowJetMaker::process_event(PHCompositeNode* topNode)
{
  cout << "------PHFlowJetMaker::process_event(PHCompositeNode*)------" << endl;
  
  //-------------------------------------------------
  // Get Information from Node Tree
  //-------------------------------------------------

  //Get calorimeter clusters from node tree
  RawClusterContainer *emc_clusters = findNode::getClass<RawClusterContainer>(topNode,"CLUSTER_CEMC");
  RawClusterContainer *hci_clusters = findNode::getClass<RawClusterContainer>(topNode,"CLUSTER_HCALIN");
  RawClusterContainer *hco_clusters = findNode::getClass<RawClusterContainer>(topNode,"CLUSTER_HCALOUT");

  //Get reconstructed tracks from nodetree
  SvtxTrackMap* reco_tracks = findNode::getClass<SvtxTrackMap>(topNode,"SvtxTrackMap");

  //-------------------------------------------------
  // Create Jets
  //-------------------------------------------------

  //Create jets from raw clusters
  vector<fastjet::PseudoJet> raw_cluster_particles;
  create_calo_pseudojets(raw_cluster_particles,emc_clusters,hci_clusters,hco_clusters);
  fastjet::ClusterSequence jet_finder_raw(raw_cluster_particles,*fJetAlgorithm);
  vector<fastjet::PseudoJet> raw_cluster_jets = jet_finder_raw.inclusive_jets(min_jet_pT);
  //unsigned int njets_raw = raw_cluster_jets.size();

  //Apply particle flow jets algorithm and create jets from flow particles
  vector<fastjet::PseudoJet> flow_particles;
  run_particle_flow(flow_particles,emc_clusters,hci_clusters,hco_clusters,reco_tracks);
  fastjet::ClusterSequence jet_finder_flow(flow_particles,*fJetAlgorithm);
  vector<fastjet::PseudoJet> flow_jets = jet_finder_flow.inclusive_jets(min_jet_pT);
  //unsigned int njets_flow = flow_jets.size();

  return 0;
}

/*
 * Run particle flow algorithm
 */
void PHFlowJetMaker::run_particle_flow(std::vector<fastjet::PseudoJet>& flow_particles, RawClusterContainer* emc_clusters, RawClusterContainer* hci_clusters, RawClusterContainer* hco_clusters, SvtxTrackMap* reco_tracks)
{
  double emc_energy = 0;
  double hci_energy = 0;
  double hco_energy = 0;
  double cluster_energy = 0;
  
  double px = 0;
  double py = 0;
  double pz = 0;
  double pt = 0;
  double et = 0;
  double p = 0;
  double track_energy = 0;
  double phi = 0;
  double eta = 0;

  //Make track maps
  tlvmap emc_map;
  tlvmap hci_map;
  tlvmap hco_map;

  for(unsigned int i = 0; i < emc_clusters->size(); i++)
  {
    RawCluster* part = emc_clusters->getCluster(i);
    double pT = (part->get_energy()/sfEMCAL)/cosh(part->get_eta());
    emc_map[i] = new TLorentzVector();
    emc_map[i]->SetPtEtaPhiE(pT,part->get_eta(),part->get_phi(),part->get_energy()/sfEMCAL);
  }

  for(unsigned int i = 0; i < hci_clusters->size(); i++)
  {
    RawCluster* part = hci_clusters->getCluster(i);
    double pT = (part->get_energy()/sfEMCAL)/cosh(part->get_eta());
    hci_map[i] = new TLorentzVector();
    hci_map[i]->SetPtEtaPhiE(pT,part->get_eta(),part->get_phi(),part->get_energy()/sfHCALIN);
  }

  for(unsigned int i = 0; i < hco_clusters->size(); i++)
  {
    RawCluster* part = hco_clusters->getCluster(i);
    double pT = (part->get_energy()/sfEMCAL)/cosh(part->get_eta());
    hco_map[i] = new TLorentzVector();
    hco_map[i]->SetPtEtaPhiE(pT,part->get_eta(),part->get_phi(),part->get_energy()/sfHCALOUT);
  }
  
  //Loop over all tracks
  for(SvtxTrackMap::Iter iter = reco_tracks->begin(); iter != reco_tracks->end(); ++iter)
    {
      SvtxTrack *trk = &iter->second;
      px = trk->get3Momentum(0);
      py = trk->get3Momentum(1);
      pz = trk->get3Momentum(2);
      pt = sqrt(px*px + py*py);
      p = sqrt(px*px + py*py + pz*pz);
      track_energy = TMath::Sqrt(p*p + 0.139*0.139); //Assume pion mass
      phi = atan2(py,px);
      eta = -log(tan(acos(pz/p)/2.0));
      et = track_energy/cosh(eta);

      //Account for angle wrap-around
      if(phi < 0)
	{
	  phi = phi + 2*TMath::Pi();
	}
      else if(phi > 2*TMath::Pi())
	{
	  phi = phi + 2*TMath::Pi();
	}

      //Quality cut on tracks
      if(trk->getQuality() > 3.0) continue;

      //Find ID of clusters that match to track in each layer
      int emcID = -1;
      int hciID = -1;
      int hcoID = -1;
      emcID = (int)trk->get_cal_cluster_id(SvtxTrack::CEMC);
      hciID = (int)trk->get_cal_cluster_id(SvtxTrack::HCALIN);
      hcoID = (int)trk->get_cal_cluster_id(SvtxTrack::HCALOUT);

      //Find energy deposited by track in each layer
      tlvmap::iterator it = emc_map.find(emcID);
      if(it != emc_map.end())
	{
	  emc_energy = emc_map[emcID]->Energy();
	}
      
      it = hci_map.find(hciID);
      if(it != hci_map.end())
	{
	  hci_energy = hci_map[hciID]->Energy();
	}
      
      it = hco_map.find(hcoID);
      if(it != hco_map.end())
	{
	  hco_energy = hco_map[hcoID]->Energy();
	}

      cluster_energy = emc_energy + hci_energy + hco_energy;

      //Does the track match the cluster to within tolerance?
      //  *matched = 0 --> clus_energy < track_energy
      //  *matched = 1 --> clus_energy > track_energy
      //  *matched = 2 --> clus_energy = track_energy 
     int matched = -1;
     matched = get_matched(cluster_energy,track_energy);

     //If matched = 1, remove track energy from clusters
      if(matched == 1)
	{
	  float fracEnergyEMC = emc_energy/cluster_energy;
	  float fracEnergyHCI = hci_energy/cluster_energy;
	  float fracEnergyHCO = hco_energy/cluster_energy;
	  
	  it = emc_map.find(emcID);
	  if(it!=emc_map.end())
	    {
	      (emc_map.find(emcID)->second)->SetE(emc_energy - fracEnergyEMC*track_energy);
	    }
	  
	  it = hci_map.find(hciID);
	  if(it!=hci_map.end())
	    {
	      (hci_map.find(hciID)->second)->SetE(hci_energy - fracEnergyHCI*track_energy);
	    }
	  
	  it = hco_map.find(hcoID);
	  if(it!=hco_map.end())
	    {
	      (hco_map.find(hcoID)->second)->SetE(hco_energy - fracEnergyHCO*track_energy);
	    }	  
	}
      else if(matched == 2)
	{
	  it = emc_map.find(emcID);
	  if(it!=emc_map.end())
	    {
	      delete emc_map[emcID];
	      emc_map.erase(emcID);
	    }

	  it = hci_map.find(hciID);
	  if(it!=emc_map.end())
	    {
	      delete hci_map[hciID];
	      hci_map.erase(hciID);
	    }

	  it = hco_map.find(hcoID);
	  if(it!=hco_map.end())
	    {
	      delete hco_map[hcoID];
	      hco_map.erase(hcoID);
	    }
	}
      else if(matched == 0)
	{
	  continue;
	}

      //Add perfectly matched and partially matched tracks to flow particle container
      if(et<0.000001)
	{
	  et = 0.001;
	  pz = et*sinh(eta);
	  track_energy = sqrt(et*et + pz*pz);
	}
      fastjet::PseudoJet pseudoJet_track(et*cos(phi),et*sin(phi),pz,track_energy);
      flow_particles.push_back(pseudoJet_track);
    }

  //Add remaining clusters to flow particle container
  for(tlvmap::iterator it = emc_map.begin(); it!=emc_map.end(); it++)
    {
      double energy_clus = (it->second)->Energy();
      double eta_clus = (it->second)->Eta();
      double phi_clus = (it->second)->Phi();
      double et_clus = energy_clus/cosh(eta_clus);
      double pz_clus = et*sinh(eta_clus);

      if(et_clus<0.000001)
	{
	  et_clus = 0.001;
	  pz_clus = et_clus*sinh(eta_clus);
	  energy_clus = sqrt(et_clus*et_clus + pz_clus*pz_clus);
	}
      fastjet::PseudoJet pseudoJet_clus(et_clus*cos(phi_clus),et_clus*sin(phi_clus),pz_clus,energy_clus);
      flow_particles.push_back(pseudoJet_clus);
    }

  for(tlvmap::iterator it = hci_map.begin(); it!=hci_map.end(); it++)
    {
      double energy_clus = (it->second)->Energy();
      double eta_clus = (it->second)->Eta();
      double phi_clus = (it->second)->Phi();
      double et_clus = energy_clus/cosh(eta_clus);
      double pz_clus = et*sinh(eta_clus);

      if(et_clus<0.000001)
	{
	  et_clus = 0.001;
	  pz_clus = et_clus*sinh(eta_clus);
	  energy_clus = sqrt(et_clus*et_clus + pz_clus*pz_clus);
	}
      fastjet::PseudoJet pseudoJet_clus(et_clus*cos(phi_clus),et_clus*sin(phi_clus),pz_clus,energy_clus);
      flow_particles.push_back(pseudoJet_clus);
    }

  for(tlvmap::iterator it = hco_map.begin(); it!=hco_map.end(); it++)
    {
      double energy_clus = (it->second)->Energy();
      double eta_clus = (it->second)->Eta();
      double phi_clus = (it->second)->Phi();
      double et_clus = energy_clus/cosh(eta_clus);
      double pz_clus = et*sinh(eta_clus);

      if(et_clus<0.000001)
	{
	  et_clus = 0.001;
	  pz_clus = et_clus*sinh(eta_clus);
	  energy_clus = sqrt(et_clus*et_clus + pz_clus*pz_clus);
	}
      fastjet::PseudoJet pseudoJet_clus(et_clus*cos(phi_clus),et_clus*sin(phi_clus),pz_clus,energy_clus);
      flow_particles.push_back(pseudoJet_clus);
    }
}

/*
 * Create pseudojets from calorimeter clusters before applying particle flow algorithm
 */
void PHFlowJetMaker::create_calo_pseudojets(std::vector<fastjet::PseudoJet>& particles, RawClusterContainer* emc_clusters, RawClusterContainer* hci_clusters, RawClusterContainer* hco_clusters)
{
  //Loop over EMCAL clusters 
  for(unsigned int i=0; i<emc_clusters->size(); i++)
    {
      RawCluster* part = emc_clusters->getCluster(i);
      double eta = part->get_eta();
      double phi = part->get_phi();
      double energy = part->get_energy()/sfEMCAL;
      double eT = energy/cosh(eta);
      double pz = eT*sinh(eta);
     
      if(eT<0.000001)
	{
	  eT = 0.001;
	  pz = eT*sinh(eta);
	  energy = sqrt(eT*eT + pz*pz);
	}
      
      fastjet::PseudoJet pseudoJet(eT*cos(phi),eT*sin(phi),pz,energy);
      particles.push_back(pseudoJet); 
    }

  //Loop over HCALIN clusters 
  for(unsigned int i=0; i<hci_clusters->size(); i++)
    {
      RawCluster* part = hci_clusters->getCluster(i);
      double eta = part->get_eta();
      double phi = part->get_phi();
      double energy = part->get_energy()/sfHCALIN;
      double eT = energy/cosh(eta);
      double pz = eT*sinh(eta);
     
      if(eT<0.000001)
	{
	  eT = 0.001;
	  pz = eT*sinh(eta);
	  energy = sqrt(eT*eT + pz*pz);
	}
      
      fastjet::PseudoJet pseudoJet(eT*cos(phi),eT*sin(phi),pz,energy);
      particles.push_back(pseudoJet); 
    }

  //Loop over HCALOUT clusters 
  for(unsigned int i=0; i<hco_clusters->size(); i++)
    {
      RawCluster* part = hco_clusters->getCluster(i);
      double eta = part->get_eta();
      double phi = part->get_phi();
      double energy = part->get_energy()/sfHCALOUT;
      double eT = energy/cosh(eta);
      double pz = eT*sinh(eta);
     
      if(eT<0.000001)
	{
	  eT = 0.001;
	  pz = eT*sinh(eta);
	  energy = sqrt(eT*eT + pz*pz);
	}
      
      fastjet::PseudoJet pseudoJet(eT*cos(phi),eT*sin(phi),pz,energy);
      particles.push_back(pseudoJet); 
    }
}

/*
 * Given a track and cluster energies, determine if they match within tolerance
 */
int PHFlowJetMaker::get_matched(double clus_energy, double track_energy)
{
  double limLo = match_tolerance_low->Eval(track_energy);
  double limHi = match_tolerance_high->Eval(track_energy);

  int matched = -1;

  if(clus_energy < limLo)
    {
      //Track energy greater than cluster energy. Most likely a fake
      matched = 0;
    }
  else if(clus_energy > limHi)
    {
      //Contaminated cluster
      matched = 1;
    }
  else
    {
      //Track and cluster match to within reason
      matched = 2;
    }

  return matched;
}

/*
 * Write jets to node tree
 */
/*
void PHFlowJetMaker::save_jets_to_nodetree()
{
  flow_jet_container = new PHPyJetContainerV2();
}
*/
/*
 * End
 */
int PHFlowJetMaker::End(PHCompositeNode* topNode)
{
  return 0;
}


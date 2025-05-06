#include "caloHistGen.h"

// For clusters and geometry
#include <calobase/RawCluster.h>
#include <calobase/RawClusterContainer.h>
#include <calobase/RawClusterUtility.h>
#include <calobase/RawTowerDefs.h>

// Tower stuff
#include <calobase/TowerInfo.h>
#include <calobase/TowerInfoContainer.h>
#include <calobase/TowerInfoDefs.h>

// for the vertex
#include <globalvertex/GlobalVertex.h>
#include <globalvertex/GlobalVertexMap.h>

// GL1 Information
#include <ffarawobjects/Gl1Packet.h>

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>
#include <phool/phool.h>

// ROOT stuff
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TTree.h>

// for cluster vertex correction
#include <CLHEP/Vector/ThreeVector.h>
#include <TLorentzVector.h>
#include <cstdint>
#include <iostream>
#include <map>
#include <utility>

//____________________________________________________________________________..
caloHistGen::caloHistGen(const std::string &name)
  : SubsysReco("CaloHistGen")
  , Outfile(name)
{
  std::cout << "caloHistGen::caloHistGen(const std::string &name) Calling ctor" << std::endl;
}

//____________________________________________________________________________..
int caloHistGen::Init(PHCompositeNode * /*topNode*/)
{
  delete out;  // make cppcheck happy (nullptrs can be deleted)
  out = new TFile(Outfile.c_str(), "RECREATE");

  h_emcTowE = new TH3F("emcTowE", "tower eta, tower phi, tower energy", 96, -0.5, 95.5, 256, -0.5, 256.5, 1000, -10, 25);
  h_OHCalTowE = new TH3F("OHCalTowE", "outer hcal tower eta, tower phi, tower energy", 24, -0.5, 23.5, 64, -0.5, 63.5, 1000, -10, 25);
  h_IHCalTowE = new TH3F("IHCalTowE", "inner hcal tower eta, tower phi, tower energy", 24, -0.5, 23.5, 64, -0.5, 63.5, 1000, -10, 25);

  h_emcTowChi2 = new TH3F("emcTowChi2", "tower eta, tower phi, tower Chi2", 96, -0.5, 95.5, 256, -0.5, 256.5, 1000, 0.5, 4e8);
  h_OHCalTowChi2 = new TH3F("OHCalTowChi2", "tower eta, tower phi, tower Chi2", 24, -0.5, 23.5, 64, -0.5, 63.5, 1000, 0.5, 4e8);
  h_IHCalTowChi2 = new TH3F("IHCalTowChi2", "tower eta, tower phi, tower Chi2", 24, -0.5, 23.5, 64, -0.5, 63.5, 1000, 0.5, 4e8);

  h_emcTowTime = new TH3F("emcTowEnergy", "tower eta, tower phi, tower Time", 96, -0.5, 95.5, 256, -0.5, 256.5, 21, -10.5, 10.5);
  h_OHCalTowTime = new TH3F("OHCalTowEnergy", "tower eta, tower phi, tower Time", 24, -0.5, 23.5, 64, -0.5, 63.5, 21, -10.5, 10.5);
  h_IHCalTowTime = new TH3F("IHCalTowTime", "tower eta, tower phi, tower Time", 24, -0.5, 23.5, 64, -0.5, 63.5, 21, -10.5, 10.5);

  h_emcTowPed = new TH3F("emcTowPed", "tower eta, tower phi, tower Ped", 96, -0.5, 95.5, 256, -0.5, 256.5, 100, 0, 5000);
  h_OHCalTowPed = new TH3F("OHCalTowPed", "tower eta, tower phi, tower Ped", 24, -0.5, 23.5, 64, -0.5, 63.5, 100, 0, 5000);
  h_IHCalTowPed = new TH3F("IHCalTowPed", "tower eta, tower phi, tower Ped", 24, -0.5, 23.5, 64, -0.5, 63.5, 100, 0, 5000);

  h_clusInfo = new TH3F("clusInfo", "cluster eta, phi, energy", 140, -1.2, 1.2, 100, -1. * M_PI, M_PI, 500, -2, 25);

  h_clusPt = new TH1F("clusPt", "cluster pT", 500, -2, 25);
  h_clusEcore = new TH1F("clusEcore", "cluster Ecore", 500, -2, 25);

  h_clusChi2 = new TH1F("clusChi2_E", "cluster chi2", 100, 0, 100);

  h_zVertex = new TH1F("zVertex", "z vertex from mbd", 200, -100, 100);

  h_zdcNSE = new TH2F("zscNSChanE", "zdc N/S Energy", 2, -0.5, 1.5, 100, 0, 500);

  h_zdcChanTime = new TH2F("zdcChanTime", "zdc timing per channel", 7, -0.5, 6.5, 21, -10.5, 10.5);

  h_diPhotonEtaPhiE = new TH3F("h_diPhontonEtaPhiE", "diphoton spatial kinematics", 100, -1.1, 1.1, 100, -1 * M_PI, M_PI, 40, 0, 20);

  h_diPhotonMassE = new TH2F("h_diPhontonMassE", "diphoton mass and energy", 200, 0, 1, 40, 0, 20);

  // so that the histos actually get written out
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Print("NODETREE");

  std::cout << "caloHistGen::Init(PHCompositeNode *topNode) Initializing" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int caloHistGen::process_event(PHCompositeNode *topNode)
{
  float vertex = -9999;
  GlobalVertexMap *vertexmap = findNode::getClass<GlobalVertexMap>(topNode, "GlobalVertexMap");
  if (!vertexmap)
  {
    std::cout << "GlobalVertexMap node is missing" << std::endl;
  }
  if (vertexmap && !vertexmap->empty())
  {
    GlobalVertex *vtx = vertexmap->begin()->second;
    if (vtx)
    {
      vertex = vtx->get_z();
      h_zVertex->Fill(vertex);
    }
  }

  Gl1Packet *gl1PacketInfo = findNode::getClass<Gl1Packet>(topNode, m_trigNode.c_str());
  if (!gl1PacketInfo)
  {
    std::cout << PHWHERE << "caloHistGen::process_event: " << m_trigNode << " node is missing. Output related to this node will be empty" << std::endl;
  }

  if (gl1PacketInfo && checkTrig)
  {
    uint64_t triggervec = gl1PacketInfo->getScaledVector();
    for (int i = 0; i < 64; i++)
    {
      bool trig_decision = ((triggervec & 0x1U) == 0x1U);

      triggervec = (triggervec >> 1U) & 0xffffffffU;
      if (!trig_decision && i == trigRequired[i])
      {
        return 0;
      }
    }
  }

  // Information on clusters
  RawClusterContainer *clusterContainer = findNode::getClass<RawClusterContainer>(topNode, m_clusterNode.c_str());
  if (!clusterContainer && storeClusters)
  {
    std::cout << PHWHERE << "caloHistGen::process_event: " << m_clusterNode << " node is missing. Output related to this node will be empty" << std::endl;
    return 0;
  }

  // tower information
  TowerInfoContainer *emcTowerContainer;
  emcTowerContainer = findNode::getClass<TowerInfoContainer>(topNode, m_emcTowerNode.c_str());
  if (!emcTowerContainer && storeEMCal)
  {
    std::cout << PHWHERE << "caloHistGen::process_event: " << m_emcTowerNode << " node is missing. Output related to this node will be empty" << std::endl;
  }

  // grab all the towers and fill their energies.
  unsigned int tower_range = 0;
  if (storeEMCal && emcTowerContainer)
  {
    std::cout << "Looping over EMCal towers" << std::endl;
    tower_range = emcTowerContainer->size();
    for (unsigned int iter = 0; iter < tower_range; iter++)
    {
      unsigned int towerkey = emcTowerContainer->encode_key(iter);
      unsigned int ieta = TowerInfoDefs::getCaloTowerEtaBin(towerkey);
      unsigned int iphi = TowerInfoDefs::getCaloTowerPhiBin(towerkey);
      double energy = emcTowerContainer->get_tower_at_channel(iter)->get_energy();
      int time = emcTowerContainer->get_tower_at_channel(iter)->get_time();
      float chi2 = emcTowerContainer->get_tower_at_channel(iter)->get_chi2();
      float pedestal = emcTowerContainer->get_tower_at_channel(iter)->get_pedestal();

      if (emcTowerContainer->get_tower_at_channel(iter)->get_isGood())
      {
        h_emcTowE->Fill(ieta, iphi, energy);
        h_emcTowChi2->Fill(ieta, iphi, chi2);
        h_emcTowTime->Fill(ieta, iphi, time);
        h_emcTowPed->Fill(ieta, iphi, pedestal);
      }
    }
  }

  if (storeClusters && storeEMCal)
  {
    std::cout << "Storing clusters" << std::endl;
    RawClusterContainer::ConstRange clusterEnd = clusterContainer->getClusters();
    RawClusterContainer::ConstIterator clusterIter;
    for (clusterIter = clusterEnd.first; clusterIter != clusterEnd.second; clusterIter++)
    {
      RawCluster *recoCluster = clusterIter->second;

      CLHEP::Hep3Vector hep_vertex(0, 0, 0);
      if (vertex != -9999)
      {
        hep_vertex.setZ(vertex);
      }
      CLHEP::Hep3Vector E_vec_cluster = RawClusterUtility::GetECoreVec(*recoCluster, hep_vertex);
      CLHEP::Hep3Vector E_vec_cluster_Full = RawClusterUtility::GetEVec(*recoCluster, hep_vertex);

      float clusE = E_vec_cluster_Full.mag();
      float clusEcore = E_vec_cluster.mag();
      float clus_eta = E_vec_cluster.pseudoRapidity();
      float clus_phi = E_vec_cluster.phi();
      float clus_pt = E_vec_cluster.perp();

      h_clusInfo->Fill(clus_eta, clus_phi, clusE);
      h_clusPt->Fill(clus_pt);
      h_clusEcore->Fill(clusEcore);
    }
  }

  // pi0 reconstruction
  if (doPi0Reco && storeEMCal)
  {
    std::cout << "Reconstructing dicluster pairs" << std::endl;
    RawClusterContainer::ConstRange clusters = clusterContainer->getClusters();
    RawClusterContainer::ConstIterator clusterIter1, clusterIter2;
    int clusterCounter1 = 0;
    int clusterCounter2 = 0;

    for (clusterIter1 = clusters.first; clusterIter1 != clusters.second; clusterIter1++)
    {
      RawCluster *recoCluster1 = clusterIter1->second;
      clusterCounter1++;
      if (recoCluster1->get_chi2() > 4)
      {
        continue;
      }

      CLHEP::Hep3Vector hep_vertex(0, 0, 0);
      if (vertex != -9999)
      {
        hep_vertex.setZ(vertex);
      }
      CLHEP::Hep3Vector E_vec_cluster1 = RawClusterUtility::GetECoreVec(*recoCluster1, hep_vertex);

      for (clusterIter2 = clusters.first; clusterIter2 != clusters.second; clusterIter2++)
      {
	clusterCounter2++;
        if (clusterCounter2 <= clusterCounter1)
        {
          continue;  // prevents double counting pairs
        }
        RawCluster *recoCluster2 = clusterIter2->second;
        CLHEP::Hep3Vector E_vec_cluster2 = RawClusterUtility::GetECoreVec(*recoCluster2, hep_vertex);
        if (recoCluster2->get_chi2() > 4)
        {
          continue;
        }
        if (E_vec_cluster1.mag() < clusEMin || E_vec_cluster2.mag() < clusEMin)
        {
          continue;
        }
        if (std::fabs(E_vec_cluster1.mag() - E_vec_cluster2.mag()) / (E_vec_cluster1.mag() + E_vec_cluster2.mag()) > maxAlpha)
        {
          continue;
        }
        TLorentzVector photon1, photon2, pi0;
        photon1.SetPtEtaPhiE(E_vec_cluster1.perp(), E_vec_cluster1.pseudoRapidity(), E_vec_cluster1.phi(), E_vec_cluster1.mag());
        photon2.SetPtEtaPhiE(E_vec_cluster2.perp(), E_vec_cluster2.pseudoRapidity(), E_vec_cluster2.phi(), E_vec_cluster2.mag());
        pi0 = photon1 + photon2;
        h_diPhotonMassE->Fill(pi0.M(), pi0.E());
      }
    }
  }
  // tower information
  TowerInfoContainer *ohcTowerContainer = findNode::getClass<TowerInfoContainer>(topNode, m_ohcTowerNode);
  TowerInfoContainer *ihcTowerContainer = findNode::getClass<TowerInfoContainer>(topNode, m_ihcTowerNode.c_str());

  if (!ohcTowerContainer || !ihcTowerContainer)
  {
    std::cout << PHWHERE << "caloHistGen::process_event: " << m_ohcTowerNode << " or " << m_ohcTowerNode << " node is missing. Output related to this node will be empty" << std::endl;
    return Fun4AllReturnCodes::ABORTEVENT;
  }

  if (storeHCals && ohcTowerContainer && ihcTowerContainer)
  {
    tower_range = ohcTowerContainer->size();
    for (unsigned int iter = 0; iter < tower_range; iter++)
    {
      unsigned int towerkey = ohcTowerContainer->encode_key(iter);
      unsigned int ieta = TowerInfoDefs::getCaloTowerEtaBin(towerkey);
      unsigned int iphi = TowerInfoDefs::getCaloTowerPhiBin(towerkey);
      int time = ohcTowerContainer->get_tower_at_channel(iter)->get_time();
      float chi2 = ohcTowerContainer->get_tower_at_channel(iter)->get_chi2();
      double energy = ohcTowerContainer->get_tower_at_channel(iter)->get_energy();
      float pedestal = ohcTowerContainer->get_tower_at_channel(iter)->get_pedestal();

      if (ohcTowerContainer->get_tower_at_channel(iter)->get_isGood())
      {
        h_OHCalTowE->Fill(ieta, iphi, energy);
        h_OHCalTowChi2->Fill(ieta, iphi, chi2);
        h_OHCalTowTime->Fill(ieta, iphi, time);
        h_OHCalTowPed->Fill(ieta, iphi, pedestal);
      }
    }

    tower_range = ihcTowerContainer->size();
    for (unsigned int iter = 0; iter < tower_range; iter++)
    {
      unsigned int towerkey = ihcTowerContainer->encode_key(iter);
      unsigned int ieta = TowerInfoDefs::getCaloTowerEtaBin(towerkey);
      unsigned int iphi = TowerInfoDefs::getCaloTowerPhiBin(towerkey);
      int time = ihcTowerContainer->get_tower_at_channel(iter)->get_time();
      float chi2 = ihcTowerContainer->get_tower_at_channel(iter)->get_chi2();
      double energy = ihcTowerContainer->get_tower_at_channel(iter)->get_energy();
      float pedestal = ihcTowerContainer->get_tower_at_channel(iter)->get_pedestal();

      if (ihcTowerContainer->get_tower_at_channel(iter)->get_isGood())
      {
        h_IHCalTowE->Fill(ieta, iphi, energy);
        h_IHCalTowChi2->Fill(ieta, iphi, chi2);
        h_IHCalTowTime->Fill(ieta, iphi, time);
        h_IHCalTowPed->Fill(ieta, iphi, pedestal);
      }
    }
  }

  TowerInfoContainer *zdcTowerContainer = findNode::getClass<TowerInfoContainer>(topNode, m_zdcTowerNode.c_str());
  if (!zdcTowerContainer)
  {
    std::cout << PHWHERE << "caloHistGen::process_event: " << m_emcTowerNode << " node is missing. Output related to this node will be empty" << std::endl;
    return Fun4AllReturnCodes::ABORTEVENT;
  }

  if (storeZDC && zdcTowerContainer)
  {
    tower_range = zdcTowerContainer->size();
    float totalZDCSouth = 0;
    float totalZDCNorth = 0;
    for (unsigned int iter = 0; iter < tower_range; iter++)
    {
      int time = zdcTowerContainer->get_tower_at_channel(iter)->get_time();
      if (iter < 13 && iter % 2 == 0)
      {
        h_zdcChanTime->Fill((int) iter / 2, time);
      }

      float energy = zdcTowerContainer->get_tower_at_channel(iter)->get_energy();
      if (iter == 0 || iter == 2 || iter == 4)
      {
        totalZDCSouth += energy;
      }
      if (iter == 8 || iter == 10 || iter == 12)
      {
        totalZDCNorth += energy;
      }
    }
    h_zdcNSE->Fill(0., totalZDCSouth);
    h_zdcNSE->Fill(1., totalZDCNorth);
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int caloHistGen::ResetEvent(PHCompositeNode * /*topNode*/)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int caloHistGen::End(PHCompositeNode * /*topNode*/)
{
  std::cout << "caloHistGen::End(PHCompositeNode *topNode) Saving Histos" << std::endl;

  out->cd();

  h_emcTowE-> Write();
  h_OHCalTowE-> Write();
  h_IHCalTowE-> Write();

  h_emcTowChi2-> Write();
  h_OHCalTowChi2-> Write();
  h_IHCalTowChi2-> Write();

  h_emcTowTime-> Write();
  h_OHCalTowTime-> Write();
  h_IHCalTowTime -> Write();
  
  h_emcTowPed-> Write();
  h_OHCalTowPed->Write();
  h_IHCalTowPed->Write();

  h_clusInfo->Write();
  h_diPhotonEtaPhiE->Write();
  h_diPhotonMassE->Write();
  
  h_clusPt->Write();
  h_clusEcore->Write();
  
  h_clusChi2->Write();

  h_zVertex->Write();
  
  h_zdcNSE->Write();

  h_zdcChanTime->Write();
  
  out->Close();
  delete out;

  return Fun4AllReturnCodes::EVENT_OK;
}


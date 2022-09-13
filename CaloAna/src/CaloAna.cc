#include "CaloAna.h"

// G4Hits includes
#include <g4main/PHG4Hit.h>
#include <g4main/PHG4HitContainer.h>

// G4Cells includes
#include <g4detectors/PHG4Cell.h>
#include <g4detectors/PHG4CellContainer.h>

// Tower includes
#include <calobase/RawTower.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawTowerGeomContainer.h>

// Cluster includes
#include <calobase/RawCluster.h>
#include <calobase/RawClusterContainer.h>

#include <fun4all/Fun4AllHistoManager.h>
#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/getClass.h>

#include <TFile.h>
#include <TNtuple.h>

#include <cassert>
#include <sstream>
#include <string>

using namespace std;

CaloAna::CaloAna(const std::string& name, const std::string& filename)
  : SubsysReco(name)
  , detector("HCALIN")
  , outfilename(filename)
{
}

CaloAna::~CaloAna()
{
  delete hm;
  delete g4hitntuple;
  delete g4cellntuple;
  delete towerntuple;
  delete clusterntuple;
}

int CaloAna::Init(PHCompositeNode*)
{
  hm = new Fun4AllHistoManager(Name());
  // create and register your histos (all types) here
  // TH1 *h1 = new TH1F("h1",....)
  // hm->registerHisto(h1);
  outfile = new TFile(outfilename.c_str(), "RECREATE");
  g4hitntuple = new TNtuple("hitntup", "G4Hits", "x0:y0:z0:x1:y1:z1:edep");
  g4cellntuple = new TNtuple("cellntup", "G4Cells", "phi:eta:edep");
  towerntuple = new TNtuple("towerntup", "Towers", "phi:eta:energy");
  clusterntuple = new TNtuple("clusterntup", "Clusters", "phi:z:energy:towers");
  return 0;
}

int CaloAna::process_event(PHCompositeNode* topNode)
{
  // For the calorimeters we have the following node name convention
  // where detector is the calorimeter name (CEMC, HCALIN, HCALOUT)
  // this is the order in which they are reconstructed
  //  G4HIT_<detector>: G4 Hits
  //  G4CELL_<detector>: Cells (combined hits inside a cell - scintillator, eta/phi bin)
  //  TOWER_SIM_<detector>: simulated tower (before pedestal and noise)
  //  TOWER_RAW_<detector>: Raw Tower (adc/tdc values - from sims or real data)
  //  TOWER_CALIB_<detector>: Calibrated towers
  //  CLUSTER_<detector>: clusters
  process_g4hits(topNode);
  process_g4cells(topNode);
  process_towers(topNode);
  process_clusters(topNode);
  return Fun4AllReturnCodes::EVENT_OK;
}

int CaloAna::process_g4hits(PHCompositeNode* topNode)
{
  ostringstream nodename;

  // loop over the G4Hits
  nodename.str("");
  nodename << "G4HIT_" << detector;
  PHG4HitContainer* hits = findNode::getClass<PHG4HitContainer>(topNode, nodename.str().c_str());
  if (hits)
  {
    // this returns an iterator to the beginning and the end of our G4Hits
    PHG4HitContainer::ConstRange hit_range = hits->getHits();
    for (PHG4HitContainer::ConstIterator hit_iter = hit_range.first; hit_iter != hit_range.second; hit_iter++)

    {
      // the pointer to the G4Hit is hit_iter->second
      g4hitntuple->Fill(hit_iter->second->get_x(0),
                        hit_iter->second->get_y(0),
                        hit_iter->second->get_z(0),
                        hit_iter->second->get_x(1),
                        hit_iter->second->get_y(1),
                        hit_iter->second->get_z(1),
                        hit_iter->second->get_edep());
    }
  }
  return Fun4AllReturnCodes::EVENT_OK;
}

int CaloAna::process_g4cells(PHCompositeNode* topNode)
{
  ostringstream nodename;

  // loop over the G4Hits
  nodename.str("");
  nodename << "G4CELL_" << detector;
  PHG4CellContainer* cells = findNode::getClass<PHG4CellContainer>(topNode, nodename.str());
  if (cells)
  {
    PHG4CellContainer::ConstRange cell_range = cells->getCells();
    int phibin = -999;
    int etabin = -999;
    for (PHG4CellContainer::ConstIterator cell_iter = cell_range.first; cell_iter != cell_range.second; cell_iter++)
    {
      if (cell_iter->second->has_binning(PHG4CellDefs::scintillatorslatbinning))
      {
        phibin = PHG4CellDefs::ScintillatorSlatBinning::get_row(cell_iter->second->get_cellid());
        etabin = PHG4CellDefs::ScintillatorSlatBinning::get_column(cell_iter->second->get_cellid());
      }
      else if (cell_iter->second->has_binning(PHG4CellDefs::sizebinning))
      {
        phibin = PHG4CellDefs::SizeBinning::get_phibin(cell_iter->second->get_cellid());
        etabin = PHG4CellDefs::SizeBinning::get_zbin(cell_iter->second->get_cellid());
      }
      else if (cell_iter->second->has_binning(PHG4CellDefs::spacalbinning))
      {
        phibin = PHG4CellDefs::SpacalBinning::get_phibin(cell_iter->second->get_cellid());
        etabin = PHG4CellDefs::SpacalBinning::get_etabin(cell_iter->second->get_cellid());
      }
      else
      {
        cout << "unknown cell binning, implement 0x" << hex << PHG4CellDefs::get_binning(cell_iter->second->get_cellid()) << dec << endl;
      }
      g4cellntuple->Fill(
          phibin,
          etabin,
          cell_iter->second->get_edep());
    }
  }
  return Fun4AllReturnCodes::EVENT_OK;
}

int CaloAna::process_towers(PHCompositeNode* topNode)
{
  ostringstream nodename;
  ostringstream geonodename;

  // loop over the G4Hits
  nodename.str("");
  nodename << "TOWER_CALIB_" << detector;
  geonodename.str("");
  geonodename << "TOWERGEOM_" << detector;
  RawTowerGeomContainer* towergeom = findNode::getClass<RawTowerGeomContainer>(topNode, geonodename.str().c_str());
  if (!towergeom)
  {
    return Fun4AllReturnCodes::EVENT_OK;
  }
  RawTowerContainer* towers = findNode::getClass<RawTowerContainer>(topNode, nodename.str().c_str());
  if (towers)
  {
    // again pair of iterators to begin and end of tower map
    RawTowerContainer::ConstRange tower_range = towers->getTowers();
    for (RawTowerContainer::ConstIterator tower_iter = tower_range.first; tower_iter != tower_range.second; tower_iter++)

    {
      int phibin = tower_iter->second->get_binphi();
      int etabin = tower_iter->second->get_bineta();
      double phi = towergeom->get_phicenter(phibin);
      double eta = towergeom->get_etacenter(etabin);
      towerntuple->Fill(phi,
                        eta,
                        tower_iter->second->get_energy());
    }
  }
  return Fun4AllReturnCodes::EVENT_OK;
}

int CaloAna::process_clusters(PHCompositeNode* topNode)
{
  ostringstream nodename;

  // loop over the G4Hits
  nodename.str("");
  nodename << "CLUSTER_" << detector;
  RawClusterContainer* clusters = findNode::getClass<RawClusterContainer>(topNode, nodename.str().c_str());
  if (clusters)
  {
    RawClusterContainer::ConstRange cluster_range = clusters->getClusters();
    for (RawClusterContainer::ConstIterator cluster_iter = cluster_range.first; cluster_iter != cluster_range.second; cluster_iter++)
    {
      clusterntuple->Fill(cluster_iter->second->get_phi(),
                          cluster_iter->second->get_z(),
                          cluster_iter->second->get_energy(),
                          cluster_iter->second->getNTowers());
    }
  }
  return Fun4AllReturnCodes::EVENT_OK;
}

int CaloAna::End(PHCompositeNode* /*topNode*/)
{
  outfile->cd();
  g4hitntuple->Write();
  g4cellntuple->Write();
  towerntuple->Write();
  clusterntuple->Write();
  outfile->Write();
  outfile->Close();
  delete outfile;
  hm->dumpHistos(outfilename, "UPDATE");
  return 0;
}

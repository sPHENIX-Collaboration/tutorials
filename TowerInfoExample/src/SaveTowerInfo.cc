#include "SaveTowerInfo.h"

#include <calobase/TowerInfov1.h>
#include <calobase/TowerInfoContainerv1.h>
#include <calobase/RawTowerv1.h>
#include <calobase/RawTowerContainer.h>

#include <phool/getClass.h>
#include <phool/PHCompositeNode.h>
#include <phool/phool.h>

/// Fun4All includes
#include <fun4all/Fun4AllReturnCodes.h>

/// C++ includes
#include <cassert>
#include <sstream>
#include <string>

using namespace std;

/**
 * SaveTowerInfo is a class developed to reconstruct jets containing a D-meson
 * The class can be adapted to tag jets using any kind of particle
 * Author: Antonio Silva (antonio.sphenix@gmail.com)
 */

/**
 * Constructor of module
 */
SaveTowerInfo::SaveTowerInfo(const std::string &name)
  : SubsysReco(name)
  , m_EMCalTowerContainer(0)
{
  /// Initialize variables and trees so we don't accidentally access
  /// memory that was never allocated
}

/**
 * Destructor of module
 */
SaveTowerInfo::~SaveTowerInfo()
{

}

/**
 * Initialize the module and prepare looping over events
 */
int SaveTowerInfo::Init(PHCompositeNode *topNode)
{
  if (Verbosity() > 5)
  {
    cout << "Beginning Init in SaveTowerInfo" << endl;
  }

  CreateNodeTree(topNode);

  return 0;
}

/**
 * Main workhorse function where each event is looped over and
 * data from each event is collected from the node tree for analysis
 */
int SaveTowerInfo::process_event(PHCompositeNode *topNode)
{
  if (Verbosity() > 5)
  {
    cout << "Beginning process_event in AnaTutorial" << endl;
  }

  RawTowerContainer *EMCalRawTowers = findNode::getClass<RawTowerContainer>(topNode, "TOWER_CALIB_CEMC");

  if(!EMCalRawTowers)
  {
    return Fun4AllReturnCodes::DISCARDEVENT;
  }

  RawTowerContainer::ConstRange tower_range = EMCalRawTowers->getTowers();

  int towercounter = 0;

  for (RawTowerContainer::ConstIterator tower_iter = tower_range.first; tower_iter != tower_range.second; tower_iter++)
  {
    TowerInfov1 *towerInfo = new TowerInfov1();
    towerInfo->set_time(tower_iter->second->get_time());
    towerInfo->set_energy(tower_iter->second->get_energy());
    m_EMCalTowerContainer->add(towerInfo,towercounter); //TowerInfo object is added to the Container
    towercounter++;
  }

  return Fun4AllReturnCodes::EVENT_OK;

}

/**
 * End the module and finish any data collection. Clean up any remaining
 * loose ends
 */
int SaveTowerInfo::End(PHCompositeNode * /*topNode*/)
{
  if (Verbosity() > 1)
  {
    cout << "Ending SaveTowerInfo analysis package" << endl;
  }


  if (Verbosity() > 1)
  {
    cout << "Finished SaveTowerInfo analysis package" << endl;
  }

  return 0;
}

void SaveTowerInfo::CreateNodeTree(PHCompositeNode *topNode)
{
  PHNodeIterator nodeItr(topNode);
  // DST node
  dst_node = static_cast<PHCompositeNode *>(
      nodeItr.findFirst("PHCompositeNode", "DST"));
  if (!dst_node)
  {
    std::cout << "PHComposite node created: DST" << std::endl;
    dst_node = new PHCompositeNode("DST");
    topNode->addNode(dst_node);
  }
  // towers
  //Detector is set when the constructor is called. Eta-phi bins will be calculated based on the detector
  m_EMCalTowerContainer = new TowerInfoContainerv1(TowerInfoContainerv1::DETECTOR::EMCAL);
  PHIODataNode<PHObject> *emcal_towerNode = new PHIODataNode<PHObject>(m_EMCalTowerContainer, "TOWERS_CEMC", "PHObject");
  dst_node->addNode(emcal_towerNode);
}

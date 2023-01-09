#include "RunTowerInfo.h"

#include <calobase/TowerInfoContainerv1.h>
#include <calobase/TowerInfov1.h>

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
 * RunTowerInfo is a class developed to reconstruct jets containing a D-meson
 * The class can be adapted to tag jets using any kind of particle
 * Author: Antonio Silva (antonio.sphenix@gmail.com)
 */

/**
 * Constructor of module
 */
RunTowerInfo::RunTowerInfo(const std::string &name)
  : SubsysReco(name)
  , m_EMCalTowerContainer(0)
{
  /// Initialize variables and trees so we don't accidentally access
  /// memory that was never allocated
}

/**
 * Destructor of module
 */
RunTowerInfo::~RunTowerInfo()
{

}

/**
 * Initialize the module and prepare looping over events
 */
int RunTowerInfo::Init(PHCompositeNode * /*topNode*/)
{
  if (Verbosity() > 5)
  {
    cout << "Beginning Init in RunTowerInfo" << endl;
  }

  return 0;
}

/**
 * Main workhorse function where each event is looped over and
 * data from each event is collected from the node tree for analysis
 */
int RunTowerInfo::process_event(PHCompositeNode *topNode)
{
  if (Verbosity() > 5)
  {
    cout << "Beginning process_event in AnaTutorial" << endl;
  }

  m_EMCalTowerContainer = findNode::getClass<TowerInfoContainerv1>(topNode, "TOWERS_CEMC");

  if(!m_EMCalTowerContainer)
  {
    return Fun4AllReturnCodes::DISCARDEVENT;
  }

  std::cout << "********************" << std::endl;

  TowerInfoContainerv1::Range tower_range = m_EMCalTowerContainer->getTowers();
  for (TowerInfoContainerv1::ConstIterator citer = tower_range.first; citer != tower_range.second; citer++)
  {
    //You can get the TowerInfo object from the map and use it to get time and amplitude
    TowerInfov1 *towerInfo = (TowerInfov1*)citer->second;
    std::cout << "Time: " << towerInfo->get_time() << std::endl;
    std::cout << "Energy (Amplitude): " << towerInfo->get_energy() << std::endl;
    //Or just directly get time and amplitude from the object in the map
    std::cout << "Time: " << citer->second->get_time() << std::endl;
    std::cout << "Energy (Amplitude): " << citer->second->get_energy() << std::endl;
    //The encripted key can be use to get the tower eta-phi bin form the TowerInfoContainer
    std::cout << "PhiBin: " << m_EMCalTowerContainer->getTowerPhiBin(citer->first) << std::endl;
    std::cout << "EtaBin: " << m_EMCalTowerContainer->getTowerEtaBin(citer->first) << std::endl;
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

/**
 * End the module and finish any data collection. Clean up any remaining
 * loose ends
 */
int RunTowerInfo::End(PHCompositeNode * /*topNode*/)
{
  if (Verbosity() > 1)
  {
    cout << "Ending RunTowerInfo analysis package" << endl;
  }


  if (Verbosity() > 1)
  {
    cout << "Finished RunTowerInfo analysis package" << endl;
  }

  return 0;
}

#ifndef SAVETOWERINFO_H__
#define SAVETOWERINFO_H__

#include <calobase/TowerInfov1.h>
#include <calobase/TowerInfoContainerv1.h>

#include <fun4all/SubsysReco.h>

/// Class declarations for use in the analysis module
class PHCompositeNode;

/// Definition of this analysis module class
class SaveTowerInfo : public SubsysReco
{
 public:

  /// Constructor
  SaveTowerInfo(const std::string &name = "SaveTowerInfo");

  // Destructor
  virtual ~SaveTowerInfo();

  /// SubsysReco initialize processing method
  int Init(PHCompositeNode *);

  /// SubsysReco event processing method
  int process_event(PHCompositeNode *);

  /// SubsysReco end processing method
  int End(PHCompositeNode *);

  void CreateNodeTree(PHCompositeNode *topNode);


 private:
  /// String to contain the outfile name containing the trees
  TowerInfoContainerv1 *m_EMCalTowerContainer; //! Container
  PHCompositeNode *dst_node;


  /**
   * Make variables for the relevant trees
   */


};

#endif

#ifndef RUNTOWERINFO_H__
#define RUNTOWERINFO_H__

#include <calobase/TowerInfoContainerv1.h>

#include <fun4all/SubsysReco.h>

/// Class declarations for use in the analysis module
class PHCompositeNode;

/// Definition of this analysis module class
class RunTowerInfo : public SubsysReco
{
 public:

  /// Constructor
  RunTowerInfo(const std::string &name = "RunTowerInfo");

  // Destructor
  virtual ~RunTowerInfo();

  /// SubsysReco initialize processing method
  int Init(PHCompositeNode *);

  /// SubsysReco event processing method
  int process_event(PHCompositeNode *);

  /// SubsysReco end processing method
  int End(PHCompositeNode *);


 private:
  /// String to contain the outfile name containing the trees
  TowerInfoContainerv1 *m_EMCalTowerContainer; //! Container


  /**
   * Make variables for the relevant trees
   */


};

#endif

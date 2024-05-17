// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef CALOTREEGEN_H
#define CALOTREEGEN_H

#include <fun4all/SubsysReco.h>

#include <string>
#include <vector>
#include <TTree.h>

class TTree;
class PHCompositeNode;
class Fun4AllHistoManager;
class TFile;
class RawCluster;
class TowerInfoContainer;
class TH1F;

class caloTreeGen : public SubsysReco
{
 public:

  caloTreeGen(const std::string &name = "caloTreeGen");

  ~caloTreeGen() override;

  /** Called during initialization.
      Typically this is where you can book histograms, and e.g.
      register them to Fun4AllServer (so they can be output to file
      using Fun4AllServer::dumpHistos() method).
  */
  int Init(PHCompositeNode *topNode) override;

  /** Called for first event when run number is known.
      Typically this is where you may want to fetch data from
      database, because you know the run number. A place
      to book histograms which have to know the run number.
  */
  int InitRun(PHCompositeNode *topNode) override;

  /** Called for each event.
      This is where you do the real work.
  */
  int process_event(PHCompositeNode *topNode) override;

  /// Clean up internals after each event.
  int ResetEvent(PHCompositeNode *topNode) override;

  /// Called at the end of each run.
  int EndRun(const int runnumber) override;

  /// Called at the end of all processing.
  int End(PHCompositeNode *topNode) override;

  /// Reset
  int Reset(PHCompositeNode * /*topNode*/) override;

  void Print(const std::string &what = "ALL") const override;

  void doClusters(int clusters, std::string clusterNode)  {storeClusters = clusters; m_clusterNode = clusterNode;}

  void doClusterDetails(int fineCluster) {storeClusterDetails = fineCluster;}
  
  void doEMCal(int emcalOn, std::string emcNode) {storeEMCal = emcalOn; m_emcTowerNode = emcNode;}

  void doHCals(int hcalsOn, std::string ohcNode, std::string ihcNode) {storeHCals = hcalsOn; m_ohcTowerNode = ohcNode; m_ihcTowerNode = ihcNode;}

  void doZDC(int zdcOn, std::string zdcNode) {storeZDC = zdcOn; m_zdcTowerNode = zdcNode;}
  
  void doTrig(int trigOn, std::string trigNode) {storeTrig = trigOn; m_trigNode = trigNode;}

 private:

  TTree *T;
  //EMCal
  std::vector<float> m_emcTowE;
  std::vector<float> m_emciEta;
  std::vector<float> m_emciPhi;
  std::vector<int> m_emcTime;
  std::vector<float> m_emcChi2;
  std::vector<float> m_emcPed;
  
  //OHCal
  std::vector<float> m_ohciTowPhi;
  std::vector<float> m_ohciTowEta;
  std::vector<float> m_ohcTowE;
  std::vector<int> m_ohcTime;
  std::vector<float> m_ohcChi2;
  std::vector<float> m_ohcPed;
    
  //IHCal
  std::vector<float> m_ihciTowPhi;
  std::vector<float> m_ihciTowEta;
  std::vector<float> m_ihcTowE;
  std::vector<int> m_ihcTime;
  std::vector<float> m_ihcChi2;
  std::vector<float> m_ihcPed;

  //ZDC
  std::vector<float> m_zdcTowE;
  std::vector<int> m_zdcSide;
  
  //SMD
  std::vector<float> m_smdE;
  std::vector<int> m_smdSide;
  
  //Clusters
  std::vector<float> m_clusterE;
  std::vector<float> m_clusterPhi;
  std::vector<float> m_clusterEta;
  std::vector<float> m_clusterPt;
  std::vector<float> m_clusterChi;
  std::vector<float> m_clusterNtow;
  std::vector<float> m_clusterTowMaxE;
  std::vector<float> m_clusterECore;
  
  std::vector<std::vector<int> > m_clusTowEta;
  std::vector<std::vector<int> > m_clusTowPhi;
  std::vector<std::vector<float> > m_clusTowE;

  //GL1 information
  std::vector<bool> m_triggerVector;
  
  float m_vertex;

  TFile *out;
  //Fun4AllHistoManager *hm = nullptr;
  std::string Outfile = "commissioning.root";

  TH1F *zVertex;
  
  float getMaxTowerE(RawCluster *cluster, TowerInfoContainer *towerContainer);
  std::vector<float> returnClusterTowE(RawCluster *cluster, TowerInfoContainer *towerContainer);
  std::vector<int> returnClusterTowPhi(RawCluster *cluster, TowerInfoContainer *towerContainer);
  std::vector<int> returnClusterTowEta(RawCluster *cluster, TowerInfoContainer *towerContainer);

  
  float totalCaloEEMCal;
  float totalCaloEOHCal;
  float totalCaloEIHCal;
  float totalCaloEZDC;
  float totalChargeMBD;

  int storeClusters = 1;
  int storeClusterDetails = 1;
  int storeEMCal = 1;
  int storeHCals = 1;
  int storeZDC = 1;
  int storeTrig = 1;
  
  std::string m_trigNode;
  std::string m_emcTowerNode;
  std::string m_ohcTowerNode;
  std::string m_ihcTowerNode;
  std::string m_zdcTowerNode;
  std::string m_clusterNode;
  
};

#endif 

// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef CALOHISTGEN_H
#define CALOHISTGEN_H

#include <fun4all/SubsysReco.h>

#include <limits>
#include <string>
#include <cstring> 

class PHCompositeNode;
class RawCluster;
class TFile;
class TH1F;
class TH2F;
class TH3F;
class TowerInfoContainer;

class caloHistGen : public SubsysReco
{
 public:
  caloHistGen(const std::string &name = "caloHistGen");

  ~caloHistGen() override = default;

  /** Called during initialization.
      Typically this is where you can book histograms, and e.g.
      register them to Fun4AllServer (so they can be output to file
      using Fun4AllServer::dumpHistos() method).
  */
  int Init(PHCompositeNode *topNode) override;

  /** Called for each event.
      This is where you do the real work.
  */
  int process_event(PHCompositeNode *topNode) override;

  /// Clean up internals after each event.
  int ResetEvent(PHCompositeNode *topNode) override;

  /// Called at the end of all processing.
  int End(PHCompositeNode *topNode) override;

  void doClusters(int clusters, const std::string &clusterNode)
  {
    storeClusters = clusters;
    m_clusterNode = clusterNode;
  }

  void doEMCal(int emcalOn, const std::string &emcNode)
  {
    storeEMCal = emcalOn;
    m_emcTowerNode = emcNode;
  }

  void doHCals(int hcalsOn, const std::string &ohcNode, const std::string &ihcNode)
  {
    storeHCals = hcalsOn;
    m_ohcTowerNode = ohcNode;
    m_ihcTowerNode = ihcNode;
  }

  void doZDC(int zdcOn, const std::string &zdcNode)
  {
    storeZDC = zdcOn;
    m_zdcTowerNode = zdcNode;
  }

  void setCaloFrac(float frac)
  {
    caloFrac = frac;
  }

  void doTrig(int trigOn, const std::string &trigNode)
  {
    checkTrig = trigOn;
    m_trigNode = trigNode;
  }

  void setPi0Reco(int doPi0)
  {
    doPi0Reco = doPi0;
  }
  
  void setMaxAlpha(float alphaMax)
  {
    maxAlpha = alphaMax;
  }
  
  void setCluster1EMin(float minClusE)
  {
    clus1EMin = minClusE;
  }

  void setCluster2EMin(float minClusE)
  {
    clus2EMin = minClusE;
  }

  void setIsAuAu(bool isHI)
  {
    isAuAu = isHI;
  }

  float getTotalCaloEnergy(TowerInfoContainer *towerContainer);
  
  void peripheralOnlyPi0(bool doPeriphOnly)
  {
    peripheralOnly = doPeriphOnly;
  }

  void setTrig(const char *selection)
  {
    if (!strcmp(selection, "minBias"))
      {
	trigRequired[10] = 1;
      }
    if (!strcmp(selection, "minBias10"))
      {
	trigRequired[12] = 1;
      }
    if (!strcmp(selection, "minBias30"))
      {
	trigRequired[13] = 1;
      }
    if (!strcmp(selection, "minBias60"))
      {
	trigRequired[14] = 1;
      }
    if (!strcmp(selection, "photon"))
      {
	trigRequired[28] = 1;
	trigRequired[29] = 1;
	trigRequired[30] = 1;
	trigRequired[31] = 1;
      }
    if (!strcmp(selection, "jet"))
      {
	trigRequired[20] = 1;
	trigRequired[21] = 1;
	trigRequired[22] = 1;
	trigRequired[23] = 1;
      }
    if (!strcmp(selection, "photonMBD"))
      {
	trigRequired[24] = 1;
	trigRequired[25] = 1;
	trigRequired[26] = 1;
	trigRequired[27] = 1;
      }
    if (!strcmp(selection, "jetMBD"))
      {
	trigRequired[16] = 1;
	trigRequired[17] = 1;
	trigRequired[18] = 1;
	trigRequired[19] = 1;
      }
  }
 
 private:
  // private methods
 
  // pointers

  TFile *out{nullptr};


  TH3F *h_emcTowE{nullptr};
  TH3F * h_OHCalTowE{nullptr};
  TH3F *h_IHCalTowE{nullptr};

  TH3F *h_emcTowChi2{nullptr};
  TH3F *h_OHCalTowChi2{nullptr};
  TH3F *h_IHCalTowChi2{nullptr};

  TH3F *h_emcTowTime{nullptr};
  TH3F *h_OHCalTowTime{nullptr};
  TH3F *h_IHCalTowTime {nullptr};

  TH3F *h_emcTowPed{nullptr};
  TH3F *h_OHCalTowPed{nullptr};
  TH3F *h_IHCalTowPed{nullptr};

  TH3F *h_clusInfo{nullptr};
  TH3F *h_diPhotonEtaPhiE{nullptr};
  TH2F *h_diPhotonMassE{nullptr};
  
  TH1F *h_clusPt{nullptr};
  TH1F *h_clusEcore{nullptr};
  
  TH1F * h_clusChi2{nullptr};

  TH1F *h_zVertex{nullptr};
  
  TH2F *h_zdcNSE{nullptr};

  TH2F *h_zdcChanTime{nullptr};

  
  
  // simple variables
  bool peripheralOnly{false};
  bool isAuAu{false};
  int storeClusters{1};
  int storeEMCal{1};
  int storeHCals{1};
  int storeZDC{1};
  int trigRequired[64]{0};
  int doPi0Reco{0};
  int checkTrig{1};
  float maxAlpha{0.7};
  float clus1EMin{0.5};
  float clus2EMin{0.5};
  float emcaldownscale{1350000. / 800.};
  float caloFrac{0.05};
  std::string m_clusterNode;
  std::string m_emcTowerNode;
  std::string m_ihcTowerNode;
  std::string m_ohcTowerNode;
  std::string m_trigNode;
  std::string m_zdcTowerNode;
  
  std::string Outfile{"commissioning.root"};

  

};

#endif

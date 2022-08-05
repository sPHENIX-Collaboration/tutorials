#ifndef CALOJETRHOEST_H
#define CALOJETRHOEST_H

#include <fun4all/SubsysReco.h>

#include <memory>
#include <string>
#include <utility>  // std::pair, std::make_pair

#include <array>
#include <vector>

class PHCompositeNode;
class JetEvalStack;
class TTree;
class TH1;
class JetInput;

/// \class CaloJetRhoEst
class CaloJetRhoEst : public SubsysReco
{
 public:
  CaloJetRhoEst(
      const std::string &recojetname = "AntiKt_Tower_r04",
      const std::string &truthjetname = "AntiKt_Truth_r04",
      const std::string &outputfilename = "CaloJetRhoEst.root");

  virtual ~CaloJetRhoEst();

  //! set eta range
  void
  setEtaRange(double low, double high)
  {
    m_etaRange.first  = low;
    m_etaRange.second = high;
  }
  //! set eta range
  void
  setPtRange(double low, double high)
  {
    m_ptRange.first  = low;
    m_ptRange.second = high;
  }
  /* void use_initial_vertex(const bool b = true) {initial_vertex = b;} */
  int Init          (PHCompositeNode *topNode);
  int InitRun       (PHCompositeNode *topNode);
  int process_event (PHCompositeNode *topNode);
  int End           (PHCompositeNode *topNode);
  void clear_vectors();
  void add_input(JetInput *input) { _inputs.push_back(input); }

 private:
  std::string m_recoJetName;
  std::string m_truthJetName;
  std::string m_outputFileName;

  //! eta range
  std::pair<double, double> m_etaRange;

  //! pT range
  std::pair<double, double> m_ptRange;

  //! flag to use initial vertex in track evaluator
  bool initial_vertex = false;

  //! Output Tree variables
  TTree *m_T;
  int   m_id;
  float m_rho;
  float m_centrality;
  float m_impactparam;

  //Calo Jets
  std::vector<float> m_eta;
  std::vector<float> m_phi;
  std::vector<float> m_e;
  std::vector<float> m_pt;
  std::vector<float> m_area;

  //Truth Jets
  std::vector<float> m_truthEta;
  std::vector<float> m_truthPhi;
  std::vector<float> m_truthE;
  std::vector<float> m_truthPt;
  std::vector<float> m_truthArea;

  std::vector<JetInput *> _inputs; // copied from /direct/sphenix+u/dstewart/vv/coresoftware/simulation/g4simulation/g4jets/JetReco.h .cc
};

#endif  // CALOJETRHOEST_H_H

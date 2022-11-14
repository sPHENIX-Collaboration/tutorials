#ifndef MYJETANALYSIS_H
#define MYJETANALYSIS_H

#include <fun4all/SubsysReco.h>

#include <g4eval/JetEvalStack.h>

#include <array>
#include <limits>
#include <memory>
#include <string>
#include <utility>  // std::pair, std::make_pair

class PHCompositeNode;
class TTree;
class TH1;

/// \class MyJetAnalysis
class MyJetAnalysis : public SubsysReco
{
 public:
  MyJetAnalysis(
      const std::string &recojetname = "AntiKt_Tower_r04",
      const std::string &truthjetname = "AntiKt_Truth_r04",
      const std::string &outputfilename = "myjetanalysis.root");

  ~MyJetAnalysis() override = default;

  //! set eta range
  void
  setEtaRange(double low, double high)
  {
    m_etaRange.first = low;
    m_etaRange.second = high;
  }
  //! set eta range
  void
  setPtRange(double low, double high)
  {
    m_ptRange.first = low;
    m_ptRange.second = high;
  }
  void use_initial_vertex(const bool b = true) {initial_vertex = b;}
  int Init(PHCompositeNode *topNode) override;
  int InitRun(PHCompositeNode *topNode) override;
  int process_event(PHCompositeNode *topNode) override;
  int End(PHCompositeNode *topNode) override;

 private:
  //! cache the jet evaluation modules
  std::shared_ptr<JetEvalStack> m_jetEvalStack;

  std::string m_recoJetName;
  std::string m_truthJetName;
  std::string m_outputFileName;

  //! eta range
  std::pair<double, double> m_etaRange;

  //! pT range
  std::pair<double, double> m_ptRange;

  //! flag to use initial vertex in track evaluator
  bool initial_vertex = false;

  //! max track-jet matching radius
  double m_trackJetMatchingRadius = std::numeric_limits<float>::signaling_NaN();

  //! Output histograms
  TH1 *m_hInclusiveE = nullptr;
  TH1 *m_hInclusiveEta = nullptr;
  TH1 *m_hInclusivePhi = nullptr;

  //! Output Tree variables
  TTree *m_T = nullptr;

  int m_event = -1;
  int m_id = -1;
  int m_nComponent = -1;
  float m_eta = std::numeric_limits<float>::signaling_NaN();
  float m_phi = std::numeric_limits<float>::signaling_NaN();
  float m_e = std::numeric_limits<float>::signaling_NaN();
  float m_pt = std::numeric_limits<float>::signaling_NaN();

  int m_truthID = -1;
  int m_truthNComponent = -1;
  float m_truthEta = std::numeric_limits<float>::signaling_NaN();
  float m_truthPhi = std::numeric_limits<float>::signaling_NaN();
  float m_truthE = std::numeric_limits<float>::signaling_NaN();
  float m_truthPt = std::numeric_limits<float>::signaling_NaN();

  //! number of matched tracks
  int m_nMatchedTrack = -1;

  enum
  {
    //! max number of tracks
    kMaxMatchedTrack = 1000
  };
  std::array<float, kMaxMatchedTrack> m_trackdR;
  std::array<float, kMaxMatchedTrack> m_trackpT;
};

#endif  // MYJETANALYSIS_H

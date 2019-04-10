#ifndef __MyJetAnalysis_H__
#define __MyJetAnalysis_H__

#include <fun4all/SubsysReco.h>

#include <memory>
#include <string>
#include <utility>  // std::pair, std::make_pair

#ifndef __CINT__
#include <array>
#endif  // #ifndef __CINT__

class PHCompositeNode;
class JetEvalStack;
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

  virtual ~MyJetAnalysis();

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

  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

 private:

#ifndef __CINT__

  //! cache the jet evaluation modules
  std::shared_ptr<JetEvalStack> m_jetEvalStack;


  std::string m_recoJetName;
  std::string m_truthJetName;
  std::string m_outputFileName;

  //! eta range
  std::pair<double, double> m_etaRange;

  //! pT range
  std::pair<double, double> m_ptRange;

  //! max track-jet matching radius
  double m_trackJetMatchingRadius;

  //! Output histograms
  TH1 *m_hInclusiveE;
  TH1 *m_hInclusiveEta;
  TH1 *m_hInclusivePhi;

  //! Output Tree variables
  TTree *m_T;

  int m_event;
  int m_id;
  int m_nComponent;
  float m_eta;
  float m_phi;
  float m_e;
  float m_pt;

  int m_truthID;
  int m_truthNComponent;
  float m_truthEta;
  float m_truthPhi;
  float m_truthE;
  float m_truthPt;

  //! number of matched tracks
  int m_nMatchedTrack;

  enum
  {
    //! max number of tracks
    kMaxMatchedTrack = 1000
  };
  std::array<float, kMaxMatchedTrack> m_trackdR;
  std::array<float, kMaxMatchedTrack> m_trackpT;

#endif  // #ifndef __CINT__
};

#endif  // __CALOEVALUATOR_H__

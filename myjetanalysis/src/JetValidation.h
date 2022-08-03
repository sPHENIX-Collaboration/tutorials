// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef JETVALIDATION_H
#define JETVALIDATION_H

#include <fun4all/SubsysReco.h>
#include <g4jets/Jetv1.h>

#include <string>
#include <vector>

class PHCompositeNode;
class TTree;

class JetValidation : public SubsysReco
{
 public:

  JetValidation(const std::string &recojetname = "AntiKt_Tower_r04",
		const std::string &truthjetname = "AntiKt_Truth_r04",
		const std::string &outputfilename = "myjetanalysis.root");

  ~JetValidation() override;

  void
    setEtaRange(double low, double high)
  {
    m_etaRange.first = low;
    m_etaRange.second = high;
  }
 void
   setPtRange(double low, double high)
 {
   m_ptRange.first = low;
   m_ptRange.second = high;
 }
 void
   doTruth(int flag)
 {
   m_doTruthJets = flag;
 }
 void
   doSeeds(int flag)
 {
   m_doSeeds = flag;
 }
 void
   doUnsub(int flag)
 {
   m_doUnsubJet = flag;
 }
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

 private:
  std::string m_recoJetName;
  std::string m_truthJetName;
  std::string m_outputFileName;
  std::pair<double, double> m_etaRange;
  std::pair<double, double> m_ptRange;
  int m_doTruthJets;
  int m_doSeeds;
  int m_doUnsubJet;

  //! Output Tree variables
  TTree *m_T;

  //! eventwise quantities
  int m_event;
  int m_nTruthJet;
  int m_nJet;
  float m_totalCalo;
  int m_centrality;
  float m_impactparam;

  //! reconstructed jets
  std::vector<int> m_id;
  std::vector<int> m_nComponent;
  std::vector<float> m_eta;
  std::vector<float> m_phi;
  std::vector<float> m_e;
  std::vector<float> m_pt;

  //! unsubtracted jets
  std::vector<float> m_unsub_pt;
  std::vector<float> m_sub_et;

  //! truth jets
  std::vector<int> m_truthID;
  std::vector<int> m_truthNComponent;
  std::vector<float> m_truthEta;
  std::vector<float> m_truthPhi;
  std::vector<float> m_truthE;
  std::vector<float> m_truthPt;
  std::vector<float> m_truthdR;

  //! seed jets
  std::vector<float> m_eta_rawseed;
  std::vector<float> m_phi_rawseed;
  std::vector<float> m_pt_rawseed;
  std::vector<float> m_e_rawseed;
  std::vector<int> m_rawseed_cut;
  std::vector<float> m_eta_subseed;
  std::vector<float> m_phi_subseed;
  std::vector<float> m_pt_subseed;
  std::vector<float> m_e_subseed;
  std::vector<int> m_subseed_cut;
};

#endif // JETVALIDATION_H

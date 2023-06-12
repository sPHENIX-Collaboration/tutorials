#include "MyJetAnalysis.h"

#include <trackbase_historic/SvtxTrackMap.h>

#include <jetbase/JetMap.h>

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/PHTFileServer.h>

#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>

#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TString.h>
#include <TTree.h>
#include <TVector3.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

MyJetAnalysis::MyJetAnalysis(const std::string& recojetname, const std::string& truthjetname, const std::string& outputfilename)
  : SubsysReco("MyJetAnalysis_" + recojetname + "_" + truthjetname)
  , m_recoJetName(recojetname)
  , m_truthJetName(truthjetname)
  , m_outputFileName(outputfilename)
  , m_etaRange(-1, 1)
  , m_ptRange(5, 100)
  , m_trackJetMatchingRadius(.7)
{
  m_trackdR.fill(std::numeric_limits<float>::signaling_NaN());
  m_trackpT.fill(std::numeric_limits<float>::signaling_NaN());
}

int MyJetAnalysis::Init(PHCompositeNode* topNode)
{
  if (Verbosity() >= MyJetAnalysis::VERBOSITY_SOME)
    std::cout << "MyJetAnalysis::Init - Outoput to " << m_outputFileName << std::endl;

  PHTFileServer::get().open(m_outputFileName, "RECREATE");

  // Histograms
  m_hInclusiveE = new TH1F(
      "hInclusive_E",  //
      TString(m_recoJetName) + " inclusive jet E;Total jet energy (GeV)", 100, 0, 100);

  m_hInclusiveEta =
      new TH1F(
          "hInclusive_eta",  //
          TString(m_recoJetName) + " inclusive jet #eta;#eta;Jet energy density", 50, -1, 1);
  m_hInclusivePhi =
      new TH1F(
          "hInclusive_phi",  //
          TString(m_recoJetName) + " inclusive jet #phi;#phi;Jet energy density", 50, -M_PI, M_PI);

  //Trees
  m_T = new TTree("T", "MyJetAnalysis Tree");

  //      int m_event;
  m_T->Branch("m_event", &m_event, "event/I");
  //      int m_id;
  m_T->Branch("id", &m_id, "id/I");
  //      int m_nComponent;
  m_T->Branch("nComponent", &m_nComponent, "nComponent/I");
  //      float m_eta;
  m_T->Branch("eta", &m_eta, "eta/F");
  //      float m_phi;
  m_T->Branch("phi", &m_phi, "phi/F");
  //      float m_e;
  m_T->Branch("e", &m_e, "e/F");
  //      float m_pt;
  m_T->Branch("pt", &m_pt, "pt/F");
  //
  //      int m_truthID;
  m_T->Branch("truthID", &m_truthID, "truthID/I");
  //      int m_truthNComponent;
  m_T->Branch("truthNComponent", &m_truthNComponent, "truthNComponent/I");
  //      float m_truthEta;
  m_T->Branch("truthEta", &m_truthEta, "truthEta/F");
  //      float m_truthPhi;
  m_T->Branch("truthPhi", &m_truthPhi, "truthPhi/F");
  //      float m_truthE;
  m_T->Branch("truthE", &m_truthE, "truthE/F");
  //      float m_truthPt;
  m_T->Branch("truthPt", &m_truthPt, "truthPt/F");
  //
  //      //! number of matched tracks
  //      int m_nMatchedTrack;
  m_T->Branch("nMatchedTrack", &m_nMatchedTrack, "nMatchedTrack/I");
  //      std::array<float, kMaxMatchedTrack> m_trackdR;
  m_T->Branch("id", m_trackdR.data(), "trackdR[nMatchedTrack]/F");
  //      std::array<float, kMaxMatchedTrack> m_trackpT;
  m_T->Branch("id", m_trackpT.data(), "trackpT[nMatchedTrack]/F");

  return Fun4AllReturnCodes::EVENT_OK;
}

int MyJetAnalysis::End(PHCompositeNode* topNode)
{
  std::cout << "MyJetAnalysis::End - Output to " << m_outputFileName << std::endl;
  PHTFileServer::get().cd(m_outputFileName);

  m_hInclusiveE->Write();
  m_hInclusiveEta->Write();
  m_hInclusivePhi->Write();
  m_T->Write();

  return Fun4AllReturnCodes::EVENT_OK;
}

int MyJetAnalysis::InitRun(PHCompositeNode* topNode)
{
  m_jetEvalStack = std::shared_ptr<JetEvalStack>(new JetEvalStack(topNode, m_recoJetName, m_truthJetName));
  m_jetEvalStack->get_stvx_eval_stack()->set_use_initial_vertex(initial_vertex);
  return Fun4AllReturnCodes::EVENT_OK;
}

int MyJetAnalysis::process_event(PHCompositeNode* topNode)
{
  if (Verbosity() >= MyJetAnalysis::VERBOSITY_SOME)
    std::cout << "MyJetAnalysis::process_event() entered" << std::endl;

  m_jetEvalStack->next_event(topNode);
  JetRecoEval* recoeval = m_jetEvalStack->get_reco_eval();
  ++m_event;

  // interface to jets
  JetMap* jets = findNode::getClass<JetMap>(topNode, m_recoJetName);
  if (!jets)
  {
    std::cout
        << "MyJetAnalysis::process_event - Error can not find DST JetMap node "
        << m_recoJetName << std::endl;
    exit(-1);
  }

  // interface to tracks
  SvtxTrackMap* trackmap = findNode::getClass<SvtxTrackMap>(topNode, "SvtxTrackMap");
  if (!trackmap)
  {
    trackmap = findNode::getClass<SvtxTrackMap>(topNode, "TrackMap");
    if (!trackmap)
    {
      std::cout
          << "MyJetAnalysis::process_event - Error can not find DST trackmap node SvtxTrackMap" << std::endl;
      exit(-1);
    }
  }
  for (JetMap::Iter iter = jets->begin(); iter != jets->end(); ++iter)
  {
    Jet* jet = iter->second;
    assert(jet);

    bool eta_cut = (jet->get_eta() >= m_etaRange.first) and (jet->get_eta() <= m_etaRange.second);
    bool pt_cut = (jet->get_pt() >= m_ptRange.first) and (jet->get_pt() <= m_ptRange.second);
    if ((not eta_cut) or (not pt_cut))
    {
      if (Verbosity() >= MyJetAnalysis::VERBOSITY_MORE)
      {
        std::cout << "MyJetAnalysis::process_event() - jet failed acceptance cut: ";
        std::cout << "eta cut: " << eta_cut << ", ptcut: " << pt_cut << std::endl;
        std::cout << "jet eta: " << jet->get_eta() << ", jet pt: " << jet->get_pt() << std::endl;
        jet->identify();
      }
      continue;
    }

    // fill histograms
    assert(m_hInclusiveE);
    m_hInclusiveE->Fill(jet->get_e());
    assert(m_hInclusiveEta);
    m_hInclusiveEta->Fill(jet->get_eta());
    assert(m_hInclusivePhi);
    m_hInclusivePhi->Fill(jet->get_phi());

    // fill trees - jet spectrum
    Jet* truthjet = recoeval->max_truth_jet_by_energy(jet);

    m_id = jet->get_id();
    m_nComponent = jet->size_comp();
    m_eta = jet->get_eta();
    m_phi = jet->get_phi();
    m_e = jet->get_e();
    m_pt = jet->get_pt();

    m_truthID = -1;
    m_truthNComponent = -1;
    m_truthEta = NAN;
    m_truthPhi = NAN;
    m_truthE = NAN;
    m_truthPt = NAN;

    if (truthjet)
    {
      m_truthID = truthjet->get_id();
      m_truthNComponent = truthjet->size_comp();
      m_truthEta = truthjet->get_eta();
      m_truthPhi = truthjet->get_phi();
      m_truthE = truthjet->get_e();
      m_truthPt = truthjet->get_pt();
    }

    // fill trees - jet track matching
    m_nMatchedTrack = 0;

    for (SvtxTrackMap::Iter iter = trackmap->begin();
         iter != trackmap->end();
         ++iter)
    {
      SvtxTrack* track = iter->second;

      TVector3 v(track->get_px(), track->get_py(), track->get_pz());
      const double dEta = v.Eta() - m_eta;
      const double dPhi = v.Phi() - m_phi;
      const double dR = sqrt(dEta * dEta + dPhi * dPhi);

      if (dR < m_trackJetMatchingRadius)
      {
        //matched track to jet

        assert(m_nMatchedTrack < kMaxMatchedTrack);

        m_trackdR[m_nMatchedTrack] = dR;
        m_trackpT[m_nMatchedTrack] = v.Perp();

        ++m_nMatchedTrack;
      }

      if (m_nMatchedTrack >= kMaxMatchedTrack)
      {
        std::cout << "MyJetAnalysis::process_event() - reached max track that matching a jet. Quit iterating tracks" << std::endl;
        break;
      }

    }  //    for (SvtxTrackMap::Iter iter = trackmap->begin();

    m_T->Fill();
  }  //   for (JetMap::Iter iter = jets->begin(); iter != jets->end(); ++iter)

  return Fun4AllReturnCodes::EVENT_OK;
}

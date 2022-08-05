#include "CaloJetRhoEst.h"

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/PHTFileServer.h>

#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>

#include <g4eval/JetEvalStack.h>

#include <trackbase_historic/SvtxTrackMap.h>

#include <centrality/CentralityInfo.h>

#include <g4jets/JetMap.h>

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

using namespace std;

CaloJetRhoEst::CaloJetRhoEst(const std::string& recojetname, const std::string& truthjetname, const std::string& outputfilename)
  : SubsysReco("CaloJetRhoEst_" + recojetname + "_" + truthjetname)
  , m_recoJetName(recojetname)
  , m_truthJetName(truthjetname)
  , m_outputFileName(outputfilename)
  , m_etaRange (-1, 1)
  , m_ptRange  (5,  100)
  , m_T  (nullptr)
  , m_id (-1)
  , m_eta       {}
  , m_phi       {}
  , m_e         {}
  , m_pt        {}
  , m_area      {}
  , m_truthEta  {}
  , m_truthPhi  {}
  , m_truthE    {}
  , m_truthPt   {}
  , m_truthArea {}
{ }

CaloJetRhoEst::~CaloJetRhoEst()
{ }

int CaloJetRhoEst::Init(PHCompositeNode* topNode)
{
  if (Verbosity() >= CaloJetRhoEst::VERBOSITY_SOME)
    cout << "CaloJetRhoEst::Init - Outoput to " << m_outputFileName << endl;

  PHTFileServer::get().open(m_outputFileName, "RECREATE");

  //Tree
  m_T = new TTree("T", "CaloJetRhoEst Tree");

  //      int m_event;
  m_T->Branch("m_id",          &m_id);
  m_T->Branch("m_rho",         &m_rho);
  m_T->Branch("m_centrality",  &m_centrality);
  m_T->Branch("m_impactparam", &m_impactparam);

  m_T->Branch("m_rawEta",    &m_eta);
  m_T->Branch("m_rawPhi",    &m_phi);
  m_T->Branch("m_rawE",      &m_e);
  m_T->Branch("m_rawPt",     &m_pt);
  m_T->Branch("m_rawArea",   &m_area);

  //Truth Jets
  m_T->Branch("m_truthEta",  &m_truthEta);
  m_T->Branch("m_truthPhi",  &m_truthPhi);
  m_T->Branch("m_truthE",    &m_truthE);
  m_T->Branch("m_truthPt",   &m_truthPt);
  m_T->Branch("m_truthArea", &m_truthArea);

  return Fun4AllReturnCodes::EVENT_OK;
}

int CaloJetRhoEst::End(PHCompositeNode* topNode)
{
  cout << "CaloJetRhoEst::End - Output to " << m_outputFileName << endl;
  PHTFileServer::get().cd(m_outputFileName);

  /* m_hInclusiveE->Write(); */
  /* m_hInclusiveEta->Write(); */
  /* m_hInclusivePhi->Write(); */
  m_T->Write();

  return Fun4AllReturnCodes::EVENT_OK;
}

int CaloJetRhoEst::InitRun(PHCompositeNode* topNode)
{
  /* m_jetEvalStack = shared_ptr<JetEvalStack>(new JetEvalStack(topNode, m_recoJetName, m_truthJetName)); */
  /* m_jetEvalStack->get_stvx_eval_stack()->set_use_initial_vertex(initial_vertex); */
  topNode->print();
  return Fun4AllReturnCodes::EVENT_OK;
}

int CaloJetRhoEst::process_event(PHCompositeNode* topNode)
{
  /* return Fun4AllReturnCodes::EVENT_OK; // FIXME :: just printing the nodes for now in order to find them */
  ++m_id;
  JetMap* jets = findNode::getClass<JetMap>(topNode, m_recoJetName);
  if (!jets)
  {
    std::cout
      << "MyJetAnalysis::process_event - Error can not find DST Reco JetMap node "
      << m_recoJetName << std::endl;
    exit(-1);
  }

  //interface to truth jets
  JetMap* jetsMC = findNode::getClass<JetMap>(topNode, m_truthJetName);
  if (!jetsMC )
  {
    std::cout
      << "MyJetAnalysis::process_event - Error can not find DST Truth JetMap node "
      << m_truthJetName << std::endl;
    exit(-1);
  }

  //centrality
  /* CentralityInfo* cent_node = findNode::getClass<CentralityInfo>(topNode, "CentralityInfo"); */
  /* if (!cent_node) */
  /* { */
  /*   std::cout */
  /*     << "MyJetAnalysis::process_event - Error can not find centrality node " */
  /*     << std::endl; */
  /*   exit(-1); */
  /* } */

  //get the event centrality/impact parameter from HIJING
  /* m_centrality  =  cent_node->get_centile(CentralityInfo::PROP::bimp); */
  /* m_impactparam =  cent_node->get_quantity(CentralityInfo::PROP::bimp); */

  //get reco jets
  cout << " olives A0 " << endl;
  for (JetMap::Iter iter = jets->begin(); iter != jets->end(); ++iter)
  {
    Jet* jet = iter->second;
    float pt = jet->get_pt();
    float eta = jet->get_eta();
    if  (pt < m_ptRange.first  || pt  > m_ptRange.second
        || eta < m_etaRange.first || eta > m_etaRange.second) continue;

    cout << " olives: A1 " << jet->get_eta() << endl;
    m_eta.push_back(jet->get_eta());
    m_phi.push_back(jet->get_phi());
    m_e.push_back(jet->get_e());
    m_pt.push_back(jet->get_pt());
  }
  for (JetMap::Iter iter = jetsMC->begin(); iter != jetsMC->end(); ++iter)
  {
    Jet* truthjet = iter->second;
    float pt = truthjet->get_pt();
    float eta = truthjet->get_eta();
    if  (pt < m_ptRange.first  || pt  > m_ptRange.second
        || eta < m_etaRange.first || eta > m_etaRange.second) continue;

    m_truthEta.push_back(truthjet->get_eta());
    cout << " olives: A2 MC " << truthjet->get_eta() << endl;
    m_truthPhi.push_back(truthjet->get_phi());
    m_truthE  .push_back(truthjet->get_e());
    m_truthPt .push_back(truthjet->get_pt());
  }
  m_T->Fill();

  return Fun4AllReturnCodes::EVENT_OK;
}

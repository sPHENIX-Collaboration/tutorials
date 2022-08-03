//module for producing a TTree with jet information for doing jet validation studies
// for questions/bugs please contact Virginia Bailey vbailey13@gsu.edu

#include "JetValidation.h"

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/PHTFileServer.h>

#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>

#include <g4jets/JetMap.h>
#include <g4jets/Jetv1.h>

#include <centrality/CentralityInfo.h>

#include <calobase/RawTower.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawTowerGeomContainer.h>

#include <jetbackground/TowerBackground.h>

#include <TTree.h>

//____________________________________________________________________________..
JetValidation::JetValidation(const std::string& recojetname, const std::string& truthjetname, const std::string& outputfilename):
 SubsysReco("JetValidation_" + recojetname + "_" + truthjetname)
 , m_recoJetName(recojetname)
 , m_truthJetName(truthjetname)
 , m_outputFileName(outputfilename)
 , m_etaRange(-1, 1)
 , m_ptRange(5, 100)
 , m_doTruthJets(0)
 , m_doSeeds(0)
 , m_doUnsubJet(0)
 , m_T(nullptr)
 , m_event(-1)
 , m_nTruthJet(-1)
 , m_nJet(-1)
 , m_id()
 , m_nComponent()
 , m_eta()
 , m_phi()
 , m_e()
 , m_pt()
 , m_sub_et()
 , m_truthID()
 , m_truthNComponent()
 , m_truthEta()
 , m_truthPhi()
 , m_truthE()
 , m_truthPt()
 , m_eta_rawseed()
 , m_phi_rawseed()
 , m_pt_rawseed()
 , m_e_rawseed()
 , m_rawseed_cut()
 , m_eta_subseed()
 , m_phi_subseed()
 , m_pt_subseed()
 , m_e_subseed()
 , m_subseed_cut()
{
  std::cout << "JetValidation::JetValidation(const std::string &name) Calling ctor" << std::endl;
}

//____________________________________________________________________________..
JetValidation::~JetValidation()
{
  std::cout << "JetValidation::~JetValidation() Calling dtor" << std::endl;
}

//____________________________________________________________________________..
int JetValidation::Init(PHCompositeNode *topNode)
{
  std::cout << "JetValidation::Init(PHCompositeNode *topNode) Initializing" << std::endl;
  PHTFileServer::get().open(m_outputFileName, "RECREATE");
  std::cout << "JetValidation::Init - Output to " << m_outputFileName << std::endl;

  // configure Tree
  m_T = new TTree("T", "MyJetAnalysis Tree");
  m_T->Branch("m_event", &m_event, "event/I");
  m_T->Branch("nJet", &m_nJet, "nJet/I");
  m_T->Branch("cent", &m_centrality);
  m_T->Branch("b", &m_impactparam);
  m_T->Branch("id", &m_id);
  m_T->Branch("nComponent", &m_nComponent);

  m_T->Branch("eta", &m_eta);
  m_T->Branch("phi", &m_phi);
  m_T->Branch("e", &m_e);
  m_T->Branch("pt", &m_pt);
  if(m_doUnsubJet)
    {
      m_T->Branch("pt_unsub", &m_unsub_pt);
      m_T->Branch("subtracted_et", &m_sub_et);
    }
  if(m_doTruthJets){
    m_T->Branch("nTruthJet", &m_nTruthJet);
    m_T->Branch("truthID", &m_truthID);
    m_T->Branch("truthNComponent", &m_truthNComponent);
    m_T->Branch("truthEta", &m_truthEta);
    m_T->Branch("truthPhi", &m_truthPhi);
    m_T->Branch("truthE", &m_truthE);
    m_T->Branch("truthPt", &m_truthPt);
  }

  if(m_doSeeds){
    m_T->Branch("rawseedEta", &m_eta_rawseed);
    m_T->Branch("rawseedPhi", &m_phi_rawseed);
    m_T->Branch("rawseedPt", &m_pt_rawseed);
    m_T->Branch("rawseedE", &m_e_rawseed);
    m_T->Branch("rawseedCut", &m_rawseed_cut);
    m_T->Branch("subseedEta", &m_eta_subseed);
    m_T->Branch("subseedPhi", &m_phi_subseed);
    m_T->Branch("subseedPt", &m_pt_subseed);
    m_T->Branch("subseedE", &m_e_subseed);
    m_T->Branch("subseedCut", &m_subseed_cut);
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int JetValidation::InitRun(PHCompositeNode *topNode)
{
  std::cout << "JetValidation::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int JetValidation::process_event(PHCompositeNode *topNode)
{
  //std::cout << "JetValidation::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
  ++m_event;

  // interface to reco jets
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
  if (!jetsMC && m_doTruthJets)
    {
      std::cout
	<< "MyJetAnalysis::process_event - Error can not find DST Truth JetMap node "
	<< m_truthJetName << std::endl;
      exit(-1);
    }
  
  // interface to jet seeds
  JetMap* seedjetsraw = findNode::getClass<JetMap>(topNode, "AntiKt_Tower_HIRecoSeedsRaw_r02");
  if (!seedjetsraw && m_doSeeds)
    {
      std::cout
	<< "MyJetAnalysis::process_event - Error can not find DST raw seed jets "
	<< std::endl;
      exit(-1);
    }

  JetMap* seedjetssub = findNode::getClass<JetMap>(topNode, "AntiKt_Tower_HIRecoSeedsSub_r02");
  if (!seedjetssub && m_doSeeds)
    {
      std::cout
	<< "MyJetAnalysis::process_event - Error can not find DST subtracted seed jets "
	<< std::endl;
      exit(-1);
    }

  //centrality
  CentralityInfo* cent_node = findNode::getClass<CentralityInfo>(topNode, "CentralityInfo");
  if (!cent_node)
    {
      std::cout
        << "MyJetAnalysis::process_event - Error can not find centrality node "
        << std::endl;
      exit(-1);
    }

  //calorimeter towers
  RawTowerContainer *towersEM3 = findNode::getClass<RawTowerContainer>(topNode, "TOWER_CALIB_CEMC_RETOWER");
  RawTowerContainer *towersIH3 = findNode::getClass<RawTowerContainer>(topNode, "TOWER_CALIB_HCALIN");
  RawTowerContainer *towersOH3 = findNode::getClass<RawTowerContainer>(topNode, "TOWER_CALIB_HCALOUT");
  RawTowerGeomContainer *tower_geom = findNode::getClass<RawTowerGeomContainer>(topNode, "TOWERGEOM_HCALIN");
  RawTowerGeomContainer *tower_geomOH = findNode::getClass<RawTowerGeomContainer>(topNode, "TOWERGEOM_HCALOUT");
  if(!towersEM3 || !towersIH3 || !towersOH3){
    std::cout
      <<"MyJetAnalysis::process_event - Error can not find raw tower node "
      << std::endl;
    exit(-1);
  }

  if(!tower_geom || !tower_geomOH){
    std::cout
      <<"MyJetAnalysis::process_event - Error can not find raw tower geometry "
      << std::endl;
    exit(-1);
  }

  //underlying event
  TowerBackground *background = findNode::getClass<TowerBackground>(topNode, "TowerBackground_Sub2");
  if(!background){
    std::cout<<"Can't get background. Exiting"<<std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
  }

  //get the event centrality/impact parameter from HIJING
  m_centrality =  cent_node->get_centile(CentralityInfo::PROP::bimp);
  m_impactparam =  cent_node->get_quantity(CentralityInfo::PROP::bimp);

  //get reco jets
  m_nJet = 0;
  float background_v2 = 0;
  float background_Psi2 = 0;
  if(m_doUnsubJet)
    {
      background_v2 = background->get_v2();
      background_Psi2 = background->get_Psi2();
    }
  for (JetMap::Iter iter = jets->begin(); iter != jets->end(); ++iter)
    {

      Jet* jet = iter->second;

      if(jet->get_pt() < 1) continue; // to remove noise jets

      m_id.push_back(jet->get_id());
      m_nComponent.push_back(jet->size_comp());
      m_eta.push_back(jet->get_eta());
      m_phi.push_back(jet->get_phi());
      m_e.push_back(jet->get_e());
      m_pt.push_back(jet->get_pt());

      if(m_doUnsubJet)
	{
	  Jet* unsubjet = new Jetv1();
	  float totalPx = 0;
	  float totalPy = 0;
	  float totalPz = 0;
	  float totalE = 0;
	  int nconst = 0;

	  for (Jet::ConstIter comp = jet->begin_comp(); comp != jet->end_comp(); ++comp)
	    {
	      RawTower *tower;
	      nconst++;

	      if ((*comp).first == 15)
		{
		  tower = towersIH3->getTower((*comp).second);
		  if(!tower || !tower_geom){
		    continue;
		  }
		  float UE = background->get_UE(1).at(tower->get_bineta());
		  float tower_phi = tower_geom->get_tower_geometry(tower->get_key())->get_phi();
		  float tower_eta = tower_geom->get_tower_geometry(tower->get_key())->get_eta();
		  UE = UE * (1 + 2 * background_v2 * cos(2 * (tower_phi - background_Psi2)));
		  totalE += tower->get_energy() + UE;
		  double pt = tower->get_energy() / cosh(tower_eta);
		  totalPx += pt * cos(tower_phi);
		  totalPy += pt * sin(tower_phi);
		  totalPz += pt * sinh(tower_eta);
		}
	      else if ((*comp).first == 16)
		{
		  tower = towersOH3->getTower((*comp).second);
		  if(!tower || !tower_geomOH)
		    {
		      continue;
		    }

		  float UE = background->get_UE(2).at(tower->get_bineta());
		  float tower_phi = tower_geomOH->get_tower_geometry(tower->get_key())->get_phi();
		  float tower_eta = tower_geomOH->get_tower_geometry(tower->get_key())->get_eta();
		  UE = UE * (1 + 2 * background_v2 * cos(2 * (tower_phi - background_Psi2)));
		  totalE +=tower->get_energy() + UE;
		  double pt = tower->get_energy() / cosh(tower_eta);
		  totalPx += pt * cos(tower_phi);
		  totalPy += pt * sin(tower_phi);
		  totalPz += pt * sinh(tower_eta);
		}
	      else if ((*comp).first == 14)
		{
		  tower = towersEM3->getTower((*comp).second);
		  if(!tower || !tower_geom)
		    {
		      continue;
		    }
		  float UE = background->get_UE(0).at(tower->get_bineta());
		  float tower_phi = tower_geom->get_tower_geometry(tower->get_key())->get_phi();
		  float tower_eta = tower_geom->get_tower_geometry(tower->get_key())->get_eta();
		  UE = UE * (1 + 2 * background_v2 * cos(2 * (tower_phi - background_Psi2)));
		  totalE +=tower->get_energy() + UE;
		  double pt = tower->get_energy() / cosh(tower_eta);
		  totalPx += pt * cos(tower_phi);
		  totalPy += pt * sin(tower_phi);
		  totalPz += pt * sinh(tower_eta);
		}
	    }
	  //get unsubtracted jet
	  unsubjet->set_px(totalPx);
	  unsubjet->set_py(totalPy);
	  unsubjet->set_pz(totalPz);
	  unsubjet->set_e(totalE);
	  m_unsub_pt.push_back(unsubjet->get_pt());
	  m_sub_et.push_back(unsubjet->get_et() - jet->get_et());
	}

      m_nJet++;
    }

  //get truth jets
  if(m_doTruthJets)
    {
      m_nTruthJet = 0;
      for (JetMap::Iter iter = jetsMC->begin(); iter != jetsMC->end(); ++iter)
	{
	  Jet* truthjet = iter->second;

	  bool eta_cut = (truthjet->get_eta() >= m_etaRange.first) and (truthjet->get_eta() <= m_etaRange.second);
	  bool pt_cut = (truthjet->get_pt() >= m_ptRange.first) and (truthjet->get_pt() <= m_ptRange.second);
	  if ((not eta_cut) or (not pt_cut)) continue;
	  m_truthID.push_back(truthjet->get_id());
	  m_truthNComponent.push_back(truthjet->size_comp());
	  m_truthEta.push_back(truthjet->get_eta());
	  m_truthPhi.push_back(truthjet->get_phi());
	  m_truthE.push_back(truthjet->get_e());
	  m_truthPt.push_back(truthjet->get_pt());
	  m_nTruthJet++;
	}
    }

  //get seed jets
  if(m_doSeeds)
    {
      for (JetMap::Iter iter = seedjetsraw->begin(); iter != seedjetsraw->end(); ++iter)
	{
	  Jet* jet = iter->second;
	  int passesCut = jet->get_property(Jet::PROPERTY::prop_SeedItr);
	  m_eta_rawseed.push_back(jet->get_eta());
	  m_phi_rawseed.push_back(jet->get_phi());
	  m_e_rawseed.push_back(jet->get_e());
	  m_pt_rawseed.push_back(jet->get_pt());
	  m_rawseed_cut.push_back(passesCut);
	}

      for (JetMap::Iter iter = seedjetssub->begin(); iter != seedjetssub->end(); ++iter)
	{
	  Jet* jet = iter->second;
	  int passesCut = jet->get_property(Jet::PROPERTY::prop_SeedItr);
	  m_eta_subseed.push_back(jet->get_eta());
	  m_phi_subseed.push_back(jet->get_phi());
	  m_e_subseed.push_back(jet->get_e());
	  m_pt_subseed.push_back(jet->get_pt());
	  m_subseed_cut.push_back(passesCut);
	}
    }
  
  //fill the tree
  m_T->Fill();

  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int JetValidation::ResetEvent(PHCompositeNode *topNode)
{
  //std::cout << "JetValidation::ResetEvent(PHCompositeNode *topNode) Resetting internal structures, prepare for next event" << std::endl;
  m_id.clear();
  m_nComponent.clear();
  m_eta.clear();
  m_phi.clear();
  m_e.clear();
  m_pt.clear();
  m_unsub_pt.clear();
  m_sub_et.clear();

  m_truthID.clear();
  m_truthNComponent.clear();
  m_truthEta.clear();
  m_truthPhi.clear();
  m_truthE.clear();
  m_truthPt.clear();
  m_truthdR.clear();

  m_eta_subseed.clear();
  m_phi_subseed.clear();
  m_e_subseed.clear();
  m_pt_subseed.clear();
  m_subseed_cut.clear();

  m_eta_rawseed.clear();
  m_phi_rawseed.clear();
  m_e_rawseed.clear();
  m_pt_rawseed.clear();
  m_rawseed_cut.clear();
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int JetValidation::EndRun(const int runnumber)
{
  std::cout << "JetValidation::EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int JetValidation::End(PHCompositeNode *topNode)
{
  std::cout << "JetValidation::End - Output to " << m_outputFileName << std::endl;
  PHTFileServer::get().cd(m_outputFileName);

  m_T->Write();
  std::cout << "JetValidation::End(PHCompositeNode *topNode) This is the End..." << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int JetValidation::Reset(PHCompositeNode *topNode)
{
 std::cout << "JetValidation::Reset(PHCompositeNode *topNode) being Reset" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void JetValidation::Print(const std::string &what) const
{
  std::cout << "JetValidation::Print(const std::string &what) const Printing info for " << what << std::endl;
}


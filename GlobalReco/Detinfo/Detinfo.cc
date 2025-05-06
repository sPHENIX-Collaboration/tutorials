#include "Detinfo.h"

#include <calobase/TowerInfoDefs.h>
#include <epd/EPDDefs.h>
#include <epd/EpdGeom.h>

#include <cdbobjects/CDBTTree.h> // for CDBTTree
#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/PHTFileServer.h>

#include <phool/getClass.h>
#include <phool/PHCompositeNode.h>

#include <calobase/TowerInfoContainer.h>
#include <calobase/TowerInfo.h>

#include <ffarawobjects/Gl1Packet.h>
#include <phool/getClass.h>

#include <globalvertex/GlobalVertex.h>
#include <globalvertex/GlobalVertexMap.h>
#include <globalvertex/MbdVertex.h>
#include <globalvertex/MbdVertexMapv1.h>

#include <calotrigger/MinimumBiasInfo.h>
#include <centrality/CentralityInfo.h>

#include <eventplaneinfo/Eventplaneinfo.h>
#include <eventplaneinfo/Eventplaneinfov1.h>
#include <eventplaneinfo/EventplaneinfoMap.h>

#include <mbd/MbdGeom.h>
#include <mbd/MbdOut.h>
#include <mbd/MbdPmtContainer.h>
#include <mbd/MbdPmtHit.h>

#include <zdcinfo/ZdcReco.h>
#include <zdcinfo/Zdcinfo.h>

#include <boost/format.hpp>


#include <TFile.h>
#include <TH3F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TProfile.h>
#include <TVector3.h>
#include <TH1.h>

#include <cassert>
#include <fstream>
#include <TGraph.h>
#include <vector>


#include <boost/format.hpp>
#include <boost/math/special_functions/sign.hpp>



//____________________________________________________________________________..
Detinfo::Detinfo(const std::string &name):
 SubsysReco(name)
{
  _event = 0;
  _outfile_name = "qa.root";

}

//____________________________________________________________________________..
Detinfo::~Detinfo()
{
  
}

//____________________________________________________________________________..
int Detinfo::Init(PHCompositeNode *topNode)
{
   
    std::cout << PHWHERE << " Opening file " << _outfile_name << std::endl;

    PHTFileServer::get().open(_outfile_name, "RECREATE");
    PHTFileServer::get().cd(_outfile_name);
   
    _event_tree = new TTree("event", "fwd => event info");
    _event_tree->Branch("event", &_event, "_event/I");
    _event_tree->Branch("zvertex", &thisvertex, "thisvertex/F");
    _event_tree->Branch("cent", &centrality_mbd_, "centrality_mbd_/F");
    _event_tree->Branch("psi2north", &psi2_north, "psi2_north/F");
    _event_tree->Branch("psi2south", &psi2_south, "psi2_south/F");
    _event_tree->Branch("mbdqsouth", &mbd_q_south, "mbd_q_south/F");
    _event_tree->Branch("mbdqnorth", &mbd_q_north, "mbd_q_north/F");
    _event_tree->Branch("sepdqsouth", &sepd_q_south, "sepd_q_south/F");
    _event_tree->Branch("sepdqnorth", &sepd_q_north, "sepd_q_north/F");
    _event_tree->Branch("zdcEsouth", &zdc_e_south, "zdc_e_south/F");
    _event_tree->Branch("zdcEnorth", &zdc_e_north, "zdc_e_north/F");
    _event_tree->Branch("is_min_bias", &is_min_bias);
    _event_tree->Branch("tile_e", &_t);
    _event_tree->Branch("tile_time", &_tt);
    _event_tree->Branch("pmt_e", &_p);
    _event_tree->Branch("pmt_time", &_pt);
    _event_tree->Branch("zdc_e", &_z);
    _event_tree->Branch("zdc_time", &_zt);
    h_triggerVec = new TH1D("h_triggerVec", "", 65, -0.5, 64.5);

    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int Detinfo::InitRun(PHCompositeNode *topNode)
{
  //get sEPD tower mapping from CBD
  if (!m_overrideSEPDMapName) {
    m_sEPDMapName = "SEPD_CHANNELMAP";
  }
  if (!m_overrideSEPDFieldName) {
    m_sEPDfieldname = "epd_channel_map";
  }
  std::string calibdir = CDBInterface::instance()->getUrl(m_sEPDMapName);
  if (!calibdir.empty()) {
        cdbttree = new CDBTTree(calibdir);
 } else
 {
    std::cout << "Detinfo::::InitRun No SEPD mapping file for domain "<< m_sEPDMapName << " found" << std::endl;
    exit(1);
 }
   
 v.clear();
 for (int i = 0; i < 768; i++)
 {
    int towerindex = cdbttree->GetIntValue(i, m_sEPDfieldname);
    if (towerindex == 999) //these are empty tiles
    {
        continue;
    }
    key = TowerInfoDefs::encode_epd(towerindex);
    v.push_back(key);
 }
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int Detinfo::process_event(PHCompositeNode *topNode)
{

    _event++;
    
    _z.clear();
    _p.clear();
    _t.clear();
    _zt.clear();
    _pt.clear();
    _tt.clear();
    
    mbd_q_south = 0.;
    mbd_q_north = 0.;
    
    zdc_e_south = 0.;
    zdc_e_north = 0.;
    
    sepd_q_south = 0.;
    sepd_q_north = 0.;
    
    //get event plane map
    EventplaneinfoMap *epmap = findNode::getClass<EventplaneinfoMap>(topNode, "EventplaneinfoMap");
    if (!epmap)
    {
        std::cout << PHWHERE << "::ERROR - cannot find EventplaneinfoMap" << std::endl;
        exit(-1);
    }

    if (epmap->empty())
    {
        return Fun4AllReturnCodes::ABORTEVENT;
    }
    
    auto _EPDS = epmap->get(EventplaneinfoMap::sEPDS);
    auto _EPDN = epmap->get(EventplaneinfoMap::sEPDN);
    
    //example psi_2 access
    std::pair<double, double> epdsouthQ2;
    std::pair<double, double> epdnorthQ2;
    epdsouthQ2 = _EPDS->get_qvector(2);
    epdnorthQ2 = _EPDN->get_qvector(2);
    
    Qx_south = epdsouthQ2.first;
    Qy_south = epdsouthQ2.second;
    Qx_north = epdnorthQ2.first;
    Qy_north = epdnorthQ2.second;
    
    Eventplaneinfo *epinfo = new Eventplaneinfov1();
    psi2_south = epinfo->GetPsi(Qx_south, Qy_south, 2);
    psi2_north = epinfo->GetPsi(Qx_north, Qy_north, 2);

    CentralityInfo* m_CentInfo = nullptr;
    MinimumBiasInfo* _minimumbiasinfo = nullptr;
    m_CentInfo = findNode::getClass<CentralityInfo>(topNode, "CentralityInfo");
    if (!m_CentInfo)
    {
        std::cout << PHWHERE << "Error, can't find CentralityInfov1. No centrality info is filled" << std::endl;
         exit(1);
    }
    
    if (m_CentInfo) centrality_mbd_ = m_CentInfo->get_centrality_bin(CentralityInfo::PROP::mbd_NS);

    _minimumbiasinfo = findNode::getClass<MinimumBiasInfo>(topNode, "MinimumBiasInfo");
    if (!_minimumbiasinfo)
    {
        std::cout << "Error, can't find MinimumBiasInfo. No minimum bias info is filled" << std::endl;
        exit(1);
    }
    
    is_min_bias = false;
    is_min_bias = (_minimumbiasinfo) ? _minimumbiasinfo->isAuAuMinimumBias() : false;

  
    MbdVertexMap *mbdmap =
          findNode::getClass<MbdVertexMap>(topNode, "MbdVertexMap");
      MbdVertex *bvertex = nullptr;
      if (mbdmap)
      {
        for (MbdVertexMap::ConstIter mbditer = mbdmap->begin();
             mbditer != mbdmap->end(); ++mbditer)
        {
          bvertex = mbditer->second;
        }
        if (bvertex)
        {
            thisvertex = bvertex->get_z();
        }
      }
      
    //--------------------------- trigger and GL1-------------------------------//
    Gl1Packet *gl1PacketInfo =
        findNode::getClass<Gl1Packet>(topNode, "GL1Packet");
    if (!gl1PacketInfo) {
      std::cout << PHWHERE << "Detinfo::process_event: GL1Packet node is missing"
                << std::endl;
    }

    uint64_t triggervec = 0;
    if (gl1PacketInfo) {
      triggervec = gl1PacketInfo->getScaledVector();
      for (int i = 0; i < 64; i++) {
        bool trig_decision = ((triggervec & 0x1U) == 0x1U);

        if (trig_decision) {
          h_triggerVec->Fill(i);
        }
        triggervec = (triggervec >> 1U) & 0xffffffffU;
      }
      triggervec = gl1PacketInfo->getScaledVector();
    }
    
 
    TowerInfoContainer* towerinfosZDC;
    towerinfosZDC = findNode::getClass<TowerInfoContainer>(topNode, "TOWERINFO_CALIB_ZDC");
    if(!towerinfosZDC)
    {
        std::cout << PHWHERE << ":: No TOWERINFO_CALIB_ZDC!" << std::endl; exit(1);
    }
   
    MbdPmtHit *m_mbd_hit{nullptr};
    MbdPmtContainer *mbdpmts = findNode::getClass<MbdPmtContainer>(topNode, "MbdPmtContainer");
     if (!mbdpmts)
     {
         std::cout << PHWHERE << "::ERROR - cannot find MbdPmtContainer" << std::endl;
         exit(1);
     }

    TowerInfoContainer* towerinfosEPD = findNode::getClass<TowerInfoContainer>(topNode, "TOWERINFO_CALIB_SEPD");
    if(!towerinfosEPD)
    {
        std::cout << PHWHERE << ":: No TOWERINFO_CALIB_SEPD!" << std::endl; exit(1);
    }
    
    Zdcinfo *zdcinfo = findNode::getClass<Zdcinfo>(topNode, "Zdcinfo");
    if(!zdcinfo)
    {
        std::cout << PHWHERE << ":: No Zdcinfo!" << std::endl; exit(1);
    }
    
    zdc_e_south = zdcinfo->get_zdc_energy(0);
    zdc_e_north = zdcinfo->get_zdc_energy(1);
    
    MbdGeom *mbdgeom = findNode::getClass<MbdGeom>(topNode, "MbdGeom");
    if (!mbdgeom)
    {
       std::cout << PHWHERE << "::ERROR - cannot find MbdGeom" << std::endl;
        exit(-1);
    }
    

    if ((triggervec >> 0xAU) & 0x1U)
    {
        //mbd       
        for (int i = 0; i < 128; i++)
        {
            m_mbd_hit = mbdpmts->get_pmt(i);
            _p.push_back(m_mbd_hit->get_q());
            _pt.push_back(m_mbd_hit->get_time());
            
            //mbd charge sums
            int arm = mbdgeom->get_arm(i);
            if (arm == 0)
            {
                mbd_q_south += m_mbd_hit->get_q();
            }
            else if (arm == 1)
            {
                mbd_q_north += m_mbd_hit->get_q();
            }
        }
        
        //sepd
        int nchannels_epd = towerinfosEPD->size();
        
        float epd_e = 0.; float epd_t = 0.;
        
        for (int channel = 0; channel < nchannels_epd;channel++)
        {
            epd_e = towerinfosEPD->get_tower_at_channel(channel)->get_energy();
            epd_t = towerinfosEPD->get_tower_at_channel(channel)->get_time_float();
            _t.push_back(epd_e);
            _tt.push_back(epd_t);
            
            //sepd charge sums
            int arm = TowerInfoDefs::get_epd_arm(v[channel]);
            if (arm == 0)
            {
                sepd_q_south += epd_e;
            }
            else if (arm == 1)
            {
                sepd_q_north += epd_e;
            }
        }
        
	  //zdc
        int nchannels_zdc = towerinfosZDC->size();
        float zdc_t = 0.;
        float zdc_e = 0.;
        
        for (int channel = 0; channel < nchannels_zdc;channel++)
        {
            zdc_t = towerinfosZDC->get_tower_at_channel(channel)->get_time_float();
            zdc_e = towerinfosZDC->get_tower_at_channel(channel)->get_energy();
            _z.push_back(zdc_e);
            _zt.push_back(zdc_t);
            
        }
        _event_tree->Fill();
        
    }//trigger
    

    std::cout<<_event<<std::endl;

    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int Detinfo::ResetEvent(PHCompositeNode *topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int Detinfo::EndRun(const int runnumber)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int Detinfo::End(PHCompositeNode *topNode)
{
    PHTFileServer::get().cd(_outfile_name);
    PHTFileServer::get().write(_outfile_name);

    return Fun4AllReturnCodes::EVENT_OK;

}

//____________________________________________________________________________..
int Detinfo::Reset(PHCompositeNode *topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

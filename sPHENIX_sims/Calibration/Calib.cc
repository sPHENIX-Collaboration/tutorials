#include "Calib.h"
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/PHTFileServer.h>
#include <fun4all/Fun4AllServer.h>

#include <g4main/PHG4HitContainer.h>
#include <g4main/PHG4Hit.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4VtxPoint.h>
#include <g4main/PHG4Particle.h>

#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>
#include <phool/PHNode.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHObject.h>
#include <phool/getClass.h>
#include <phool/phool.h>


#include <calobase/RawTowerDefs.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/RawTower.h>
#include <calobase/RawTowerGeomContainer.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawClusterContainer.h>
#include <calobase/RawCluster.h>
#include <calobase/RawTowerv1.h>

#include <phgeom/PHGeomUtility.h>
#include <TTree.h>
#include <TH2D.h>
#include <TProfile2D.h>
#include <TVector3.h>
#include <TRandom3.h>
#include <TMath.h>

#include <fstream>



using namespace std;

//----------------------------------------------------------------------------//
//-- Constructor:
//--  simple initialization
//----------------------------------------------------------------------------//

Calib::Calib(const string &name) :
  SubsysReco(name), _event_tree(NULL)
{
    //initialize
    _event = 0;
    _outfile_name = "Tower_test.root";

}

//----------------------------------------------------------------------------//
//-- Init():
//--   Intialize all histograms, trees, and ntuples
//----------------------------------------------------------------------------//
int Calib::Init(PHCompositeNode *topNode) {

    cout << PHWHERE << " Opening file " << _outfile_name << endl;

    PHTFileServer::get().open(_outfile_name, "RECREATE");
    PHTFileServer::get().cd(_outfile_name);
    
    Double_t xbinlimits[25] =
     {
         -1.10001,-1.00833,-0.916665,-0.824998,-0.733332,-0.641665,-0.549998,-0.458332,-0.366665,-0.274998,-0.183332,-0.091665,0.,0.0916683,0.183335,0.275002,0.366668,0.458335,0.550002,0.641668,0.733335,0.825002,0.916668,1.00833,1.10001
   };
    
    Double_t ybinlimits[64] =
     {
         0.122236,0.220412,0.318586,0.416761,0.514936,0.613111,0.711286,0.80946,0.907635,1.00581,1.10399,1.20216,1.30034,1.39851,1.49669,1.59486,1.69304,1.79121,1.88938,1.98756,2.08573,2.18391,2.28208,2.38026,2.47843,2.57661,2.67478,2.77296,2.87113,2.96931,3.06748,3.16566,3.26383,3.36201,3.46018,3.55836,3.65653,3.75471,3.85288,3.95105,4.04923,4.1474,4.24558,4.34375,4.44193,4.5401,4.63828,4.73645,4.83463,4.9328,5.03098,5.12915,5.22733,5.3255,5.42368,5.52185,5.62003,5.7182,5.81638,5.91455,6.01273,6.1109,6.20907,6.30725
};
    
    Long_t xbinnumb = sizeof(xbinlimits)/sizeof(Double_t) - 1;
    Long_t ybinnumb = sizeof(ybinlimits)/sizeof(Double_t) - 1;
    

    hprof2d  = new TProfile2D("hprof2d","Profile of tower energy versus eta and phi", xbinnumb,xbinlimits, ybinnumb,ybinlimits);
    
    _event_tree = new TTree("event", "Calib => event info");
    
    _event_tree->Branch("event", &_event, "_event/I");
    _event_tree->Branch("oCalib_ohcal", &_oCalib_ohcal, "_oCalib_ohcal/F");
    
    
    return Fun4AllReturnCodes::EVENT_OK;
}

int Calib::InitRun(PHCompositeNode *topNode)
{
    return Fun4AllReturnCodes::EVENT_OK;
}

//----------------------------------------------------------------------------//
//-- process_event():
//--   Call user instructions for every event.
//--   This function contains the analysis structure.
//----------------------------------------------------------------------------//

int Calib::process_event(PHCompositeNode *topNode)
{
    _event++;

    GetNodes(topNode);

    fill_tree(topNode);

    return Fun4AllReturnCodes::EVENT_OK;
}

//----------------------------------------------------------------------------//
//-- End():
//--   End method, wrap everything up
//----------------------------------------------------------------------------//

int Calib::EndRun(PHCompositeNode *topNode)
{

    return Fun4AllReturnCodes::EVENT_OK;

}

int Calib::End(PHCompositeNode *topNode)
{

    PHTFileServer::get().cd(_outfile_name);
    PHTFileServer::get().write(_outfile_name);

    return Fun4AllReturnCodes::EVENT_OK;
}

//----------------------------------------------------------------------------//
//-- fill_tree():
//--   Fill the various trees...
//----------------------------------------------------------------------------//

void Calib::fill_tree(PHCompositeNode *topNode)
{

  cout << _event << endl;

    
   
    //ohcal  towers
       int ietabin, iphibin = -1;
      float oh_ocal = 0.0;
      float OHCAL_o[24][64] = {{0.0}};
      float OHCAL_tower_eta[24] = {0.0};
      float OHCAL_tower_phi[64] = {0.0};

    //loop over ohcal towers
    RawTowerContainer::ConstRange begin_end_oo = _ohcal_towers_o->getTowers();
      RawTowerContainer::ConstIterator oh_o = begin_end_oo.first;
      for (; oh_o != begin_end_oo.second; ++oh_o) {
        RawTowerDefs::keytype towerid_o_o = oh_o->first;
        RawTower *rawtower_o_o = _ohcal_towers_o->getTower(towerid_o_o);
        if(rawtower_o_o) {
        RawTowerGeom *tgeo_o_o = _ohcal_towergeom->get_tower_geometry(towerid_o_o);
        // get towers energy
        oh_ocal  = rawtower_o_o->get_energy();
            
        // binning in eta and phi
        ietabin = tgeo_o_o->get_column();
        iphibin = tgeo_o_o->get_row();
        if((iphibin >= 0)&&(ietabin >= 0))
        {
            OHCAL_o[ietabin][iphibin] += oh_ocal;
            OHCAL_tower_eta[ietabin] = tgeo_o_o->get_eta();
            OHCAL_tower_phi[iphibin] = tgeo_o_o->get_phi();
        }
            
       }
      }
    
  

    //---- Store tower energies ------//
    _oCalib_ohcal = 0.0;
  
    for(int i=0; i<24; i++){
          for(int j=0; j<64; j++){
              _oCalib_ohcal += OHCAL_o[i][j];
              hprof2d->Fill(OHCAL_tower_eta[i], OHCAL_tower_phi[j],OHCAL_o[i][j]);
          }
    }
   
  
_event_tree->Fill();


return;

}

//----------------------------------------------------------------------------//
//-- GetNodes():
//--   Get all the all the required nodes off the node tree
//----------------------------------------------------------------------------//
int Calib::GetNodes(PHCompositeNode * topNode) {
    
    //oHCAL
 
    _ohcal_towers_o = findNode::getClass<RawTowerContainer>(topNode, "TOWER_CALIB_HCALOUT");
    if (!_ohcal_towers_o)
      {
    std::cout << PHWHERE << ": Could not find node TOWER_CALIB_HCALOUT" << std::endl;
      return Fun4AllReturnCodes::ABORTEVENT;
      }
    
    
      //towergeom
    _ohcal_towergeom = findNode::getClass<RawTowerGeomContainer>(topNode, "TOWERGEOM_HCALOUT");
    if (! _ohcal_towergeom)
      {
    cout << PHWHERE << ": Could not find node TOWERGEOM_HCALOUT" << endl;
      return Fun4AllReturnCodes::ABORTEVENT;
      }
   
     return Fun4AllReturnCodes::EVENT_OK;
}


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
    
   Double_t ybinlimits[65] =
    {
        -3.16565,-3.01936,-2.92119,-2.82301,-2.72484,-2.62666,-2.52849,-2.43032,-2.33214,-2.23397,-2.13579,-2.03762,-1.93944,-1.84127,-1.74309,-1.64492,-1.54674,-1.44857,-1.35039,-1.25222,-1.15404,-1.05587,-0.95769,-0.859519,-0.761344,-0.663169,-0.564994,-0.46682,-0.368645,-0.27047,-0.172295,-0.074121,0.024068900,0.122229,0.220404,0.318578,0.416753,0.514928,0.613103,0.711278,0.809452,0.907627,1.0058,1.10398,1.20215,1.30033,1.3985,1.49668,1.59485,1.69303,1.7912,1.88937,1.98755,2.08572,2.1839,2.28207,2.38025,2.47842,2.5766,2.67477,2.77295,2.87112,2.9693,3.06747,3.16565
    };
    
    Long_t xbinnumb = sizeof(xbinlimits)/sizeof(Double_t) - 1;
    Long_t ybinnumb = sizeof(ybinlimits)/sizeof(Double_t) - 1;
    

    hprof2d  = new TProfile2D("hprof2d","Profile of tower energy versus eta and phi", xbinnumb,xbinlimits, ybinnumb,ybinlimits);
    hprof2d->SetXTitle("#eta");
    hprof2d->SetYTitle("#phi");
  
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
    
  

    //---- Store energy ------//
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


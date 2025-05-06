#include <GlobalVariables.C>
#include <Detinfo.h>

#include <ffamodules/CDBInterface.h>

#include <phool/recoConsts.h>
#include <phool/PHRandomSeed.h>

#include <phparameter/PHParameterUtils.h>

#include <fun4all/InputFileHandler.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllNoSyncDstInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/Fun4AllSyncManager.h>

#include <caloreco/CaloTowerBuilder.h>
#include <caloreco/CaloTowerCalib.h>

#include <epd/EpdReco.h>
#include <mbd/MbdReco.h>

#include <centrality/CentralityReco.h>
#include <g4centrality/PHG4CentralityReco.h>

#include <calotrigger/MinimumBiasClassifier.h>

#include <globalvertex/GlobalVertexReco.h>

#include <eventplaneinfo/Eventplaneinfo.h>
#include <eventplaneinfo/EventPlaneReco.h>

#include <zdcinfo/ZdcReco.h>


R__LOAD_LIBRARY( libfun4all.so )
R__LOAD_LIBRARY( libffamodules.so )
R__LOAD_LIBRARY( libmbd.so )
R__LOAD_LIBRARY( libg4mbd.so )
R__LOAD_LIBRARY(libepd.so)
R__LOAD_LIBRARY( libcalotrigger.so )
R__LOAD_LIBRARY( libg4centrality.so )
R__LOAD_LIBRARY( libcentrality.so )
R__LOAD_LIBRARY( libglobalvertex.so )
R__LOAD_LIBRARY( libg4vertex.so )
R__LOAD_LIBRARY( libzdcinfo.so )
R__LOAD_LIBRARY(libeventplaneinfo.so)
R__LOAD_LIBRARY(libDetinfo.so)


void Fun4All_Global_2024(
    const std::string & prodTag = "2024p009",
    const int timeStamp = 54912,
    const int nEvents = 1000
   )
{

    Enable::VERBOSITY = 0;
  
    CDB::global_tag = prodTag;
    CDB::timestamp = static_cast<uint64_t>( timeStamp );

    auto se = Fun4AllServer::instance();
    se -> Verbosity( Enable::VERBOSITY );

    auto rc = recoConsts::instance();
    CDBInterface::instance( ) -> Verbosity( 0 );
    rc -> set_StringFlag( "CDB_GLOBALTAG", CDB::global_tag );
    rc -> set_uint64Flag( "TIMESTAMP", CDB::timestamp );

    Fun4AllInputManager *in = new Fun4AllDstInputManager("in1");
    std::string filein = "/sphenix/lustre01/sphnxpro/physics/run2auau/caloy2fitting/new_2024p007/run_00054900_00055000/DST_CALOFITTING_run2auau_new_2024p007-00054912-";
        
        for (int i = 0; i < 490; i++)
        {
            std::string seg = "0000";
            if(i >= 10 && i<100) seg = "000";
            if(i >= 100 && i<1000) seg = "00";
            filein += seg;
            std::string str2 = std::to_string(i);
            filein += str2;
            filein += ".root";
            in->AddFile(filein);
            filein = "/sphenix/lustre01/sphnxpro/physics/run2auau/caloy2fitting/new_2024p007/run_00054900_00055000/DST_CALOFITTING_run2auau_new_2024p007-00054912-";

        }
     
    se->registerInputManager(in);

      
    EpdReco *epdreco = new EpdReco();
    se->registerSubsystem(epdreco);
    
    auto mbdreco = new MbdReco();
    se->registerSubsystem( mbdreco );
    
    auto gvertex = new GlobalVertexReco();
    se->registerSubsystem( gvertex );
    
    CaloTowerCalib *calibZDC = new CaloTowerCalib("ZDC");
    calibZDC->set_detector_type(CaloTowerDefs::ZDC);
    se->registerSubsystem(calibZDC); 

    auto zdcreco = new ZdcReco();
    zdcreco->set_zdc1_cut(0.0);
    zdcreco->set_zdc2_cut(0.0);
    se->registerSubsystem( zdcreco );

    auto mb = new MinimumBiasClassifier();
    mb->Verbosity( Enable::VERBOSITY );
    mb->setOverwriteScale("/sphenix/user/dlis/Projects/centrality/cdb/calibrations/scales/cdb_centrality_scale_54912.root"); // will change run by run
    mb->setOverwriteVtx("/sphenix/user/dlis/Projects/centrality/cdb/calibrations/vertexscales/cdb_centrality_vertex_scale_54912.root"); // will change run by run
    se->registerSubsystem( mb );

    auto cent = new CentralityReco();
    cent->setOverwriteScale("/sphenix/user/dlis/Projects/centrality/cdb/calibrations/scales/cdb_centrality_scale_54912.root"); // will change run by run
    cent->setOverwriteVtx("/sphenix/user/dlis/Projects/centrality/cdb/calibrations/vertexscales/cdb_centrality_vertex_scale_54912.root"); // will change run by run
    cent->setOverwriteDivs("/sphenix/user/dlis/Projects/centrality/cdb/calibrations/divs/cdb_centrality_54912.root");
    se->registerSubsystem( cent );
    
    EventPlaneReco *epreco = new EventPlaneReco();
    epreco->set_sepd_epreco(true);
    se->registerSubsystem(epreco);
   

    Detinfo *detana = new Detinfo();
    detana->set_filename("Global_reco.root");
    se->registerSubsystem(detana);
  
    se -> run( nEvents );
    se -> End();   

    std::cout << "Done!" << std::endl;
    gSystem -> Exit( 0 );
}

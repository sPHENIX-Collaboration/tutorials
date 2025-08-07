#ifndef MACRO_FUN4ALL_CALOTREEGEN_C
#define MACRO_FUN4ALL_CALOTREEGEN_C

#include <ffamodules/CDBInterface.h>
#include <fun4all/Fun4AllUtils.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/PHRandomSeed.h>
#include <phool/recoConsts.h>

#include <calohistgen/caloHistGen.h>

#include <Calo_Calib.C>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffarawobjects.so)
R__LOAD_LIBRARY(libcaloHistGen.so)


void Fun4All_CaloHistGen(const int nEvents = 10000, const std::string &fnameCalib = "/direct/sphenix+lustre01/sphnxpro/production/run3auau/physics/caloy2calib/new_newcdbtag_v006/run_00068100_00068200/DST_CALO_run3auau_new_newcdbtag_v006-00068152-00000.root", const std::string &fnameRaw = "/direct/sphenix+lustre01/sphnxpro/production/run3auau/physics/caloy2fitting/new_newcdbtag_v006/run_00068100_00068200/DST_CALOFITTING_run3auau_new_newcdbtag_v006-00068152-00000.root",const std::string &outName = "commissioning.root", const std::string &dbtag = "ProdA_2024")//Constructor for Run 25 Au+Au
//void Fun4All_CaloHistGen(const int nEvents = 0, const std::string &fnameCalib = "/direct/sphenix+lustre01/sphnxpro/production/run2pp/physics/ana468_2024p012_v001/DST_CALO/run_00052000_00052100/dst/DST_CALO_run2pp_ana468_2024p012_v001-00052032-00000.root", const std::string &fnameRaw = "/direct/sphenix+lustre01/sphnxpro/production/run2pp/physics/ana468_2024p012_v001/DST_CALOFITTING/run_00052000_00052100/dst/DST_CALOFITTING_run2pp_ana468_2024p012_v001-00052032-00000.root",const std::string &outName = "commissioning.root", const std::string &dbtag = "ProdA_2024")//Constructor for Run 24 p+p
{
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  recoConsts *rc = recoConsts::instance();
  
  pair<int, int> runseg = Fun4AllUtils::GetRunSegment(fnameCalib);
  int runnumber = runseg.first;

  bool isAuAu = false;
  if(fnameCalib.find("auau") != std::string::npos)
    {
      std::cout << "This is Au+Au, setting flags and cuts" << std::endl;
      isAuAu = true;
    }
  else
    {
      std::cout << "This is p+p, Au+Au flags will not be set" << std::endl;
    }

  
  rc -> set_StringFlag("CDB_GLOBALTAG",dbtag);
  rc -> set_uint64Flag("TIMESTAMP",runnumber);
  CDBInterface::instance() -> Verbosity(1);

  gSystem->Load("libg4dst");
  
  Fun4AllInputManager *inRaw = new Fun4AllDstInputManager("DSTRaw");
  inRaw->AddFile(fnameRaw);
  se->registerInputManager(inRaw);

  Fun4AllInputManager *inCalib = new Fun4AllDstInputManager("DSTCalib");
  inCalib->AddFile(fnameCalib);
  se->registerInputManager(inCalib);

  //you need this call to calibrate the data!
  Process_Calo_Calib();

  
  caloHistGen *calo = new caloHistGen(outName);

  // Let the subsys reco know what we're looking at
  calo->setIsAuAu(isAuAu);

  // What subsystems do you want?
  calo->doEMCal(1, "TOWERINFO_CALIB_CEMC");
  // Store EMCal clusters?
  calo->doClusters(1, "CLUSTERINFO_CEMC");

  // Store HCal information?
  calo->doHCals(1, "TOWERINFO_CALIB_HCALOUT", "TOWERINFO_CALIB_HCALIN");

  // Store ZDC information?
  calo->doZDC(1, "TOWERS_ZDC");

  // Store GL1 Information?
  //calo->doTrig(0, "GL1Packet");//Uncomment for p+p 2024
  calo->doTrig(0, "14001");//Temporary for Au+Au 2025

  //reconstruct diphoton pairs?
  calo->setPi0Reco(1);
  calo->setMaxAlpha(0.6);
  if(isAuAu)
    {
      calo->setCluster1EMin(1.3);
      calo->setCluster2EMin(0.7);
      calo->peripheralOnlyPi0(true);//only do reco in peripheral bins where S/B ratio is better.
      calo->setCaloFrac(0.2);
    }
  else
    {
      calo->setCluster1EMin(0.5);
      calo->setCluster2EMin(0.5);
    }

  if(!isAuAu)//as of 06/24/25 we're just booting up the photon triggers and need the right trigger map, leaving out for now.
    {
      calo->setTrig("jet");
      calo->setTrig("photon");
    }
  
  se->registerSubsystem(calo);

  se->run(nEvents);
  se->End();
  se->PrintTimer();
  std::cout << "All done!" << std::endl;

  gSystem->Exit(0);
}

#endif

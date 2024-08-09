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

#include <calotreegen/caloTreeGen.h>

#include <Calo_Calib.C>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffarawobjects.so)
R__LOAD_LIBRARY(libcaloTreeGen.so)

  void Fun4All_CaloTreeGen(const int nEvents = 0, const std::string &fname = "DST_CALO_run2pp_new_2024p004-00048089-00018.root", const std::string &inName = "commissioning.root", const std::string &dbtag = "ProdA_2024")
{
  Fun4AllServer *se = Fun4AllServer::instance();
  recoConsts *rc = recoConsts::instance();
  
  pair<int, int> runseg = Fun4AllUtils::GetRunSegment(fname);
  int runnumber = runseg.first;

  rc -> set_StringFlag("CDB_GLOBALTAG",dbtag);
  rc -> set_uint64Flag("TIMESTAMP",runnumber);
  CDBInterface::instance() -> Verbosity(1);

  gSystem->Load("libg4dst");
  
  Process_Calo_Calib();

  caloTreeGen *calo = new caloTreeGen(inName);
  // What subsystems do you want?
  calo->doEMCal(1, "TOWERINFO_CALIB_CEMC");
  // Store EMCal clusters?
  calo->doClusters(1, "CLUSTERINFO_CEMC");

  // Store tower information for each EMCal cluster?
  calo->doClusterDetails(1);

  // Store HCal information?
  calo->doHCals(1, "TOWERINFO_CALIB_HCALOUT", "TOWERINFO_CALIB_HCALIN");

  // Store ZDC information?
  calo->doZDC(1, "TOWERINFO_CALIB_ZDC");

  // Store GL1 Information?
  calo->doTrig(1, "GL1Packet");

  se->registerSubsystem(calo);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTcalo");
  in->AddFile(fname);

  se->registerInputManager(in);

  se->run(nEvents);
  se->End();
  se->PrintTimer();
  std::cout << "All done!" << std::endl;

  gSystem->Exit(0);
}

#endif

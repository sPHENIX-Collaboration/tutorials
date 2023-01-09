#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)

#include <runtowerinfo/SaveTowerInfo.h>
#include <runtowerinfo/RunTowerInfo.h>

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libruntowerinfo.so)

#endif

void Fun4All_TowerBuilder(vector<string> myInputLists = {"productionFiles-PYTHIA8_PP_MB-dst_calo_cluster-00000.list", "productionFiles-PYTHIA8_PP_MB-dst_vertex-00000.list", "productionFiles-PYTHIA8_PP_MB-dst_tracks-00000.list"}, const int nEvents = 100)
{
  gSystem->Load("libg4dst");
  Fun4AllServer *se = Fun4AllServer::instance();


  for (unsigned int i = 0; i < myInputLists.size(); ++i)
  {
    Fun4AllInputManager *infile = new Fun4AllDstInputManager("DSTin_" + to_string(i));
    infile->AddListFile(myInputLists[i]);
    se->registerInputManager(infile);
  }

  SaveTowerInfo *sti = new SaveTowerInfo();
  se->registerSubsystem(sti);

  RunTowerInfo *rti = new RunTowerInfo();
  se->registerSubsystem(rti);

  se->run(nEvents);
  se->End();
  se->PrintTimer();
  gSystem->Exit(0);
}

#ifndef MACRO_FUN4ALLJETANA_C
#define MACRO_FUN4ALLJETANA_C

#include <G4_Jets.C>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>


// here you need your package name (set in configure.ac)
// this suppresses a cling warning when reading an eigen include
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-internal"
#include <myjetanalysis/MyJetAnalysis.h>
#pragma GCC diagnostic pop

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libmyjetanalysis.so)

void Fun4All_JetAna_MDC(const int nevnt = 0, const std::string &inlist1 = "dst_tracks.list",  const std::string &inlist2 = "dst_calo_cluster.list", const std::string &inlist3 = "dst_truth.list")
{
  gSystem->Load("libmyjetanalysis");
  gSystem->Load("libg4dst");

  Jet_Reco();

  Fun4AllServer *se = Fun4AllServer::instance();
  // uncomment if you want the event counter printed
  //se->Verbosity(1);
  MyJetAnalysis *myJetAnalysis = new MyJetAnalysis("AntiKt_Tower_r04", "AntiKt_Truth_r04", "myjetanalysis.root");
  //myJetAnalysis->Verbosity(10);
  // change lower pt and eta cut to make them visible
  myJetAnalysis->setPtRange(1, 100);
  myJetAnalysis->setEtaRange(-1.1, 1.1);
  myJetAnalysis->use_initial_vertex(true);
  se->registerSubsystem(myJetAnalysis);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin1");
  in->AddListFile(inlist1);
  se->registerInputManager(in);
  in = new Fun4AllDstInputManager("DSTin2");
  in->AddListFile(inlist2);
  se->registerInputManager(in);
  in = new Fun4AllDstInputManager("DSTin3");
  in->AddListFile(inlist3);
  se->registerInputManager(in);

  se->run(nevnt);
  se->End();
  delete se;
  gSystem->Exit(0);
}

#endif

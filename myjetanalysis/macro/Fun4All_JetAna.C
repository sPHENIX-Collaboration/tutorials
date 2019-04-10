#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>

// here you need your package name (set in configure.ac)
#include <myjetanalysis/MyJetAnalysis.h>
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libmyjetanalysis.so)
#endif

void Fun4All_JetAna(const int nevnt, const char *inputfile = "")
{
  gSystem->Load("libmyjetanalysis");

  Fun4AllServer *se = Fun4AllServer::instance();

MyJetAnalysis *myJetAnalysis = new MyJetAnalysis("AntiKt_Tower_r04","AntiKt_Truth_r04","myjetanalysis.root");
  se->registerSubsystem(myJetAnalysis);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin",inputfile);
  se->registerInputManager( in );

  se->run(nevnt);
  se->End();
  delete se;
  gSystem->Exit(0);
}

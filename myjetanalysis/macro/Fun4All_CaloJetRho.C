#ifndef MACRO_FUN4ALLJETANA_C
#define MACRO_FUN4ALLJETANA_C

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>

// here you need your package name (set in configure.ac)
#include <calojetrhoest/CaloJetRhoEst.h>
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libcalojetrhoest.so)

void Fun4All_CaloJetRho(const int nevnt = 0, const char *inputfile = "dst_tracks.list")
{
  gSystem->Load("libcalojetrhoest");
  gSystem->Load("libg4dst");

  Fun4AllServer *se = Fun4AllServer::instance();

  CaloJetRhoEst *myJetAnalysis = new CaloJetRhoEst("AntiKt_Tower_r04", "AntiKt_Truth_r04", "myjetanalysis.root");
  //  myJetAnalysis->Verbosity(0);
  // change lower pt and eta cut to make them visible using the example
  //  pythia8 file
  myJetAnalysis->setPtRange(1, 100);
  myJetAnalysis->setEtaRange(-1.1, 1.1);
  se->registerSubsystem(myJetAnalysis);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
  in->AddListFile(inputfile);
  se->registerInputManager(in);

  se->run(nevnt);
  se->End();
  delete se;
  gSystem->Exit(0);
}

#endif

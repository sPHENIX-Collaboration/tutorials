#ifndef MACRO_FUN4ALL_CALOANA_C
#define MACRO_FUN4ALL_CALOANA_C

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>

#include <caloana/CaloAna.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libcaloana.so)

void Fun4All_CaloAna(const char *fname = "/sphenix/sim/sim01/tutorials/CaloAna/G4sPHENIX_Pythia8.root")
{
  gSystem->Load("libg4dst");
  Fun4AllServer *se = Fun4AllServer::instance();
  CaloAna *ca = new CaloAna("CALOANA","out.root");
  // choose CEMC, HCALIN or HCALOUT or whatever you named your
  // calorimeter
  ca->Detector("CEMC");
  se->registerSubsystem(ca);
  Fun4AllInputManager *in = new Fun4AllDstInputManager("in");
  in->fileopen(fname);
  se->registerInputManager(in);
  se->run();
  se->End();
}

#endif

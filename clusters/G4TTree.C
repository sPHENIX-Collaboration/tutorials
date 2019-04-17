#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>

#include <g4histos/G4HitTTree.h>
#include <g4histos/G4RawTowerTTree.h>
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4histos.so)
#endif

void G4TTree(
  const char *fname = "/sphenix/sim/sim01/tutorials/clusters/G4sPHENIX_Pythia8.root",
  const int nevnt = 0,
  const char *outfile="towers.root")
{
  gSystem->Load("libg4histos");
  gSystem->Load("libg4dst.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  char detector[100];
  char outnode[100];
  sprintf(detector,"%s","CEMC"); 
 //  se->Verbosity(10);
  G4HitTTree *tt = new G4HitTTree();
  tt->Detector(detector);
  se->registerSubsystem(tt);

  G4RawTowerTTree *rt = new G4RawTowerTTree();
  rt->Detector(detector);
  se->registerSubsystem(rt);
  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
  in->fileopen(fname);
  se->registerInputManager(in);
  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("DSTout",outfile);
  sprintf(outnode,"G4RootHit_%s",detector);
  out->AddNode(outnode);
  sprintf(outnode,"G4RootRawTower_%s",detector);
  out->AddNode(outnode);
  se->registerOutputManager(out);

  se->run(nevnt);
  se->End();
  delete se;
}

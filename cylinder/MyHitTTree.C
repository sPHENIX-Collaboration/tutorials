#ifndef MACRO_MYHITTTREE_C
#define MACRO_MYHITTTREE_C

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <g4histos/G4HitTTree.h>

R__LOAD_LIBRARY(libg4histos.so)

void MyHitTTree(const char *fname, const int nevnt = 0, const char *outfile="hits.root")
{
  gSystem->Load("libg4dst.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  char detector[100];
  char outnode[100];
  sprintf(detector,"%s","SVTX");
  sprintf(outnode,"G4RootHit_%s",detector);
  //  se->Verbosity(10);
  G4HitTTree *tt = new G4HitTTree();
  tt->Detector(detector);
  se->registerSubsystem(tt);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
  in->fileopen(fname);
  se->registerInputManager(in);
  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("DSTout",outfile);
  out->AddNode(outnode);
  se->registerOutputManager(out);

  se->run(nevnt);
  se->End();
  delete se;
}

#endif

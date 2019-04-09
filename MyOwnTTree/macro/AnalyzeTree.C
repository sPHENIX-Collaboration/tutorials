#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <mytree/AnalyzeSimpleTree.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libmytree.so)
#endif

void  AnalyzeTree()
{
  gSystem->Load("libmytree.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  Fun4AllInputManager *in1 = new Fun4AllDstInputManager("DSTIN1");
  se->registerInputManager(in1);
  se->fileopen("DSTIN1","mytree.root");
  SubsysReco *mytree = new AnalyzeSimpleTree();
  se->registerSubsystem(mytree);
  se->run(100);
  se->End();
  se->dumpHistos();
  delete se;
}

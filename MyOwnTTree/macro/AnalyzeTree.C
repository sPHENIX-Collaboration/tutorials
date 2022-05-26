#ifndef MACRO_ANALYZETREE_C
#define MACRO_ANALYZETREE_C

#include <mytree/AnalyzeSimpleTree.h>

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libmytree.so)

void  AnalyzeTree()
{
  Fun4AllServer *se = Fun4AllServer::instance();
  Fun4AllInputManager *in1 = new Fun4AllDstInputManager("DSTIN1");
  se->registerInputManager(in1);
  se->fileopen("DSTIN1","mytree.root");
  SubsysReco *mytree = new AnalyzeSimpleTree();
  se->registerSubsystem(mytree);
  se->run(100);
  se->End();
//  se->dumpHistos("dummy.root");
  delete se;
}

#endif

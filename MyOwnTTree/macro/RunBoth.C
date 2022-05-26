#ifndef MACRO_RUNBOTH_C
#define MACRO_RUNBOTH_C

// here you need your package name (set in configure.ac)
#include <mytree/AnalyzeSimpleTree.h>
#include <mytree/MakeSimpleTree.h>

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>

#include <phool/recoConsts.h>


R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libmytree.so)

void  RunBoth()
{
  Fun4AllServer *se = Fun4AllServer::instance();
  // since it doesn't matter we use a dummy input manager which just
  // drives the event loop with a runnumber of 310000
  recoConsts *rc = recoConsts::instance();
    rc->set_IntFlag( "RUNNUMBER", 310000);
  Fun4AllInputManager *in = new Fun4AllDummyInputManager( "DSTin");
  se->registerInputManager( in );
  // make my ttree
  SubsysReco *mytree = new MakeSimpleTree("MYTREE");
  se->registerSubsystem(mytree);
  // and analyze it in the same process
  SubsysReco *myana = new AnalyzeSimpleTree();
  se->registerSubsystem(myana);

  se->run(100);
  se->End();
  se->dumpHistos();
  delete se;
}

#endif

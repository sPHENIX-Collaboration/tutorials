#ifndef MACRO_MAKETREE_C
#define MACRO_MAKETREE_C

// here you need your package name (set in configure.ac)
#include <mytree/MakeSimpleTree.h>

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libmytree.so)

void  MakeTree()
{
  Fun4AllServer *se = Fun4AllServer::instance();

  // since it doesn't matter we use a dummy input manager which just
  // drives the event loop with a runnumber of 310000
  recoConsts *rc = recoConsts::instance();
    rc->set_IntFlag( "RUNNUMBER", 310000);
  Fun4AllInputManager *in = new Fun4AllDummyInputManager( "DSTin");
  se->registerInputManager( in );

  // normally it's a good idea to have the name of the module
  // as argument. The name is needed if you want to have this module
  // discard events from the output. In case you want to run a few of these
  // modules you need to be able to set unique names somewhere
  SubsysReco *mytree = new MakeSimpleTree("MYTREE");
  se->registerSubsystem(mytree);
  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("OUT","mytree.root");
  // This is one of the few places where the name of a module is actually important - 
  // e.g. using duplicate names will definitely screw you here
  // The AddEventSelector tells the output manager which module can discard events
  // multiple modules are allowed (no list, you need an AddEventSelector line for
  // every module).
  out->AddEventSelector("MYTREE");
  // this adds these nodes to the output (if you do not select any the default is to
  // write all nodes. The nodes under The RunNode are always saved entirely
  out->AddNode("MYSIMPLETREE");
  out->AddNode("MYTCARRAY");
  se->registerOutputManager(out);
  se->run(100);
  se->End();
  delete se;
  gSystem->Exit(0);
}

#endif

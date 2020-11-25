#pragma once
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>

#include <mysimplereco/MySimpleReco.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libMySimpleReco.so)

void Fun4All_MySimpleReco(const int nEvents = 3)
{
  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance();
  MySimpleReco *myreco = new MySimpleReco();
  se->registerSubsystem(myreco);
// this (dummy) input manager just drives the event loop
  Fun4AllInputManager *in = new Fun4AllDummyInputManager("Bozo");
  in->Verbosity(1);
  se->registerInputManager( in );
// events = 0 => run till end of input file
  if (nEvents <= 0)
  {
    return;
  }
  cout << endl << "Running over " << nEvents << " Events" << endl;
  se->run(nEvents);
  cout << endl << "Calling End in Fun4All_MySimpleReco.C" << endl;
  se->End();
  cout << endl << "All done, calling delete Fun4AllServer" << endl;
  delete se;
  cout << endl << "gSystem->Exit(0)" << endl;
  gSystem->Exit(0);
}


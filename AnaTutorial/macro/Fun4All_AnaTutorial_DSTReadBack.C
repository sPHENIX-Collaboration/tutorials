#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>

#include <TSystem.h>

#include <anatutorial/AnaTutorial.h>

R__LOAD_LIBRARY(libanatutorial.so)

R__LOAD_LIBRARY(libfun4all.so)

//! Simple macro to readback previous generated simulation DSTs to analyze with `AnaTutorial`
void Fun4All_AnaTutorial_DSTReadBack(const int nevnt = 0, const std::string & inputfile = "G4sPHENIX.root")
{
  gSystem->Load("libg4dst");
  Fun4AllServer *se = Fun4AllServer::instance();

  AnaTutorial *anaTutorial = new AnaTutorial("anaTutorial", inputfile + "_anaTutorial.root");
  anaTutorial->setMinJetPt(4.);
  anaTutorial->Verbosity(0);
  anaTutorial->analyzeTracks(true);
  anaTutorial->analyzeClusters(true);
  anaTutorial->analyzeJets(true);
  anaTutorial->analyzeTruth(false);
  se->registerSubsystem(anaTutorial);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
  in->fileopen(inputfile);
  // or
  // in->AddListFile(inputfile);
  se->registerInputManager(in);

  se->run(nevnt);
  se->End();
  delete se;

  cout <<"All done. Exiting..."<<endl;

  gSystem->Exit(0);
}

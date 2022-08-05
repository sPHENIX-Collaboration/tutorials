#ifndef MACRO_FUN4ALLJETANA_C
#define MACRO_FUN4ALLJETANA_C

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>

/* #include <GlobalVariables.C> */
/* #include <G4_Global.C> */
#include <g4jets/FastJetAlgo.h>
#include <g4jets/JetReco.h>
#include <g4jets/TowerJetInput.h>
#include <g4jets/TruthJetInput.h>

#include <jetbackground/FastJetAlgoSub.h>

// here you need your package name (set in configure.ac)
#include <calojetrhoest/CaloJetRhoEst.h>
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4jets.so)
R__LOAD_LIBRARY(libjetbackground.so)
R__LOAD_LIBRARY(libcalojetrhoest.so)

void Fun4All_CaloJetRho(const int nevnt = 19)
{
  gSystem->Load("libcalojetrhoest");
  gSystem->Load("libg4dst");

  Fun4AllServer *se = Fun4AllServer::instance();

  int verbosity = 1;

  if (false) {
    JetReco                     *towerjetreco               =    new JetReco();
    towerjetreco->add_input(new TowerJetInput(Jet::CEMC_TOWER));
    towerjetreco->add_input(new TowerJetInput(Jet::HCALIN_TOWER));
    towerjetreco->add_input(new TowerJetInput(Jet::HCALOUT_TOWER));
    towerjetreco->add_algo(new  FastJetAlgoSub(Jet::ANTIKT, 0.4, 1), "AntiKt_Tower_r04_Sub1");
    towerjetreco->set_algo_node("ANTIKT");
    towerjetreco->set_input_node("TOWER");
    towerjetreco->Verbosity(verbosity);

    se->registerSubsystem(towerjetreco);
  }

  //  myJetAnalysis->Verbosity(0);
  // change lower pt and eta cut to make them visible using the example
  //  pythia8 file
  CaloJetRhoEst *myJetAnalysis = new CaloJetRhoEst("AntiKt_Tower_r04_Sub1", "AntiKt_Truth_r04", "myjetanalysis.root");
  myJetAnalysis->setPtRange(5, 100);
  myJetAnalysis->setEtaRange(-1.1, 1.1);
  myJetAnalysis->add_input(new TowerJetInput(Jet::CEMC_TOWER));
  myJetAnalysis->add_input(new TowerJetInput(Jet::HCALIN_TOWER));
  myJetAnalysis->add_input(new TowerJetInput(Jet::HCALOUT_TOWER));
  se->registerSubsystem(myJetAnalysis);

  // need truth jets
  // need calo  jets
  // need event info
  // need primary vertex
// $ CreateFileList.pl -run 4 -n 1000 -type 11 DST_VERTEX DST_CALO_G4HIT DST_CALO_CLUSTER DST_TRUTH_JET

  Fun4AllInputManager *intrue = new Fun4AllDstInputManager("DSTtruth");
  intrue->AddListFile("dst_truth_jet.list");
  se->registerInputManager(intrue);

  Fun4AllInputManager *incalo = new Fun4AllDstInputManager("DSTcalo");
  incalo->AddListFile("dst_calo_g4hit.list");
  se->registerInputManager(incalo);

  Fun4AllInputManager *incalocluster = new Fun4AllDstInputManager("DSTcalocluster");
  incalocluster->AddListFile("dst_calo_cluster.list");
  se->registerInputManager(incalocluster);

  Fun4AllInputManager *invertex = new Fun4AllDstInputManager("DSTvertex");
  invertex->AddListFile("dst_vertex.list");
  se->registerInputManager(invertex);

  se->run(nevnt);
  se->End();
  delete se;
  gSystem->Exit(0);
}

#endif

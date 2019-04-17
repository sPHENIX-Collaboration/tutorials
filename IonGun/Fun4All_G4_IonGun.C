#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <g4detectors/PHG4BlockSubsystem.h>
#include <g4histos/G4EdepNtuple.h>
#include <g4histos/G4HitNtuple.h>
#include <g4main/PHG4IonGun.h>
#include <g4main/PHG4ParticleGun.h>
#include <g4main/PHG4PrimaryGeneratorAction.h>
#include <g4main/PHG4Reco.h>
#include <g4main/PHG4TruthSubsystem.h>
#include "GlobalVariables.h"
#include "DisplayOn.C"

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4histos)
#endif

void Fun4All_G4_IonGun(int nEvents = 10)
{

  bool WriteDst = true; // set to true if yoy want to save everything in a Dst

  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance();
  //  se->Verbosity(1);
// size of the box in cm
  double xsize = 20.;  
  double ysize = 20.;  
  double zsize = 10.;  
// needs to be registered before PHG4Reco, otherwise you generate
// the ion after the G4 simulation is run leaving you with an empty detector
  igun = new PHG4IonGun();
  igun->SetA(197);
  igun->SetZ(79);
  igun->SetMom(0,0,1*197);
  igun->SetCharge(79);
  se->registerSubsystem(igun);
  
  PHG4Reco* g4Reco = new PHG4Reco();
// the default is a solenoidal field - we certainly do not want this
  g4Reco->set_field(0);
// set the world size and shape, make it large enough for your volume to fit
// but not much larger since G4 will track particles until they leave the
// world volume (or they interact/decay)
  g4Reco->SetWorldSizeX(500);
  g4Reco->SetWorldSizeY(500);
  g4Reco->SetWorldSizeZ(500);
  g4Reco->SetWorldShape("G4BOX");
// if you want vacuum use G4_Galactic (the thinnest material in G4)
  g4Reco->SetWorldMaterial("G4_AIR");
// Choose an existing physics list. They are explicitely implemented in
// our code, if you need another existing one, let us know.
// BIC is binary intranuclear cascade, suitable for ions
  g4Reco->SetPhysicsList("QGSP_BIC");

  PHG4BlockSubsystem *box = new PHG4BlockSubsystem("box1",1);
  box->set_double_param("size_x",xsize);
  box->set_double_param("size_y",ysize);
  box->set_double_param("size_z",zsize);
// shift by zsize/2. puts ion gun just at the front of the target box
// shifting by 10cm more gives some space to see the ion trail (but
// you will incur energy loss in 10cm of air)
  box->set_double_param("place_z",zsize/2.+10);
  box->set_string_param("material","G4_WATER"); // material of target box
  // normally G4 determines the stepsize automatically according to what
// physics process is chosen, leading to very coarse eloss resolution
// this sets the step size to 1mm, caveat: if the step sizes are too small
// the eloss calculation can be out of whack leading to very strange dEdx
  box->set_double_param("steplimits",0.1);
// do not integrate energy loss in active vlomue, store each G4 step
// as separate hit
  box->set_int_param("use_g4steps",1);
  box->SetActive();
  // this will check for overlaps during construction, if you have multiple 
  // volumes in very close proximity you might want to run this once with 
  // this flag enabled
  // box->OverlapCheck(1); 
  // the name used to construct the hit node, used in the ntuple code
  // to identify the target volume (case sensitive)
  box->SuperDetector("box");
  g4Reco->registerSubsystem(box);

  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);
  se->registerSubsystem( g4Reco );



// first argument is the name, second the filename for the ntuple
  G4EdepNtuple *edn = new G4EdepNtuple("EdepNtuple","G4EdepNtuple.root");
  edn->AddNode("box",1); // connects hit node name with detid in ntuple (G4HIT_box is detid=1)
  se->registerSubsystem(edn);
  G4HitNtuple *hit = new G4HitNtuple("HitNtuple","G4HitNtuple.root");
  hit->AddNode("box",1); // connects hit node name with detid in ntuple (G4HIT_box is detid=1)
  se->registerSubsystem(hit);
  ///////////////////////////////////////////
  // IOManagers...
  ///////////////////////////////////////////

  if (WriteDst)
  {
    Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT","G4.root");
    se->registerOutputManager(out);
  }
  Fun4AllInputManager *in = new Fun4AllDummyInputManager( "DUMMY");
  se->registerInputManager( in );
// only run if number of events is positive, otherwise quit here
// so one can start the display and then run events by hand
  if (nEvents > 0)
  {
    se->run(nEvents);
    se->End();
    std::cout << "All done" << std::endl;
    delete se;
    gSystem->Exit(0);
  }
}

PHG4IonGun *getgun()
{
  return igun;
}

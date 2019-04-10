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
#include <g4main/PHG4IonGun.h>
#include <g4main/PHG4Reco.h>
#include <g4main/PHG4PrimaryGeneratorAction.h>
#include "GlobalVariables.h"
#include "DisplayOn.C"

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4histos)
#endif

void Fun4All_G4_IonGun(
  int nEvents = 10
  )
{

  bool WriteDst = false; // set to true if yoy want to save everything in a Dst

  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance();
  //  se->Verbosity(1);

  double xsize = 20.;  
  double ysize = 20.;  
  double zsize = 10.;  
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
  box->set_double_param("place_z",zsize/2.+10);// shift box by 10 cm so we do not create particles in its center
  box->set_string_param("material","G4_WATER"); // material of box
  box->set_int_param("use_g4steps",1);
  box->SetActive();
  // this will check for overlaps during construction, if you have multiple volumes in very close 
  // proximity you might want to run this once with this flag enabled
  // box->OverlapCheck(1); 
  box->SuperDetector("box");
  g4Reco->registerSubsystem(box);

  se->registerSubsystem( g4Reco );

// the Ion Gun needs to be added after the se->registerSubsystem( g4Reco );
// which initializes G4 so we can set the G4 GeneratorAction
  igun = new PHG4IonGun();
  igun->SetA(197);
  igun->SetZ(79);
  igun->SetMom(0,0,100*197);
  igun->SetCharge(79);
  g4Reco->setGeneratorAction(igun);


// first argument is the name, second the filename for the ntuple
  G4EdepNtuple *edn = new G4EdepNtuple("EdepNtuple","G4EdepNtuple.root");
  edn->AddNode("box",1); // connects hit node name with detid in ntuple (G4HIT_box is detid=1)
  se->registerSubsystem(edn);
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

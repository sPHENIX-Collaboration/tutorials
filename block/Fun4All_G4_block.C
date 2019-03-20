#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <g4detectors/PHG4BlockSubsystem.h>
#include <g4eval/PHG4DSTReader.h>
#include <g4main/PHG4ParticleGun.h>
#include <g4main/PHG4Reco.h>
#include <g4main/PHG4TruthSubsystem.h>

R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
R__LOAD_LIBRARY(libg4detectors.so)
#endif

int Fun4All_G4_block(const int nEvents = 10, const char *outfile=NULL)
{

  gSystem->Load("libfun4all");
  gSystem->Load("libg4detectors");
  gSystem->Load("libg4testbench");
  gSystem->Load("libg4eval");
  gSystem->Load("libg4histos");

  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance();
  //  se->Verbosity(1);

  // particle gun
  PHG4ParticleGun *gun = new PHG4ParticleGun("PGUN");
  //  gun->set_name("anti_proton");
  gun->set_name("pi-");
  //  gun->set_name("proton");
  gun->set_vtx(0,0,0);
  gun->set_mom(0,0,1);
  se->registerSubsystem(gun);

  // Fun4All G4 module
  PHG4Reco* g4Reco = new PHG4Reco();
  // no magnetic field
  g4Reco->set_field(0);
  // size of the world - every detector has to fit in here
  g4Reco->SetWorldSizeX(500);
  g4Reco->SetWorldSizeY(500);
  g4Reco->SetWorldSizeZ(2000);
  // shape of our world - it is a box
  g4Reco->SetWorldShape("G4BOX");
  // this is what our world is filled with
  g4Reco->SetWorldMaterial("G4_AIR");
  // Geant4 Physics list to use
  g4Reco->SetPhysicsList("QGSP_BERT");

  // our block "detector", size is in cm
  double xsize = 200.;  
  double ysize = 200.;  
  double zsize = 400.;  
  PHG4BlockSubsystem *box = new PHG4BlockSubsystem("box");
  box->set_double_param("size_x",xsize);
  box->set_double_param("size_y",ysize);
  box->set_double_param("size_z",zsize);
  box->set_double_param("place_z",zsize/2.+100);// shift box so we do not create particles in its center and shift by 10 so we can see the track of the incoming particle
  box->set_string_param("material","G4_POLYSTYRENE"); // material of box
  box->SetActive(); // it is an active volume - save G4Hits
  g4Reco->registerSubsystem(box);

  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);

  se->registerSubsystem( g4Reco );

  ///////////////////////////////////////////
  // Output
  ///////////////////////////////////////////

  if (outfile)
    {
      Fun4AllOutputManager *out = new Fun4AllDstOutputManager("DSTOUT",outfile);
      se->registerOutputManager(out);

    }
  if (outfile)
    {
      // save a comprehensive  evaluation file
      PHG4DSTReader* ana = new PHG4DSTReader(
          string(outfile) + string("_DSTReader.root"));
      ana->set_save_particle(true);
      ana->set_load_all_particle(false);
      ana->set_load_active_particle(true);
      ana->set_save_vertex(true);
      if (nEvents > 0 && nEvents < 2)
        {
          ana->Verbosity(2);
        }
      ana->AddNode("box_0");
      se->registerSubsystem(ana);
    }

   // input - we need a dummy to drive the event loop
  Fun4AllInputManager *in = new Fun4AllDummyInputManager( "JADE");
  se->registerInputManager( in );

  // a quick evaluator to inspect on hit/particle/tower level


  if (nEvents > 0)
    {
      se->run(nEvents);
      // finish job - close and save output files
      se->End();
      std::cout << "All done" << std::endl;

      // cleanup - delete the server and exit
      delete se;
      gSystem->Exit(0);
    }
   return 0;

}

PHG4ParticleGun *get_gun(const char *name="PGUN")
{
  Fun4AllServer *se = Fun4AllServer::instance();
  PHG4ParticleGun *pgun = (PHG4ParticleGun  *) se->getSubsysReco(name);
  return pgun;
}


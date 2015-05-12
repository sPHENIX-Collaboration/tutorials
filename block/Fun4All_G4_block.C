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
  g4Reco->SetWorldSizeZ(500);
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
  box->SetSize(xsize,ysize,zsize);
  box->SetCenterZ(zsize/2.); // shift box so we do not create particles in its center
  box->SetMaterial("G4_W");
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
      Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT",outfile);
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
   return;

}

PHG4ParticleGun *get_gun(const char *name="PGUN")
{
  Fun4AllServer *se = Fun4AllServer::instance();
  PHG4ParticleGun *pgun = (PHG4ParticleGun  *) se->getSubsysReco(name);
  return pgun;
}


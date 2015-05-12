int Min_cemc_layer = 1;
int Max_cemc_layer = 1;
int Fun4All_G4_Clusters(
		       int nEvents = 100,
		       const char * inputFile = "/phenix/sim01/sPHENIX/pythia/reference_design/fieldmap/G4Hits_Pythia_0.root",
		       const char * outputFile = "G4sPHENIXClusters.root"
		       )
{
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libphhepmc.so");
  gSystem->Load("libg4testbench.so");
  gSystem->Load("libg4hough.so");
  gSystem->Load("libcemc.so");
  gSystem->Load("libg4eval.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  //------------------
  // Detector Division
  //------------------

  PHG4CylinderCellReco *cemc_cells = new PHG4CylinderCellReco("CEMCCYLCELLRECO");
  cemc_cells->Detector("CEMC");
  for (int i = Min_cemc_layer; i <= Max_cemc_layer; i++) {
    cemc_cells->etaphisize(i, 0.024, 0.024);
  }
  se->registerSubsystem(cemc_cells);

  //-----------------------------
  // CEMC towering and clustering
  //-----------------------------
  RawTowerBuilder *TowerBuilder = new RawTowerBuilder("EmcRawTowerBuilder");
  TowerBuilder->Detector("CEMC");
  TowerBuilder->Verbosity(0);
  se->registerSubsystem( TowerBuilder );
      
  RawClusterBuilder* ClusterBuilder = new RawClusterBuilder("EmcRawClusterBuilder");
  ClusterBuilder->Detector("CEMC");
  ClusterBuilder->Verbosity(0);
  se->registerSubsystem( ClusterBuilder );


      // Hits file
      Fun4AllInputManager *hitsin = new Fun4AllDstInputManager("DSTin");
      hitsin->fileopen(inputFile);
      se->registerInputManager(hitsin);
      Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outputFile);
      out->AddNode("G4HIT_CEMC");
      out->AddNode("G4CELL_CEMC");
      out->AddNode("TOWER_CEMC");
      out->AddNode("CLUSTER_CEMC");
   se->registerOutputManager(out);

  //-----------------
  // Event processing
  //-----------------
  se->run(nEvents);

  //-----
  // Exit
  //-----

  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  gSystem->Exit(0);
}

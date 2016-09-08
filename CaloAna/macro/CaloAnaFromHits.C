void CaloAnaFromHits(const char *fname = "/sphenix/sim/sim01/production/2016-07-21/single_particle/spacal2d/zerofield/G4Hits_sPHENIX_e+_eta0_16GeV-0000.root")
{
  gSystem->Load("libcaloana");

  // Begin create cells, towers, clusters
  // The G4Setup_sPHENIX.C macro comes from 
  // /sphenix/sim/sim01/production/2016-07-21/single_particle/spacal2d/
  // and should match the data file being analyzed
gROOT->SetMacroPath("/sphenix/sim/sim01/production/2016-07-21/single_particle/spacal2d");
  gROOT->LoadMacro("G4Setup_sPHENIX.C");
  G4Init(false,false,true,false,false,false,false);

  CEMC_Cells();
  CEMC_Towers();
  CEMC_Clusters();

  // End create cells, towers, clusters

  Fun4AllServer *se = Fun4AllServer::instance();

  // Create input and output objects

  CaloAna *ca = new CaloAna("CALOANA","cemc.root");
  ca->Detector("CEMC");
  se->registerSubsystem(ca);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("in");
  in->fileopen(fname);
  se->registerInputManager(in);

  se->run(10);
  se->End();
}


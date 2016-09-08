void CaloAna(const char *fname = "/phenix/scratch/pinkenbu/G4sPHENIXCells.root")
{
  gSystem->Load("libg4vertex_io");
  gSystem->Load("libg4hough_io");
  gSystem->Load("libcaloana");
  Fun4AllServer *se = Fun4AllServer::instance();
  CaloAna *ca = new CaloAna("CALOANA","out.root");
  // choose CEMC, HCALIN or HCALOUT or whatever you named your
  // calorimeter
  ca->Detector("HCALIN");
  se->registerSubsystem(ca);
  Fun4AllInputManager *in = new Fun4AllDstInputManager("in");
  in->fileopen(fname);
  se->registerInputManager(in);
  se->run();
  se->End();
}

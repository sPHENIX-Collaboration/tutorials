void G4TTree(const char *fname, const int nevnt = 0, const char *outfile="towers.root")
{
  gSystem->Load("libg4histos");
  Fun4AllServer *se = Fun4AllServer::instance();
  char detector[100];
  char outnode[100];
  sprintf(detector,"%s","CEMC"); 
 //  se->Verbosity(10);
  G4HitTTree *tt = new G4HitTTree();
  tt->Detector(detector);
  se->registerSubsystem(tt);

  G4RawTowerTTree *rt = new G4RawTowerTTree();
  rt->Detector(detector);
  se->registerSubsystem(rt);
  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
  in->fileopen(fname);
  se->registerInputManager(in);
  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("DSTout",outfile);
  sprintf(outnode,"G4RootHit_%s",detector);
  out->AddNode(outnode);
  sprintf(outnode,"G4RootRawTower_%s",detector);
  out->AddNode(outnode);
  se->registerOutputManager(out);

  se->run(nevnt);
  se->End();
  delete se;
}

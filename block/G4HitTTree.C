void G4HitTTree(const char *fname, const int nevnt = 0, const char *outfile="hits.root")
{
  gSystem->Load("libg4histos");
  Fun4AllServer *se = Fun4AllServer::instance();
  char detector[100];
  char outnode[100];
  sprintf(detector,"%s","box_0");
  sprintf(outnode,"G4RootHit_%s",detector);
  //  se->Verbosity(10);
  G4HitTTree *tt = new G4HitTTree();
  tt->Detector(detector);
  se->registerSubsystem(tt);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
  in->fileopen(fname);
  se->registerInputManager(in);
  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("DSTout",outfile);
  out->AddNode(outnode);
  se->registerOutputManager(out);

  se->run(nevnt);
  se->End();
  delete se;
}

void  AnalyzeTree()
{
  gSystem->Load("libmytree.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  Fun4AllInputManager *in1 = new Fun4AllDstInputManager("DSTIN1");
  se->registerInputManager(in1);
  se->fileopen("DSTIN1","mytree.root");
  SubsysReco *mytree = new AnalyzeSimpleTree();
  se->registerSubsystem(mytree);
  se->run(100);
  se->End();
  se->dumpHistos();
  delete se;
}

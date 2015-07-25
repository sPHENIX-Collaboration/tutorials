void  RunBoth()
{
  gSystem->Load("libmytree.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  // since it doesn't matter we use a dummy input manager which just
  // drives the event loop with a runnumber of 310000
  recoConsts *rc = recoConsts::instance()
    rc->set_IntFlag( "RUNNUMBER", 310000);
  Fun4AllInputManager *in = new Fun4AllDummyInputManager( "DSTin");
  se->registerInputManager( in );
  // make my ttree
  SubsysReco *mytree = new MakeSimpleTree("MYTREE");
  se->registerSubsystem(mytree);
  // and analyze it in the same process
  SubsysReco *myana = new AnalyzeSimpleTree();
  se->registerSubsystem(myana);

  se->run(100);
  se->End();
  se->dumpHistos();
  delete se;
}

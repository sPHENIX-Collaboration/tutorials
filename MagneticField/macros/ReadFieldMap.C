#ifndef MACRO_READFIELDMAP_C
#define MACRO_READFIELDMAP_C

#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <fieldmapreadback/FieldMapReadBack.h>

void ReadFieldMap(const int nEvents = 1, const std::string &filename="DST_TRKR_CLUSTER_sHijing_0_20fm_50kHz_bkg_0_20fm-0000000001-04001.root")
{
  gSystem->Load("libg4dst.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(); // set it to 1 if you want event printouts
  FieldMapReadBack *fmap = new FieldMapReadBack();
  se->registerSubsystem(fmap);
   Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
   in->fileopen(filename);
   se->registerInputManager(in);
//   Fun4AllDummyInputManager *in2 = new Fun4AllDummyInputManager("Zen");
//   se->registerInputManager(in2);
   if (nEvents < 0)
   {
     return;
   }
   se->run(nEvents);
   se->End();
   delete se;
   gSystem->Exit(0);
}


#endif

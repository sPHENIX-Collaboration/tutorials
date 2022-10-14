#ifndef MACRO_READFIELDMAP_C
#define MACRO_READFIELDMAP_C

#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <fieldmapreadback/FieldMapReadBack.h>

R__LOAD_LIBRARY(libfieldmapreadbackread.so)

// reads fieldmap from the DST. Needs only one event to force the opening
// and reading of the runwise information, running more just wastes your time

FieldMapReadBack *ReadFieldMap(const int nEvents = 1, const std::string &filename="DST_TRKR_CLUSTER_sHijing_0_20fm_50kHz_bkg_0_20fm-0000000001-04001.root")
{
  gSystem->Load("libg4dst.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(); // set it to 1 if you want event printouts
  FieldMapReadBack *fmap = new FieldMapReadBack();
  se->registerSubsystem(fmap);
  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
  in->fileopen(filename);
  se->registerInputManager(in);
  if (nEvents < 0)
  {
    return;
  }
  se->run(nEvents);
  return fmap;
}


#endif

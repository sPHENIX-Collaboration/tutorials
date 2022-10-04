#ifndef MACRO_FUN4ALLG4CYLINDER_C
#define MACRO_FUN4ALLG4CYLINDER_C

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4eval/PHG4DSTReader.h>
#include <g4main/PHG4ParticleGenerator.h>
#include <g4main/PHG4Reco.h>
#include <g4main/PHG4TruthSubsystem.h>
#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
R__LOAD_LIBRARY(libg4detectors.so)

int Fun4All_G4_Cylinder(const int nEvents = 10, const char * outfile = nullptr)
{

  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  recoConsts *rc = recoConsts::instance();
//  rc->set_IntFlag("RANDOMSEED", 12345); // if you want to use a fixed seed
  // PHG4ParticleGenerator generates particle
  // distributions in eta/phi/mom range
  PHG4ParticleGenerator *gen = new PHG4ParticleGenerator("PGENERATOR");
  //gen->set_name("gamma");
  gen->set_name("e+");
  gen->set_vtx(0, 0, 0);
  gen->set_eta_range(-0.05, +0.05);
  gen->set_mom_range(0.4, 10.0);
  gen->set_phi_range(0., 90. / 180.*TMath::Pi()); // 0-90 deg
  se->registerSubsystem(gen);

  PHG4Reco* g4Reco = new PHG4Reco();
  g4Reco->set_field(1.4); // 1.5 T solenoidal field 


  double si_thickness[6] = {0.02, 0.02, 0.0625, 0.032, 0.032, 0.032};
  double svxrad[6] = {2.71, 4.63, 11.765, 25.46, 41.38, 63.66};
  double length[6] = {20., 20., 36., -1., - 1., - 1.}; // -1 use eta coverage to determine length
  PHG4CylinderSubsystem *cyl;
  // here is our silicon:
   for (int ilayer = 0; ilayer < 6; ilayer++)
    {
      cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
      cyl->set_double_param("radius",svxrad[ilayer]);
      cyl->set_string_param("material","G4_Si");
      cyl->set_double_param("thickness",si_thickness[ilayer]);
      cyl->SetActive();
      cyl->SuperDetector("SVTX");
      if (length[ilayer] > 0)
        {
          cyl->set_double_param("length",length[ilayer]);
        }
      g4Reco->registerSubsystem( cyl );
    }
  se->registerSubsystem( g4Reco );

  if (outfile)
    {
      Fun4AllOutputManager *out = new Fun4AllDstOutputManager("DSTOUT",outfile);
      se->registerOutputManager(out);
    }
  Fun4AllInputManager *in = new Fun4AllDummyInputManager( "JADE");
  se->registerInputManager( in );


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
  return 0;
}

PHG4ParticleGenerator *get_gen(const char *name="PGENERATOR")
{
  Fun4AllServer *se = Fun4AllServer::instance();
  PHG4ParticleGenerator *pgun = (PHG4ParticleGenerator *) se->getSubsysReco(name);
  return pgun;
}

#endif

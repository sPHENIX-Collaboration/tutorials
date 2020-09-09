#ifndef FUN4ALL_G4_MOMENTUM_PROJECTION_C
#define FUN4ALL_G4_MOMENTUM_PROJECTION_C

#include <g4detectors/PHG4CylinderSubsystem.h>

#include <g4trackfastsim/PHG4TrackFastSim.h>
#include <g4trackfastsim/PHG4TrackFastSimEval.h>

#include <g4main/PHG4ParticleGenerator.h>
#include <g4main/PHG4ParticleGun.h>
#include <g4main/PHG4Reco.h>
#include <g4main/PHG4TruthSubsystem.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4trackfastsim.so)

int Fun4All_G4_Momentum_Projection(const int nEvents = 1000, const string &evalfile = "FastTrackingEval.root", const string &outfile = "")
{
  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  recoConsts *rc = recoConsts::instance();
  // if you want to use a fixed seed for reproducible results
  //rc->set_IntFlag("RANDOMSEED", 12345); // if you want to use a fixed seed
  // PHG4ParticleGenerator generates particle
  // distributions in eta/phi/mom range
  PHG4ParticleGenerator *gen = new PHG4ParticleGenerator("PGENERATOR");
  gen->set_name("pi-");
  gen->set_vtx(0, 0, 0);
  gen->set_eta_range(0.5, 0.5);
  gen->set_mom_range(2, 2);                   // GeV/c
  gen->set_phi_range(0., 90. / 180. * M_PI);  // 0-90 deg

  se->registerSubsystem(gen);

  PHG4Reco *g4Reco = new PHG4Reco();
  g4Reco->set_field(1.5);  // 1.5 T solenoidal field

  double si_thickness[6] = {0.02, 0.02, 0.0625, 0.032, 0.032, 0.032};
  double svxrad[6] = {2.71, 4.63, 11.765, 25.46, 41.38, 63.66};
  double length[6] = {20., 20., 36., -1., -1., -1.};  // -1 use eta coverage to determine length
  PHG4CylinderSubsystem *cyl;
  // here is our silicon:
  for (int ilayer = 0; ilayer < 6; ilayer++)
  {
    cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
    cyl->set_double_param("radius", svxrad[ilayer]);
    cyl->set_string_param("material", "G4_Si");
    cyl->set_double_param("thickness", si_thickness[ilayer]);
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    if (length[ilayer] > 0)
    {
      cyl->set_double_param("length", length[ilayer]);
    }
    g4Reco->registerSubsystem(cyl);
  }

  // Black hole swallows everything - prevent loopers from returning
  // to inner detectors
  cyl = new PHG4CylinderSubsystem("BlackHole", 0);
  cyl->set_double_param("radius", 80);      // 80 cm
  cyl->set_double_param("thickness", 0.1);  // does not matter (but > 0)
  cyl->SetActive();
  cyl->BlackHole();  // eats everything
  g4Reco->registerSubsystem(cyl);

  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);

  se->registerSubsystem(g4Reco);

  //---------------------------
  // fast pattern recognition and full Kalman filter
  // output evaluation file for truth track and reco tracks are PHG4TruthInfoContainer
  //---------------------------
  PHG4TrackFastSim *kalman = new PHG4TrackFastSim("PHG4TrackFastSim");
  kalman->set_use_vertex_in_fitting(false);
  kalman->set_sub_top_node_name("SVTX");
  kalman->set_trackmap_out_name("SvtxTrackMap");

  //  add Si Trtacker
  kalman->add_phg4hits(
      "G4HIT_SVTX",                //      const std::string& phg4hitsNames,
      PHG4TrackFastSim::Cylinder,  //      const DETECTOR_TYPE phg4dettype,
      300e-4,                      //       radial-resolution [cm]
      30e-4,                       //        azimuthal-resolution [cm]
      1,                           //      z-resolution [cm]
      1,                           //      efficiency,
      0                            //      noise hits
  );

  kalman->add_cylinder_state("MyCylinder1", 2.);   // projection onto cylinder with radius = 2cm
  kalman->add_cylinder_state("MyCylinder2", 70.);  // projection onto cylinder with radius = 70cm
  kalman->add_zplane_state("MyPlane1", 100.);      // projection onto z-plane at 100cm

  se->registerSubsystem(kalman);

  PHG4TrackFastSimEval *fast_sim_eval = new PHG4TrackFastSimEval("FastTrackingEval");
  fast_sim_eval->set_filename(evalfile);
  // Add the above projections to evaluation ntuple
  fast_sim_eval->AddProjection("MyCylinder1");
  fast_sim_eval->AddProjection("MyCylinder2");
  fast_sim_eval->AddProjection("MyPlane1");

  se->registerSubsystem(fast_sim_eval);
  //---------------------------

  //---------------------------
  // output DST file for further offlien analysis
  //---------------------------
  if (!outfile.empty())
  {
    Fun4AllOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outfile);
    se->registerOutputManager(out);
  }
  Fun4AllInputManager *in = new Fun4AllDummyInputManager("JASMINE");
  se->registerInputManager(in);

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

PHG4ParticleGenerator *get_gen(const char *name = "PGENERATOR")
{
  Fun4AllServer *se = Fun4AllServer::instance();
  PHG4ParticleGenerator *pgun = (PHG4ParticleGenerator *) se->getSubsysReco(name);
  return pgun;
}

#endif

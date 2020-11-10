#ifndef MACRO_MATSCAN_C
#define MACRO_MATSCAN_C

// just a dumb macro to run this before I forget how this is done
// to pipe the output into a file (T.T here) execute
// .L matscan.C
// ROOT6:
// .> T.T
// matscan()
// .q

// the span is the delta phi/theta you want to cover, not the maximum
// angle. The default is 10 bins in azimuth at theta=0.1 (almost 
// midrapidity, exact midrapidity we have gaps in the calorimeters and inner tracking

#include <fun4all/Fun4AllServer.h>
#include <g4main/PHG4Reco.h>

R__LOAD_LIBRARY(libg4testbench.so)


namespace MATSCAN
{
  float phimin = 0.;
  float phispan = 360.;
  int phibins = 10;
  float thetamin = 0.1; // theta = 0 is perpendicular to beam axis
  float thetaspan = 360.;
  int thetabins = 1;
}

void matscan()
{
  Fun4AllServer *se = Fun4AllServer::instance();
  PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
  g4->InitRun(se->topNode());
  char cmd[200];
  // set the desired phi range and binning (10 bins from 0-90 deg)
  sprintf(cmd,"/control/matScan/phi %d %f %f deg",MATSCAN::phibins,MATSCAN::phimin,MATSCAN::phispan);
  cout << "executing " << cmd << endl;
  g4->ApplyCommand(cmd);
  // set theta range - one at theta=0 which is vertically w.r.t. the beam axis
  sprintf(cmd,"/control/matScan/theta  %d %f %f deg",MATSCAN::thetabins,MATSCAN::thetamin,MATSCAN::thetaspan);
  cout << "executing " << cmd << endl;
  g4->ApplyCommand(cmd);
  // do the scan
  cout << "starting scan - patience" << endl;
  g4->ApplyCommand("/control/matScan/scan");
  cout << "All done" << endl;
}


void set_phimin(const float f)
{
  MATSCAN::phimin = f;
}

void set_phispan(const float f)
{
  MATSCAN::phispan = f;
}

void set_phibins(const int i)
{
  MATSCAN::phibins = i;
}

void set_thetamin(const float f)
{
  MATSCAN::thetamin = f;
}

void set_thetaspan(const float f)
{
  MATSCAN::thetaspan = f;
}

void set_thetabins(const int i)
{
  MATSCAN::thetabins = i;
}

void print()
{
  cout << "phibins: " << MATSCAN::phibins << endl;
  cout << "phimin: " << MATSCAN::phimin << endl;
  cout << "phispan: " << MATSCAN::phispan << endl;

  cout << "thetabins: " << MATSCAN::thetabins << endl;
  cout << "thetamin: " << MATSCAN::thetamin << endl;
  cout << "thetaspan: " << MATSCAN::thetaspan << endl;
}

// set values for 100 bins in phi from 0-5 degrees at midrapitity
void setmidrap()
{
  set_phibins(100);
  set_phimin(0);
  set_phispan(5);
  set_thetabins(1);
  set_thetamin(0.1);
  set_thetaspan(0);
}

void set_thetascan()
{
  set_phibins(1);
  set_phimin(1.); // do phi=1deg to avoid phi=0 in case there are discontinuities
  set_phispan(1);
  set_thetamin(-60);
  set_thetaspan(120);
  set_thetabins(60);
}

#endif

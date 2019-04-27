#pragma once
// just a dumb macro to run this before I forget how this is done
// to pipe the output into a file (T.T here) execute
// .L matscan.C
// ROOT5:
// matscan(); > T.T
// ROOT6:
// .> T.T
// matscan()
// .q

// the span is the delta phi/theta you want to cover, not the maximum
// angle. The default is 10 bins in azimuth at theta=0.1 (almost 
// midrapidity, exact midrapidity we have gaps in the calorimeters and inner tracking
float phimin = 0.;
float phispan = 360.;
int phibins = 10;
float thetamin = 0.1; // theta = 0 is perpendicular to beam axis
float thetaspan = 360.;
int thetabins = 1;

#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/Fun4AllServer.h>
#include <g4main/PHG4Reco.h>

R__LOAD_LIBRARY(libg4testbench.so)
#endif


void matscan()
{
  Fun4AllServer *se = Fun4AllServer::instance();
  PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
  g4->InitRun(se->topNode());
  char cmd[200];
  // set the desired phi range and binning (10 bins from 0-90 deg)
  sprintf(cmd,"/control/matScan/phi %d %f %f deg",phibins,phimin,phispan);
  cout << "executing " << cmd << endl;
  g4->ApplyCommand(cmd);
  // set theta range - one at theta=0 which is vertically w.r.t. the beam axis
  sprintf(cmd,"/control/matScan/theta  %d %f %f deg",thetabins,thetamin,thetaspan);
  cout << "executing " << cmd << endl;
  g4->ApplyCommand(cmd);
  // do the scan
  cout << "starting scan - patience" << endl;
  g4->ApplyCommand("/control/matScan/scan");
  cout << "All done" << endl;
}


void set_phimin(const float f)
{
  phimin = f;
}

void set_phispan(const float f)
{
  phispan = f;
}

void set_phibins(const int i)
{
  phibins = i;
}

void set_thetamin(const float f)
{
  thetamin = f;
}

void set_thetaspan(const float f)
{
  thetaspan = f;
}

void set_thetabins(const int i)
{
  thetabins = i;
}

void print()
{
  cout << "phibins: " << phibins << endl;
  cout << "phimin: " << phimin << endl;
  cout << "phispan: " << phispan << endl;

  cout << "thetabins: " << thetabins << endl;
  cout << "thetamin: " << thetamin << endl;
  cout << "thetaspan: " << thetaspan << endl;
}

// set values for 100 bins in phi from 0-5 degrees at midrapitity
void setmidrap()
{
  set_thetabins(1);
  set_phibins(100);
  set_phispan(5);
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

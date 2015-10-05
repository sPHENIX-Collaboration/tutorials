// just a dumb macro to run this before I forget how this is done
// to pipe the output into a file (T.T here) execute
// .L matscan.C
// matscan(); > T.T

// the span is the delta phi/theta you want to cover, not the maximum
// angle 
float phimin = 0.;
float phispan = 360.;
int phibins = 10;
float thetamin = 0.; // theta = 0 is perpendicular to beam axis
float thetaspan = 360.;
int thetabins = 10;

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

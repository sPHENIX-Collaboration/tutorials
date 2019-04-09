Small example of an Ion Gun shooting at a block of material
Usage:
// set up the environment:
source /opt/sphenix/core/bin/phenix_setup.csh -n

// start root
root.exe

// Run the default macro (10 events by default, argument is number of events)
.x Fun4All_G4_IonGun.C

the energy deposition is saved in an ntuple in G4EdepNtuple.root which contains the summed up 
energy deposited in the volume (separated by layers if implemented) as well as the sum over the
layers (layer = -1 for this).

Use the Display:

.x Fun4All_G4_IonGun.C(0)
.L DisplayOn.C
PHG4Reco *g4 = DisplayOn()

// get pointer to framework
Fun4AllServer *se = Fun4AllServer::instance();
// run one event
se->run(1);

displaycmd() gives some useful drawing commands

Modifying the Ion Gun:

// get a pointer to the ion gun
PHG4IonGun *gun = getgun()

// print current setting:
gun->Print()

This macro displays the output of various event generators. The input is 
selected by true/false switches. It is set to pythia6 as default. If 
HepMC files or EIC TTree file input is selected the filename has to be given 
as second argument. By default the first event is displayed. The visualization
macro contains a bunch of commented out options you may play with (after 
reading the GEANT4 docs).
Usage:

root.exe

.x Fun4All_Generator_Display.C()

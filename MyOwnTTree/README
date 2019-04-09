This directory contains 2 examples to create your own TTree using a class (and
Fun4All). The sources are in ./src, the macros to run the show are in ./macros.

The MySimpleTree class is stored in a TTree by itself and then inside a
TClonesArray (just to show you that the MySimpleTree class doesn't need any
modifications for the TClonesArray).

Unlike our DST's there is no virtual base class to keep old files readable 
after you change the content of your class. It's not difficult to add this 
feature but I wanted to keep it straight and simple.

Library build:
Don't build the library in the source area - it can be done but you do not do 
yourself any favors with this. Create a build area and think of an install area location (the install area will be created during "make install", you need to have write access). Go to the build area and

<srcdir>/autogen.sh --prefix=<install area>
make install

To run the macros you have to add your install area to the LD_LIBRARY_PATH and 
to the ROOT_INCLUDE_PATH (where root6 looks for includes mentioned in the 
macro). For the ROOT_INCLUDE_PATH we have a setup script which will add your install area but also the includes in OFFLINE_MAIN (and G4 and ROOT).

setenv LD_LIBRARY_PATH <install area>/lib:$LD_LIBRARY_PATH
source /opt/sphenix/core/bin/setup_root6.csh  <install area>

With these settings you can run the macros in the macro directory.

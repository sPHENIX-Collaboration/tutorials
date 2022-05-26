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

&lt;srcdir&gt;/autogen.sh --prefix=&lt;install area&gt;
make install

To run the macros you have to add your install area to the LD_LIBRARY_PATH and 
to the ROOT_INCLUDE_PATH (where root6 looks for includes mentioned in the 
macro). We have a setup script which will set all your paths accordingly.

source /opt/sphenix/core/bin/setup_local.csh  &lt;install area&gt;

With these settings you can run the macros in the macro directory.

#CreateSubsysRecoModule

run create_me.sh in source to create all files needed. This will create a 
SubsysReco module MySimpleReco and gives instructions how to build it and add 
the installation to the environment.

In macros, the Fun4All_MySimpleReco.C macro will run the just created module 
using a dummy input manafer which just drives the event loop. Each method 
called prints a message so you can see when they get called.
If you use this for your own module use

CreateSubsysRecoModule.pl <your module name>

options are:

  --all : Also create autogen.sh, configure.ac and Makefile.am

  --overwrite : overwrite existing files (handle with care, we only have snapshots on our $HOME disk)

#!/bin/csh

# This is an example script to run the basic macro associated to AnaTutorial. 
# It should only be considered an example! You need to modify the base directory and install directory to wherever you are working
# You should also adjust the line immediately below to mat
setenv HOME /phenix/u/$LOGNAME # (or /star/u/$LOGNAME or /eic/u/$LOGNAME)
source /etc/csh.login
foreach i (/etc/profile.d/*.csh)
  source $i
end
source $HOME/.login

# Set the baseDir to your directory where your macro exists
set baseDir=/sphenix/user/jdosbo/git/sphenix/tutorials/AnaTutorial/macro
# Set the install directory to the path where your AnaTutorial library is installed
set MYINSTALL=/sphenix/user/jdosbo/install

echo $baseDir

echo "Processing job "$1

# Source the newest sPHENIX build environment
source /cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/bin/sphenix_setup.csh -n
# Source the local environment with your AnaTutorial installation
source /cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/bin/setup_local.csh $MYINSTALL

cd $baseDir

# Run the Fun4All macro for 4 events
root -b -q Fun4All_AnaTutorial_sPHENIX.C\(4\)

#!/bin/bash

# This is an example bash script to run the basic macro associated to AnaTutorial.
# It should only be considered an example! You need to modify the base directory and install directory to wherever you are working
# You should also adjust the line immediately below to mat
 export USER="$(id -u -n)"
 export LOGNAME=${USER}
 export HOME=/phenix/u/${LOGNAME} # (or /star/u/${LOGNAME} or /sphenix/u/${LOGNAME} or /eic/u/${LOGNAME})

# Set the baseDir to your directory where your macro exists
baseDir=/sphenix/user/jdosbo/git/sphenix/tutorials/AnaTutorial/macro
# Set the install directory to the path where your AnaTutorial library is installed
MYINSTALL=/sphenix/user/jdosbo/install

echo $baseDir

echo "Processing job "$1

# Source the newest sPHENIX build environment
source /opt/sphenix/core/bin/sphenix_setup.sh -n
# Source the local environment with your AnaTutorial installation
source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL

# print the environment - needed for debugging (when you ask for help)
printenv

cd $baseDir

# Run the Fun4All macro for 4 events
root -b -q Fun4All_AnaTutorial_sPHENIX.C\(4\)

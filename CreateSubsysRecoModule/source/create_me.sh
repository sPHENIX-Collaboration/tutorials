#! /bin/bash
# You want to source the setup script before you run this
if [[ -z "$OFFLINE_MAIN" ]]
then
 echo "please source the setup script"
 exit 1
fi
# create all files (including autogen.sh, configure.ac and Makefile.am)
CreateSubsysRecoModule.pl MySimpleReco --all --overwrite
local_installdir=`pwd`/install

echo
echo "now create an installation dir and a build subdir, go to it and run"
echo
echo "../autogen.sh --prefix=$local_installdir"
echo "make install"
echo
echo "before testing, do not forget to source the setup_local.csh"
echo
echo "source /opt/sphenix/core/bin/setup_local.csh $local_installdir"
echo
unset local_installdir

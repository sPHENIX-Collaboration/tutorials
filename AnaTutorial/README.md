# Analysis Tutorial

This package is intended to demonstrate some basic features of analysis tutorials and walk a new user through building a package, extracting physics information from the NodeTree, and then using that package to produce a root file that can be used for further analysis. The package demonstrates some basic features and contains some useful code for extracting physics information like reconstructed tracks, reconstructed EMCal clusters, and truth particle information (amongst some other things).


## Building The Package

The first step to running the tutorial is to build the analysis package from src. Follow these instructions:

```bash
$ cd src
$ mkdir build
$ cd build
$ ../autogen.sh --prefix=$MYINSTALL
$ make
$ make install
```

Here, this assumes that you have set an environment variable `$MYINSTALL` that points to your install directory. For example, `$ echo $MYINSTALL` returns `/home/vagrant/install` for me, where the install directory is set on my virtual machine when running the Singularity container. The install directory can be wherever you like, but it is generally good practice to have one where all of your installation libraries will live.

You should also make sure that your `$LD_LIBRARY_PATH` and `$ROOT_INCLUDE_PATH` point to the correct place for your install directory. See [here](https://wiki.bnl.gov/sPHENIX/index.php/Example_of_using_DST_nodes) and [here](https://wiki.bnl.gov/sPHENIX/index.php/Sphenix_root6) for more details. You can use the following script to do this for you:


```bash
source /opt/sphenix/core/bin/setup_local.csh $MYINSTALL
```


## Running the example macro

The example macro, found in `macro/Fun4All_AnaTutorial.C`, is just the default `Fun4All_G4_sPHENIX.C` macro with the AnaTutorial package added to it. If you would like to see how the default macro and this macro are different, you can just open `Fun4All_AnaTutorial.C` in your favorite text editor and search for `anatutorial`, or run a `diff` command on the two files. 

Running the macro requires the rest of the sPHENIX macro repository. If you have already cloned it from git, you can simply do

```
$ cp Fun4All_AnaTutorial.C /the/path/to/macros/macros/g4simulations
```

and then run `$ root -l Fun4All_AnaTutorial.C`. Otherwise, first clone the macros repository as directed [here](https://wiki.bnl.gov/sPHENIX/index.php/Code_Repository), and then move the macro to your newly cloned `macros` directory.
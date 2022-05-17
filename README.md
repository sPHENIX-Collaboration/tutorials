
# Welcome

This tutorial respository carries a few example cases of analysis for [sPHENIX collaboration](https://www.sphenix.bnl.gov/). 

* Generators
  * __eventgenerator_display__: Display the output of our event generators in an empty world
* Simulation setups
  * __block__: build a simple block-shaped detector in Geant4
  * __cylinder__: build a simple cylinder-shaped detector in Geant4
  * __Momentum__: simulation/reconstruction for quick tracker tuning
* Simulation checks
  * __materialscan__: scan radiation and hadronic interaction length in ROOT command prompt
  * __PHG4DSTReader__: poke at simulation results with ROOT command lines. For official analysis please use analysis module such as the next a few items below
* Example analysis modules
  * __CreateSubsysRecoModule__: Create a SubsysReco module template yourself - no cut and paste anymore
  * __AnaTutorial__: analysis tutorial with compiled module processing track, clusters and jets. See [Recording and slides by Joe Osborn](https://indico.bnl.gov/event/7254/).
  * __MyOwnTTree__: two examples to create your own TTree using analysis module in the Fun4All framework
  * __myjetanalysis__: example to analysis jet and to perform jet fragmentation and jet shape analysis
  * __CaloAna__: example to fetch calorimeter hit, tower and clusters and save to a NTuple
* __JupyterLab__: run the sPHENIX anaysis on the [BNL SDCC Jupyter Lab web interface](https://jupyter.sdcc.bnl.gov/). 

# Useful links 

* Tutorials
  * [Get started on day-1](https://wiki.bnl.gov/sPHENIX/index.php/SPHENIX_software_day-1_checklist)
  * [Most recent tutorial session](https://indico.bnl.gov/event/7254/)
* Documentation
  * [Software wiki](https://wiki.bnl.gov/sPHENIX/index.php/Software)
  * [Doxygen: easier way to browse code](https://sphenix-collaboration.github.io/doxygen/)
  * [Build a package](https://wiki.bnl.gov/sPHENIX/index.php/Example_of_using_DST_nodes#Building%20a%20package)
* Communication
  * [Simulation and software meeting series](https://indico.bnl.gov/categoryDisplay.py?categId=88)
  * [Discussion on this repository](https://lists.bnl.gov/mailman/listinfo/sphenix-github-l)

[![logo](https://avatars3.githubusercontent.com/u/12069843?s=200&v=4)](https://www.sphenix.bnl.gov/)

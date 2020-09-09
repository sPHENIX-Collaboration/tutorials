# Example simulation/reconstruction for quick tracker tuning

Before running any of the macros, source the sphenix setup script:
```
source /opt/sphenix/core/bin/sphenix_setup.csh
```

The output file (by default FastTrackingEval.root) contains a tracks ntuple with track quantities (truth and reconstructed) and an ntuple with vertex quantities. The eta ranges differ in the macros to play with the coverage.

* Fun4All_G4_Momentum.C: 6 layer silicon cylinder detector with constant 1.5T solenoidal field.

* Fun4All_G4_Momentum_Projection.C: Same as above. With added projections on two cylinders (2cm and 70cm) and a vertical plane at 100cm downstream

* Fun4All_G4_Momentum_Projection_Detectors.C: Same as above, with added cylinders at the projections to get a measure of the actual simulated track

* Fun4All_G4_Momentum_Projection_Calorimeters.C: 6 layer silicon cylinder detector with constant 1.5T solenoidal field with central barrel EMCal (CEMC) and corresponding projections to it center. It runs very significantly longer than the other macros because the calorimeter sims are cpu intensive.

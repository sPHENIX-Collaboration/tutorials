***Be aware that the following instructions will run the material scan with our default world volume which is air and the default world is a cylinder with 5m radius.*** 

The cylinder shape will ensure azimuthal symmetry of the world but the air can add significantly to the X0. If you do a material scan consider to replace the world material by G4_Galactic. To do this find where our G4 module is created in the macro and add SetWorldMaterial("G4_Galactic"):
```
PHG4Reco* g4Reco = new PHG4Reco();
g4Reco->SetWorldMaterial("G4_Galactic")
```

How to use:
start root and
run the Fun4All_G4_sPHENIX.C (or fsPHENIX or ePHENIX) macro with -1 events 
(which just loads the G4 configuration). You'll need a lot of sub-macros, you 
definitely want to run this in another directory.

```
.x Fun4All_G4_sPHENIX.C(-1)
```

Load the matscan.C macro

```
.L matscan.C
```

and set the theta/phi ranges and number of bins you want with

```
set_phimin() // in degrees
set_phispan() // in degrees, phi range is phimin - phimin+phispan
set_phibins() // number of bins in this phi range
```

same for theta.
For scans in phi use theta_span = theta_min and theta_bins = 1, for midrapidity
scans you can use the setmidrap(); to set this up

For scans in theta use phi_span = phi_min and phi_bins = 1, use set_thetascan()
to do a scan in theta.

if you are running the same ranges all the time, just adjust those values in 
the macro.
Execute matscan() and store output in some file (here T.T), for Root5 use:

```
matscan(); > T.T
```

Root6:
uses a slightly different scheme, you need to redirect all output (at which time
you will not see any prompt anymore - genius)

```
.> T.T
matscan()
.q
```

Sadly this dumps more than just the x0/lamda values for each phi/theta bin.
In order to extract those, quit root and run matscan_digest.pl to extract 
x0/lamda0 for each phi/theta bin:

```
perl matscan_digest.pl T.T
```

This will create a file matscan.dat. 
plot_matscan.C is a simple plotting macro which plots x0/lamda vs phi (leave
theta constant by theta_span = theta_min and theta_bins = 1:

```
.x plot_matscan.C("matscan.dat")
```

If you want to plot theta (keeping phi constant when running matscan.C), use

```
.x plot_matscan.C("matscan.dat",1)
```

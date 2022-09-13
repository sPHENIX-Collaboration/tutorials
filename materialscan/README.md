The Fun4All_G4_sPHENIX.C and  G4Setup_sPHENIX.C macros which used to be here were outdated and therefore removed. Use a standard Fun4All_G4_sPHENIX.C macro instead. For the up to date version fetch it from

https://github.com/sPHENIX-Collaboration/macros/tree/master/detectors/sPHENIX

you will need Fun4All_G4_sPHENIX.C and G4Setup_sPHENIX.C

***Be aware that the following instructions will run the material scan with our default world volume which is air and the default world is a cylinder with 5m radius. This can add significantly to the radiation length X0***

The cylinder shape will ensure azimuthal symmetry of the world but the air can add significantly to the X0. If you do a material scan consider to replace the world material by G4_Galactic. To do this find where our G4 module is created in the macro and add SetWorldMaterial("G4_Galactic"):
```
PHG4Reco* g4Reco = new PHG4Reco();
g4Reco->SetWorldMaterial("G4_GALACTIC")
```

The included copy of the Fun4All_G4_sPHENIX.C macro uses G4_Galactic via the setting
```
G4WORLD::WorldMaterial = "G4_Galactic";
```
all Fun4All macros should support this setting (using a commented out line, if not let us know)

How to use:
start root and
run the Fun4All_G4_sPHENIX.C macro with -1 events
(which just loads the G4 configuration).

```
.x Fun4All_G4_sPHENIX.C(-1)
```

Load the matscan.C macro

```
.L matscan.C
```

By default it scans 10 bins in phi at midrapidity (theta=0.1 deg to avoid the gap many detectors have at exactly midrapidity). You can set the theta/phi ranges and number of bins you want with

```
set_phimin(float val) // in degrees
set_phispan(float val) // in degrees, phi range is phimin to phimin+phispan
set_phibins(float val) // number of bins in this phi range
```

same for theta.
```
set_thetamin(float val) // in degrees
set_thetaspan(float val) // in degrees, theta range is thetamin to thetamin+thetaspan
set_thetabins(float val) // number of bins in this theta range
```

For scans in phi use theta_span = theta_min and theta_bins = 1, for midrapidity
scans you can use

```
setmidrap();
```

to set this up. This scans 100 bins in phi between phi=0 and phi=5

For a default theta scan use
```
set_thetascan();
```
This scans 60 bins in theta from -60 to 120 deg at phi = 1 deg to avoid
possible gaps at phi=0. To set up your own theta scans, use phi_span = phi_min
and phi_bins = 1


if you are running the same ranges all the time, just adjust those values
in the macro.
Execute matscan() and store output in some file (here T.T). Root6
uses an interesting scheme, you need to redirect all output (at which time
you will not see any prompt anymore - pure genius)

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
.L plot_matscan.C
plot_matscan("matscan.dat");
```

If you want to plot theta (keeping phi constant when running matscan.C), use

```
.L plot_matscan.C
plot_matscan.C("matscan.dat",1);
```

If you want to plot eta (keeping phi constant when running matscan.C), use

```
.L plot_matscan.C
plot_matscan.C("matscan.dat",2);
```

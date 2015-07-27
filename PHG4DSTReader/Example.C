// $Id: $                                                                                             

/*!
 * \file Example.C
 * \brief 
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision:   $
 * \date $Date: $
 */

#include <cmath>
#include <TFile.h>
#include <TString.h>
#include <TPad.h>
#include <TTree.h>
#include <cassert>
using namespace std;

// allow you to access them in command line after running this macro
TFile * _file0 = NULL;
TTree * T = NULL;

/*
 * \brief Example macro to use output for PHG4DSTReader
 *
 * (s)PHENIX DST files are designed for high speed IO for batch computing,
 * the content is not necessarily easily readable through ROOT commandline.
 * This maybe some headache during testing/development stage.
 *
 * PHG4DSTReader is designed to solve this problem, by exporting a copy of
 * DST tree into a ROOT readable TTree file. User can directly inspect the content
 * using ROOT commandlines or simple macros.
 * https://www.phenix.bnl.gov/WWW/sPHENIX/doxygen/html/d4/dc9/classPHG4DSTReader.html
 *
 * PHG4DSTReader is by-default disabled in sPHENIX simulation, but can be enabled
 * for small-scale test simulation runs by following switch in  Fun4All_G4_sPHENIX.C
 * bool do_DSTReader = true;
 *
 * This example macro work on its output, usually named *_DSTReader.root.
 * If you do not yet have a ready use outout file, welcome to try the default example file (single pions in sPHENIX).
 *
 * */
void
Example( //
    const TString infile =
        "/phenix/u/jinhuang/links/webpage/sPHENIX/G4sPHENIXCells.root_DSTReader.root")
{

  // it is good to load the libg4eval lib here,
  // which allow us to use the compiled function in the output class
  gSystem->Load("libg4eval.so");

  // open file, get the tree
  _file0 = new TFile(infile);
  assert(_file0);
  T = (TTree *) _file0->GetObjectChecked("T", "TTree");
  assert(T);

  // Now I have bunch of example to show how to use this file.
  // They are self-explanatory
  //  and all the lines can be copy-paste in ROOT command lines too
  CheckItOut();
  AcessGeant4Particles();
  WhereIsTheHits();
  AccessMoreHitProperies();
  PlotCalorimeterSamplingFraction();
  AccessCalorimeterTowers();
}

//! First, checkout what is in there
void
CheckItOut()
{

  // This print out the content of the first event to screen.
  // It contain a bunch of nodes here:
  //  G4Hit_*.*         - PHG4Hit for various subsystems, https://www.phenix.bnl.gov/WWW/sPHENIX/doxygen/html/d3/d9e/classPHG4Hit.html
  //  TOWER_*.*         - RawTower for calorimeters, https://www.phenix.bnl.gov/WWW/sPHENIX/doxygen/html/d9/dd8/classRawTower.html
  //  PHG4Particle.*    - Geant4 particles, https://www.phenix.bnl.gov/WWW/sPHENIX/doxygen/html/de/dc9/classPHG4Particle.html
  //  PHG4VtxPoint.*    - Vertex point Geant4 particles were produced, https://www.phenix.bnl.gov/WWW/sPHENIX/doxygen/html/d6/d81/classPHG4VtxPoint.html
  T->Show(0);

  // Anything you see from the last print can be directly plotted using T->Draw();
  new TCanvas("CheckItOut", "CheckItOut");
  T->Draw(
      "Sum$(G4HIT_HCALOUT.edep) + Sum$(G4HIT_ABSORBER_HCALOUT.edep)>>hSum_CEMC(30,0,30)");
  hSum_CEMC->SetTitle(
      "Total energy deposition in Outer HCal;Energy deposition (GeV)");
}

//! Access information for the truth Geant4 particles
void
AcessGeant4Particles()
{

  // I am interested in first particle in the array, which is the generated pion.
  // Here our life would be easier with some new definition of variables
  T->SetAlias("PHG4Particle0_p",
      "1*sqrt(PHG4Particle[0].fpx**2+PHG4Particle[0].fpy**2+PHG4Particle[0].fpz**2)");
  T->SetAlias("PHG4Particle0_eta",
      "0.5*log((PHG4Particle0_p+PHG4Particle[0].fpz)/(PHG4Particle0_p-PHG4Particle[0].fpz))");
  T->SetAlias("PHG4Particle0_pT",
      "1*sqrt(PHG4Particle[0].fpx**2+PHG4Particle[0].fpy**2)");

  new TCanvas("AcessGeant4Particles", "AcessGeant4Particles");
  T->Draw("PHG4Particle0_pT>>hPHG4Particle0_pT(30,0,50)");
  hPHG4Particle0_pT->SetTitle("pT for the first Geant4 particle;pT (GeV/c)");

}

//! X-Y distribution for the hits
//! note, as long as we load libg4eval.so, the compiled functions can be used in T->Draw. Example, I will use
//! PHG4Hit::get_avg_x(), https://www.phenix.bnl.gov/WWW/sPHENIX/doxygen/html/d3/d9e/classPHG4Hit.html#a99663034e71d3f324eb878eb0e3b36ba
void
WhereIsTheHits()
{

  new TCanvas("WhereIsTheHits", "WhereIsTheHits");
  gPad->DrawFrame(-250, -250, 250, 250,
      "Geant4 Hit distribution;X (cm);Y (cm)");

  T->Draw("G4HIT_SVTX.get_avg_y():G4HIT_SVTX.get_avg_x()", "", "* same");
  T->Draw(
      "G4HIT_ABSORBER_CEMC.get_avg_y():G4HIT_ABSORBER_CEMC.get_avg_x()>>hG4HIT_ABSORBER_CEMC(500,-250,250,500,-250,250)",
      "", "scat same");
  T->Draw(
      "G4HIT_ABSORBER_HCALIN.get_avg_y():G4HIT_ABSORBER_HCALIN.get_avg_x()>>hG4HIT_ABSORBER_HCALIN(500,-250,250,500,-250,250)",
      "", "scat same");
  T->Draw(
      "G4HIT_ABSORBER_HCALOUT.get_avg_y():G4HIT_ABSORBER_HCALOUT.get_avg_x()>>hG4HIT_ABSORBER_HCALOUT(500,-250,250,500,-250,250)",
      "", "scat same");

}

//! some of the hit properties are compacted in std::map, and not shown in T->Show
//! but we can still access them through the access functions of PHG4Hits
void
AccessMoreHitProperies()
{
  new TCanvas("AccessMoreHitProperies", "AccessMoreHitProperies");

  //only plot the hit if the responsible Geant4 particle is trackid = 1 (the initial thrown particle)
  T->Draw("G4HIT_SVTX.get_layer()>>hladder_get_layer(7,-0.5,6.5)","G4HIT_SVTX.trackid == 1");
  hladder_get_layer->SetTitle("Silicon detector ladder index for primary particle hit;ladder layer index");

}

void
PlotCalorimeterSamplingFraction()
{

  // Here our life would be easier with some new definition of variables
  T->SetAlias("CEMC_Sample",
      "Sum$(G4HIT_CEMC.edep)/(Sum$(G4HIT_CEMC.edep) + Sum$(G4HIT_ABSORBER_CEMC.edep))");
  T->SetAlias("HCALIN_Sample",
      "Sum$(G4HIT_HCALIN.edep)/(Sum$(G4HIT_HCALIN.edep) + Sum$(G4HIT_ABSORBER_HCALIN.edep))");
  T->SetAlias("HCALOUT_Sample",
      "Sum$(G4HIT_HCALOUT.edep)/(Sum$(G4HIT_HCALOUT.edep) + Sum$(G4HIT_ABSORBER_HCALOUT.edep))");

  new TCanvas("PlotCalorimeterSamplingFraction", "PlotCalorimeterSamplingFraction");

  T->Draw("CEMC_Sample>>CEMC_Sample(30,0,.15)");
  CEMC_Sample->SetLineColor(kRed);
  T->Draw("HCALIN_Sample>>HCALIN_Sample(30,0,.15)","","same");
  HCALIN_Sample->SetLineColor(kMagenta);
  T->Draw("HCALOUT_Sample>>HCALOUT_Sample(30,0,.15)","","same");
  HCALOUT_Sample->SetLineColor(kBlue);
  CEMC_Sample->SetTitle("Sampling fraction for three calorimeters; E_{Scint}/(E_{Scint} + E_{Absorber})");

}

void
AccessCalorimeterTowers()
{
  new TCanvas("AccessCalorimeterTowers", "AccessCalorimeterTowers");

  T->Draw("TOWER_HCALOUT.get_bineta()>>hTowerBin(22,-0.5,21.5)","TOWER_HCALOUT.get_energy()");
  hTowerBin -> SetTitle("HCal tower eta bin distribution; Eta Bin # ; Sum energy in scintillator (GeV)");

}


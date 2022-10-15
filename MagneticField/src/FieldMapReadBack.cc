
#include "FieldMapReadBack.h"

#include <phfield/PHField.h>
#include <phfield/PHField3DCartesian.h>
#include <phfield/PHFieldUtility.h>

#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>

#include <Geant4/G4SystemOfUnits.hh>

#include <iostream>

//____________________________________________________________________________..
int FieldMapReadBack::InitRun(PHCompositeNode *topNode)
{
  fieldmap = PHFieldUtility::GetFieldMapNode(nullptr, topNode);
  if (fieldmap)
  {
    std::cout << "Found or created fieldmap" << std::endl;
  }
  else
  {
    std::cout << "Fieldmap not found or created" << std::endl;
  }
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int FieldMapReadBack::process_event(PHCompositeNode *topNode)
{
  double bfield[3];
  // the output needs to be converted into the unit you want - here it is tesla
  fieldmap->GetFieldValue(Point, bfield);
  std::cout << "bx: " << bfield[0] / tesla
            << " by: " << bfield[1] / tesla
            << " bz: " << bfield[2] / tesla
            << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

void FieldMapReadBack::Load3dCartMap(const std::string &fname, const float magfield_rescale)
{
  fieldmap = new PHField3DCartesian(fname, magfield_rescale);
}

void FieldMapReadBack::PrintField(const double x, const double y, const double z, const double t)
{
  SetFieldPoint(x, y, z, t);

  double Bf[3];
  fieldmap->GetFieldValue(Point, Bf);
  std::cout << "Point: " << x << "/" << y << "/" << z << " cm" << std::endl;
  std::cout << "BField: " << Bf[0] / tesla << "/" << Bf[1] / tesla << "/" << Bf[2] / tesla << std::endl;
  return;
}

void FieldMapReadBack::SetFieldPoint(const double x, const double y, const double z, const double t)
{
  Point[0] = x * cm;
  Point[1] = y * cm;
  Point[2] = z * cm;
  Point[3] = t * s;
}

void FieldMapReadBack::Verbosity(const int i)
{
  if (fieldmap)
  {
    fieldmap->Verbosity(i);
  }
  Fun4AllBase::Verbosity(i);
  return;
}

#ifndef PRINT3DFIELDMAP_H
#define PRINT3DFIELDMAP_H

#include <fieldmapreadback/FieldMapReadBack.h>

FieldMapReadBack *Print3dFieldMap(const float magfield_rescale = 1.)
{
  FieldMapReadBack *fmap = new FieldMapReadBack();
  fmap->Load3dCartMap("$CALIBRATIONROOT/Field/Map/sphenix3dbigmapxyz.root",magfield_rescale);
  fmap->Verbosity(2);
  fmap->PrintField(0,0,0);
  return fmap;
}

#endif

#ifndef PRINT3DFIELDMAP_H
#define PRINT3DFIELDMAP_H

#include <fieldmapreadback/FieldMapReadBack.h>

R__LOAD_LIBRARY(libfieldmapreadbackread.so)

// our maps can be found under
//$CALIBRATIONROOT/Field/Map

FieldMapReadBack *Print3dFieldMap(const std::string &mapname, const float magfield_rescale = 1.)
{
  FieldMapReadBack *fmap = new FieldMapReadBack();
  fmap->Load3dCartMap(mapname, magfield_rescale);
  fmap->Verbosity(2);
  fmap->PrintField(0, 0, 0);
  return fmap;
}

#endif

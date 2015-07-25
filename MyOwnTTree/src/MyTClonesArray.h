#ifndef MYTCLONESARRAY_H__
#define MYTCLONESARRAY_H__
// This is how to store a class inside a TClonesArray, useful for
// e.g. tracks or in general anything where you have many objects per event.
// This class doesn't make play the virtual base class inheritance game we do
// for our dst's, so old files won't be readable if you change this class
// root might still be able to read parts of old files but I really 
// wouldn't rely on this

#include <phool/PHObject.h>

class TClonesArray;
class MySimpleTree;

class MyTClonesArray : public PHObject
{
 public:
  MyTClonesArray();
  virtual ~MyTClonesArray();

  void Reset();
  MySimpleTree *GetNewItem();
  MySimpleTree *GetItem(const unsigned int i) const;
  int Entries();

 protected:

  TClonesArray *MyTCArray;

  ClassDef(MyTClonesArray,1)
};

#endif /*MYTCLONESARRAY_H__*/

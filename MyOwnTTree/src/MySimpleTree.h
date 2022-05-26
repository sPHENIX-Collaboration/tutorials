#ifndef MYSIMPLETREE_h
#define MYSIMPLETREE_h

// This class just stores an int and a float. While not needed it is 
// better in the long run to get used to use methods to get to the
// data. It doesn't matter for the root TTree which is saved but if
// you save this class inside a TClonesArray (the other example) it
// makes it easier. It also allows greater flexibility, e.g. if you ever
// want to ("dammit I forgot to multiply myfloat with Pi") modify the
// values on the fly:
// exchange
//  float MyFloat() const {return myfloat;}
// by
//  float MyFloat() const {return myfloat*M_PI;}
// and you don't have to change your analysis code one bit


#include <phool/PHObject.h>

#include <cmath> // for NAN

class MySimpleTree: public PHObject
{

 public:

  MySimpleTree();
  virtual ~MySimpleTree() {}

  void Reset();

  void MyFloat(const float f) {myfloat = f;}
  float MyFloat() const {return myfloat;}
  void MyInt(const int i) {myint = i;}
  int MyInt() const {return myint;}

 private:
  int myint = -9999;
  float myfloat = NAN;

  ClassDef(MySimpleTree,1)

};

#endif

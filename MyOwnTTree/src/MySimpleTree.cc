#include "MySimpleTree.h"

#include <cmath>

MySimpleTree::MySimpleTree()
{
  Reset(); // Init all data members
  return ;
}

void MySimpleTree::Reset()  // reset data members (called by phool node reset)
{
  myint = -9999;
  myfloat = NAN; // that's the way to get NaN's
  return ;
}


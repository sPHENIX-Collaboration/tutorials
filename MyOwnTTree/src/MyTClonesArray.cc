// This class puts rows of MySimpleTree classes into a TClonesArray

#include "MyTClonesArray.h"
#include "MySimpleTree.h"

#include <phool/phool.h>

#include <TClonesArray.h>
#include <iostream>

ClassImp(MyTClonesArray)

using namespace std;

// DEFAULTSIZE gives the initial number of entries
// the TClonesArray can hold and also the step size
// by which it is extended when needed
// this should be a meaningful number (like what you
// expect in most of the cases) to limit the number of
// times the TClonesArray has to be extended (and shrunk again
// at the end of the event) which eats a bit of cpu time

static int DEFAULTSIZE = 10;

MyTClonesArray::MyTClonesArray()
{
  MyTCArray = new TClonesArray("MySimpleTree",DEFAULTSIZE );
}


MyTClonesArray::~MyTClonesArray()
{
  if (MyTCArray)
    {
      MyTCArray->Clear();
      delete MyTCArray;
    }
  return ;
}


void 
MyTClonesArray::Reset()
{
  MyTCArray->Clear();
  if (MyTCArray->GetSize() > DEFAULTSIZE)
    {
      MyTCArray->Expand(DEFAULTSIZE);
    }
  return ;
}

// this method returns a pointer to the newly generated item
// in the TClonesArray.
// There is a bit TCLonesArray magic involved here but if you
// replace MySimpleTree by your class everywhere you should be fine.
// I once figured out a way to do this even more generic, but I couldn't
// locate this code on short notice (it's somewhere in our phuniverse I guess)
MySimpleTree *
MyTClonesArray::GetNewItem()
{
  // in a TClonesArray GetLast returns the last valid index, so add 1 to get
  // the one we need to create
  TClonesArray &cl = *MyTCArray;
  int nextindex = MyTCArray->GetLast()+1;
  // check if we are at the current capacity of our TClonesArray and 
  // if so extend the TClonesArray
  if (nextindex == MyTCArray->GetSize())
    {
      MyTCArray->Expand(MyTCArray->GetSize() + DEFAULTSIZE);
    }
  new(cl[nextindex]) MySimpleTree();
  return (static_cast<MySimpleTree *> (cl[nextindex]));
}

MySimpleTree *
MyTClonesArray::GetItem(const unsigned int i) const
{

  if (i > (unsigned int) MyTCArray->GetLast())
    {
      cout << PHWHERE << " Index " << i 
           << " out of range, max = " << MyTCArray->GetLast() << endl;
      return 0;
    }

  return (static_cast<MySimpleTree *> (MyTCArray->UncheckedAt(i)));
}

int
MyTClonesArray::Entries()
{
  return MyTCArray->GetEntriesFast();
}

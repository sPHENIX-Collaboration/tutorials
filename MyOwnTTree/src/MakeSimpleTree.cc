#include "MakeSimpleTree.h"
#include "MySimpleTree.h"
#include "MyTClonesArray.h"

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/getClass.h>

#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>
#include <phool/PHNodeIterator.h>

using namespace std;

MakeSimpleTree::MakeSimpleTree(const string &name): SubsysReco(name)
{
  return;
}

int
MakeSimpleTree::Init(PHCompositeNode *topNode)
{
  PHNodeIterator iter(topNode);

  PHCompositeNode *dstNode;
  dstNode = dynamic_cast<PHCompositeNode*>(iter.findFirst("PHCompositeNode", "DST"));
  if (!dstNode)
    {
      cout << PHWHERE << "DST Node missing doing nothing" << endl;
      return -1;
    }

  // this creates your objects and puts them on the node tree for
  // later saving. The "PHObject" argument is needed since phool still defaults
  // to the old staf tables. The second argument is the name you give to that node. It
  // makes ones life easier if the name is identical or related to the object you store
  // but it can be anything.
  // The name of the Node will be reflected in the branch name inside the root file
  // Keep that in mind if you want to analyze the root file standalone
  MySimpleTree *mytree = new MySimpleTree();
  PHIODataNode <PHObject> *newNode = new PHIODataNode <PHObject>(mytree,"MYSIMPLETREE","PHObject");
  dstNode->addNode(newNode);
  MyTClonesArray *mycontainer = new MyTClonesArray();
  newNode = new PHIODataNode <PHObject>(mycontainer,"MYTCARRAY","PHObject");
  dstNode->addNode(newNode);
  return 0;
}

int
MakeSimpleTree::process_event(PHCompositeNode *topNode)
{
  static int i = 0;
  MySimpleTree *mytree = findNode::getClass<MySimpleTree>(topNode,"MYSIMPLETREE");
  float f = i;
  mytree->MyFloat(f);
  mytree->MyInt(i);
   MyTClonesArray *mycontainer = findNode::getClass<MyTClonesArray>(topNode,"MYTCARRAY");
   for (int j=0; j<i;j++)
     {
       MySimpleTree *item = mycontainer->GetNewItem();
       item->MyFloat(f);
       item->MyInt(i);
     }
   mycontainer->MyEventInt(i);
   mycontainer->MyEventFloat(f);
   i++;
   // Fun4All looks at the return codes from each module
   // DISCARDEVENT tells an output manager which has this module
   // added to its "EventSelector" not to write this event out
   // You should use this if you don't want to store data from
   // this event (even "empty" objects take space and make the looping
   // over it inefficient)
   // 
   // EVENT_OK tells Fun4All all went well
   // This code just drops every other event from the output
   if (i % 2)
     {
       return Fun4AllReturnCodes::DISCARDEVENT;
     }
   return Fun4AllReturnCodes::EVENT_OK;
}

#include "AnalyzeSimpleTree.h"
#include "MySimpleTree.h"
#include "MyTClonesArray.h"

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllHistoManager.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/getClass.h>

#include <phool/PHCompositeNode.h>

#include <TH1.h>
#include <TH2.h>

using namespace std;

AnalyzeSimpleTree::AnalyzeSimpleTree(const string &name): SubsysReco(name)
{
  return ;
}

int
AnalyzeSimpleTree::Init(PHCompositeNode *topNode)
{
  Fun4AllServer *se = Fun4AllServer::instance();
  // this makes a Fun4AllHistoManager which can be used to save your histograms easily
  // with roots pathetic TDirectory handling and the hoops and loops Fun4All has to
  // do to accomodate for it it's hard to save histograms on your own and it might not
  // work reliably. But still keep local pointers to the histograms in your class instead
  // of retrieving them from the histo manager via their name. This lookup is a string search
  // which is extremely slow and you do not want to do this for every event
  hm = new Fun4AllHistoManager("MYHISTOS");
  se->registerHistoManager(hm);
  myfloats = new TH1F("myfloats", "these are stupid floats", 201, -0.5, 200);
  my2dfloats = new TH2F("my2dfloats", "these floats are stupid in 2d", 201, -0.5, 199.5, 201, -0.5, 199.5);
  // this reegisters the histograms with the histo manager for later saving. It will use
  // the names you gave to the histograms when you created them.
  hm->registerHisto(myfloats);
  hm->registerHisto(my2dfloats);
  return 0;
}

int
AnalyzeSimpleTree::process_event(PHCompositeNode *topNode)
{
  // Find the object on the node tree and fill some of its content into a histogram
  MySimpleTree *mytree = findNode::getClass<MySimpleTree>(topNode, "MYSIMPLETREE");
  myfloats->Fill(mytree->MyFloat());
  // for TClonesArrays we need to loop over its Entries, get a pointer to the class
  // which is stored inside it and then use that pointer to fill a histogram
  MyTClonesArray *mycontainer = findNode::getClass<MyTClonesArray>(topNode, "MYTCARRAY");
  for (int j = 0; j < mycontainer->Entries();j++)
    {
      MySimpleTree *item = mycontainer->GetItem(j);
      my2dfloats->Fill(mytree->MyFloat(), item->MyFloat());
    }
  return Fun4AllReturnCodes::EVENT_OK;
}

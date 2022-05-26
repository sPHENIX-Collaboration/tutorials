#ifndef ANALYZESIMPLETREE_H__
#define ANALYZESIMPLETREE_H__

#include <fun4all/SubsysReco.h>

class Fun4AllHistoManager;
class TH1;
class TH2;

class AnalyzeSimpleTree: public SubsysReco
{
 public:

  AnalyzeSimpleTree(const std::string &name = "ANALYZETREE");
  ~AnalyzeSimpleTree()  override {}

  int Init(PHCompositeNode * /*topNode*/) override;

  int process_event(PHCompositeNode *topNode) override;

  int End(PHCompositeNode * /*topNode*/) override;
 private:

  Fun4AllHistoManager *hm = nullptr;
  TH1 *myfloats = nullptr;
  TH2 *my2dfloats = nullptr;
};

#endif /* ANALYZESIMPLETREE_H__ */


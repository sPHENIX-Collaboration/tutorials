#ifndef MAKESIMPLETREE_H__
#define MAKESIMPLETREE_H__

#include <fun4all/SubsysReco.h>

class MakeSimpleTree: public SubsysReco
{
 public:

  MakeSimpleTree(const std::string &name = "MAKETREE");
  ~MakeSimpleTree() override {}

  int Init(PHCompositeNode *topNode) override;

  int process_event(PHCompositeNode *topNode) override;

 private:


};

#endif /* MAKESIMPLETREE_H__ */


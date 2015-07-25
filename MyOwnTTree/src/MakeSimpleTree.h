#ifndef MAKESIMPLETREE_H__
#define MAKESIMPLETREE_H__

#include <fun4all/SubsysReco.h>

class MakeSimpleTree: public SubsysReco
{
 public:

  MakeSimpleTree(const std::string &name = "MAKETREE");
  virtual ~MakeSimpleTree() {}

  int Init(PHCompositeNode *topNode);

  int process_event(PHCompositeNode *topNode);

 protected:


};

#endif /* MAKESIMPLETREE_H__ */


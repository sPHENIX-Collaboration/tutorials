// Tell emacs that this is a C++ source
//  -*- C++ -*-.

#ifndef DETINFO_H
#define DETINFO_H

#include <fun4all/SubsysReco.h>

#include <string>
#include <vector>
#include <TMath.h>

#include <cdbobjects/CDBTTree.h>



class TFile;
class TTree;
class TNtuple;
class TH1;
class TProfile;

class PHCompositeNode;

class Detinfo : public SubsysReco
{
 public:

  Detinfo(const std::string &name = "Detinfo");

  ~Detinfo() override;

  int Init(PHCompositeNode *topNode) override;
  int InitRun(PHCompositeNode *topNode) override;
  int process_event(PHCompositeNode *topNode) override;
  int ResetEvent(PHCompositeNode *topNode) override;
  int EndRun(const int runnumber) override;
  int End(PHCompositeNode *topNode) override;
  int Reset(PHCompositeNode * /*topNode*/) override;
  
  void set_filename(const char* file)
  { if(file) _outfile_name = file; }
    
 private:
 std::string _outfile_name;
 std::string m_sEPDMapName;
 std::string m_sEPDfieldname;
 bool m_overrideSEPDMapName{false};
 bool m_overrideSEPDFieldName{false};
 CDBTTree *cdbttree{nullptr};
 unsigned int key = 999;
 std::vector<unsigned int> v;
 
 uint64_t GL1Packet_BCO_ = 0;
 uint64_t intt_bco = 0;
   

  TTree* _event_tree;
  std::vector<int> _c;
  std::vector<float> _t;
  std::vector<float> _tt;
  std::vector<int> _a;
  std::vector<float> _p;
  std::vector<float> _pt;
  std::vector<float> _z;
  std::vector<float> _zt;
 
 TH1 *h_triggerVec{nullptr};
 float thisvertex{-999};
 float centrality_mbd_{std::numeric_limits<float>::quiet_NaN()};
 bool is_min_bias;
    
 float Qx_south{std::numeric_limits<float>::quiet_NaN()};
 float Qy_south{std::numeric_limits<float>::quiet_NaN()};
 float Qx_north{std::numeric_limits<float>::quiet_NaN()};
 float Qy_north{std::numeric_limits<float>::quiet_NaN()};
    
 float mbd_q_south{0.};
 float mbd_q_north{0.};
 float sepd_q_south{0.};
 float sepd_q_north{0.};
 float zdc_e_south{0.};
 float zdc_e_north{0.};

 float psi2_south{std::numeric_limits<float>::quiet_NaN()};
 float psi2_north{std::numeric_limits<float>::quiet_NaN()};
 int _event;
    
};

#endif // DETINFO_H

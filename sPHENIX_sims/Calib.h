

#ifndef __Calib_H__
#define __Calib_H__

#include <fun4all/SubsysReco.h>
#include <string>
#include <vector>



//Forward declerations
class PHCompositeNode;
class TFile; 
class TTree;
class RawTowerContainer;
class RawTowerGeomContainer;
class TProfile2D;

// towers
RawTowerContainer* _ohcal_towers_o;



//tower geom
RawTowerGeomContainer* _ohcal_towergeom;




//Brief: basic ntuple and histogram creation for sim evaluation
class Calib: public SubsysReco
{
 public: 
  //Default constructor
    Calib(const std::string &name="Calib");

  //Initialization, called for initialization
  int Init(PHCompositeNode *);

  int InitRun(PHCompositeNode *); 

  //Process Event, called for each event
  int process_event(PHCompositeNode *);

  //End, write and close files
  int EndRun(PHCompositeNode *);
  int End(PHCompositeNode *);

  //Change output filename
  void set_filename(const char* file)
  { if(file) _outfile_name = file; }

 private:
  //output filename
  std::string _outfile_name;
   
  //Event counter
  int _event;

  float _oCalib_ohcal;

  //User modules
  void fill_tree(PHCompositeNode*);

  //Get all the nodes
  int GetNodes(PHCompositeNode *);
  
  TProfile2D *hprof2d;

  //TTrees
  TTree* _event_tree;


 
};

#endif //* __Calib_H__ *//

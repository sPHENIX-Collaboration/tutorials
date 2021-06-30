// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef FIELDMAPREADBACK_H
#define FIELDMAPREADBACK_H

#include <fun4all/SubsysReco.h>

#include <string>

class PHCompositeNode;
class PHField;

class FieldMapReadBack : public SubsysReco
{
 public:
  FieldMapReadBack(const std::string &name = "FieldMapReadBack") {}

  virtual ~FieldMapReadBack() {}

  /** Called for first event when run number is known.
      Typically this is where you may want to fetch data from
      database, because you know the run number. A place
      to book histograms which have to know the run number.
   */
  int InitRun(PHCompositeNode *topNode) override;

  /** Called for each event.
      This is where you do the real work.
   */
  int process_event(PHCompositeNode *topNode) override;

  void Load3dCartMap(const std::string &fname, const float magfield_rescale = 1.0);
  void PrintField(const double x, const double y, const double z, const double t = 0.);
  void Verbosity(const int i) override;
  void SetFieldPoint(const double x, const double y, const double z, const double t = 0.);

 private:
  PHField *fieldmap = nullptr;
  double Point[4] = {0};
};

#endif  // FIELDMAPREADBACK_H

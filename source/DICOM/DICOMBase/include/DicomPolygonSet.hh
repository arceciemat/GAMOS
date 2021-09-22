#ifndef DicomPolygonSet__HH
#define DicomPolygonSet__HH

#include "DicomVLineSet.hh"
class DicomVImageStr;
class DicomPolygonList;
#include <vector>
#include <map>
#include "globals.hh"

class DicomPolygonSet : public DicomVLineSet
{ 
public:
  DicomPolygonSet( G4String name, DPOrientation ori);
  DicomPolygonSet(DicomPolygonSet* polySetOld, DicomVImage* image, DPOrientation ori);
  ~DicomPolygonSet(){};

public:
  void FindVoxelsInXY( DicomVImageStr* );
  
};

#endif
  

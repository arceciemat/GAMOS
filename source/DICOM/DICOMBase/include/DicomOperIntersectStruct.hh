#ifndef DicomOperIntersectStruct__HH
#define DicomOperIntersectStruct__HH

#include "DicomVOperator.hh"
class DicomVImage;
class DicomPolygonList;

class DicomOperIntersectStruct : public DicomVOperator
{ 
public:
  DicomOperIntersectStruct( G4int iOrder = INT_MAX, G4String name = "DicomOperIntersectStruct");
  ~DicomOperIntersectStruct(){};

public:
  virtual void Operate( DicomVImage* );
  void Operate( DicomVImage* image, DicomPolygonList* polyList );
  
private:
};

#endif
  

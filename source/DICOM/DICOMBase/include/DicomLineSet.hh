#ifndef DicomLineSet__HH
#define DicomLineSet__HH

#include "DicomVLineSet.hh"
class DicomVImageStr;
#include <vector>
#include <map>
#include "globals.hh"

class DicomLineSet : public DicomVLineSet
{ 
public:
  DicomLineSet( G4String name, DPOrientation ori );
  ~DicomLineSet(){};

public:
  void FindVoxelsInXY( DicomVImageStr* );
  
};

#endif
  

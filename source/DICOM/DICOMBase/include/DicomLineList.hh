#ifndef DicomLineList__HH
#define DicomLineList__HH

#include "DicomVLineList.hh"
class DicomVImage;

#include <vector>

class DicomLineList : public DicomVLineList
{ 
public:
  DicomLineList( G4String name, size_t num, DPOrientation ori );
  ~DicomLineList(){};

};

#endif
  

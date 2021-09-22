#ifndef DicomReaderLine__HH
#define DicomReaderLine__HH
#include "globals.hh"
class DicomVLineSet;

class DicomReaderLine 
{
public:
  DicomReaderLine(G4String fileName);
  ~DicomReaderLine(){};

  DicomVLineSet* GetLineSet() const {
    return theLineSet;
  }

private:
  G4String theFileName;
  DicomVLineSet* theLineSet;  
};

#endif

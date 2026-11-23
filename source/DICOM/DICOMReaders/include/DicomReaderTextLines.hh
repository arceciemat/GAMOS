#ifndef DicomReaderTextLines__HH
#define DicomReaderTextLines__HH
#include "globals.hh"
#include "DicomVReader.hh"
class DicomVLineSet;

class DicomReaderTextLines : public DicomVReader
{
public:
  DicomReaderTextLines(G4String fileName);
  ~DicomReaderTextLines(){};

  DicomVLineSet* GetLineSet() const {
    return theLineSet;
  }

  virtual void ReadData();
  
private:
  DicomVLineSet* theLineSet;  
};

#endif

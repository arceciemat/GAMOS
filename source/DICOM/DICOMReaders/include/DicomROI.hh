#ifndef DicomROI__HH
#define DicomROI__HH

#include "dcmtk/dcmdata/dcfilefo.h"

#include <vector>
#include <iostream>
#include "DicomROIContour.hh"

class DicomROI 
{ 
public:
  DicomROI(int ROINumber, OFString ROIName);
  ~DicomROI(){};

public:
  void AddContour( DicomROIContour* );
  std::vector<DicomROIContour*> GetContours() const {
    return theContours;
  }
  void Print(std::ostream& out);
  int GetNumber() const {
    return theNumber;
  }
  OFString GetName() const {
    return theName;
  }

private:
  int theNumber;
  OFString theName;

  std::vector<DicomROIContour*> theContours;
  
};

#endif
  

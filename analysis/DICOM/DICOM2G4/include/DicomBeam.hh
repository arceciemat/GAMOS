#ifndef DicomBeam__HH
#define DicomBeam__HH

#include "dcmtk/dcmdata/dcfilefo.h"
#include "G4ThreeVector.hh"

#include <vector>
#include <fstream>
class DicomVBeamDevice;
class DicomVBeamRTControlPoint;
class DicomBeamCompensator;
class DicomBeamBlock;
class DicomBeamWedge;
#include "DICOM/DICOMBase/include/DicomVObjectWithParams.hh"

class DicomBeam : public DicomVObjectWithParams
{ 
public:
  DicomBeam(bool bIsRef);
  ~DicomBeam(){};

public:
   void SetNumber(Sint32 dat){ 
    theNumber = dat;
  }
  void AddDevice( DicomVBeamDevice* db ){
    theDevices.push_back(db);
  }
  void AddControlPoint( DicomVBeamRTControlPoint* db ){
    theControlPoints.push_back(db);
  }
  void AddCompensator( DicomBeamCompensator* db ){
    theCompensators.push_back(db);
  }
  void AddBlock( DicomBeamBlock* db ){
    theBlocks.push_back(db);
  }
  void AddWedge( DicomBeamWedge* db ){
    theWedges.push_back(db);
  }
  size_t GetNControlPoints() const {
    return theControlPoints.size();
  }
  DicomVBeamRTControlPoint* GetControlPoint( size_t ii ) {
    return theControlPoints[ii];
  }

  void SetControlPointMetersets();
  
  void DumpToFileF(std::string fileName);
  
private:
  //  Float64 theSourceAxisDistance; 1 for RT, 2 for RTIon (VirtualSourceAxisDistances)
  std::vector<DicomVBeamDevice*> theDevices;
  std::vector<DicomVBeamRTControlPoint*> theControlPoints;
  std::vector<DicomBeamCompensator*> theCompensators; 
  std::vector<DicomBeamBlock*> theBlocks; 
  std::vector<DicomBeamWedge*> theWedges; 
  Sint32 theNumber;
  bool bIsReferenced;
};

#endif  
  

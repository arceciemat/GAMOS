#ifndef DicomMgr__HH
#define DicomMgr__HH
#include <vector>
#include "globals.hh"
#include "GamosCore/GamosUtils/include/GmVerbosity.hh"

#include "DicomVImage.hh"
#include "DicomLineDrawingOptions.hh"
#include <map>
class DicomVOperator;
class DcmDataset;
typedef std::multimap<DIModality,DicomVImage*> mmmodi;

#include "TColor.h"

typedef std::map<G4String,DicomLineDrawingOptions*> msldo;

class DicomMgr 
{
public:
  static DicomMgr* GetInstance();
  ~DicomMgr(){};

private:
  DicomMgr();

public:
  void AddImage( DicomVImage* image );

  void AddOperator( DicomVOperator* oper, G4int iOrder );
  void OperateAll();
  void OperateAll( DicomVImage* img );
  DicomVOperator* FindOperator( G4String name );
  
  void SetVerbosity( G4String verbstr );

  void SetInPolygonNCheck( G4int nsc ){
    theInPolygonNCheck = nsc;
  }
  
  G4int GetInPolygonNCheck() const {
    return theInPolygonNCheck;
  }

  G4int GetNofImages( DIModality mod ) const;

  void SetRotateXY180( G4bool brot ) {
    bRotateXY180 = brot;
  }
  G4bool GetRotateXY180() const {
    return bRotateXY180;
  }

  void ReadLineDrawingOptios( G4String drawLinesFN );  
  msldo GetLineDrawingOptions() const {
    return theLineDrawingOptions;
  }
  std::vector<DicomVOperator*> GetOperators() const {
    return theOperators;
  }

public:
  std::vector<DicomVImage*> GetImages(DIModality mod, G4bool bMustExist = true) const;

  void SetImagePositionPatientIsPosVoxelCentre( G4bool pvc ) {
    bImagePositionPatientIsPosVoxelCentre = pvc;
  }
  G4bool GetImagePositionPatientIsPosVoxelCentre() const  {
    return bImagePositionPatientIsPosVoxelCentre;
  }
  
private:
  static DicomMgr* theInstance;

  mmmodi theImages;
  std::vector<DicomVOperator*> theOperators;
  std::multimap<G4int,DicomVOperator*> theOperatorsOrdered;
  G4int theInPolygonNCheck;
  
  G4bool bRotateXY180;

  msldo theLineDrawingOptions;
  G4bool bImagePositionPatientIsPosVoxelCentre;
};

#endif

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

typedef std::pair<G4String,DicomLineDrawingOptions*> psldo;
typedef std::vector<std::pair<G4String,DicomLineDrawingOptions*> > vpsldo;

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

  void SetReflectY( G4bool brot ) {
    bReflectY = brot;
  }
  G4bool GetReflectY() const {
    return bReflectY;
  }
  void SetRotateXY180( G4bool brot ) {
    bRotateXY180 = brot;
  }
  G4bool GetRotateXY180() const {
    return bRotateXY180;
  }

  void ReadLineDrawingOptions( G4String drawLinesFN );  
  vpsldo GetLineDrawingOptions() const {
    return theLineDrawingOptions;
  }
  std::vector<DicomVOperator*> GetOperators() const {
    return theOperators;
  }

public:
  std::vector<DicomVImage*> GetImages(DIModality mod, G4bool bMustExist = true) const;
  std::vector<DicomVImage*> GetImagesVector();
  mmmodi GetImages() const {
    return theImages;
  }
  
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
  
  G4bool bReflectY;
  G4bool bRotateXY180;

  vpsldo theLineDrawingOptions;
  G4bool bImagePositionPatientIsPosVoxelCentre;
};

#endif

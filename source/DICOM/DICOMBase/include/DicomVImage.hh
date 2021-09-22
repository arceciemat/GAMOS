#ifndef DicomVImage__HH
#define DicomVImage__HH

#include "DicomVLine.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
class DicomVLineSet;
class DicomVLineList;
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
typedef std::pair<size_t,G4ThreeVector> pidv; // id is voxel x/y id + 0=line above, 1=line right, 2=line down, 3=line left, 4=diagonal
typedef std::pair<pidv,pidv> ppidv;

enum DIModality{ DIM_CT, DIM_NM, DIM_RTStruct, DIM_RTDose, DIM_G4dcmCT_MateID, DIM_G4dcmCT_MateDens, DIM_G4dcmCT_StructID, DIM_G4dcmNM, DIM_G4dcmDose, DIM_Sqdose, DIM_Sqdose_sq, DIM_Sqdose_err, DIM_Sqdose_errRel, DIM_GammaIndex, DIM_Interfile, DIM_OTHER };
enum DIModalityShort{ DIMS_CT, DIMS_NM, DIMS_Struct, DIMS_Dose, DIMS_CT_MateID, DIMS_CT_MateDens, DIMS_Dose_sq, DIMS_Dose_err, DIMS_Dose_errRel, DIMS_GammaIndex, DIMS_Interfile, DIMS_OTHER };

enum DIOrientation{ DIO_XY, DIO_XZ, DIO_YZ };

class DicomVImage
{ 
public:
  DicomVImage(G4String name, DIModality mod);
  DicomVImage(DicomVImage* dfi, G4String name, DIModality modality, DIOrientation = DIO_XY);
  DicomVImage(const DicomVImage& dfi);
  virtual ~DicomVImage();
  void SetHeader(const DicomVImage* imageNew );

  DicomVImage operator=( const DicomVImage& rhs );
  DicomVImage operator!=( const DicomVImage& rhs );
  void operator+=( const DicomVImage& rhs );
  DicomVImage operator+( const DicomVImage& rhs );
  DicomVImage operator*=( const G4double val );
  // Get and set methods
  size_t GetNoVoxelsX() const { return theNoVoxelsX; };
  size_t GetNoVoxelsY() const { return theNoVoxelsY; };
  size_t GetNoVoxelsZ() const { return theNoVoxelsZ; };
  size_t GetNoVoxelsXY() const { return theNoVoxelsX*theNoVoxelsY; };
  size_t GetNoVoxels() const { return theNoVoxelsX*theNoVoxelsY*theNoVoxelsZ; };
  std::vector<size_t> GetNoVoxelsV();
  
  G4double GetVoxelDimX() const { return (theMaxX-theMinX)/theNoVoxelsX; };
  G4double GetVoxelDimY() const { return (theMaxY-theMinY)/theNoVoxelsY; };
  G4double GetVoxelDimZ() const { return (theMaxZ-theMinZ)/theNoVoxelsZ; };
  std::vector<G4double> GetVoxelDimV();

  G4double GetMinX() const { return theMinX; };
  G4double GetMinY() const { return theMinY; };
  G4double GetMinZ() const { return theMinZ; };
  std::vector<G4double> GetMinV();
  G4double GetMaxX() const { return theMaxX; };
  G4double GetMaxY() const { return theMaxY; };
  G4double GetMaxZ() const { return theMaxZ; };
  std::vector<G4double> GetMaxV();

  G4ThreeVector GetCentre();
  
  void SetNoVoxelsX(const size_t val) {
    theNoVoxelsX = val;
  }
  void SetNoVoxelsY(const size_t val) {
    theNoVoxelsY = val;
  }
  void SetNoVoxelsZ(const size_t val) {
    theNoVoxelsZ = val;
  }
  
  void SetMinX(const G4double val) {
    theMinX = val;
  };
  void SetMaxX(const G4double val) {
    theMaxX = val;
  };
  void SetMinY(const G4double val) {
    theMinY = val;
  };
  void SetMaxY(const G4double val) {
    theMaxY = val;
  };
  void SetMinZ(const G4double val) {
    theMinZ = val;
  };
  void SetMaxZ(const G4double val) {
    theMaxZ = val;
  };

  void SetParamsX( const size_t nvox, const G4double min, const G4double max ) {
    theNoVoxelsX = nvox;
    theMinX = min;
    theMaxX = max;
  }
  void SetParamsY( const size_t nvox, const G4double min, const G4double max ) {
    theNoVoxelsY = nvox;
    theMinY = min;
    theMaxY = max;
  }
  void SetParamsZ( const size_t nvox, const G4double min, const G4double max ) {
    theNoVoxelsZ = nvox;
    theMinZ = min;
    theMaxZ = max;
  }
  
  G4String GetName() const {
    return theName;
  }
  
  std::vector<G4double>* GetData() const {
    return theData;
  }
  G4double GetData(size_t ii) const {
    return theData->at(ii);
  }

  DIModality GetModality() const {
    return theModality;
  }
  G4String GetModalityStr() const;
  static G4String GetModalityStr(DIModality mod);
  DIModalityShort GetModalityShort();
  DIModalityShort GetModalityShort(DIModality mod);
  G4String GetModalityShortStr() const;
  static G4String GetModalityShortStr(DIModality mod);
   
  void SetData( std::vector<G4double>* dat ) {
    if( theData && theData != dat ) theData->clear();
    theData = dat;
  }

  void SetData( size_t id, G4double val ) {
    theData->at(id) = val;
  }

  DIOrientation GetOrientation() const {
    return theOrientation;
  }
  void SetOrientation( DIOrientation ori ) {
    theOrientation = ori;
  }
  G4String GetOrientationName() const;

  G4RotationMatrix* GetRotation() const {
    return theRotation;
  }
  void SetRotation( G4RotationMatrix* rot ) {
    theRotation = rot;
  }

  G4bool IsAutomaticOperate() const {
    return bAutomaticOperate;
  }
  void SetAutomaticOperate(G4bool bAO) {
    bAutomaticOperate = bAO;
  }

  virtual G4bool ReadDataFromTextFile( std::ifstream& fin, G4bool bReadHeader = true );
    
  virtual void DumpHeaderToTextFile(std::ostream& fout);
  virtual void DumpDataToTextFile(std::ostream& fout, G4bool bFloat = false);
  void DumpToSqdoseFile(FILE* fout, DicomVImage* errorImage = 0 );
  void CheckImageHeadersAreEqual( const DicomVImage* img2 );

  void DrawAssocLinesXY( G4bool bCloseFiles = 1 );

  void PrintHeader(std::ostream& out = G4cout );

  G4String GetExtraParam( const G4String& key );
  G4double GetExtraParamNumeric( const G4String& key );
  G4bool ExtraParamExists( const G4String& key );
  void AddExtraParam( G4String key, G4String value );

  virtual G4double GetMaxValue();
  virtual std::pair<G4double,G4ThreeVector> GetMaxValueAndPoint();
  virtual G4double GetValueAtPoint(G4ThreeVector point, G4ThreeVector extension=G4ThreeVector());

  void ClearData();

  G4ThreeVector GetPosition(int ii);
  G4ThreeVector GetPosition(int ix, int iy, int iz);
  
protected:
  virtual void DrawIsodPoints( std::vector<ppidv> pointPairsIsod, DicomVLineList* polyList, G4double isodoseValue, DPOrientation ori );

protected:
  G4String theName;
  DIModality theModality;
  DIOrientation theOrientation;
  
  G4double theMinX;
  G4double theMaxX;
  size_t theNoVoxelsX;
  G4double theMinY;
  G4double theMaxY;
  size_t theNoVoxelsY;
  G4double theMinZ; // minimum extension of voxels (position of wall)
  G4double theMaxZ; // maximum extension of voxels (position of wall)
  size_t theNoVoxelsZ;  // number of voxels in each dimensions

  //  G4double theVoxelDimX;
  //  G4double theVoxelDimY;
  //  G4double theVoxelDimZ; // voxels width

  G4double thePrecision;

  //-- extra parameters
  std::map<G4String,G4String> theExtraParams;
  
public:
  std::vector<G4double>* theData;
  G4RotationMatrix* theRotation;
  G4bool bAutomaticOperate; // if not automatic, Operations have to be invoked explicitly, not by DicomMgr::OperateAll

  //  static size_t theStructureIdxShift;
  G4double GetPrecision() const {
    return thePrecision;
  }
  
  void SetPrecision(G4double prec) {
    thePrecision = prec;
  }
  
  //  static double DP_PRECISION;

  virtual void BuildIsodLinesXY(std::vector<G4double> isodPerCents);
  virtual void BuildIsodLinesXZ(std::vector<G4double> isodPerCents);
  virtual void BuildIsodLinesYZ(std::vector<G4double> isodPerCents);
  virtual DicomVLineSet* GetIsodosesXY () const {
    return theIsodosesXY;
  }
  virtual DicomVLineSet* GetIsodosesXZ () const {
    return theIsodosesXZ;
  }
  virtual DicomVLineSet* GetIsodosesYZ () const {
    return theIsodosesYZ;
  }
  
  virtual G4bool IsDose() const;

private:
  G4bool bIsodosesNormalized;
  DicomVLineSet* theIsodosesXY;
  DicomVLineSet* theIsodosesXZ;
  DicomVLineSet* theIsodosesYZ;
};

std::ostream& operator<<(std::ostream& os, const DicomVImage& );

#endif
  

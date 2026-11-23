#ifndef DicomVReaderImage__HH
#define DicomVReaderImage__HH

#include "DicomVReader.hh"
class DicomMgr;
class DicomReaderMgr;
class DicomUIDInfo;
#include "G4ThreeVector.hh"
#include "DICOM/DICOMBase/include/DicomVImage.hh"

class DicomVReaderImage : public DicomVReader
{ 
public:
  DicomVReaderImage(DRModality mod);
  DicomVReaderImage(DcmDataset* dset, DRModality mod);
  virtual ~DicomVReaderImage();

public:
  void InitialiseTo0();
  virtual void ReadHeaderAndPixels( G4bool bReadPixelData = true );

  void operator+=( const DicomVReaderImage& rhs );
  DicomVReaderImage operator+( const DicomVReaderImage& rhs );
  void AddData( const DicomVReaderImage* rhs );

  // add two slices that have the same dimensions, merging them in Z
  virtual void DICOMCreateImages(){};
  void ResizeImage();
  
  DicomVImage* CreateImage(G4String name, DIModality modality, G4bool bAddVoxelData = true, G4bool bAutoOpr = true);

  DicomVImage* GetImage() {
    return theDicomImage;
  }

 // Get and set methods
  G4String GetFileName() const { return theFileName; }
  size_t GetNoVoxelsX() const { return theNoVoxelsX; }
  size_t GetNoVoxelsY() const { return theNoVoxelsY; }
  size_t GetNoVoxelsZ() const { return theNoVoxelsZ; }
  size_t GetNoVoxelsXY() const { return theNoVoxelsX*theNoVoxelsY; }
  size_t GetNoVoxelsXZ() const { return theNoVoxelsX*theNoVoxelsZ; }
  size_t GetNoVoxelsYZ() const { return theNoVoxelsY*theNoVoxelsZ; }
  size_t GetNoVoxels() const { return theNoVoxelsX*theNoVoxelsY*theNoVoxelsZ; }

  G4double GetVoxelDimX() const { return (theMaxX-theMinX)/theNoVoxelsX; }
  G4double GetVoxelDimY() const { return (theMaxY-theMinY)/theNoVoxelsY; }
  G4double GetVoxelDimZ() const { return (theMaxZ-theMinZ)/theNoVoxelsZ; }

  G4double GetMinX() const { return theMinX; }
  G4double GetMinY() const { return theMinY; }
  G4double GetMinZ() const { return theMinZ; }
  G4double GetMaxX() const { return theMaxX; }
  G4double GetMaxY() const { return theMaxY; }
  G4double GetMaxZ() const { return theMaxZ; }
  
  void SetNoVoxelsX(const size_t& val) { theNoVoxelsX = val; }
  void SetNoVoxelsY(const size_t& val) { theNoVoxelsY = val; }
  void SetNoVoxelsZ(const size_t& val) { theNoVoxelsZ = val; }
  
  void SetMinX(const G4double& val) { theMinX = val; }
  void SetMaxX(const G4double& val) { theMaxX = val; }
  void SetMinY(const G4double& val) { theMinY = val; }
  void SetMaxY(const G4double& val) { theMaxY = val; }
  void SetMinZ(const G4double& val) { theMinZ = val; }
  void SetMaxZ(const G4double& val) { theMaxZ = val; }

  void SetParamsX( const size_t nvox, const G4double min, const G4double max ) {
    theNoVoxelsX = nvox;
    theMinX = min;
    theMaxX = max;
    theVoxelDimX = (max-min)/nvox;
  }
  void SetParamsY( const size_t nvox, const G4double min, const G4double max ) {
    theNoVoxelsY = nvox;
    theMinY = min;
    theMaxY = max;
    theVoxelDimY = (max-min)/nvox;
  }
  void SetParamsZ( const size_t nvox, const G4double min, const G4double max ) {
    theNoVoxelsZ = nvox;
    theMinZ = min;
    theMaxZ = max;
    theVoxelDimZ = (max-min)/nvox;
  }
  
  const G4double& GetLocation() const { return theLocation; }
  
  void SetLocation(const G4double& val) { theLocation = val; }

  G4ThreeVector GetOrientationRows() const { return theOrientationRows; }
  G4ThreeVector GetOrientationColumns() const { return theOrientationColumns; }

  std::vector<G4double>* GetVoxelData() const {
    return theVoxelData;
  }
  G4double GetVoxelData(G4int ii) const {
    return theVoxelData->at(ii);
  }
  void SetVoxelData( std::vector<G4double>* vd ) {
    theVoxelData = vd;
  }

  virtual void ReadPixelData();
  
  void SetCompression( G4int comp ) {
    theCompression = comp;
  }

  G4int GetCompression() const {
    return theCompression;
  }
  G4String GetPatientPosition() const {
    return thePatientPosition;
  }

  DicomUIDInfo* GetUIDInfo() {
    return theUIDInfo;
  }

private:
  template <typename T> inline bool CheckConsistency(const T&, const T&, G4String);

  void CheckIsEquivalent(const DicomVReaderImage *rhs, G4bool bContiguous);

protected:
  void Print( std::ostream& out );
  
protected:
  G4double theLocation;
  G4double theBitAllocated;
  G4double theDoseGridScaling;
  G4double theRescaleSlope;
  G4double theRescaleIntercept;

  size_t theNoVoxelsX, theNoVoxelsY, theNoVoxelsZ;  // number of voxels in each dimensions
  G4double theMinX,theMinY,theMinZ; // minimum extension of voxels (position of wall)
  G4double theMaxX,theMaxY,theMaxZ; // maximum extension of voxels (position of wall)
  G4double theVoxelDimX, theVoxelDimY, theVoxelDimZ;

  G4ThreeVector theOrientationRows;
  G4ThreeVector theOrientationColumns;
  
  std::vector<G4double>* theVoxelData; // not int, also used for NM and RTDose

  G4String theFileName;
  DicomMgr* theDicomMgr;
  DicomVImage* theDicomImage;
  DicomReaderMgr* theReaderMgr;

  G4String thePatientPosition; // orientation : HFS/FFS/..
  G4int theCompression;

  DicomUIDInfo* theUIDInfo;

  G4bool bZDirectionInvert;
};

//============================================================================
template <typename T>
inline bool DicomVReaderImage::CheckConsistency(const T& val1, const T& val2, 
                                                       G4String category) {
  if(val1 != val2) {
    G4Exception("DicomVReaderImager::CheckConsistency", 
                "Consistency Mismatch : Keeping previous value if nonzero",
                JustWarning, category.c_str());
        return false;
  }
  return true;
}

#endif
  

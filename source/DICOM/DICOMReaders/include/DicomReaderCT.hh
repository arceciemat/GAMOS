#ifndef DicomReaderCT__HH
#define DicomReaderCT__HH
#include "DicomVReaderImage.hh"
class DcmDataSet;
class DicomPolygonSet;
class DicomVImage;

class DicomReaderCT : public DicomVReaderImage
{ 
public:
  DicomReaderCT(DcmDataset* dset);
  DicomReaderCT(G4String fileName );
  DicomReaderCT();
  ~DicomReaderCT(){};

public:
  virtual void DICOMCreateImages();
  void RecalculateMateIDFromMateDens();

  static void AddMaterial( std::vector<G4String> data );
  static void AddMaterialDensity( std::vector<G4String> data );
  static void AddCT2Density( std::vector<G4String> data );
  static G4double Hounsfield2density(G4int Hval);
  static size_t GetMaterialIndex( G4double density );
  static size_t GetMaterialIndexByDensity( G4double density );
  DicomVImage* GetMateIDImage() const {
    return theMateIDImage;
  }
  DicomVImage* GetMateDensImage() const {
    return theMateDensImage;
  }
  G4bool IsOnlyHU() const {
    return bOnlyHU;
  }
  void SetOnlyHU( G4bool bohu ) {
    bOnlyHU = bohu;
  }
  
#ifdef WIN32  //WINDEB       
	#if defined DICOMReaders_ALLOC_EXPORT
		G4DLLEXPORT G4bool IsMaterialDensities();
		G4DLLEXPORT std::map<G4double, G4String> GetMaterials();
		G4DLLEXPORT std::map<G4int, G4double> GetCT2Density();
		G4DLLEXPORT std::map<G4double, G4String> GetMaterialDensities();
	#else
		G4DLLIMPORT G4bool IsMaterialDensities();
		G4DLLIMPORT std::map<G4double, G4String> GetMaterials();
		G4DLLIMPORT std::map<G4int, G4double> GetCT2Density();
		G4DLLIMPORT std::map<G4double, G4String> GetMaterialDensities();
	#endif
#else
	static G4bool IsMaterialDensities();
	static std::map<G4double, G4String> GetMaterials();
	static std::map<G4int, G4double> GetCT2Density();
	static std::map<G4double, G4String> GetMaterialDensities();
#endif

private:

	static G4bool bMaterialDensities;
  static std::map<G4double,G4String> theMaterials;
  static std::map<G4double,G4String> theMaterialDensities;
  static std::map<G4int,G4double> theCT2Density;

  DicomVImage* theMateIDImage;
  DicomVImage* theMateDensImage;
  G4bool bOnlyHU;
};

#endif
  

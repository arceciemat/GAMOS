#ifndef DicomReaderMgr__HH
#define DicomReaderMgr__HH

#include "DicomVReaderImage.hh"
#include <vector>
#include <map>
#include "globals.hh"
#include "GamosCore/GamosUtils/include/GmVerbosity.hh"

class DicomVReaderImage;
class DicomReaderCT;
class DicomReaderNM;
class DicomReaderRTDose;
class DicomVImage;
class DicomMgr;
class DicomParameterMgr;
class DicomReaderG4dcmCT; //GDEB

typedef std::multimap<DRModality,DicomVReader*> mmmdr;
typedef std::vector<DicomVReaderImage*> vdri;
typedef std::map<G4double,DicomVReaderImage*> mdri;
typedef std::multimap<DRModality,DicomVReaderImage*> mmmdri;

class DicomReaderMgr 
{
public:
  static DicomReaderMgr* GetInstance();
  ~DicomReaderMgr(){};

private:
  DicomReaderMgr();

public:
  void ProcessData();
  void CreateReaders();
  void CreateImages();
  void CreateImagesCT( DicomReaderCT* imgMergedReaderCT );
  
  //  std::vector<DicomVReader*> GetReaders(G4String name, G4bool bMustExist = true) const;
  //  std::vector<DicomVReaderImage*> GetImageReaders(G4String name, G4bool bMustExist = true) const;
  std::vector<DicomVReader*> GetReaders(DRModality mod, G4bool bMustExist = true) const;
  std::vector<DicomVReaderImage*> GetImageReaders(DRModality mod, G4bool bMustExist = true) const;
  mmmdri GetImageReaders() const {
    return theImageReaders;
  }
  std::vector<DicomVImage*> GetImages(G4bool bAll = false) const;

  void CheckSlices(mdri files);
  DicomVReaderImage* MergeSlices(mdri files, DRModality modality);
  DicomReaderCT* MergeSlicesCT(mdri files);
  DicomReaderNM* MergeSlicesNM(mdri files);
  DicomReaderRTDose* MergeRTDose(vdri files);

  void AddReader(DicomVReader* reader, DRModality mod);
  void AddImageReader(DicomVReaderImage* imgReader, DRModality mod);

  void DeleteReader(DicomVReader* reader);
  void DeleteImageReader(DicomVReaderImage* reader);

  size_t GetNofImageReaders(DRModality mod);
  size_t GetNofReaders(DRModality mod);
  size_t GetNofReaders();
  std::map<G4int,G4String> GetStructIDsNames();

  void SetCTOnlyHU( G4bool bOnlyHU );

  void SetMergeRTReaders( G4bool bm ) {
    bMergeRTReaders = bm;
  }
  
private:
  static DicomReaderMgr* theInstance;
  DicomMgr* theDicomMgr;
  DicomParameterMgr* theParamMgr;
  
  mmmdr theReaders;
  mmmdri theImageReaders;

  G4bool bMergeRTReaders;
};

#endif

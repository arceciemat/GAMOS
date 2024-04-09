#include "DicomReaderCT.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomPolygon.hh"
#include "DICOM/DICOMBase/include/DicomPolygonList.hh"
#include "DICOM/DICOMBase/include/DicomPolygonSet.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosReadDICOM/include/GmReadPhantomStMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4ThreeVector.hh"
#include "G4UIcommand.hh"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcpixel.h"
#include "dcmtk/dcmdata/dcpxitem.h"
#include "dcmtk/dcmdata/dcpixseq.h"
#include "dcmtk/config/osconfig.h"   // make sure OS specific configuration is included 
#include "dcmtk/dcmrt/drtdose.h"    // DRTDoseIOD
using namespace std;

#ifdef WIN32	
#if defined DICOMReaders_ALLOC_EXPORT 
G4bool DicomReaderCT::bMaterialDensities = false;
std::map<G4double, G4String> DicomReaderCT::theMaterials;
std::map<G4int, G4double> DicomReaderCT::theCT2Density;
std::map<G4double, G4String> DicomReaderCT::theMaterialDensities;
#endif
#else
G4bool DicomReaderCT::bMaterialDensities = false;
std::map<G4double, G4String> DicomReaderCT::theMaterials;
std::map<G4int, G4double> DicomReaderCT::theCT2Density;
std::map<G4double, G4String> DicomReaderCT::theMaterialDensities;
#endif
 
#include "CLHEP/Random/RandFlat.h"

//-----------------------------------------------------------------------------
DicomReaderCT::DicomReaderCT(DcmDataset* dset) : DicomVReaderImage(dset, DRM_CT)
{
  if( DicomVerb(debugVerb) ) G4cout << this <<"DicomReaderCT::DicomReaderCT(DcmDataset " << dset << G4endl; 
  theMateIDImage = 0;
  theMateDensImage = 0;
  
  ReadHeaderAndPixels();
  bOnlyHU = false;
}

//-----------------------------------------------------------------------------
DicomReaderCT::DicomReaderCT(G4String fName ) : DicomVReaderImage(DRM_CT)
{
  theFileName = fName;
  theMateIDImage = 0;
  theMateDensImage = 0;

  if( DicomVerb(debugVerb) ) G4cout << "DicomReaderCT::DicomReaderCT fileName " << fName << G4endl; 
  DcmFileFormat dfile;
  if( ! (dfile.loadFile(fName.c_str())).good() ) {
    G4Exception("DicomReaderCT::ReadFile",
		"",
		FatalErrorInArgument,
		("Error reading file " + fName).c_str());
  }
  DcmDataset* dset = dfile.getDataset();
  theDataset = dset;

  ReadHeaderAndPixels();

  bOnlyHU = false;
}

//-----------------------------------------------------------------------------
DicomReaderCT::DicomReaderCT() : DicomVReaderImage(DRM_CT)
{
  theMateIDImage = 0;
  theMateDensImage = 0;

  InitialiseTo0();

  bOnlyHU = false;
}

//-----------------------------------------------------------------------------
void DicomReaderCT::DICOMCreateImages()
{
  theDicomImage = CreateImage("CT_HU", DIM_CT, true, false );

  if( bOnlyHU ) return;
  
  theMateIDImage = CreateImage("CT_MateID", DIM_G4dcmCT_MateID, false, false );
  G4cout << this << " DicomReaderCT::DICOMCreateImages " << theMateIDImage << G4endl; 
  
  std::vector<G4double>* imageMateIDData = theMateIDImage->GetData();
  std::vector<G4double>* imageHUData = theDicomImage->GetData();
 
  theMateDensImage = CreateImage("CT_MateDens", DIM_G4dcmCT_MateDens, false, false );
  std::vector<G4double>* imageMateDensData = theMateDensImage->GetData();

  size_t copyNo = 0;
  size_t copyNoC = 0;
  double meanHU = 0.;
  size_t nVoxelXY = theNoVoxelsX*theNoVoxelsY;
  for( size_t iz = 0; iz < theNoVoxelsZ; iz ++ ) {
    for( size_t ir = 0; ir < theNoVoxelsY; ir += theCompression ) {
      for( size_t ic = 0; ic < theNoVoxelsX; ic += theCompression ) {
	//	size_t copyNoC = ic+ir*theNoVoxelsXC+iz*theNoVoxelsXYC;
	if( DicomVerb(testVerb) )  G4cout << "iz " << iz << " ir " << ir << " ic " << ic << G4endl; //GDEB
	meanHU = 0.;
	int isumrMax = min(ir+theCompression,theNoVoxelsY);
	int isumcMax = min(ic+theCompression,theNoVoxelsX);
	for( int isumr = ir; isumr < isumrMax; isumr ++ ) {
	  for( int isumc = ic; isumc < isumcMax; isumc ++ ) {
	    copyNo = isumc+isumr*theNoVoxelsX+iz*nVoxelXY;
	    meanHU += imageHUData->at(copyNo);
	    if( DicomVerb(testVerb) )  G4cout << " COPYNO= " << copyNo << " HU " << imageHUData->at(copyNo) << " meanHU " << meanHU << G4endl; 
	    //      G4cout << isumr << " " << isumc << " GET mean " << meanHU << G4endl;
	  }
	}
	meanHU /= (isumrMax-ir)*(isumcMax-ic);
	G4double meanDens = Hounsfield2density(std::round(meanHU));
	imageMateDensData->at(copyNoC) = meanDens;	
	if( DicomVerb(testVerb) ) G4cout << copyNo << " " << iz << ":" << ir << ":" << ic << " FINAL meanHU " << meanHU << " " << meanDens << G4endl; 
	size_t mateID = GetMaterialIndex(meanDens);
	imageMateIDData->at(copyNoC++) = mateID;	
      }	
    }
  }

  //-  G4cout << " MATEIDIMAGE2 " << theMateIDImage->IsAutomaticOperate() << G4endl; //GDEB
  DicomVOperator* operResize = DicomMgr::GetInstance()->FindOperator("DicomOperResize");
  if( operResize ) {
    /*    GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
        if( paramMgr->IsParameterInScript("nVoxX") || paramMgr->IsParameterInScript("nVoxY") ) {
      G4Exception(" DicomReaderCT::DICOMCreateImages",
		  "",
		  FatalException,
		  "Cannot change nVoxX / nVoxY, because Hounsfield numbers have to be recalculated: use :COMPRESSION in metadata file instead");
		  }*/
  }
  
}

//-----------------------------------------------------------------------------
void DicomReaderCT::AddMaterial( std::vector<G4String> wl )
{
  if( theMaterials.size() > 0 && bMaterialDensities ) {
    G4Exception("DicomReaderCT::AddMaterial",
		"",
		FatalException,
		"Trying to add a Material with :MATE and another with :MATE_DENS, check your input file");
  }
  bMaterialDensities = false;
  theMaterials[G4UIcommand::ConvertToDouble(wl[2])] = wl[1];
  
}

//-----------------------------------------------------------------------------
void DicomReaderCT::AddMaterialDensity( std::vector<G4String> wl )
{
  if( theMaterialDensities.size() > 0 && !bMaterialDensities ) {
    G4Exception("DicomReaderCT::AddMaterial",
		"",
		FatalException,
		"Trying to add a Material with :MATE and another with :MATE_DENS, check your input file");
  }
  bMaterialDensities = true;
  theMaterialDensities[G4UIcommand::ConvertToDouble(wl[2])] = wl[1];
}


//-----------------------------------------------------------------------------
void DicomReaderCT::AddCT2Density( std::vector<G4String> wl)
{
  theCT2Density[GmGenUtils::GetInt(wl[1])] = GmGenUtils::GetValue(wl[2]);
  if( DicomVerb(warningVerb) ) G4cout << " AddCT2density " << theCT2Density.size() << " : " << GmGenUtils::GetInt(wl[1]) << " = " << GmGenUtils::GetValue(wl[2]) <<G4endl;

}
 
//-----------------------------------------------------------------------------
G4double DicomReaderCT::Hounsfield2density(G4int Hval)
{
  if( theCT2Density.size() == 0 ) {
    G4Exception("DicomReaderCT::Hounsfield2density",
		"",
		FatalException,
		"No :CT2D line in input file");
  }
  std::map<G4int,G4double>::const_iterator ite = theCT2Density.begin();
  G4int minHval = (*ite).first;
  if( Hval < minHval ) {
    G4Exception("DicomReaderCT::Hounsfield2density",
		"",
		FatalException,
		("Hval value too small, change input file "+std::to_string(G4int(Hval)) + " < " + std::to_string(minHval)).c_str());
  }

  ite = theCT2Density.end(); ite--;
  G4int maxHval = (*ite).first;
  if( G4int(Hval) > maxHval ) {
    G4Exception("DicomReaderCT::Hounsfield2density",
		"",
		FatalException,
		("Hval value too big, change CT2Density.dat file "+std::to_string(G4int(Hval)) + " > " + std::to_string(maxHval)).c_str());
  }
  
  G4float density = -1.;
  G4double deltaCT = 0;
  G4double deltaDensity = 0;
  
  ite = theCT2Density.upper_bound(Hval);
  std::map<G4int,G4double>::const_iterator itePrev = ite; itePrev--;
  
  deltaCT = (*ite).first - (*itePrev).first;
  if( DicomVerb(testVerb) ) G4cout << " CT2D deltaCT " << deltaCT << " " <<  (*ite).first<< " - " << (*itePrev).first << G4endl;
  deltaDensity = (*ite).second - (*itePrev).second;
  if( DicomVerb(testVerb) ) G4cout << " CT2D deltaDensity " <<deltaDensity  << " " <<  (*ite).second<< " - " << (*itePrev).second << G4endl;
  
  // interpolating linearly
  density = (*ite).second - (((*ite).first-Hval)*deltaDensity/deltaCT );
  if( DicomVerb(testVerb) ) G4cout << " density= " << density << " = " << (*ite).second << " - (( " << (*ite).first << " - " << Hval << " )* " << deltaDensity << " / " << deltaCT << G4endl;

  if(density < 0.) {
    G4Exception("DicomReaderCT::Hounsfield2Density",
		"",
		FatalException,
		G4String("@@@ Error negative density = " + std::to_string(density) + " from HV = " + std::to_string(G4int(Hval))).c_str());
  }

  if( DicomVerb(testVerb) ) G4cout << " Hval2density " << Hval << " -> " << density << G4endl;
  
  return density;
  
}

//-----------------------------------------------------------------------------
size_t DicomReaderCT::GetMaterialIndex( G4double Hval)
{
  std::map<G4double,G4String>::iterator ite = theMaterials.upper_bound(Hval);
  if( ite == theMaterials.end() ) {
    ite--;
    G4Exception("DicomReaderCT::GetMaterialIndex",
		"",
		FatalException,
		("Hounsfield value too big, change input file "+std::to_string(Hval) + " > " + std::to_string((*ite).first)).c_str());
  }

  size_t dist = std::distance( theMaterials.begin(), ite );
  
  return  dist;
  
}


//-----------------------------------------------------------------------------
size_t DicomReaderCT::GetMaterialIndexByDensity( G4double density )
{
  std::map<G4double,G4String>::iterator ite = theMaterialDensities.upper_bound(density);
  if( ite == theMaterialDensities.end() ) {
    ite--;
    G4Exception("DicomReaderCT::GetMaterialIndexByDensity",
		"",
		FatalException,
		("Density too big, change input file "+std::to_string(density) + " > " + std::to_string((*ite).first)).c_str());
  }

  size_t dist = std::distance( theMaterialDensities.begin(), ite );
  
  return  dist;
  
}

//-----------------------------------------------------------------------------
void DicomReaderCT::RecalculateMateIDFromMateDens()
{
  size_t nVoxelX = theMateDensImage->GetNoVoxelsX();
  size_t nVoxelY = theMateDensImage->GetNoVoxelsY();
  size_t nVoxelZ = theMateDensImage->GetNoVoxelsZ();
  theMateIDImage->SetHeader(theMateDensImage);
  std::vector<G4double>* imageMateIDData = theMateIDImage->GetData();
  std::vector<G4double>* imageMateDensData = theMateDensImage->GetData();

  if( DicomVerb(infoVerb) ) G4cout << this << " DicomReaderCT::RecalculateMateIDFromMateDens " << G4endl; 
  size_t copyNo = 0;
  for( size_t iz = 0; iz < nVoxelZ; iz ++ ) {
    for( size_t ir = 0; ir < nVoxelY; ir ++ ) {
      for( size_t ic = 0; ic < nVoxelX; ic ++ ) {
	if( DicomVerb(testVerb) )  G4cout << "iz " << iz << " ir " << ir << " ic " << ic << G4endl; 
	G4double meanDens = imageMateDensData->at(copyNo);
	//      G4cout << ir << " " << ic << " FINAL mean " << meanDens << G4endl;
	size_t mateID = GetMaterialIndex(meanDens);
	imageMateIDData->at(copyNo++) = mateID;	
      }	
    }
  }

  //-  G4cout << " MATEIDIMAGE2 " << theMateIDImage->IsAutomaticOperate() << G4endl; //GDEB
  
}

//-----------------------------------------------------------------------------
std::map<G4double, G4String> DicomReaderCT::GetMaterials() 
{
  return theMaterials;
}

//-----------------------------------------------------------------------------
G4bool DicomReaderCT::IsMaterialDensities()
{
  return bMaterialDensities;
}

//-----------------------------------------------------------------------------
std::map<G4double, G4String> DicomReaderCT::GetMaterialDensities()
{
  return theMaterialDensities;
}

//-----------------------------------------------------------------------------
std::map<G4int, G4double> DicomReaderCT::GetCT2Density()
{
  return theCT2Density;
}

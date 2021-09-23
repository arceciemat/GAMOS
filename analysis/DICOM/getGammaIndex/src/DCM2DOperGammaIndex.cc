#include "DCM2DOperGammaIndex.hh"
#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomPolygonSet.hh"

#include "DICOM/DICOMBase/include/DicomDrawerROOT.hh" 
#include "DICOM/DICOMBase/include/DicomMgr.hh" 

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "ROOTUtilities/GetValue.C"
#include "ROOTUtilities/GenUtils.C"
#include "ROOTUtilities/PlotData/PlotUtils.C"


//------------------------------------------------------------------------
DCM2DOperGammaIndex::DCM2DOperGammaIndex( G4int , G4String )
{
  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  bMaxGammaValue = false;
  theMaxGammaValue = 100; 
  theMaxGammaValue = theParamMgr->GetNumericValue("maxGammaValue",theMaxGammaValue);
  if( theMaxGammaValue != DBL_MAX ) bMaxGammaValue = 1;
  
  MAXDISTVOXEL = theParamMgr->GetNumericValue("maxDistVoxel",theMaxGammaValue);

  if ( theMaxGammaValue < MAXDISTVOXEL ) {
    G4Exception("GetGammaIndex",
		"",
		JustWarning,
		"maxValue smaller than maxDistVoxel, setting both equal");
    MAXDISTVOXEL = G4int(theMaxGammaValue);
  }

  bMaxAtPoint = false;
  theMaxAtPointFactor = 1.;

  theDrawer = new DicomDrawerROOT();

  bHisto1D = G4bool(theParamMgr->GetNumericValue("bHisto1D",1));

  bErrors = false;
  theNErrorSigmas = 0.; 

}

//------------------------------------------------------------------------
void DCM2DOperGammaIndex::OperateXY( DicomVImage* image1, DicomVImage* image2, DicomVImage* imageErr1, DicomVImage* imageErr2, double perCentLim, double minDistLim, double pval, DicomVImage* imageOut )
{
  G4bool bDistanceByVoxel = false;//  TODO

  G4bool bInterpolateVoxels = G4bool(theParamMgr->GetNumericValue("bInterpolateVoxels",0));
  //  if( bInterpolateVoxels && *image1 == *image2 ) { // if images have same dimensions, no interpolation is needed
  //  bInterpolateVoxels = false; 
  // }
  
  //----- Extract image1 data
  G4int nVoxX1 = image1->GetNoVoxelsX();
  G4int nVoxY1 = image1->GetNoVoxelsY();
  G4int nVoxXY1 = nVoxX1*nVoxY1;
  G4int nVoxZ1 = image1->GetNoVoxelsZ();
  G4int nVox1 = nVoxX1*nVoxY1*nVoxZ1;
  G4double fMinX1 = image1->GetMinX();
  G4double fMinY1 = image1->GetMinY();
  G4double fMinZ1 = image1->GetMinZ();
  G4double fMaxX1 = image1->GetMaxX();
  G4double fMaxY1 = image1->GetMaxY();
  G4double fMaxZ1 = image1->GetMaxZ();
  G4double fVoxDimX1 = image1->GetVoxelDimX();
  G4double fVoxDimY1 = image1->GetVoxelDimY();
  G4double fVoxDimZ1 = image1->GetVoxelDimZ();
  std::vector<G4double> iData1 = *(image1->GetData());
  //-  DicomVImage* imageOut = new DicomVImage(image1,"GIImageXY_"+image1->GetModalityShort(),DIM_GammaIndex);
  std::vector<G4double>* iDataOut = imageOut->GetData();
  std::vector<G4double> iDataErr1;
  if( imageErr1 != 0 ) iDataErr1 = *(imageErr1->GetData());

  //  std::vector<G4double>* iDataOut = new std::vector<G4double>(nVoxXY1*nVoxZ1);
  //  G4cout << " START iDataOut " << iDataOut << G4endl; //GDEB
  TH1F* histoGI;
  double theHGILim;
  TCanvas* canvas;
  if( bHisto1D ) {
    //  TH2F* histoXSvsGI;
    theHGILim = std::min(100.,double(MAXDISTVOXEL));
    canvas = new TCanvas();
    gStyle->SetOptStat(1111111);
    canvas->Draw();
  }
    //  std::map<G4double,TH2F*> theHistosXSvsGI;
    
  /*  if( bImageEqual ) {
      DicomVImage* image2Eq;
      if( !theResizeOperator ) theResizeOperator = new DicomOperResize(1,"GIOperResize");
      theResizeOperator->Operate(image2Eq, nVoxX1, fMinX1, fMaxX1, nVoxY1, fMinY1, fMaxY1, nVoxZ1, fMinZ1, fMaxZ1);  // make image2 as image1
      }*/
  
  if( DicomVerb(debugVerb) ) G4cout << "@@@ START DCM2DOperGammaIndex " << perCentLim << " " << minDistLim << " " << pval << G4endl;
  
  /*  DicomVImage* imageErr1 = image1->GetErrorData();
      std::vector<DIfloat>* iDataErr1 = 0;
      if( imageErr1 ) iDataErr1 = imageErr1->GetData();
  */  
  if( DicomVerb(infoVerb) )
    std::cout << "@@@ READ image nVoxX/Y/Z " << nVoxX1 << " " << nVoxY1 << " " << nVoxZ1 << G4endl
	      << " fMinX/Y/Z " << fMinX1 << " " << fMinY1 << " " << fMinZ1 << G4endl
	      << " fMaxX/Y/Z " << fMaxX1 << " " << fMaxY1 << " " << fMaxZ1 << G4endl
	      << " fVoxDimX/Y/Z " << fVoxDimX1 << " " << fVoxDimY1 << " " << fVoxDimZ1 << G4endl
	      << " NDOSES " << nVox1 << G4endl;
  
  //----- Extract image2 data
  G4int nVoxX2 = image2->GetNoVoxelsX();
  G4int nVoxY2 = image2->GetNoVoxelsY();
  G4int nVoxZ2 = image2->GetNoVoxelsZ();
  G4int nVoxXY2 = nVoxX2*nVoxY2;
  G4int nVox2 = nVoxX2*nVoxY2*nVoxZ2;
  //  size_t nVoxZ2 = image2->GetNoVoxelsZ();
  G4double fMinX2 = image2->GetMinX();
  G4double fMinY2 = image2->GetMinY();
  G4double fMinZ2 = image2->GetMinZ();
  G4double fMaxX2 = image2->GetMaxX();
  G4double fMaxY2 = image2->GetMaxY();
  G4double fMaxZ2 = image2->GetMaxZ();
  G4double fVoxDimX2 = image2->GetVoxelDimX();
  G4double fVoxDimY2 = image2->GetVoxelDimY();
  G4double fVoxDimZ2 = image2->GetVoxelDimZ();
  std::vector<G4double> iData2 = *(image2->GetData());
  std::vector<G4double> iDataErr2;
  if( imageErr2 != 0 ) iDataErr2 = *(imageErr2->GetData());

  if( DicomVerb(infoVerb) )
    std::cout << "@@@ READ sqdose2 nVoxX/Y/Z " << nVoxX2 << " " << nVoxY2 << " " << nVoxZ2 << G4endl
	      << " fMinX/Y/Z " << fMinX2 << " " << fMinY2 << " " << fMinZ2 << G4endl
	      << " fMaxX/Y/Z " << fMaxX2 << " " << fMaxY2 << " " << fMaxZ2 << G4endl
	      << " fVoxDimX/Y/Z " << fVoxDimX2 << " " << fVoxDimY2 << " " << fVoxDimZ2 << G4endl
	      << " NDOSES " << nVox2 << G4endl;

  //----- Start calculation
  G4int nGI = 0;
  G4int nPOINTS = 0;
  std::multiset<double> theGammaFinal; 
  G4double fVoxX1X2 = (std::min(fMaxX1,fMaxX2)-std::max(fMinX1,fMinX2));
  G4int nVoxX1X2 =  GmGenUtils::GetBelowInt(fVoxX1X2/fVoxDimX1); 
  G4double hfVoxDimX1X2 = nVoxX1X2*fVoxDimX1;
  G4double diffhfVoxDimX1X2 = hfVoxDimX1X2 - fVoxX1X2;
  if( DicomVerb(testVerb) ) G4cout << " fVoxX1X2 " << fVoxX1X2 << " nVoxX1X2 " << nVoxX1X2 << " hfVoxDimX1X2 " <<hfVoxDimX1X2 << " diffhfVoxDimX1X2 " <<  diffhfVoxDimX1X2 << G4endl;
  G4double fMinX1X2 = std::max(fMinX1,fMinX2)-diffhfVoxDimX1X2/2.;
  G4double fMaxX1X2 = std::min(fMaxX1,fMaxX2)+diffhfVoxDimX1X2/2.;
  G4double fVoxDimX1X2 = (fMaxX1X2-fMinX1X2)/nVoxX1X2;
  fMinX1X2 = std::max(fMinX1X2, GetMinAtVoxel(fMinX1, fMinX1X2, fVoxDimX1X2) );
  fMaxX1X2 = std::min(fMaxX1X2, GetMaxAtVoxel(fMaxX1, fMaxX1X2, nVoxX1X2, fVoxDimX1X2) );
  nVoxX1X2 = GmGenUtils::GetBelowInt((fMaxX1X2-fMinX1X2)/fVoxDimX1X2); 

  G4double fVoxY1Y2 = (std::min(fMaxY1,fMaxY2)-std::max(fMinY1,fMinY2));
  G4int nVoxY1Y2 =  GmGenUtils::GetBelowInt(fVoxY1Y2/fVoxDimY1); 
  G4double hfVoxDimY1Y2 = nVoxY1Y2*fVoxDimY1;
  G4double diffhfVoxDimY1Y2 = hfVoxDimY1Y2 - fVoxY1Y2;
  if( DicomVerb(testVerb) ) G4cout << " fVoxY1Y2 " << fVoxY1Y2 << " nVoxY1Y2 " << nVoxY1Y2 << " hfVoxDimY1Y2 " <<hfVoxDimY1Y2 << " diffhfVoxDimY1Y2 " <<  diffhfVoxDimY1Y2 << G4endl;
  G4double fMinY1Y2 = std::max(fMinY1,fMinY2)-diffhfVoxDimY1Y2/2.;
  G4double fMaxY1Y2 = std::min(fMaxY1,fMaxY2)+diffhfVoxDimY1Y2/2.;
  G4double fVoxDimY1Y2 = (fMaxY1Y2-fMinY1Y2)/nVoxY1Y2;
  fMinY1Y2 = std::max(fMinY1Y2, GetMinAtVoxel(fMinY1, fMinY1Y2, fVoxDimY1Y2) );
  fMaxY1Y2 = std::min(fMaxY1Y2, GetMaxAtVoxel(fMaxY1, fMaxY1Y2, nVoxY1Y2, fVoxDimY1Y2) );
  nVoxY1Y2 = GmGenUtils::GetBelowInt((fMaxY1Y2-fMinY1Y2)/fVoxDimY1Y2); 
  G4int ptExtN = G4int(theParamMgr->GetNumericValue("pointExtensionN",0));
  G4ThreeVector pointExtension(ptExtN*image1->GetVoxelDimX(),ptExtN*image1->GetVoxelDimY(),ptExtN*image1->GetVoxelDimZ());

  //----- Histogram with all Z planes
  std::string limstrALL = "GammaIndex_XY_"+GmGenUtils::ftoa(perCentLim)+"_"+GmGenUtils::ftoa(minDistLim)+"_1D_ALL";
  TH1F* histoGIALL;
  if( bHisto1D ) {
    histoGIALL = new TH1F((limstrALL).c_str(),(limstrALL).c_str(),std::max(100,int(theHGILim*10)),0.,theHGILim);
  }

  float maxDosePC = 0;
  if( thePerCentType == GIPCT_ImageMax ) {
    maxDosePC = image1->GetMaxValue();
    if( DicomVerb(debugVerb) ) G4cout << thePerCentType << " maxDosePC = maxDose1 " << maxDosePC << G4endl;
  } else if( thePerCentType == GIPCT_AtPoint ){
    maxDosePC = image1->GetValueAtPoint(thePointPerCent,pointExtension);
    if( DicomVerb(debugVerb) ) G4cout << thePerCentType << " maxDosePC = theMaxAtPoint1st " << maxDosePC << G4endl;
    if( maxDosePC == 0. ) {
      G4Exception("DCM2DOperGammaIndex",
		  "",
		  JustWarning,
		  ("MAX DOSE IS 0., AT POINT ("+GmGenUtils::ftoa(thePointPerCent.x())+","+GmGenUtils::ftoa(thePointPerCent.x())+","+GmGenUtils::ftoa(thePointPerCent.x())+")    REPLACE IT WITH MAXIMUM DOSE").c_str());
      maxDosePC = image1->GetMaxValue();
    }
  } else { // GIPCT_Max2Images
    maxDosePC = std::max(image1->GetMaxValue(),image2->GetMaxValue());
    if( DicomVerb(debugVerb) ) G4cout << thePerCentType << " maxDosePC = max(maxDose1,maxDose2) " << maxDosePC << G4endl;
  }
    
  //----- Loop to each Z plane
  for( G4int nvz1 = 0; nvz1 < nVoxZ1; nvz1++ ){
    if( DicomVerb(infoVerb) ) G4cout << " Z PLANE " << nvz1 << G4endl; 
    G4double zMin = fMinZ1 + nvz1*fVoxDimZ1;
    G4double zMax = zMin + fVoxDimZ1;
    if( zMin < fMinZ1 || zMax > fMaxZ1 ) continue;
    if( zMin < fMinZ2 || zMax > fMaxZ2 ) continue;
    
    G4String zstr = GmGenUtils::ftoa(zMin+fVoxDimZ1/2.);
    std::string limstr = "GammaIndex_XY_"+GmGenUtils::ftoa(perCentLim)+"_"+GmGenUtils::ftoa(minDistLim)+"_1D_"+zstr;

    if( bHisto1D ) {
      histoGI = new TH1F((limstr).c_str(),(limstr).c_str(),std::max(100,int(theHGILim*10)),0.,theHGILim);
    }
    //    histoXSvsGI = new TH2F(limstr.c_str(),limstr.c_str(),nVoxX1, fMinX1, fMinX1+nVoxX1*widthX1, nVoxY1, fMinY1, fMinY1+nVoxY1*widthY1);
    //    histoXSvsGI = new TH2F(limstr.c_str(),limstr.c_str(),nVoxX1X2, std::max(fMinX1,fMinX2), std::min(fMaxX1,fMaxX2), nVoxY1Y2,  std::max(fMinY1,fMinY2), std::min(fMaxY1,fMaxY2));
    //    histoXSvsGI = new TH2F((limstr+"_2D").c_str(),(limstr+"_2D").c_str(),nVoxX1X2, fMinX1X2, fMaxX1X2, nVoxY1Y2, fMinY1Y2, fMaxY1Y2);
    
    float doseErrAtNv1 = 0;
    float doseErrAtNv2 = 0;
    
    //----- Loop in XY plane
    /*    for( G4int nv1 = 0; nv1 < nVox1; nv1++ ){
	  G4double dataAtNv1 = iData1[nv1];
	  G4cout << nv1 << " TESTdata " << dataAtNv1 << " data2 " << iData2[nv1] << G4endl; //GDEB
	  } */
	
    for( G4int nvxy1 = 0; nvxy1 < nVoxXY1; nvxy1++ ){
      G4int nv1 = nvz1*nVoxXY1+nvxy1; 
      G4double dataAtNv1 = iData1[nv1];
      //      G4double dataOutAtNv1 = iDataOut[nv1];
      G4int nvx1 = nv1%nVoxX1;
      G4double xMin = fMinX1 + nvx1*fVoxDimX1;     
      G4double xMax = xMin + fVoxDimX1;
      if( xMin < fMinX1 || xMax > fMaxX1 ) continue;
      if( xMin < fMinX2 || xMax > fMaxX2 ) continue;
      G4int nvy1 = (nv1/nVoxX1)%nVoxY1;
      G4double yMin = fMinY1 + nvy1*fVoxDimY1;
      G4double yMax = yMin + fVoxDimY1;
      if( yMin < fMinY1 || yMax > fMaxY1 ) continue;
      if( yMin < fMinY2 || yMax > fMaxY2 ) continue;      
      //    if( nz1 == 2 ) break;//GDEB
        //      G4int copyNo1GI = nvx1 + nvy1*nVoxX1 + nvz1*nVoxXY1; //--- voxel ID
          
      G4ThreeVector voxelCentre1( fMinX1+(nvx1+0.5)*fVoxDimX1, fMinY1+(nvy1+0.5)*fVoxDimY1, fMinZ1+(nvz1+0.5)*fVoxDimZ1 );
      
      if( DicomVerb(debugVerb) ) G4cout << "@@@ LOOPING VOXEL " << nv1 << " = " << nvx1 << " " << nvy1 << " " << nvz1 << " " << dataAtNv1 << " -> " << voxelCentre1 << G4endl;

      if( thePerCentType == GIPCT_Voxel ){
	maxDosePC = dataAtNv1;
	if( DicomVerb(debugVerb) ) G4cout << thePerCentType << " maxDosePC = dataVoxel1st " << maxDosePC << " " << nv1 << G4endl;
      }
      
      double gammaClosest = 0; 
      double perCent;
      double minGamma; 
      if( !bInterpolateVoxels ) {
	// Get closest voxel in sqdose2
	G4int nCVox2X =  GmGenUtils::GetBelowInt((voxelCentre1.x()-fMinX2)/fVoxDimX2);
	if( nCVox2X < 0 || nCVox2X >= G4int(nVoxX2) ) {
	  if( DicomVerb(testVerb) ) G4cout << " CONTINUE VOXEL X NOT FOUND IN IMAGE2 " << nCVox2X << "< 0" << "||" << nCVox2X << ">=" << nVoxX2 << G4endl; 
	  continue;
	}
	G4int nCVox2Y = GmGenUtils::GetBelowInt((voxelCentre1.y()-fMinY2)/fVoxDimY2);
	if( nCVox2Y < 0 || nCVox2Y >= G4int(nVoxY2) ) {
	  //	  	  if( DicomVerb(testVerb) ) G4cout << "  CONTINUE VOXEL Y NOT FOUND IN IMAGE2 " << nCVox2Y << "< 0" << "||" << nCVox2Y << ">=" << nVoxY2 << G4endl;
	  continue;
	}
      
	G4int nCVox2Z = GmGenUtils::GetBelowInt((voxelCentre1.z()-fMinZ2)/fVoxDimZ2);
	if( nCVox2Z < 0 || nCVox2Z >= G4int(nVoxZ2) ) {
	  G4Exception("DCM2DOperGammaIndex",
		      "",
		      FatalException,
		      "THIS SHOULD NOT HAPPEN");
	  continue;
	}
	// Get distance and dose 
	//--- Get gamma from percent and distance
	int nv2 = nCVox2X + nCVox2Y*nVoxX2 + nCVox2Z*nVoxXY2;
	G4double dataAtNv2 = iData2[nv2];
	if( dataAtNv1 == 0. || dataAtNv2 == 0. ) {
	  if( DicomVerb(testVerb) ) G4cout << " CONTINUE VOXEL DATA 0:  " << nv1 << " : " << dataAtNv1 << " " << nv2 << " : " << dataAtNv2 << G4endl; 
	  //-	  histoXSvsGI->Fill(voxelCentre1.x(), voxelCentre1.y(),log10(1.e-9));
	  // iDataOut->at(nv1) = log10(1.e-9);
	  continue; // not if voxel is 0 (SHOULD BE AN OPTION)
	}
	G4ThreeVector voxelCentre2( fMinX2+(nCVox2X+0.5)*fVoxDimX2, fMinY2+(nCVox2Y+0.5)*fVoxDimY2, fMinZ2+(nCVox2Z+0.5)*fVoxDimZ2 );
	if( DicomVerb(debugVerb) ) G4cout << "@@@ CLOSEST VOXEL2 " << nv2 << " = " << nCVox2X << " " << nCVox2Y << " " << nCVox2Z << " -> " << voxelCentre2 << G4endl;
	if( iDataErr1.size() != 0 )  {
	  doseErrAtNv1 = iDataErr1[nv1];
	} else {
	  doseErrAtNv1 = 0.;
	}
	if( iDataErr2.size() != 0 )  {
	  doseErrAtNv2 = iDataErr2[nv2];
	} else {
	  doseErrAtNv2 = 0.;
	} 
	//		G4cout << " BERROR " << bErrors << " bOnlyDiff " << bOnlyDiff<< G4endl; //GDEB
	if( bErrors ) {
	  if( !bOnlyDiff ) {
	    dataAtNv1 > dataAtNv2 ? 
	      perCent = std::max((dataAtNv1-doseErrAtNv1*theNErrorSigmas) - (dataAtNv2+doseErrAtNv2*theNErrorSigmas),G4double(0.))*100./maxDosePC :
	      perCent = std::max((dataAtNv2-doseErrAtNv2*theNErrorSigmas) - (dataAtNv1+doseErrAtNv1*theNErrorSigmas),G4double(0.))*100./maxDosePC;
	    if( DicomVerb(debugVerb) ) G4cout << nvz1*nVoxXY1+nvy1*nVoxX1+nvx1 << " " << theNErrorSigmas << " @@@ bErrors PERCENT CLOSEST " << perCent << " = (" << dataAtNv1 << " - " << dataAtNv2 << " )/ " << doseErrAtNv1 << " " << doseErrAtNv2 << G4endl;
	  } else {
	    if( doseErrAtNv1 == 0. || doseErrAtNv2 == 0. ) {
	      perCent = 0.;
	      if( DicomVerb(debugVerb) ) G4cout << "@@@ ONLY DIFF PERCENT CLOSEST = 0 " << doseErrAtNv1 << " " << doseErrAtNv2 << G4endl;
	    } else {
	      G4double doseErr = sqrt(doseErrAtNv1*doseErrAtNv1+doseErrAtNv2*doseErrAtNv2);
	      perCent = (dataAtNv1-dataAtNv2)/doseErr;
	      if( DicomVerb(debugVerb) ) G4cout << "@@@ ONLY DIFF PERCENT CLOSEST " << perCent << " = (" << dataAtNv1 << " - " << dataAtNv2 << " )/ " << doseErr << " " << doseErrAtNv1 << " " << doseErrAtNv2 << G4endl;
	    }
	  }
	} else {
	  if( !bOnlyDiff ) {
	    perCent = fabs(dataAtNv1-dataAtNv2)*100./maxDosePC;
	  } else {
	    perCent = (dataAtNv1-dataAtNv2)/maxDosePC;
	      if( DicomVerb(debugVerb) ) G4cout << "@@@ ONLY DIFF PERCENT CLOSEST " << perCent << " = (" << dataAtNv1 << " - " << dataAtNv2 << " )/ " << maxDosePC << G4endl;
	  }
	}
	if( DicomVerb(debugVerb) ) G4cout <<nvz1*nVoxXY1+nvy1*nVoxX1+nvx1 << " @@@ PERCENT CLOSEST " << perCent << " = (" << dataAtNv1 << " - " << dataAtNv2 << ")*100/ " << maxDosePC << " err1 "  << doseErrAtNv1*theNErrorSigmas << " err2 " << doseErrAtNv2*theNErrorSigmas << " pt " << voxelCentre1 << G4endl;
	//    double gammaPC = perCent/perCentLim;
	//      double distance = (voxelCentre1-voxelCentre2).mag();
	double distance;
	if( bDistanceByVoxel ) {
	  distance = 0;
	} else {
	  distance = (voxelCentre1-voxelCentre2).perp();
	}
	if( DicomVerb(debugVerb) ) G4cout << "@@@ DISTANCE SAME POSITION " << distance << " pt " << voxelCentre1 << G4endl;
	if( !bOnlyDiff ) {
	  gammaClosest = sqrt(std::pow(perCent/perCentLim,2)+std::pow(distance/minDistLim,2));
	} else {
	  gammaClosest = perCent/perCentLim;
	}
	if( DicomVerb(debugVerb) ) G4cout << "@@@ GAMMA PC_CLOSEST DIST_SAME_POS " << gammaClosest << G4endl;
	
	double distLoop = gammaClosest*minDistLim; // further than this it will make gamma bigger
	G4int nVoxDistLoop = std::max(distLoop/fVoxDimX2,distLoop/fVoxDimY2); // maximum number of voxels in X or Y
	//      nVoxDistLoop = 0;
	if( DicomVerb(debugVerb) ) G4cout << "@@@ DISTANCE TO LOOP " << distLoop << " ->NVox " << nVoxDistLoop << "  XY " << fVoxDimX2 << " " <<fVoxDimY2 << G4endl;
	if( nVoxDistLoop > MAXDISTVOXEL ) {
	  if( DicomVerb(debugVerb) )  G4cerr << "!! WARNING: TOO BIG GAMMA CLOSEST " << nVoxDistLoop << " > " << MAXDISTVOXEL << " " << gammaClosest << " SEARCHING IMAGE2 VOXEL " << nv1 << " = " << nvx1 << " " << nvy1 << " " << nvz1 << " -> " << voxelCentre1 << G4endl;	
	  if( DicomVerb(debugVerb) ) G4cout << "@@@ PERCENT CLOSEST " << perCent << " = (" << dataAtNv1 << " - " << dataAtNv2 << ")*100/ " << maxDosePC << " err1 "  << doseErrAtNv1*theNErrorSigmas << " err2 " << doseErrAtNv2*theNErrorSigmas << G4endl;
	  nVoxDistLoop = MAXDISTVOXEL;
	}
	minGamma = gammaClosest; 
	for( G4int iloop = 1; iloop <= nVoxDistLoop; iloop++ ) {
	  for( int ilx = -std::min(iloop,nCVox2X); ilx <= std::min(iloop,G4int(nVoxX2-nCVox2X-1)); ilx++ ) {
	    for( int ily = -std::min(iloop,nCVox2Y); ily <= std::min(iloop,G4int(nVoxY2-nCVox2Y-1)); ily++ ) {
	      //	for( int ilx = -iloop; ilx <= iloop; ilx++ ) {
	      //  for( int ily = -iloop; ily <= iloop; ily++ ) {
	      if( fabs(ilx) != iloop && fabs(ily) != iloop ) continue;
	      int copyNo2 = (ilx+nCVox2X) + (ily+nCVox2Y)*nVoxX2 + nCVox2Z*nVoxXY2;
	      dataAtNv2 = iData2[copyNo2];     
	      // if( copyNo2 < 0 ) continue;
	      if( DicomVerb(testVerb) ) G4cout << iloop << "@@@ SEARCHING IMAGE2 OTHER VOXELS " << ilx << " " << ily << " -> " << copyNo2 << G4endl;
	      double pc;
	      /*	      if( bErrors ) {
		if( doseErrors1.size() != 0 )  {
		doseErrAtNv1 = doseErrors1[nv1);
		} else {
		  doseErrAtNv1 = 0.;
		}
		if( doseErrors2.size() != 0 )  {
		  doseErrAtNv2 = doseErrors2[copyNo2);
		} else {
		  doseErrAtNv2 = 0.;
		}
		dataAtNv1 > dataAtNv2 ? 
		  pc = std::max((dataAtNv1-doseErrAtNv1*theNErrorSigmas) - (dataAtNv2+doseErrAtNv2*theNErrorSigmas),G4double(0.))*100./maxDosePC :
		  pc = std::max((dataAtNv2-doseErrAtNv2*theNErrorSigmas) - (dataAtNv1+doseErrAtNv1*theNErrorSigmas),G4double(0.))*100./maxDosePC;
		  } else { */
	      pc = fabs(dataAtNv1-dataAtNv2)*100./maxDosePC;
		//	      }
	      
	      G4ThreeVector voxCent2( fMinX2+(ilx+nCVox2X)*fVoxDimX2, fMinY2+(ily+nCVox2Y)*fVoxDimY2, fMinZ2+nCVox2Z*fVoxDimZ2 );
	      double dist;
	      if( bDistanceByVoxel ) {
		dist = sqrt(fabs(ilx)*fVoxDimX2*fabs(ilx)*fVoxDimX2 + fabs(ily)*fVoxDimY2*fabs(ily)*fVoxDimY2);
	      } else {
		dist = (voxelCentre1-voxCent2).mag();
	      }
	      double gamma;
	      if( !bOnlyDiff ) {
		gamma = sqrt(std::pow(pc/perCentLim,2)+std::pow(dist/minDistLim,2));
	      } else {
		gamma = pc/perCentLim;
	      }
	      if( DicomVerb(testVerb) ) G4cout << ilx << ":" << ily << "@@@ PERCENT " << pc << " " << dataAtNv1 << " - " << dataAtNv2 << G4endl;
	      if( DicomVerb(testVerb) ) G4cout << ilx << ":" << ily << "@@@ DISTANCE " << dist << " " << voxelCentre1 << " - " << voxCent2 << G4endl; 
	      if( !bOnlyDiff ) {
		minGamma = std::min(minGamma,gamma);
	      } else {
		if( fabs(gamma) < fabs(minGamma) ) minGamma = gamma;
	      }
	      if( DicomVerb(debugVerb) ) G4cout << ilx << ":" << ily << "@@@ MINGAMMMA " << minGamma << " GAMMA " << gamma << " : PC= " << pc << " DIST= " << dist << G4endl;
	    }
	  }
	}
      } else { // bInterpolateVoxels 
	
	// minimum and maximum of voxel1
	G4double minVX1 = fMinX1+(nvx1)*fVoxDimX1;
	G4double maxVX1 = fMinX1+(nvx1+1)*fVoxDimX1;
	G4double minVY1 = fMinY1+(nvy1)*fVoxDimY1;
	G4double maxVY1 = fMinY1+(nvy1+1)*fVoxDimY1;
	G4double minVZ1 = fMinZ1+(nvz1)*fVoxDimZ1;
	G4double maxVZ1 = fMinZ1+(nvz1+1)*fVoxDimZ1;
	if( DicomVerb(testVerb) ) G4cout << " min/max voxel 1 " << minVX1 << " " << maxVX1 << " " <<  minVY1 << " " << maxVY1 << " " << minVZ1 << " " << maxVZ1 << G4endl;
	// get voxels in sqdose2 included in voxel1
	G4int nVXmin2 = GmGenUtils::GetBelowInt((minVX1-fMinX2)/fVoxDimX2);
	G4int nVXmax2 = GmGenUtils::GetBelowInt((maxVX1-fMinX2)/fVoxDimX2);
	G4int nVYmin2 = GmGenUtils::GetBelowInt((minVY1-fMinY2)/fVoxDimY2);
	G4int nVYmax2 = GmGenUtils::GetBelowInt((maxVY1-fMinY2)/fVoxDimY2);
	G4int nVZmin2 = GmGenUtils::GetBelowInt((minVZ1-fMinZ2)/fVoxDimZ2);
	G4int nVZmax2 = GmGenUtils::GetBelowInt((maxVZ1-fMinZ2)/fVoxDimZ2);
	if( DicomVerb(testVerb) ) G4cout << " min/max voxel 2 "<< fMinX2+(nVXmin2)*fVoxDimX2 << " "<< fMinX2+(nVXmax2)*fVoxDimX2 << " "<< fMinY2+(nVYmin2)*fVoxDimY2 << " "<< fMinY2+(nVYmax2)*fVoxDimY2 << " "<< fMinZ2+(nVZmin2)*fVoxDimZ2 << " " << fMinZ2+(nVZmax2)*fVoxDimZ2 << G4endl;
	if( nVXmin2 < 0 ) nVXmin2 = 0;
	if( nVXmax2 < 0 ) continue;
	if( nVXmin2 >= G4int(nVoxX2) ) continue;
	if( nVXmax2 >= G4int(nVoxX2) ) nVXmax2 = nVoxX2-1;
	if( nVYmin2 < 0 ) nVYmin2 = 0;
	if( nVYmax2 < 0 ) continue;
	if( nVYmin2 >= nVoxY2 ) continue;
	if( nVYmax2 >= nVoxY2 ) nVYmax2 = nVoxY2-1;
	if( nVZmin2 < 0 ) nVZmin2 = 0;
	if( nVZmax2 < 0 ) continue;
	if( nVZmin2 >= nVoxZ2 ) continue;
	if( nVZmax2 >= nVoxZ2 ) nVZmax2 = nVoxZ2-1;
	
	// Get distance and dose 
	//--- Get gamma from percent and distance
	// Get dose form distribution in voxels
	G4double dose2 = 0.;
	G4double volu2 = 0.;
	if( DicomVerb(testVerb) ) G4cout << " LOOPING X " << nVXmin2 << ":" << nVXmax2 << " Y " << nVYmin2 << ":" << nVYmax2 << " Z " << nVZmin2 << ":" << nVZmax2 << " centre " << voxelCentre1 << G4endl;
	for( int ix = nVXmin2; ix <= nVXmax2; ix++ ) {
	  for( int iy = nVYmin2; iy <= nVYmax2; iy++ ) {
	    for( int iz = nVZmin2; iz <= nVZmax2; iz++ ) {
	      G4double vWmin = std::max(minVX1,fMinX2+(ix)*fVoxDimX2);
	      G4double vWmax = std::min(maxVX1,fMinX2+(ix+1)*fVoxDimX2);
	      G4double propX = (vWmax-vWmin)/fVoxDimX1;
	      if( DicomVerb(testVerb) ) G4cout << " X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propX << G4endl;
	      vWmin = std::max(minVY1,fMinY2+(iy)*fVoxDimY2);
	      vWmax = std::min(maxVY1,fMinY2+(iy+1)*fVoxDimY2);
	      G4double propY = (vWmax-vWmin)/fVoxDimY1;
	      if( DicomVerb(testVerb) ) G4cout << " X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propY << G4endl;
	      vWmin = std::max(minVZ1,fMinZ2+(iz)*fVoxDimZ2);
	      vWmax = std::min(maxVZ1,fMinZ2+(iz+1)*fVoxDimZ2);
	      G4double propZ = (vWmax-vWmin)/fVoxDimZ1;
	      if( DicomVerb(testVerb) ) G4cout << " X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propZ << G4endl;
	      
	      volu2 += propX*propY*propZ;
	      int nv2 = ix + iy*nVoxX2 + iz*nVoxXY2;
	      G4double dataAtNv2 = iData2[nv2];
	      dose2 += propX*propY*propZ * dataAtNv2;
	      if( DicomVerb(testVerb) ) G4cout << " DOSE PROP " << ix << ":" << iy << ":" << iz << " :" << nv2 << " dose " << dose2 << " volu " << volu2 << G4endl;
	    }
	  }
	}
	
	perCent = fabs(dataAtNv1-dose2)*100./maxDosePC;
	double distance = 0.; // it is the same voxel
	if( DicomVerb(debugVerb) ) G4cout << "@@@ PERCENT CLOSEST " << perCent << " = (" << dataAtNv1 << " - " << dose2 << ")*100/ " << maxDosePC << " err1 "  << doseErrAtNv1*theNErrorSigmas << " err2 " << doseErrAtNv2*theNErrorSigmas << G4endl; 
	if( DicomVerb(debugVerb) ) G4cout << "@@@ DISTANCE CLOSEST " << distance << G4endl;
	gammaClosest = sqrt(std::pow(perCent/perCentLim,2)+std::pow(distance/minDistLim,2));
	if( DicomVerb(debugVerb) ) G4cout << "@@@ GAMMA CLOSEST " << gammaClosest << G4endl;
	
	double distLoop = gammaClosest*minDistLim; // further than this it will make gamma bigger
	G4int nVoxDistLoop = std::max(distLoop/fVoxDimX1,distLoop/fVoxDimY1); // maximum number of voxels in X or Y
	//      nVoxDistLoop = 0;
	if( DicomVerb(debugVerb) ) G4cout << "@@@ DISTANCE TO LOOP " << distLoop << " ->NVox " << nVoxDistLoop << G4endl;
	if( nVoxDistLoop > MAXDISTVOXEL ) {
	  G4cerr << "!! WARNING: TOO BIG GAMMA CLOSEST " << nVoxDistLoop << " > " << MAXDISTVOXEL << " " << gammaClosest << " LOOPING VOXEL " << nv1 << " = " << nvx1 << " " << nvy1 << " " << nvz1 << " -> " << voxelCentre1 << G4endl;	
	  if( DicomVerb(debugVerb) ) G4cout << "@@@ PERCENT CLOSEST " << perCent << " = (" << dataAtNv1 << " - " << dose2 << ")*100/ " << maxDosePC << " err1 "  << doseErrAtNv1*theNErrorSigmas << " err2 " << doseErrAtNv2*theNErrorSigmas << G4endl;
	  nVoxDistLoop = MAXDISTVOXEL;
	}
	minGamma = gammaClosest;
	
	for( G4int iloop = 1; iloop <= nVoxDistLoop; iloop++ ) {
	  for( int ilx = -std::min(iloop,nvx1); ilx <= std::min(iloop,nVoxX1-nvx1-1); ilx++ ) { // do not loop out of borders, i.e. iloop < nvx1 , iloop >= nVoxX1-nvx1
	    for( int ily = -std::min(iloop,nvy1); ily <= std::min(iloop,nVoxY1-nvy1-1); ily++ ) {
	      if( fabs(ilx) != iloop && fabs(ily) != iloop ) continue;
	      // minimum and maximum of voxel1
	      /*	      G4double minVX1 = fMinX1+(nvx1+ilx)*fVoxDimX1;
			      G4double maxVX1 = fMinX1+(nvx1+ilx+1)*fVoxDimX1;
			      G4double minVY1 = fMinY1+(nvy1+ilx)*fVoxDimY1;
			      G4double maxVY1 = fMinY1+(nvy1+ilx+1)*fVoxDimY1;
			      if( DicomVerb(testVerb) ) G4cout << "O min/max voxel 1 " << minVX1 << " " << maxVX1 << " " <<  minVY1 << " " << maxVY1 << " " << minVZ1 << " " << maxVZ1 << G4endl;
	      // get voxels in sqdose2 included in voxel1
	      G4int nVXmin2 = GmGenUtils::GetBelowInt((minVX1-fMinX2)/fVoxDimX2);
	      G4int nVXmax2 = GmGenUtils::GetBelowInt((maxVX1-fMinX2)/fVoxDimX2);
	      G4int nVYmin2 = GmGenUtils::GetBelowInt((minVY1-fMinY2)/fVoxDimY2);
	      G4int nVYmax2 = GmGenUtils::GetBelowInt((maxVY1-fMinY2)/fVoxDimY2);
	      G4int nVZmin2 = GmGenUtils::GetBelowInt((minVZ1-fMinZ2)/fVoxDimZ2);
	      G4int nVZmax2 = GmGenUtils::GetBelowInt((maxVZ1-fMinZ2)/fVoxDimZ2);
	      if( DicomVerb(testVerb) ) G4cout << "O min/max voxel 2 "<< fMinX2+(nVXmin2)*fVoxDimX2 << " "<< fMinX2+(nVXmax2)*fVoxDimX2 << " "<< fMinY2+(nVYmin2)*fVoxDimY2 << " "<< fMinY2+(nVYmax2)*fVoxDimY2 << " "<< fMinZ2+(nVZmin2)*fVoxDimZ2 << " " << fMinZ2+(nVZmax2)*fVoxDimZ2 << G4endl; */
	      if( nVXmin2 < 0 ) nVXmin2 = 0;
	      if( nVXmax2 < 0 ) continue;
	      if( nVXmin2 >= nVoxX2 ) continue;
	      if( nVXmax2 >= nVoxX2 ) nVXmax2 = nVoxX2-1;
	      if( nVYmin2 < 0 ) nVYmin2 = 0;
	      if( nVYmax2 < 0 ) continue;
	      if( nVYmin2 >= nVoxY2 ) continue;
	      if( nVYmax2 >= nVoxY2 ) nVYmax2 = nVoxY2-1;
	      if( nVZmin2 < 0 ) nVZmin2 = 0;
	      if( nVZmax2 < 0 ) continue;
	      if( nVZmin2 >= nVoxZ2 ) continue;
	      if( nVZmax2 >= nVoxZ2 ) nVZmax2 = nVoxZ2-1;
	      
	      // Get distance and dose 
	      //--- Get gamma from percent and distance
	      // Get dose form distribution in vozels
	      dose2 = 0.;
	      volu2 = 0.;
	      if( DicomVerb(testVerb) ) G4cout << "O LOOPING X " << nVXmin2 << ":" << nVXmax2 << " Y " << nVYmin2 << ":" << nVYmax2 << " Z " << nVZmin2 << ":" << nVZmax2 << " centre " << voxelCentre1 << G4endl;
	      for( int ix = nVXmin2; ix <= nVXmax2; ix++ ) {
		for( int iy = nVYmin2; iy <= nVYmax2; iy++ ) {
		  for( int iz = nVZmin2; iz <= nVZmax2; iz++ ) {
		    G4double vWmin = std::max(minVX1,fMinX2+(ix)*fVoxDimX2);
		    G4double vWmax = std::min(maxVX1,fMinX2+(ix+1)*fVoxDimX2);
		    G4double propX = (vWmax-vWmin)/fVoxDimX1;
		    if( DicomVerb(testVerb) ) G4cout << "O X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propX << G4endl;
		    vWmin = std::max(minVY1,fMinY2+(iy)*fVoxDimY2);
		    vWmax = std::min(maxVY1,fMinY2+(iy+1)*fVoxDimY2);
		    G4double propY = (vWmax-vWmin)/fVoxDimY1;
		    if( DicomVerb(testVerb) ) G4cout << "O X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propY << G4endl;
		    vWmin = std::max(minVZ1,fMinZ2+(iz)*fVoxDimZ2);
		    vWmax = std::min(maxVZ1,fMinZ2+(iz+1)*fVoxDimZ2);
		    G4double propZ = (vWmax-vWmin)/fVoxDimZ1;
		    if( DicomVerb(testVerb) ) G4cout << "O X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propZ << G4endl;
		    
		    volu2 += propX*propY*propZ;
		    int nv2 = ix + iy*nVoxX2 + iz*nVoxXY2;
		    G4double dataAtNv2 = iData2[nv2];
		    dose2 += propX*propY*propZ * dataAtNv2;
		    if( DicomVerb(testVerb) ) G4cout << "O DOSE PROP " << ix << ":" << iy << ":" << iz << " :" << nv2 << " dose " << dose2 << " volu " << volu2 << G4endl; 
		  }
		}
	      }
	      
	      double pc = fabs(dataAtNv1-dose2)*100./maxDosePC;
	      
	      G4ThreeVector voxCentN( fMinX1+(ilx+nvx1)*fVoxDimX1, fMinY1+(ily+nvy1)*fVoxDimY1, fMinZ1+nvz1*fVoxDimZ1 );
	      double dist;
	      if( bDistanceByVoxel ) {
		dist = sqrt(fabs(ilx)*fVoxDimX1*fabs(ilx)*fVoxDimX1 + fabs(ily)*fVoxDimY1*fabs(ily)*fVoxDimY1);
	      } else {
		dist = (voxelCentre1-voxCentN).mag();
	      }
	      double gamma = sqrt(std::pow(pc/perCentLim,2)+std::pow(dist/minDistLim,2));
	      if( DicomVerb(testVerb) ) G4cout << ilx << ":" << ily << "@@@ PERCENT " << pc << " " << dataAtNv1 << " - " << dose2 << G4endl;
	      if( DicomVerb(testVerb) ) G4cout << ilx << ":" << ily << "@@@ DISTANCE " << dist << " " << voxelCentre1 << " - " << voxCentN << G4endl; 
	      minGamma = std::min(minGamma,gamma);
	      if( DicomVerb(testVerb) ) G4cout << ilx << ":" << ily << "@@@ MINGAMMMA " << minGamma << " GAMMA " << gamma << " : PC= " << pc << " DIST= " << dist << G4endl;
	    }
	  }
	}
      }

      if( bHisto1D ) {
	histoGI->Fill(minGamma);
	histoGIALL->Fill(minGamma);
      }
      iDataOut->at(nv1) = minGamma;
      //      if( minGamma != 0 ) G4cout << " FILL iDataOut " << nv1 << ": " << iDataOut->at(nv1) << "= " <<minGamma << G4endl; //GDEB
      //      histoXSvsGI->Fill(voxelCentre1.x(), voxelCentre1.y(),minGamma);
      if( minGamma > 1. ) nGI++;
      nPOINTS++;
      if( DicomVerb(debugVerb) ) std::cout << "FINAL gamma " << voxelCentre1 << " = " << minGamma << std::endl;
      theGammaFinal.insert(std::multiset<double>::value_type(minGamma));
    }

    //----- PLOT GI
    //    pad1->cd();
    if( bHisto1D ) {
      histoGI->Draw("histo");
    }
    double gammaPVal = 0.;
    std::multiset<double>::const_iterator iteg;
    int jj = 0;
    G4int nGamma = theGammaFinal.size();
    for( iteg = theGammaFinal.begin(); iteg != theGammaFinal.end(); iteg++, jj++ ) {	
      if( DicomVerb(testVerb) ) std::cout << jj << " PVAL " << G4double(jj)/nGamma << " = " << *iteg << std::endl; 
      if( G4double(jj)/nGamma > 0.95 ) {
	gammaPVal = *iteg;
	break;
      }
    }
    if( bHisto1D ) {
      G4double mean = histoGI->GetMean();
      mean = int(mean*100)/100.;
      G4double rms = histoGI->GetRMS(); 
      rms = int(rms*100)/100.;
      if( DicomVerb(debugVerb) ) std::cout << perCentLim << "% / " << minDistLim << "mm : MEAN GAMMA INDEX " << histoGI->GetMean() << " +- " << histoGI->GetRMS() << " P" << pval*100 << "= " << gammaPVal <<  std::endl;
      //      xInit = 375*canvas->GetWw()/500;
      double xInit = nvz1*100*canvas->GetWw()/500;
      //      xInit = 290*canvas->GetWw()/500;
      double yInit= nvz1*100*canvas->GetWh()/500;
      EColor color = kBlack;
      //    pad1->cd(); 
      DrawWordInPave( histoGI, "G" + GmGenUtils::ftoa(perCentLim)+"/"+GmGenUtils::ftoa(minDistLim)+ ": " + GmGenUtils::ftoa(mean) + "+-" + GmGenUtils::ftoa(rms) + " P" + GmGenUtils::itoa(pval*100) + "= " + GmGenUtils::ftoa(int(gammaPVal*100)/100.), xInit, yInit, color, 0.02 );
      canvas->Print(("his"+G4String(histoGI->GetName())+".gif").c_str());
    }
   //    pad1->cd(); 
    //    theHistosXSvsGI[zMin+fVoxDimZ1/2.] = histoXSvsGI;
    /*    if( !bMaxAtPoint && !bMaxGammaValue ) {
      gStyle->SetOptStat(0);
      //      histoXSvsGI->Draw("colz");
      //     canvas->Print(("his"+G4String(histoXSvsGI->GetName())+".gif").c_str());
      } */
  }
  if( bHisto1D ) {
    G4double mean = histoGIALL->GetMean();
    mean = int(mean*100)/100.;
    G4double rms = histoGIALL->GetRMS(); 
    rms = int(rms*100)/100.;
    if( DicomVerb(debugVerb) ) std::cout << perCentLim << "% / " << minDistLim << "mm : MEAN GAMMA INDEX " << histoGIALL->GetMean() << " +- " << histoGIALL->GetRMS() << std::endl;
     //    pad1->cd(); 
    gStyle->SetOptStat(1111111);
    histoGIALL->SetMaximum(50000);
    histoGIALL->Draw("histo");
    double xInit = 200*theMaxGammaValue/2.*canvas->GetWw()/500;
      //      xInit = 290*canvas->GetWw()/500;    
    double yInit= 25000.*canvas->GetWh()/500;
    EColor color = kBlack;
      //    pad1->cd(); 
    //    DrawWordInPave( histoGI, "G" + GmGenUtils::ftoa(perCentLim)+"/"+GmGenUtils::ftoa(minDistLim)+ ": " + GmGenUtils::ftoa(mean) + "+-" + GmGenUtils::ftoa(rms) + " P" + GmGenUtils::itoa(pval*100) + "= " + GmGenUtils::ftoa(int(gammaPVal*100)/100.), xInit, yInit, color, 0.02 );
    //    DrawWordInPave( histoGI, "HOLA AQUI ESTAaMOS", xInit, yInit, color, 0.02 );

    canvas->Print(("his"+G4String(histoGIALL->GetName())+".gif").c_str());
    canvas->SetLogy(1);
    histoGIALL->Draw("histo");
    canvas->Print(("his"+G4String(histoGIALL->GetName())+"logY.gif").c_str());
  }

}

//------------------------------------------------------------------------
void DCM2DOperGammaIndex::OperateXZ( DicomVImage* image1, DicomVImage* image2, double perCentLim, double minDistLim, double pval, DicomVImage* imageOut )
{
  G4bool bDistanceByVoxel = false;//  TODO

  G4bool bInterpolateVoxels = G4bool(theParamMgr->GetNumericValue("bInterpolateVoxels",0));
  //  if( bInterpolateVoxels && *image1 == *image2 ) { // if images have same dimensions, no interpolation is needed
  //  bInterpolateVoxels = false; 
  // }
  
  //----- Extract image1 data
  G4int nVoxX1 = image1->GetNoVoxelsX();
  G4int nVoxY1 = image1->GetNoVoxelsY();
  G4int nVoxXY1 = nVoxX1*nVoxY1;
  G4int nVoxZ1 = image1->GetNoVoxelsZ();
  G4int nVoxXZ1 = nVoxX1*nVoxZ1;
  G4int nVox1 = nVoxX1*nVoxY1*nVoxZ1;
  G4double fMinX1 = image1->GetMinX();
  G4double fMinY1 = image1->GetMinY();
  G4double fMinZ1 = image1->GetMinZ();
  G4double fMaxX1 = image1->GetMaxX();
  G4double fMaxY1 = image1->GetMaxY();
  G4double fMaxZ1 = image1->GetMaxZ();
  G4double fVoxDimX1 = image1->GetVoxelDimX();
  G4double fVoxDimY1 = image1->GetVoxelDimY();
  G4double fVoxDimZ1 = image1->GetVoxelDimZ();
  std::vector<G4double> iData1 = *(image1->GetData());
  std::vector<G4double>* iDataOut = imageOut->GetData();

  TH1F* histoGI; 
  //  TH2F* histoXSvsGI;
  double theHGILim = std::min(100.,double(MAXDISTVOXEL));
  TCanvas* canvas = new TCanvas();
  //  std::map<G4double,TH2F*> theHistosXSvsGI;
 
  /*  if( bImageEqual ) {
    DicomVImage* image2Eq;
    if( !theResizeOperator ) theResizeOperator = new DicomOperResize(1,"GIOperResize");
    theResizeOperator->Operate(image2Eq, nVoxX1, fMinX1, fMaxX1, nVoxY1, fMinY1, fMaxY1, nVoxZ1, fMinZ1, fMaxZ1);  // make image2 as image1
    }*/
  
  if( DicomVerb(debugVerb) ) G4cout << "@@@ START DCM2DOperGammaIndex " << perCentLim << " " << minDistLim << " " << pval << G4endl;

  /*  DicomVImage* imageErr1 = image1->GetErrorData();
  std::vector<DIfloat>* iDataErr1 = 0;
  if( imageErr1 ) iDataErr1 = imageErr1->GetData();
  */  
  if( DicomVerb(infoVerb) )
    std::cout << "@@@ READ image nVoxX/Y/Z " << nVoxX1 << " " << nVoxY1 << " " << nVoxZ1 << G4endl
	      << " fMinX/Y/Z " << fMinX1 << " " << fMinY1 << " " << fMinZ1 << G4endl
	      << " fMaxX/Y/Z " << fMaxX1 << " " << fMaxY1 << " " << fMaxZ1 << G4endl
	      << " fVoxDimX/Y/Z " << fVoxDimX1 << " " << fVoxDimY1 << " " << fVoxDimZ1 << G4endl
	      << " NDOSES " << nVox1 << G4endl;

  //----- Extract image2 data
  G4int nVoxX2 = image2->GetNoVoxelsX();
  G4int nVoxY2 = image2->GetNoVoxelsY();
  G4int nVoxZ2 = image2->GetNoVoxelsZ();
  G4int nVoxXY2 = nVoxX2*nVoxY2;
  G4int nVox2 = nVoxX2*nVoxY2*nVoxZ2;
  //  size_t nVoxZ2 = image2->GetNoVoxelsZ();
  G4double fMinX2 = image2->GetMinX();
  G4double fMinY2 = image2->GetMinY();
  G4double fMinZ2 = image2->GetMinZ();
  G4double fMaxX2 = image2->GetMaxX();
  G4double fMaxY2 = image2->GetMaxY();
  G4double fMaxZ2 = image2->GetMaxZ();
  G4double fVoxDimX2 = image2->GetVoxelDimX();
  G4double fVoxDimY2 = image2->GetVoxelDimY();
  G4double fVoxDimZ2 = image2->GetVoxelDimZ();
  std::vector<G4double> iData2 = *(image2->GetData());
  /*  DicomVImage* imageErr2 = image2->GetErrorData();
  std::vector<DIfloat>* iDataErr2 = 0;
  if( imageErr2 ) iDataErr2 = imageErr2->GetData();
  */
  if( DicomVerb(infoVerb) )
    std::cout << "@@@ READ sqdose2 nVoxX/Y/Z " << nVoxX2 << " " << nVoxY2 << " " << nVoxZ2 << G4endl
	      << " fMinX/Y/Z " << fMinX2 << " " << fMinY2 << " " << fMinZ2 << G4endl
	      << " fMaxX/Y/Z " << fMaxX2 << " " << fMaxY2 << " " << fMaxZ2 << G4endl
	      << " fVoxDimX/Y/Z " << fVoxDimX2 << " " << fVoxDimY2 << " " << fVoxDimZ2 << G4endl
	      << " NDOSES " << nVox2 << G4endl;

  //----- Start calculation
  G4int nGI = 0;
  G4int nPOINTS = 0;
  std::multiset<double> theGammaFinal; 
  G4double fVoxX1X2 = (std::min(fMaxX1,fMaxX2)-std::max(fMinX1,fMinX2));
  G4int nVoxX1X2 =  GmGenUtils::GetBelowInt(fVoxX1X2/fVoxDimX1); 
  G4double hfVoxDimX1X2 = nVoxX1X2*fVoxDimX1;
  G4double diffhfVoxDimX1X2 = hfVoxDimX1X2 - fVoxX1X2;
  if( DicomVerb(testVerb) ) G4cout << " fVoxX1X2 " << fVoxX1X2 << " nVoxX1X2 " << nVoxX1X2 << " hfVoxDimX1X2 " <<hfVoxDimX1X2 << " diffhfVoxDimX1X2 " <<  diffhfVoxDimX1X2 << G4endl;
  G4double fMinX1X2 = std::max(fMinX1,fMinX2)-diffhfVoxDimX1X2/2.;
  G4double fMaxX1X2 = std::min(fMaxX1,fMaxX2)+diffhfVoxDimX1X2/2.;
  G4double fVoxDimX1X2 = (fMaxX1X2-fMinX1X2)/nVoxX1X2;
  fMinX1X2 = std::max(fMinX1X2, GetMinAtVoxel(fMinX1, fMinX1X2, fVoxDimX1X2) );
  fMaxX1X2 = std::min(fMaxX1X2, GetMaxAtVoxel(fMaxX1, fMaxX1X2, nVoxX1X2, fVoxDimX1X2) );
  nVoxX1X2 = GmGenUtils::GetBelowInt((fMaxX1X2-fMinX1X2)/fVoxDimX1X2); 

  
  G4double fVoxZ1Z2 = (std::min(fMaxZ1,fMaxZ2)-std::max(fMinZ1,fMinZ2));
  G4int nVoxZ1Z2 =  GmGenUtils::GetBelowInt(fVoxZ1Z2/fVoxDimZ1); 
  G4double hfVoxDimZ1Z2 = nVoxZ1Z2*fVoxDimZ1;
  G4double diffhfVoxDimZ1Z2 = hfVoxDimZ1Z2 - fVoxZ1Z2;
  if( DicomVerb(infoVerb) ) G4cout << " fVoxZ1Z2 " << fVoxZ1Z2 << " nVoxZ1Z2 " << nVoxZ1Z2 << " hfVoxDimZ1Z2 " <<hfVoxDimZ1Z2 << " diffhfVoxDimZ1Z2 " <<  diffhfVoxDimZ1Z2 << G4endl;
  G4double fMinZ1Z2 = std::max(fMinZ1,fMinZ2)-diffhfVoxDimZ1Z2/2.;
  G4double fMaxZ1Z2 = std::min(fMaxZ1,fMaxZ2)+diffhfVoxDimZ1Z2/2.;
  G4double fVoxDimZ1Z2 = (fMaxZ1Z2-fMinZ1Z2)/nVoxZ1Z2;
  fMinZ1Z2 = std::max(fMinZ1Z2, GetMinAtVoxel(fMinZ1, fMinZ1Z2, fVoxDimZ1Z2) );
  fMaxZ1Z2 = std::min(fMaxZ1Z2, GetMaxAtVoxel(fMaxZ1, fMaxZ1Z2, nVoxZ1Z2, fVoxDimZ1Z2) );
  nVoxZ1Z2 = GmGenUtils::GetBelowInt((fMaxZ1Z2-fMinZ1Z2)/fVoxDimZ1Z2); 
  G4int ptExtN = G4int(theParamMgr->GetNumericValue("pointExtensionN",0));
  G4ThreeVector pointExtension(ptExtN*image1->GetVoxelDimX(),ptExtN*image1->GetVoxelDimY(),ptExtN*image1->GetVoxelDimZ());

  float maxDosePC = 0;
  if( thePerCentType == GIPCT_ImageMax ) {
    maxDosePC = image1->GetMaxValue();
    if( DicomVerb(debugVerb) ) G4cout << thePerCentType << " maxDosePC = maxDose1 " << maxDosePC << G4endl;
  } else if( thePerCentType == GIPCT_AtPoint ){
    maxDosePC = image1->GetValueAtPoint(thePointPerCent,pointExtension);
    if( DicomVerb(debugVerb) ) G4cout << thePerCentType << " maxDosePC = theMaxAtPoint1st " << maxDosePC << G4endl; 
  } else { // None
    maxDosePC = std::max(image1->GetMaxValue(),image2->GetMaxValue());
    if( DicomVerb(debugVerb) ) G4cout << thePerCentType << " maxDosePC = max(maxDose1,maxDose2) " << maxDosePC << G4endl;
  }

  //----- Loop to each Y plane
  for( G4int nvy1 = 0; nvy1 < nVoxY1; nvy1++ ){
     if( DicomVerb(testVerb) ) G4cout << " Y PLANE " << nvy1 << G4endl; 
    G4double yMin = fMinY1 + nvy1*fVoxDimY1;
    G4double yMax = yMin + fVoxDimY1;
    if( yMin < fMinY1 || yMax > fMaxY1 ) continue;
    if( yMin < fMinY2 || yMax > fMaxY2 ) continue;
    
    G4String ystr = GmGenUtils::ftoa(yMin+fVoxDimY1/2.);
    std::string limstr = "GammaIndex_XZ_"+GmGenUtils::ftoa(perCentLim)+"_"+GmGenUtils::ftoa(minDistLim)+"_1D_"+ystr;
    if( bHisto1D ) {
      histoGI = new TH1F((limstr).c_str(),(limstr).c_str(),std::min(100.,theHGILim*10),0.,theHGILim);
    }
  
    //    histoXSvsGI = new TH2F(limstr.c_str(),limstr.c_str(),nVoxX1, fMinX1, fMinX1+nVoxX1*fVoxDimX1, nVoxY1, fMinY1, fMinY1+nVoxY1*fVoxDimY1);
    //t    histoXSvsGI = new TH2F(limstr.c_str(),limstr.c_str(),nVoxX1X2, std::max(fMinX1,fMinX2), std::min(fMaxX1,fMaxX2), nVoxY1Y2,  std::max(fMinY1,fMinY2), std::min(fMaxY1,fMaxY2));
    //    histoXSvsGI = new TH2F((limstr+"_2D").c_str(),(limstr+"_2D").c_str(),nVoxX1X2, fMinX1X2, fMaxX1X2, nVoxZ1Z2, fMinZ1Z2, fMaxZ1Z2);
     if( DicomVerb(testVerb) ) {
      G4cout << " HISTOXSvsGIO " << nVoxX1<< " " <<  fMinX1<< " " <<  fMinX1+nVoxX1*fVoxDimX1<< " " <<  nVoxZ1<< " " <<  fMinZ1<< " " <<  fMinZ1+nVoxZ1*fVoxDimZ1 << G4endl;
      G4cout << " HISTOXSvsGIO " << (fMinX1+nVoxX1*fVoxDimX1-fMinX1)/nVoxX1 << " " <<  (fMinZ1+nVoxZ1*fVoxDimZ1-fMinZ1)/nVoxZ1 << G4endl; 
      G4cout << " HISTOXSvsGIN " << nVoxX1X2<< " " << fMinX1X2 << " " << fMaxX1X2 << " " <<  nVoxZ1Z2<< " " <<   std::max(fMinZ1,fMinZ2)<< " " <<  std::min(fMaxZ1,fMaxZ2) << G4endl; 
      G4cout << " HISTOXSvsGIN " << (fMaxX1X2-fMinX1X2)/nVoxX1X2 << " " <<  (fMaxZ1Z2-fMinZ1Z2)/nVoxZ1Z2 << G4endl; 
    }
    
    float doseErrAtNv1 = 0.;
    float doseErrAtNv2 = 0.;
    
    //----- Loop in XY plane
    /*    for( G4int nv1 = 0; nv1 < nVox1; nv1++ ){
	  G4double dataAtNv1 = iData1[nv1];
	  G4cout << nv1 << " TESTdata " << dataAtNv1 << " data2 " << iData2[nv1] << G4endl; //GDEB
	  } */
   for( G4int nvxz1 = 0; nvxz1 < nVoxXZ1; nvxz1++ ){
      G4int nvx1 = nvxz1%nVoxX1;
      G4int nvz1 = (nvxz1/nVoxX1)%nVoxZ1;
      G4int nv1 = nvz1*nVoxXY1+nvy1*nVoxX1+nvx1;
      G4double dataAtNv1 = iData1[nv1];
      G4double xMin = fMinX1 + nvx1*fVoxDimX1;
      G4double xMax = xMin + fVoxDimX1;
      if( xMin < fMinX1 || xMax > fMaxX1 ) continue;
      if( xMin < fMinX2 || xMax > fMaxX2 ) continue;
      G4double zMin = fMinZ1 + nvz1*fVoxDimZ1;
      G4double zMax = zMin + fVoxDimZ1;
      if( zMin < fMinZ1 || zMax > fMaxZ1 ) continue;
      if( zMin < fMinZ2 || zMax > fMaxZ2 ) continue;
      
      //    if( nz1 == 2 ) break;//GDEB
      //      G4int copyNo1GI = nvx1 + nvy1*nVoxX1 + nvz1*nVoxXY1; //--- voxel ID
          
      G4ThreeVector voxelCentre1( fMinX1+(nvx1+0.5)*fVoxDimX1, fMinY1+(nvy1+0.5)*fVoxDimY1, fMinZ1+(nvz1+0.5)*fVoxDimZ1 );
      
      if( DicomVerb(debugVerb) ) G4cout << "@@@ LOOPING VOXEL " << nv1 << " = " << nvx1 << " " << nvy1 << " " << nvz1 << " -> " << voxelCentre1 << G4endl;

      if( thePerCentType == GIPCT_Voxel ){
	maxDosePC = dataAtNv1;
	if( DicomVerb(debugVerb) ) G4cout << thePerCentType << " maxDosePC = dataVoxel1st " << maxDosePC << " " << nv1 << G4endl;
      }
      //      G4cout << " maxDosePC = dataAtNv1 " << maxDosePC << " " << dataAtNv1 << " " << nv1 << G4endl; //GDEB
      
      double gammaClosest = 0; 
      double perCent;
      double minGamma; 
      if( !bInterpolateVoxels ) {
	// Get closest voxel in sqdose2
	G4int nCVox2X =  GmGenUtils::GetBelowInt((voxelCentre1.x()-fMinX2)/fVoxDimX2);
	if( nCVox2X < 0 || nCVox2X >= G4int(nVoxX2) ) {
	  if( DicomVerb(testVerb) ) G4cout << " CONTINUE VOXEL X NOT FOUND IN IMAGE2 " << nCVox2X << "< 0" << "||" << nCVox2X << ">=" << nVoxX2 << G4endl; 
	  continue;
	}
	G4int nCVox2Y = GmGenUtils::GetBelowInt((voxelCentre1.y()-fMinY2)/fVoxDimY2);
	if( nCVox2Y < 0 || nCVox2Y >= G4int(nVoxY2) ) {
	  //	  	  if( DicomVerb(testVerb) ) G4cout << "  CONTINUE VOXEL Y NOT FOUND IN IMAGE2 " << nCVox2Y << "< 0" << "||" << nCVox2Y << ">=" << nVoxY2 << G4endl;
	  continue;
	}
      
	G4int nCVox2Z = GmGenUtils::GetBelowInt((voxelCentre1.z()-fMinZ2)/fVoxDimZ2);
	if( nCVox2Y < 0 || nCVox2Y >= G4int(nVoxY2) ) {
	  G4Exception("DCM2DOperGammaIndex",
		      "",
		      FatalException,
		      "THIS SHOULD NOT HAPPEN");
	  continue;
	}
	// Get distance and dose 
	//--- Get gamma from percent and distance
	int nv2 = nCVox2X + nCVox2Y*nVoxX2 + nCVox2Z*nVoxXY2;
	G4double dataAtNv2 = iData2[nv2];
	if( dataAtNv1 == 0. || dataAtNv2 == 0. ) {
	  if( DicomVerb(testVerb) ) G4cout << " CONTINUE VOXEL DATA 0:  " << nv1 << " : " << dataAtNv1 << " " << nv2 << " : " << dataAtNv2 << G4endl; 
	  //-	  histoXSvsGI->Fill(voxelCentre1.x(), voxelCentre1.y(),log10(1.e-9));
	  //	  iDataOut->at(nv1) = log10(1.e-9);
	  continue; // not if voxel is 0 (SHOULD BE AN OPTION)
	}
	G4ThreeVector voxelCentre2( fMinX2+(nCVox2X+0.5)*fVoxDimX2, fMinY2+(nCVox2Y+0.5)*fVoxDimY2, fMinZ2+(nCVox2Z+0.5)*fVoxDimZ2 );
	if( DicomVerb(debugVerb) ) G4cout << "@@@ CLOSEST VOXEL2 " << nv2 << " = " << nCVox2X << " " << nCVox2Y << " " << nCVox2Z << " -> " << voxelCentre2 << G4endl;
	/*	if( ( doseErrors1.size() != 0 )  {
		doseErrAtNv1 = doseErrors1[nv1);
		} else {
		doseErrAtNv1 = 0.;
	}
	if( doseErrors2.size() != 0 )  {
	doseErrAtNv2 = doseErrors2[nv2);
	} else {
	  doseErrAtNv2 = 0.;
	  } */
	if( bErrors ) {
	  dataAtNv1 > dataAtNv2 ? 
	    perCent = std::max((dataAtNv1-doseErrAtNv1*theNErrorSigmas) - (dataAtNv2+doseErrAtNv2*theNErrorSigmas),G4double(0.))*100./maxDosePC :
	    perCent = std::max((dataAtNv2-doseErrAtNv2*theNErrorSigmas) - (dataAtNv1+doseErrAtNv1*theNErrorSigmas),G4double(0.))*100./maxDosePC;				 
	} else {
	  perCent = fabs(dataAtNv1-dataAtNv2)*100./maxDosePC;
	}
	if( DicomVerb(debugVerb) ) G4cout << "@@@ PERCENT CLOSEST " << perCent << " = (" << dataAtNv1 << " - " << dataAtNv2 << ")*100/ " << maxDosePC << " err1 "  << doseErrAtNv1*theNErrorSigmas << " err2 " << doseErrAtNv2*theNErrorSigmas << G4endl;
	//    double gammaPC = perCent/perCentLim;
	//      double distance = (voxelCentre1-voxelCentre2).mag();
	double distance;
	if( bDistanceByVoxel ) {
	  distance = 0;
	} else {
	  distance = (voxelCentre1-voxelCentre2).perp();
	}
	if( DicomVerb(debugVerb) ) G4cout << "@@@ DISTANCE CLOSEST " << distance << G4endl;
	gammaClosest = sqrt(std::pow(perCent/perCentLim,2)+std::pow(distance/minDistLim,2));
	
	if( DicomVerb(debugVerb) ) G4cout << "@@@ GAMMA CLOSEST " << gammaClosest << G4endl;
	
	double distLoop = gammaClosest*minDistLim; // further than this it will make gamma bigger
	G4int nVoxDistLoop = std::max(distLoop/fVoxDimX2,distLoop/fVoxDimZ2); // maximum number of voxels in X or Z
	//      nVoxDistLoop = 0;
	if( DicomVerb(debugVerb) ) G4cout << "@@@ DISTANCE TO LOOP " << distLoop << " ->NVox " << nVoxDistLoop << "   " << fVoxDimX2 << " " << fVoxDimZ2 << G4endl;
	if( nVoxDistLoop > MAXDISTVOXEL ) {
	  if( DicomVerb(debugVerb) )  G4cerr << "!! WARNING: TOO BIG GAMMA CLOSEST " << nVoxDistLoop << " > " << MAXDISTVOXEL << " " << gammaClosest << " SEARCHING IMAGE2 VOXEL " << nv1 << " = " << nvx1 << " " << nvy1 << " " << nvz1 << " -> " << voxelCentre1 << G4endl;	
	  if( DicomVerb(debugVerb) ) G4cout << "@@@ PERCENT CLOSEST " << perCent << " = (" << dataAtNv1 << " - " << dataAtNv2 << ")*100/ " << maxDosePC << " err1 "  << doseErrAtNv1*theNErrorSigmas << " err2 " << doseErrAtNv2*theNErrorSigmas << G4endl;
	  nVoxDistLoop = MAXDISTVOXEL;
	}
	minGamma = gammaClosest; 
	for( G4int iloop = 1; iloop <= nVoxDistLoop; iloop++ ) {
	  for( int ilx = -std::min(iloop,nCVox2X); ilx <= std::min(iloop,G4int(nVoxX2-nCVox2X-1)); ilx++ ) {
	    for( int ilz = -std::min(iloop,nCVox2Z); ilz <= std::min(iloop,nVoxZ2-nCVox2Z-1); ilz++ ) {
	      if( fabs(ilx) != iloop && fabs(ilz) != iloop ) continue;
	      int copyNo2 = (ilx+nCVox2X) + nCVox2Y*nVoxX2 + (ilz+nCVox2Z)*nVoxXY2;
	      dataAtNv2 = iData2[copyNo2];     
	      // if( copyNo2 < 0 ) continue;
	      if( DicomVerb(testVerb) ) G4cout << iloop << "@@@ SEARCHING IMAGE2 OTHER VOXELS " << ilx << " " << ilz << " -> " << copyNo2 << G4endl;
	      double pc;
	      /*	      if( bErrors ) {
		if( doseErrors1.size() != 0 )  {
		doseErrAtNv1 = doseErrors1[nv1);
		} else {
		  doseErrAtNv1 = 0.;
		}
		if( doseErrors2.size() != 0 )  {
		  doseErrAtNv2 = doseErrors2[copyNo2);
		} else {
		  doseErrAtNv2 = 0.;
		}
		dataAtNv1 > dataAtNv2 ? 
		  pc = std::max((dataAtNv1-doseErrAtNv1*theNErrorSigmas) - (dataAtNv2+doseErrAtNv2*theNErrorSigmas),G4double(0.))*100./maxDosePC :
		  pc = std::max((dataAtNv2-doseErrAtNv2*theNErrorSigmas) - (dataAtNv1+doseErrAtNv1*theNErrorSigmas),G4double(0.))*100./maxDosePC;
		  } else { */
	      pc = fabs(dataAtNv1-dataAtNv2)*100./maxDosePC;
		//	      }
	      
	      G4ThreeVector voxCent2( fMinX2+(ilx+nCVox2X)*fVoxDimX2, fMinY2+nCVox2Y*fVoxDimY2, fMinZ2+(ilz+nCVox2Z)*fVoxDimZ2 );
	      double dist;
	      if( bDistanceByVoxel ) {
		dist = sqrt(fabs(ilx)*fVoxDimX2*fabs(ilx)*fVoxDimX2 + fabs(ilz)*fVoxDimZ2*fabs(ilz)*fVoxDimZ2);
	      } else {
		dist = (voxelCentre1-voxCent2).mag();
	      }
	      double gamma = sqrt(std::pow(pc/perCentLim,2)+std::pow(dist/minDistLim,2));
	      if( DicomVerb(testVerb) ) G4cout << ilx << ":" << ilz << "@@@ PERCENT " << pc << " " << dataAtNv1 << " - " << dataAtNv2 << G4endl;
	      if( DicomVerb(testVerb) ) G4cout << ilx << ":" << ilz << "@@@ DISTANCE " << dist << " " << voxelCentre1 << " - " << voxCent2 << G4endl; 
	      minGamma = std::min(minGamma,gamma);
	      if( DicomVerb(testVerb) ) G4cout << ilx << ":" << ilz << "@@@ MINGAMMMA " << minGamma << " GAMMA " << gamma << " : PC= " << pc << " DIST= " << dist << G4endl;
	    }
	  }
	}
      } else { // bInterpolateVoxels 
	
	// minimum and maximum of voxel1
	G4double minVX1 = fMinX1+(nvx1)*fVoxDimX1;
	G4double maxVX1 = fMinX1+(nvx1+1)*fVoxDimX1;
	G4double minVY1 = fMinY1+(nvy1)*fVoxDimY1;
	G4double maxVY1 = fMinY1+(nvy1+1)*fVoxDimY1;
	G4double minVZ1 = fMinZ1+(nvz1)*fVoxDimZ1;
	G4double maxVZ1 = fMinZ1+(nvz1+1)*fVoxDimZ1;
	if( DicomVerb(testVerb) ) G4cout << " min/max voxel 1 " << minVX1 << " " << maxVX1 << " " <<  minVY1 << " " << maxVY1 << " " << minVZ1 << " " << maxVZ1 << G4endl;
	// get voxels in sqdose2 included in voxel1
	G4int nVXmin2 = GmGenUtils::GetBelowInt((minVX1-fMinX2)/fVoxDimX2);
	G4int nVXmax2 = GmGenUtils::GetBelowInt((maxVX1-fMinX2)/fVoxDimX2);
	G4int nVYmin2 = GmGenUtils::GetBelowInt((minVY1-fMinY2)/fVoxDimY2);
	G4int nVYmax2 = GmGenUtils::GetBelowInt((maxVY1-fMinY2)/fVoxDimY2);
	G4int nVZmin2 = GmGenUtils::GetBelowInt((minVZ1-fMinZ2)/fVoxDimZ2);
	G4int nVZmax2 = GmGenUtils::GetBelowInt((maxVZ1-fMinZ2)/fVoxDimZ2);
	if( DicomVerb(testVerb) ) G4cout << " min/max voxel 2 "<< fMinX2+(nVXmin2)*fVoxDimX2 << " "<< fMinX2+(nVXmax2)*fVoxDimX2 << " "<< fMinY2+(nVYmin2)*fVoxDimY2 << " "<< fMinY2+(nVYmax2)*fVoxDimY2 << " "<< fMinZ2+(nVZmin2)*fVoxDimZ2 << " " << fMinZ2+(nVZmax2)*fVoxDimZ2 << G4endl;
	if( nVXmin2 < 0 ) nVXmin2 = 0;
	if( nVXmax2 < 0 ) continue;
	if( nVXmin2 >= G4int(nVoxX2) ) continue;
	if( nVXmax2 >= G4int(nVoxX2) ) nVXmax2 = nVoxX2-1;
	if( nVYmin2 < 0 ) nVYmin2 = 0;
	if( nVYmax2 < 0 ) continue;
	if( nVYmin2 >= nVoxY2 ) continue;
	if( nVYmax2 >= nVoxY2 ) nVYmax2 = nVoxY2-1;
	if( nVZmin2 < 0 ) nVZmin2 = 0;
	if( nVZmax2 < 0 ) continue;
	if( nVZmin2 >= nVoxZ2 ) continue;
	if( nVZmax2 >= nVoxZ2 ) nVZmax2 = nVoxZ2-1;
	
	// Get distance and dose 
	//--- Get gamma from percent and distance
	// Get dose form distribution in voxels
	G4double dose2 = 0.;
	G4double volu2 = 0.;
	if( DicomVerb(testVerb) ) G4cout << " LOOPING X " << nVXmin2 << ":" << nVXmax2 << " Y " << nVYmin2 << ":" << nVYmax2 << " Z " << nVZmin2 << ":" << nVZmax2 << " centre " << voxelCentre1 << G4endl;
	for( int ix = nVXmin2; ix <= nVXmax2; ix++ ) {
	  for( int iy = nVYmin2; iy <= nVYmax2; iy++ ) {
	    for( int iz = nVZmin2; iz <= nVZmax2; iz++ ) {
	      G4double vWmin = std::max(minVX1,fMinX2+(ix)*fVoxDimX2);
	      G4double vWmax = std::min(maxVX1,fMinX2+(ix+1)*fVoxDimX2);
	      G4double propX = (vWmax-vWmin)/fVoxDimX1;
	      if( DicomVerb(testVerb) ) G4cout << " X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propX << G4endl;
	      vWmin = std::max(minVY1,fMinY2+(iy)*fVoxDimY2);
	      vWmax = std::min(maxVY1,fMinY2+(iy+1)*fVoxDimY2);
	      G4double propY = (vWmax-vWmin)/fVoxDimY1;
	      if( DicomVerb(testVerb) ) G4cout << " X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propY << G4endl;
	      vWmin = std::max(minVZ1,fMinZ2+(iz)*fVoxDimZ2);
	      vWmax = std::min(maxVZ1,fMinZ2+(iz+1)*fVoxDimZ2);
	      G4double propZ = (vWmax-vWmin)/fVoxDimZ1;
	      if( DicomVerb(testVerb) ) G4cout << " X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propZ << G4endl;
	      
	      volu2 += propX*propY*propZ;
	      int nv2 = ix + iy*nVoxX2 + iz*nVoxXY2;
	      G4double dataAtNv2 = iData2[nv2];
	      dose2 += propX*propY*propZ * dataAtNv2;
	      if( DicomVerb(testVerb) ) G4cout << " DOSE PROP " << ix << ":" << iy << ":" << iz << " :" << nv2 << " dose " << dose2 << " volu " << volu2 << G4endl;
	    }
	  }
	}
	
	perCent = fabs(dataAtNv1-dose2)*100./maxDosePC;
	double distance = 0.; // it is the same voxel
	if( DicomVerb(debugVerb) ) G4cout << "@@@ PERCENT CLOSEST " << perCent << " = (" << dataAtNv1 << " - " << dose2 << ")*100/ " << maxDosePC << " err1 "  << doseErrAtNv1*theNErrorSigmas << " err2 " << doseErrAtNv2*theNErrorSigmas << G4endl; 
	if( DicomVerb(debugVerb) ) G4cout << "@@@ DISTANCE CLOSEST " << distance << G4endl;
	gammaClosest = sqrt(std::pow(perCent/perCentLim,2)+std::pow(distance/minDistLim,2));
	if( DicomVerb(debugVerb) ) G4cout << "@@@ GAMMA CLOSEST " << gammaClosest << G4endl;
	
	double distLoop = gammaClosest*minDistLim; // further than this it will make gamma bigger
	G4int nVoxDistLoop = std::max(distLoop/fVoxDimX1,distLoop/fVoxDimZ1); // maximum number of voxels in X or Z
	//      nVoxDistLoop = 0;
	if( DicomVerb(debugVerb) ) G4cout << "@@@ DISTANCE TO LOOP " << distLoop << " ->NVox " << nVoxDistLoop << G4endl;
	if( nVoxDistLoop > MAXDISTVOXEL ) {
	  G4cerr << "!! WARNING: TOO BIG GAMMA CLOSEST " << nVoxDistLoop << " > " << MAXDISTVOXEL << " " << gammaClosest << " LOOPING VOXEL " << nv1 << " = " << nvx1 << " " << nvy1 << " " << nvz1 << " -> " << voxelCentre1 << G4endl;	
	  if( DicomVerb(debugVerb) ) G4cout << "@@@ PERCENT CLOSEST " << perCent << " = (" << dataAtNv1 << " - " << dose2 << ")*100/ " << maxDosePC << " err1 "  << doseErrAtNv1*theNErrorSigmas << " err2 " << doseErrAtNv2*theNErrorSigmas << G4endl;
	  nVoxDistLoop = MAXDISTVOXEL;
	}
	minGamma = gammaClosest;
	
	for( G4int iloop = 1; iloop <= nVoxDistLoop; iloop++ ) {
	  for( int ilx = -std::min(iloop,nvx1); ilx <= std::min(iloop,nVoxX1-nvx1-1); ilx++ ) { // do not loop out of borders, i.e. iloop < nvx1 , iloop >= nVoxX1-nvx1
	    for( int ilz = -std::min(iloop,nvz1); ilz <= std::min(iloop,nVoxZ1-nvz1-1); ilz++ ) {
	      if( fabs(ilx) != iloop && fabs(ilz) != iloop ) continue;
	      // minimum and maximum of voxel1
	      /*	      G4double minVX1 = fMinX1+(nvx1+ilx)*fVoxDimX1;
			      G4double maxVX1 = fMinX1+(nvx1+ilx+1)*fVoxDimX1;
			      G4double minVY1 = fMinY1+(nvy1+ilx)*fVoxDimY1;
			      G4double maxVY1 = fMinY1+(nvy1+ilx+1)*fVoxDimY1;
			      if( DicomVerb(testVerb) ) G4cout << "O min/max voxel 1 " << minVX1 << " " << maxVX1 << " " <<  minVY1 << " " << maxVY1 << " " << minVZ1 << " " << maxVZ1 << G4endl;
	      // get voxels in sqdose2 included in voxel1
	      G4int nVXmin2 = GmGenUtils::GetBelowInt((minVX1-fMinX2)/fVoxDimX2);
	      G4int nVXmax2 = GmGenUtils::GetBelowInt((maxVX1-fMinX2)/fVoxDimX2);
	      G4int nVYmin2 = GmGenUtils::GetBelowInt((minVY1-fMinY2)/fVoxDimY2);
	      G4int nVYmax2 = GmGenUtils::GetBelowInt((maxVY1-fMinY2)/fVoxDimY2);
	      G4int nVZmin2 = GmGenUtils::GetBelowInt((minVZ1-fMinZ2)/fVoxDimZ2);
	      G4int nVZmax2 = GmGenUtils::GetBelowInt((maxVZ1-fMinZ2)/fVoxDimZ2);
	      if( DicomVerb(testVerb) ) G4cout << "O min/max voxel 2 "<< fMinX2+(nVXmin2)*fVoxDimX2 << " "<< fMinX2+(nVXmax2)*fVoxDimX2 << " "<< fMinY2+(nVYmin2)*fVoxDimY2 << " "<< fMinY2+(nVYmax2)*fVoxDimY2 << " "<< fMinZ2+(nVZmin2)*fVoxDimZ2 << " " << fMinZ2+(nVZmax2)*fVoxDimZ2 << G4endl; */
	      if( nVXmin2 < 0 ) nVXmin2 = 0;
	      if( nVXmax2 < 0 ) continue;
	      if( nVXmin2 >= nVoxX2 ) continue;
	      if( nVXmax2 >= nVoxX2 ) nVXmax2 = nVoxX2-1;
	      if( nVYmin2 < 0 ) nVYmin2 = 0;
	      if( nVYmax2 < 0 ) continue;
	      if( nVYmin2 >= nVoxY2 ) continue;
	      if( nVYmax2 >= nVoxY2 ) nVYmax2 = nVoxY2-1;
	      if( nVZmin2 < 0 ) nVZmin2 = 0;
	      if( nVZmax2 < 0 ) continue;
	      if( nVZmin2 >= nVoxZ2 ) continue;
	      if( nVZmax2 >= nVoxZ2 ) nVZmax2 = nVoxZ2-1;
	      
	      // Get distance and dose 
	      //--- Get gamma from percent and distance
	      // Get dose form distribution in vozels
	      dose2 = 0.;
	      volu2 = 0.;
	      if( DicomVerb(testVerb) ) G4cout << "O LOOPING X " << nVXmin2 << ":" << nVXmax2 << " Y " << nVYmin2 << ":" << nVYmax2 << " Z " << nVZmin2 << ":" << nVZmax2 << " centre " << voxelCentre1 << G4endl;
	      for( int ix = nVXmin2; ix <= nVXmax2; ix++ ) {
		for( int iy = nVYmin2; iy <= nVYmax2; iy++ ) {
		  for( int iz = nVZmin2; iz <= nVZmax2; iz++ ) {
		    G4double vWmin = std::max(minVX1,fMinX2+(ix)*fVoxDimX2);
		    G4double vWmax = std::min(maxVX1,fMinX2+(ix+1)*fVoxDimX2);
		    G4double propX = (vWmax-vWmin)/fVoxDimX1;
		    if( DicomVerb(testVerb) ) G4cout << "O X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propX << G4endl;
		    vWmin = std::max(minVY1,fMinY2+(iy)*fVoxDimY2);
		    vWmax = std::min(maxVY1,fMinY2+(iy+1)*fVoxDimY2);
		    G4double propY = (vWmax-vWmin)/fVoxDimY1;
		    if( DicomVerb(testVerb) ) G4cout << "O X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propY << G4endl;
		    vWmin = std::max(minVZ1,fMinZ2+(iz)*fVoxDimZ2);
		    vWmax = std::min(maxVZ1,fMinZ2+(iz+1)*fVoxDimZ2);
		    G4double propZ = (vWmax-vWmin)/fVoxDimZ1;
		    if( DicomVerb(testVerb) ) G4cout << "O X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propZ << G4endl;
		    
		    volu2 += propX*propY*propZ;
		    int nv2 = ix + iy*nVoxX2 + iz*nVoxXY2;
		    G4double dataAtNv2 = iData2[nv2];
		    dose2 += propX*propY*propZ * dataAtNv2;
		    if( DicomVerb(testVerb) ) G4cout << "O DOSE PROP " << ix << ":" << iy << ":" << iz << " :" << nv2 << " dose " << dose2 << " volu " << volu2 << G4endl; 
		  }
		}
	      }
	      
	      double pc = fabs(dataAtNv1-dose2)*100./maxDosePC;
	      
	      G4ThreeVector voxCentN( fMinX1+(ilx+nvx1)*fVoxDimX1, fMinY1+nvy1*fVoxDimY1, fMinZ1+(ilz+nvz1)*fVoxDimZ1 );
	      double dist;
	      if( bDistanceByVoxel ) {
		dist = sqrt(fabs(ilx)*fVoxDimX1*fabs(ilx)*fVoxDimX1 + fabs(ilz)*fVoxDimZ1*fabs(ilz)*fVoxDimZ1);
	      } else {
		dist = (voxelCentre1-voxCentN).mag();
	      }
	      double gamma = sqrt(std::pow(pc/perCentLim,2)+std::pow(dist/minDistLim,2));
	      if( DicomVerb(testVerb) ) G4cout << ilx << ":" << ilz << "@@@ PERCENT " << pc << " " << dataAtNv1 << " - " << dose2 << G4endl;
	      if( DicomVerb(testVerb) ) G4cout << ilx << ":" << ilz << "@@@ DISTANCE " << dist << " " << voxelCentre1 << " - " << voxCentN << G4endl; 
	      minGamma = std::min(minGamma,gamma);
	      if( DicomVerb(testVerb) ) G4cout << ilx << ":" << ilz << "@@@ MINGAMMMA " << minGamma << " GAMMA " << gamma << " : PC= " << pc << " DIST= " << dist << G4endl;
	    }
	  }
	}
      }
           
      if( bHisto1D ) {
	histoGI->Fill(minGamma);
      }
      iDataOut->at(nv1) = minGamma;
      //      if( minGamma != 0 ) G4cout << " FILL iDataOut " << nv1 << ": " << iDataOut->at(nv1) << "= " <<minGamma << G4endl; //GDEB
      //      histoXSvsGI->Fill(voxelCentre1.x(), voxelCentre1.z(),minGamma);
      if( minGamma > 1. ) nGI++;
      nPOINTS++;
      if( DicomVerb(debugVerb) ) std::cout << "FINAL gamma " << voxelCentre1 << " = " << minGamma << std::endl;
      theGammaFinal.insert(std::multiset<double>::value_type(minGamma));
    }

    //----- PLOT GI
    //    pad1->cd(); 
    if( bHisto1D ) {
      gStyle->SetOptStat(1111111);
      histoGI->Draw("colz");
    }
    double gammaPVal = 0.;
    std::multiset<double>::const_iterator iteg;
    int jj = 0;
    G4int nGamma = theGammaFinal.size();
    for( iteg = theGammaFinal.begin(); iteg != theGammaFinal.end(); iteg++, jj++ ) {	
      if( DicomVerb(debugVerb) ) std::cout << jj << " PVAL " << G4double(jj)/nGamma << " = " << *iteg << std::endl; 
      if( G4double(jj)/nGamma > 0.95 ) {
	gammaPVal = *iteg;
	break;
      }
    }
    if( bHisto1D ) {
      G4double mean = histoGI->GetMean();
      mean = int(mean*100)/100.;
      G4double rms = histoGI->GetRMS(); 
      rms = int(rms*100)/100.;
      if( DicomVerb(debugVerb) ) std::cout << perCentLim << "% / " << minDistLim << "mm : MEAN GAMMA INDEX " << histoGI->GetMean() << " +- " << histoGI->GetRMS() << " P" << pval*100 << "= " << gammaPVal <<  std::endl;
      double xInit = 375*canvas->GetWw()/500;
      //    double xInit = nvy1*100*canvas->GetWw()/500;
      double yInit = 290*canvas->GetWw()/500;
      //    double yInit= nvy1*100*canvas->GetWh()/500;
      EColor color = kBlack;
      //    pad1->cd(); 
      DrawWordInPave( histoGI, "G" + GmGenUtils::ftoa(perCentLim)+"/"+GmGenUtils::ftoa(minDistLim)+ ": " + GmGenUtils::ftoa(mean) + "+-" + GmGenUtils::ftoa(rms) + " P" + GmGenUtils::itoa(pval*100) + "= " + GmGenUtils::ftoa(int(gammaPVal*100)/100.), xInit, yInit, color, 0.02 );
      canvas->Print(("his"+G4String(histoGI->GetName())+".gif").c_str());
    }
   //    pad1->cd(); 
    /*    theHistosXSvsGI[yMin+fVoxDimY1/2.] = histoXSvsGI;
	  if( !bMaxAtPoint && !bMaxGammaValue ) {
	  gStyle->SetOptStat(0);
	  histoXSvsGI->Draw("colz");
	  canvas->Print(("his"+G4String(histoXSvsGI->GetName())+".gif").c_str());
	  } */
  }
    
  /*  //---- Print histos 2D after normalization at Isocentre
  if( bMaxAtPoint || bMaxGammaValue ) {
    if( bMaxAtPoint ) SetHistoAxisMaxAtPoint(theHistosXSvsGI, theMaxAtPoint, "XZ");
    if( bMaxGammaValue ) SetHistoMaxValue(theHistosXSvsGI, theMaxGammaValue);
    std::map<G4double,TH2F*>::const_iterator iteh;
    for( iteh = theHistosXSvsGI.begin(); iteh != theHistosXSvsGI.end(); iteh++ ) {
      //      pad1->cd();
      TH2F* histo = (*iteh).second; 
      gStyle->SetOptStat(0);
      histo->Draw("colz");
      canvas->Print(("his"+G4String(histo->GetName())+".gif").c_str());
    }
  }*/

  /*  //---- Print histos per Structure
  if( bPerSt ) {
    G4int nVoxelX_dcm = theDicomReaderCT->GetNoVoxelsX();
    G4int nVoxelY_dcm = theDicomReaderCT->GetNoVoxelsY();
    G4int nVoxelXY_dcm = nVoxelX_dcm*nVoxelY_dcm;
    G4int nVoxelZ_dcm = theDicomReaderCT->GetNoVoxelsZ();
    G4double fFVoxDimX_dcm = theDicomReaderCT->GetVoxelHalfX()*2;
    G4double fFVoxDimY_dcm = theDicomReaderCT->GetVoxelHalfY()*2;
    G4double fFVoxDimZ_dcm = theDicomReaderCT->GetVoxelHalfZ()*2;
    G4double fMinX_dcm = theDicomReaderCT->GetMinX();
    G4double fMinY_dcm = theDicomReaderCT->GetMinY();
    G4double fMinZ_dcm = theDicomReaderCT->GetMinZ();
    
    if( theHistosXSvsGI.size() == 0 ) {
      G4Exception(theExeName,
		  "",
		  FatalException,
		  "No gamma index histogram at any Z plane");
    }
    std::map<G4double,TH2F*>::const_iterator iteh0 = theHistosXSvsGI.begin();
    
    TAxis* xAxis0 = (*iteh0).second->GetXaxis();
    TAxis* yAxis0 = (*iteh0).second->GetYaxis();
    //  G4int nVoxelX_Dose = nVoxX1X2;
    //  G4int nVoxelY_Dose = nVoxY1Y2;
    //  G4int nVoxelXY_Dose = nVoxelX_Dose*nVoxelY_Dose;
    //  G4int nVoxelZ_Dose = theHistosXSvsGI.size();
    //  G4int nVoxelXYZ_Dose = nVoxelXY_Dose*nVoxelZ_Dose;
    G4double fFVoxDimX_Dose = xAxis0->GetBinWidth(1); //(fMaxX1X2-fMaxX1X2)/nVoxX1X2
    G4double fFVoxDimY_Dose = yAxis0->GetBinWidth(1);
    G4double fFVoxDimZ_Dose = fVoxDimZ1;
    G4double fMinX_Dose = fMinX1X2;
    G4double fMinY_Dose = fMinY1Y2;
    G4double fMinZ_Dose = (*iteh0).first-fVoxDimZ1/2.;
    
    std::map<G4int,G4int> theGISmallPerSt;
    std::map<G4int,G4int> theNVoxelsPerSt;
    std::map<G4double,TH2F*>::const_iterator iteh;
    G4int iz_Dose = 0;
    GmReadPhantomStMgr* phantomStMgr = GmReadPhantomStMgr::GetInstance();
    std::map<G4int,G4String> theStructs = phantomStMgr->GetStructs();

    for( iteh = theHistosXSvsGI.begin(); iteh != theHistosXSvsGI.end(); iteh++, iz_Dose++) {
      TH2F* histo2 = (*iteh).second;
      TAxis* xAxis = histo2->GetXaxis();
      TAxis* yAxis = histo2->GetYaxis();
      if( DicomVerb(debugVerb) ) G4cout << " PERST HISTO " << histo2->GetName() << " " << (*iteh).first << G4endl;
      for( int ix = 1; ix < xAxis->GetNbins(); ix++ ) {
	for( int iy = 1; iy < yAxis->GetNbins(); iy++ ) {
	  G4int ix_Dose = ix;
	  G4int iy_Dose = iy;
	  G4double X_Dose = fMinX_Dose+fFVoxDimX_Dose*(ix_Dose+0.5);
	  G4double Y_Dose = fMinY_Dose+fFVoxDimY_Dose*(iy_Dose+0.5);
	  G4double Z_Dose = fMinZ_Dose+fFVoxDimZ_Dose*(iz_Dose+0.5);
	  G4int ix_dcm = GmGenUtils::GetBelowInt((X_Dose-fMinX_dcm)/fFVoxDimX_dcm-0.01);
	  G4int iy_dcm = GmGenUtils::GetBelowInt((Y_Dose-fMinY_dcm)/fFVoxDimY_dcm-0.01);
	  G4int iz_dcm = GmGenUtils::GetBelowInt((Z_Dose-fMinZ_dcm)/fFVoxDimZ_dcm-0.01);
	  if( ix_dcm < 0 || ix_dcm >= G4int(nVoxelX_dcm) || 
	      iy_dcm < 0 || iy_dcm >= G4int(nVoxelY_dcm) ||
	      iz_dcm < 0 || iz_dcm >= G4int(nVoxelZ_dcm) ) {
	    continue;
	  }
	  G4int iiSt = ix_dcm +iy_dcm*nVoxelX_dcm+iz_dcm*nVoxelXY_dcm;
	  std::vector<G4int> ists = phantomStMgr->GetStIDList(iiSt);
	  for( G4int jj = 0; jj < ists.size(); jj++ ) {
	    if( histo2->GetBinContent(ix,iy) == 0. ) continue;
	    G4int ist = G4int(ists[jj]);
	    if( histo2->GetBinContent(ix,iy) <= 1. ) theGISmallPerSt[ist]++;
	    theNVoxelsPerSt[ist]++;
	    if( DicomVerb(testVerb) ) G4cout << jj << " FILL PERST " << ix_dcm << " " << iy_dcm << " " << iz_dcm << " " << X_Dose << " " << Y_Dose << " " << Z_Dose << " ::: " << ist << " : " << theStructs[ist] << " = " << theGISmallPerSt[ist] << " / " << theNVoxelsPerSt[ist] << " GI " <<  histo2->GetBinContent(ix,iy) << G4endl;
	    // "ISO 54 GY (SBRT 4D)" 	    if( ist == 29 ) G4cout << theStructs[ist] << " " << jj << " FILL PERST " << ix_dcm << " " << iy_dcm << " " << iz_dcm << " " << X_Dose << " " << Y_Dose << " " << Z_Dose << " ::: " << ist << " : " << theStructs[ist] << " = " << theGISmallPerSt[ist] << " / " << theNVoxelsPerSt[ist] << " GI " <<  histo2->GetBinContent(ix,iy) << G4endl;
	  }
	}
      }
    }
    
    if( DicomVerb(testVerb) ) G4cout << " PERST HISTO ENDED " << G4endl;
    std::map<G4int,G4int>::const_iterator iteb = theGISmallPerSt.begin();
    std::map<G4int,G4int>::const_iterator iten = theNVoxelsPerSt.begin();
    TH1F* histoSt = new TH1F("GI<1","GI<1",theGISmallPerSt.size(), 0, theGISmallPerSt.size());
    G4int ii = 1;
    for( ; iteb != theGISmallPerSt.end(); iteb++, ii++ ) {
      G4int ist = (*iteb).first;
      G4String binLabel = theStructs[ist];
      if( binLabel == "" ) binLabel = "NO_STRUCT";
      histoSt->GetXaxis()->SetBinLabel(ii,binLabel.c_str());
      if( DicomVerb(debugVerb) ) G4cout << ii << " ist " << ist << " BINS LABEL " << binLabel << G4endl;
    }
    TCanvas* canvasSt = new TCanvas;
    // canvas->SetCanvasSize(theNCanvasPixels,theNCanvasPixels);
    canvasSt->SetCanvasSize(theNCanvasPixels,theNCanvasPixels);
    canvasSt->Draw();
    canvasSt->cd();
    histoSt->SetMaximum(100.);
    histoSt->Draw();
    ii = 0;
    for( iteb = theGISmallPerSt.begin() ; iteb != theGISmallPerSt.end(); iteb++, iten++, ii++ ) {
      G4double GIBval = G4double((*iteb).second)/G4double((*iten).second)*100.;
      TMarker* marker = new TMarker( ii+0.5, GIBval, 20);
      G4int ist = (*iteb).first;
      marker->SetMarkerSize(1.5);
      if( DicomVerb(testVerb) ) G4cout << ii << " ist " << ist << " GISmallPerSt " << theStructs[ist] << " : " << GIBval << " = "<< (*iteb).second << " / " <<(*iten).second<< G4endl; 
      marker->Draw();
    }
    std::string gifstr = "hisDCMGetGammaIndex_"+GmGenUtils::ftoa(perCentLim)+"_"+GmGenUtils::ftoa(minDistLim)+"_PerStructure";
    canvasSt->Print((gifstr+".gif").c_str());
  }
  */
}

//------------------------------------------------------------------------
void DCM2DOperGammaIndex::OperateYZ( DicomVImage* image1, DicomVImage* image2, double perCentLim, double minDistLim, double pval, DicomVImage* imageOut )
{
  G4bool bDistanceByVoxel = false;//  TODO

  G4bool bInterpolateVoxels = G4bool(theParamMgr->GetNumericValue("bInterpolateVoxels",0));
  //  if( bInterpolateVoxels && *image1 == *image2 ) { // if images have same dimensions, no interpolation is needed
  //  bInterpolateVoxels = false; 
  // }
  
  //----- Extract image1 data
  G4int nVoxX1 = image1->GetNoVoxelsX();
  G4int nVoxY1 = image1->GetNoVoxelsY();
  G4int nVoxXY1 = nVoxX1*nVoxY1;
  G4int nVoxZ1 = image1->GetNoVoxelsZ();
  G4int nVoxYZ1 = nVoxY1*nVoxZ1;
  G4int nVox1 = nVoxX1*nVoxY1*nVoxZ1;
  G4double fMinX1 = image1->GetMinX();
  G4double fMinY1 = image1->GetMinY();
  G4double fMinZ1 = image1->GetMinZ();
  G4double fMaxX1 = image1->GetMaxX();
  G4double fMaxY1 = image1->GetMaxY();
  G4double fMaxZ1 = image1->GetMaxZ();
  G4double fVoxDimX1 = image1->GetVoxelDimX();
  G4double fVoxDimY1 = image1->GetVoxelDimY();
  G4double fVoxDimZ1 = image1->GetVoxelDimZ();
  std::vector<G4double> iData1 = *(image1->GetData());
  std::vector<G4double>* iDataOut = imageOut->GetData();

  TH1F* histoGI; 
  //  TH2F* histoXSvsGI;
  double theHGILim = std::min(100.,double(MAXDISTVOXEL));
  TCanvas* canvas = new TCanvas();
  //  std::map<G4double,TH2F*> theHistosXSvsGI;
 
  /*  if( bImageEqual ) {
    DicomVImage* image2Eq;
    if( !theResizeOperator ) theResizeOperator = new DicomOperResize(1,"GIOperResize");
    theResizeOperator->Operate(image2Eq, nVoxX1, fMinX1, fMaxX1, nVoxY1, fMinY1, fMaxY1, nVoxZ1, fMinZ1, fMaxZ1);  // make image2 as image1
    }*/
  
  if( DicomVerb(debugVerb) ) G4cout << "@@@ START DCM2DOperGammaIndex " << perCentLim << " " << minDistLim << " " << pval << G4endl;

  /*  DicomVImage* imageErr1 = image1->GetErrorData();
  std::vector<DIfloat>* iDataErr1 = 0;
  if( imageErr1 ) iDataErr1 = imageErr1->GetData();
  */  
  if( DicomVerb(infoVerb) )
    std::cout << "@@@ READ image nVoxX/Y/Z " << nVoxX1 << " " << nVoxY1 << " " << nVoxZ1 << G4endl
	      << " fMinX/Y/Z " << fMinX1 << " " << fMinY1 << " " << fMinZ1 << G4endl
	      << " fMaxX/Y/Z " << fMaxX1 << " " << fMaxY1 << " " << fMaxZ1 << G4endl
	      << " fVoxDimX/Y/Z " << fVoxDimX1 << " " << fVoxDimY1 << " " << fVoxDimZ1 << G4endl
	      << " NDOSES " << nVox1 << G4endl;

  //----- Extract image2 data
  G4int nVoxX2 = image2->GetNoVoxelsX();
  G4int nVoxY2 = image2->GetNoVoxelsY();
  G4int nVoxZ2 = image2->GetNoVoxelsZ();
  G4int nVoxXY2 = nVoxX2*nVoxY2;
  G4int nVox2 = nVoxX2*nVoxY2*nVoxZ2;
  //  size_t nVoxZ2 = image2->GetNoVoxelsZ();
  G4double fMinX2 = image2->GetMinX();
  G4double fMinY2 = image2->GetMinY();
  G4double fMinZ2 = image2->GetMinZ();
  G4double fMaxX2 = image2->GetMaxX();
  G4double fMaxY2 = image2->GetMaxY();
  G4double fMaxZ2 = image2->GetMaxZ();
  G4double fVoxDimX2 = image2->GetVoxelDimX();
  G4double fVoxDimY2 = image2->GetVoxelDimY();
  G4double fVoxDimZ2 = image2->GetVoxelDimZ();
  std::vector<G4double> iData2 = *(image2->GetData());
  /*  DicomVImage* imageErr2 = image2->GetErrorData();
  std::vector<DIfloat>* iDataErr2 = 0;
  if( imageErr2 ) iDataErr2 = imageErr2->GetData();
  */
  if( DicomVerb(infoVerb) )
    std::cout << "@@@ READ sqdose2 nVoxX/Y/Z " << nVoxX2 << " " << nVoxY2 << " " << nVoxZ2 << G4endl
	      << " fMinX/Y/Z " << fMinX2 << " " << fMinY2 << " " << fMinZ2 << G4endl
	      << " fMaxX/Y/Z " << fMaxX2 << " " << fMaxY2 << " " << fMaxZ2 << G4endl
	      << " fVoxDimX/Y/Z " << fVoxDimX2 << " " << fVoxDimY2 << " " << fVoxDimZ2 << G4endl
	      << " NDOSES " << nVox2 << G4endl;

  //----- Start calculation
  G4int nGI = 0;
  G4int nPOINTS = 0;
  std::multiset<double> theGammaFinal; 

  G4double fVoxY1Y2 = (std::min(fMaxY1,fMaxY2)-std::max(fMinY1,fMinY2));
  G4int nVoxY1Y2 =  GmGenUtils::GetBelowInt(fVoxY1Y2/fVoxDimY1); 
  G4double hfVoxDimY1Y2 = nVoxY1Y2*fVoxDimY1;
  G4double diffhfVoxDimY1Y2 = hfVoxDimY1Y2 - fVoxY1Y2;
  if( DicomVerb(testVerb) ) G4cout << " fVoxY1Y2 " << fVoxY1Y2 << " nVoxY1Y2 " << nVoxY1Y2 << " hfVoxDimY1Y2 " <<hfVoxDimY1Y2 << " diffhfVoxDimY1Y2 " <<  diffhfVoxDimY1Y2 << G4endl;
  G4double fMinY1Y2 = std::max(fMinY1,fMinY2)-diffhfVoxDimY1Y2/2.;
  G4double fMaxY1Y2 = std::min(fMaxY1,fMaxY2)+diffhfVoxDimY1Y2/2.;
  G4double fVoxDimY1Y2 = (fMaxY1Y2-fMinY1Y2)/nVoxY1Y2;
  fMinY1Y2 = std::max(fMinY1Y2, GetMinAtVoxel(fMinY1, fMinY1Y2, fVoxDimY1Y2) );
  fMaxY1Y2 = std::min(fMaxY1Y2, GetMaxAtVoxel(fMaxY1, fMaxY1Y2, nVoxY1Y2, fVoxDimY1Y2) );
  nVoxY1Y2 = GmGenUtils::GetBelowInt((fMaxY1Y2-fMinY1Y2)/fVoxDimY1Y2); 

  G4double fVoxZ1Z2 = (std::min(fMaxZ1,fMaxZ2)-std::max(fMinZ1,fMinZ2));
  G4int nVoxZ1Z2 =  GmGenUtils::GetBelowInt(fVoxZ1Z2/fVoxDimZ1); 
  G4double hfVoxDimZ1Z2 = nVoxZ1Z2*fVoxDimZ1;
  G4double diffhfVoxDimZ1Z2 = hfVoxDimZ1Z2 - fVoxZ1Z2;
  if( DicomVerb(testVerb) ) G4cout << " fVoxZ1Z2 " << fVoxZ1Z2 << " nVoxZ1Z2 " << nVoxZ1Z2 << " hfVoxDimZ1Z2 " <<hfVoxDimZ1Z2 << " diffhfVoxDimZ1Z2 " <<  diffhfVoxDimZ1Z2 << G4endl;
  G4double fMinZ1Z2 = std::max(fMinZ1,fMinZ2)-diffhfVoxDimZ1Z2/2.;
  G4double fMaxZ1Z2 = std::min(fMaxZ1,fMaxZ2)+diffhfVoxDimZ1Z2/2.;
  G4double fVoxDimZ1Z2 = (fMaxZ1Z2-fMinZ1Z2)/nVoxZ1Z2;
  fMinZ1Z2 = std::max(fMinZ1Z2, GetMinAtVoxel(fMinZ1, fMinZ1Z2, fVoxDimZ1Z2) );
  fMaxZ1Z2 = std::min(fMaxZ1Z2, GetMaxAtVoxel(fMaxZ1, fMaxZ1Z2, nVoxZ1Z2, fVoxDimZ1Z2) );
  nVoxZ1Z2 = GmGenUtils::GetBelowInt((fMaxZ1Z2-fMinZ1Z2)/fVoxDimZ1Z2); 
  G4int ptExtN = G4int(theParamMgr->GetNumericValue("pointExtensionN",0));
  G4ThreeVector pointExtension(ptExtN*image1->GetVoxelDimX(),ptExtN*image1->GetVoxelDimY(),ptExtN*image1->GetVoxelDimZ());

  float maxDosePC = 0;
  if( thePerCentType == GIPCT_ImageMax ) {
    maxDosePC = image1->GetMaxValue();
    if( DicomVerb(debugVerb) ) G4cout << thePerCentType << " maxDosePC = maxDose1 " << maxDosePC << G4endl;
  } else if( thePerCentType == GIPCT_AtPoint ){
    maxDosePC = image1->GetValueAtPoint(thePointPerCent,pointExtension);
    if( DicomVerb(debugVerb) ) G4cout << thePerCentType << " maxDosePC = theMaxAtPoint1st " << maxDosePC << G4endl; 
  } else { // None
    maxDosePC = std::max(image1->GetMaxValue(),image2->GetMaxValue());
    if( DicomVerb(debugVerb) ) G4cout << thePerCentType << " maxDosePC = max(maxDose1,maxDose2) " << maxDosePC << G4endl;
  }
      
  //----- Loop to each X plane
  for( G4int nvx1 = 0; nvx1 < nVoxX1; nvx1++ ){
    if( DicomVerb(infoVerb) ) G4cout << " X PLANE " << nvx1 << G4endl; 
    G4double xMin = fMinX1 + nvx1*fVoxDimX1;
    G4double xMax = xMin + fVoxDimX1;
    if( xMin < fMinX1 || xMax > fMaxX1 ) continue;
    if( xMin < fMinX2 || xMax > fMaxX2 ) continue;
    
    G4String xstr = GmGenUtils::ftoa(xMin+fVoxDimX1/2.);
    std::string limstr = "GammaIndex_YZ_"+GmGenUtils::ftoa(perCentLim)+"_"+GmGenUtils::ftoa(minDistLim)+"_1D_"+xstr;
    
    if( bHisto1D ) {
      histoGI = new TH1F((limstr).c_str(),(limstr).c_str(),std::min(100.,theHGILim*10),0.,theHGILim);
    }
    //    histoXSvsGI = new TH2F(limstr.c_str(),limstr.c_str(),nVoxX1, fMinX1, fMinX1+nVoxX1*widthX1, nVoxY1, fMinY1, fMinY1+nVoxY1*widthY1);
    //t    histoXSvsGI = new TH2F(limstr.c_str(),limstr.c_str(),nVoxX1X2, std::max(fMinX1,fMinX2), std::min(fMaxX1,fMaxX2), nVoxY1Y2,  std::max(fMinY1,fMinY2), std::min(fMaxY1,fMaxY2));
    //    histoXSvsGI = new TH2F((limstr+"_2D").c_str(),(limstr+"_2D").c_str(),nVoxY1Y2, fMinY1Y2, fMaxY1Y2, nVoxZ1Z2, fMinZ1Z2, fMaxZ1Z2);
    
    float doseErrAtNv1 = 0.;
    float doseErrAtNv2 = 0.;
    
    //----- Loop in XY plane
    /*    for( G4int nv1 = 0; nv1 < nVox1; nv1++ ){
	  G4double dataAtNv1 = iData1[nv1];
	  G4cout << nv1 << " TESTdata " << dataAtNv1 << " data2 " << iData2[nv1] << G4endl; //GDEB
	  } */
    for( G4int nvyz1 = 0; nvyz1 < nVoxYZ1; nvyz1++ ){
      G4int nvy1 = nvyz1%nVoxY1;
      G4int nvz1 = (nvyz1/nVoxY1)%nVoxZ1;
      G4int nv1 = nvz1*nVoxXY1+nvy1*nVoxX1+nvx1;
      
      G4double dataAtNv1 = iData1[nv1];
      //      G4double dataOutAtNv1 = iDataOut[nv1];
      G4double yMin = fMinY1 + nvy1*fVoxDimY1;     
      G4double yMax = yMin + fVoxDimY1;
      if( yMin < fMinY1 || yMax > fMaxY1 ) continue;
      if( yMin < fMinY2 || yMax > fMaxY2 ) continue;
      G4double zMin = fMinZ1 + nvz1*fVoxDimZ1;
      G4double zMax = zMin + fVoxDimZ1;
      if( zMin < fMinZ1 || zMax > fMaxZ1 ) continue;
      if( zMin < fMinZ2 || zMax > fMaxZ2 ) continue;      
      //    if( nz1 == 2 ) break;//GDEB
      //      G4int copyNo1GI = nvx1 + nvy1*nVoxX1 + nvz1*nVoxXY1; //--- voxel ID
          
      G4ThreeVector voxelCentre1( fMinX1+(nvx1+0.5)*fVoxDimX1, fMinY1+(nvy1+0.5)*fVoxDimY1, fMinZ1+(nvz1+0.5)*fVoxDimZ1 );
      
      if( DicomVerb(debugVerb) ) G4cout << "@@@ LOOPING VOXEL " << nv1 << " = " << nvx1 << " " << nvy1 << " " << nvz1 << " -> " << voxelCentre1 << G4endl;

      if( thePerCentType == GIPCT_Voxel ){
	maxDosePC = dataAtNv1;
	if( DicomVerb(debugVerb) ) G4cout << thePerCentType << " maxDosePC = dataVoxel1st " << maxDosePC << " " << nv1 << G4endl; 
      }
      //      G4cout << " maxDosePC = dataAtNv1 " << maxDosePC << " " << dataAtNv1 << " " << nv1 << G4endl; //GDEB
      
      double gammaClosest = 0; 
      double perCent;
      double minGamma; 
      if( !bInterpolateVoxels ) {
	// Get closest voxel in sqdose2
	G4int nCVox2X =  GmGenUtils::GetBelowInt((voxelCentre1.x()-fMinX2)/fVoxDimX2);
	if( nCVox2X < 0 || nCVox2X >= G4int(nVoxX2) ) {
	  if( DicomVerb(testVerb) ) G4cout << " CONTINUE VOXEL X NOT FOUND IN IMAGE2 " << nCVox2X << "< 0" << "||" << nCVox2X << ">=" << nVoxX2 << G4endl; 
	  continue;
	}
	G4int nCVox2Y = GmGenUtils::GetBelowInt((voxelCentre1.y()-fMinY2)/fVoxDimY2);
	if( nCVox2Y < 0 || nCVox2Y >= G4int(nVoxY2) ) {
	  //	  	  if( DicomVerb(testVerb) ) G4cout << "  CONTINUE VOXEL Y NOT FOUND IN IMAGE2 " << nCVox2Y << "< 0" << "||" << nCVox2Y << ">=" << nVoxY2 << G4endl;
	  continue;
	}
    
	G4int nCVox2Z = GmGenUtils::GetBelowInt((voxelCentre1.z()-fMinZ2)/fVoxDimZ2);
	if( nCVox2X < 0 || nCVox2X >= G4int(nVoxX2) ) {
	  G4Exception("DCM2DOperGammaIndex",
		      "",
		      FatalException,
		      "THIS SHOULD NOT HAPPEN");
	  continue;
	}
	// Get distance and dose 
	//--- Get gamma from percent and distance
	int nv2 = nCVox2X + nCVox2Y*nVoxX2 + nCVox2Z*nVoxXY2;
	G4double dataAtNv2 = iData2[nv2];
	if( dataAtNv1 == 0. || dataAtNv2 == 0. ) {
	  if( DicomVerb(testVerb) ) G4cout << " CONTINUE VOXEL DATA 0:  " << nv1 << " : " << dataAtNv1 << " " << nv2 << " : " << dataAtNv2 << G4endl; 
	  //-	  histoXSvsGI->Fill(voxelCentre1.x(), voxelCentre1.y(),log10(1.e-9));
	  //	  iDataOut->at(nv1) = log10(1.e-9);
	  continue; // not if voxel is 0 (SHOULD BE AN OPTION)
	}
	G4ThreeVector voxelCentre2( fMinX2+(nCVox2X+0.5)*fVoxDimX2, fMinY2+(nCVox2Y+0.5)*fVoxDimY2, fMinZ2+(nCVox2Z+0.5)*fVoxDimZ2 );
	if( DicomVerb(debugVerb) ) G4cout << "@@@ CLOSEST VOXEL2 " << nv2 << " = " << nCVox2X << " " << nCVox2Y << " " << nCVox2Z << " -> " << voxelCentre2 << G4endl;
	/*	if( doseErrors1.size() != 0 )  {
		doseErrAtNv1 = doseErrors1[nv1);
		} else {
		doseErrAtNv1 = 0.;
	}
	if( doseErrors2.size() != 0 )  {
	doseErrAtNv2 = doseErrors2[nv2);
	} else {
	  doseErrAtNv2 = 0.;
	  } */
	if( bErrors ) {
	  dataAtNv1 > dataAtNv2 ? 
	    perCent = std::max((dataAtNv1-doseErrAtNv1*theNErrorSigmas) - (dataAtNv2+doseErrAtNv2*theNErrorSigmas),G4double(0.))*100./maxDosePC :
	    perCent = std::max((dataAtNv2-doseErrAtNv2*theNErrorSigmas) - (dataAtNv1+doseErrAtNv1*theNErrorSigmas),G4double(0.))*100./maxDosePC;				 
	} else {
	  perCent = fabs(dataAtNv1-dataAtNv2)*100./maxDosePC;
	}
	if( DicomVerb(debugVerb) ) G4cout << "@@@ PERCENT CLOSEST " << perCent << " = (" << dataAtNv1 << " - " << dataAtNv2 << ")*100/ " << maxDosePC << " err1 "  << doseErrAtNv1*theNErrorSigmas << " err2 " << doseErrAtNv2*theNErrorSigmas << G4endl;
	//    double gammaPC = perCent/perCentLim;
	//      double distance = (voxelCentre1-voxelCentre2).mag();
	double distance;
	if( bDistanceByVoxel ) {
	  distance = 0;
	} else {
	  distance = (voxelCentre1-voxelCentre2).perp();
	}
	if( DicomVerb(debugVerb) ) G4cout << "@@@ DISTANCE CLOSEST " << distance << G4endl;
	gammaClosest = sqrt(std::pow(perCent/perCentLim,2)+std::pow(distance/minDistLim,2));
	
	if( DicomVerb(debugVerb) ) G4cout << "@@@ GAMMA CLOSEST " << gammaClosest << G4endl;
	
	double distLoop = gammaClosest*minDistLim; // further than this it will make gamma bigger
	G4int nVoxDistLoop = std::max(distLoop/fVoxDimY2,distLoop/fVoxDimZ2); // maximum number of voxels in Y or Z
	//      nVoxDistLoop = 0;
	if( DicomVerb(debugVerb) ) G4cout << "@@@ DISTANCE TO LOOP " << distLoop << " ->NVox " << nVoxDistLoop << "   " << fVoxDimY2 << " " <<fVoxDimZ2 << G4endl;
	if( nVoxDistLoop > MAXDISTVOXEL ) {
	  if( DicomVerb(debugVerb) )  G4cerr << "!! WARNING: TOO BIG GAMMA CLOSEST " << nVoxDistLoop << " > " << MAXDISTVOXEL << " " << gammaClosest << " SEARCHING IMAGE2 VOXEL " << nv1 << " = " << nvx1 << " " << nvy1 << " " << nvz1 << " -> " << voxelCentre1 << G4endl;	
	  if( DicomVerb(debugVerb) ) G4cout << "@@@ PERCENT CLOSEST " << perCent << " = (" << dataAtNv1 << " - " << dataAtNv2 << ")*100/ " << maxDosePC << " err1 "  << doseErrAtNv1*theNErrorSigmas << " err2 " << doseErrAtNv2*theNErrorSigmas << G4endl;
	  nVoxDistLoop = MAXDISTVOXEL;
	}
	minGamma = gammaClosest; 
	for( G4int iloop = 1; iloop <= nVoxDistLoop; iloop++ ) {
	  for( int ily = -std::min(iloop,nCVox2Y); ily <= std::min(iloop,G4int(nVoxY2-nCVox2Y-1)); ily++ ) {
	    for( int ilz = -std::min(iloop,nCVox2Z); ilz <= std::min(iloop,G4int(nVoxZ2-nCVox2Z-1)); ilz++ ) {
	      //	for( int ilx = -iloop; ilx <= iloop; ilx++ ) {
	      //  for( int ily = -iloop; ily <= iloop; ily++ ) {
	      if( fabs(ily) != iloop && fabs(ilz) != iloop ) continue;
	      int copyNo2 = nCVox2X + (ily+nCVox2Y)*nVoxX2 + (ilz+nCVox2Z)*nVoxXY2;
	      dataAtNv2 = iData2[copyNo2];     
	      // if( copyNo2 < 0 ) continue;
	      if( DicomVerb(testVerb) ) G4cout << iloop << "@@@ SEARCHING IMAGE2 OTHER VOXELS " << ily << " " << ilz << " -> " << copyNo2 << G4endl;
	      double pc;
	      /*	      if( bErrors ) {
		if( doseErrors1.size() != 0 )  {
		doseErrAtNv1 = doseErrors1[nv1);
		} else {
		  doseErrAtNv1 = 0.;
		}
		if( doseErrors2.size() != 0 )  {
		  doseErrAtNv2 = doseErrors2[copyNo2);
		} else {
		  doseErrAtNv2 = 0.;
		}
		dataAtNv1 > dataAtNv2 ? 
		  pc = std::max((dataAtNv1-doseErrAtNv1*theNErrorSigmas) - (dataAtNv2+doseErrAtNv2*theNErrorSigmas),G4double(0.))*100./maxDosePC :
		  pc = std::max((dataAtNv2-doseErrAtNv2*theNErrorSigmas) - (dataAtNv1+doseErrAtNv1*theNErrorSigmas),G4double(0.))*100./maxDosePC;
		  } else { */
	      pc = fabs(dataAtNv1-dataAtNv2)*100./maxDosePC;
		//	      }
	      
	      G4ThreeVector voxCent2( fMinX2+nCVox2X*fVoxDimX2, fMinY2+(ily+nCVox2Y)*fVoxDimY2, fMinZ2+(ilz+nCVox2Z)*fVoxDimZ2 );
	      double dist;
	      if( bDistanceByVoxel ) {
		dist = sqrt(fabs(ily)*fVoxDimY2*fabs(ily)*fVoxDimY2 + fabs(ilz)*fVoxDimZ2*fabs(ilz)*fVoxDimZ2);
	      } else {
		dist = (voxelCentre1-voxCent2).mag();
	      }
	      double gamma = sqrt(std::pow(pc/perCentLim,2)+std::pow(dist/minDistLim,2));
	      if( DicomVerb(testVerb) ) G4cout << ily << ":" << ilz << "@@@ PERCENT " << pc << " " << dataAtNv1 << " - " << dataAtNv2 << G4endl;
	      if( DicomVerb(testVerb) ) G4cout << ily << ":" << ilz << "@@@ DISTANCE " << dist << " " << voxelCentre1 << " - " << voxCent2 << G4endl; 
	      minGamma = std::min(minGamma,gamma);
	      if( DicomVerb(testVerb) ) G4cout << ily << ":" << ilz << "@@@ MINGAMMMA " << minGamma << " GAMMA " << gamma << " : PC= " << pc << " DIST= " << dist << G4endl;
	    }
	  }
	}
      } else { // bInterpolateVoxels 
	
	// minimum and maximum of voxel1
	G4double minVX1 = fMinX1+(nvx1)*fVoxDimX1;
	G4double maxVX1 = fMinX1+(nvx1+1)*fVoxDimX1;
	G4double minVY1 = fMinY1+(nvy1)*fVoxDimY1;
	G4double maxVY1 = fMinY1+(nvy1+1)*fVoxDimY1;
	G4double minVZ1 = fMinZ1+(nvz1)*fVoxDimZ1;
	G4double maxVZ1 = fMinZ1+(nvz1+1)*fVoxDimZ1;
	if( DicomVerb(testVerb) ) G4cout << " min/max voxel 1 " << minVX1 << " " << maxVX1 << " " <<  minVY1 << " " << maxVY1 << " " << minVZ1 << " " << maxVZ1 << G4endl;
	// get voxels in sqdose2 included in voxel1
	G4int nVXmin2 = GmGenUtils::GetBelowInt((minVX1-fMinX2)/fVoxDimX2);
	G4int nVXmax2 = GmGenUtils::GetBelowInt((maxVX1-fMinX2)/fVoxDimX2);
	G4int nVYmin2 = GmGenUtils::GetBelowInt((minVY1-fMinY2)/fVoxDimY2);
	G4int nVYmax2 = GmGenUtils::GetBelowInt((maxVY1-fMinY2)/fVoxDimY2);
	G4int nVZmin2 = GmGenUtils::GetBelowInt((minVZ1-fMinZ2)/fVoxDimZ2);
	G4int nVZmax2 = GmGenUtils::GetBelowInt((maxVZ1-fMinZ2)/fVoxDimZ2);
	if( DicomVerb(testVerb) ) G4cout << " min/max voxel 2 "<< fMinX2+(nVXmin2)*fVoxDimX2 << " "<< fMinX2+(nVXmax2)*fVoxDimX2 << " "<< fMinY2+(nVYmin2)*fVoxDimY2 << " "<< fMinY2+(nVYmax2)*fVoxDimY2 << " "<< fMinZ2+(nVZmin2)*fVoxDimZ2 << " " << fMinZ2+(nVZmax2)*fVoxDimZ2 << G4endl;
	if( nVXmin2 < 0 ) nVXmin2 = 0;
	if( nVXmax2 < 0 ) continue;
	if( nVXmin2 >= G4int(nVoxX2) ) continue;
	if( nVXmax2 >= G4int(nVoxX2) ) nVXmax2 = nVoxX2-1;
	if( nVYmin2 < 0 ) nVYmin2 = 0;
	if( nVYmax2 < 0 ) continue;
	if( nVYmin2 >= nVoxY2 ) continue;
	if( nVYmax2 >= nVoxY2 ) nVYmax2 = nVoxY2-1;
	if( nVZmin2 < 0 ) nVZmin2 = 0;
	if( nVZmax2 < 0 ) continue;
	if( nVZmin2 >= nVoxZ2 ) continue;
	if( nVZmax2 >= nVoxZ2 ) nVZmax2 = nVoxZ2-1;
	
	// Get distance and dose 
	//--- Get gamma from percent and distance
	// Get dose form distribution in voxels
	G4double dose2 = 0.;
	G4double volu2 = 0.;
	if( DicomVerb(testVerb) ) G4cout << " LOOPING X " << nVXmin2 << ":" << nVXmax2 << " Y " << nVYmin2 << ":" << nVYmax2 << " Z " << nVZmin2 << ":" << nVZmax2 << " centre " << voxelCentre1 << G4endl;
	for( int ix = nVXmin2; ix <= nVXmax2; ix++ ) {
	  for( int iy = nVYmin2; iy <= nVYmax2; iy++ ) {
	    for( int iz = nVZmin2; iz <= nVZmax2; iz++ ) {
	      G4double vWmin = std::max(minVX1,fMinX2+(ix)*fVoxDimX2);
	      G4double vWmax = std::min(maxVX1,fMinX2+(ix+1)*fVoxDimX2);
	      G4double propX = (vWmax-vWmin)/fVoxDimX1;
	      if( DicomVerb(testVerb) ) G4cout << " X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propX << G4endl;
	      vWmin = std::max(minVY1,fMinY2+(iy)*fVoxDimY2);
	      vWmax = std::min(maxVY1,fMinY2+(iy+1)*fVoxDimY2);
	      G4double propY = (vWmax-vWmin)/fVoxDimY1;
	      if( DicomVerb(testVerb) ) G4cout << " X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propY << G4endl;
	      vWmin = std::max(minVZ1,fMinZ2+(iz)*fVoxDimZ2);
	      vWmax = std::min(maxVZ1,fMinZ2+(iz+1)*fVoxDimZ2);
	      G4double propZ = (vWmax-vWmin)/fVoxDimZ1;
	      if( DicomVerb(testVerb) ) G4cout << " X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propZ << G4endl;
	      
	      volu2 += propX*propY*propZ;
	      int nv2 = ix + iy*nVoxX2 + iz*nVoxXY2;
	      G4double dataAtNv2 = iData2[nv2];
	      dose2 += propX*propY*propZ * dataAtNv2;
	      if( DicomVerb(testVerb) ) G4cout << " DOSE PROP " << ix << ":" << iy << ":" << iz << " :" << nv2 << " dose " << dose2 << " volu " << volu2 << G4endl;
	    }
	  }
	}
	
	perCent = fabs(dataAtNv1-dose2)*100./maxDosePC;
	double distance = 0.; // it is the same voxel
	if( DicomVerb(debugVerb) ) G4cout << "@@@ PERCENT CLOSEST " << perCent << " = (" << dataAtNv1 << " - " << dose2 << ")*100/ " << maxDosePC << " err1 "  << doseErrAtNv1*theNErrorSigmas << " err2 " << doseErrAtNv2*theNErrorSigmas << G4endl; 
	if( DicomVerb(debugVerb) ) G4cout << "@@@ DISTANCE CLOSEST " << distance << G4endl;
	gammaClosest = sqrt(std::pow(perCent/perCentLim,2)+std::pow(distance/minDistLim,2));
	if( DicomVerb(debugVerb) ) G4cout << "@@@ GAMMA CLOSEST " << gammaClosest << G4endl;
	
	double distLoop = gammaClosest*minDistLim; // further than this it will make gamma bigger
	G4int nVoxDistLoop = std::max(distLoop/fVoxDimY1,distLoop/fVoxDimZ1); // maximum number of voxels in X or Y
	//      nVoxDistLoop = 0;
	if( DicomVerb(debugVerb) ) G4cout << "@@@ DISTANCE TO LOOP " << distLoop << " ->NVox " << nVoxDistLoop << G4endl;
	if( nVoxDistLoop > MAXDISTVOXEL ) {
	  G4cerr << "!! WARNING: TOO BIG GAMMA CLOSEST " << nVoxDistLoop << " > " << MAXDISTVOXEL << " " << gammaClosest << " LOOPING VOXEL " << nv1 << " = " << nvx1 << " " << nvy1 << " " << nvz1 << " -> " << voxelCentre1 << G4endl;	
	  if( DicomVerb(debugVerb) ) G4cout << "@@@ PERCENT CLOSEST " << perCent << " = (" << dataAtNv1 << " - " << dose2 << ")*100/ " << maxDosePC << " err1 "  << doseErrAtNv1*theNErrorSigmas << " err2 " << doseErrAtNv2*theNErrorSigmas << G4endl;
	  nVoxDistLoop = MAXDISTVOXEL;
	}
	minGamma = gammaClosest;
	
	for( G4int iloop = 1; iloop <= nVoxDistLoop; iloop++ ) {
	  for( int ily = -std::min(iloop,nvy1); ily <= std::min(iloop,nVoxY1-nvy1-1); ily++ ) { // do not loop out of borders, i.e. iloop < nvy1 , iloop >= nVoxY1-nvy1
	    for( int ilz = -std::min(iloop,nvz1); ilz <= std::min(iloop,nVoxZ1-nvz1-1); ilz++ ) {
	      if( fabs(ily) != iloop && fabs(ilz) != iloop ) continue;
	      // minimum and maximum of voxel1
	      /*	      G4double minVX1 = fMinX1+(nvx1+ilx)*fVoxDimX1;
			      G4double maxVX1 = fMinX1+(nvx1+ilx+1)*fVoxDimX1;
			      G4double minVY1 = fMinY1+(nvy1+ilx)*fVoxDimY1;
			      G4double maxVY1 = fMinY1+(nvy1+ilx+1)*fVoxDimY1;
			      if( DicomVerb(testVerb) ) G4cout << "O min/max voxel 1 " << minVX1 << " " << maxVX1 << " " <<  minVY1 << " " << maxVY1 << " " << minVZ1 << " " << maxVZ1 << G4endl;
	      // get voxels in sqdose2 included in voxel1
	      G4int nVXmin2 = GmGenUtils::GetBelowInt((minVX1-fMinX2)/fVoxDimX2);
	      G4int nVXmax2 = GmGenUtils::GetBelowInt((maxVX1-fMinX2)/fVoxDimX2);
	      G4int nVYmin2 = GmGenUtils::GetBelowInt((minVY1-fMinY2)/fVoxDimY2);
	      G4int nVYmax2 = GmGenUtils::GetBelowInt((maxVY1-fMinY2)/fVoxDimY2);
	      G4int nVZmin2 = GmGenUtils::GetBelowInt((minVZ1-fMinZ2)/fVoxDimZ2);
	      G4int nVZmax2 = GmGenUtils::GetBelowInt((maxVZ1-fMinZ2)/fVoxDimZ2);
	      if( DicomVerb(testVerb) ) G4cout << "O min/max voxel 2 "<< fMinX2+(nVXmin2)*fVoxDimX2 << " "<< fMinX2+(nVXmax2)*fVoxDimX2 << " "<< fMinY2+(nVYmin2)*fVoxDimY2 << " "<< fMinY2+(nVYmax2)*fVoxDimY2 << " "<< fMinZ2+(nVZmin2)*fVoxDimZ2 << " " << fMinZ2+(nVZmax2)*fVoxDimZ2 << G4endl; */
	      if( nVXmin2 < 0 ) nVXmin2 = 0;
	      if( nVXmax2 < 0 ) continue;
	      if( nVXmin2 >= nVoxX2 ) continue;
	      if( nVXmax2 >= nVoxX2 ) nVXmax2 = nVoxX2-1;
	      if( nVYmin2 < 0 ) nVYmin2 = 0;
	      if( nVYmax2 < 0 ) continue;
	      if( nVYmin2 >= nVoxY2 ) continue;
	      if( nVYmax2 >= nVoxY2 ) nVYmax2 = nVoxY2-1;
	      if( nVZmin2 < 0 ) nVZmin2 = 0;
	      if( nVZmax2 < 0 ) continue;
	      if( nVZmin2 >= nVoxZ2 ) continue;
	      if( nVZmax2 >= nVoxZ2 ) nVZmax2 = nVoxZ2-1;
	      
	      // Get distance and dose 
	      //--- Get gamma from percent and distance
	      // Get dose form distribution in vozels
	      dose2 = 0.;
	      volu2 = 0.;
	      if( DicomVerb(testVerb) ) G4cout << "O LOOPING X " << nVXmin2 << ":" << nVXmax2 << " Y " << nVYmin2 << ":" << nVYmax2 << " Z " << nVZmin2 << ":" << nVZmax2 << " centre " << voxelCentre1 << G4endl;
	      for( int ix = nVXmin2; ix <= nVXmax2; ix++ ) {
		for( int iy = nVYmin2; iy <= nVYmax2; iy++ ) {
		  for( int iz = nVZmin2; iz <= nVZmax2; iz++ ) {
		    G4double vWmin = std::max(minVX1,fMinX2+(ix)*fVoxDimX2);
		    G4double vWmax = std::min(maxVX1,fMinX2+(ix+1)*fVoxDimX2);
		    G4double propX = (vWmax-vWmin)/fVoxDimX1;
		    if( DicomVerb(testVerb) ) G4cout << "O X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propX << G4endl;
		    vWmin = std::max(minVY1,fMinY2+(iy)*fVoxDimY2);
		    vWmax = std::min(maxVY1,fMinY2+(iy+1)*fVoxDimY2);
		    G4double propY = (vWmax-vWmin)/fVoxDimY1;
		    if( DicomVerb(testVerb) ) G4cout << "O X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propY << G4endl;
		    vWmin = std::max(minVZ1,fMinZ2+(iz)*fVoxDimZ2);
		    vWmax = std::min(maxVZ1,fMinZ2+(iz+1)*fVoxDimZ2);
		    G4double propZ = (vWmax-vWmin)/fVoxDimZ1;
		    if( DicomVerb(testVerb) ) G4cout << "O X Vminmax " << vWmin << " : " <<  vWmax << " prop " << propZ << G4endl;
		    
		    volu2 += propX*propY*propZ;
		    int nv2 = ix + iy*nVoxX2 + iz*nVoxXY2;
		    G4double dataAtNv2 = iData2[nv2];
		    dose2 += propX*propY*propZ * dataAtNv2;
		    if( DicomVerb(testVerb) ) G4cout << "O DOSE PROP " << ix << ":" << iy << ":" << iz << " :" << nv2 << " dose " << dose2 << " volu " << volu2 << G4endl; 
		  }
		}
	      }
	      
	      double pc = fabs(dataAtNv1-dose2)*100./maxDosePC;
	      
	      G4ThreeVector voxCentN( fMinX1+nvx1*fVoxDimX1, fMinY1+(ily+nvy1)*fVoxDimY1, fMinZ1+(ilz+nvz1)*fVoxDimZ1 );
	      double dist;
	      if( bDistanceByVoxel ) {
		dist = sqrt(fabs(ily)*fVoxDimY1*fabs(ily)*fVoxDimY1 + fabs(ilz)*fVoxDimZ1*fabs(ilz)*fVoxDimZ1);
	      } else {
		dist = (voxelCentre1-voxCentN).mag();
	      }
	      double gamma = sqrt(std::pow(pc/perCentLim,2)+std::pow(dist/minDistLim,2));
	      if( DicomVerb(testVerb) ) G4cout << ily << ":" << ilz << "@@@ PERCENT " << pc << " " << dataAtNv1 << " - " << dose2 << G4endl;
	      if( DicomVerb(testVerb) ) G4cout << ily << ":" << ilz << "@@@ DISTANCE " << dist << " " << voxelCentre1 << " - " << voxCentN << G4endl; 
	      minGamma = std::min(minGamma,gamma);
	      if( DicomVerb(testVerb) ) G4cout << ily << ":" << ilz << "@@@ MINGAMMMA " << minGamma << " GAMMA " << gamma << " : PC= " << pc << " DIST= " << dist << G4endl;
	    }
	  }
	}
      }
           
      if( bHisto1D ) {
	histoGI->Fill(minGamma);
      }
      iDataOut->at(nv1) = minGamma;
      //      histoXSvsGI->Fill(voxelCentre1.y(), voxelCentre1.z(),minGamma);
      if( minGamma > 1. ) nGI++;
      nPOINTS++;
      if( DicomVerb(debugVerb) ) std::cout << "FINAL gamma " << voxelCentre1 << " = " << minGamma << std::endl;
      theGammaFinal.insert(std::multiset<double>::value_type(minGamma));
    }

    //----- PLOT GI
    //    pad1->cd(); 
    if( bHisto1D ) {
      gStyle->SetOptStat(1111111);
      histoGI->Draw("colz");
    }
    double gammaPVal = 0.;
    std::multiset<double>::const_iterator iteg;
    int jj = 0;
    G4int nGamma = theGammaFinal.size();
    for( iteg = theGammaFinal.begin(); iteg != theGammaFinal.end(); iteg++, jj++ ) {	
      if( DicomVerb(debugVerb) ) std::cout << jj << " PVAL " << G4double(jj)/nGamma << " = " << *iteg << std::endl; 
      if( G4double(jj)/nGamma > 0.95 ) {
	gammaPVal = *iteg;
	break;
      }
    }
    if( bHisto1D ) {
      G4double mean = histoGI->GetMean();
      mean = int(mean*100)/100.;
      G4double rms = histoGI->GetRMS(); 
      rms = int(rms*100)/100.;
      if( DicomVerb(debugVerb) ) std::cout << perCentLim << "% / " << minDistLim << "mm : MEAN GAMMA INDEX " << histoGI->GetMean() << " +- " << histoGI->GetRMS() << " P" << pval*100 << "= " << gammaPVal <<  std::endl;
      //      xInit = 375*canvas->GetWw()/500;
      double xInit = nvx1*100*canvas->GetWw()/500;
      //      xInit = 290*canvas->GetWw()/500;
      double yInit= nvx1*100*canvas->GetWh()/500;
      EColor color = kBlack;
    //    pad1->cd(); 
      DrawWordInPave( histoGI, "G" + GmGenUtils::ftoa(perCentLim)+"/"+GmGenUtils::ftoa(minDistLim)+ ": " + GmGenUtils::ftoa(mean) + "+-" + GmGenUtils::ftoa(rms) + " P" + GmGenUtils::itoa(pval*100) + "= " + GmGenUtils::ftoa(int(gammaPVal*100)/100.), xInit, yInit, color, 0.02 );
      canvas->Print(("his"+G4String(histoGI->GetName())+".gif").c_str());
    }
   //    pad1->cd(); 
    /*    theHistosXSvsGI[xMin+fVoxDimX1/2.] = histoXSvsGI;
    if( !bMaxAtPoint && !bMaxGammaValue ) {
      gStyle->SetOptStat(0);
      histoXSvsGI->Draw("colz");
      canvas->Print(("his"+G4String(histoXSvsGI->GetName())+".gif").c_str());
      } */
  }
    
  /*  //---- Print histos 2D after normalization at Isocentre
  if( bMaxAtPoint || bMaxGammaValue ) {
    if( bMaxAtPoint ) SetHistoAxisMaxAtPoint(theHistosXSvsGI, theMaxAtPoint, "YZ");
    if( bMaxGammaValue ) SetHistoMaxValue(theHistosXSvsGI, theMaxGammaValue);
    std::map<G4double,TH2F*>::const_iterator iteh;
    for( iteh = theHistosXSvsGI.begin(); iteh != theHistosXSvsGI.end(); iteh++ ) {
      //      pad1->cd();
      TH2F* histo = (*iteh).second;
      gStyle->SetOptStat(0);
      histo->Draw("colz");
      canvas->Print(("his"+G4String(histo->GetName())+".gif").c_str());
    }
  }
  */
  /*  //---- Print histos per Structure
  if( bPerSt ) {
    G4int nVoxelX_dcm = theDicomReaderCT->GetNoVoxelsX();
    G4int nVoxelY_dcm = theDicomReaderCT->GetNoVoxelsY();
    G4int nVoxelXY_dcm = nVoxelX_dcm*nVoxelY_dcm;
    G4int nVoxelZ_dcm = theDicomReaderCT->GetNoVoxelsZ();
    G4double fFVoxDimX_dcm = theDicomReaderCT->GetVoxelHalfX()*2;
    G4double fFVoxDimY_dcm = theDicomReaderCT->GetVoxelHalfY()*2;
    G4double fFVoxDimZ_dcm = theDicomReaderCT->GetVoxelHalfZ()*2;
    G4double fMinX_dcm = theDicomReaderCT->GetMinX();
    G4double fMinY_dcm = theDicomReaderCT->GetMinY();
    G4double fMinZ_dcm = theDicomReaderCT->GetMinZ();
    
    if( theHistosXSvsGI.size() == 0 ) {
      G4Exception(theExeName,
		  "",
		  FatalException,
		  "No gamma index histogram at any Z plane");
    }
    std::map<G4double,TH2F*>::const_iterator iteh0 = theHistosXSvsGI.begin();
    
    TAxis* xAxis0 = (*iteh0).second->GetXaxis();
    TAxis* yAxis0 = (*iteh0).second->GetYaxis();
    //  G4int nVoxelX_Dose = nVoxX1X2;
    //  G4int nVoxelY_Dose = nVoxY1Y2;
    //  G4int nVoxelXY_Dose = nVoxelX_Dose*nVoxelY_Dose;
    //  G4int nVoxelZ_Dose = theHistosXSvsGI.size();
    //  G4int nVoxelXYZ_Dose = nVoxelXY_Dose*nVoxelZ_Dose;
    G4double fFVoxDimX_Dose = xAxis0->GetBinWidth(1); //(fMaxX1X2-fMaxX1X2)/nVoxX1X2
    G4double fFVoxDimY_Dose = yAxis0->GetBinWidth(1);
    G4double fFVoxDimZ_Dose = fVoxDimZ1;
    G4double fMinX_Dose = fMinX1X2;
    G4double fMinY_Dose = fMinY1Y2;
    G4double fMinZ_Dose = (*iteh0).first-fVoxDimZ1/2.;
    
    std::map<G4int,G4int> theGISmallPerSt;
    std::map<G4int,G4int> theNVoxelsPerSt;
    std::map<G4double,TH2F*>::const_iterator iteh;
    G4int iz_Dose = 0;
    GmReadPhantomStMgr* phantomStMgr = GmReadPhantomStMgr::GetInstance();
    std::map<G4int,G4String> theStructs = phantomStMgr->GetStructs();

    for( iteh = theHistosXSvsGI.begin(); iteh != theHistosXSvsGI.end(); iteh++, iz_Dose++) {
      TH2F* histo2 = (*iteh).second;
      TAxis* xAxis = histo2->GetXaxis();
      TAxis* yAxis = histo2->GetYaxis();
      if( DicomVerb(debugVerb) ) G4cout << " PERST HISTO " << histo2->GetName() << " " << (*iteh).first << G4endl;
      for( int ix = 1; ix < xAxis->GetNbins(); ix++ ) {
	for( int iy = 1; iy < yAxis->GetNbins(); iy++ ) {
	  G4int ix_Dose = ix;
	  G4int iy_Dose = iy;
	  G4double X_Dose = fMinX_Dose+fFVoxDimX_Dose*(ix_Dose+0.5);
	  G4double Y_Dose = fMinY_Dose+fFVoxDimY_Dose*(iy_Dose+0.5);
	  G4double Z_Dose = fMinZ_Dose+fFVoxDimZ_Dose*(iz_Dose+0.5);
	  G4int ix_dcm = GmGenUtils::GetBelowInt((X_Dose-fMinX_dcm)/fFVoxDimX_dcm-0.01);
	  G4int iy_dcm = GmGenUtils::GetBelowInt((Y_Dose-fMinY_dcm)/fFVoxDimY_dcm-0.01);
	  G4int iz_dcm = GmGenUtils::GetBelowInt((Z_Dose-fMinZ_dcm)/fFVoxDimZ_dcm-0.01);
	  if( ix_dcm < 0 || ix_dcm >= G4int(nVoxelX_dcm) || 
	      iy_dcm < 0 || iy_dcm >= G4int(nVoxelY_dcm) ||
	      iz_dcm < 0 || iz_dcm >= G4int(nVoxelZ_dcm) ) {
	    continue;
	  }
	  G4int iiSt = ix_dcm +iy_dcm*nVoxelX_dcm+iz_dcm*nVoxelXY_dcm;
	  std::vector<G4int> ists = phantomStMgr->GetStIDList(iiSt);
	  for( G4int jj = 0; jj < ists.size(); jj++ ) {
	    if( histo2->GetBinContent(ix,iy) == 0. ) continue;
	    G4int ist = G4int(ists[jj]);
	    if( histo2->GetBinContent(ix,iy) <= 1. ) theGISmallPerSt[ist]++;
	    theNVoxelsPerSt[ist]++;
	    if( DicomVerb(testVerb) ) G4cout << jj << " FILL PERST " << ix_dcm << " " << iy_dcm << " " << iz_dcm << " " << X_Dose << " " << Y_Dose << " " << Z_Dose << " ::: " << ist << " : " << theStructs[ist] << " = " << theGISmallPerSt[ist] << " / " << theNVoxelsPerSt[ist] << " GI " <<  histo2->GetBinContent(ix,iy) << G4endl;
	    // "ISO 54 GY (SBRT 4D)" 	    if( ist == 29 ) G4cout << theStructs[ist] << " " << jj << " FILL PERST " << ix_dcm << " " << iy_dcm << " " << iz_dcm << " " << X_Dose << " " << Y_Dose << " " << Z_Dose << " ::: " << ist << " : " << theStructs[ist] << " = " << theGISmallPerSt[ist] << " / " << theNVoxelsPerSt[ist] << " GI " <<  histo2->GetBinContent(ix,iy) << G4endl;
	  }
	}
      }
    }
    
    if( DicomVerb(testVerb) ) G4cout << " PERST HISTO ENDED " << G4endl;
    std::map<G4int,G4int>::const_iterator iteb = theGISmallPerSt.begin();
    std::map<G4int,G4int>::const_iterator iten = theNVoxelsPerSt.begin();
    TH1F* histoSt = new TH1F("GI<1","GI<1",theGISmallPerSt.size(), 0, theGISmallPerSt.size());
    G4int ii = 1;
    for( ; iteb != theGISmallPerSt.end(); iteb++, ii++ ) {
      G4int ist = (*iteb).first;
      G4String binLabel = theStructs[ist];
      if( binLabel == "" ) binLabel = "NO_STRUCT";
      histoSt->GetXaxis()->SetBinLabel(ii,binLabel.c_str());
      if( DicomVerb(debugVerb) ) G4cout << ii << " ist " << ist << " BINS LABEL " << binLabel << G4endl;
    }
    TCanvas* canvasSt = new TCanvas;
    // canvas->SetCanvasSize(theNCanvasPixels,theNCanvasPixels);
    canvasSt->SetCanvasSize(theNCanvasPixels,theNCanvasPixels);
    canvasSt->Draw();
    canvasSt->cd();
    histoSt->SetMaximum(100.);
    histoSt->Draw();
    ii = 0;
    for( iteb = theGISmallPerSt.begin() ; iteb != theGISmallPerSt.end(); iteb++, iten++, ii++ ) {
      G4double GIBval = G4double((*iteb).second)/G4double((*iten).second)*100.;
      TMarker* marker = new TMarker( ii+0.5, GIBval, 20);
      G4int ist = (*iteb).first;
      marker->SetMarkerSize(1.5);
      if( DicomVerb(testVerb) ) G4cout << ii << " ist " << ist << " GISmallPerSt " << theStructs[ist] << " : " << GIBval << " = "<< (*iteb).second << " / " <<(*iten).second<< G4endl; 
      marker->Draw();
    }
    std::string gifstr = "hisDCMGetGammaIndex_"+GmGenUtils::ftoa(perCentLim)+"_"+GmGenUtils::ftoa(minDistLim)+"_PerStructure";
    canvasSt->Print((gifstr+".gif").c_str());
  }
  */
}

G4double DCM2DOperGammaIndex::GetMinAtVoxel( G4double imageMin, G4double phantomMin, G4double voxelFVoxDim )
{
  if( imageMin != -DBL_MAX ) {
    G4int idMin = GmGenUtils::GetBelowInt( (imageMin-phantomMin)/voxelFVoxDim );
    //    G4cout << " GetMinAtVoxel " << idMin << " = " << (imageMin-phantomMin)/voxelFVoxDim << " = (" << imageMin << " - " << phantomMin << ")/ " << voxelFVoxDim <<G4endl; //GDEB
    return phantomMin + idMin * voxelFVoxDim;
  } else {
    return phantomMin;
  }
}

//-------------------------------------------------------------------
G4double DCM2DOperGammaIndex::GetMaxAtVoxel( G4double imageMax, G4double phantomMax, G4int nVox, G4double voxelFVoxDim )
{
  if( imageMax != DBL_MAX ) {
    G4double phantomMin = phantomMax-nVox*voxelFVoxDim;
    G4int idMax = GmGenUtils::GetAboveInt( (imageMax-phantomMin)/voxelFVoxDim );
    return phantomMin + idMax * voxelFVoxDim;

  } else {
    return phantomMax;
  }
}

  

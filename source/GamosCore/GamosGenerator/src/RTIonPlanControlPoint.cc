#include "RTIonPlanControlPoint.hh"
#include "RTIonPlanSubControlPoint.hh"
#include "RTIonPlanBeam.hh"
#include "GmGenerVerbosity.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "G4tgrUtils.hh"
#ifndef GAMOS_NO_ROOT
#include "TH2F.h"
#include "TCanvas.h"
#include "TEllipse.h"
#include "TColor.h"
#endif


//-----------------------------------------------------------------------
RTIonPlanControlPoint::RTIonPlanControlPoint( const G4String& fileName, RTIonPlanBeam* beam)
{
  theBeam = beam;
  theFileName = fileName;

  GmFileIn fin = GmFileIn::GetInstance(fileName,1);
  std::vector<G4String> wl;
  //  G4cout << this << " RTIonPlanControlPoint " << fileName <<  " BEAM " << theBeam << G4endl; //GDEB
  for(;;) {
    if( !fin.GetWordsInLine(wl) ) break;
    
    //------------------------------- parameter number
    if( wl[0]== ":P" )  {
      CheckIfNewParameter( wl, 0 );
      theParams[ wl[1] ] = G4tgrUtils::GetDouble( wl[2] );
      //      G4cout << this << " CPReadParams " << wl[1] << " = " << wl[2] << G4endl; //GDEB
   //------------------------------- parameter string
    } else if( wl[0] == ":PS" ) {
      CheckIfNewParameter( wl, 0 );
      theParamStrs[ wl[1] ] = wl[2];
    } else if( wl[0] == "ScanSpotPositions" ) {
      G4int nSCs = GetParam("NumberOfScanSpotPositions",1);
      int ii = 0;
      for( ; ii < nSCs; ii++ ) {
	if( !fin.GetWordsInLine(wl) ) break;
	if( wl.size() != 3 ) {
	  G4Exception("RTIonPlanControlPoint",
		      "",
		      FatalException,
		      ("ScanSpotPosition should have 3 words (POS_X POS_Y METERSET), but it has "
		       + GmGenUtils::itoa(wl.size())).c_str());
	}
	RTIonPlanSubControlPoint* subCP = new RTIonPlanSubControlPoint(wl,this);
	theSubCPs.push_back(subCP);
	//	G4cout<< this << " " << ii << " ADD SUBCP " << theSubCPs[ii] << " " << theSubCPs.size() << G4endl; //GDEB
      }
      if( ii != nSCs ) {
	G4Exception("RTIonPlanControlPoint",
		    "",
		    FatalException,
		    ("Number of ScanSpotPositions " + GmGenUtils::itoa(ii)
		     + " is different than expected " + GmGenUtils::itoa(nSCs)).c_str());
      }
    
    }
  }
    
  //G4int CPNo = GetParam("ControlPointIndex",1);
  //  theAccumulativeMeterset = GetParam("AccumulativeMeterset",1);
  theGantryAngle = GetParam("GantryAngle",1)*CLHEP::deg;
  theGantryPitchAngle = GetParam("GantryPitchAngle",1)*CLHEP::deg;
  theLimitingDeviceAngle = GetParam("LimitingDeviceAngle",1)*CLHEP::deg;
  //  theIsocenterPosition = G4ThreeVector( GetParam("IsocenterPosition_X",1), GetParam("IsocenterPosition_Y",1), GetParam("IsocenterPosition_Z",1) );
  theNominalBeamEnergy = GetParam("NominalBeamEnergy",1);
  theNumberOfPaintings = GetParam("NumberOfPaintings",1);
  theScanningSpotSize = GetParam("ScanningSpotSize",1);
  theSourceAxisDistanceX = theBeam->GetParam("VirtualSourceAxisDistanceX",1);
  theSourceAxisDistanceY = theBeam->GetParam("VirtualSourceAxisDistanceY",1);
  theIndex = G4int(GetParam("ControlPointIndex",1));
  theSnoutPosition = GetParam("SnoutPosition",1);
  /*  if(theNumberOfPaintings != 1 ) {
      G4Exception("RTIonPlanControlPoint::RTIonPlanControlPoint()",
                  "",
		  FatalException,
		  "NumberOfPaintings not treated, please contact GAMOS authors");
		  }*/

  iCurrentPainting = 1;

}

//-----------------------------------------------------------------------
void RTIonPlanControlPoint::RescaleMetersets()
{
  return;
  
  G4double beamMS = 0.;  
  if( theBeam->ParamExists("BeamMeterset") ) {
    beamMS = theBeam->GetParam("BeamMeterset",0);
  } else {
    beamMS = theBeam->GetParam("MetersetWeight",1);
  }
  G4int nSCs = GetParam("NumberOfpotPositions",1);
  std::vector<G4double> metersets;
  G4double metersetSC = 0; // sum of metersets of pots (should be equal to beam meterset)
  for( int ii = 0; ii < nSCs; ii++ ) {
    metersetSC += theSubCPs[ii]->GetMeterset();
  }
  G4double metersetFactor = beamMS / metersetSC;
  //  G4cout << " metersetFactor " << metersetFactor << " =  " << beamMS << " / " << metersetSC << G4endl; //GDEB
  for( int ii = 0; ii < nSCs; ii++ ) {
    theSubCPs[ii]->SetMeterset(theSubCPs[ii]->GetMeterset()*metersetFactor);
  }
}

      
//ROTATIONS FOLLOW IEC 61217    ftp://dicom.nema.org/MEDICAL/dicom/2014c/output/chtml/part03/figures/PS3.3_C.8.8.25-2a.svg     ftp://dicom.nema.org/MEDICAL/dicom/2014c/output/chtml/part03/figures/PS3.3_C.8.8.25-2b.svg
//-----------------------------------------------------------------------
RTBeamStateData RTIonPlanControlPoint::GetBeamStateData(RTBeamStateData bsData)
{
  //    scanSpotData.PositionX = theSubCPs[iNextScanSpot]->GetPosX();
  //    scanSpotData.PositionY = theSubCPs[iNextScanSpot]->GetPosX();
  bsData.PositionZ = theSnoutPosition;
  bsData.Energy = theNominalBeamEnergy;
  bsData.RotAngleX = 0.;
  bsData.RotAngleY = theGantryAngle;
  bsData.RotAngleX = theGantryPitchAngle;
  bsData.RotAngleZ = theLimitingDeviceAngle;
  bsData.SpotSize = theScanningSpotSize;
  bsData.SourceAxisDistanceX = theSourceAxisDistanceX;
  bsData.SourceAxisDistanceY = theSourceAxisDistanceY;
  bsData.ControlPoint = this;
  bsData.Beam = theBeam;
  bsData.IsocenterPosition = G4ThreeVector(GetParam("IsocenterPosition_X"),GetParam("IsocenterPosition_Y"),GetParam("IsocenterPosition_Z"));
  //    G4cout << iNextBeamState <<"  RTIonPlanControlPoint::NextBeamState PositionX " <<   bsData.PositionX << " Y " <<  bsData.PositionY  << " Z " << bsData.PositionZ << G4endl; //GDEB
  
  /*?} else {
    if( iCurrentPainting >= theNumberOfPaintings ) {
      incp++;
    }
       iCurrentPainting++;
       iCurrentScanSpot = -1;
  */
  return bsData;
}

//-------------------------------------------------------------
void RTIonPlanControlPoint::PlotBeamStates(G4double minmaxX, G4double minmaxY, G4double maxMeterset)
{
#ifndef GAMOS_NO_ROOT
  G4String histoName = "RTIonPlanBeamState_" + GmGenUtils::itoa(theBeam->GetIndex()) + "_" + GmGenUtils::itoa(theIndex);
  TH2F* histo = new TH2F( histoName.c_str(), histoName.c_str(), 100, -minmaxX, minmaxX, 100, -minmaxY, minmaxY );

  TCanvas* theCanvas = new TCanvas("");
  theCanvas->SetCanvasSize(1000,1000);
  theCanvas->Draw();
  histo->Draw();
/*t  std::map<G4double,int> orderedSets;
  for( size_t ii = 0; ii < theMetersets.size(); ii++ ) {
    orderedSets[theMetersets[ii]] = ii;
  }
  
  std::map<G4double,int>::const_iterator ite;
  for( ite = orderedSets.begin(); ite != orderedSets.end(); ite++ ) {
    G4int ii = (*ite).second;
    TEllipse* el = new TEllipse(theSubCPs[ii]->GetPosX(), theSubCPs[ii]->GetPosX(), theScanningSpotSize/2., theScanningSpotSize/2. );
    G4int isatur = theMetersets[ii]/maxMeterset*100;
    el->SetFillColor(1000+isatur);
    el->SetFillStyle(1001);
    el->SetLineColor(1000+isatur);
    el->Draw();
  }
*/
  theCanvas->Print((histoName+".gif").c_str());  
#endif
}


 

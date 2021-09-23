#include "RTIonPlanBeam.hh"
#include "RTIonPlanControlPoint.hh"
#include "G4PrimaryVertex.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include <fstream>
 
//-----------------------------------------------------------------------
RTIonPlanBeam::RTIonPlanBeam( const G4String& fileName, const G4String cpFileName )
{
  theFileName = fileName;
  G4String CPFileName = cpFileName;

  GmFileIn fin = GmFileIn::GetInstance(fileName,1);
  std::vector<G4String> wl;

  ReadParams(fin);
  //  G4cout << "RTIonPlanBeams N PARAM " << theParams.size() << G4endl; //GDEB

  theIndex = GetParam("BeamNumber",1);
  G4int nCPs = GetParam("NumberOfControlPoints",1);
  //  G4cout << "RTIonPlanBeams NCPs " << nCPs << G4endl; //GDEB
  G4int nCPsMAX = 5*nCPs;
  for( int ib = 0; ib < nCPs; ib++ ) {
    std::ifstream fint( CPFileName + "_" +GmGenUtils::itoa(theIndex)
		  + "_" + GmGenUtils::itoa(ib));
    if( fint.is_open() ) {
      theControlPoints.push_back( new RTIonPlanControlPoint(
		      CPFileName + "_" +GmGenUtils::itoa(theIndex)
		      + "_" + GmGenUtils::itoa(ib), this));
      //      G4cout << " RTIonPlanBeam ADDING CP : "<< CPFileName + "_" +GmGenUtils::itoa(theIndex)	+ "_" + GmGenUtils::itoa(ib) << G4endl; //GDEB

    } else {
      G4ExceptionSeverity sever = JustWarning;
      if ( ib > nCPsMAX ) sever = FatalException;
      G4Exception("RTIonPlanBeam::RTIonPlanBeam",
                  "",
                  sever,
		  ("Control Point file not found: " + CPFileName
		   + "_" +GmGenUtils::itoa(theIndex)
		   + "_" + GmGenUtils::itoa(ib)).c_str());
      nCPs++;
    }
  }

  ReorderControlPoints();
  iCurrentControlPoint = 0;

}

// WEIRD DATA: EVEN (starting at 0) ControlPoints have 0 RTIonCP_CumulativeMetersetWeight, but non-0 BeamState metersets, while ODD ControlPoints have non-0 RTIonCP_CumulativeMetersetWeight, but 0 BeamState metersets, with same  BeamState's than previous ControlPoint
//-------------------------------------------------------------
void RTIonPlanBeam::ReorderControlPoints()
{
  std::vector<RTVPlanControlPoint*> cpNew;
  std::vector<RTVPlanControlPoint*>::iterator ite;
  std::vector<RTVPlanControlPoint*>::iterator iteold;
  for( ite = theControlPoints.begin(); ite != theControlPoints.end(); ite++ ) {
    std::vector<RTVPlanSubControlPoint*> subCPs = (*ite)->GetSubCPs();
    G4bool bZero = true;
    for( size_t ii = 0; ii < subCPs.size(); ii++ ) {
      if( subCPs[ii]->GetMeterset() != 0. ) {
	bZero = false;
      }
    }
    G4cout << " RTIonPlanBeam::ReorderControlPoints " <<  subCPs.size() << G4endl; //GDEB
    if( bZero ) {
      if( ite == theControlPoints.begin() ) {
	G4Exception("RTIonPlanBeam::ReorderControlPoints",
		    "",
		    FatalException,
		    "First ControlPoint file has 0 metersets, do not know what to do with this file. Please contact GAMOS authors");
      } else {
	iteold = ite; iteold--;
	(*ite)->SetMetersets( (*iteold)->GetSubCPs() );
	//	for( size_t im = 0; im < (*ite)->GetMetersets().size(); im++ ) {
	//	  G4cout << im << " : " << (*ite)->GetMetersets()[0] << " COPY METERSET " << (*ite)->GetMetersets()[im] << G4endl; //GDEB
	//	}
	cpNew.push_back(*ite);
      }
    } 
  }

  if( cpNew.size() != 0 ) {
    for( ite = theControlPoints.begin(); ite != theControlPoints.end(); ite++ ) {
      //    delete *ite;
    }
    theControlPoints = cpNew;
  }

  //-- rescale metersets so that sum of BeamState's metersets = beam meterset
  for( ite = theControlPoints.begin(); ite != theControlPoints.end(); ite++ ) {
    (*ite)->RescaleMetersets();
  }
  
  /*
  std::vector<RTIonPlanControlPoint*> CPs = GetControlPoints();
  for( size_t iicp = 0; iicp < CPs.size(); iicp++ ) {
    std::vector<G4double> meterSets = CPs[iicp]->GetMetersets();
    for( size_t iims = 0; iims < meterSets.size(); iims++ ) {
      if( GenerVerb(debugVerb) ) G4cout << CPs[iicp] << "RECOUNT METERSETS " << iicp << " : " << iims << " <- " << meterSets[iims] << G4endl; 
    }
    }*/
  
}
    

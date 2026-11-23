#include "RTPlanBeam.hh"
#include "RTPlanControlPoint.hh"
#include "G4PrimaryVertex.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include <fstream>
 
//-----------------------------------------------------------------------
RTPlanBeam::RTPlanBeam( const G4String& fileName, const G4String cpFileName )
{
  theFileName = fileName;

  G4String CPFileName = cpFileName;

  GmFileIn fin = GmFileIn::GetInstance(fileName,1);
  std::vector<G4String> wl;

  ReadParams(fin);
  //  G4cout << "RTPlanBeams N PARAM " << theParams.size() << G4endl; //GDEB

  theIndex = GetParam("BeamNumber",1);
  G4int nCPs = GetParam("NumberOfControlPoints",1);
  //  G4cout << this << " RTPlanBeams NCPs " << nCPs << G4endl; //GDEB
  //  G4bool bOneFile = false;
  for( int icp = 0; icp < nCPs; icp++ ) {
    std::ifstream fint(CPFileName + "_" +GmGenUtils::itoa(theIndex)
		       + "_" + GmGenUtils::itoa(icp));
    if( fint.good() ) {
      theControlPoints.push_back( new RTPlanControlPoint(
		      CPFileName + "_" +GmGenUtils::itoa(theIndex)
		      + "_" + GmGenUtils::itoa(icp), this));
      //      bOneFile = true;
    } else {
      G4Exception("RTPlanBeam::RTPlanBeam",
		  "",
		  JustWarning,
		  ("Control Point file not found: " + CPFileName
		   + "_" +GmGenUtils::itoa(theIndex)
		   + "_" + GmGenUtils::itoa(icp)).c_str());
      nCPs++; // if one is missing try one more
    }
    //    G4cout << theControlPoints[theControlPoints.size()-1] << " RTPlanBeams add CPs " << theControlPoints.size() << G4endl; //GDEB
    /*    if( !bOneFile ) {
      G4Exception("RTPlanBeam::RTPlanBeam",
		  "",
		  JustWarning,
		  ("No Control Point file found with prefix: " + CPFileName
		   + "_" +GmGenUtils::itoa(theIndex)).c_str());
		   } */
  }
  // rescale meterset to FinalCumulativeMetersetWeight
  G4double FCmeterset = GetParam("FinalCumulativeMetersetWeight",1.);
  if( FCmeterset != 1. ) {
    for( int icp = 0; icp < nCPs; icp++ ) {
      RTVPlanControlPoint* CP = theControlPoints[icp];
      std::vector<RTVPlanSubControlPoint*> subCPs = CP->GetSubCPs();
      for( size_t iscp = 0; iscp < subCPs.size(); iscp++ ) {
	subCPs[iscp]->SetMeterset( subCPs[iscp]->GetMeterset()/FCmeterset );
      }
    }
  }
      
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) {
    G4cout << " RTPlanBeam::RTPlanBeam NCPs " << theIndex << theControlPoints.size() << G4endl; 
  }
#endif

  ReorderControlPoints();
  iCurrentControlPoint = 0;

}

// For StepAndShot even control points have meterset=0 (they are just displacement), and all data is equal to next control point: copy data from next control point and eliminate next
//-------------------------------------------------------------
void RTPlanBeam::ReorderControlPoints()
{
  std::vector<RTVPlanControlPoint*> cpNew;
  std::vector<RTVPlanControlPoint*>::const_iterator ite, ite2, iteN;
  /*  for( ite = theControlPoints.begin(); ite != theControlPoints.end(); ite++ ) {
    G4cout << " PRINT Control Point " << *(*ite) << " " << G4endl; //GDEB
    } */
  for( ite = theControlPoints.begin(), iteN = theControlPoints.begin() ; ite != theControlPoints.end(); ite++, iteN++ ) {
    //    G4cout << " CHECKING Control Point " << *(*ite) << " " << G4endl; //GDEB
    ite2 = ite;
    std::vector<RTVPlanSubControlPoint*> subCPs = (*ite)->GetSubCPs();
    G4bool bZero = true;
    for( size_t ii = 0; ii < subCPs.size(); ii++ ) {
      G4double meterSet = subCPs[ii]->GetMeterset();
      if( meterSet != 0. ) {
	bZero = false;
      }
    }
    if( bZero ) {
      ite2++;
      if( ite2 == theControlPoints.end() ) {
	G4Exception("RTPlanBeam::ReorderControlPoints",
		    "",
		    FatalException,
		    "Last ControlPoint file has 0 meterset, do not know what to do with this file. Please contact GAMOS authors");
      } else {
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) {
	  G4cout << " COMPARE " << *(*ite) << " " << *(*ite2) << G4endl; 
	}
#endif
	if( *(*ite) == *(*ite2) ) {
	  (*ite)->SetParam("MetersetWeight",(*ite2)->GetParam("MetersetWeight"));
	  (*ite)->SetParam("CumulativeMetersetWeight",(*ite2)->GetParam("CumulativeMetersetWeight"));
	  (*ite)->SetMetersets( (*ite2)->GetSubCPs() );
	} else {
	  // ANALYZE DIFFERENCE
	  G4bool bSkip0CPs = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("RTPlanBeam:Skip0ControlPoints",1));
	  if( bSkip0CPs ) {
	    G4Exception("RTPlanBeam::ReorderControlPoints",
			"",
			JustWarning,
			"ControlPoint file has 0 meterset and next one is not equivalent, do not know what to do with this file. Please contact GAMOS authors");
	  } else {
	    G4Exception("RTPlanBeam::ReorderControlPoints",
			"",
			FatalException,
			"ControlPoint file has 0 meterset and next one is not equivalent, do not know what to do with this file. Please contact GAMOS authors");
	  }
	}
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) {
	  G4cout << " RTPlanBeam::ReorderControlPoints deleting point " << (*ite2)->GetIndex() << " N POINTS " << cpNew.size() << G4endl; 
	}
#endif 
      }
      ite++;
      ite2--; //use it to save in new list
    }
    cpNew.push_back(*ite2);
  }

  if( cpNew.size() != 0 ) {
    //    for( rite = theControlPoints.rbegin(); rite != theControlPoints.rend(); rite++ ) {
      //    delete *rite;
    //    }
    theControlPoints = cpNew;
  }

  #ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) {
    G4cout << " RTPlanBeam::ReorderControlPoints " << theIndex << " NCPs " << cpNew.size() << G4endl; 
  }
#endif
  /*
  std::vector<RTPlanControlPoint*> CPs = GetControlPoints();
  for( size_t iicp = 0; iicp < CPs.size(); iicp++ ) {
    std::vector<G4double> meterSets = CPs[iicp]->GetMetersets();
    for( size_t iims = 0; iims < meterSets.size(); iims++ ) {
      if( GenerVerb(debugVerb) ) G4cout << CPs[iicp] << "RECOUNT METERSETS " << iicp << " : " << iims << " <- " << meterSets[iims] << G4endl; 
    }
    }*/
  
}
    

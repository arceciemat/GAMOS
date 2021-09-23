#include "RTIonPlanScanSpotSource.hh"
#include "RTIonPlanBeam.hh"
#include "RTPlanMgr.hh"
#include "RTIonPlanSubControlPoint.hh"
#include "G4PrimaryVertex.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "GmGenerDistTimeConstant.hh"
#include "GmGenerDistDirectionConst.hh"
#include "GmGenerDistDirectionCone.hh"
#include "GmGenerDistPositionDisc.hh" 
#include "GmGenerDistPositionDiscGaussian.hh" 
#include "GmGenerDistEnergyConstant.hh"
#include "GmGenerDistEnergyGaussian.hh"
#include "GmGeneratorMgr.hh"

#include "G4PrimaryParticle.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"

#include "CLHEP/Random/RandFlat.h"
#ifndef GAMOS_NO_ROOT
#include "TColor.h"
#endif

//-----------------------------------------------------------------------
RTIonPlanScanSpotSource::RTIonPlanScanSpotSource( const G4String& name, const G4String& partName): RTVPlanSource( name )
{
  theType = "RTIonPlanScanSpotSource";

  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  thePlanFileName = parMgr->GetStringValue(theName+":PlanFileNamePrefix","RTIonPlan"); 
  theBeamFileName = parMgr->GetStringValue(theName+":BeamFileNamePrefix","RTIonPlanBeam"); 
  theCPFileName = parMgr->GetStringValue(theName+":ControlPointFileNamePrefix","RTIonPlanControlPoint");

  /*  G4String posDist = parMgr->GetStringValue(theName+":PositionDist","Disc");
  if( posDist == "Disc" ) {
    bDiscGaussian = false;
  } else {
    bDiscGaussian = true;
    } */
  
  Initialize(partName);
  
  ReadPlanData();
  RTPlanMgr::GetInstance()->SetBeams(theBeams);
}


//---------------------------------------------------------------------
void RTIonPlanScanSpotSource::ReadPlanData()
{
  GmFileIn fin = GmFileIn::GetInstance(thePlanFileName,1);
  ReadParams(fin);

  G4cout << "RTPlanScanSpotSource::ReadPlanData() N PARAM " << theParams.size() << G4endl; //GDEB
  int ii = 1;
  for(;;ii++) { //  Check if various FractionGroup's exists (very rare)
    G4String fgstr = "FractionGroup_"+GmGenUtils::itoa(ii);
    if( ParamExists(fgstr+"_Number") ) { 
      G4int nBeams = GetParam(fgstr+"_NumberOfBeams",1);
      if( nBeams == 0 ) {
	 G4ExceptionSeverity es;
	 if( ii == 1 ) {
	   es = FatalException;
	 } else {
	   es = JustWarning;
	 }
	 G4Exception("RTIonPlanScanSpotSource::ReadPlanData",
		     "",
		     es,
		     ("NumberOfBeams is not 0 in fraction " + GmGenUtils::itoa(ii)).c_str());
      }
		    
      //      G4cout << "RTVPlanSource::ReadPlanData() nBeams " << nBeams << G4endl; //GDEB
      //      theBeamDispX = GmParameterMgr::GetInstance()->GetNumericValue(theName+":BeamPositionX",0.);
      //      theBeamDispY = GmParameterMgr::GetInstance()->GetNumericValue(theName+":BeamPositionY",0.);
      for( int ib = 1; ib <= nBeams; ib++ ) {
	std::ifstream fint( theBeamFileName + "_" +GmGenUtils::itoa(ib));
	if( fint.good() ) {
	  RTIonPlanBeam * beam = new RTIonPlanBeam( theBeamFileName + "_" +GmGenUtils::itoa(ib), theCPFileName );
	  theBeams.push_back( beam );
	  beam->SetSource(this);
	}
      }
    } else {
      if( ii == 1 )  {
	G4Exception("RTIonPlanScanSpotSource::ReadPlanData",
		    "",
		    FatalException,
		    "No FractionGroup number 1, please contact GAMOS authors");
      } else {
	break;
      }
    }
  }

  if( theBeams.size() == 0 ) {
    G4Exception("RTIonPlanScanSpotSource::ReadPlanData",
		"",
		FatalException,
		"No BEAM defined, check your data files");
  }
  
}

//-------------------------------------------------------------
G4double RTIonPlanScanSpotSource::GetSpotSize( RTBeamStateData spData )
{
  return spData.SpotSize/(2.*sqrt(2*log(2.))); // SpotSize is FWHM: used as sigma
}

//-------------------------------------------------------------
void RTIonPlanScanSpotSource::SetSpotSize( GmVGenerDistPosition2D* , const RTBeamStateData& spdata )
{
  G4bool bInitDist = false;
  if( !bInitDist ) {
    GmVGenerDistPosition* posD = GetPositionDistribution();
    //    G4cout << "RTIonPlanScanSpotSource::SetSpotSize( " << posD->GetName() <<  G4endl; //GDEB  
    if( posD->GetName() == "GmGenerDistPositionDisc" ) {
      GmGenerDistPositionDisc* posDD = dynamic_cast<GmGenerDistPositionDisc*>(posD);
      posDD->SetRadius(GetSpotSize(spdata));
    } else if( posD->GetName() == "GmGenerDistPositionDiscGaussian" ) {
      GmGenerDistPositionDiscGaussian* posDD = dynamic_cast<GmGenerDistPositionDiscGaussian*>(posD);
      posDD->SetSigma(GetSpotSize(spdata));
    }
    if( GenerVerb(debugVerb) ) {
      G4cout << "  RTIonPlanScanSpotSource::SetSpotSize " << posD->GetName() << " : " << GetSpotSize(spdata) << G4endl;
    }
  }

}

//-------------------------------------------------------------
void RTIonPlanScanSpotSource::PlotBeamStates()
{
#ifndef GAMOS_NO_ROOT
  G4double minX = DBL_MAX;
  G4double maxX = -DBL_MAX;
  G4double minY = DBL_MAX;
  G4double maxY = -DBL_MAX;
  G4double maxMS = -DBL_MAX;
  G4double minmaxX = 0.; 
  G4double minmaxY = 0.; 
  for( size_t iib = 0; iib < theBeams.size(); iib++ ) {
    std::vector<RTVPlanControlPoint*> CPs = theBeams[iib]->GetControlPoints();
    for( size_t iicp = 0; iicp < CPs.size(); iicp++ ) {
      RTIonPlanControlPoint* CP = static_cast<RTIonPlanControlPoint*>(CPs[iicp]);
      std::vector<RTVPlanSubControlPoint*> subCPs = CP->GetSubCPs();
      G4double spotSize = CP->GetScanningSpotSize();
      for( size_t iip = 0; iip < subCPs.size(); iip++ ) {
	RTIonPlanSubControlPoint* scp = dynamic_cast<RTIonPlanSubControlPoint*>(subCPs[iip]);
	minX = std::min(minX,scp->GetPosX()-spotSize);
	maxX = std::max(maxX,scp->GetPosX()+spotSize);
	minY = std::min(minX,scp->GetPosY()-spotSize);
	maxY = std::max(maxX,scp->GetPosY()+spotSize);
	maxMS = std::max(maxMS,scp->GetMeterset());
      }
    }
  }
  minmaxX = std::max(fabs(minX),fabs(maxX));
  minmaxY = std::max(fabs(minY),fabs(maxY));

  for( G4int ii = 0; ii < 100; ii++ ) {
    float satur = ii/100.;
    new TColor( 1000+ii, satur, 0, 0 );
  }

  for( size_t iib = 0; iib < theBeams.size(); iib++ ) {
    std::vector<RTVPlanControlPoint*> CPs = theBeams[iib]->GetControlPoints();
    for( size_t iicp = 0; iicp < CPs.size(); iicp++ ) {
      CPs[iicp]->PlotBeamStates(minmaxX,minmaxY,maxMS);
    }
  }

#endif
}

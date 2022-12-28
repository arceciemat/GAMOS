#include "RTPlanSource.hh"
#include "RTPlanBeam.hh"
#include "RTPlanMgr.hh"
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

//-----------------------------------------------------------------------
RTPlanSource::RTPlanSource( const G4String& name, const G4String& partName): RTVPlanSource( name )
{
  theType = "RTPlanSource";

  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  G4cout << " RTPlanSource NAME " << theName << G4endl; //GDEB
  thePlanFileName = parMgr->GetStringValue(theName+":PlanFileNamePrefix","RTPlan"); 
  theBeamFileName = parMgr->GetStringValue(theName+":BeamFileNamePrefix","RTPlanBeam"); 
  theCPFileName = parMgr->GetStringValue(theName+":ControlPointFileNamePrefix","RTPlanControlPoint");

  /*G4String posDist = GmParameterMgr::GetInstance()->GetStringValue(theName+":PositionDist","DiscGaussian");
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
void RTPlanSource::ReadPlanData()
{
  GmFileIn fin = GmFileIn::GetInstance(thePlanFileName,1);
  ReadParams(fin);

  //  G4cout << "RTPlanSource::ReadPlanData() N PARAM " << theParams.size() << G4endl; //GDEB
  std::set<int> fracGroups;
  for(int ii = 0; ii < 1000;ii++) { // only up to 1000!!
    G4String fgstr = "FractionGroup_"+GmGenUtils::itoa(ii);
    if( ParamExists(fgstr+"_Number") ) { 
      fracGroups.insert(ii);
    }
  }
  //  Check at least one FractionGroup exists 
  if( fracGroups.size() == 0 ) {
    G4Exception("RTPlanSource::ReadPlanData",
		"",
		FatalException,
		"No FractionGroup existsin 'RTPlan', please contact GAMOS authors");
  }
  //  Check if various FractionGroup's exists (very rare)
  if( fracGroups.size() != 1 ) {
    G4Exception("RTPlanSource::ReadPlanData",
		"",
		JustWarning,
		"More than one FractionGroup in 'RTPlan', please contact GAMOS authors");
  }
  //---- Loop to fraction groups
  for( std::set<int>::iterator ite = fracGroups.begin(); ite != fracGroups.end(); ite++ ) {
    int ii = (*ite);    
    G4String fgstr = "FractionGroup_"+GmGenUtils::itoa(ii);
    G4int nBeams = GetParam(fgstr+"_NumberOfBeams",1);
    if( nBeams == 0 ) {
      G4ExceptionSeverity es;
      if( ii == 1 ) {
	es = FatalException;
      } else {
	es = JustWarning;
      }
      G4Exception("RTPlanSource::ReadPlanData",
		  "",
		     es,
		  ("NumberOfBeams is not 0 in fraction " + GmGenUtils::itoa(ii)).c_str());
    }
    
    //      G4cout << "RTPlanSource::ReadPlanData() nBeams " << nBeams << G4endl; //GDEB      
    G4bool bOneFile = false;
    for( int ib = 1; ib <= nBeams; ib++ ) {
      std::ifstream fint(theBeamFileName + "_" +GmGenUtils::itoa(ib));
      if( fint.good() ) {
	RTPlanBeam* beam = new RTPlanBeam( theBeamFileName + "_" +GmGenUtils::itoa(ib), theCPFileName );
	theBeams.push_back( beam );
	beam->SetSource(this);
	bOneFile = true;
      } else {
	G4Exception("RTPlanSource::RTPlanSource",
		    "",
		    JustWarning,
		    ("Beam file not found: " + theBeamFileName
		     + "_" +GmGenUtils::itoa(ib)).c_str());
      }
      if( !bOneFile ) {
	G4Exception("RTPlanSource::RTPlanSource",
		    "",
		    JustWarning,
		    ("No Beam file found with prefix: " + theBeamFileName
		     + "_" +GmGenUtils::itoa(ib)).c_str());
	
      }
    }
  }

  if( theBeams.size() == 0 ) {
    G4Exception("RTPlanSource::ReadPlanData",
		"",
		FatalException,
		"No BEAM defined, check your data files");
  }

}


/*//-------------------------------------------------------------
G4double RTPlanSource::GetSpotSize( RTBeamStateData )
{
  if( !bDiscGaussian ) {
    return theDiscSigma;
  } else {
    return theDiscRadius;
  }
}
*/

//-------------------------------------------------------------
void RTPlanSource::SetSpotSize( GmVGenerDistPosition2D* , const RTBeamStateData& )
{
  return;
  // do nothing (taken from distribution defined in input script)
  
}

//-------------------------------------------------------------
void RTPlanSource::PlotBeamStates()
{
  return;

}

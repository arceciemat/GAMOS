#include "GmVRecHitBuilder.hh"
#include "GmHitsEventMgr.hh"
#include "GmSDVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmDistributionMgr.hh"
#include "GamosCore/GamosData/Distributions/include/GmVNumericDistribution.hh"
#include "GmHitsEventMgr.hh"

GmVRecHitBuilder::GmVRecHitBuilder()
{

}

//------------------------------------------------------------------------
void GmVRecHitBuilder::SetParams()
{  

  G4String distName = GmParameterMgr::GetInstance()->GetStringValue(theName+":EnergyEfficiencyDistribution","");
  if( distName != "" ) {
    std::vector<G4String> params;
    params.push_back(distName);
    params.push_back(distName);
    theEfficiencyDistribution = (GmVNumericDistribution*)(GmDistributionMgr::GetInstance()->FindOrBuildDistribution(params));
    theHitsEventMgr = GmHitsEventMgr::GetInstance();
  } else {
    theEfficiencyDistribution = 0;
  }

}

//------------------------------------------------------------------------
void GmVRecHitBuilder::SetSDType(const G4String& sdtype)
{
  theSDType = sdtype;
  theSDType = theSDType.substr(1,theSDType.size()-2);
 
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << " GmVRecHitBuilder::SetSDType " << "SD:RecHit:MinimumEnergyBehaviour:"+theSDType << G4endl;
#endif  

  theMinHitEnergy = GmParameterMgr::GetInstance()->GetNumericValue("SD:Hit:MinimumEnergy:"+theSDType, 0.);


  G4String meb = GmParameterMgr::GetInstance()->GetStringValue("SD:RecHit:MinimumEnergyBehaviour:"+theSDType, "");

  /*  if( meb == "" || meb == "AcceptAll" ){
    theMEBehaviour = MEAcceptAll;

      } else if( meb == "DeleteSmall" ) {
    theMEBehaviour = MEDeleteSmall;
    
    } else*/
  if( meb == "AcceptIf1Big" ) {
    theMEBehaviour = MEAcceptIf1Big;

  } else if( meb == "DeleteIf1Small" ) {
    theMEBehaviour = MEDeleteIf1Small;

  } else if( meb != "" ) {
    G4Exception("GmVRecHitBuilder",
		"Wrong value for minimum energy behaviour",
		FatalErrorInArgument,
		(G4String("Values can be AcceptIf1Big / DeleteIf1Small , it is") + meb).c_str());
  }

  if( meb != "" ) {
    theMinRecHitEnergy = GmParameterMgr::GetInstance()->GetNumericValue("SD:RecHit:MinimumEnergy:"+theSDType, 0.);
  } else {
    if( GmParameterMgr::GetInstance()->IsParameterInScript("SD:RecHit:MinimumEnergy:"+theSDType) ) {
      G4Exception("GmVRecHitBuilder",
		  "Minimum hit energy cannot be set if behaviour is not defined",
		FatalErrorInArgument,
		  "Use /gamos/setParam SD:RecHit:MinimumEnergyBehaviour ");
    }
  }

  theEnergyResol = GmParameterMgr::GetInstance()->GetNumericValue("SD:EnergyResol:"+theSDType,0.);

  theEnergyResolFluct = GmParameterMgr::GetInstance()->GetNumericValue("SD:EnergyResolFluct:"+theSDType,0.);

  theEnergyResolInstr = GmParameterMgr::GetInstance()->GetNumericValue("SD:EnergyResolInstr:"+theSDType,0.);

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << "GmVRecHitBuilder:: " << "SD:EnergyResol:"+theSDType << " Energy resolution " << theEnergyResol << " resolution Fluctuations " << theEnergyResolFluct << " resolution Instrument " << theEnergyResolInstr << G4endl;
#endif

  theTimeResol = GmParameterMgr::GetInstance()->GetNumericValue("SD:TimeResol:"+theSDType,0.);
}

//------------------------------------------------------------------------
void GmVRecHitBuilder::CheckRecHitsMinEnergy()
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << " GmVRecHitBuilder::CheckRecHitsMinEnergy() N hits at start= " << theRecHits.size() << G4endl;
#endif

  std::vector<GmRecHit*>::iterator iterh;
  std::vector< std::vector<GmRecHit*>::iterator > vites;
  /* already not considered when building rechit
    if( theMEBehaviour == MEDeleteSmall ) {
    for( iterh = theRecHits.begin(); iterh != theRecHits.end(); iterh++ ) {
    if( (*iterh)->GetEnergy() < theMinRecHitEnergy ) {
	vites.push_back(iterh);
      }
    }
    std::vector<std::vector<GmRecHit*>::iterator>::reverse_iterator iteitev;
    for( iteitev = vites.rbegin(); iteitev != vites.rend(); iteitev++ ){
      theRecHits.erase(*iteitev);
    }
  
  } else 
*/
  if( theMEBehaviour == MEAcceptIf1Big ) {
    G4bool b1Big = false;
    for( iterh = theRecHits.begin(); iterh != theRecHits.end(); iterh++ ) {
      if( (*iterh)->GetEnergy() > theMinRecHitEnergy ) {
	b1Big = true;
      }
    }
    if( !b1Big ) theRecHits.clear();

  } else if( theMEBehaviour == MEDeleteIf1Small ) {
    G4bool b1Small = false;
    for( iterh = theRecHits.begin(); iterh != theRecHits.end(); iterh++ ) {
      if( (*iterh)->GetEnergy() < theMinRecHitEnergy ) {
	b1Small = true;
      }
    }
    if( b1Small ) theRecHits.clear();
  }

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << " GmVRecHitBuilder::CheckRecHitsMinEnergy() N hits at end= " << theRecHits.size() << G4endl;
#endif

}

//------------------------------------------------------------------------
void GmVRecHitBuilder::CheckEnergyEfficiency()
{
  if( theEfficiencyDistribution ) {
    std::vector<GmRecHit*>::const_iterator iter;
    for( iter = theRecHits.begin(); iter != theRecHits.end(); iter++ ) {
      G4double effic = theEfficiencyDistribution->GetNumericValueFromIndex( (*iter)->GetEnergy() );
      if( effic < 1. ) {
	G4double rand = CLHEP::RandFlat::shoot();
	if( rand < effic ) {
#ifndef GAMOS_NO_VERBOSE
	  if( SDVerb(debugVerb) ) G4cout << " GmVRecHitBuilder::CheckEnergyEfficiency hit deleted " << *(*iter) << G4endl;
#endif
	  theHitsEventMgr->DeleteHits( *iter );
	}
      }
      
    }
  }

}


//------------------------------------------------------------------------
void GmVRecHitBuilder::SmearRecHitsEnergy()
{ 
  std::vector<GmRecHit*>::const_iterator iter;
  for( iter = theRecHits.begin(); iter != theRecHits.end(); iter++ ) {
    GmRecHit* recHit = *iter;
    G4double rhEnergy = recHit->GetEnergy();
    G4double ran = CLHEP::RandGauss::shoot(0., theEnergyResol );
    G4double factor = ran;
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << " GmVRecHitBuilder::SmearRecHitsEnergy factor " << factor << " " << ran << " Resol " << theEnergyResol << G4endl;
#endif
    if( theEnergyResolFluct != 0. ) {
      G4double ran = CLHEP::RandGauss::shoot(0., theEnergyResolFluct );
      factor += ran / sqrt(rhEnergy);
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(debugVerb) ) G4cout << " GmVRecHitBuilder::SmearRecHitsEnergy factor resolFluct " << factor << " " << ran << " Resol " << theEnergyResolFluct << G4endl;
#endif
    }

    if( theEnergyResolInstr != 0. ) {
      G4double ran = CLHEP::RandGauss::shoot(0., theEnergyResolInstr );
      factor += ran / rhEnergy;
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(debugVerb) ) G4cout << " GmVRecHitBuilder::SmearRecHitsEnergy factor resolInstr " << factor << " " << ran << " Resol " << theEnergyResolInstr << G4endl;
#endif
    }
    rhEnergy += rhEnergy*factor;
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << " GmVRecHitBuilder::SmearRecHitsEnergy final Energy " << rhEnergy << " nonSmeared Energy " << recHit->GetEnergy() << G4endl;
#endif
    recHit->SetEnergySmeared( rhEnergy );
  }

}

//------------------------------------------------------------------------
void GmVRecHitBuilder::SmearRecHitsTime()
{ 
  std::vector<GmRecHit*>::const_iterator iter;
  for( iter = theRecHits.begin(); iter != theRecHits.end(); iter++ ) {
    GmRecHit* recHit = *iter;
    recHit->SetTimeMinSmeared( recHit->GetTimeMin() * CLHEP::RandGauss::shoot(1., theTimeResol ));
  }

}

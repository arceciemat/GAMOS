#include "GmParticleSource.hh"
#include "GmVGenerDistTime.hh"
#include "GmVGenerDistEnergy.hh"
#include "GmVGenerDistPosition.hh"
#include "GmVGenerDistDirection.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "GamosCore/GamosData/Distributions/include/GmVNumericDistribution.hh"
#include "GamosCore/GamosBase/Base/include/GmDistributionMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"

// CorrectPosDir
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosBase/Base/include/GmHistoReaderCSV.hh"
#include "GamosCore/GamosBase/Base/include/GmPhysicsLinearVector.hh"
#include "GmGenerDistEnergyFromMultiFileE.hh"
#include "GmGenerDistEnergyFromFile.hh"
#include "GmGenerDistEnergyConstant.hh"

#include "G4PrimaryVertex.hh"
#include "G4ParticleDefinition.hh"
#include "CLHEP/Random/RandFlat.h"

#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "GmGeneratorDistributionFactories.hh"
#endif

//-----------------------------------------------------------------------
GmParticleSource::GmParticleSource( const G4String& name )
{
  theName = name;
  theLastDecayTimeAnySource = 0.;
  bUsedLastTime = true;

  bBiasDistributions = false;

  theMaxBiasIterations = GmParameterMgr::GetInstance()->GetNumericValue(theName+":MaxBiasIterations",10000);

  G4String localVolume = GmParameterMgr::GetInstance()->GetStringValue("GmGenerator:LocalVolume","");
  //  G4cout << " GET localVolume " << localVolume << G4endl; //GDEB
  if( localVolume != "" ) {
    bLocalVolume = true;
    std::vector<GmTouchable*> touchables = GmGeometryUtils::GetInstance()->GetTouchables( localVolume );
    if( touchables.size() == 0 ) {
      G4Exception("GmGenerator::GmGenerator",
		  "",
		  FatalErrorInArgument,
		  ("Local volume name not found: "+localVolume).c_str());
    } else if( touchables.size() != 1 ) {
      G4Exception("GmGenerator::GmGenerator",
		  "",
		  FatalErrorInArgument,
		  ("Local volume can only have one placement, it has "+GmGenUtils::itoa(touchables.size())).c_str());
    }
    theLocalVolumePosition = touchables[0]->GetGlobalPosition();
    theLocalVolumeRotation = touchables[0]->GetGlobalRotation();
    //    G4cout << " FILL localVolume " << localVolume << " " <<  theLocalVolumePosition << " " << theLocalVolumeRotation << G4endl; //GDEB
  } else {
    bLocalVolume = false;
  }

  bCorrectPosDir = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":CorrectPosDir",0.));
  
  if( bCorrectPosDir ) {
    ReadEnerDistF();
  }


}


//-----------------------------------------------------------------------
void GmParticleSource::CheckDistributionsExist()
{
  if( !theTimeDistribution || !theDirectionDistribution || !thePositionDistribution || !theEnergyDistribution ) {
    G4cerr << "!!One distribution does not exist for ParticleSource: " << theName << " time " << theTimeDistribution << " dir " << theDirectionDistribution << " pos " << thePositionDistribution << " ener " << theEnergyDistribution << G4endl;
    G4Exception("GmParticleSource::CheckDistributionsExit",
		"Wrong argument",
		FatalErrorInArgument,
		"");
  }


}


//-----------------------------------------------------------------------
void GmParticleSource::SetDistributionTime( const G4String& distName, const std::vector<G4String>& wl )
{
#ifdef ROOT5
  theTimeDistribution = Reflex::PluginService::Create<GmVGenerDistTime*>(distName);
#else
  theTimeDistribution = GmGenerDistTimeFactory::get()->create(distName);
#endif
  if( !theTimeDistribution ) {
    G4Exception("GmParticleSource::SetDistributionTime","Fatal error in argument",FatalErrorInArgument,G4String(" Distribution does not exist, check documentation").c_str());
  }
  theTimeDistribution->SetName(distName);
  
  theTimeDistribution->SetParticleSource( this );
  theTimeDistribution->CopyParams( wl );
  theTimeDistribution->SetParams( wl );

}


//-----------------------------------------------------------------------
void GmParticleSource::SetDistributionEnergy( const G4String& distName, const std::vector<G4String>& wl )
{
#ifdef ROOT5
  theEnergyDistribution = Reflex::PluginService::Create<GmVGenerDistEnergy*>(distName);
#else
  theEnergyDistribution = GmGenerDistEnergyFactory::get()->create(distName);
#endif
  if( !theEnergyDistribution ) {
    G4Exception("GmParticleSource::SetDistributionEnergy","Fatal error in argument",FatalErrorInArgument,G4String(" Distribution does not exist, check documentation ").c_str());
  }
  theEnergyDistribution->SetName(distName);
  theEnergyDistribution->SetParticleSource( this );
  theEnergyDistribution->CopyParams( wl ); 
  theEnergyDistribution->SetParams( wl ); 

}


//-----------------------------------------------------------------------
void GmParticleSource::SetDistributionPosition( const G4String& distName, const std::vector<G4String>& wl )
{
#ifdef ROOT5
  thePositionDistribution = Reflex::PluginService::Create<GmVGenerDistPosition*>(distName);
#else
  thePositionDistribution = GmGenerDistPositionFactory::get()->create(distName);
#endif
  if(  !thePositionDistribution ) {
    G4Exception("GmParticleSource::SetDistributionPosition","Fatal error in argument",FatalErrorInArgument,G4String(" Distribution does not exist, check documentation ").c_str());
  }
  thePositionDistribution->SetName(distName);

  thePositionDistribution->SetParticleSource( this );
  thePositionDistribution->CopyParams( wl );
  thePositionDistribution->SetParams( wl );

  //  G4cout << this << " thePositionDistribution" << thePositionDistribution << " " << thePositionDistribution->GetParticleSource() <<  G4endl; //GDEB
  
}

//-----------------------------------------------------------------------
void GmParticleSource::SetDistributionDirection( const G4String& distName, const std::vector<G4String>& wl )
{
#ifdef ROOT5
  theDirectionDistribution = Reflex::PluginService::Create<GmVGenerDistDirection*>(distName);
#else
  theDirectionDistribution = GmGenerDistDirectionFactory::get()->create(distName);
#endif
  if(  !theDirectionDistribution ) {
    G4Exception("GmParticleSource::SetDistributionDirection","Fatal error in argument",FatalErrorInArgument,G4String(" Distribution does not exist, check documentation ").c_str());
  }
  theDirectionDistribution->SetName(distName);

  theDirectionDistribution->SetParticleSource( this );
  theDirectionDistribution->CopyParams( wl );
  theDirectionDistribution->SetParams( wl );

  // G4cout << this << " theDirectionDistribution" << theDirectionDistribution << " " << theDirectionDistribution->GetParticleSource() <<  G4endl; //GDEB
}
//-----------------------------------------------------------------------
G4double GmParticleSource::GenerateTime()
{
  if( bUsedLastTime ) {
    theTime = theTimeDistribution->GenerateTime( this );
  } else {
    theTime -= theLastDecayTimeAnySource;
    //    if( theTime < 0 ) theTime = 0.;
    if( theTime < 0 ) {
      G4Exception("GmParticleSource::GenerateTime",
		  "time - theLastDecayTimeAnySource is negative",
		  FatalErrorInArgument,
		  G4String("Time= "+GmGenUtils::ftoa(theTime+theLastDecayTimeAnySource)+" LastDecayTimeAnySource= "+GmGenUtils::ftoa(theLastDecayTimeAnySource)).c_str());
    }
  }
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << "GmParticleSource::GenerateTime: " << theName << " iso activity " << theActivity << " time= " << theTime << " theLastDecayTimeAnySource " << theLastDecayTimeAnySource << G4endl;
#endif

  return theTime;
}



//-----------------------------------------------------------------------
void GmParticleSource::LastTimeActive( G4double )
{
  theLastDecayTimeAnySource = 0.;
  bUsedLastTime = true;
}


//-----------------------------------------------------------------------
void GmParticleSource::LastTimeNotActive( G4double tim )
{
  //  theLastDecayTimeAnySource += tim;
  theLastDecayTimeAnySource = tim;
  bUsedLastTime = false;
  //  G4cout << theName << "  GmParticleSource::LastTimeNotActive theLastDecayTimeAnySource " <<  theLastDecayTimeAnySource << G4endl; //GDEB
}


//-----------------------------------------------------------------------
void GmParticleSource::ReInitialiseDistributions()
{

  theTimeDistribution->ResetParams();
  theEnergyDistribution->ResetParams();
  thePositionDistribution->ResetParams();
  theDirectionDistribution->ResetParams();

}

//---------------------------------------------------------------------
void GmParticleSource::AddBiasDistribution( const G4String& varName, const G4String& distName )
{
  std::vector<G4String> distNames;
  distNames.push_back(distName);
  distNames.push_back(distName);
  GmVDistribution* distrib = GmDistributionMgr::GetInstance()->FindOrBuildDistribution( distNames, true );
  distrib->Normalize();
 
  GmVNumericDistribution* distribN = (GmVNumericDistribution*)distrib;
  theBiasDistributions[varName] = distribN;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmParticleSource::AddBiasDistribution " << varName << " DIST " << distName << G4endl; 
#endif
  bBiasDistributions = true;
}


//-----------------------------------------------------------------------
void GmParticleSource::BiasPosition()
{
  std::map<G4String,GmVNumericDistribution*>::const_iterator ite = theBiasDistributions.find("PosX");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( thePosition.x() );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      thePosition = thePositionDistribution->GeneratePosition( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasPosition",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }
  

  ite = theBiasDistributions.find("PosY");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( thePosition.y() );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      thePosition = thePositionDistribution->GeneratePosition( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasPosition",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }

  ite = theBiasDistributions.find("PosZ");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( thePosition.z() );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      thePosition = thePositionDistribution->GeneratePosition( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasPosition",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }


  ite = theBiasDistributions.find("PosR2");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( thePosition.perp() );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      thePosition = thePositionDistribution->GeneratePosition( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasPosition",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }


  ite = theBiasDistributions.find("PosR");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( thePosition.mag() );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      thePosition = thePositionDistribution->GeneratePosition( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasPosition",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }


  ite = theBiasDistributions.find("PosTheta");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( thePosition.theta() );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      thePosition = thePositionDistribution->GeneratePosition( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasPosition",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }


  ite = theBiasDistributions.find("PosPhi");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( thePosition.phi() );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      thePosition = thePositionDistribution->GeneratePosition( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasPosition",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }
 
}


//-----------------------------------------------------------------------
void GmParticleSource::BiasDirection()
{
  std::map<G4String,GmVNumericDistribution*>::const_iterator ite = theBiasDistributions.find("DirTheta");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( theDirection.theta() );
      G4double rnd = CLHEP::RandFlat::shoot();
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) 
	G4cout << ii << " GmParticleSource::BiasDirection DirTheta= " << theDirection.theta() << " val= " << val << " rndm " << rnd << G4endl;
#endif
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      theDirection = theDirectionDistribution->GenerateDirection( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasDirection",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }

  ite = theBiasDistributions.find("DirPhi");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( theDirection.phi() );
      G4double rnd = CLHEP::RandFlat::shoot();
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "GmParticleSource::BiasDirection DirPhi " << rnd << " < " << val << G4endl; 
#endif
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      theDirection = theDirectionDistribution->GenerateDirection( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasDirection",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }
}
  

//-----------------------------------------------------------------------
void GmParticleSource::BiasEnergy()
{
  std::map<G4String,GmVNumericDistribution*>::const_iterator ite = theBiasDistributions.find("Energy");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( theEnergy );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      theEnergy = theEnergyDistribution->GenerateEnergy( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasEnergy",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }

}
 
//-----------------------------------------------------------------------
std::vector<G4PrimaryVertex*> GmParticleSource::GenerateVertices( G4double time )
{
  std::vector<G4PrimaryVertex*> vtxs;
  vtxs.push_back(GenerateVertex( time ));

  return vtxs;
}

//-----------------------------------------------------------------------
G4ThreeVector GmParticleSource::PositionInLocalVolume( G4ThreeVector position )
{
  //  G4ThreeVector pos1 = position; //GDEB
  position = theLocalVolumeRotation*position;
  //  G4cout << " LocalVolume pos ROT " << position << " from " << pos1 << G4endl; //GDEB
  position += theLocalVolumePosition;
  //  G4cout << " LocalVolume pos " << position << " from " << position-theLocalVolumePosition << "  " << theLocalVolumePosition << G4endl; //GDEB

  return position;
}


//-----------------------------------------------------------------------
G4ThreeVector GmParticleSource::DirectionInLocalVolume( G4ThreeVector direction )
{
  //  G4ThreeVector dir1 = direction; //GDEB
  direction = theLocalVolumeRotation*direction;
  //  G4cout << " LocalVolume dir " << direction << " from " << dir1 << G4endl; //GDEB

  return direction;

}



//-----------------------------------------------------------------------
void GmParticleSource::ReadEnerDistF()
{
  GmFileIn fin = GmFileIn::GetInstance("enerDistF.lis", true);
  std::vector<G4String> wl;
  for( ;; ){
    if( !fin.GetWordsInLine(wl) ) break;
    theCorrectPosDirEnergyDistFiles[wl[1]] = wl[0];
    //    G4cout << " GmParticleSource::ReadEnerDistF( FILL " << wl[1] << " = " << wl[0] << G4endl; //GDEB
  }
}


//-----------------------------------------------------------------------
void GmParticleSource::ReadCorrectPosDirExpMCProfAir1D(G4String energyNominal)
{
  //--- Get MC and Exp files 
  //results.profAir1D/profAir1D.ScoreAir_228.7_confE228.7_ALL.Exp.csv
  //results.profAir1D/profAir1D.ScoreAirI_070.2_confE070.2_ALL.MC.csv
    
  GmFileIn fin = GmFileIn::GetInstance("profAir1D.lis", true);
  std::vector<G4String> wl;  
  G4String ExpFileName, MCFileName;
  for( ;; ){
    if( !fin.GetWordsInLine(wl) ) break;
    //    G4cout << " ReadCorrectPosDirExpMCProfAir1D read " << wl[0] << G4endl; //GDEB
    if( wl[0].find(energyNominal) != std::string::npos ) {
      if( wl[0].find("Exp") != std::string::npos ) {
	ExpFileName = wl[0];
	//G4cout << " ReadCorrectPosDirExpMCProfAir1D Exp " << ExpFileName << G4endl; //GDEB
      } else {
	MCFileName = wl[0];
	//G4cout << " ReadCorrectPosDirExpMCProfAir1D MC " << MCFileName << G4endl; //GDEB
      }
    }
  }
  
  //--- Divide histograms at X and Y, depth 000
  GmHistoReaderCSV* ExpReader = new GmHistoReaderCSV(ExpFileName);
  GmHistoReaderCSV* MCReader = new GmHistoReaderCSV(MCFileName);
  GmHisto1* ExpHistoX = ExpReader->GetHisto1("h"+energyNominal+"_X_+000", true);
  GmHisto1* MCHistoX = MCReader->GetHisto1("h"+energyNominal+"_X_+000", true);
  G4double MCXmin;
  G4double MCXmax;
  G4double ExpXmin;
  G4double MCXwid = MCHistoX->GetBinWidth(0);
  G4double ExpXwid = ExpHistoX->GetBinWidth(0);
#ifndef GAMOS_NO_ROOT
  MCXmin = MCHistoX->GetXaxis()->GetXmin()+MCXwid/2; // not min value in histogram, but min value for GmPhysicsLinearVector 
  MCXmax = MCHistoX->GetXaxis()->GetXmax()-MCXwid/2;
  ExpXmin = ExpHistoX->GetXaxis()->GetXmin()+ExpXwid/2;
#else
  MCXmin = MCHistoX->GetLowEdge()+MCXwid/2;
  MCXmax = MCHistoX->GetUpperEdge()-MCXwid/2;
  ExpXmin = ExpHistoX->GetLowEdge()+ExpXwid/2;
#endif
  G4int MCNbin = MCHistoX->GetNbinsX();
  //  G4int ExpNbin = ExpHistoX->GetNbinsX();
  G4double MCbinw = MCHistoX->GetBinWidth(0);
  G4double Expbinw = ExpHistoX->GetBinWidth(0);
  //  G4cout << " ReadCorrectPosDirExpMCProfAir1D " << energyNominal << " : MCXminmax " << MCXmin << " " << MCXmax << " " << MCNbin << " " << MCbinw << G4endl; //GDEB
  //G4cout << " ReadCorrectPosDirExpMCProfAir1D " << energyNominal << " : ExpXminmax " << ExpXmin << " " << " " << ExpNbin << " " << Expbinw << G4endl; //GDEB
  std::vector<G4double> binVec; 
  std::vector<G4double> dataVec;
  for( G4int ii = 1; ii < MCNbin+1; ii++ ) { // first is underflow, last overflow
    G4double xval = MCXmin+(ii-1)*MCbinw;
    binVec.push_back(xval);
    G4double dataMC = MCHistoX->GetBinContent(ii);
    G4int ibinExp1 = int((xval-ExpXmin)/Expbinw)+1; // bin 0 is underflow
    G4int ibinExp2 = ibinExp1+1;
    G4double dataExp1 = ExpHistoX->GetBinContent(ibinExp1);
    G4double dataExp2 = ExpHistoX->GetBinContent(ibinExp2);
    //    G4cout << xval << " ibinExp12 " << ibinExp1 << " " << ibinExp2 << " dataExp12 " << dataExp1 << " " << dataExp2 << G4endl; //GDEB
    G4double dataExp = dataExp1 + (dataExp2-dataExp1)/Expbinw*(xval-(ExpXmin+(ibinExp1-1)*Expbinw)); 
    G4cout << energyNominal << " " << ii <<" " << xval << " ReadCorrectPosDirExpMCProfAir1D " << ibinExp1 << " dataExp= "<< dataExp << " = " << dataExp1 <<"+" << (dataExp2-dataExp1)/Expbinw<<"*("<<xval<<"-"<<(ExpXmin+(ibinExp1-1)*Expbinw) << G4endl; //GDEB
    G4double data = 0.;
    if ( dataMC != 0 ) {
      data = dataExp/dataMC;
    }
    dataVec.push_back(data);
    G4cout << ii << " " << xval << " ReadCorrectPosDirExpMCProfAir1D CORR_FACTOR X " << data << " : dataExp " << dataExp << " dataMC " << dataMC << G4endl; //GDEB
  
  }
  
  GmPhysicsLinearVector* distExpMCX = new GmPhysicsLinearVector(MCXmin,MCXmax,MCNbin,false);
  distExpMCX->FillData(binVec,dataVec);
  theCorrectPosDirDistributionsX[energyNominal] = distExpMCX;  
 
  GmHisto1* ExpHistoY = ExpReader->GetHisto1("h"+energyNominal+"_Y_+000", true);
  GmHisto1* MCHistoY = MCReader->GetHisto1("h"+energyNominal+"_Y_+000", true);
  G4double MCYmin;
  G4double MCYmax;
  G4double ExpYmin;
  G4double MCYwid = MCHistoY->GetBinWidth(0);
  G4double ExpYwid = ExpHistoY->GetBinWidth(0);
#ifndef GAMOS_NO_ROOT
  MCYmin = MCHistoY->GetXaxis()->GetXmin()+MCYwid/2;
  MCYmax = MCHistoY->GetXaxis()->GetXmax()-MCYwid/2;
  ExpYmin = ExpHistoY->GetXaxis()->GetXmin()+ExpYwid/2;
#else
  MCYmin = MCHistoY->GetLowEdge()+MCYwid/2;
  MCYmax = MCHistoY->GetUpperEdge()-MCYwid/2;
  ExpYmin = ExpHistoY->GetLowEdge()+ExpYwid/2;
#endif
  MCNbin = MCHistoY->GetNbinsX();
  MCbinw = MCHistoY->GetBinWidth(0);
  Expbinw = ExpHistoY->GetBinWidth(0);
  binVec.clear();
  dataVec.clear();
  for( G4int ii = 1; ii < MCNbin+1; ii++ ) {
    G4double yval = MCYmin+(ii-1)*MCbinw;
    binVec.push_back(yval);
    G4double dataMC = MCHistoY->GetBinContent(ii);
    // G4cout << ii << " yval " << yval <<" " <<  MCYmin  << " " << MCbinw << " dataMC " << dataMC << G4endl; //GDEB
    G4int ibinExp1 = int((yval-ExpYmin)/Expbinw)+1; // bin 0 is underflow
    G4int ibinExp2 = ibinExp1+1;
    G4double dataExp1 = ExpHistoY->GetBinContent(ibinExp1);
    G4double dataExp2 = ExpHistoY->GetBinContent(ibinExp2);
    G4cout << yval << " ibinExp12 " << ibinExp1 << " " << ibinExp2 << " dataExp12 " << dataExp1 << " " << dataExp2 << G4endl; //GDEB
    G4double dataExp = dataExp1 + (dataExp2-dataExp1)/Expbinw*(yval-(ExpYmin+(ibinExp1-1)*Expbinw));
    G4cout << energyNominal << " " << ii << " ReadCorrectPosDirExpMCProfAir1D " << ibinExp1 << " dataExp= "<< dataExp << " = " << dataExp1 <<"+" << (dataExp2-dataExp1)/Expbinw<<"*("<<yval<<"-"<<(ExpYmin+(ibinExp1-1)*Expbinw) << G4endl; //GDEB
    G4double data = 0.;
    if ( dataMC != 0 ) {
      data = dataExp/dataMC;
    }
    dataVec.push_back(data);
    G4cout << ii << " " << yval << " ReadCorrectPosDirExpMCProfAir1D CORR_FACTOR Y " << data << " : dataExp " << dataExp << " dataMC " << dataMC << G4endl; //GDEB
  }

  GmPhysicsLinearVector* distExpMCY = new GmPhysicsLinearVector(MCYmin,MCYmax,MCNbin,false);
  distExpMCY->FillData(binVec,dataVec);
  theCorrectPosDirDistributionsY[energyNominal] = distExpMCY;  
    
}


//-----------------------------------------------------------------------
G4bool GmParticleSource::CorrectPosDir()
{
  GmGenerDistEnergyFromMultiFileE* energyDistFMF = dynamic_cast<GmGenerDistEnergyFromMultiFileE*>(theEnergyDistribution);
  GmGenerDistEnergyConstant* energyDistConst = dynamic_cast<GmGenerDistEnergyConstant*>(theEnergyDistribution);
  G4String energyNominal; 
  if( energyDistFMF ) {
    GmGenerDistEnergyFromFile* energyFileD = energyDistFMF->GetCurrentDist();
    G4String energyFileName = energyFileD->GetFileName(); // enerAfterNozzle.221.73-s0.00.lis
    G4String energyReal = energyFileName.substr(16,6);
    std::map<G4String,G4String>::const_iterator itee = theCorrectPosDirEnergyDistFiles.find(energyFileName);
    if( itee == theCorrectPosDirEnergyDistFiles.end() ) {
      G4Exception("GmParticleSource CorrectPosDir",
		  "",
		  FatalException,
		  ("no energy distribution file for energy "+energyFileName).c_str());
    }
    energyNominal = itee->second;
  } else  if( energyDistConst ) {
    energyNominal = GmGenUtils::ftoa(energyDistConst->GetEnergy());

  } else {
    G4Exception("GmParticleSource::CorrectPosDir",
		"exc1",
		FatalException,
		"energy distribution is not of type GmGenerDistEnergyFromFile");
  }

  //G4cout << " bCorrectPosDir energyReal " << energyReal << " energyFileName " << energyFileName << " energyNominal " <<energyNominal << G4endl; //GDEB
  std::map<G4String,GmPhysicsLinearVector*>::const_iterator itepd = theCorrectPosDirDistributionsX.find(energyNominal);
  if( itepd == theCorrectPosDirDistributionsX.end() ) {
    ReadCorrectPosDirExpMCProfAir1D(energyNominal);
  }
  /*    for( std::map<G4String,GmPhysicsLinearVector*>::const_iterator itepd = theCorrectPosDirDistributionsX.begin(); itepd != theCorrectPosDirDistributionsX.end(); itepd++ ) {
	G4cout << " READING theCorrectPosDirDistributionsX " << itepd->first << " " << itepd->second << G4endl; //GDEB
	}*/
  itepd = theCorrectPosDirDistributionsX.find(energyNominal);
  GmPhysicsLinearVector* posDirCorrDistX = itepd->second;
  //G4cout << " READ theCorrectPosDirDistributionsX " << " = " <<posDirCorrDistX << G4endl; //GDEB
  //G4cout << " READ theCorrectPosDirDistributionsX " << itepd->first << " " << itepd->second << " = " <<posDirCorrDistX << G4endl; //GDEB
  itepd = theCorrectPosDirDistributionsY.find(energyNominal);
  GmPhysicsLinearVector* posDirCorrDistY = itepd->second;
  //G4cout << " READ theCorrectPosDirDistributionsY " << " = " <<posDirCorrDistY << G4endl; //GDEB
  //G4cout << " READ theCorrectPosDirDistributionsY " << itepd->first << " " << itepd->second << " = " <<posDirCorrDistY << G4endl; //GDEB
  // Extrapolate position to isocenter plane (at 0,0,0)
  G4double distZ = -thePosition.z(); 
  G4double posX = thePosition.x() + theDirection.x()*distZ;
  //    if( abs(posX) > posDirCorrDistX->GetMaxEnergy() ) return false; // assume histograms is symmetrical
  G4double posY = thePosition.y() + theDirection.y()*distZ;
  //    if( abs(posY) > posDirCorrDistY->GetMaxEnergy() ) return false; // assume histograms is symmetrical
  G4double posR2 = sqrt(posX*posX+posY*posY);
  if( abs(posR2) > posDirCorrDistX->GetMaxEnergy() ) return false; // assume histograms is symmetrical
  //    G4cout << " POSXY " <<posX << " " <<posY << " POS " << thePosition << " DIR " << theDirection << G4endl; //GDEB
  G4bool bOutOfRange = false;
  G4double corrFactorX = posDirCorrDistX->GetValue(posR2*std::copysign(1.0, posX),bOutOfRange) * 0.9; // must be smaller than 1, if not
  if( corrFactorX > 1. || corrFactorX == 0. ) {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " corrFactorX TOO BIG, made 1 " << corrFactorX << " posX " << posX << G4endl;
#endif
    corrFactorX = 1.; // do not count values with big abs(X), which have a big correction factor
  }
  G4double corrFactorY = posDirCorrDistY->GetValue(posR2*std::copysign(1.0, posY),bOutOfRange) * 0.9;
  if( corrFactorY > 1. || corrFactorY == 0. ) {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " corrFactorY TOO BIG, made 1 " << corrFactorY << " posY " << posY << G4endl;
#endif
    corrFactorY = 1.; // do not count values with big abs(Y), which have a big correction factor
  }
  /*    if ( posY > 50 ) {
	corrFactorY = 1.; // do not count values with big abs(Y), which have a big correction factor
	}*/
  // G4cout << posDirCorrDistX << " CorrectPosDir posX " << posX << " " << corrFactorX << " posY " << posY << " " << corrFactorY << G4endl; //GDEB
  
  //--- Get CorrFactor R2
  G4double corrFactorR2;
  if( posX != 0. ) {
    G4double angle = atan(posY/posX);
    //  G4cout << " ANGLE0 " << angle << ":" << angle/CLHEP::deg << " " << posX << " " << posY << G4endl; //GDEB
    angle = std::fmod(angle,M_PI);
    //      G4cout << " ANGLE1 " << angle << ":" << angle/CLHEP::deg << G4endl; //GDEB
    corrFactorR2 = (corrFactorX*(M_PI/2-angle)+corrFactorY*angle)/(M_PI/2);
    //      G4cout << " corrFactorR2 " << corrFactorR2 << " FROM corrFactorXY " << corrFactorX << " " << corrFactorY << " posX " << posX <<" posY " << posY << "  angle=" << angle/CLHEP::deg << " " <<  atan(posY/posX)/CLHEP::deg << G4endl; //GDEB
  } else {
    corrFactorR2 = corrFactorY;
  }
  G4double randVal = CLHEP::RandFlat::shoot();
  //    G4cout << " RANDOMR2 " << randVal << " >? " << corrFactorR2 << G4endl; //GDEB
  if( randVal > corrFactorR2 ) return false;
  //    G4cout << " GmParticleSource::CorrectPosDir OK " << posX << " " << posY << G4endl; //GDEB
  
  return true;
}

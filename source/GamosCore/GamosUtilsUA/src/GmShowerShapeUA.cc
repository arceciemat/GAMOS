#include "GmShowerShapeUA.hh"
#include "GmUtilsUAVerbosity.hh"

#include "G4Event.hh"
#include "G4Step.hh"
#include "G4Track.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmLine.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmShowerShapeUA::GmShowerShapeUA()
{
  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  G4String spu = paramMgr->GetStringValue(theName+":StepPointToUse","Linear");
  if( spu == "Pre" ) {
    thePointToUse = TSPUPre; // what to store in GmSSData second to last steps
    theInterpolationType = SSITPre;  // what to use for step position
  } else if( spu == "Post" ) {
    thePointToUse = TSPUPost;
    theInterpolationType = SSITPost;
  } else if( spu == "Linear" ) {
    thePointToUse = TSPUPrePost;
    theInterpolationType = SSITLinear;
 } else {
    G4Exception("GmShowerShapeUA::GmShowerShapeUA",
		"Wrong value of parameter GmShowerShapeUA:StepPointToUse",
		FatalErrorInArgument,
		G4String("It must be a Pre, Post or Linear, while it is " + spu).c_str());
  }

  spu = paramMgr->GetStringValue(theName+":ShowerDirection","FirstTrack");
  if( spu == "FirstTrack" ) {
    bShowerDirection1stTrack = true;
    //    bStorePreInFirstTrack = true;
  } else if( spu == "Shower" ) { 
    bShowerDirection1stTrack = false;
    //    bStorePreInFirstTrack = false;
 } else {
    G4Exception("GmShowerShapeUA::GmShowerShapeUA",
		"Wrong value of parameter GmShowerShapeUA:ShowerDirection",
		FatalErrorInArgument,
		G4String("It must be a FirstTrack or Shower, while it is " + spu).c_str());
  }

  bIncludeOtherStepsOfFirstTrack = G4bool(paramMgr->GetNumericValue(theName+":IncludeOtherStepsOfFirstTrack",1));

  theRadii.insert(0.1);
  theRadii.insert(0.3);
  theRadii.insert(0.5);
  theRadii.insert(1.);
  theRadii.insert(5.);
  theRadii.insert(DBL_MAX);

  std::vector<G4double> rad;
  rad = paramMgr->GetVNumericValue(theName+":Radii",rad);

  if( rad.size() != 0 ) theRadii.clear();
  for( unsigned int ii = 0; ii < rad.size(); ii++ ){
    if( rad[ii] <= 0. ) {
      G4Exception("GmShowerShapeUA::GmShowerShapeUA",
		  "Wrong value of parameter GmShowerShapeUA:Radii",
		  FatalErrorInArgument,
		  G4String("It must be a positive value, while it is " + GmGenUtils::ftoa(rad[ii])).c_str());
    }
    theRadii.insert( rad[ii] );
  }

  theTextFileName = paramMgr->GetStringValue(theName+":WriteTextFileName","");
  if( theTextFileName == "" ) {
    theTextFileOut = 0;
  }

  theBinFileName = paramMgr->GetStringValue(theName+":WriteBinFileName","");
  if( theBinFileName == "" ) {
    theBinFileOut = 0;
  }

}


//----------------------------------------------------------------
void GmShowerShapeUA::InitialiseHistoNames()
{
  SetHistoNameAndNumber("shower",theFilters,theClassifier);
}

//----------------------------------------------------------------
void GmShowerShapeUA::BeginOfRunAction( const G4Run* )
{
  if( theTextFileName != "" ) { 
    theTextFileOut = new std::ofstream(theTextFileName);
  }

  if( theBinFileName != "" ) { 
    theBinFileOut = fopen(theBinFileName,"wb");
  }

}


//----------------------------------------------------------------
void GmShowerShapeUA::BookHistos(G4int index)
{
  theHistos.insert(index);

  G4String hname;
  if( theClassifier ) hname += theClassifier->GetIndexName(index);
  G4String histoName = theHistoName + "_" + hname;
  G4int histoNumber = theHistoNumber + index*100;

  // Create histograms
  theAnaMgr->CreateHisto1D(theHistoName+": Total Energy",100,0.,1,histoNumber+1);
  theAnaMgr->CreateHisto1D(theHistoName+": Maximum R transv",100,0.,1000.,histoNumber+2);
  theAnaMgr->CreateHisto1D(theHistoName+": Maximum R long",100,0.,1000.,histoNumber+3);
  theAnaMgr->CreateHisto1D(theHistoName+": Average R transv",100,0.,1000.,histoNumber+4);
  theAnaMgr->CreateHisto1D(theHistoName+": Average R long",100,0.,1000.,histoNumber+5);
  theAnaMgr->CreateHisto1D(theHistoName+": Shower direction theta",100,0.,180.,histoNumber+6);
  theAnaMgr->CreateHisto1D(theHistoName+": Shower direction phi",100,-180.,180.,histoNumber+7);

  std::set<G4double>::const_iterator ite;
  unsigned int ii = 0;
  for( ite = theRadii.begin(); ite != theRadii.end(); ite++, ii++ ) {
    G4String hname2 = theHistoName+": Energy inside radius " + GmGenUtils::ftoa((*ite));
    theAnaMgr->CreateHisto1D(hname2,100,0.,1.,histoNumber+10+ii);

    hname2 = theHistoName+": Relative energy in radius " + GmGenUtils::ftoa((*ite));
    theAnaMgr->CreateHisto1D(hname2,100,0.,1.,histoNumber+20+ii);
  }
  
}


//----------------------------------------------------------------
void GmShowerShapeUA::UserSteppingAction(const G4Step* aStep )
{
#ifndef GAMOS_NO_VERBOSE
  if( UtilsUAVerb(debugVerb) ) G4cout << " GmShowerShapeUA::UserSteppingAction " << G4endl;
#endif
  
  G4Track* aTrack = const_cast<G4Track*>(aStep->GetTrack());
  G4int showerID; 
  
  GmSSData* ssdata = CheckWhichShower( aStep );
  GmTrackInfo* trkInfo = 0;
  if( aTrack->GetUserInformation() == 0 ) {
    trkInfo = new GmTrackInfo();
    //	G4cout << " G4TrackInfo CREATE " << trkInfo  << G4endl; //GDEB
  } else {
    G4VUserTrackInformation* g4TrkInfo = aTrack->GetUserInformation();
    if( ! dynamic_cast<GmTrackInfo*>(g4TrkInfo) ) {
      G4Exception("GmAncestorsFilter::AcceptStep",
		  "",
		  FatalException,
		  "Creating a GmTrackInfo for a track that has already a G4VUserTrackInformation");
    } else {
      trkInfo = dynamic_cast<GmTrackInfo*>(g4TrkInfo);
    }
  }

  if( ! ssdata ) {
    showerID = theShowerData.size();
    ssdata = new GmSSData( theShowerData.size(), aStep, thePointToUse );
    theShowerData[theShowerData.size()] = ssdata;
    
    G4int index;
    if (theClassifier ) {
      index = theClassifier->GetIndexFromStep(aStep);
    } else {
      index = 0;
    }
    if( theHistos.find(index) == theHistos.end() ) BookHistos(index);
    ssdata->SetClassifierIndex( index );
    
  } else {
    showerID = ssdata->GetShowerID(); 
    ssdata->AddStep( aStep );
  }
  trkInfo->SetIntValue("showerID", showerID );
  aTrack->SetUserInformation( trkInfo ); 
  
  G4TrackVector fSec = GmG4Utils::GetSecondariesOfCurrentStep();
  G4TrackVector::iterator ite;
  for( ite = fSec.begin(); ite != fSec.end(); ite++ ) {
    GmTrackInfo* trkInfo2 = 0;
    if( (*ite)->GetUserInformation() == 0 ) {
      trkInfo2 = new GmTrackInfo();
      //	G4cout << " G4TrackInfo CREATE " << trkInfo2  << G4endl; //GDEB
    } else {
      G4VUserTrackInformation* g4TrkInfo = (*ite)->GetUserInformation();
      if( ! dynamic_cast<GmTrackInfo*>(g4TrkInfo) ) {
	G4Exception("GmAncestorsFilter::AcceptStep",
		    "",
		    FatalException,
		    "Creating a GmTrackInfo for a track that has already a G4VUserTrackInformation");
      } else {
	trkInfo2 = dynamic_cast<GmTrackInfo*>(g4TrkInfo);
      }
    }    
    trkInfo2->SetIntValue("showerID", showerID );
    (*ite)->SetUserInformation( trkInfo2 ); 
  }
}


//----------------------------------------------------------------
GmSSData* GmShowerShapeUA::CheckWhichShower( const G4Step* aStep )
{

  G4VUserTrackInformation* trkInfo = aStep->GetTrack()->GetUserInformation();
  
  if( !trkInfo ) {
    return (GmSSData*)0;
  } else {
    GmTrackInfo* ssTrkInfo = static_cast<GmTrackInfo*>(trkInfo);
    G4int showerID = ssTrkInfo->GetIntValue("showerID");
    if( ! bIncludeOtherStepsOfFirstTrack ) {
      if( aStep->GetTrack()->GetTrackID() == theShowerData[showerID]->GetFirstTrackID() ) {
	return (GmSSData*)0;
      }
    }
    return theShowerData[showerID];
  }
  
}

//----------------------------------------------------------------
void GmShowerShapeUA::EndOfEventAction( const G4Event* evt )
{
  G4int it1 = 0;  
  std::map<G4int, GmSSData*>::const_iterator ite;
  if( theTextFileOut ) DumpText( evt );
  if( theBinFileOut ) DumpBin( evt );

  for( ite = theShowerData.begin(); ite != theShowerData.end(); ite++ ){
    GmSSData* ssdata = (*ite).second;
    std::vector<GmTrajStep*> steps = ssdata->GetSteps();
#ifndef GAMOS_NO_VERBOSE
    if( UtilsUAVerb(testVerb) ) G4cout << " Looping to shower " << it1++ << " Nsteps " << steps.size() << G4endl; 
#endif

    if( theTextFileOut ) ssdata->Dump( *theTextFileOut );
    if( theBinFileOut ) ssdata->Dump( theBinFileOut );

    if( steps.size() == 1 ) continue;

    G4ThreeVector dir1;
    if( bShowerDirection1stTrack ) {
      dir1 = ssdata->GetFirstTrackDir();
    } else {
      dir1 = GetShowerDirection(ssdata);
    }

    if( dir1.mag() == 0 ) {
      if( UtilsUAVerb(warningVerb) ) G4Exception("GmShowerShapeUA::EndOfEventAction",
		  "Direction of first track in shower is (0,0,0)",
		  JustWarning,
		  "This shower will not be considered");
      continue;
    }
    
    G4int histoNumber = theHistoNumber + ssdata->GetClassifierIndex()*100;
    theAnaMgr->GetHisto1(histoNumber+6)->Fill( dir1.theta()/CLHEP::deg );
    theAnaMgr->GetHisto1(histoNumber+7)->Fill( dir1.phi()/CLHEP::deg );
    G4double totalEnergy = ssdata->GetTotalEnergy();
    theAnaMgr->GetHisto1(histoNumber+1)->Fill( totalEnergy );

    G4ThreeVector pos1 = ssdata->GetFirstTrackPos();
    GmLine line( pos1, dir1 );
    std::vector<GmTrajStep*>::const_iterator itep;
    G4double maxDistTransv = 0.;
    G4double maxDistLong = 0.;
    G4double totDistTransv = 0.;
    G4double totDistLong = 0.;
    G4double totEner = 0.;
    std::vector<G4double> EnerInRadii(theRadii.size());
    std::vector<G4double>::reverse_iterator ritee;
    std::vector<G4double>::const_iterator itee;
    std::set<G4double>::const_iterator iterf;
    std::set<G4double>::reverse_iterator riter;

    itep = steps.begin();
    itep++;  // do not compare first step with itself
    for( ; itep != steps.end(); itep++ ) {
      G4ThreeVector stepPos;
      if( theInterpolationType == SSITPre ) {
	stepPos = (*itep)->GetPositionPre();
      } else if( theInterpolationType == SSITPost ) {
	stepPos = (*itep)->GetPositionPost();
      } else if( theInterpolationType == SSITLinear ) {
	stepPos = ( (*itep)->GetPositionPre() + (*itep)->GetPositionPost() ) / 2.;
      }
      
      G4double distTransv = line.GetDist( stepPos );
      G4double distLong = (pos1 - stepPos ).mag();

      maxDistTransv = std::max( maxDistTransv, distTransv );
      maxDistLong = std::max( maxDistLong, distLong );

      G4double depoE = (*itep)->GetDepositedEnergy();
      totDistTransv += distTransv * depoE;
      totDistLong += distLong * depoE;
      totEner += depoE;

      //--- Assign E depo to circles
      iterf = theRadii.upper_bound(distTransv);
      for( riter = theRadii.rbegin(), ritee = EnerInRadii.rbegin(); riter != theRadii.rend(); riter++, ritee++ ) {
	(*ritee) += depoE;    
#ifndef GAMOS_NO_VERBOSE
	if( UtilsUAVerb(testVerb) ) G4cout << *riter << " " << *iterf << " EnerInRadii " << distTransv << " DepoE " << depoE << " totE " << *ritee << G4endl;
#endif
	if( *riter == *iterf ) break;
      }
    }
    G4double aveDistTransv = totDistTransv/totEner;
    G4double aveDistLong = totDistLong/totEner;
    theAnaMgr->GetHisto1(histoNumber+2)->Fill( maxDistTransv );
    theAnaMgr->GetHisto1(histoNumber+3)->Fill( maxDistLong );
    theAnaMgr->GetHisto1(histoNumber+4)->Fill( aveDistTransv );
    theAnaMgr->GetHisto1(histoNumber+5)->Fill( aveDistLong );
    unsigned int ii = 0;
    for( itee = EnerInRadii.begin(); itee != EnerInRadii.end(); itee++, ii++ ) {
      if( *itee != 0. ) {
	theAnaMgr->GetHisto1(histoNumber+10+ii)->Fill( *itee );
	theAnaMgr->GetHisto1(histoNumber+20+ii)->Fill( *itee / totalEnergy );
      }
    }
    
  }
  
  theShowerData.clear();  
  
}

//----------------------------------------------------------------
G4ThreeVector GmShowerShapeUA::GetShowerDirection( GmSSData* ssdata)
{
  G4ThreeVector showerDir;

  std::vector<GmTrajStep*> steps = ssdata->GetSteps();
  std::vector<GmTrajStep*>::const_iterator itep = steps.begin();
  G4ThreeVector stepPos1st = (*itep)->GetPositionPost();
  itep++;  // do not compare first step with itself
  for( ; itep != steps.end(); itep++ ) {
    G4ThreeVector stepPos;
    if( theInterpolationType == SSITPre ) {
      stepPos = (*itep)->GetPositionPre();
    } else if( theInterpolationType == SSITPost ) {
      stepPos = (*itep)->GetPositionPost();
    } else if( theInterpolationType == SSITLinear ) {
      stepPos = ( (*itep)->GetPositionPre() + (*itep)->GetPositionPost() ) / 2.;
    }
    G4ThreeVector diffPos = stepPos - stepPos1st;
    showerDir += diffPos * (*itep)->GetDepositedEnergy();
  }

#ifndef GAMOS_NO_VERBOSE
  if( UtilsUAVerb(debugVerb) ) G4cout << "GmShowerShapeUA::GetShowerDirection " << showerDir/showerDir.mag() << G4endl;
#endif

  return showerDir / showerDir.mag();

}

//----------------------------------------------------------------
void GmShowerShapeUA::DumpText( const G4Event* evt  )
{
  *theTextFileOut << evt->GetEventID() << G4endl;
}

//----------------------------------------------------------------
void GmShowerShapeUA::DumpBin( const G4Event* evt  )
{
  G4int data = evt->GetEventID();
  fwrite (&data, sizeof(G4int),1,theBinFileOut); 
}

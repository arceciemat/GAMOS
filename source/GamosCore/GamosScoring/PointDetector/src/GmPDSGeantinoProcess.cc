#define _USE_MATH_DEFINES
#include <cmath>

#include "GmPDSGeantinoProcess.hh"
#include "GmPDSProcessHelper.hh"
#include "GmPDSInvertGeantinoStackUA.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GmPDSUtils.hh"

#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Geantino.hh"
#include "G4SteppingManager.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#ifdef WIN32 //WINDEB
#define GAMOS_NO_VERBOSE 
#endif

#include <cmath>

//------------------------------------------------------------------
GmPDSGeantinoProcess::GmPDSGeantinoProcess(const G4String& name, G4SteppingManager* fpSM, G4ParticleDefinition* origParticle ): GmPDSVProcess( name, fpSM )
{
#ifndef GAMOS_NO_VERBOSE
	if(ScoringVerb(testVerb)) G4cout << "GmPDSGeantinoProcess::GmPDSGeantinoProcess " << this << G4endl;
#endif
  theOriginalParticle = origParticle;
  G4String partName = origParticle->GetParticleName();

  bUseOriginalParticleSplitting = G4bool(GetPDSNumericParameter("UseVRSplitting",partName,0));
  if(bUseOriginalParticleSplitting) {
    //-    GmUserActionFactory::get()->create("GmPDSInvertGeantinoStackUA");
    theInverseStackingUA = GmPDSInvertGeantinoStackUA::GetInstance();
  }

  theMinimumGeantinoWeight = GetPDSNumericParameter("MinimumGeantinoWeight",partName,1.E-100);
  theInvMinimumGeantinoWeightRR = 1./GetPDSNumericParameter("MinimumGeantinoWeightRRFactor",partName,100.);
  
  bUseMinimumGeantinoWeight = G4bool(GetPDSNumericParameter("UseVRMinimumGeantinoWeight",partName,0));
  theMinimumWeightForSplitting = log(GetPDSNumericParameter("MinimumWeightForSplitting",partName,1.E-30));
  theMaximumWeightForSplitting = log(GetPDSNumericParameter("MaximumWeightForSplitting",partName,1.E-5));
  theMaximumSplitting = GetPDSNumericParameter("MaximumSplitting",partName,100.);
  theSplittingFactor = log(theMaximumSplitting)/(theMaximumWeightForSplitting-theMinimumWeightForSplitting);

  theExclusionRadius = GetPDSNumericParameter("ExclusionRadius",partName,1.*CLHEP::mm);

}
  
//------------------------------------------------------------------
GmPDSGeantinoProcess::~GmPDSGeantinoProcess()
{
}

//------------------------------------------------------------------
G4VParticleChange* GmPDSGeantinoProcess::PostStepDoIt( const G4Track& aTrack, const G4Step& aStep)
{

  aParticleChange.Initialize(aTrack);

  G4VUserTrackInformation* trki = aTrack.GetUserInformation();
  GmTrackInfo* gmtrki = dynamic_cast<GmTrackInfo*>( trki );
  //  theCurrentHelper = theHelpers[gmtrki->IsFromNeutron]; // get the GmPDSNeutronProcess or the GmPDSGammaProcess helper
  if( gmtrki->GetParticle() != theOriginalParticle ) return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  
  if( aTrack.GetKineticEnergy() == 0. ) return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);

  ChangeGeantinoWeight(&aStep);

  G4int DR = theCurrentHelper->DetectorReached( aStep );
  if( DR != -1 ) { 
    if( bUseOriginalParticleSplitting ) SplitOriginalParticle( aTrack, aStep, aParticleChange );
   
    G4double wei = GetWeight(&aTrack);
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(testVerb) ) G4cout << "GmPDSGeantinoProcess::PostStepDoIt weight " << wei << G4endl;
#endif
    
    theCurrentHelper->FillScores(aTrack, 1, DR);

    /*    G4Track* aTrackNC = const_cast<G4Track*>(aStep.GetTrack());
    G4cout << " SetTrackStatus fStopAndKill " << G4endl;
    //    const_cast<G4Track*>(&aTrack)->SetTrackStatus( fStopAndKill );
    aTrackNC->SetTrackStatus( fStopAndKill );*/
    aParticleChange.ProposeEnergy(0.);

    G4double dist =theCurrentHelper->GetDistanceToDetectorIncm( aTrack.GetVertexPosition(),DR );
    G4int nhALL = 0;
    if( theCurrentHelper->bControlHistos ) {
      nhALL = theCurrentHelper->theHistoNumber + 100000 * gmtrki->GetIntValue("ClassifierIndex") + 100 * DR;
      theCurrentHelper->theAnaMgr->GetHisto1(nhALL+91)->Fill(dist*CLHEP::cm);
      theCurrentHelper->theAnaMgr->GetHisto2(nhALL+92)->Fill(dist*CLHEP::cm,log10(wei));
    }
    std::map<G4double,Flux2Dose>::iterator ite = theCurrentHelper->theFlux2Dose.upper_bound( aTrack.GetVertexKineticEnergy());
    if( ite == theCurrentHelper->theFlux2Dose.end() ) {
      ite--;
      G4Exception("GmPDSGeantinoProcess::PostStepDoIt",
		  "energy out of bounds",
		  JustWarning,
		  G4String("Energy="+GmGenUtils::ftoa(aTrack.GetVertexKineticEnergy()) + " bigger than biggest energy in Flux2Dose coefficients" + GmGenUtils::ftoa((*ite).first) ).c_str());
    }
    if( theCurrentHelper->bControlHistos ) {
      theCurrentHelper->theAnaMgr->GetHisto2(nhALL+93)->Fill(dist*CLHEP::cm,log10(wei) * (*ite).second.Hstar);
    }

  } else {
    if( bUseMinimumGeantinoWeight ) {
      //---- Play Russian Roulette if weight is small
      G4double wei = GetWeight(&aTrack);
      if( wei < theMinimumGeantinoWeight ) {
	if( CLHEP::RandFlat::shoot() < theInvMinimumGeantinoWeightRR ) {
	  G4Track* aTrackNC = const_cast<G4Track*>(aStep.GetTrack());
	  SetWeight( aTrackNC, wei / theInvMinimumGeantinoWeightRR );
#ifndef GAMOS_NO_VERBOSE
	  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSGeantinoProcess::PostStepDoIt low weight " << wei << " weight increased by Russian Roulette to " << wei / theInvMinimumGeantinoWeightRR << G4endl;
#endif
	} else {
#ifndef GAMOS_NO_VERBOSE
	  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSGeantinoProcess::PostStepDoIt low weight " << wei << " killed by Russian Roulette " << G4endl;
#endif
	  aParticleChange.ProposeEnergy(0.);
	}
      }
    }
  }

  if( aTrack.GetNextVolume() == 0 ) { //OutOfWorld
    G4cout << " INITIAL POINT " << aTrack.GetVertexPosition() << G4endl
	   << " INITIAL DIR " << aTrack.GetVertexMomentumDirection() << G4endl
	   << " CURRENT POINT " << aTrack.GetPosition() << G4endl 
	   << " CURRENT DIR " << aTrack.GetMomentumDirection() << G4endl; 

    G4Exception("GmPDSGeantinoProcess::PostStepDoIt  track reaches OutOfWorld, direction is then wrong",
		"",
		JustWarning,
		G4String("EVT " + GmGenUtils::itoa(G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID()) + "TRACK " + GmGenUtils::itoa(aTrack.GetTrackID())).c_str());
  }
  
  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

//------------------------------------------------------------------
void GmPDSGeantinoProcess::SplitOriginalParticle( const G4Track& aTrack, const G4Step& aStep, G4ParticleChange& aParticleChange2 )
{
  G4double logwei = log(GetWeight(&aTrack));
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << "GmPDSGeantinoProcess::SplitOriginalPartile logwei " << logwei << " minimum " << theMinimumWeightForSplitting << " maximum " << theMaximumWeightForSplitting << G4endl;
#endif
  if( logwei > theMinimumWeightForSplitting && logwei < theMaximumWeightForSplitting ) {
    G4VUserTrackInformation* trki = aTrack.GetUserInformation();
    GmTrackInfo* gmtrki = dynamic_cast<GmTrackInfo*>( trki );
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(testVerb) ) G4cout << "GmPDSGeantinoProcess::SplitOriginalPartile is particle split already? " << gmtrki->GetBoolValue("IsOriginalParticleSplit") << G4endl;
#endif
    
    if(!gmtrki->GetBoolValue("IsOriginalParticleSplit")) { 
      
      G4int nSplit = G4int(exp(( logwei - theMinimumWeightForSplitting ) * theSplittingFactor ));
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(testVerb) ) 
      G4cout << " NSPLIT " << nSplit << " =  " << exp(( logwei - theMinimumWeightForSplitting ) * theSplittingFactor) << " log(wei) " << log10( exp(logwei)) <<	" theMinimumWeightForSplitting " << log10(exp(theMinimumWeightForSplitting)) << " theSplittingFactor " << log10(exp(theSplittingFactor)) << G4endl;
#endif

#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << "GmPDSGeantinoProcess::SplitOriginalPartile nSplit= " << nSplit << " event " << G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID() << " parent " << aTrack.GetParentID() << " log(wei) " << logwei  << " log(minWeight) " << theMinimumWeightForSplitting << " log(maxWeight) " << theMaximumWeightForSplitting << G4endl;
#endif
      G4double weinew = exp(logwei)/nSplit;
      G4String procDefStepName = aStep.GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
      G4Point3D point; // dummy
      aParticleChange2.SetNumberOfSecondaries(nSplit);
      for( G4int ii = 0; ii < nSplit-1; ii++ ) {
	G4Track* trk = new G4Track( new G4DynamicParticle( theOriginalParticle, gmtrki->GetThreeVectorValue("OriginalParticleDir"), aTrack.GetVertexKineticEnergy() ), aTrack.GetGlobalTime()-aTrack.GetLocalTime(), aTrack.GetVertexPosition() );
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << "GmPDSGeantinoProcess::SplitOriginalPartile  new G4Track ORIGINAL_PARTICLE: " << theOriginalParticle->GetParticleName() << " dir " << gmtrki->GetThreeVectorValue("OriginalParticleDir") << " =? " << aTrack.GetVertexMomentumDirection() << " pos " <<  aTrack.GetVertexPosition() << " ener " << aTrack.GetVertexKineticEnergy() << G4endl;
#endif
	trk->SetGoodForTrackingFlag();
	trk->SetTouchableHandle(aTrack.GetTouchableHandle());
	GmTrackInfo* gmtrkiOrig = new GmTrackInfo("GmPDSTrackInfo");
	gmtrkiOrig->SetStringValue("ProcessName",procDefStepName);
	gmtrkiOrig->SetThreeVectorValue("Point",point);
	//	gmtrkiOrig->SetBoolValue("IsOriginalParticleSplit",isSplit);
	gmtrkiOrig->SetBoolValue("IsOriginalParticleSplit",true);
	
	trk->SetUserInformation( gmtrkiOrig );

	SetWeight(trk, gmtrki->GetDoubleValue("ParentWeight")/nSplit);
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << trk << " GmPDSGeantinoProcess::SplitOriginalPartile  new weight " << weinew << " = " << trk->GetWeight() << " = " << GetWeight(trk) << " old weight " << exp(logwei) << " pos " <<  trk->GetVertexPosition() << G4endl;
#endif
	aParticleChange2.AddSecondary(trk);
      }
      // Diminish weight of all geantinos in the TrackStack, including this one, if they point to detector
      if( nSplit > 1 ) theInverseStackingUA->ChangeGeantinoWeight( 1./nSplit, aTrack.GetParentID(), gmtrki->GetIntValue("DetectorID") );
      G4Track* aTrackNC = const_cast<G4Track*>(&aTrack);
      SetWeight(aTrackNC, weinew); // diminish weight of current geantino 
    }
  }


}

//------------------------------------------------------------------
void GmPDSGeantinoProcess::ChangeGeantinoWeight(const G4Step* aStep)
{
  //----- Get number of interaction lengths travelled
  const G4Track* aTrack = aStep->GetTrack();
  G4Track* aTracknc = const_cast<G4Track*>(aTrack);

#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(testVerb) ) {
    G4VUserTrackInformation* trki = aTrack->GetUserInformation();
    GmTrackInfo* gmtrki = dynamic_cast<GmTrackInfo*>( trki );
    G4Point3D point = gmtrki->GetThreeVectorValue("Point"); // geantino has in TrackInformation the point it is aimed to
    G4ThreeVector trkpos  = aTrack->GetPosition();
    G4ThreeVector stepPtDir = point - trkpos;
    stepPtDir = stepPtDir.unit();
    G4ThreeVector trkdir = aTrack->GetMomentumDirection();
    G4double distpt = (point-trkpos).mag();
    G4cout << "GmPDSGeantinoProcess::ChangeGeantinoWeight  dir_to_point = " << stepPtDir << " dir_track " << trkdir << " distance_to_point " << distpt << G4endl;
	}
#endif

  //----- Get mean free path  
  // G4double nmfp = aStep->GetStepLength() / theMeanFreePathCalculator->GetMeanFreePath("neutron",aStep->GetTrack()->GetKineticEnergy(),aStep->GetTrack()->GetVolume()->GetLogicalVolume()->GetMaterial()->GetName();
  G4double XS = theCurrentHelper->GetCrossSection( aStep );

  //----- Check if geantino is created inside the exclusion sphere
  G4ThreeVector vtx = aTrack->GetVertexPosition();
  G4VUserTrackInformation* trki = aTrack->GetUserInformation();
  GmTrackInfo* gmtrki = dynamic_cast<GmTrackInfo*>( trki );
  G4Point3D point = gmtrki->GetThreeVectorValue("Point"); // geantino has in TrackInformation the point it is aimed to
  G4double dist = (point-vtx).mag()/CLHEP::cm;
  if( dist < theExclusionRadius/CLHEP::cm ){
    G4double eRcm = theExclusionRadius/CLHEP::cm;
    G4double wei = GetWeight(aTracknc);
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(infoVerb) ) G4cout << "GmPDSGeantinoProcess::ChangeGeantinoWeight  particle in exclusion sphere " << dist*CLHEP::cm << " < " << theExclusionRadius*CLHEP::cm << " WEIGHT0 = " << wei << G4endl;
#endif
    //--- Take out the 1/2PIR^2 radius
    wei *= (2.*(acos(0.)*2)*pow( dist,2 ) );
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(infoVerb) ) G4cout << " GmPDSGeantinoProcess::ChangeGeantinoWeight  particle in exclusion sphere " << dist*CLHEP::cm << " WEIGHT1 = " << wei << G4endl;
#endif
    
    G4double mfpcm = (1./XS)/CLHEP::cm;
    //    wei *= (2/pow(XS,3.)-exp(-theExclusionRadius*XS)*(2.*pow(mfpcm,3.)+2*eRcm*mfpcm*mfpcm+eRcm*eRcm*mfpcm))/(pow(eRcm,3.)/mfpcm/3.);
    wei *= (1-exp(-theExclusionRadius*XS))/(2./3.*CLHEP::pi*pow(eRcm,3.)/mfpcm);
    SetWeight( aTracknc, wei );
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(warningVerb) ) G4cerr << " GmPDSGeantinoProcess::ChangeGeantinoWeight  particle in exclusion sphere " << dist << " WEIGHT = " << wei << " f " << (2.+2*eRcm+eRcm*eRcm) << " " << exp(-theExclusionRadius*XS) << " " << (2-exp(-theExclusionRadius*XS)*(2.+2*eRcm+eRcm*eRcm)) << " " << pow(eRcm,3.) << " " << XS/CLHEP::cm/3. << " XS " << XS <<  G4endl;
#endif
  } else {

    G4double nmfp = aStep->GetStepLength() * XS;  //mfp is inverted!
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmPDSGeantinoProcess::ChangeGeantinoWeight  : ener " << aTrack->GetKineticEnergy() << " MFP " << 1./XS << " nMFP " << nmfp << G4endl;
#endif
    //  G4double nmfp = aStep->GetStepLength() / aTrack->GetMaterial()->GetNuclearInterLength();
    
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << "  GmPDSGeantinoProcess::ChangeGeantinoWeight:  old weight " <<  GetWeight(aTracknc) << G4endl;
#endif
    SetWeight( aTracknc, GetWeight(aTracknc) * exp(-nmfp) );
#ifndef GAMOS_NO_VERBOSE
    G4cout <<  ScoringVerb(debugVerb) <<"  GmPDSGeantinoProcess::ChangeGeantinoWeight : ener " << aTrack->GetKineticEnergy() << "  nmfp " << nmfp << " exp(-nmfp) " << exp(-nmfp) << " new weight " << GetWeight(aTracknc) << G4endl;
#endif

  }
    
  /*  //----- Kill tracks of low weight
  G4double weight = GetWeight(aTracknc);
  if( weight < theMinimumWeightForRR ) {
//   aTracknc->SetTrackStatus( fStopAndKill );
    ApplyRussianRoulette( aTracknc, weight );
    }*/

}

//------------------------------------------------------------------
G4double GmPDSGeantinoProcess::GetMeanFreePath(const G4Track& ,
				   G4double  ,
				   G4ForceCondition* )
{
  //  *fc = InActivated;
  //  return 0.1;
  return DBL_MAX;
}
 

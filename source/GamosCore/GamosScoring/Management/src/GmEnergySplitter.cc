//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#include "GmEnergySplitter.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4VSolid.hh"
#include "G4UnitsTable.hh"
#include "G4RegularNavigationHelper.hh"
#include "G4EnergyLossForExtrapolator.hh"
#include "G4EmCalculator.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Step.hh"
#include "G4PVParameterised.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//
// Created: 
// 
///////////////////////////////////////////////////////////////////////////////

GmEnergySplitter::GmEnergySplitter()
{
   theElossExt = new G4EnergyLossForExtrapolator(0);
   thePhantomParam = 0;
   theNIterations = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmEnergySplitter:NIterations",2));
   G4cout << "GmEnergySplitter::GmEnergySplitter" << G4endl; //GDEB
		
}

GmEnergySplitter::~GmEnergySplitter()
{;}

G4int GmEnergySplitter::SplitEnergyInVolumes(const G4Step* aStep )
{
  theEnergies.clear();

  G4double edep = aStep->GetTotalEnergyDeposit();

#ifdef VERBOSE_ENERSPLIT
  G4bool verbose = 1;
  if( verbose ) G4cout << "GmEnergySplitter::SplitEnergyInVolumes totalEdepo " << aStep->GetTotalEnergyDeposit() 
		       << " Nsteps " << G4RegularNavigationHelper::Instance()->theStepLengths.size() << G4endl;
#endif    

  if( aStep->GetTrack()->GetDefinition()->GetPDGCharge() == 0)  { // neutral particle: energy is deposited at the end of the step (if the process is Transportation , edepo = 0)
    G4RegularNavigationHelper::Instance()->ClearStepLengths();
    G4RegularNavigationHelper::Instance()->AddStepLength( aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(), aStep->GetStepLength() );
    theEnergies.push_back(edep);
    //    G4cout << " AddStepLength post " << aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber() << G4endl;
    return 1;
  }
  // if not  G4RegularNavigation::ComputeStepSkippingEqualMaterials, theStepLengths will be empty
  if( G4RegularNavigationHelper::Instance()->theStepLengths.size() == 0 ) {
    G4RegularNavigationHelper::Instance()->AddStepLength( aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(), aStep->GetStepLength() );
    //    G4cout << " AddStepLength pre " << aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber() << G4endl;
  }


  if( G4RegularNavigationHelper::Instance()->theStepLengths.size() == 1 ) {
    theEnergies.push_back(edep);
    return theEnergies.size();
  }

  if( !thePhantomParam ) GetPhantomParam(TRUE);
  
  if( aStep == 0 ) return FALSE; // it is 0 when called by GmScoringMgr after last event
  
  //----- Distribute energy deposited in voxels 
  std::vector< std::pair<G4int,G4double> > rnsl = G4RegularNavigationHelper::Instance()->theStepLengths; 

  const G4ParticleDefinition* part = aStep->GetTrack()->GetDefinition();
  G4double kinEnergyPreOrig = aStep->GetPreStepPoint()->GetKineticEnergy();
  G4double kinEnergyPre = kinEnergyPreOrig;
  
  G4double stepLength = aStep->GetStepLength();
  G4double slSum = 0.;
  
  for( unsigned int ii = 0; ii < rnsl.size(); ii++ ){
    G4double sl = rnsl[ii].second;
    slSum += sl;
#ifdef VERBOSE_ENERSPLIT
    if(verbose) G4cout  << "GmEnergySplitter::SplitEnergyInVolumes"<< ii << " RN: iter1 step length geom " << sl << G4endl;
#endif
  }
  
#ifdef VERBOSE_ENERSPLIT
  if( verbose )
    G4cout << "GmEnergySplitter RN:  step length geom TOTAL " << slSum 
	   << " true TOTAL " << stepLength 
	   << " ratio " << stepLength/slSum 
	   << " Energy " << aStep->GetPreStepPoint()->GetKineticEnergy() 
	   << " Material " << aStep->GetPreStepPoint()->GetMaterial()->GetName() 
	   << " Number of geom steps " << rnsl.size() << G4endl;
#endif
  //----- No iterations to correct elost and msc => distribute energy deposited according to geometrical step length in each voxel
  if( theNIterations == 0 ) { 
    for( unsigned int ii = 0; ii < rnsl.size(); ii++ ){
      G4double sl = G4RegularNavigationHelper::Instance()->theStepLengths[ii].second;
      G4double edepStep = edep * sl/slSum; //divide edep along steps, proportional to step length
#ifdef VERBOSE_ENERSPLIT
      if(verbose) G4cout  << "GmEnergySplitter::SplitEnergyInVolumes"<< ii 
			  << " edep " << edepStep << G4endl;
#endif
	
      theEnergies.push_back(edepStep);
	
    }
  } else { //  1 or more iterations demanded
      
#ifdef VERBOSE_ENERSPLIT
    // print corrected energy at iteration 0 
    if(verbose)  {
      G4double slSump = 0.;
      for( size_t ii2 = 0; ii2 < rnsl.size(); ii2++ ){
	G4double sl = rnsl[ii2].second;
	slSump += sl;
      }
      for( size_t ii2 = 0; ii2 < rnsl.size(); ii2++ ){
	G4cout  << "GmEnergySplitter::SplitEnergyInVolumes "<< ii2
		<< " RN: iter0 corrected energy lost " << edep*rnsl[ii2].second/slSump  
		<< G4endl;
      }
    }
#endif

    G4double slRatio = stepLength/slSum;
#ifdef VERBOSE_ENERSPLIT
    if(verbose) G4cout << "GmEnergySplitter::SplitEnergyInVolumes  RN: iter 0, step ratio " << slRatio << G4endl;
#endif
      
    //--- energy at each interaction
    G4EmCalculator emcalc;
    G4double totalELost = 0.;
    std::vector<G4double> stepLengths;
    for( int iiter = 1; iiter <= theNIterations; iiter++ ) {
      //--- iter1: distribute true step length in each voxel: geom SL in each voxel is multiplied by a constant so that the sum gives the total true step length
      if( iiter == 1 ) {
	for( size_t ii2 = 0; ii2 < rnsl.size(); ii2++ ){
	  G4double sl = rnsl[ii2].second;
	  stepLengths.push_back( sl * slRatio );
#ifdef VERBOSE_ENERSPLIT
	  if(verbose) G4cout  << "GmEnergySplitter::SplitEnergyInVolumes"<< ii2 << " RN: iter" << iiter << " corrected step length " << sl*slRatio << G4endl;
#endif
	}
	
	for( size_t ii2 = 0; ii2 < rnsl.size(); ii2++ ){
	  const G4Material* mate = thePhantomParam->GetMaterial( rnsl[ii2].first );
	  G4double dEdx = 0.;
	  if( kinEnergyPre > 0. ) {  //t check this 
	    dEdx = emcalc.GetDEDX(kinEnergyPre, part, mate);
	  }
	  G4double elost = stepLengths[ii2] * dEdx;
	  
#ifdef VERBOSE_ENERSPLIT
	  if(verbose) G4cout  << "GmEnergySplitter::SplitEnergyInVolumes"<< ii2 << " RN: iter1 energy lost "  << elost 
			      << " energy at interaction " << kinEnergyPre 
			      << " = stepLength " << stepLengths[ii2] 
			      << " * dEdx " << dEdx << G4endl;
#endif
	  kinEnergyPre -= elost;
	  theEnergies.push_back( elost );
	  totalELost += elost;
	}
	
      } else{
	//------ 2nd and other iterations
	//----- Get step lengths corrected by changing geom2true correction
	//-- Get ratios for each energy 
	slSum = 0.;
	kinEnergyPre = kinEnergyPreOrig;
	for( size_t ii2 = 0; ii2 < rnsl.size(); ii2++ ){
	  const G4Material* mate = thePhantomParam->GetMaterial( rnsl[ii2].first );
	  stepLengths[ii2] = theElossExt->TrueStepLength( kinEnergyPre, rnsl[ii2].second , mate, part );
	  kinEnergyPre -= theEnergies[ii2];
	  
#ifdef VERBOSE_ENERSPLIT
	  if(verbose) G4cout << "GmEnergySplitter::SplitEnergyInVolumes" << ii2 
			     << " RN: iter" << iiter << " step length geom " << stepLengths[ii2] 
			     << " geom2true " << rnsl[ii2].second / stepLengths[ii2]  << G4endl;
#endif
	  
	  slSum += stepLengths[ii2];
	}
	
	//Correct step lengths so that they sum the total step length
	G4double slratio = aStep->GetStepLength()/slSum;
#ifdef VERBOSE_ENERSPLIT
	if(verbose) G4cout << "GmEnergySplitter::SplitEnergyInVolumes" << " RN: iter" << iiter << " step ratio " << slRatio << G4endl;
#endif
	for( size_t ii2 = 0; ii2 < rnsl.size(); ii2++ ){
	  stepLengths[ii2] *= slratio;
#ifdef VERBOSE_ENERSPLIT
	  if(verbose) G4cout  << "GmEnergySplitter::SplitEnergyInVolumes"<< ii2 << " RN: iter" << iiter << " corrected step length " << stepLengths[ii2] << G4endl;
#endif
	}
	
	//---- Recalculate energy lost with this new step lengths
	G4double kinEnergyPre2 = aStep->GetPreStepPoint()->GetKineticEnergy();
	totalELost = 0.;
	for( size_t ii2 = 0; ii2 < rnsl.size(); ii2++ ){
	  const G4Material* mate = thePhantomParam->GetMaterial( rnsl[ii2].first );
	  G4double dEdx = 0.;
	  if( kinEnergyPre2 > 0. ) {
	    dEdx = emcalc.GetDEDX(kinEnergyPre2, part, mate);
	  }
	  G4double elost = stepLengths[ii2] * dEdx;
#ifdef VERBOSE_ENERSPLIT
	  if(verbose) G4cout  << "GmEnergySplitter::SplitEnergyInVolumes"<< ii2 << " RN: iter" << iiter << " energy lost " << elost 
			      << " energy at interaction " << kinEnergyPre2 
			      << " = stepLength " << stepLengths[ii2] 
			      << " * dEdx " << dEdx << G4endl;
#endif
	  kinEnergyPre2 -= elost;
	  theEnergies[ii2] = elost;
	  totalELost += elost;
	}
	
      }
      
      //correct energies so that they reproduce the real step energy lost
      G4double enerRatio = (edep/totalELost);
      
#ifdef VERBOSE_ENERSPLIT
      if(verbose) G4cout  << "GmEnergySplitter::SplitEnergyInVolumes"<< " RN: iter" << iiter << " energy ratio " << enerRatio << G4endl;
#endif
	
#ifdef VERBOSE_ENERSPLIT
      G4double elostTot = 0.; 
#endif
      for( size_t ii2 = 0; ii2 < theEnergies.size(); ii2++ ){
	theEnergies[ii2] *= enerRatio;
#ifdef VERBOSE_ENERSPLIT
	elostTot += theEnergies[ii2];
	if(verbose) G4cout  << "GmEnergySplitter::SplitEnergyInVolumes "<< ii2 << " RN: iter" << iiter << " corrected energy lost " << theEnergies[ii2] 
			    << " orig elost " << theEnergies[ii2]/enerRatio 
			    << " energy before interaction " << kinEnergyPreOrig-elostTot+theEnergies[ii2]
			    << " energy after interaction " << kinEnergyPreOrig-elostTot
			    << G4endl;
#endif
      }
    }
    
  }
  
  return theEnergies.size();
}


//-----------------------------------------------------------------------
void GmEnergySplitter::GetPhantomParam(G4bool mustExist)
{
  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator cite;
  for( cite = pvs->begin(); cite != pvs->end(); cite++ ) {
    //    G4cout << " PV " << (*cite)->GetName() << " " << (*cite)->GetTranslation() << G4endl;
    if( IsPhantomVolume( *cite ) ) {
      const G4PVParameterised* pvparam = static_cast<const G4PVParameterised*>(*cite);
      G4VPVParameterisation* param = pvparam->GetParameterisation();
      //    if( static_cast<const G4PhantomParameterisation*>(param) ){
      //    if( static_cast<const G4PhantomParameterisation*>(param) ){
      //      G4cout << "G4PhantomParameterisation volume found  " << (*cite)->GetName() << G4endl;
      thePhantomParam = static_cast<G4PhantomParameterisation*>(param);
    }
  }
  
  if( !thePhantomParam && mustExist ) G4Exception("GmEnergySplitter::GetPhantomParam",
						  "Wrong argument",
						  FatalErrorInArgument,
						  "No G4PhantomParameterisation found ");
 
  
}


//-----------------------------------------------------------------------
G4bool GmEnergySplitter::IsPhantomVolume( G4VPhysicalVolume* pv )
{
  EAxis axis;
  G4int nReplicas;
  G4double width,offset;
  G4bool consuming;
  pv->GetReplicationData(axis,nReplicas,width,offset,consuming);
  EVolume type = (consuming) ? kReplica : kParameterised;
  if( type == kParameterised && pv->GetRegularStructureId() == 1 ) {  
    return TRUE;
  } else {
    return FALSE;
  }

} 


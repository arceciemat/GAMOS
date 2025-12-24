//#define VERBOSE_DOSEDEP
#include "G4VPrimitiveScorer.hh"
#include "G4EnergyLossForExtrapolator.hh"
#include "GmG4PSDoseDeposit_old.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4VSolid.hh"
#include "G4UnitsTable.hh"
#include "G4PhantomParameterisation.hh"
#include "G4RegularNavigationHelper.hh"
#include "G4EmCalculator.hh"

//G4bool GmG4PSDoseDeposit_old::bNewEvent = TRUE;

//--------------------------------------------------------------------
GmG4PSDoseDeposit_old::GmG4PSDoseDeposit_old(G4String name) 
   :GmVPrimitiveScorer(name)
{ 
  theUnit = CLHEP::gray;
  theUnitName = G4String("Gy");

  theElossExt = new G4EnergyLossForExtrapolator(0);
  thePhantomParam = 0;

  theNIterations = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmG4PSDoseDeposit_old:NIterations",2));

  theGeomUtils = GmGeometryUtils::GetInstance();  
}


//--------------------------------------------------------------------
G4bool GmG4PSDoseDeposit_old::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  G4double volume = theGeomUtils->GetCubicVolume( aStep->GetPreStepPoint()->GetPhysicalVolume() );

  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
  G4double dose    = edep / ( density * volume );
  G4double weight = 1.;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "  GmG4PSDoseDeposit_old::ProcessHits dose " << dose 
	   << " edep " << edep 
	   << " elost " << aStep->GetPreStepPoint()->GetKineticEnergy() - aStep->GetPostStepPoint()->GetKineticEnergy() 
	   << " density " << density 
	   << " volume " << volume << G4endl;
#endif
  if( fWeighted ) {
    weight = aStep->GetPreStepPoint()->GetWeight(); 
    //    dose *= weight; 
  }

  FillScorer( aStep, dose, weight );
  //  FillScorer( aStep, dose, aStep->GetPreStepPoint()->GetWeight());
  return TRUE;
}


//--------------------------------------------------------------------
G4bool GmG4PSDoseDeposit_old::FillScorer(G4Step* aStep, G4double val, G4double wei)
{

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmG4PSDoseDeposit_old::FillScorer" << EvtMap << " EvtMap Number of entries " << EvtMap->entries() << " tmp " << theSumV_tmp.size() << G4endl;
#endif

  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmG4PSDoseDeposit_old::FillScorer totalEdepo " << aStep->GetTotalEnergyDeposit() 
				      << " Nsteps " << G4RegularNavigationHelper::Instance()->theStepLengths.size() << G4endl;
#endif
  //----- Do not distribute dose in voxels 
  if( !IsRegularScoring(aStep) )  { // we are only counting dose deposit
    G4int index = GetIndex(aStep);

    GmVPrimitiveScorer::FillScorer( aStep, index, val, wei );
    
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb)) G4cout  << "GmG4PSDoseDeposit_old::FillScorer  RN: energy lost " << val*wei << " index " << index << G4endl;
#endif
    
    //----- Distribute dose in voxels 
  } else {
    G4double charge = aStep->GetTrack()->GetDefinition()->GetPDGCharge();
    // use index of postStep, do not use classifier
    if( charge == 0 ) {
      GmVPrimitiveScorer::FillScorer( aStep, aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(), val, wei );
      return TRUE;
    }


    std::vector< std::pair<G4int,G4double> > rnsl = G4RegularNavigationHelper::Instance()->theStepLengths; 
#ifndef GAMOS_NO_VERBOSE
    if(ScoringVerb(debugVerb)) G4cout  << "GmG4PSDoseDeposit_old::FillScorer NSTEPS "<< rnsl.size() << G4endl;
#endif
    //      G4double geomSL = rnsl[0].second;
    //      G4double SL = aStep->GetStepLength(); 
    if( !thePhantomParam ) thePhantomParam = GmGeometryUtils::GetInstance()->GetPhantomParam();
    //      const G4Material* mate = thePhantomParam->GetMaterial( rnsl[0].first );
    const G4ParticleDefinition* part = aStep->GetTrack()->GetDefinition();
    G4double kinEnergyPreOrig = aStep->GetPreStepPoint()->GetKineticEnergy();
    G4double kinEnergyPre = kinEnergyPreOrig;
    
    G4double stepLength = aStep->GetStepLength();
    G4double slSum = 0.;
    
    for( unsigned int ii = 0; ii < rnsl.size(); ii++ ){
      G4double sl = rnsl[ii].second;
      slSum += sl;
#ifndef GAMOS_NO_VERBOSE
      if(ScoringVerb(debugVerb)) G4cout  << "GmG4PSDoseDeposit_old::FillScorer "<< ii << " RN: iter1 step length geom " << sl << G4endl;
#endif
    }
    
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) )
      G4cout << "GmG4PSDoseDeposit_old RN:  step length geom TOTAL " << slSum 
	     << " true TOTAL " << stepLength 
	     << " ratio " << stepLength/slSum 
	     << " Energy " << aStep->GetPreStepPoint()->GetKineticEnergy() 
	     << " Material " << aStep->GetPreStepPoint()->GetMaterial()->GetName() 
	     << " Number of geom steps " << rnsl.size() << G4endl;
#endif
    //----- No iterations to correct elost and msc, distribute dose according to geometrical step length in each voxel
    if( theNIterations == 0 ) { 
      for( unsigned int ii = 0; ii < rnsl.size(); ii++ ){
	G4int index = G4RegularNavigationHelper::Instance()->theStepLengths[ii].first;
	G4double sl = G4RegularNavigationHelper::Instance()->theStepLengths[ii].second;
	G4double valStep = val * sl/slSum; //divide dose along steps, proportional to step lengthr
#ifndef GAMOS_NO_VERBOSE
	G4double valwei = valStep*wei;
	if(ScoringVerb(debugVerb)) G4cout  << "GmG4PSDoseDeposit_old::FillScorer"<< ii 
			    << " dose " << valwei 
			    << " in " << index << G4endl;
#endif
	
	GmVPrimitiveScorer::FillScorer( aStep, index, valStep, wei );
	
      }
    } else { //  1 or more iterations demanded
      G4double stepEdepo = aStep->GetTotalEnergyDeposit();
      G4double stepElost = aStep->GetPreStepPoint()->GetKineticEnergy() - aStep->GetPostStepPoint()->GetKineticEnergy();

#ifndef GAMOS_NO_VERBOSE
      // print corrected energy at iteration 0 
      if(ScoringVerb(debugVerb))  {
	G4double slSum2 = 0.;
	for( size_t ii = 0; ii < rnsl.size(); ii++ ){
	  G4double sl = rnsl[ii].second;
	  slSum2 += sl;
	}
	
	for( size_t ii = 0; ii < rnsl.size(); ii++ ){
	  	  G4cout  << "GmG4PSDoseDeposit_old::FillScorer "<< ii
	    //-		  << " RN: iter0 corrected energy lost " << aStep->GetTotalEnergyDeposit()*rnsl[ii].second/slSum  
		  << " RN: iter0 corrected energy lost " << stepElost*rnsl[ii].second/slSum2  
		  << G4endl;
	}
      }
#endif
      G4double slRatio = stepLength/slSum;
#ifndef GAMOS_NO_VERBOSE
      if(ScoringVerb(debugVerb)) G4cout << "GmG4PSDoseDeposit_old::FillScorer  RN: iter0, step ratio " << slRatio << G4endl;
#endif
      
      //--- energy at each interaction
      G4EmCalculator emcalc;
      G4double totalELost = 0.;
      std::vector<G4double> kinELost;
      std::vector<G4double> stepLengths;
      for( int iiter = 1; iiter <= theNIterations; iiter++ ) {
	//--- iter1: distribute true step length in each voxel: geom SL in each voxel is multiplied by a constant so that the sum gives the total true step length
	if( iiter == 1 ) {
	  for( size_t ii = 0; ii < rnsl.size(); ii++ ){
	    G4double sl = rnsl[ii].second;
	    stepLengths.push_back( sl * slRatio );
#ifndef GAMOS_NO_VERBOSE
	    if(ScoringVerb(debugVerb)) G4cout  << "GmG4PSDoseDeposit_old::FillScorer"<< ii << " RN: iter" << iiter << " corrected step length " << sl*slRatio << G4endl;
#endif
	  }
	  
	  for( size_t ii = 0; ii < rnsl.size(); ii++ ){
	    const G4Material* mate = thePhantomParam->GetMaterial( rnsl[ii].first );
	    G4double dEdx = 0.;
	    if( kinEnergyPre > 0. ) {  //t check this 
	      dEdx = emcalc.GetDEDX(kinEnergyPre, part, mate);
	    }
	    G4double elost = stepLengths[ii] * dEdx * stepElost/stepEdepo;
	    
#ifndef GAMOS_NO_VERBOSE
	    if(ScoringVerb(debugVerb)) G4cout  << "GmG4PSDoseDeposit_old::FillScorer"<< ii << " RN: iter1 dEdx*sl energy lost "  << elost 
				<< " energy at interaction " << kinEnergyPre 
				<< " = stepLength " << stepLengths[ii] 
				<< " * dEdx " << dEdx << G4endl;
#endif
	    kinEnergyPre -= elost;
	    kinELost.push_back( elost );
	    totalELost += elost;
	  }
	  
	} else{
	  //------ 2nd and other iterations
	  //----- Get step lengths corrected by changing geom2true correction
	  //-- Get ratios for each energy 
	  slSum = 0.;
	  kinEnergyPre = kinEnergyPreOrig;
	  for( size_t ii = 0; ii < rnsl.size(); ii++ ){
	    const G4Material* mate = thePhantomParam->GetMaterial( rnsl[ii].first );
	    stepLengths[ii] = theElossExt->TrueStepLength( kinEnergyPre, rnsl[ii].second , mate, part );
	    kinEnergyPre -= kinELost[ii];
	    
#ifndef GAMOS_NO_VERBOSE
	    if(ScoringVerb(debugVerb)) G4cout << "GmG4PSDoseDeposit_old::FillScorer" << ii 
			       << " RN: iter" << iiter << " step length geom " << stepLengths[ii] 
			       << " geom2true " << rnsl[ii].second / stepLengths[ii]  << G4endl;
#endif
	    
	    slSum += stepLengths[ii];
	  }
	  
	  //Correct step lengths so that they sum the total step length
	  G4double slratio = aStep->GetStepLength()/slSum;
#ifndef GAMOS_NO_VERBOSE
	  if(ScoringVerb(debugVerb)) G4cout << "GmG4PSDoseDeposit_old::FillScorer RN: iter" << iiter << " step ratio " << slRatio << G4endl;
#endif
	  for( size_t ii = 0; ii < rnsl.size(); ii++ ){
	    stepLengths[ii] *= slratio;
#ifndef GAMOS_NO_VERBOSE
	    if(ScoringVerb(debugVerb)) G4cout  << "GmG4PSDoseDeposit_old::FillScorer RN: iter" << iiter << " corrected step length " << stepLengths[ii] << G4endl;
#endif
	    }
	  
	  //---- Recalculate energy lost with this new step lengths
	  G4double kinEnergyPre2 = aStep->GetPreStepPoint()->GetKineticEnergy();
	  totalELost = 0.;
	  for( size_t ii = 0; ii < rnsl.size(); ii++ ){
	    const G4Material* mate = thePhantomParam->GetMaterial( rnsl[ii].first );
	    G4double dEdx = 0.;
	    if( kinEnergyPre2 > 0. ) {
	      dEdx = emcalc.GetDEDX(kinEnergyPre2, part, mate);
	    }	 
	    G4double elost = stepLengths[ii] * dEdx * stepElost/stepEdepo;
#ifndef GAMOS_NO_VERBOSE
	    if(ScoringVerb(debugVerb)) G4cout  << "GmG4PSDoseDeposit_old::FillScorer"<< ii << " RN: iter" << iiter << " energy lost " << elost 
				<< " energy at interaction " << kinEnergyPre2 
				<< " = stepLength " << stepLengths[ii] 
				<< " * dEdx " << dEdx << G4endl;
#endif
	    kinEnergyPre2 -= elost;
	    kinELost[ii] = elost;
	    totalELost += elost;
	  }
	  
	}
	
	//correct energies so that they reproduce the real step energy lost
	//	G4double enerRatio = (aStep->GetTotalEnergyDeposit()/totalELost);
	G4double enerRatio = stepElost/totalELost;
	
#ifndef GAMOS_NO_VERBOSE
	if(ScoringVerb(debugVerb)) G4cout  << "GmG4PSDoseDeposit_old::FillScorer RN: iter" << iiter << " energy ratio " << enerRatio << G4endl;
#endif
	
#ifndef GAMOS_NO_VERBOSE
	G4double elostTot = 0.; 
#endif
	for( size_t ii = 0; ii < kinELost.size(); ii++ ){
	  kinELost[ii] *= enerRatio;
#ifndef GAMOS_NO_VERBOSE
	  elostTot += kinELost[ii];
	  if(ScoringVerb(debugVerb)) G4cout  << "GmG4PSDoseDeposit_old::FillScorer "<< ii << " RN: iter" << iiter << " corrected energy lost " << kinELost[ii] 
			      << " orig elost " << kinELost[ii]/enerRatio 
			      << " energy before interaction " << kinEnergyPreOrig-elostTot+kinELost[ii]
			      << " energy after interaction " << kinEnergyPreOrig-elostTot
			      << G4endl;
#endif
	}
      }
      
      //---- Compute the dose (for N iterations)
      G4double valsum = 0.;
      G4double volume = theGeomUtils->GetCubicVolume( aStep->GetPreStepPoint()->GetPhysicalVolume() );
      G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
      
      for( size_t ii = 0; ii < kinELost.size(); ii++ ){
	G4double dose  = kinELost[ii]*stepEdepo/stepElost / ( density * volume );
	G4double valwei = dose*wei;
	G4int index = rnsl[ii].first;
#ifndef GAMOS_NO_VERBOSE
	if(ScoringVerb(debugVerb)) G4cout  << "GmG4PSDoseDeposit_old::FillScorer"<< ii 
			    << " dose " << valwei
			    << " in " << index
			    << " RN: deposited energy " << kinELost[ii]*wei 
			    << G4endl;
#endif
	
	GmVPrimitiveScorer::FillScorer( aStep, index, dose, wei );
	valsum += valwei;
	
      }
      
      //      G4cout << "VAL-VALSUM " << (val-valsum)/val << " VAL " << val << " VALSUM " << valsum << G4endl;
#ifndef GAMOS_NO_VERBOSE
      if( (val-valsum)/val > 1.E-9 ) if( ScoringVerb(debugVerb) ) G4cout << "GmG4PSDoseDeposit_old::FillScorer"<< "ERRORVAL-VALSUM " << (val-valsum)/val << " VAL " << val << " VALSUM " << valsum << G4endl;
#endif
    }
  }

  G4RegularNavigationHelper::Instance()->ClearStepLengths();

  return TRUE;
} 


//--------------------------------------------------------------------
G4double GmG4PSDoseDeposit_old::GetGeom2TrueStepLength( G4double kinEnergy ) 
{

  G4double g2tratio;
  if( kinEnergy < 0.04 ) {
    g2tratio = 1.776 - 22.4*kinEnergy + 988.6*pow(kinEnergy,2.) -2.231e4*pow(kinEnergy,3.) + 1.718e5*pow(kinEnergy,4.);
  } else if( kinEnergy < 1. ) {
    g2tratio = 1.405 - 0.9334*kinEnergy + 1.072*pow(kinEnergy,2.) -0.4395*pow(kinEnergy,3.);
  } else {// if( kinEnergy < 6. ) {
    g2tratio = 1.1 + 0.008554*kinEnergy - 0.009195*pow(kinEnergy,2.) + 0.001009*pow(kinEnergy,3.);
  }

  return g2tratio;
}


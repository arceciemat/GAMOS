#include "GmPDSScore.hh"
#include "GmPDSDetector.hh"

#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosBase/Base/include/GmConvergenceTester.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include <fstream>

G4int GmPDSScore::theNumberOfScores = 0;

//-----------------------------------------------------------------------
G4bool GmPDSScore::bHstar = TRUE;
G4bool GmPDSScore::bHp0 = FALSE;
G4bool GmPDSScore::bHp15 = FALSE;
G4bool GmPDSScore::bHp30 = FALSE;
G4bool GmPDSScore::bHp45 = FALSE;
G4bool GmPDSScore::bHp60 = FALSE;
G4bool GmPDSScore::bHp75 = FALSE;

//-----------------------------------------------------------------------
GmPDSScore::GmPDSScore( G4String name, G4int detID, std::set<G4double>& energies ) : 
  theName( name ), theDetID( detID ), theEnergies(energies)
{
  theID = GmPDSScore::theNumberOfScores++;
  G4int nEnergies = theEnergies.size();

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmPDSScore::GmPDSScore " << name << " detID " << detID << " setID " << theID << G4endl;
#endif
  theFluxes = std::vector<G4double>(nEnergies,0.);
  theFluxesPrevEvent = std::vector<G4double>(nEnergies,0.);
  theFluxes2 = std::vector<G4double>(nEnergies,0.);
  theFluxes3 = std::vector<G4double>(nEnergies,0.);
  theFluxes4 = std::vector<G4double>(nEnergies,0.);
  theFluxCounts = std::vector<int>(nEnergies,0);
  theHstar = 0;
  theHp0 = 0;
  theHp15 = 0;
  theHp30 = 0;
  theHp45 = 0;
  theHp60 = 0;
  theHp75 = 0;

  G4String convergence = GmParameterMgr::GetInstance()->GetStringValue("GmPDS:ConvergenceTester","");
  if( convergence != "") {
    theConvergenceTester = new GmConvergenceTester(theName + " : " + convergence); 
  } else {
    theConvergenceTester = 0;
  }

}

//-----------------------------------------------------------------------
GmPDSScore::~GmPDSScore()
{

}

//-----------------------------------------------------------------------
void GmPDSScore::FillEnergy( G4double wei, const Flux2Dose& flux2Dose, G4int enerID )
{
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSScore::FillEnergy " << theName << " enerID " << enerID << " wei " << wei << G4endl;
#endif
  theFluxes[enerID] += wei;
  theFluxCounts[enerID] += 1;

  if( bHstar ) {
    theHstar += wei * flux2Dose.Hstar;
  }
  if( bHp0 ) {
    theHp0 += wei * flux2Dose.Hp0;
  }
  if( bHp15 ) {
    theHp15 += wei * flux2Dose.Hp15;
  }
  if( bHp30 ) {
    theHp30 += wei * flux2Dose.Hp30;
  }
  if( bHp45 ) {
    theHp45 += wei * flux2Dose.Hp45;
  }
  if( bHp60 ) {
    theHp60 += wei * flux2Dose.Hp60;
  }
  if( bHp75 ) {
    theHp75 += wei * flux2Dose.Hp75;
  }
  
}


//-----------------------------------------------------------------------
void GmPDSScore::UpdateFluxErrorsThisEvent()
{
  //---- Count the change in flux for each energy bin, to calculate the error
  std::vector<G4double>::iterator itef = theFluxes.begin();
  std::vector<G4double>::iterator itefp = theFluxesPrevEvent.begin();
  std::vector<G4double>::iterator itef2 = theFluxes2.begin();
  std::vector<G4double>::iterator itef3 = theFluxes3.begin();
  std::vector<G4double>::iterator itef4 = theFluxes4.begin();
  G4double eventSum = 0.;
  for( ; itef != theFluxes.end(); itef++,itefp++,itef2++,itef3++,itef4++){
#ifndef GAMOS_NO_VERBOSE
    //    if( ScoringVerb(debugVerb) ) G4cout << "GmPDSScore " << theName << " To fill flux2 = " <<  pow((*itef) - (*itefp),2) << " = " << (*itef) << " - " << (*itefp) << G4endl;
#endif
    G4double fluxInEvent = (*itef) - (*itefp);
    (*itef2) += pow(fluxInEvent,2);
    (*itef3) += pow(fluxInEvent,3);
    (*itef4) += pow(fluxInEvent,4);
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(testVerb) ) G4cout << "GmPDSScore::UpdateFluxErrorsThisEvent " << theName << " filling flux in Event : " << fluxInEvent << " f2= " << (*itef2) << " prev= " << (*itefp) << G4endl;
#endif
    *itefp = *itef;
    if( theConvergenceTester ) {
      eventSum += fluxInEvent;
    }
  }

  if( theConvergenceTester ) {
    theConvergenceTester->AddScore( eventSum );
  }

}

//-----------------------------------------------------------------------
void GmPDSScore::PrintResults( std::ostream& out, GmPDSDetector* det )
{
  G4String detName = det->GetName();
  G4ThreeVector detPoint = det->GetCentrePoint();

  G4double NEvents = GmNumberOfEvent::GetNumberOfEvent();
  out << "%%%%%% SCORE IN POINT DETECTOR FOR set " << theName << " at " << detPoint << " NEVENTS= " << NEvents << G4endl;

  out << " MultiFunctionalDet: " << detName << G4endl;
  out << " PrimitiveScorer: PointDetector " << theName << G4endl;
  out << " Number of entries= " << theFluxes.size() << G4endl;

  std::set<G4double>::const_iterator itee = theEnergies.begin();
  std::set<G4double>::const_iterator itee2;
  std::vector<G4double>::const_iterator itef = theFluxes.begin();
  std::vector<G4double>::const_iterator itef2 = theFluxes2.begin();
  std::vector<G4double>::const_iterator itef3 = theFluxes3.begin();
  std::vector<G4double>::const_iterator itef4 = theFluxes4.begin();
  std::vector<int>::const_iterator itec = theFluxCounts.begin();

  G4int nHits = 0;
  G4double fluxTot = 0.;
  G4double fluxTotErr2 = 0.;

  for( ; itef != theFluxes.end(); itee++, itef++,itec++,itef2++,itef3++,itef4++){
    G4double flux = (*itef)/NEvents;
    G4double fluxErr2 = 1./(NEvents-1) * ((*itef2)/NEvents-flux*flux);
    //    out << "NEvents " << NEvents << " fluxErr2 " << fluxErr2 << " " << (*itef2)/NEvents << " " << (*itef)*(*itef)/NEvents/NEvents << G4endl;
    fluxTot += flux;
    fluxTotErr2 += fluxErr2;
    nHits += (*itec);
    out << " index: ";
    if( itee == theEnergies.begin() ) {
      out << " 0-";
    } else {
      itee2 = itee; itee2--;
      out << *itee2 << "-";
    }
      
    out << *itee << " = " << flux << " +-(REL) ";

    if( flux != 0 ) {
      out << sqrt(fluxErr2)/flux;
    } else {
      out << fluxErr2;
    }
    out << " sumW2 " << *itef2 << " sumW3 " << *itef3 << " sumW4 " << *itef4 
	<< "  N " << *itec << G4endl;

  }
  
  G4double fluxTotErr = 0.;
  if( fluxTot != 0. ) fluxTotErr = sqrt(fluxTotErr2)/fluxTot;
  //    out << theName << "  FLUX_TOTAL/particle= " << fluxTot << " +-(REL) " << fluxTotErr << "  N " << nHits << G4endl;
  out << theName << " SUM_ALL: " << fluxTot << " +-(REL) " << fluxTotErr << " cm-2 " << "  N " << nHits << G4endl;
  if( bHstar ) {
    out << theName << " Hstar= " << theHstar/NEvents << " pSv/particle " << G4endl;
  }
  if( bHp0 ) {
    out << theName << " Hp(10,0)= " << theHp0/NEvents << " pSv/particle " << G4endl;
  }
  if( bHp15 ) {
    out << theName << " Hp(10,15)= " << theHp15/NEvents << " pSv/particle " << G4endl;
  }
  if( bHp30 ) {
    out << theName << " Hp(10,30)= " << theHp30/NEvents << " pSv/particle " << G4endl;
  }
  if( bHp45 ) {
    out << theName << " Hp(10,45)= " << theHp45/NEvents << " pSv/particle " << G4endl;
  }
  if( bHp60 ) {
    out << theName << " Hp(10,60)= " << theHp60/NEvents << " pSv/particle " << G4endl;
  }
  if( bHp75 ) {
    out << theName << " Hp(10,75)= " << theHp75/NEvents << " pSv/particle " << G4endl;
  }
  
  if( theConvergenceTester ) {
    theConvergenceTester->ShowResult();
    theConvergenceTester->ShowHistory();
  }

}



#include "DoseData.hh"
#include "DoserData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

int DoseData::verbosity = 1;
DoseIndexValType DoseData::theIndexValType= SIVT_MIDDLE;

//-----------------------------------------------------------------
void DoseData::SetDoserData( DoserData* sd )
{
  theDoserData = sd;
}

/*
void DoseData::MultiplyByNEvents()
{
  // for summing each dose is multiplied by the number of events
  double nev = theDoserData->GetNEvents();
  theNEvents = G4int(nev);
  //  G4cout << " SetDoserDataPRE " << theName << " TOT " << theDoseTot << " nev " << nev << G4endl;
  theDoseTot *= nev; 
  if( DoseData::verbosity >= 3 ) G4cout << " SetDoserData " << theName << " TOT " << theDoseTot << G4endl;
  // only used for dispersion  theDoseSq *= nev*nev; 
}
*/

bool DoseData::operator!=( const DoseData& sd2 )
{
  return !(*this == sd2);

}
bool DoseData::operator==( const DoseData& sd2 )
{
  //     G4cout << " CHECKSEPAR " << theName << " " << sd2.GetName() << G4endl;
 // compare name also checking that 1.E-05-0.0001 is equal than 1.E-05-1.E-04
  if (theName == sd2.GetName() ) {
    /*    G4cout << " FATAL error, the Dose in Doser don't have the same fosrc/GAMOSFileData.ccat  " 
	   << " First Dose " << theName
	   << " Second Dose " << sd2.GetName() 
	   << G4endl; */
    return true;
  } else {
    // try to decompose: look for a '-' separating min and max
    int iSepar1 = -1;
    for( size_t ii = 1; ii < theName.length(); ii++ ) {
      if( theName[ii] == '-' ) {
	if( theName[ii-1] != 'E' && theName[ii-1] != 'e' ) {
	  iSepar1 = ii;
	  break;
	}
      }
    }
    int iSepar2 = -1;
    G4String theName2 = sd2.GetName();
    for( size_t ii = 1; ii < theName2.length(); ii++ ) {
      if( theName2[ii] == '-' ) {
	if( theName2[ii-1] != 'E' && theName2[ii-1] != 'e' ) {
	  iSepar2 = ii;
	  break;
	}
      }
    }
    //    G4cout << " SEPAR " << theName << " " << theName2 << " " << iSepar1 << " " << iSepar2 <<  G4endl;
    if( iSepar1 != -1 && iSepar2 != -1 ) {
      G4String name1Sub1 = theName.substr(0,iSepar1);
      if( !GmGenUtils::IsNumber(name1Sub1) ) return false;
      G4String name2Sub1 = theName2.substr(0,iSepar2);
      if( !GmGenUtils::IsNumber(name2Sub1) ) return false;
      G4String name1Sub2 = theName.substr(iSepar1+1,theName.length());
      if( !GmGenUtils::IsNumber(name2Sub1) ) return false;
      G4String name2Sub2 = theName2.substr(iSepar2+1,theName.length());
      if( !GmGenUtils::IsNumber(name2Sub2) ) return false;
      if( GmGenUtils::GetValue(name1Sub1) == GmGenUtils::GetValue(name2Sub1) && 
	  GmGenUtils::GetValue(name1Sub2) == GmGenUtils::GetValue(name2Sub2) ) {
	return true;
      }

    }

  }
  return false;

}


DoseData DoseData::operator+=( const DoseData& ed2 )
{ 
  //  if( theEnergy == 0.01)  G4cout << " 1theInvErrorExp2 " << this << " " << theInvErrorExp2 << G4endl;
  if( DoseData::verbosity >= 2 ) {
    G4cout <<  " theDose " << theDose << " * " << GetNEvents() << " + " << ed2.GetDose() << " * " << ed2.GetNEvents() << G4endl; 
    G4cout << "   DoseData::operator+=ed1 " << theName << " : " << theDose << " +- " << theDoseError << " DoseSq " << theDoseSq << G4endl; 
    G4cout << "   DoseData::operator+=ed2 " << theName << " : " << ed2.GetDose() << " +- " << ed2.GetDoseError() << " DoseSq " << ed2.GetDoseSq() << G4endl; 
  }

  theDose = (theDose*GetNEvents()+ed2.GetDose()*ed2.GetNEvents())/(GetNEvents()+ed2.GetNEvents());
  //  theDose = (theDose+ed2.GetDose()*ed2.GetNEvents())/(GetNEvents()+ed2.GetNEvents());

  //  theDoseTot += ed2.GetDoseTot();
  //  theDose += ed2.GetDose();
  theNumberOfFiles++;

  theDoseSq += ed2.GetDoseSq();

  if ( ed2.GetDoseError() != 0. ) {    
    theDoseDivideError += (ed2.GetDose())/sqr(ed2.GetDoseError());
    theInvErrorExp2 += 1/sqr(ed2.GetDoseError());
    if( theInvErrorExp2 != 0. ) {
      theDoseError = 1./sqrt(theInvErrorExp2);
    } else {
      theDoseError = 0.;
    }

    //    if( DoseData::verbosity >= 2 )
      //      G4cout << "   DoseData::operator+=: " << bTotal << " Add DoseData " << theName << " WITH " << ed2.GetName() << " " << theDose << " doseSq " << theDoseSq << " theDoseDivideError " << theDoseDivideError << " theInvErrorExp2 " << theInvErrorExp2 << G4endl;
  }  
  
  if( DoseData::verbosity >= 2 )
    G4cout << "   DoseData::operator+= " << theName << " : " << theDose << " +- " << theDoseError << " DoseSq " << theDoseSq << " DoseDivideError " << theDoseDivideError << " InvErrorExp2 " << theInvErrorExp2 << G4endl;

  return *this;
}

//-----------------------------------------------------------------
DoseData & DoseData::operator*=( G4double wei )
{
  theDose *= wei;

  theDoseSq *= wei*wei;

  if ( GetDoseError() != 0. ) {    
    theInvErrorExp2 *= 1./(wei*wei);
  }  
  
  if( DoseData::verbosity >= 2 )
    G4cout << "   DoseData::operator*=: " << theDose << " +- " << theDoseError << " DoseSq " << theDoseSq << " DoseDivideError " << theDoseDivideError << " InvErrorExp2 " << theInvErrorExp2 << G4endl;

  return *this;

}

long double DoseData::GetDose() const
{
  return theDose;
}

long double DoseData::GetDoseSq() const
{
  return theDoseSq;
}

long double DoseData::GetDoseError() const
{
  return theDoseError;
}


G4double DoseData::GetNEvents() const
{
  return theDoserData->GetNEvents();
}


G4bool DoseData::SelfCheck() const
{
  if( GetName() == "=" ) { 
    G4cerr << "!!!ERROR in Dose " << theName << " " << theDose << " +- " << theDoseError << " " << theDoseSq << G4endl;
    G4Exception("DoseData::SelfCheck",
		"Wrong data",
		JustWarning,
		"Dose name dose not exists");
    return false;
  } 

  return true;
}

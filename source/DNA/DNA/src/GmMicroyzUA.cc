#include "G4Run.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4ios.hh"

#include "GmMicroyzUA.hh"
#include "GmMicroyzSD.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"

//----------------------------------------------------------------
GmMicroyzUA::GmMicroyzUA( GmMicroyzSD * sd)
  : theSD(sd)
{
}

//----------------------------------------------------------------
void GmMicroyzUA::EndOfRunAction( const G4Run* )
{
  G4int nev = GmNumberOfEvent::GetNumberOfEvent();
  G4double meanY = theSD->GetSumY()/nev;
  G4double sumY2 = theSD->GetSumY2();
  G4double meanY2 = theSD->GetSumY2()/nev;
  G4double sumY4 = theSD->GetSumY4();
  G4double meanZ = theSD->GetSumZ()/nev;
  G4double sumZ2 = theSD->GetSumZ2();
  G4double meanZ2 = theSD->GetSumZ2()/nev;
  G4double sumZ4 = theSD->GetSumZ4();
  G4double meanYerr = sqrt((sumY2-meanY*nev)/(nev-1)/nev);
  G4double meanY2err = sqrt((sumY4-meanY2*nev)/(nev-1)/nev);
  G4double meanZerr = sqrt((sumZ2-meanZ*nev)/(nev-1)/nev);
  G4double meanZ2err = sqrt((sumZ4-meanZ2*nev)/(nev-1)/nev);
  G4double meanYerrRel = 0.;
  if( meanY != 0. ) meanYerrRel = meanYerr/meanY;
  G4double meanY2errRel = 0.;
  if( meanY2 != 0. ) meanY2errRel = meanY2err/meanY2;
  G4double meanZerrRel = 0.;
  if( meanZ != 0. ) meanZerrRel = meanZerr/meanZ;
  G4double meanZ2errRel = 0.;
  if( meanZ2 != 0. ) meanZ2errRel = meanZ2err/meanZ2;
  G4cout << " @#@# Frequency-mean lineal energy (y_F)=  " << meanY << " +-(REL) " << meanYerrRel << G4endl;
  G4cout << " @#@# Dose-mean lineal energy (y_D)=       " << meanY2/meanY << " +-(REL) " << meanY2errRel << G4endl;
  G4cout << " @#@# Frequency-mean specific energy (z_F)= " << meanZ << " +-(REL) " << meanZerrRel << G4endl;
  G4cout << " @#@# Dose-mean specific energy (z_D)=      " << meanZ2/meanZ << " +-(REL) " << meanZ2errRel << G4endl;
}

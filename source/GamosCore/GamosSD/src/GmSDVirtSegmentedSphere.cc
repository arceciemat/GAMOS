#include "GmSDVirtSegmentedSphere.hh"
#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"

GmSDVirtSegmentedSphere::GmSDVirtSegmentedSphere(G4String sdtype): GmVSD(sdtype)
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << "GmSDVirtSegmentedSphere:->"<<SensitiveDetectorName << G4endl;
#endif

}

GmSDVirtSegmentedSphere::~GmSDVirtSegmentedSphere(){}

void GmSDVirtSegmentedSphere::EndOfEvent(G4HCofThisEvent*HCE)
{
#ifndef GAMOS_NO_VERBOSE
  if(theHitsInEvent.size() != 0) if( SDVerb(infoVerb) ) G4cout << this << "GmSDVirtSegmentedSphere::EndOfEvent nhits " << SensitiveDetectorName << " Nhits " << theHitsInEvent.size() << G4endl;
#endif
  GmVSD::EndOfEvent( HCE );
}

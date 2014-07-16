#ifndef GmFilterFromClassifier_h
#define GmFilterFromClassifier_h 1

class G4ParticleDefinition;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
class GmVClassifier;


////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters steps by classifier index
// 
///////////////////////////////////////////////////////////////////////////////

class GmFilterFromClassifier : public GmVFilter 
{

  public: // with description
      GmFilterFromClassifier(G4String name);


      virtual ~GmFilterFromClassifier();

  public: // with description
      virtual G4bool AcceptStep(const G4Step*);

      //
  void show();
  virtual void SetParameters( std::vector<G4String>& );

  private:
  GmVClassifier* theClassifier;
  G4int theIndex;

};

#endif

#ifndef GmVModule_h
#define GmVModule_h

// Base class for geometry modules 
// Pedro Arce
// 31.08.2009: Creation

#include <vector>
#include <map>
#include <sstream>
#include "G4tgrLineProcessor.hh"

class GmVModule { 

 public:
  GmVModule( const std::vector<G4String>& par );
  ~GmVModule();

 public:
  virtual void BuildObjects() = 0;

  friend std::ostream& operator<<(std::ostream&, const GmVModule&);

  const G4String& GetName() const {
    return theName; 
  }
  std::vector<G4String> GetParams() const {
    return theParams;
  }
  G4String GetParams( G4int ii );

  void SetLineProcessor(G4tgrLineProcessor* lp ){
    theLineProcessor = lp;
  }

protected:
  void BuildObject( std::ostringstream& fout );

protected:
  G4String theName; 
  std::vector<G4String> theParams;
  std::map<G4String,G4String> theWords;

  G4tgrLineProcessor* theLineProcessor;
}; 

#endif

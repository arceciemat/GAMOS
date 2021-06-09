#ifndef GmSDType_HH
#define GmSDType_HH

// Aqui voy a poner dos clases: la de GmHits y la de GmSDType sensitivo
#include "globals.hh"

enum SDType { SDTracker, SDCalor };

class GmSDType {
private:
  GmSDType(){};
  ~GmSDType(){};
  
public:
  static SDType GetType( const G4String& type );
  static G4String Print( SDType );

};

#endif

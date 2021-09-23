#ifndef GmUIcmdWithAString_hh
#define GmUIcmdWithAString_hh 1

#include "G4UIcmdWithAString.hh"

// class description:
//  A concrete class of G4UIcommand. The command defined by this class
// takes a string. Incase the parameter string contains space(s), it
// must be enclosed by double-quotations (").
//  General information of G4UIcommand is given in G4UIcommand.hh.


class GmUIcmdWithAString : public G4UIcmdWithAString
{
  public: // with description
    GmUIcmdWithAString
    (const char * theCommandPath,G4UImessenger * theMessenger);
    //  Constructor. The command string with full path directory
    // and the pointer to the messenger must be given.

  virtual G4int DoIt(G4String parameterList);

};

#endif

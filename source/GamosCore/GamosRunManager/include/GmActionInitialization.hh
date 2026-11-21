#ifndef GmActionInitialization_H
#define GmActionInitialization_H

// GAMOS multithreading: action initialization used with G4MTRunManager / G4TaskRunManager.
// The GAMOS objects of each worker thread (generator, user actions, filters, scorers, SDs, ...)
// are created by replaying on the worker the UI commands executed on the master
// (Geant4 command broadcasting). Build() only creates, on the worker thread,
// the GAMOS messengers that will receive these commands.

#include "G4VUserActionInitialization.hh"

class GmActionInitialization : public G4VUserActionInitialization
{
public:
  GmActionInitialization(){};
  virtual ~GmActionInitialization(){};

  virtual void BuildForMaster() const;
  virtual void Build() const;
};

#endif

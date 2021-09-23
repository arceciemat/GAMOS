#ifndef GmVBiasingOperator_hh
#define GmVBiasingOperator_hh 1

#include "G4VBiasingOperator.hh"
class G4ParticleDefinition;
class G4VProcess;
class G4BiasingProcessInterface;

typedef std::multimap<G4String, G4String> mmss;
typedef std::multimap<G4ParticleDefinition*, G4VProcess*> mmpp;

class GmVBiasingOperator : public G4VBiasingOperator
{
public:
// ---------------
// -- Constructor:
// ---------------
  GmVBiasingOperator(G4String name);
  virtual ~GmVBiasingOperator();
  G4bool IsNonPhysicsBiased() const {
    return bNonPhysicsBiased;
  }
  virtual void AddParticle( G4String partName );
  virtual void AddParticleProcess( G4String partName, G4String procName );
  void ConvertParticleProcessToPointers();
  virtual G4bool CheckParticleProcess(G4ParticleDefinition* part, const G4BiasingProcessInterface* proc);

protected:
  mmss theParticleProcessNames;
  mmpp theParticleProcesses;
  G4bool bNonPhysicsBiased;

};

#endif

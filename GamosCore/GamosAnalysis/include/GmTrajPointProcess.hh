#ifndef GmTrajPointProcess_H
#define GmTrajPointProcess_H

#include <vector>
#include "globals.hh"
#include "GmVTrajPoint.hh"
class G4StepPoint;

class GmTrajPointProcess: public GmVTrajPoint
{
public:
  GmTrajPointProcess( const G4StepPoint* );
  virtual ~GmTrajPointProcess();

  G4String GetProcessName() const { return theProcessName;}

protected:
  G4String theProcessName;

};

std::ostream & operator<<(std::ostream & os, const GmTrajPointProcess &);

typedef std::vector<GmTrajPointProcess> GmTrajPointProcessList;

#endif

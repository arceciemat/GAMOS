#ifndef RTIAEASharedReader_h
#define RTIAEASharedReader_h

// Reading of IAEA phase space files shared by all threads (GAMOS multithreading):
// one file position for the whole job, so that each particle is read once as in sequential mode;
// skipping of the first particles (at start of each run) and recycling of the file are also global.
// In sequential mode the behaviour is identical to the previous code of the generators.

#include "globals.hh"
#include <cstdio>
class iaea_record_type;

class RTIAEASharedReader
{
public:
  enum ReadResult { RTIAEA_OK = 0, RTIAEA_RECYCLED = 1, RTIAEA_EXHAUSTED = -1 };

  // ownFile: file opened by the generator instance (used as shared file by the first reader)
  static ReadResult ReadParticle( iaea_record_type* record, FILE* ownFile, const G4String& fileName,
				  G4int nSkip, G4int maxNRecycle, G4int& nRecycled, const G4String& caller );
};

#endif

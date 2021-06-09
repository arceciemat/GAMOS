#ifndef GmDetUnitIDBuilder_HH
#define GmDetUnitIDBuilder_HH

class G4Step;

class GmDetUnitIDBuilder 
{
public:
  GmDetUnitIDBuilder(){};
  virtual ~GmDetUnitIDBuilder(){};

  virtual unsigned long long BuildID( G4Step* aStep ) = 0;

};

#endif

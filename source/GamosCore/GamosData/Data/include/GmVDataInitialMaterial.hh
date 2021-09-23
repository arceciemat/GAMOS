#ifndef GmVDataInitialMaterial_hh
#define GmVDataInitialMaterial_hh

class G4Material;
class G4VPhysicalVolume;

class GmVDataInitialMaterial 
{
public:
  GmVDataInitialMaterial(){};
  ~GmVDataInitialMaterial(){};

  G4Material* GetMateFromPV( G4VPhysicalVolume* pv );

};
#endif

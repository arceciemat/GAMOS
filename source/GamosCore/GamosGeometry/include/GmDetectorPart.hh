#ifndef GmDetectorPart_H
#define GmDetectorPart_H

class G4LogicalVolume;

class GmDetectorPart 
{
public:

  GmDetectorPart(){};

  virtual ~GmDetectorPart(){};

  virtual G4LogicalVolume* Construct(const G4String& name) = 0;


private: 

};
#endif

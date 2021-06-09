#ifndef GmGeometryUtils_H
#define GmGeometryUtils_H

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VSolid;
class G4Region;
class GmTouchable;
class G4Material;
class G4Track;
class G4TouchableHistory;
#include <map>
#include <vector>
#include <set>
#include "G4ThreeVector.hh"

typedef std::map< G4VPhysicalVolume*, G4VPhysicalVolume* > mpvpv;
typedef std::multimap< G4LogicalVolume*, G4VPhysicalVolume* > mmlvpv;
typedef std::vector< std::pair<G4String, G4int> > vpsi;
typedef std::vector< std::vector< G4VPhysicalVolume* > > vvpv;
typedef std::multimap< G4String, G4VPhysicalVolume* > mmspv;
typedef std::map< G4VPhysicalVolume*, G4LogicalVolume* > mpvlv;

class GmGeometryUtils 
{
 public:
  static GmGeometryUtils* GetInstance();

  ~GmGeometryUtils();

private:
  GmGeometryUtils();

public:
  //  G4LogicalVolume* FindLV( G4String lvname, G4bool mustExist = true );
  void DumpSummary( std::ostream& out = G4cout);
  void DumpG4LVList( std::ostream& out = G4cout);
  void DumpG4LVTree( std::ostream& out = G4cout);
  void DumpMaterialList( std::ostream& out = G4cout, G4int verbosity = 1 );
  void DumpG4LVLeaf( G4LogicalVolume* lv, unsigned int leafDepth, std::ostream& out = G4cout );

  void DumpG4PVLVTree( std::ostream& out = G4cout, G4int verbosity = 11 );
  void DumpLV(G4LogicalVolume* lv, unsigned int leafDepth, std::ostream& out = G4cout, G4int verbosity = 11 );
  void DumpPV(G4VPhysicalVolume* pv, unsigned int leafDepth, std::ostream& out = G4cout, G4int verbosity = 11 );
  //void DumpTouch(G4VPhysicalVolume* pv, unsigned int leafDepth, std::ostream& out = G4cout );
  void DumpSolid( G4VSolid* sol, unsigned int leafDepth, std::ostream& out = G4cout );
  G4VPhysicalVolume* GetTopPV();
  G4LogicalVolume* GetTopLV(G4bool bGeomInit = true);

  G4Material* GetMaterial( const G4String& name, bool exists = true );
  std::vector<G4Material*> GetMaterials( const G4String& name, bool exists = true );

  std::vector<GmTouchable*> GetTouchables( const G4String& name, bool exists = true );
  std::vector<G4VPhysicalVolume*> GetPhysicalVolumes( const G4String& name, bool exists = true, G4int nVols = -1 );
  std::vector<G4String> GetPhysicalVolumeNames( const G4String& name, bool exists = true );
  std::vector<G4LogicalVolume*> GetLogicalVolumes( const G4String& name, bool exists = true, G4int nVols = -1 );
  std::vector<G4Region*> GetRegions( const G4String& name, bool exists = true);

  std::set<G4String> GetAllSDTypes();

  G4double GetDistanceToOutNoDirection(const G4Track* aTrack);
  G4double GetDistanceToOutDirection(const G4Track* aTrack);

  int CountNoTouchables();

  void RebuildDictionaries();

  bool CheckPVCopyNo( const G4VPhysicalVolume* pv, G4int copyNo );
  vpsi ExtractAncestorsRequested( const G4String& name );

  G4String GetG4TouchableLongName(G4TouchableHistory* touch);

  std::vector<G4Material*> GetMaterialsUsed();

  G4double GetCubicVolume( G4VPhysicalVolume* pv );

  G4double CalculateParameterisedCubicVolume( G4VPhysicalVolume* pv, G4bool bPVExists );

  G4String BuildTouchableName( const G4ThreeVector& pos );

private:
  void BuildDictionaries();

  void Add1Touchable( G4LogicalVolume* lv, int& nTouch, int nReplicas);

  G4String SpacesFromLeafDepth( unsigned int leafDepth );
  void DumpG4PVLVLeaf(G4LogicalVolume* lv, unsigned int leafDepth, std::ostream& out = G4cout, G4int verbosity = 11 );
  void AddParentPV( const G4VPhysicalVolume* pv, std::vector<G4VPhysicalVolume*>& vpv, vpsi& ancestorsRequested, G4int ancestorLevel );
  void ExpandCopyNoList( std::vector< std::vector<G4int> >& vvi, std::vector<G4int> vi, G4int level );
  G4double CalculateCubicVolume( G4LogicalVolume* lv );

private:
  static GmGeometryUtils* theInstance;

  int                 nchar;
  G4String            name;
  mpvpv               thePVTree;
  G4VPhysicalVolume*  theTopPV; 
  // G4NavigationHistory fHistory;

  vvpv theAncestorLines;
  // physical volume container: name, G4VPhysicalVolume*
  mmspv thePVs;
  //key = logical volume, value = physical volume from this LV (to allow easy retrieval of all PV's of one LV)
  mmlvpv theLVPVTree;
 //key = physical volume, value = logical volume where the PV is placed (GEANT4 does not keep the pointer from a PV to the LV where it is placed)
  mpvlv thePVLVTree;

  G4bool bPreCalculatedPhantomCubicVolumes;

  G4TouchableHistory* theTouchable;

  std::map<G4LogicalVolume*, G4double> thePreCalculatedCubicVolumes;
  std::map<G4VPhysicalVolume*, std::map<G4int, G4double>* > thePreCalculatedParameterisedCubicVolumes;
};


/*
VERBOSITY: 
LV: last digit
>= 1: name 
>= 2: solid and material names
>= 3: solid and visualisation

PV: before last digit
>= 1: name and copy number, mother name
>= 2: translation, has rotation or not
>= 3: rotation, replica or parameterisation data
*/

#endif

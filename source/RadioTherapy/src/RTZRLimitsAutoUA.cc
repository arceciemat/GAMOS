#include "RTZRLimitsAutoUA.hh"
#include "RTVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4ios.hh"
#include "G4EventManager.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VisExtent.hh"
#include "G4UserSteppingAction.hh"
#include "G4PhysicalVolumeStore.hh"

//---------------------------------------------------------------------
RTZRLimitsAutoUA::RTZRLimitsAutoUA()
{
}

//---------------------------------------------------------------------
void RTZRLimitsAutoUA::BuildXYLimits()
{
  //--- Get world logical volume and dimensions
  GetWorld();

  //----- Make list of all volumes placed in world
  BuildVolumesInWorld();

  BuildXYSizes();

  XYSizes2Limits();

}

//---------------------------------------------------------------------
void RTZRLimitsAutoUA::BuildVolumesInWorld()
{
  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  G4PhysicalVolumeStore::const_iterator ite;
  for(ite = pvs->begin(); ite != pvs->end(); ite++ ){
    if( (*ite)->GetMotherLogical() == theWorldLogical ) {
#ifndef GAMOS_NO_VERBOSE
      if( RTVerb(infoVerb) ) G4cout << "RTZRLimitsAutoUA::BuildVolumesInWorld: theVolumesInWorld " << (*ite)->GetName() << G4endl;
#endif
      theVolumesInWorld.push_back(*ite);
    }
  }
}

//---------------------------------------------------------------------
void RTZRLimitsAutoUA::BuildXYSizes() 
{
  std::vector<const G4VPhysicalVolume*>::const_iterator ite;
  for(ite = theVolumesInWorld.begin(); ite != theVolumesInWorld.end(); ite++ ){
    const G4VPhysicalVolume* pv  = *ite;

    G4VSolid* solid = pv->GetLogicalVolume()->GetSolid();
    G4ThreeVector pos = pv->GetTranslation();
    G4VisExtent extent = solid->GetExtent();
    G4double zmin = pos.z() + extent.GetZmin();
    G4double zmax = pos.z() + extent.GetZmax();
    
    XYdata dat;
    dat.xmin  = pos.x() + extent.GetXmin();
    dat.xmax = pos.x() + extent.GetXmax();
    dat.ymin = pos.y() + extent.GetYmin();
    dat.ymax = pos.y() + extent.GetYmax();
    theXYsizes[zmin] = dat; 
    theXYsizes[zmax] = dat; 

    std::map<G4double,G4String>::const_iterator itemm = theXYvolName.find(zmin);

    if( itemm == theXYvolName.end() ) {
      theXYvolName[zmin] = pv->GetName() + "_min";
    } else {
      theXYvolName[zmin] = pv->GetName() + "_min" + (*itemm).second;
    }
    itemm = theXYvolName.find(zmax);
    if( itemm == theXYvolName.end() ) {
      theXYvolName[zmax] = pv->GetName() + "_max";
    } else {
      theXYvolName[zmax] = pv->GetName() + "_max" + (*itemm).second;
    }

#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(infoVerb) ) G4cout << "RTZRLimitsAutoUA::BuildXYSizes: theXYsizes " << pv->GetName() << " zmin= " << zmin << " zmax= " << zmax << " dat= " << dat << G4endl;
#endif
    
  }

  // add a copy of the first XY at -worldZ
  theXYsizes[-theWorldZ] = (*(theXYsizes.begin())).second;
  theXYvolName[-theWorldZ] = "World_min";
  // add as two XYsizes at the end so that all particles stop after last module
  XYdata dat; 
  dat.xmin = -theWorldX;
  dat.xmax = theWorldX;
  dat.ymin = -theWorldY;
  dat.ymax = theWorldY;
  //?  theXYsizes[(*(theXYsizes.rbegin())).first] = dat;
  theXYsizes[theWorldZ] = dat;
  theXYvolName[theWorldZ] = "World_max";

  std::map<G4double,XYdata>::const_iterator itexy;
  for( itexy = theXYsizes.begin(); itexy != theXYsizes.end(); itexy++ ) {
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(infoVerb) ) G4cout << "RTZRLimitsAutoUA::BuildXYSizes: theXYsizes  z= " << (*itexy).first << " dat= " << (*itexy).second << G4endl;
#endif
  }
}

//---------------------------------------------------------------------
void RTZRLimitsAutoUA::XYSizes2Limits() 
{
  std::map<G4double,XYdata>::const_iterator ite1 = theXYsizes.begin();
  std::map<G4double,XYdata>::const_iterator ite2;
  //  ite1++;
  XYdata dat;

  for( ;ite1 != theXYsizes.end(); ite1++ ){
    dat.xmin = (*ite1).second.xmin;
    dat.xmax = (*ite1).second.xmax;
    dat.ymin = (*ite1).second.ymin;
    dat.ymax = (*ite1).second.ymax;
    //--- look if another dat at same Z
    G4double zval = (*ite1).first;
    ite2 = theXYlimits.find(zval);
    if( ite2 != theXYlimits.end() ) {
      XYdata dat2 = (*ite2).second;
      dat.xmin = std::min( dat.xmin, dat2.xmin );
      dat.xmax = std::max( dat.xmax, dat2.xmax );
      dat.ymin = std::min( dat.ymin, dat2.ymin );
      dat.ymax = std::max( dat.ymax, dat2.ymax );
    }
    theXYlimits[(*ite1).first] = dat;
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(infoVerb) ) G4cout << "RTZRLimitsAutoUA::XYSizes2Limits theXYlimits " << (*ite1).first << " = " << dat << G4endl;
#endif
  }
  std::map<G4double,XYdata>::const_iterator itexy;
  for( itexy = theXYlimits.begin(); itexy != theXYlimits.end(); itexy++ ) {
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(infoVerb) ) G4cout << " theXYlimits  z= " << (*itexy).first << " dat= " << (*itexy).second << G4endl;
#endif
  }

}

#include "G4Run.hh"
#include "G4Event.hh"

#include "GmPhantomDensityHistosUA.hh"
#include "Gm3ddoseHeader.hh"
#include "GmRegularParamUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4PVParameterised.hh"
#include "G4Material.hh"

//-----------------------------------------------------------------------
GmPhantomDensityHistosUA::GmPhantomDensityHistosUA()
{
  //  thePhantomName = GmParameterMgr::GetInstance()->GetStringValue("GmPhantomDensityHistosUA::CrystalName","lyso_crystal");

  theAnaMgr = GmAnalysisMgr::GetInstance("phantom") ;
}

//-----------------------------------------------------------------------
void GmPhantomDensityHistosUA::BeginOfRunAction( const G4Run* )
{
  G4bool phantomRegular = FALSE;
  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator cite;
  for( cite = pvs->begin(); cite != pvs->end(); cite++ ) {
    G4cout << " PV " << (*cite)->GetName() << " " << (*cite)->GetTranslation() << G4endl;
    G4PhantomParameterisation* paramreg = GmRegularParamUtils::GetInstance()->GetPhantomParam( *cite, FALSE );
    if( paramreg != 0 ){
      if( phantomRegular ) G4Exception("GmPhantomDensityHistosUA::BeginOfRunAction",
				       "Error",
				       FatalException,
				       "Two G4PhantomParameterisation's found ");
      phantomRegular = TRUE;
      thePhantomStructure = *cite;
      theRegularParam = paramreg;
    }
  }

  if( !phantomRegular ) G4Exception("GmPhantomDensityHistosUA::BeginOfRunAction",
				    "Error",
				    FatalException,
				    "No G4PhantomParameterisation found ");

  BookHistos();  

  FillHistos();
}


//-----------------------------------------------------------------------
void GmPhantomDensityHistosUA::BookHistos()
{
  G4String hnam;
  G4String hnamt;
  G4int nx, ny, nz;
  nx = theRegularParam->GetNoVoxelX();
  ny = theRegularParam->GetNoVoxelY();
  nz = theRegularParam->GetNoVoxelZ();

  //----- Histograms for energy deposited
  for( G4int ii = 0; ii < nz; ii++ ){        
    hnam = G4String("Density in phantom Z plane "+GmGenUtils::itoa(ii));
    theAnaMgr->CreateHisto2D(hnam,nx,0,float(nx),ny,0,float(ny),48000+ii);
  }

}


//-----------------------------------------------------------------------
void GmPhantomDensityHistosUA::FillHistos()
{

  G4int nx, ny, nz;
  nx = theRegularParam->GetNoVoxelX();
  ny = theRegularParam->GetNoVoxelY();
  nz = theRegularParam->GetNoVoxelZ();

  for( G4int iz = 0; iz < nz; iz++ ){        
    for( G4int iy = 0; iy < ny; iy++ ){        
      for( G4int ix = 0; ix < nx; ix++ ){        
	G4double density = theRegularParam->GetMaterial( ix, iy, iz)->GetDensity()/CLHEP::g*CLHEP::cm3;
	theAnaMgr->GetHisto2D(48000+iz)->Fill( ix, iy, density );
      }
    }
  }
}

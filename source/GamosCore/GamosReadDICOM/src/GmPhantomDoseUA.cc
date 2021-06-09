#include "G4Run.hh"
#include "G4Event.hh"

#include "GmPhantomDoseUA.hh"
#include "Gm3ddoseHeader.hh"
#include "GmRegularParamUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4PVParameterised.hh"

//-----------------------------------------------------------------------
GmPhantomDoseUA::GmPhantomDoseUA()
{
  //  thePhantomName = GmParameterMgr::GetInstance()->GetStringValue("GmPhantomDoseUA::CrystalName","lyso_crystal");

  theAnaMgr = GmAnalysisMgr::GetInstance("EGSphantom") ;
}

//-----------------------------------------------------------------------
void GmPhantomDoseUA::BeginOfRunAction( const G4Run* )
{
  G4bool phantomRegular = FALSE;
  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator cite;
  for( cite = pvs->begin(); cite != pvs->end(); cite++ ) {
    G4cout << " PV " << (*cite)->GetName() << " " << (*cite)->GetTranslation() << G4endl;
    G4PhantomParameterisation* paramreg = GmRegularParamUtils::GetInstance()->GetPhantomParam( *cite, FALSE );
    if( paramreg != 0 ){
      if( phantomRegular ) G4Exception("GmPhantomDoseUA::BeginOfRunAction",
				       "Error",
				       FatalException,
				       "Two G4PhantomParameterisation's found ");
      phantomRegular = TRUE;
      thePhantomStructure = *cite;
      theRegularParam = paramreg;
      Write3ddoseHeader();
      InitialiseDoseCounters();
    }
  }

  if( !phantomRegular ) G4Exception("GmPhantomDoseUA::BeginOfRunAction",
				    "Error",
				    FatalException,
				    "No G4PhantomParameterisation found ");

  BookHistos();  

}

//-----------------------------------------------------------------------
void GmPhantomDoseUA::BeginOfEventAction( const G4Event* )
{
}

//-----------------------------------------------------------------------
void GmPhantomDoseUA::EndOfEventAction( const G4Event* evt )
{
  if( evt->GetEventID() % 100000 == 99999 ) {
    *theDoseFile << " $$$$$ evt->GetEventID() " << G4endl;
    //    Write3ddose();
  }

}

//-----------------------------------------------------------------------
void GmPhantomDoseUA::EndOfRunAction( const G4Run* )
{
  Write3ddose();
  FillDoseHistos();
}

//-----------------------------------------------------------------------
void GmPhantomDoseUA::PreUserTrackingAction(const G4Track* )
{
  //  if( aTrack->GetKineticEnergy() > 100 ) G4cout << aTrack->GetTrackID() << " BIG TRACK START " <<  aTrack->GetKineticEnergy() << G4endl;
}

//-----------------------------------------------------------------------
void GmPhantomDoseUA::PostUserTrackingAction(const G4Track* )
{
  //  if( aTrack->GetVertexKineticEnergy() > 100 ) G4cout << aTrack->GetTrackID() << " BIG TRACK END " <<  aTrack->GetKineticEnergy() << G4endl;
}

//-----------------------------------------------------------------------
void GmPhantomDoseUA::UserSteppingAction(const G4Step* aStep)
{ 
  G4Track* aTrack = aStep->GetTrack();
  if( IsInterestingStep( aStep ) ) {
    
    G4double edepo = aStep->GetTotalEnergyDeposit();
    if( edepo > 0. ){
      //     G4cout << " MATE " << aTrack->GetVolume()->GetLogicalVolume()->GetMaterial()->GetName() << " " << aTrack->GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity()/g*cm3 << " " << aTrack->GetPosition().y() << G4endl;
      size_t copyNo = (size_t)(aTrack->GetVolume()->GetCopyNo());
//      G4cout << " In Phantom " << copyNo << G4endl;
      theDose[copyNo] += edepo;
      theDose2[copyNo] += edepo*edepo;
      theDoseCounts[copyNo] += 1;
      G4ThreeVector poslocal = (aStep->GetPreStepPoint()->GetPosition()+aStep->GetPostStepPoint()->GetPosition())/2.;
      //G4cout << " dose " << edepo << " pos " << poslocal << G4endl; 
      theAnaMgr->GetHisto1(47000+1)->Fill( poslocal.x(), edepo );
      theAnaMgr->GetHisto1(47000+2)->Fill( poslocal.y(), edepo );
      theAnaMgr->GetHisto1(47000+3)->Fill( poslocal.z(), edepo );
      theAnaMgr->GetHisto2(47000+11)->Fill( poslocal.x(), poslocal.y(), edepo );
      theAnaMgr->GetHisto2(47000+12)->Fill( poslocal.x(), poslocal.z(), edepo );
      theAnaMgr->GetHisto2(47000+13)->Fill( poslocal.y(), poslocal.z(), edepo );

    }
  }

}

//-----------------------------------------------------------------------
G4bool GmPhantomDoseUA::IsInterestingStep(const G4Step* aStep)
{
  G4Track* aTrack = aStep->GetTrack();
  if( aTrack->GetVolume() == thePhantomStructure ) {
    return TRUE;
  } else { 
    return FALSE;
  }

}


//-----------------------------------------------------------------------
void GmPhantomDoseUA::BookHistos()
{
  G4String hnam;
  G4String hnamt;
  G4double xmin, xmax;
  G4double ymin, ymax;
  G4double zmin, zmax;
  G4int xstep, ystep, zstep;
  xstep = theRegularParam->GetNoVoxelX();
  xmin = thePhantomMinusCorner.x();
  xmax = thePhantomMinusCorner.x()+2.*theRegularParam->GetVoxelHalfX()*xstep;
  ystep = theRegularParam->GetNoVoxelY();
  ymin = thePhantomMinusCorner.y();
  ymax = thePhantomMinusCorner.y()+2.*theRegularParam->GetVoxelHalfY()*ystep;
  zstep = theRegularParam->GetNoVoxelZ();
  zmin = thePhantomMinusCorner.z();
  zmax = thePhantomMinusCorner.z()+2.*theRegularParam->GetVoxelHalfZ()*zstep;

  G4cout << "HISTOS xstep " << xstep << " min " << xmin << " max " << xmax << G4endl;
  G4cout << "HISTOS ystep " << ystep << " min " << ymin << " max " << ymax << G4endl;
  G4cout << "HISTOS zstep " << zstep << " min " << zmin << " max " << zmax << G4endl;
  //----- Histograms for energy deposited
  hnam = G4String("Energy deposited in detector along X (MeV)");
  theAnaMgr->CreateHisto1D(hnam,xstep,xmin,xmax,47000+1);
  hnam = G4String("Energy deposited in detector along Y (MeV)");
  theAnaMgr->CreateHisto1D(hnam,ystep,ymin,ymax,47000+2);
  hnam = G4String("Energy deposited in detector along Z (MeV)");
  theAnaMgr->CreateHisto1D(hnam,zstep,zmin,zmax,47000+3);
  hnam = G4String("Energy deposited in detector along XY (MeV)");
  theAnaMgr->CreateHisto2D(hnam,xstep,xmin,xmax,ystep,ymin,ymax,47000+11);
  hnam = G4String("Energy deposited in detector along XZ (MeV)");
  theAnaMgr->CreateHisto2D(hnam,xstep,xmin,xmax,zstep,zmin,zmax,47000+12);
  hnam = G4String("Energy deposited in detector along YZ (MeV)");
  theAnaMgr->CreateHisto2D(hnam,ystep,ymin,ymax,zstep,zmin,zmax,47000+13);


  G4int nx, ny, nz;
  nx = theRegularParam->GetNoVoxelX();
  ny = theRegularParam->GetNoVoxelY();
  nz = theRegularParam->GetNoVoxelZ();

  //----- Histograms for energy deposited
  for( G4int ii = 0; ii < nz; ii++ ){        
    hnam = G4String("Dose in phantom Z plane "+GmGenUtils::itoa(ii));
    theAnaMgr->CreateHisto2D(hnam,nx,0,float(nx),ny,0,float(ny),57000+ii);
  }

}


//-----------------------------------------------------------------------
void GmPhantomDoseUA::Write3ddoseHeader()
{
  Gm3ddoseHeader head;

  G4String fileName = GmParameterMgr::GetInstance()->GetStringValue("GmPhantomDoseUA:FileName","phantom.gm3ddose");
  theDoseFile = new std::ofstream(fileName);
  size_t nx = theRegularParam->GetNoVoxelX();
  size_t ny = theRegularParam->GetNoVoxelY();
  size_t nz = theRegularParam->GetNoVoxelZ();
  head.SetNoVoxelX( nx );
  head.SetNoVoxelY( ny );
  head.SetNoVoxelZ( nz );
  //  *theDoseFile << nx << "  " << ny << "  " << nz << G4endl;
  thePhantomMinusCorner = (theRegularParam->GetTranslation(0) + theRegularParam->GetTranslation(theRegularParam->GetNoVoxel()-1) )/ 2;
  G4double voxelX = theRegularParam->GetVoxelHalfX()*2;
  G4double voxelY = theRegularParam->GetVoxelHalfY()*2;
  G4double voxelZ = theRegularParam->GetVoxelHalfZ()*2;
  G4cout << " phantom minus corner " << thePhantomMinusCorner << " " << theRegularParam->GetTranslation(0) << " " << theRegularParam->GetTranslation(theRegularParam->GetNoVoxel()-1) << G4endl;
  G4cout << " voxelX " << theRegularParam->GetVoxelHalfX() << " = " << voxelX <<  "  " << -voxelX*nx/2. << " " << G4ThreeVector( -nx*voxelX/2.,-ny*voxelY/2.,-nz*voxelZ/2.) << G4endl;
  thePhantomMinusCorner -= G4ThreeVector(voxelX*nx/2.,voxelY*ny/2.,voxelZ*nz/2.);
  G4cout << " phantom minus corner " << thePhantomMinusCorner << G4endl;
  
  std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( thePhantomStructure->GetName() );
  GmTouchable* pcont = touchs[0];
  thePhantomMinusCorner += pcont->GetGlobalPosition();
  G4cout << " phantom minus corner " << thePhantomMinusCorner << *pcont << G4endl;
  for( std::vector<GmTouchable*>::const_iterator ite = touchs.begin(); ite != touchs.end(); ite++ ) {
    delete *ite;
  }

  std::vector<float> vval;
  for( size_t ii = 0; ii <= nx; ii++ ) {
    vval.push_back( thePhantomMinusCorner.x() + ii*voxelX );
    //    *theDoseFile << thePhantomMinusCorner.x() + ii*voxelX << " ";
  }

  head.SetVoxelLimitsX( vval );

  vval.clear();
  //  *theDoseFile << G4endl;
  for( size_t ii = 0; ii <= ny; ii++ ) {
    vval.push_back( thePhantomMinusCorner.y() + ii*voxelY );
    // *theDoseFile << thePhantomMinusCorner.y() + ii*voxelY << " ";
  }
  //  *theDoseFile << G4endl;
  head.SetVoxelLimitsY( vval );

  vval.clear();
  for( size_t ii = 0; ii <= nz; ii++ ) {
    vval.push_back( thePhantomMinusCorner.z() + ii*voxelZ );
    //  *theDoseFile << thePhantomMinusCorner.z() + ii*voxelZ << " ";
  }
  head.SetVoxelLimitsZ( vval );

  //  *theDoseFile << G4endl;

  *theDoseFile << head;
}


//-----------------------------------------------------------------------
void GmPhantomDoseUA::FillDoseHistos()
{
  size_t nx = theRegularParam->GetNoVoxelX();
  size_t ny = theRegularParam->GetNoVoxelY();

  size_t siz = theDose.size();

  for( size_t ii = 0; ii < siz; ii++ ) {
    G4int ix = ii%nx;
    G4int iy = (ii/nx)%ny;
    G4int iz = (ii/nx/ny);
    theAnaMgr->GetHisto2D(57000+iz)->Fill( ix, iy, theDose[ii] );
  }
}

//-----------------------------------------------------------------------
void GmPhantomDoseUA::InitialiseDoseCounters()
{
 G4cout << " no voxel " << theRegularParam->GetNoVoxel() << G4endl;
  theDose.resize(theRegularParam->GetNoVoxel());
  theDose2.resize(theRegularParam->GetNoVoxel());
  theDoseCounts.resize(theRegularParam->GetNoVoxel());
}


//-----------------------------------------------------------------------
void GmPhantomDoseUA::Write3ddose()
{
  G4double vol = theRegularParam->GetVoxelHalfX()* theRegularParam->GetVoxelHalfY()*theRegularParam->GetVoxelHalfZ()*8.;
  size_t siz = theDose.size();
  size_t nx = theRegularParam->GetNoVoxelX();

  for( size_t ii = 0; ii < siz; ii++ ) {
    G4double dens = theRegularParam->ComputeMaterial(ii,(G4VPhysicalVolume*)0,(G4VTouchable*)0)->GetDensity()/ CLHEP::kg*CLHEP::mm3;
    G4double mass = dens*vol;
    //    G4cout << "density " << dens << " mass " << mass << " joule/(MeV) " << joule/(MeV) << G4endl;
    *theDoseFile << theDose[ii]/(CLHEP::joule*mass);
    if( ii%nx == nx-1) {
      *theDoseFile << G4endl;
    } else {
      *theDoseFile << " ";
    }
  }

  for( size_t ii = 0; ii < siz; ii++ ) {
    size_t nc = theDoseCounts[ii];
    float error;
    if( nc <= 1 ) {
      error = 0;
    } else {
      float dose = theDose[ii];
      error = 1./(nc-1.)*(theDose2[ii]/nc - dose*dose/nc/nc );
      G4double dens = theRegularParam->ComputeMaterial(ii,(G4VPhysicalVolume*)0,(G4VTouchable*)0)->GetDensity()/ CLHEP::kg*CLHEP::mm3;
      G4double mass = dens*vol;
      error /= (CLHEP::joule*mass);
    }
    *theDoseFile << error;
    if( ii%nx == nx-1) {
      *theDoseFile << G4endl;
    } else {
      *theDoseFile << " ";
    }
  }
}

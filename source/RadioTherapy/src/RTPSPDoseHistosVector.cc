#include "RTPSPDoseHistosVector.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosReadDICOM/include/GmRegularParamUtils.hh"
#include "GamosCore/GamosReadDICOM/include/GmRegularParamUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorerVector.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdoseHeader.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4PhantomParameterisation.hh"

//------------------------------------------------------------------
RTPSPDoseHistosVector::RTPSPDoseHistosVector(G4String name, GmSqdoseHeader* doseh) :
  GmVPSPrinter( name )
{
  theAnaMgr = GmAnalysisMgr::GetInstance("dose_"+name);
  theDoseHeader = doseh;
  BookHistos();
  theNHistos = 0;

}


//------------------------------------------------------------------
void RTPSPDoseHistosVector::BookHistos()
{
  
  G4RotationMatrix phantomRot;
  if( !theDoseHeader ){
    theRegParam = GmGeometryUtils::GetInstance()->GetPhantomParam();
    theNVoxelX = G4int(theRegParam->GetNoVoxelsX());
    theNVoxelY = G4int(theRegParam->GetNoVoxelsY());
    theNVoxelZ = G4int(theRegParam->GetNoVoxelsZ());
    theDimX = theRegParam->GetVoxelHalfX();
    theDimY = theRegParam->GetVoxelHalfY();
    theDimZ = theRegParam->GetVoxelHalfZ();
    thePhantomTranslation = GmRegularParamUtils::GetInstance()->GetPhantomMotherTranslation();
    phantomRot = GmRegularParamUtils::GetInstance()->GetPhantomMotherRotation( true );
  } else {
    theNVoxelX = G4int(theDoseHeader->GetNoVoxelsX());
    theNVoxelY = G4int(theDoseHeader->GetNoVoxelsY());
    theNVoxelZ = G4int(theDoseHeader->GetNoVoxelsZ());
    theDimX = theDoseHeader->GetVoxelHalfX();
    theDimY = theDoseHeader->GetVoxelHalfY();
    theDimZ = theDoseHeader->GetVoxelHalfZ();
    thePhantomTranslation = theDoseHeader->GetTranslation();
    phantomRot = theDoseHeader->GetRotation();
  }
  thePhantomTranslation = phantomRot * thePhantomTranslation;

  G4cout << " RTPSPDoseHistosVector nvoxel " << theNVoxelX << " " << theNVoxelY << " " << theNVoxelZ << G4endl;
  G4cout << " RTPSPDoseHistosVector dim " << theDimX << " " << theDimY << " " << theDimZ << G4endl;
  G4cout << " RTPSPDoseHistosVector translation " <<  thePhantomTranslation << G4endl;
  G4cout << " RTPSPDoseHistosVector rotation " << phantomRot << G4endl;

  double doseMin = -17;
  double doseMax = -12;
  //--- define dose - volume histo
  //  theAnaMgr->CreateHisto2D("RTPSPDoseHistosVector: Dose-volume",200, int(log10(minDose)),int(log10(maxDose))+1.,36021);
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  theAnaMgr->CreateHisto1D("RTPSPDoseHistosVector"+sepa+" Dose",100,doseMin,doseMax,36201);
  theAnaMgr->CreateHisto1D("RTPSPDoseHistosVector"+sepa+" Dose-volume",100,doseMin,doseMax,36202);
    
}

//------------------------------------------------------------------
void RTPSPDoseHistosVector::CheckHistoLimits( G4int nXmin, G4int nXmax, G4int nYmin, G4int nYmax, G4int nZmin, G4int nZmax )
{
  if( nXmin < 0 ) {
    G4Exception("RTPSPDoseHistosVector::FillHisto",
		"X min too small",
		JustWarning,
		G4String(GmGenUtils::itoa(nXmin)+" < "
			 +GmGenUtils::itoa(0)).c_str());
    nXmin = 0;
  }
  if( nXmax >= theNVoxelX ) {
    G4Exception("RTPSPDoseHistosVector::FillHisto",
		"X max too big",
		JustWarning,
		G4String(GmGenUtils::itoa(nXmax)+" >= "
			 +GmGenUtils::itoa(theNVoxelX)).c_str());
    nXmax = theNVoxelX-1;
  }
  if( nYmin < 0 ) {
    G4Exception("RTPSPDoseHistosVector::FillHisto",
		"Y min too small",
		JustWarning,
		G4String(GmGenUtils::itoa(nYmin)+" < "
			 +GmGenUtils::itoa(0)).c_str());
    nYmin = 0;
  }
  if( nYmax >= theNVoxelY ) {
    G4Exception("RTPSPDoseHistosVector::FillHisto",
		"Y max too big",
		JustWarning,
		G4String(GmGenUtils::itoa(nYmax)+" >= "
			 +GmGenUtils::itoa(theNVoxelY)).c_str());
    nYmax = theNVoxelY-1;
  }
  if( nZmin < 0 ) {
    G4Exception("RTPSPDoseHistosVector::FillHisto",
		"Z min too small",
		JustWarning,
		G4String(GmGenUtils::itoa(nZmin)+" < "
			 +GmGenUtils::itoa(0)).c_str());
    nZmin = 0;
  }
  if( nZmax >= theNVoxelZ ) {
    G4Exception("RTPSPDoseHistosVector::FillHisto",
		"Z max too big",
		JustWarning,
		G4String(GmGenUtils::itoa(nZmax)+" >= "
			 +GmGenUtils::itoa(theNVoxelZ)).c_str());
    nZmax = theNVoxelZ-1;
  }
}


//------------------------------------------------------------------
void RTPSPDoseHistosVector::FillHisto( std::vector<G4String>& wl )
{
  if( wl[1] == "Profile1N_X") {
    FillHisto1N_X(wl);
  }else if( wl[1] == "Profile1N_Y") {
    FillHisto1N_Y(wl);
  }else if( wl[1] == "Profile1N_Z") {
    FillHisto1N_Z(wl);
  }else if( wl[1] == "Profile2N_XY") {
    FillHisto2N_XY(wl);
  }else if( wl[1] == "Profile2N_XZ") {
    FillHisto2N_XZ(wl);
  }else if( wl[1] == "Profile2N_YZ") {
    FillHisto2N_YZ(wl);
  }    

}


//------------------------------------------------------------------
void RTPSPDoseHistosVector::FillHisto1N_X( std::vector<G4String>& wl  )
{
  G4int nXmin = GmGenUtils::GetInt(wl[2]);
  G4int nXmax = GmGenUtils::GetInt(wl[3]);
  G4int nYmin = GmGenUtils::GetInt(wl[4]);
  G4int nYmax = GmGenUtils::GetInt(wl[5]);
  G4int nZmin = GmGenUtils::GetInt(wl[6]);
  G4int nZmax = GmGenUtils::GetInt(wl[7]);
  G4String hisName = wl[0];

  CheckHistoLimits( nXmin, nXmax, nYmin, nYmax, nZmin, nZmax );
  FillHisto1N_X( hisName, nXmin, nXmax, nYmin, nYmax, nZmin, nZmax );

}

//------------------------------------------------------------------
void RTPSPDoseHistosVector::FillHisto1N_Y( std::vector<G4String>& wl  )
{
  G4int nXmin = GmGenUtils::GetInt(wl[4]);
  G4int nXmax = GmGenUtils::GetInt(wl[5]);
  G4int nYmin = GmGenUtils::GetInt(wl[2]);
  G4int nYmax = GmGenUtils::GetInt(wl[3]);
  G4int nZmin = GmGenUtils::GetInt(wl[6]);
  G4int nZmax = GmGenUtils::GetInt(wl[7]);
  G4String hisName = wl[0];

  CheckHistoLimits( nXmin, nXmax, nYmin, nYmax, nZmin, nZmax );
  FillHisto1N_Y( hisName, nYmin, nYmax, nXmin, nXmax, nZmin, nZmax );

}

//------------------------------------------------------------------
void RTPSPDoseHistosVector::FillHisto1N_Z( std::vector<G4String>& wl  )
{
  G4int nXmin = GmGenUtils::GetInt(wl[4]);
  G4int nXmax = GmGenUtils::GetInt(wl[5]);
  G4int nYmin = GmGenUtils::GetInt(wl[6]);
  G4int nYmax = GmGenUtils::GetInt(wl[7]);
  G4int nZmin = GmGenUtils::GetInt(wl[2]);
  G4int nZmax = GmGenUtils::GetInt(wl[3]);
  G4String hisName = wl[0];

  CheckHistoLimits( nXmin, nXmax, nYmin, nYmax, nZmin, nZmax );
  FillHisto1N_Z( hisName, nZmin, nZmax, nXmin, nXmax, nYmin, nYmax );

}

 
//------------------------------------------------------------------
void RTPSPDoseHistosVector::FillHisto1N_X( const G4String& hisName, G4int nXmin, G4int nXmax, G4int nYmin, G4int nYmax, G4int nZmin, G4int nZmax )
{ 
  theNHistos++;
  G4int ih = 66300+theNHistos;
  theAnaMgr->CreateHisto1D(hisName.c_str(),nXmax-nXmin+1,ConvertN2DimX(nXmin)-theDimX,ConvertN2DimX(nXmax)+theDimX,ih);

  GmHisto1* his = theAnaMgr->GetHisto1( ih );

  for( unsigned int copyNo = 0; copyNo < theMap.size(); copyNo++ ) {
    G4double val = (theMap)[copyNo];
    G4int ix = copyNo%theNVoxelX;
    if( ix < nXmin || ix > nXmax ) continue;
    G4int iy = (copyNo/theNVoxelX)%theNVoxelY;
    if( iy < nYmin || iy > nYmax ) continue;
    G4int iz = copyNo/theNVoxelX/theNVoxelY;
    if( iz < nZmin || iz > nZmax ) continue;
    G4double hx = ConvertN2DimX(ix);
    G4double dose = val*theNevInv*theUnitInv;
    G4double doseError = theScorer->GetError( copyNo )*theUnitInv;
    //    G4cout << " FILLX " << copyNo << " " << ix << " " << iy << " " << iz << " dose " << dose << " dose " << dose << " +- " << sqrt(doseError) << G4endl;
    G4double hError = his->GetBinError(ix+1);
    his->Fill( hx, dose );
    //    G4cout << dose << " =?= " << his->GetBinContent(ix+1) << " HXERROR " << ix << " " << hError << " =?= " << his->GetBinError(ix+1) << " " << doseError << G4endl;
    his->SetBinError(ix+1, sqrt(sqr(hError)+sqr(doseError)));
  }
  
}

//------------------------------------------------------------------
void RTPSPDoseHistosVector::FillHisto1N_Y( const G4String& hisName, G4int nYmin, G4int nYmax, G4int nXmin, G4int nXmax, G4int nZmin, G4int nZmax )
{ 
  theNHistos++;
  G4int ih = 66300+theNHistos;
  theAnaMgr->CreateHisto1D(hisName.c_str(),nYmax-nYmin+1,ConvertN2DimY(nYmin)-theDimY,ConvertN2DimY(nYmax)+theDimY,ih);

  GmHisto1* his = theAnaMgr->GetHisto1( ih );

  std::map<G4int,G4double*>::const_iterator ite;
  for( unsigned int copyNo = 0; copyNo < theMap.size(); copyNo++ ) {
    G4double val = (theMap)[copyNo];
    G4int ix = copyNo%theNVoxelX;
    if( ix < nXmin || ix > nXmax ) continue;
    G4int iy = (copyNo/theNVoxelX)%theNVoxelY;
    if( iy < nYmin || iy > nYmax ) continue;
    G4int iz = copyNo/theNVoxelX/theNVoxelY;
    if( iz < nZmin || iz > nZmax ) continue;
    G4double hy = ConvertN2DimY(iy);
    G4double dose = val*theNevInv*theUnitInv;
    G4double doseError = theScorer->GetError( copyNo )*theUnitInv;
    G4double hError = his->GetBinError(iy);
    his->Fill( hy, dose );
    //    G4cout << " HY " << hy << " " << dose << G4endl;
    his->SetBinError(iy+1, sqrt(sqr(hError)+sqr(doseError)));
  }
  
}


//------------------------------------------------------------------
void RTPSPDoseHistosVector::FillHisto1N_Z( const G4String& hisName, G4int nZmin, G4int nZmax, G4int nXmin, G4int nXmax, G4int nYmin, G4int nYmax )
{ 
  theNHistos++;
  G4int ih = 66300+theNHistos;
  theAnaMgr->CreateHisto1D(hisName.c_str(),nZmax-nZmin+1,ConvertN2DimZ(nZmin)-theDimZ,ConvertN2DimZ(nZmax)+theDimZ,ih);

  GmHisto1* his = theAnaMgr->GetHisto1( ih );

  std::map<G4int,G4double*>::const_iterator ite;
  for( unsigned int copyNo = 0; copyNo < theMap.size(); copyNo++ ) {
    G4double val = (theMap)[copyNo];
    G4int ix = copyNo%theNVoxelX;
    if( ix < nXmin || ix > nXmax ) continue;
    G4int iy = (copyNo/theNVoxelX)%theNVoxelY;
    if( iy < nYmin || iy > nYmax ) continue;
    G4int iz = copyNo/theNVoxelX/theNVoxelY;
    if( iz < nZmin || iz > nZmax ) continue;
    G4double dose = val*theNevInv*theUnitInv;
    G4double hz = ConvertN2DimZ(iz);
    G4double doseError = theScorer->GetError( copyNo )*theUnitInv;
    G4double hError = his->GetBinError(iz);
    his->Fill( hz, dose );
    //    his->SetBinError(iz, sqrt(doseError) );
    his->SetBinError(iz+1, sqrt(sqr(hError)+sqr(doseError)) );
    //    G4cout << " HZ " << hz << " " << dose << " " << his->GetBinContent(ibinz) << " " << his->GetBinError(ibinz) << " " << iz << " " << hz << " " << ibinz << G4endl;
  }
  
}


//------------------------------------------------------------------
void RTPSPDoseHistosVector::FillHisto2N_XY( std::vector<G4String>& wl  )
{
  G4int nXmin = GmGenUtils::GetInt(wl[2]);
  G4int nXmax = GmGenUtils::GetInt(wl[3]);
  G4int nYmin = GmGenUtils::GetInt(wl[4]);
  G4int nYmax = GmGenUtils::GetInt(wl[5]);
  G4int nZmin = GmGenUtils::GetInt(wl[6]);
  G4int nZmax = GmGenUtils::GetInt(wl[7]);
  G4String hisName = wl[0];

  CheckHistoLimits( nXmin, nXmax, nYmin, nYmax, nZmin, nZmax );
  FillHisto2N_XY( hisName, nXmin, nXmax, nYmin, nYmax, nZmin, nZmax );

}

//------------------------------------------------------------------
void RTPSPDoseHistosVector::FillHisto2N_XZ( std::vector<G4String>& wl  )
{
  G4int nXmin = GmGenUtils::GetInt(wl[2]);
  G4int nXmax = GmGenUtils::GetInt(wl[3]);
  G4int nYmin = GmGenUtils::GetInt(wl[6]);
  G4int nYmax = GmGenUtils::GetInt(wl[7]);
  G4int nZmin = GmGenUtils::GetInt(wl[4]);
  G4int nZmax = GmGenUtils::GetInt(wl[5]);
  G4String hisName = wl[0];

  CheckHistoLimits( nXmin, nXmax, nYmin, nYmax, nZmin, nZmax );
  FillHisto2N_XZ( hisName, nXmin, nXmax, nZmin, nZmax, nYmin, nYmax );

}

//------------------------------------------------------------------
void RTPSPDoseHistosVector::FillHisto2N_YZ( std::vector<G4String>& wl  )
{
  G4int nXmin = GmGenUtils::GetInt(wl[6]);
  G4int nXmax = GmGenUtils::GetInt(wl[7]);
  G4int nYmin = GmGenUtils::GetInt(wl[2]);
  G4int nYmax = GmGenUtils::GetInt(wl[3]);
  G4int nZmin = GmGenUtils::GetInt(wl[4]);
  G4int nZmax = GmGenUtils::GetInt(wl[5]);
  G4String hisName = wl[0];

  CheckHistoLimits( nXmin, nXmax, nYmin, nYmax, nZmin, nZmax );
  FillHisto2N_YZ( hisName, nYmin, nYmax, nZmin, nZmax, nXmin, nXmax );

}

//------------------------------------------------------------------
void RTPSPDoseHistosVector::FillHisto2N_XY( const G4String& hisName, G4int nXmin, G4int nXmax, G4int nYmin, G4int nYmax, G4int nZmin, G4int nZmax )
{ 
  theNHistos++;
  G4int ih = 66300+theNHistos;
  theAnaMgr->CreateHisto2D(hisName.c_str(),
			   nXmax-nXmin+1,ConvertN2DimX(nXmin)-theDimX,ConvertN2DimX(nXmax)+theDimX,
			   nYmax-nYmin+1,ConvertN2DimY(nYmin)-theDimY,ConvertN2DimY(nYmax)+theDimY,ih);
  GmHisto2* his = theAnaMgr->GetHisto2( ih );

  std::map<G4int,G4double*>::const_iterator ite;
  for( unsigned int copyNo = 0; copyNo < theMap.size(); copyNo++ ) {
    G4double val = (theMap)[copyNo];
    G4int ix = copyNo%theNVoxelX;
    if( ix < nXmin || ix > nXmax ) continue;
    G4int iy = (copyNo/theNVoxelX)%theNVoxelY;
    if( iy < nYmin || iy > nYmax ) continue;
    G4int iz = copyNo/theNVoxelX/theNVoxelY;
    if( iz < nZmin || iz > nZmax ) continue;
    G4double hx = ConvertN2DimX(ix);
    G4double hy = ConvertN2DimY(iy);
    G4double dose = val*theNevInv*theUnitInv;
    G4double doseError = theScorer->GetError( copyNo )*theUnitInv;
    G4double hError = his->GetBinError(ix+1,iy+1);
    his->Fill( hx, hy, dose);
    his->SetBinError(ix+1, iy+1, sqrt(sqr(hError)+sqr(doseError)));
    //    G4cout << " HX " << hx << " " << dose << " i " << ix << " hx G4endl;
    //    his->SetBinError(iz, sqrt(doseError) );
  }
  
}

//------------------------------------------------------------------
void RTPSPDoseHistosVector::FillHisto2N_XZ( const G4String& hisName, G4int nXmin, G4int nXmax, G4int nZmin, G4int nZmax, G4int nYmin, G4int nYmax )
{ 
  theNHistos++;
  G4int ih = 66300+theNHistos;
  theAnaMgr->CreateHisto2D(hisName.c_str(),
			   nXmax-nXmin+1,ConvertN2DimX(nXmin)-theDimX,ConvertN2DimX(nXmax)+theDimX,
			   nZmax-nZmin+1,ConvertN2DimZ(nZmin)-theDimZ,ConvertN2DimZ(nZmax)+theDimZ,ih);
  GmHisto2* his = theAnaMgr->GetHisto2( ih );

  std::map<G4int,G4double*>::const_iterator ite;
  for( unsigned int copyNo = 0; copyNo < theMap.size(); copyNo++ ) {
    G4double val = (theMap)[copyNo];
    G4int ix = copyNo%theNVoxelX;
    if( ix < nXmin || ix > nXmax ) continue;
    G4int iy = (copyNo/theNVoxelX)%theNVoxelY;
    if( iy < nYmin || iy > nYmax ) continue;
    G4int iz = copyNo/theNVoxelX/theNVoxelY;
    if( iz < nZmin || iz > nZmax ) continue;
    G4double hx = ConvertN2DimX(ix);
    G4double hz = ConvertN2DimZ(iz);
    G4double dose = val*theNevInv*theUnitInv;
    G4double doseError = theScorer->GetError( copyNo )*theUnitInv;
    G4double hError = his->GetBinError(ix+1,iz+1);
    his->Fill( hx, hz, dose);
    his->SetBinError(ix+1, iz+1, sqrt(sqr(hError)+sqr(doseError)));
    //    G4cout << " HX " << hx << " " << dose << G4endl;
    //    his->SetBinError(iz, sqrt(doseError) );
  }
  
}

//------------------------------------------------------------------
void RTPSPDoseHistosVector::FillHisto2N_YZ( const G4String& hisName, G4int nYmin, G4int nYmax, G4int nZmin, G4int nZmax, G4int nXmin, G4int nXmax )
{ 
  theNHistos++;
  G4int ih = 66300+theNHistos;
  theAnaMgr->CreateHisto2D(hisName.c_str(),
			   nYmax-nYmin+1,ConvertN2DimY(nYmin)-theDimY,ConvertN2DimY(nYmax)+theDimY,
			   nZmax-nZmin+1,ConvertN2DimZ(nZmin)-theDimZ,ConvertN2DimZ(nZmax)+theDimZ,ih);
  GmHisto2* his = theAnaMgr->GetHisto2( ih );

  for( unsigned int copyNo = 0; copyNo < theMap.size(); copyNo++ ) {
    G4double val = (theMap)[copyNo];
    G4int ix = copyNo%theNVoxelX;
    if( ix < nXmin || ix > nXmax ) continue;
    G4int iy = (copyNo/theNVoxelX)%theNVoxelY;
    if( iy < nYmin || iy > nYmax ) continue;
    G4int iz = copyNo/theNVoxelX/theNVoxelY;
    if( iz < nZmin || iz > nZmax ) continue;
    G4double hy = ConvertN2DimY(iy);
    G4double hz = ConvertN2DimZ(iz);
    G4double dose = val*theNevInv*theUnitInv;
    G4double doseError = theScorer->GetError( copyNo )*theUnitInv;
    G4double hError = his->GetBinError(iy+1,iz+1);
    his->Fill( hy, hz, dose);
    his->SetBinError(iy+1, iz+1, sqrt(sqr(hError)+sqr(doseError)));
    //    G4cout << " HX " << hx << " " << dose << G4endl;
    //    his->SetBinError(iz, sqrt(doseError) );
  }
  
}


//------------------------------------------------------------------
G4double RTPSPDoseHistosVector::ConvertN2DimX(G4int nX)
{
  //  G4cout << "  ConvertN2DimX " << nX << " -> " << (-theNVoxelX+2*nX+1)*theDimX + thePhantomTranslation.x() << " nVoxelX " << theNVoxelX << " dimX " << theDimX << G4endl;
  return (-theNVoxelX+2*nX+1)*theDimX + thePhantomTranslation.x();
}

//------------------------------------------------------------------
G4double RTPSPDoseHistosVector::ConvertN2DimY(G4int nY)
{
  //G4cout << "  ConvertN2DimY " << nY << " -> " << (-theNVoxelY+2*nY+1)*theDimY + thePhantomTranslation.y() << G4endl;

  return (-theNVoxelY+2*nY+1)*theDimY + thePhantomTranslation.y();
}


//------------------------------------------------------------------
G4double RTPSPDoseHistosVector::ConvertN2DimZ(G4int nZ)
{
  //G4cout << "  ConvertN2DimZ " << nZ << " -> " << (-theNVoxelZ+2*nZ+1)*theDimZ + thePhantomTranslation.z() << G4endl;
  return (-theNVoxelZ+2*nZ+1)*theDimZ + thePhantomTranslation.z();
}

//------------------------------------------------------------------
void RTPSPDoseHistosVector::FillHisto1( GmHisto1* his, G4int ibin, G4double val, G4double error )
{
  G4double herror = 0.;
  if( his->GetBinContent(ibin+1) != 0.) {
    herror = his->GetBinError(ibin+1);
  } 
  his->SetBinContent(ibin+1, his->GetBinContent(ibin+1)+val);
  his->SetBinError(ibin+1, sqrt(herror*herror+error*error) );
}


//------------------------------------------------------------------
void RTPSPDoseHistosVector::DumpAllVector( std::vector<G4double>* RunMap, GmVPrimitiveScorerVector* scorer )
{
  theScorer = scorer;
  theMap = *RunMap;

  //----- Get minimum and maximum dose
  G4double minDose = 1.E99;
  G4double maxDose = 0.;
  theUnitInv = 1./scorer->GetUnit();
  theNevInv = 1./GmNumberOfEvent::GetNumberOfEvent();
  //  std::map<G4int,G4double*>::iterator ite;
  for( unsigned int idx = 0; idx < theMap.size(); idx++ ) {
    G4double val = (*RunMap)[idx];
    if( val == 0) continue;
    if( val < minDose ) minDose = val;
    if( val > maxDose ) maxDose = val;
  }  
  minDose *= theUnitInv*theNevInv;
  maxDose *= theUnitInv*theNevInv;
  G4cout << " MINIMUM DOSE " << minDose << G4endl;
  G4cout << " MAXIMUM DOSE " << maxDose << G4endl;
//--- Average errors
  G4double theF20 = 0.;
  G4double theF50 = 0.;
  G4double theF90 = 0.;
  G4double theDose20 = maxDose*0.2;
  G4double theDose50 = maxDose*0.5;
  G4double theDose90 = maxDose*0.9;
  G4int theN20 = 0;
  G4int theN50 = 0;
  G4int theN90 = 0;
  //  G4cout << " RTPSPDoseHistosVector::DumpAll  entries " << RunMap->entries() << G4endl;

  if( !theDoseHeader ){
    theRegParam = GmGeometryUtils::GetInstance()->GetPhantomParam();
    theNVoxelX = G4int(theRegParam->GetNoVoxelsX());
    theNVoxelY = G4int(theRegParam->GetNoVoxelsY());
    theNVoxelZ = G4int(theRegParam->GetNoVoxelsZ());
    theDimX = theRegParam->GetVoxelHalfX();
    theDimY = theRegParam->GetVoxelHalfY();
    theDimZ = theRegParam->GetVoxelHalfZ();

  } else {
    theNVoxelX = G4int(theDoseHeader->GetNoVoxelsX());
    theNVoxelY = G4int(theDoseHeader->GetNoVoxelsY());
    theNVoxelZ = G4int(theDoseHeader->GetNoVoxelsZ());
    theDimX = theDoseHeader->GetVoxelHalfX();
    theDimY = theDoseHeader->GetVoxelHalfY();
    theDimZ = theDoseHeader->GetVoxelHalfZ();
  }
  //---- Get nz for Z of dose maximum
  // ite += theNVoxelX/2 + theNVoxelX*theNVoxelY/2; // centre voxel in X & Y
  G4double maxDoseZ = 0.;
  //  for(; ite != RunMap->GetMap()->end(); ite+=theNVoxelX*theNVoxelY){
  for( unsigned int idx = 0; idx < RunMap->size(); idx++ ) {
    G4double val = (*RunMap)[idx];
    G4int copyNo = idx;
    G4int nx = (unsigned int)(copyNo%theNVoxelX);
    G4int ny = (unsigned int)( (copyNo/theNVoxelX)%theNVoxelY );
    if( nx == theNVoxelX/2 && ny == theNVoxelY/2 ) {
      if( val > maxDoseZ ){
	maxDoseZ = val;
      }
    }
  }
  //---- Get nz for Z of dose maximum and nz at Z=50, 100 & 150 mm
  GmHisto1* hisDV = theAnaMgr->GetHisto1D(36202);
  
  for( unsigned int idx = 0; idx < RunMap->size(); idx++ ) {
    G4double val = (*RunMap)[idx];
    //  G4int copyNo = ite->first;
    //  G4int nx = (unsigned int)(copyNo%theNVoxelX);
    //  G4int ny = (unsigned int)( (copyNo/theNVoxelX)%theNVoxelY );
    //  G4int nz = (unsigned int)(copyNo/(theNVoxelX*theNVoxelY));
    //    G4cout << " RTPSPDoseHistosVector::DumpAll copyNo " << copyNo << " " << nx << " " << ny << " " << nz << G4endl;
    
    val = val*theNevInv*theUnitInv;
    //    G4double error = scorer->GetError( (*ite).first )*theUnitInv;
    
    G4double log10val = log10(val);
    theAnaMgr->GetHisto1D(36201)->Fill(log10val);

    for( G4int ih = 1; ih < hisDV->GetNbinsX(); ih++ ){
      if( hisDV->GetBinLowEdge(ih) < log10val ) {
	hisDV->SetBinContent(ih, hisDV->GetBinContent(ih)+1 );
      }
    }
    
    if( val > theDose20 ) {
      theF20 += (scorer->GetError( idx )*theUnitInv)/val;
      theN20++;
      //      G4cout << theN20 << "F20 " << theF20 << " val " << val << " /" << theScorer->GetError( (*ite).first )*theUnitInv << " error " << theScorer->GetError( (*ite).first ) << " index " << (*ite).first << G4endl;
    }
    if( val > theDose50 ) {
      theF50 += (scorer->GetError( idx )*theUnitInv)/val;
      theN50++;
    }
    if( val > theDose90 ) {
      theF90 += (scorer->GetError( idx )*theUnitInv)/val;
      theN90++;
      // G4cout << copyNo << " F90 " << theF90 << " N90 " << theN90 << " " << scorer->GetError( (*ite).first )*theUnitInv << " " <<  val << G4endl;
    }
  }
  if( theN20 != 0. ) { 
    theF20 /= theN20;
  } else {
    theF20 = 0.;
  }
  if( theN50 != 0. ) { 
    theF50 /= theN50;
  } else {
    theF50 = 0.;
  }
  if( theN90 != 0. ) { 
    theF90 /= theN90;
  } else {
    theF90 = 0.;
  }
  G4cout << scorer->GetName() << " AVERAGE ERROR 20% = " << theF20 << G4endl;
  G4cout << scorer->GetName() << " AVERAGE ERROR 50% = " << theF50 << G4endl;
  G4cout << scorer->GetName() << " AVERAGE ERROR 90% = " << theF90 << G4endl;
  
  FillHisto1N_X("RTPSPDoseHistosVector:Dose Profile X_merged",0,theNVoxelX-1,0,theNVoxelY-1,0,theNVoxelZ-1);
  FillHisto1N_Y("RTPSPDoseHistosVector:Dose Profile Y_merged",0,theNVoxelY-1,0,theNVoxelX-1,0,theNVoxelZ-1);
  FillHisto1N_Z("RTPSPDoseHistosVector:Dose Profile Z_merged",0,theNVoxelZ-1,0,theNVoxelX-1,0,theNVoxelY-1);

  FillHisto2N_XY("RTPSPDoseHistosVector:Dose XY_merged",0,theNVoxelX-1,0,theNVoxelY-1,0,theNVoxelZ-1);
  FillHisto2N_XZ("RTPSPDoseHistosVector:Dose XZ_merged",0,theNVoxelX-1,0,theNVoxelZ-1,0,theNVoxelY-1);
  FillHisto2N_YZ("RTPSPDoseHistosVector:Dose YZ_merged",0,theNVoxelY-1,0,theNVoxelZ-1,0,theNVoxelX-1);

}

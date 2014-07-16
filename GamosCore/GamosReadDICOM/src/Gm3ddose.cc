#include "Gm3ddose.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-----------------------------------------------------------------------
Gm3ddose::Gm3ddose()
{
  theHeader = 0;
}

//-----------------------------------------------------------------------
std::ostream& operator<<(std::ostream &out, 
			 const Gm3ddose &gmdose)
{
  out << *(gmdose.GetHeader());
  size_t siz = gmdose.GetDoses().size();
  size_t nz = gmdose.GetHeader()->GetNoVoxelZ();
  std::vector<float> dose = gmdose.GetDoses();
  std::vector<float> doseError = gmdose.GetDoseErrors();

  //  G4cout << &gmdose << " writing dose " << dose[0] << " " << dose[1] << G4endl;
  for( size_t ii = 0; ii < siz; ii++ ) {
    out << dose[ii];
    if( ii%nz == nz-1) {
      out << G4endl;
    } else {
      out << " ";
    }
  }

  for( size_t ii = 0; ii < siz; ii++ ) {
    out << doseError[ii];
    if( ii%nz == nz-1) {
      out << G4endl;
    } else {
      out << " ";
    }
  }

  return out;

}

//-----------------------------------------------------------------------
void Gm3ddose::Read( G4String& fileName )
{
  GmFileIn fin = GmFileIn::GetInstance( fileName );
  Read(fin);
}

//-----------------------------------------------------------------------
Gm3ddose::Gm3ddose(const GmSqdose& sqd)
{
  theHeader = new Gm3ddoseHeader(*(sqd.GetHeader()));

  theDoses = sqd.GetDoses();
  std::vector<float> doseSqs = sqd.GetDoseSqs();

  float nev = theHeader->GetNumberOfEvents();
  float nev1= nev - 1;
  size_t nv = theDoses.size();
  for( size_t jj = 0; jj < nv; jj++ ){
    theDoseErrors.push_back( (doseSqs[jj]*nev - theDoses[jj]*theDoses[jj] ) / nev1);
  }

}


//-----------------------------------------------------------------------
void Gm3ddose::Read( GmFileIn& fin )
{
  if( theHeader != 0 ){
    G4Exception("Gm3ddose::Read",
		"Wrong argument",
		FatalErrorInArgument, 
		"This dose does have a header already");
  }
  theHeader = new Gm3ddoseHeader;

  theHeader->Read( fin );

  size_t nv = theHeader->GetNoVoxelX() *
  theHeader->GetNoVoxelY() * 
	       theHeader->GetNoVoxelZ();
  std::vector<G4String> wl;
  size_t iv = 0;
  for( ;; ){
    if( ! fin.GetWordsInLine(wl) ) break;
    for( size_t jj = 0; jj < wl.size(); jj++ ){
      theDoses.push_back( GmGenUtils::GetValue(wl[jj]) );
      //      if( theDoses.size()%100000 == 1 ) G4cout << " READ dose " << theDoses.size() << " = " << theDoses[theDoses.size()-1] << G4endl;
    }
    iv+= wl.size();
    if( iv == nv ) break;
    if( iv > nv ) G4Exception("Gm3ddose::Read",
			      "Too many dose values",FatalErrorInArgument,G4String("It must have "+GmGenUtils::itoa(nv)+" and it has "+GmGenUtils::itoa(iv)+" values").c_str());
  }

  iv = 0;
  for( ;; ){
    if( ! fin.GetWordsInLine(wl) ) break;
    for( size_t jj = 0; jj < wl.size(); jj++ ){
      theDoseErrors.push_back( GmGenUtils::GetValue(wl[jj]) );
      //      if( theDoseErrors.size()%100000 == 1 ) G4cout << " READ dose error " << theDoseErrors.size() << " = " << theDoseErrors[theDoseErrors.size()-1] << G4endl;
    }
    iv+= wl.size();
    if( iv == nv ) break;
    if( iv > nv ) G4Exception("Gm3ddose::Read",
			      "Too many dose error values",FatalErrorInArgument,G4String("It must have "+GmGenUtils::itoa(nv)+" and it has "+GmGenUtils::itoa(iv)+" values").c_str());
  }

  if( theDoses.size() != theDoseErrors.size() ){
    G4Exception("Gm3ddose::Read",
		"Wrong format of dose file",FatalErrorInArgument,G4String(" number of dose values = " + GmGenUtils::itoa(theDoses.size())+ "must be equal to number of error values"+GmGenUtils::itoa(theDoseErrors.size())).c_str());
  }

  //  G4cout << this <<" dose value " << theDoses[0] << G4endl;
}

//-----------------------------------------------------------------------
Gm3ddose Gm3ddose::operator+=( const Gm3ddose& gmdose )
{
  size_t siz = theDoses.size();
  std::vector<float> doseB = gmdose.GetDoses();
  std::vector<float> doseErrorB = gmdose.GetDoseErrors();

  //  G4double nevA = theHeader->GetNumberOfEvents();
  // G4double nevB = gmdose.GetHeader()->GetNumberOfEvents();
  //  G4double nevSUM = nevA + nevB;
  for( size_t ii = 0; ii < siz; ii++ ) {
    //    if( ii == 0 ) G4cout << this << " dose1 " << theDoses[ii] << " " << &gmdose << " doseB " << doseB[ii] << G4endl;
    theDoses[ii] = theDoses[ii] + doseB[ii];
    theDoseErrors[ii] = sqrt(sqr(theDoseErrors[ii]) + sqr(doseErrorB[ii]));
    //    theDoses[ii] = (theDoses[ii]*nevA + doseB[ii]*nevB) / nevSUM;
    //    theDoseErrors[ii] = sqrt(sqr(theDoseErrors[ii]*nevA) + sqr(doseErrorB[ii]*nevB)/sqr(nevSUM*nevSUM));
    //    if( ii == 0 ) G4cout << "summed dose1 " << theDoses[ii] << " doseB " << doseB[ii] << G4endl;
  }

  //  G4cout << "dose value sum " << theDoses[0] << G4endl;

  return *this;
}

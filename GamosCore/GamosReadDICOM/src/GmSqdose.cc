#include "GmSqdose.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//-----------------------------------------------------------------------
GmSqdose::GmSqdose()
{
  theHeader = 0;
  G4String stype = GmParameterMgr::GetInstance()->GetStringValue("GmSqdose:FileType","ALL");
  if( stype == "ALL" || stype == "all") {
    theType = SqTALL;
  } else if( stype == "FILLED" ||  stype == "filled") {
    theType = SqTFILLED;
  } else {
    G4Exception("GmSqdose::GmSqdose",
		"Wrong FileType",
		FatalErrorInArgument,
		G4String("FileType can be ALL/all or FILLED/filled, it is " + stype ).c_str());
  }
}

//-----------------------------------------------------------------------
GmSqdose::~GmSqdose()
{
  // delete theHeader;
}

//-----------------------------------------------------------------------
void GmSqdose::Print( FILE* out )
{
  theHeader->Print(out);

  if(fwrite(&theType,
	    sizeof(size_t),1,out)!=1){
    G4Exception("GmSqdose::Print",
		"Error",
		FatalException,
		"Error writing type");
  }

  if( theType == SqTALL ) {
    size_t siz = theHeader->GetNoVoxelX()*
      theHeader->GetNoVoxelY()*
      theHeader->GetNoVoxelZ();
    //----- Loop to all voxels and print dose
    for( size_t ii = 0; ii < siz; ii++ ) {
      float dose = theDoses[ii];
      if(fwrite((float *) &dose,
		sizeof(float),1,out)!=1) {
	G4Exception("GmSqdose::Print",
		    "Error",
		    FatalException,
		    "Error writing dose");
      }
    }
    
    for( size_t ii = 0; ii < siz; ii++ ) {
      float dose2 = theDoseSqs[ii];
      if(fwrite((float *) &dose2,
		sizeof(float),1,out)!=1){
	G4Exception("GmSqdose::Print",
		    "Error",
		    FatalException,
		    "Error writing doseSq");
      }
    }
  } else if( theType == SqTFILLED ) {
    size_t siz = theDoses.size();
    for( size_t ii = 0; ii < siz; ii++ ) {
      float dose = theDoses[ii];
      float dose2 = theDoseSqs[ii];
      if( dose != 0. ) {
	if(fwrite(&ii,
		  sizeof(size_t),1,out)!=1){
	  G4Exception("GmSqdose::Print",
		      "Error",
		      FatalException,
		      "Error writing index");
	}
	if(fwrite((float *) &dose,
		  sizeof(float),1,out)!=1){
	  G4Exception("GmSqdose::Print",
		    "Error",
		    FatalException,
		      "Error writing dose");
	}
	if(fwrite((float *) &dose2,
		  sizeof(float),1,out)!=1){
	  G4Exception("GmSqdose::Print",
		    "Error",
		    FatalException,
		      "Error writing doseSq");
	} 
	//	G4cout << " WRITING DOSE " << ii << " " << dose << " " << dose2 << G4endl;     
      }
    }
    
  }

}

//-----------------------------------------------------------------------
void GmSqdose::Read( G4String fileName )
{
  FILE * fin = fopen(fileName,"rb");
  Read(fin);
}


//-----------------------------------------------------------------------
void GmSqdose::Read( FILE* fin )
{
  if( theHeader != 0 ){
    G4Exception("GmSqdose::Read",
		    "Error",
		    FatalException,
		"This dose does have a header already");
  }
  theHeader = new GmSqdoseHeader;
  theHeader->Read( fin );

  size_t nv = theHeader->GetNoVoxelX() *
    theHeader->GetNoVoxelY() * 
    theHeader->GetNoVoxelZ();
 
  if( fread(&theType, sizeof(size_t),  1, fin) != 1) {
    G4Exception("GmSqdose::Read",
		    "Error",
		    FatalException,
		"Problem reading type ");
  }
  G4cout << " GmSqdose::Read type " << theType << G4endl;

  if( theType == 1 ) {
    for( size_t iv = 0; iv < nv; iv++ ){
      float ftmp;
      if( fread(&ftmp, sizeof(float),  1, fin) != 1) {
	G4Exception("GmSqdose::Read()",
"Problem reading dose ",FatalErrorInArgument,G4String("Reading voxel number "+GmGenUtils::itoa(iv)).c_str());
      }
      theDoses.push_back( ftmp );
      //    if( theDoses.size()%100000 == 1 ) G4cout << iv << " READ dose " << theDoses.size() << " = " << theDoses[theDoses.size()-1] << G4endl;
    }
    
    for( size_t iv = 0; iv < nv; iv++ ){
      float ftmp;
      if( fread(&ftmp, sizeof(float),  1, fin) != 1) {
	G4Exception("GmSqdose::Read()","Problem reading dose ",FatalErrorInArgument,G4String("Reading voxel number "+GmGenUtils::itoa(iv)).c_str());
      }
      theDoseSqs.push_back( ftmp );
      //    if( theDoseSqs.size()%100000 == 1 ) G4cout << iv << " READ dose2 " << theDoseSqs.size() << " = " << theDoseSqs[theDoseSqs.size()-1] << G4endl;
    }
  } else if( theType == 2 ){
    G4int idPrev = -1;
    size_t iv;
    for( iv = 0; iv < nv; iv++ ){
      size_t idNow;
      if( fread(&idNow, sizeof(size_t), 1, fin) != 1) {
	if(feof(fin)) {
	  break;
	} else {
	  G4Exception("GmSqdose::Read()","Problem reading voxel ID ",FatalErrorInArgument,G4String("Reading voxel number "+GmGenUtils::itoa(idNow)).c_str());
	}
      }
      float ftmp;
      if( fread(&ftmp, sizeof(float),  1, fin) != 1) {
	G4Exception("GmSqdose::Read()","Problem reading dose ",FatalErrorInArgument,G4String("Reading voxel number "+GmGenUtils::itoa(idNow)).c_str());
      }
      for( size_t ii = 0; ii < idNow-idPrev-1; ii++ ){
	theDoses.push_back( 0. );
      }
      theDoses.push_back( ftmp );

      if( fread(&ftmp, sizeof(float),  1, fin) != 1) {
	G4Exception("GmSqdose::Read()","Problem reading dose ",FatalErrorInArgument,G4String("Reading voxel number "+GmGenUtils::itoa(idNow)).c_str());
      }
      for( size_t ii = 0; ii < idNow-idPrev-1; ii++ ){
	theDoseSqs.push_back( 0. );
      }
      theDoseSqs.push_back( ftmp );

      //      G4cout << iv << " READ DOSE " << idNow << " prev " << idPrev << " dose " << ftmp1 << " dose2 " << ftmp << G4endl;

      idPrev = idNow;
      //    if( theDoseSqs.size()%100000 == 1 ) G4cout << iv << " READ dose2 " << theDoseSqs.size() << " = " << theDoseSqs[theDoseSqs.size()-1] << G4endl;
    }
    G4cout << " GmSqdose Number of voxels read " << iv << " out of " << nv << G4endl;
  }
}

//-----------------------------------------------------------------------
GmSqdose GmSqdose::operator+=( const GmSqdose& gmdose )
{
  theHeader->SetNumberOfEvents(theHeader->GetNumberOfEvents()+gmdose.GetHeader()->GetNumberOfEvents());
  
  std::vector<float> doses = gmdose.GetDoses();
  std::vector<float> doses2 = gmdose.GetDoseSqs();

  size_t siz = theDoses.size();
  if( doses.size() > siz ) {
    for( size_t ii = siz; ii < doses.size(); ii++ ) {
      theDoses[ii] = 0.;
    }
  }
  
  //  G4double nevA = theHeader->GetNumberOfEvents();
  //  G4double nevB = gmdose.GetHeader()->GetNumberOfEvents();
  // G4double nevSUM = nevA + nevB;
  // G4cout << " nevSUM " << nevSUM  << " nevA  " <<  nevA << " nevB " << nevB << G4endl;
  for( size_t ii = 0; ii < siz; ii++ ) {
    //    if( ii == 0 ) G4cout << this << " dose1 " << theDoses[ii] << " " << &gmdose << " dose2 " << dose2[ii] << G4endl;
    //    theDoses[ii] = (theDoses[ii]*nevA + doses[ii]*nevB) / nevSUM;
    //   theDoseSqs[ii] = (theDoseSqs[ii]*nevA*nevA + doses2[ii]*nevB*nevB)/(nevSUM*nevSUM);
    if( ii < doses.size() ) {
      theDoses[ii] = theDoses[ii] + doses[ii];
      theDoseSqs[ii] = theDoseSqs[ii] + doses2[ii];
    }

    // if( ii == 0 )  G4cout << "summed dose1 " << theDoses[ii] << " dose2 " << doses[ii] << G4endl;
    // if( ii == 0)  G4cout << "summed doseSq1 " << theDoseSqs[ii] << " dose2 " << doses2[ii] << G4endl;
  }

  //  G4cout << "dose value sum " << theDoses[0] << G4endl;

  return *this;
}

//--------------------------------------------------------------------
void GmSqdose::CalculateErrors()
{
  size_t nvox = theDoses.size();
  G4double nEvents = theHeader->GetNumberOfEvents();
  for( size_t ii = 0; ii < nvox; ii++ ){
    G4double error = (theDoseSqs[ii]*nEvents - theDoses[ii]*theDoses[ii]) / (nEvents-1);
    if( error < 0. ) {
      if( error < -1.E-30 ) G4cerr << " !!WARNING GmSqdose::CalculateErrors negative error, set to 0. " << error << G4endl;
      error = 0.;
    } else {
      error = std::sqrt(error)/nEvents;
    }

    theErrors.push_back( error );
  
    //     G4cout << " GetError " << ii q<< " e= " << error << " S1 " << theDoseSqs[ii]*nEvents << " s2 " << theDoses[ii]*theDoses[ii] << " S3 " << (nEvents*nEvents*(nEvents-1)) << G4endl;
  }
}

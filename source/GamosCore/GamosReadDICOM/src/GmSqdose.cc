#include "GmSqdose.hh"
#include "Gm3ddose.hh"
#include "GmReadDICOMVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include <limits>
G4bool GmSqdose::bNo0Dose = false;

//-----------------------------------------------------------------------
GmSqdose::GmSqdose()
{

  theHeader = 0;
  G4String stype = GmParameterMgr::GetInstance()->GetStringValue("GmSqdose:FileType","ALL");
  for(size_t ii = 0; ii < stype.length(); ii++) {
    stype[ii] = toupper(stype[ii]);
  }
  if( stype == "ALL" ) {
    theType = SqTALL;
  } else if( stype == "FILLED" ) {
    theType = SqTFILLED;
  } else if( stype == "CROSS_PHANTOM" || stype == "CROSSPHANTOM" ) {
    theType = SqTCROSS_PHANTOM;
  } else {
    G4Exception("GmSqdose::GmSqdose",
		"Wrong FileType",
		FatalErrorInArgument,
		G4String("FileType can be ALL, FILLED or CROSS_PHANTOM, it is " + stype ).c_str());
  }

  bBelowFloatPrecisionSetTo0 = GmParameterMgr::GetInstance()->GetNumericValue("GmSqdose:BelowFloatPrecisionSetTo0",1);
  //   G4cout << this << " bBelowFloatPrecisionSetTo0 " << bBelowFloatPrecisionSetTo0 << G4endl;  //GDEB

}

//-----------------------------------------------------------------------
GmSqdose::~GmSqdose()
{
  // delete theHeader;
}

//Size of bool: 1
//Size of char: 1
//Size of unsigned short int: 2
//Size of short int: 2
//Size of unsigned long int: 4
//Size of long: 4
//Size of int: 4
//Size of unsigned long long: 8
//Size of long long: 8
//Size of unsigned int: 4
//Size of float: 4
//Size of double: 8


//-----------------------------------------------------------------------
void GmSqdose::Print( FILE* out )
{
  theHeader->Print(out);

#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) G4cout << "GmSqdose::Print TYPE " << theType << G4endl;
#endif
  if(GmGenUtils::fwriteLittleEndian4(&theType,
				     sizeof(size_t),1,out)!=1){
    G4Exception("GmSqdose::Print",
		"Error",
		FatalException,
		"Error writing type");
  }

  size_t siz = 0;
  if( theType == SqTALL ) {
    siz = theHeader->GetNoVoxelsX()*
      theHeader->GetNoVoxelsY()*
      theHeader->GetNoVoxelsZ();
  } else if ( theType == SqTCROSS_PHANTOM ) {
    siz = theDoses.size();
  }
  //  G4cout << " N VOXELS WRITE " << siz << G4endl;  //GDEB
  if( theType == SqTALL || theType == SqTCROSS_PHANTOM ) {
    //----- Loop to all voxels and print dose
    //    G4cout << "WRITE SQDOSE TYPE " << theType <<  "NVOXELS " << siz << G4endl; //GDEB
    for( size_t ii = 0; ii < siz; ii++ ) {
      float dose = theDoses[ii];
      //      G4cout << sizeof(dose) << " WRITE SQDOSE " << dose << " " << ii << G4endl; //GDEB
      if(GmGenUtils::fwriteLittleEndian4(&dose,
		sizeof(float),1,out)!=1) {
	G4Exception("GmSqdose::Print",
		    "Error",
		    FatalException,
		    "Error writing dose");
      }
    }
    
    for( size_t ii = 0; ii < siz; ii++ ) {
      float dose2 = theDoseSqs[ii];
      if(GmGenUtils::fwriteLittleEndian4(&dose2,
			    sizeof(float),1,out)!=1){
	G4Exception("GmSqdose::Print",
		    "Error",
		    FatalException,
		    "Error writing doseSq");
      }
    }
  } else if( theType == SqTFILLED ) {
    siz = theDoses.size();
    for( size_t ii = 0; ii < siz; ii++ ) {
      float dose = theDoses[ii];
      float dose2 = theDoseSqs[ii];
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(warningVerb) ) if( ii%1000000 == 0 ) G4cout << " GmSqdose Print DOSE " << ii <<" " << dose << " " << dose2 << G4endl;
#endif
      if( dose != 0. ) {
	if(GmGenUtils::fwriteLittleEndian4(&ii,
		  sizeof(size_t),1,out)!=1){
	  G4Exception("GmSqdose::Print",
		      "Error",
		      FatalException,
		      "Error writing index");
	}
	if(GmGenUtils::fwriteLittleEndian4(&dose,
		  sizeof(float),1,out)!=1){
	  G4Exception("GmSqdose::Print",
		    "Error",
		    FatalException,
		      "Error writing dose");
	}
	if(GmGenUtils::fwriteLittleEndian4(&dose2,
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
    
  if( !fin ) {
    G4Exception("GmSqdose::Read",
		"Error",
		FatalException,
		("File not found: "+ fileName).c_str());
  }

  Read(fin);
}


//-----------------------------------------------------------------------
void GmSqdose::Read( FILE* fin )
{
  if( !fin ) {
    G4Exception("GmSqdose::Read",
		"Error",
		FatalException,
		"File not found");
  }

    if( theHeader != 0 ){
    G4Exception("GmSqdose::Read",
		    "Error",
		    FatalException,
		"This dose does have a header already");
  }
  theHeader = new GmSqdoseHeader;
  theHeader->Read( fin );

  if( GmGenUtils::freadLittleEndian4(&theType, sizeof(size_t), 1, fin) != 1) {
    G4Exception("GmSqdose::Read",
		    "Error",
		    FatalException,
		"Problem reading type ");
  }
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(-warningVerb) ) G4cout << " GmSqdose::Read type " << theType << G4endl;
#endif

  size_t nv = 0;
  if( theType == SqTALL ) {
    nv = theHeader->GetNoVoxelsX()*
      theHeader->GetNoVoxelsY()*
      theHeader->GetNoVoxelsZ();
  } else if ( theType == SqTCROSS_PHANTOM ) {
    nv = (theHeader->GetNoVoxelsX()+theHeader->GetNoVoxelsY()-1)*
      theHeader->GetNoVoxelsZ();
    //    G4cout << " SQDOSE_CROSS nv " << nv << " " << theHeader->GetNoVoxelsX() << " " << theHeader->GetNoVoxelsY() << " " <<  theHeader->GetNoVoxelsZ() << G4endl; //GDEB
  }
  //  G4cout << " READ nv " << nv << G4endl; //GDEB  

  G4int nVoxX =  theHeader->GetNoVoxelsX();
  G4int nVoxY =  theHeader->GetNoVoxelsY();
  G4int nVoxZ =  theHeader->GetNoVoxelsZ();
  G4int nVoxXY = nVoxX * nVoxY;
  G4double minX =  theHeader->GetMinX();
  G4double minY =  theHeader->GetMinY();
  G4double minZ =  theHeader->GetMinZ();
  G4double voxX = theHeader->GetVoxelHalfX()*2;
  G4double voxY = theHeader->GetVoxelHalfY()*2;
  G4double voxZ = theHeader->GetVoxelHalfZ()*2;
  //  G4cout << " VOXXYZ " << voxX << " " <<voxY << " " << voxZ << G4endl; //GDEB
  if( theType == SqTALL || theType == SqTCROSS_PHANTOM  ) {
    for( size_t iv = 0; iv < nv; iv++ ){
      //      G4cout << " READ iv " << iv << G4endl; //GDEB
      float ftmp;
      if( GmGenUtils::freadLittleEndian4(&ftmp, sizeof(float), 1, fin) != 1) {
	G4Exception("GmSqdose::Read()",
		    "Problem reading dose ",
		    FatalErrorInArgument,
		    G4String("Reading voxel number "+GmGenUtils::itoa(iv)).c_str());
      }
      theDoses.push_back( ftmp );
      G4int iz = G4int(iv/nVoxXY);
      G4int ix = G4int(iv%nVoxX);
      G4int iy = G4int(iv/nVoxX%nVoxY);
      //      if( iy == nVoxY-1 && ftmp != 0 )  G4cout << " GmSqdose " << ix << ":" << iy << ":" << iz << " copyNo " << iv << " XYZ  " << minX+voxX*(ix+0.5) << " " <<minY+voxY*(iy+0.5) << " " <<minZ+voxZ*(iz+0.5) <<  " = " << ftmp << G4endl; //GDEB RATON.Water

      // G4cout << this << "  " << iv << " 1SQDOSE " << ftmp << " N " << theDoses.size() << G4endl; //GDEB 
      //      G4cout << " 1SQDOSE " << ftmp << " " << iv << G4endl; //GDEB
    }
      
    for( size_t iv = 0; iv < nv; iv++ ){
      float ftmp;
      if( GmGenUtils::freadLittleEndian4(&ftmp, sizeof(float), 1, fin) != 1) {
	G4Exception("GmSqdose::Read()",
		    "Problem reading dose ",
		    FatalErrorInArgument,
		    G4String("Reading voxel number "+GmGenUtils::itoa(iv)).c_str());
      }
      theDoseSqs.push_back( ftmp );
      // G4cout << iv << " 1SQDOSESQ " << ftmp << G4endl; //GDEB
      //      G4cout << " 2SQDOSE CROSS " << ftmp << " " << iv << G4endl; //GDEB
      //      if( theDoseSqs.size()%1000 == 1 ) G4cout << iv << " READ dose2 " << theDoseSqs.size() << " = " << theDoseSqs[theDoseSqs.size()-1] << G4endl;//GDEB
    }
    G4cout << " GmSqdose Number of voxels read " << nv+nv+1 << G4endl;
  } else if( theType == SqTFILLED ){
    G4int idPrev = -1;
    size_t iv;
    for( iv = 0; iv < nv; iv++ ){
      size_t idNow;
      if( GmGenUtils::freadLittleEndian4(&idNow, sizeof(size_t), 1, fin) != 1) {
	if(feof(fin)) {
	  break;
	} else {
	  G4Exception("GmSqdose::Read()","Problem reading voxel ID ",FatalErrorInArgument,G4String("Reading voxel number "+GmGenUtils::itoa(idNow)).c_str());
	}
      }
      float ftmp;
      if( GmGenUtils::freadLittleEndian4(&ftmp, sizeof(float),  1, fin) != 1) {
	G4Exception("GmSqdose::Read()","Problem reading dose ",FatalErrorInArgument,G4String("Reading voxel number "+GmGenUtils::itoa(idNow)).c_str());
      }
      for( size_t ii = 0; ii < idNow-idPrev-1; ii++ ){
	theDoses.push_back( 0. );
      }
      theDoses.push_back( ftmp );
      //      if( idNow%1000 == 0 ) G4cout << " 2READ dose " << theDoses.size() << " = " << theDoses[theDoses.size()-1] << G4endl;//GDEB

      if( GmGenUtils::freadLittleEndian4(&ftmp, sizeof(float),  1, fin) != 1) {
	G4Exception("GmSqdose::Read()","Problem reading dose ",FatalErrorInArgument,G4String("Reading voxel number "+GmGenUtils::itoa(idNow)).c_str());
      }
      for( size_t ii = 0; ii < idNow-idPrev-1; ii++ ){
	theDoseSqs.push_back( 0. );
      }
      theDoseSqs.push_back( ftmp );
      //      if( theDoseSqs.size()%1000 == 1 ) G4cout << iv << " READ dose2 " << theDoseSqs.size() << " = " << theDoseSqs[theDoseSqs.size()-1] << G4endl;//GDEB

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
  G4float nev1 = theHeader->GetNumberOfEvents();
  G4float nev2 = gmdose.GetHeader()->GetNumberOfEvents();
  G4float nev1Sq = nev1*nev1;
  G4float nev2Sq = nev2*nev2;
  G4float nevT = nev1+nev2;
  G4float nevTSq = nevT*nevT;
  theHeader->SetNumberOfEvents(nevT);
  std::vector<float> doses = gmdose.GetDoses();
  std::vector<float> doseSqs = gmdose.GetDoseSqs();

  size_t siz = theDoses.size();
  //  G4cout << this << " GmSqdose+= " << siz << " " << doses.size() << G4endl; //GDEB
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
    //   theDoseSqs[ii] = (theDoseSqs[ii]*nevA*nevA + doseSqs[ii]*nevB*nevB)/(nevSUM*nevSUM);
    if( ii < doses.size() ) {
      if( GmSqdose::bNo0Dose ) {
	if( theDoses[ii] == 0. ) {
	  theDoses[ii] = doses[ii];
	  theDoseSqs[ii] = doseSqs[ii];
	  continue;
	}
	if( doses[ii] == 0. ) {
	  continue;
	}
      }
      G4bool bError0 = 0; // if both errors are 0, set DoseSqs so that new error is 0
      G4double errd1 = theDoses[ii]==0 ? 0 : theDoseSqs[ii]*nev1 - theDoses[ii]*theDoses[ii];
      //      G4cout << ii << " errd1 " << errd1 << " = " << theDoseSqs[ii] << " * " << nev1<< "  - " <<theDoses[ii]*theDoses[ii] <<  " : " << theDoses[ii]  << " :: " <<  sqrt(fabs(errd1))/theDoses[ii] << G4endl; //GDEB
      
      if( theDoses[ii] == 0 || fabs(errd1)/theDoses[ii]/theDoses[ii] < 5.e-07 ) {
	//	G4double errd2 = doses[ii]==0 ? 0 : doseSqs[ii]*nev2 - doses[ii]*doses[ii];      
	G4double errd2 = doseSqs[ii]*nev2 - doses[ii]*doses[ii];      
	//	G4cout << ii << " errd2 " << errd2 << " = " << doseSqs[ii] << " * " << nev2<< "  - " <<doses[ii]*doses[ii] << " : " << doses[ii] << " :: " << sqrt(fabs(errd2))/doses[ii] << " ::: " << fabs(errd2)/doses[ii]/doses[ii] << G4endl; //GDEB
	if( doses[ii] == 0 || fabs(errd2)/doses[ii]/doses[ii] < 5.e-07 ) {
	  bError0 = 1;
	}
      }
      theDoses[ii] = (theDoses[ii]*nev1 + doses[ii]*nev2)/nevT;
      if( bError0 ) {
	theDoseSqs[ii] = theDoses[ii]*theDoses[ii]/nevT;
	//	G4cout << ii << " NEW ERROR " << theDoseSqs[ii] << " " << nevT << " " << theDoseSqs[ii]*theHeader->GetNumberOfEvents()- theDoses[ii]*theDoses[ii] << " : " << theDoses[ii] << G4endl; //GDEB
      } else {
	theDoseSqs[ii] = (theDoseSqs[ii]*nev1Sq + doseSqs[ii]*nev2Sq)/nevTSq;
      }
      // if error=0, make error of sum = 0

    }
    //    if( theDoses[ii] != 0 )  G4cout << ii << "summed dose1 " << theDoses[ii] << " dose2 " << doses[ii] << G4endl;
    // if( ii == 0)  G4cout << "summed doseSq1 " << theDoseSqs[ii] << " dose2 " << doseSqs[ii] << G4endl;
  }

  //  G4cout << "dose value sum " << theDoses[0] << G4endl;

  return *this;
}

//-----------------------------------------------------------------------
GmSqdose GmSqdose::operator*=( G4double factor )
{
  G4double factor2 = factor*factor;

  size_t siz = theDoses.size();
  
  for( size_t ii = 0; ii < siz; ii++ ) {
    //    if( ii == 0 ) G4cout << this << " dose1 " << theDoses[ii] << " " << &gmdose << " dose2 " << dose2[ii] << G4endl;
    //    theDoses[ii] = (theDoses[ii]*nevA + doses[ii]*nevB) / nevSUM;
    //   theDoseSqs[ii] = (theDoseSqs[ii]*nevA*nevA + doseSqs[ii]*nevB*nevB)/(nevSUM*nevSUM);
    theDoses[ii] *= factor;
    theDoseSqs[ii] *= factor2;
    // if( ii == 0 )  G4cout << "summed dose1 " << theDoses[ii] << " dose2 " << doses[ii] << G4endl;
    // if( ii == 0)  G4cout << "summed doseSq1 " << theDoseSqs[ii] << " dose2 " << doseSqs[ii] << G4endl;
  }

  //  G4cout << "dose value sum " << theDoses[0] << G4endl;

  return *this;
}

//-----------------------------------------------------------------------
GmSqdose GmSqdose::operator*=( const GmSqdose& gmdose )
{
  G4float nev1 = theHeader->GetNumberOfEvents();
  G4float nev2 = gmdose.GetHeader()->GetNumberOfEvents();
  G4float nev1Sq = nev1*nev1;
  G4float nev2Sq = nev2*nev2;
  G4float nevT = nev1*nev2;
  G4float nevTSq = nevT*nevT;
  theHeader->SetNumberOfEvents(nevT);
  std::vector<float> doses = gmdose.GetDoses();
  std::vector<float> doseSqs = gmdose.GetDoseSqs();

  size_t siz = theDoses.size();
  if( doses.size() > siz ) {
    for( size_t ii = siz; ii < doses.size(); ii++ ) {
      theDoses[ii] = 0.;
    }
  }
  
  for( size_t ii = 0; ii < siz; ii++ ) {
    if( GmSqdose::bNo0Dose ) {
      if( theDoses[ii] == 0. ) {
	theDoses[ii] = doses[ii];
	theDoseSqs[ii] = doseSqs[ii];
	continue;
      }
      if( doses[ii] == 0. ) {
	continue;
      }
    }
    if( ii < doses.size() ) {
      G4bool bError0 = 0; // if both errors are 0, set DoseSqs so that new error is 0
      G4double errd1 = theDoses[ii]==0 ? 0 : theDoseSqs[ii]*nev1 - theDoses[ii]*theDoses[ii];
      //      G4cout << ii << " errd1 " << errd1 << " = " << theDoseSqs[ii] << " * " << nev1<< "  - " <<theDoses[ii]*theDoses[ii] <<  " : " << theDoses[ii]  << " :: " <<  sqrt(fabs(errd1))/theDoses[ii] << G4endl; //GDEB
      
      if( theDoses[ii] == 0 || fabs(errd1)/theDoses[ii]/theDoses[ii] < 5.e-07 ) {
	//	G4double errd2 = doses[ii]==0 ? 0 : doseSqs[ii]*nev2 - doses[ii]*doses[ii];      
	G4double errd2 = doseSqs[ii]*nev2 - doses[ii]*doses[ii];      
	//	G4cout << ii << " errd2 " << errd2 << " = " << doseSqs[ii] << " * " << nev2<< "  - " <<doses[ii]*doses[ii] << " : " << doses[ii] << " :: " << sqrt(fabs(errd2))/doses[ii] << " ::: " << fabs(errd2)/doses[ii]/doses[ii] << G4endl; //GDEB
	if( doses[ii] == 0 || fabs(errd2)/doses[ii]/doses[ii] < 5.e-07 ) {
	  bError0 = 1;
	}
      }
      theDoses[ii] = (theDoses[ii]*nev1 * doses[ii]*nev2)/nevT;
      
      if( bError0 ) {
	theDoseSqs[ii] = theDoses[ii]*theDoses[ii]/nevT;
	//	G4cout << ii << " NEW ERROR " << theDoseSqs[ii] << " " << nevT << " " << theDoseSqs[ii]*theHeader->GetNumberOfEvents()- theDoses[ii]*theDoses[ii] << " : " << theDoses[ii] << G4endl; //GDEB
      } else {
	theDoseSqs[ii] = (theDoseSqs[ii]*nev1Sq * doseSqs[ii]*nev2Sq)/nevTSq;
      }
      // if error=0, make error of sum = 0

    }
    //    if( theDoses[ii] != 0 )  G4cout << ii << "summed dose1 " << theDoses[ii] << " dose2 " << doses[ii] << G4endl;
    // if( ii == 0)  G4cout << "summed doseSq1 " << theDoseSqs[ii] << " dose2 " << doseSqs[ii] << G4endl;
  }

  //  G4cout << "dose value sum " << theDoses[0] << G4endl;

  return *this;
}

//-----------------------------------------------------------------------
void GmSqdose::CalculateErrors()
{
  size_t nvox = theDoses.size();
  G4double nEvents = theHeader->GetNumberOfEvents();
  
  for( size_t ii = 0; ii < nvox; ii++ ){
    G4double error = (theDoseSqs[ii]*nEvents - theDoses[ii]*theDoses[ii]) / (nEvents-1); 
    //    if(error != 0 ) G4cout << ii << " CalculateErrors error " << error << " = " << theDoseSqs[ii] << " * " << nEvents << " - " << theDoses[ii]*theDoses[ii] << " : " << theDoses[ii]<<  G4endl; // GDEB
    //    G4cerr << " FLOAT_MIN " << std::numeric_limits<float>::min() << G4endl;1.17549e-38 ?? 1.4013e-45
    //-- float precision problem: correct setting error_REL = 1 (assume dose value is very small, so error is 100%)
    if( theDoseSqs[ii] == 0. && theDoses[ii] ) {
      error = theDoses[ii]*theDoses[ii];
    }  // after this, no negative error should happen becasue of this
    // THIS HAPPENS IF THE DOSE HAS BEEN MULTIPLIED BY A NUMBER IF THE DOSE WAS VERY SMALL THAT DOSESQ WAS APPROXIMATED TO 0, AND DOSE*DOSE WAS ALSO APPROXIMATED TO 0, BUT AFTER MULTIPLICATION DOSE*DOSE <> 0  !!WARNING GmSqdose::CalculateErrors negative error, set to 0. 4384 : 0 = 0 * 3.2e+07 - 0 / 3.2e+07 : 8.80046e-26

    if( sqrt(fabs(error))/theDoses[ii] < 1.44129e-05 ) {
      //    if( bBelowFloatPrecisionSetTo0 && sqrt(fabs(error))/theDoses[ii] < 1.44129e-05 ) {
      error = 0.; // no negative error should happen
      //    G4cout << this << " " << ii << " SET ERROT TO 0 " << bBelowFloatPrecisionSetTo0 << G4endl;
    }
    //    if( theDoses[ii] != 0 )
    if( error < 0. ) { // probably form DICOM2G4n when image had error =0
      //      if( error < -1.E-30 ) G4cerr << " !!WARNING GmSqdose::CalculateErrors negative error, set to 0. " << error << G4endl;
      //-- send a warning only if relative error > 1e-4
      if( theDoses[ii] == 0 || sqrt(fabs(error))/theDoses[ii] > 1.e-4 ) G4cerr << " !!WARNING GmSqdose::CalculateErrors negative error, set to 0. " << ii << " : " << error << " = " << theDoseSqs[ii] << " * " << nEvents << " - " << theDoses[ii]*theDoses[ii] << " / " << (nEvents-1) << " : " <<  theDoses[ii]  << " REL " << sqrt(fabs(error))/theDoses[ii] << G4endl;
      error = 0.;
    } else {
      error = std::sqrt(error);
    }
    
    //    if(error != 0 ) G4cout << ii << " CalculateErrors error " << error << " = " << theDoseSqs[ii] << " * " << nEvents << " - " << theDoses[ii]*theDoses[ii] << " : " << theDoses[ii]<<  G4endl; // GDEB
 
    theDoseErrors.push_back( error );
    //    if( ii%100000 == 1 ) G4cout << " DOSE GetError " << ii << " " << theDoses[ii] << " +- " << error << " " << theDoseSqs[ii] << " nev " << nEvents << G4endl;
    /*    if( ii >1 && ii < 1000  ) {
      G4cout << ii << " SQ FINAL ERROR " << theDoseErrors[ii]<< G4endl;  //GDEB
      } */
  }
}

//-----------------------------------------------------------------------
GmSqdose::GmSqdose(const Gm3ddose& dose3d)
{
  theType = SqTALL;

  theHeader = new GmSqdoseHeader(*(dose3d.GetHeader()));

  //  G4cout << " GmSqdose(const Gm3ddose& NV " << theHeader->GetNoVoxelsX()<< " " << theHeader->GetNoVoxelsY() << " " << theHeader->GetNoVoxelsZ() << G4endl; //GDEB
 
  theDoses = dose3d.GetDoses();
  theDoseErrors = dose3d.GetDoseErrors();

  G4double nEvents = theHeader->GetNumberOfEvents();
  size_t nv = theDoses.size();
  for( size_t jj = 0; jj < nv; jj++ ){
    theDoseSqs.push_back( (theDoseErrors[jj]*theDoses[jj]*theDoseErrors[jj]*theDoses[jj]*nEvents*nEvents*(nEvents-1)+theDoses[jj]*theDoses[jj])/nEvents/1.E3 );
    //?    theDoses[jj] *= nEvents/1.E3;
    theDoses[jj] *= nEvents;
    //    G4double error = (theDoseSqs[jj]*nEvents - theDoses[jj]*theDoses[jj]) / (nEvents-1);
    //    if(jj%1000 == 0 ) G4cout << " GmSqdose DOSE " << jj << " " << theDoses[jj] << " +- " << theDoseErrors[jj] << " D2 " << theDoseSqs[jj] << G4endl; //GDEB
   }

}

//-----------------------------------------------------------------------
G4String GmSqdose::GetTypeStr() const
{
  switch (theType) {
  case SqTALL:
    return "SqTALL";
  case SqTFILLED:
    return "SqTFILLED";
  case SqTCROSS_PHANTOM:
    return "SqTCROSS_PHANTOM";
  };
  return "NOTYPE";

}


//-----------------------------------------------------------------------
void GmSqdose::Displace( G4ThreeVector theDisp ) 
{
  GmSqdoseHeader* head = GetHeader();    
  head->Displace(theDisp);
}

//-----------------------------------------------------------------------
void GmSqdose::SumDisplaced( GmSqdose* doseNew )
{

  GmSqdoseHeader* head1 = GetHeader();    
  GmSqdoseHeader* head2 = doseNew->GetHeader();

  //----- Check the two has same voxel width
  G4double voxelWidthX1 = head1->GetVoxelHalfX()*2;
  G4double voxelWidthX2 = head2->GetVoxelHalfX()*2;
  G4double voxelWidthY1 = head1->GetVoxelHalfY()*2;
  G4double voxelWidthY2 = head2->GetVoxelHalfY()*2;
  G4double voxelWidthZ1 = head1->GetVoxelHalfZ()*2;
  G4double voxelWidthZ2 = head2->GetVoxelHalfZ()*2;

  if( voxelWidthX1 != voxelWidthX2 || voxelWidthY1 != voxelWidthY2 || voxelWidthZ1 != voxelWidthZ2 ) {
    G4cerr << "  GmSqdose::SumDisplaced: voxelWidthX1/2 " <<  voxelWidthX1 << " " << voxelWidthX2
	   << " voxelWidthY1/2 " <<  voxelWidthY1 << " " << voxelWidthY2
	   << " voxelWidthZ1/2 " <<  voxelWidthZ1 << " " << voxelWidthZ2 << G4endl;
    G4Exception("GmSqdose::SumDisplaced",
		"",
		FatalException,
		"VoxelWidths are not the equal");
  }
  G4float nev1 = head1->GetNumberOfEvents();
  G4float nev2 = head2->GetNumberOfEvents();
  G4float nev1Sq = nev1*nev1;
  G4float nev2Sq = nev2*nev2;
  G4float nevT = nev1+nev2;
  G4float nevTSq = nevT*nevT;

  //----- Translate relative displacement to voxelIDs
  G4double dispXMin = head2->GetMinX()-head1->GetMinX();
  G4double dispXMax = head2->GetMaxX()-head1->GetMaxX();
  G4double dispYMin = head2->GetMinY()-head1->GetMinY();
  G4double dispYMax = head2->GetMaxY()-head1->GetMaxY();
  G4double dispZMin = head2->GetMinZ()-head1->GetMinZ();
  G4double dispZMax = head2->GetMaxZ()-head1->GetMaxZ();
  G4int idxMin = int(dispXMin/voxelWidthX1);
  G4int idyMin = int(dispYMin/voxelWidthY1);
  G4int idzMin = int(dispZMin/voxelWidthZ1);
  G4int idxMax = int(dispXMax/voxelWidthX1);
  G4int idyMax = int(dispYMax/voxelWidthY1);
  G4int idzMax = int(dispZMax/voxelWidthZ1);
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) )
    G4cout << " GmSqdose::SumDisplaced " 
	   << " idxMin/Max " << idxMin <<"/"<< idxMax << " " << dispXMin << " " <<dispXMax << " " << voxelWidthX1 
	   << " idyMin/Max " << idyMin <<"/"<< idyMax << " " << dispYMin << " " <<dispYMax << " " << voxelWidthY1 
	   << " idzMin/Max " << idzMin <<"/"<< idzMax << " " << dispZMin << " " <<dispZMax << " " << voxelWidthZ1 << G4endl;
#endif
  
  std::vector<float> dosesNew = doseNew->GetDoses();
  std::vector<float> doseSqsNew = doseNew->GetDoseSqs();
  G4int nVoxelsX1 = head1->GetNoVoxelsX();
  G4int nVoxelsX2 = head2->GetNoVoxelsX();
  G4int nVoxelsY1 = head1->GetNoVoxelsY();
  G4int nVoxelsY2 = head2->GetNoVoxelsY();
  G4int nVoxelsZ1 = head1->GetNoVoxelsZ();
  G4int nVoxelsZ2 = head2->GetNoVoxelsZ();
  G4int nVoxelsXY1 = nVoxelsX1*nVoxelsY1;
  G4int nVoxelsXY2 = nVoxelsX2*nVoxelsY2;
  
  //----- Set voxel ID limits for loop
  G4int ixmin = std::max(0,idxMin);
  G4int ixmax = std::min(nVoxelsX1,nVoxelsX1+idxMax); ixmax = std::min(ixmax,nVoxelsX2+idxMax);
  //  head1->SetNoVoxelsX(ixmax-ixmin);
  G4int iymin = std::max(0,idyMin);
  G4int iymax = std::min(nVoxelsY1,nVoxelsY1+idyMax); iymax = std::min(iymax,nVoxelsY2+idyMax);
  //  head1->SetNoVoxelsY(iymax-iymin);
  G4int izmin = std::max(0,idzMin);
  G4int izmax = std::min(nVoxelsZ1,nVoxelsZ1+idzMax); izmax = std::min(izmax,nVoxelsZ2+idzMax);
  //  head1->SetNoVoxelsZ(izmax-izmin);
  head1->ChangeVoxelLimits(ixmin,ixmax,iymin,iymax,izmin,izmax);
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) )
    G4cout << " GmSqdose::SumDisplaced: ixmin/max " << ixmin<<"/"<<ixmax << " nVoxelsX1/2 " << nVoxelsX1<<"/"<<nVoxelsX2 
	   << " iymin/max " << iymin<<"/"<<iymax << " nVoxelsY1/2 " << nVoxelsY1<<"/"<<nVoxelsY2
	   << " izmin/max " << izmin<<"/"<<izmax << " nVoxelsZ1/2 " << nVoxelsZ1<<"/"<<nVoxelsZ2 << G4endl;
#endif
  std::vector<float> sumDoses(head1->GetNoVoxels());
  std::vector<float> sumDoseSqs(head1->GetNoVoxels());

  G4int copyNo = 0;
  for( G4int iz = izmin; iz < izmax; iz++ ) {
    for( G4int iy = iymin; iy < iymax; iy++ ) {
      for( G4int ix = ixmin; ix < ixmax; ix++, copyNo++ ) {
	G4int copyNo1 = ix + iy*nVoxelsY1 + iz*nVoxelsXY1;
	G4int copyNo2 = ix-idxMin + (iy-idyMin)*nVoxelsY2 + (iz-idyMin)*nVoxelsXY2;
	G4bool bError0 = 0; // if both errors are 0, set DoseSqs so that new error is 0
	G4double errd1 = theDoses[copyNo1]==0 ? 0 : theDoseSqs[copyNo1]*nev1 - theDoses[copyNo1]*theDoses[copyNo1];
	//G4cout << copyNo1 << " errd1 " << errd1 << " = " << theDoseSqs[copyNo1] << " * " << nev1<< "  - " <<theDoses[copyNo1]*theDoses[copyNo1] <<  " : " << theDoses[copyNo1]  << " :: " <<  sqrt(fabs(errd1))/theDoses[copyNo1] << G4endl; //GDEB
	if( theDoses[copyNo1] == 0 || fabs(errd1)/theDoses[copyNo1]/theDoses[copyNo1] < 5.e-07 ) {
	  //	G4double errd2 = dosesNew[copyNo2]==0 ? 0 : dosesNewNew[copyNo2]*nev2 - dosesNew[copyNo2]*dosesNew[copyNo2];      
	  G4double errd2 = dosesNew[copyNo2]*nev2 - dosesNew[copyNo2]*dosesNew[copyNo2];      
	  //	G4cout << copyNo2 << " errd2 " << errd2 << " = " << dosesNewNew[copyNo2] << " * " << nev2<< "  - " <<dosesNew[copyNo2]*dosesNew[copyNo2] << " : " << dosesNew[copyNo2] << " :: " << sqrt(fabs(errd2))/dosesNew[copyNo2] << " ::: " << fabs(errd2)/dosesNew[copyNo2]/dosesNew[copyNo2] << G4endl; //GDEB
	  if( dosesNew[copyNo2] == 0 || fabs(errd2)/dosesNew[copyNo2]/dosesNew[copyNo2] < 5.e-07 ) {
	    bError0 = 1;
	  }
	}
	//	if(theDoses[copyNo1] != 0. || dosesNew[copyNo2] != 0. ) G4cout << copyNo1 << copyNo2 << " BEF Dose1 " << theDoses[copyNo1] << " * " << nev1<< "  - " << dosesNew[copyNo2] <<  " * " << nev2 << " / " << nevT << G4endl; //GDEB
	sumDoses[copyNo] = (theDoses[copyNo1]*nev1 + dosesNew[copyNo2]*nev2)/nevT;
	//	G4cout  <<ix<<":"<<iy<<":"<<iz<< " " << copyNo << " " << copyNo1 << " " << copyNo2 << " FINAL Dose1 " << sumDoses[copyNo] << G4endl; //GDEB
	if( bError0 ) {
	  sumDoseSqs[copyNo] = theDoses[copyNo1]*theDoses[copyNo1]/nevT;
	  //	  G4cout << copyNo1 << " NEW ERROR " << theDoseSqs[copyNo1] << " " << nevT << " " << theDoseSqs[copyNo1]*theHeader->GetNumberOfEvents()- theDoses[copyNo1]*theDoses[copyNo1] << " : " << theDoses[copyNo1] << G4endl; //GDEB
	} else {
	  sumDoseSqs[copyNo] = (theDoseSqs[copyNo1]*nev1Sq + doseSqsNew[copyNo2]*nev2Sq)/nevTSq;
	  //	  G4cout <<ix<<":"<<iy<<":"<<iz<< " " << copyNo << " FINAL ERROR " << sumDoseSqs[copyNo] << " " << theDoseSqs[copyNo1] << "*" << nev1Sq << " +doseSq2 " << doseSqsNew[copyNo2] << "*" << nev2Sq << "/" << nevTSq << G4endl; //GDEB
	}
	// if error=0, make error of sum = 0
	
      }
    }
      //    if( theDoses[copyNo1] != 0 )  G4cout << copyNo1 << "summed dose1 " << theDoses[copyNo1] << " dose2 " << doses[copyNo1] << G4endl;
    // if( copyNo1 == 0)  G4cout << "summed doseSq1 " << theDoseSqs[copyNo1] << " dose2 " << dosesNew[copyNo2] << G4endl;
  }

  theDoses.clear();
  theDoses = sumDoses;
  theDoseSqs.clear();
  theDoseSqs = sumDoseSqs;
  G4cout << "dose value sum " << theDoses[0] << G4endl;

}


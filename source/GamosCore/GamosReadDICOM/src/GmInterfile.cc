#include "GmInterfile.hh"
#include "GmReadDICOMVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include <limits>

//-----------------------------------------------------------------------
GmInterfile::GmInterfile()
{

  theHeader = 0;
  G4String stype = GmParameterMgr::GetInstance()->GetStringValue("GmInterfile:FileType","test");

  bBelowFloatPrecisionSetTo0 = GmParameterMgr::GetInstance()->GetNumericValue("GmInterfile:BelowFloatPrecisionSetTo0",1);
  //   G4cout << this << " bBelowFloatPrecisionSetTo0 " << bBelowFloatPrecisionSetTo0 << G4endl;  //GDEB
}

//-----------------------------------------------------------------------
GmInterfile::~GmInterfile()
{
  // delete theHeader;
}

//-----------------------------------------------------------------------
//void GmInterfile::Print( std::string fileName, std::string fileNameHeader ) 
void GmInterfile::Print( std::string fileName ) 
{
  FILE* out = fopen(fileName.c_str(),"wb");
  Print(out);
  /*  if( fileNameHeader == "" ) {
    size_t suffixP = fileName.rfind(".");
    if( suffixP = std::string::npos ) {
      G4Exception("GmInterfile::Print",
		  "",
		  JustWarning,
		  ("fileName does not have a suffix "+fileName).c_str());
      fileNameHeader = fileName+".hv";
    } else {
      fileNameHeader = fileName.substr(fileName,suffixP)+".hv";
    }
  }

  theHeader = new GmInterFileHeader();
  theHeader->Print(fileNameHeader);
  theHeader->Print(out);
  */
 
}

//-----------------------------------------------------------------------
void GmInterfile::Print( FILE* out )
{
  if( ! theHeader ) {
    G4Exception("GmInterfile::Print",
		"",
		FatalException,
		"Interfile file cannot be printed without a header. Please check your code");
  }
  G4double theNEvents = theHeader->GetNoEvents();
  
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) G4cout << "GmInterfile::Print " << G4endl;
#endif
  G4int nVoxels = theHeader->GetNoVoxels();
   
  if( theHeader->GetNumberFormat() == "float" ) {
    float *data;
    data = (float*) malloc( nVoxels*sizeof(float));
    for( int ii = 0; ii < nVoxels; ii++ ) {
      data[ii] = theData[ii]*theNEvents;
    }
    fwrite(data,1,theHeader->GetNoVoxels()*sizeof(float), out);
  } else if( theHeader->GetNumberFormat() == "double" ) {
    double *data;
    data = (double*) malloc( nVoxels*sizeof(double));
    fwrite(data,1,theHeader->GetNoVoxels()*sizeof(double), out);
    for( int ii = 0; ii < nVoxels; ii++ ) {
      theData[ii] = data[ii]*theNEvents;
    }
  } else if( theHeader->GetNumberFormat() == "long double" ) {
    long double *data;
    data = (long double*) malloc( nVoxels*sizeof(long double));
    fwrite(data,1,theHeader->GetNoVoxels()*sizeof(long double), out);
    for( int ii = 0; ii < nVoxels; ii++ ) {
      theData[ii] = data[ii]*theNEvents;
    }
  } else {
    G4Exception("GmInterfile::Print",
		  "",
		  FatalException,
		  ("number format must be float or double or 'long double', it is "+ theHeader->GetNumberFormat()).c_str());
  }
  
  fclose(out);

}

//-----------------------------------------------------------------------
void GmInterfile::Read( G4String fileName )
{
  FILE * fin = fopen(fileName,"rb");    
  if( !fin ) {
    G4Exception("GmInterfile::Read",
		"",
		FatalException,
		("File not found: "+ fileName).c_str());
  }

  Read(fin);
}


//-----------------------------------------------------------------------
void GmInterfile::Read( FILE* fin )
{
  if( ! theHeader ) {
    G4Exception("GmInterfile::Read",
		"",
		FatalException,
		"Interfile file cannot be printed without a header. Please check your code");
  }

  size_t nVoxels = theHeader->GetNoVoxels();
  G4int nBytesPerPixel = theHeader->GetNoBytesPerPixel();
  if( theHeader->GetNumberFormat() == "float" ) {
    float *fData; 
    //theData = calloc( nVoxels,sizeof(float));
    fData = (float*) malloc(nVoxels*sizeof(float));
    //    fread(fData,1,nVoxels*nBytesPerPixel, fin);
    fread(fData,1,nVoxels*sizeof(float), fin);
    for( size_t ii = 0; ii < nVoxels; ii ++ ) {
      theData.push_back(fData[ii]);
      //      G4cout << "  GmInterfile::Read( " << ii << " : " << fData[ii] << G4endl; //GDEB
    }
  } else if( theHeader->GetNumberFormat() == "double" ) {
    double *fData;
    //theData = calloc( nVoxels,sizeof(double));
    fData = (double*) malloc(nVoxels*sizeof(double));
    fread(fData,1,nVoxels*nBytesPerPixel, fin);
    for( size_t ii = 0; ii < nVoxels; ii ++ ) {
      theData.push_back(double(fData[ii]));
    }
  } else if( theHeader->GetNumberFormat() == "long double" ) {
    long double *fData;
    //theData = calloc( nVoxels,sizeof(long double));
    fData = (long double*) malloc(nVoxels*sizeof(long double));
    fread(fData,1,nVoxels*nBytesPerPixel, fin);
    for( size_t ii = 0; ii < nVoxels; ii ++ ) {
      theData.push_back((long double)fData[ii]);
    }
  } else if( theHeader->GetNumberFormat() == "signed integer" ) {
     if(nBytesPerPixel == 1) {
      int8_t *fData;
      //theData = calloc( nVoxels,sizeof(int8_t));
      fData = (int8_t*) malloc(nVoxels*sizeof(int8_t));
      fread(fData,1,nVoxels*nBytesPerPixel, fin);
      for( size_t ii = 0; ii < nVoxels; ii ++ ) {      
	theData.push_back((int8_t)fData[ii]);
      }
   } else if(nBytesPerPixel == 2) {
      int16_t *fData;
      //theData = calloc( nVoxels,sizeof(int16_t));
      fData = (int16_t*) malloc(nVoxels*sizeof(int16_t));
      fread(fData,1,nVoxels*nBytesPerPixel, fin);
      for( size_t ii = 0; ii < nVoxels; ii ++ ) {      
	theData.push_back((int16_t)fData[ii]);
	/*	if( ii%100000 == 0 ) {
 	  G4cout << ii << " data= " << fData[ii] << G4endl; //GDEB
	  for(int16_t bit = sizeof(fData[ii])*8 - 1; bit >= 0; --bit) {
	    std::cout << ((fData[ii] >> bit) & 1);
	  }
	  std::cout << std::endl;
	}
	}*/
      }
     } else if(nBytesPerPixel == 4) {
      int32_t *fData;
      //theData = calloc( nVoxels,sizeof(int32_t));
      fData = (int32_t*) malloc(nVoxels*sizeof(int32_t));
      fread(fData,1,nVoxels*nBytesPerPixel, fin);
      for( size_t ii = 0; ii < nVoxels; ii ++ ) {      
	theData.push_back((int32_t)fData[ii]);
      }
    }
  } else {
    G4Exception("GmInterfile::Read",
		  "",
		  FatalException,
		  ("number format must be float or double or 'long double or singed integer', it is "+ theHeader->GetNumberFormat()).c_str());
  }

}

//-----------------------------------------------------------------------
GmInterfile GmInterfile::operator+=( const GmInterfile& gmdata )
{
  G4float nev1 = theHeader->GetNumberOfEvents();
  G4float nev2 = gmdata.GetHeader()->GetNumberOfEvents();
  G4float nevT = nev1+nev2;
  theHeader->SetNumberOfEvents(nevT);
  std::vector<float> data = gmdata.GetData();
  
  size_t siz = theData.size();
  if( data.size() > siz ) {
    for( size_t ii = siz; ii < data.size(); ii++ ) {
      theData[ii] = 0.;
    }
  }
  
  //  G4double nevA = theHeader->GetNumberOfEvents();
  //  G4double nevB = gmdata.GetHeader()->GetNumberOfEvents();
  // G4double nevSUM = nevA + nevB;
  // G4cout << " nevSUM " << nevSUM  << " nevA  " <<  nevA << " nevB " << nevB << G4endl;
  for( size_t ii = 0; ii < siz; ii++ ) {
    //    if( ii == 0 ) G4cout << this << " data1 " << theData[ii] << " " << &gmdata << " data2 " << data2[ii] << G4endl;
    //    theData[ii] = (theData[ii]*nevA + data[ii]*nevB) / nevSUM;
    theData[ii] = (theData[ii]*nev1 + data[ii]*nev2)/nevT;
  }
  //    if( theData[ii] != 0 )  G4cout << ii << "summed data1 " << theData[ii] << " data2 " << data[ii] << G4endl;
  
  //  G4cout << "data value sum " << theData[0] << G4endl;

  return *this;
}

//-----------------------------------------------------------------------
GmInterfile GmInterfile::operator*=( G4double factor )
{
  size_t siz = theData.size();
  
  for( size_t ii = 0; ii < siz; ii++ ) {
    //    if( ii == 0 ) G4cout << this << " data1 " << theData[ii] << " " << &gmdata << " data2 " << data2[ii] << G4endl;
    //    theData[ii] = (theData[ii]*nevA + data[ii]*nevB) / nevSUM;
    theData[ii] *= factor;
    // if( ii == 0 )  G4cout << "summed data1 " << theData[ii] << " data2 " << data[ii] << G4endl;
  }

  //  G4cout << "data value sum " << theData[0] << G4endl;

  return *this;
}


//-----------------------------------------------------------------------
void GmInterfile::Displace( G4ThreeVector theDisp ) 
{
  GmInterfileHeader* head = GetHeader();    
  head->Displace(theDisp);
}

//-----------------------------------------------------------------------
void GmInterfile::SumDisplaced( GmInterfile* fileNew )
{

  GmInterfileHeader* head1 = GetHeader();    
  GmInterfileHeader* head2 = fileNew->GetHeader();

  //----- Check the two has same voxel width
  G4double voxelWidthX1 = head1->GetVoxelHalfX()*2;
  G4double voxelWidthX2 = head2->GetVoxelHalfX()*2;
  G4double voxelWidthY1 = head1->GetVoxelHalfY()*2;
  G4double voxelWidthY2 = head2->GetVoxelHalfY()*2;
  G4double voxelWidthZ1 = head1->GetVoxelHalfZ()*2;
  G4double voxelWidthZ2 = head2->GetVoxelHalfZ()*2;

  if( voxelWidthX1 != voxelWidthX2 || voxelWidthY1 != voxelWidthY2 || voxelWidthZ1 != voxelWidthZ2 ) {
    G4cerr << "  GmInterfile::SumDisplaced: voxelWidthX1/2 " <<  voxelWidthX1 << " " << voxelWidthX2
	   << " voxelWidthY1/2 " <<  voxelWidthY1 << " " << voxelWidthY2
	   << " voxelWidthZ1/2 " <<  voxelWidthZ1 << " " << voxelWidthZ2 << G4endl;
    G4Exception("GmInterfile::SumDisplaced",
		"",
		FatalException,
		"VoxelWidths are not the equal");
  }
  G4float nev1 = head1->GetNumberOfEvents();
  G4float nev2 = head2->GetNumberOfEvents();
  G4float nevT = nev1+nev2;

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
  if( ReadDICOMVerb(-debugVerb) )
    G4cout << " GmInterfile::SumDisplaced " 
	   << " idxMin/Max " << idxMin <<"/"<< idxMax << " " << dispXMin << " " <<dispXMax << " " << voxelWidthX1 
	   << " idyMin/Max " << idyMin <<"/"<< idyMax << " " << dispYMin << " " <<dispYMax << " " << voxelWidthY1 
	   << " idzMin/Max " << idzMin <<"/"<< idzMax << " " << dispZMin << " " <<dispZMax << " " << voxelWidthZ1 << G4endl;
#endif
  
  std::vector<float> dataNew = fileNew->GetData();
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
  if( ReadDICOMVerb(-debugVerb) )
    G4cout << " GmInterfile::SumDisplaced: ixmin/max " << ixmin<<"/"<<ixmax << " nVoxelsX1/2 " << nVoxelsX1<<"/"<<nVoxelsX2 
	   << " iymin/max " << iymin<<"/"<<iymax << " nVoxelsY1/2 " << nVoxelsY1<<"/"<<nVoxelsY2
	   << " izmin/max " << izmin<<"/"<<izmax << " nVoxelsZ1/2 " << nVoxelsZ1<<"/"<<nVoxelsZ2 << G4endl;
#endif
  std::vector<float> sumData(head1->GetNoVoxels());

  G4int copyNo = 0;
  for( G4int iz = izmin; iz < izmax; iz++ ) {
    for( G4int iy = iymin; iy < iymax; iy++ ) {
      for( G4int ix = ixmin; ix < ixmax; ix++, copyNo++ ) {
	G4int copyNo1 = ix + iy*nVoxelsY1 + iz*nVoxelsXY1;
	G4int copyNo2 = ix-idxMin + (iy-idyMin)*nVoxelsY2 + (iz-idyMin)*nVoxelsXY2;
	sumData[copyNo] = (theData[copyNo1]*nev1 + dataNew[copyNo2]*nev2)/nevT;
	//	G4cout  <<ix<<":"<<iy<<":"<<iz<< " " << copyNo << " " << copyNo1 << " " << copyNo2 << " FINAL Data1 " << sumData[copyNo] << G4endl; //GDEB
      }
    }
      //    if( theData[copyNo1] != 0 )  G4cout << copyNo1 << "summed data1 " << theData[copyNo1] << " data2 " << data[copyNo1] << G4endl;
  }

  theData.clear();
  theData = sumData;
  G4cout << "data value sum " << theData[0] << G4endl;

}


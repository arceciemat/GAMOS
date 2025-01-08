#include "DicomReaderMgr.hh"

#include "DicomReaderG4dcmCT.hh"
#include "DicomReaderG4dcmNM.hh"
#include "DicomReaderG4dcmDose.hh"
#include "DicomReaderSqdose.hh"
#include "DicomReaderCT.hh"
#include "DicomReaderNM.hh"
#include "DicomReaderRTDose.hh"
#include "DicomReaderRTStruct.hh"
#include "DicomReaderVOIStruct.hh"
#include "DicomReaderTextLines.hh"
#include "DicomReaderInterfile.hh"

#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomVOperator.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomPolygonSet.hh"
#include "DICOM/DICOMBase/include/DicomVLineList.hh"  
#include "DICOM/DICOMBase/include/DicomVImageStr.hh"  

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosReadDICOM/include/GmReadPhantomStMgr.hh"

#include "G4UIcommand.hh"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcuid.h"
#ifndef ROOT5
#include "SEAL_Foundation/PluginManager/PluginManager/PluginManager.h"
#endif
DicomReaderMgr* DicomReaderMgr::theInstance = 0;
 
//-----------------------------------------------------------------------------
DicomReaderMgr* DicomReaderMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new DicomReaderMgr;
  }  
  return theInstance;
}

//-----------------------------------------------------------------------------
DicomReaderMgr::DicomReaderMgr()
{
#ifndef ROOT5
  seal::PluginManager::get()->initialise();
#endif
  theDicomMgr = DicomMgr::GetInstance();
  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  bMergeRTReaders = true;
}

//-----------------------------------------------------------------------------
void DicomReaderMgr::AddReader( DicomVReader* reader, DRModality mod )
{
  theReaders.insert(mmmdr::value_type(mod,reader));
  /*  for(mmmdr::const_iterator ite1 = theReaders.begin(); ite1 != theReaders.end(); ite1++ )  {
    G4cout << this << "  " << theReaders.size() << " DicomReaderMgr::AddedReaders " << DicomVReader::GetModalityStr(ite1->first) << " " << ite1->second << G4endl; //GDEB    
    }*/
}

//-----------------------------------------------------------------------------
void DicomReaderMgr::AddImageReader( DicomVReaderImage* reader, DRModality mod )
{
  theImageReaders.insert(mmmdri::value_type(mod,reader));
  if( DicomVerb(debugVerb) ) G4cout << reader << " " << theImageReaders.size() << " " << reader << " DicomReaderMgr::AddImageReader " << DicomVReader::GetModalityStr(mod) << " " << reader->GetName() <<  G4endl;
}

//-----------------------------------------------------------------------------
void DicomReaderMgr::DeleteReader(DicomVReader* reader)
{
  for( mmmdr::const_iterator ite = theReaders.begin(); ite != theReaders.end(); ite++ ) {
    if( reader == ite->second ) {
      theReaders.erase(ite);
      break;
    }
  }
}

//-----------------------------------------------------------------------------
void DicomReaderMgr::DeleteImageReader(DicomVReaderImage* reader)
{
  for( mmmdri::const_iterator ite = theImageReaders.begin(); ite != theImageReaders.end(); ite++ ) {
    if( reader == ite->second ) {
      if( DicomVerb(debugVerb) )  G4cout << " DeleteImageReader " << DicomVReader::GetModalityStr(reader->GetModality()) << " "<< reader->GetName() << " " << reader <<" N= " << theImageReaders.size()-1 <<  G4endl;
      theImageReaders.erase(ite);
      break;
    }
  }
  DeleteReader( reader );
}

//-----------------------------------------------------------------------------
void DicomReaderMgr::ProcessData()
{
  
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) G4cout << " DicomReaderMgr::ProcessData() " << G4endl; 
#endif
  CreateReaders();
  CreateImages();

  theDicomMgr->OperateAll();
}

//-----------------------------------------------------------------------------
void DicomReaderMgr::CreateReaders()
{
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) )  G4cout << " DicomReaderMgr::CreateReaders() " << G4endl;
#endif

  std::vector<G4String> files;
  
  files = theParamMgr->GetVStringValue("fCT",files);
  for( size_t ii = 0; ii < files.size(); ii++ ) {
    new DicomReaderCT(files[ii]);
  }
  files.clear();
  files = theParamMgr->GetVStringValue("fNM",files);
  for( size_t ii = 0; ii < files.size(); ii++ ) {
    new DicomReaderNM(files[ii]);
  }
  files.clear();
  files = theParamMgr->GetVStringValue("fRTDose",files);
  for( size_t ii = 0; ii < files.size(); ii++ ) {
    new DicomReaderRTDose(files[ii]);
  }
  files.clear();
  files = theParamMgr->GetVStringValue("fSqdose",files);
  for( size_t ii = 0; ii < files.size(); ii++ ) {
    new DicomReaderSqdose(files[ii]);
  }
  files.clear();
  files = theParamMgr->GetVStringValue("fG4dcmCT",files);
  for( size_t ii = 0; ii < files.size(); ii++ ) {
    new DicomReaderG4dcmCT(files[ii]);
  }
  files.clear();
  files = theParamMgr->GetVStringValue("fG4dcmNM",files);
  for( size_t ii = 0; ii < files.size(); ii++ ) {
    new DicomReaderG4dcmNM(files[ii]);
  }
  files.clear();
  files = theParamMgr->GetVStringValue("fG4dcmDose",files);
  for( size_t ii = 0; ii < files.size(); ii++ ) {
    new DicomReaderG4dcmDose(files[ii]);
  }
  files.clear();
  files = theParamMgr->GetVStringValue("fRTStruct",files);
  for( size_t ii = 0; ii < files.size(); ii++ ) {
    //    G4cout << " DICOMREADERMGR  new DicomReaderRTStruct " << ii << " : " << files[ii] << G4endl; //GDEB
    new DicomReaderRTStruct(files[ii]);
  }
  files.clear();
  files = theParamMgr->GetVStringValue("fVOIStruct",files);
  for( size_t ii = 0; ii < files.size(); ii++ ) {
    //    G4cout << " DICOMREADERMGR  new DicomReaderVOIStruct " << ii << " : " << files[ii] << G4endl; //GDEB
    new DicomReaderVOIStruct(files[ii]);
  }
  files.clear();
  files = theParamMgr->GetVStringValue("fTextLines",files);
  for( size_t ii = 0; ii < files.size(); ii++ ) {
    new DicomReaderTextLines(files[ii]);
  }
  files.clear();
  files = theParamMgr->GetVStringValue("fInterfile",files); 
  for( size_t ii = 0; ii < files.size(); ii++ ) {
    new DicomReaderInterfile(files[ii]);
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) {
    std::vector<DicomVReaderImage*>  imageReaders = GetImageReaders(DRM_CT,false);
    G4cout << " DicomReaderMgr N READERS " << imageReaders.size() << G4endl; //GDEB
  }
#endif
}

//-----------------------------------------------------------------------------
void DicomReaderMgr::SetCTOnlyHU( G4bool bOnlyHU ) 
{
  for( mmmdri::iterator ite = theImageReaders.begin(); ite != theImageReaders.end(); ite++) {
    //----- Find list of readers of each modality
    DRModality mod = ite->first;
    DicomVReaderImage* dri = ite->second;
    if( mod == DRM_CT ) {
      DicomReaderCT* readerCT = dynamic_cast<DicomReaderCT*>(dri);
      readerCT->SetOnlyHU(bOnlyHU);
    }
  }
}

//-----------------------------------------------------------------------------
void DicomReaderMgr::CreateImages()
{ 
  //---- Create images for image DICOM readers (other readers have built the images in ReadHeaderAndPixels)
  //  for( mmmdri::iterator ite = theImageReaders.begin(); ite != theImageReaders.end(); ite = theImageReaders.upper_bound(ite->first)) {
  mdri ZOrderedReadersCT;
  mdri ZOrderedReadersNM;
  vdri ReadersRTDose;
  for( mmmdri::iterator ite = theImageReaders.begin(); ite != theImageReaders.end(); ite++) {
    //----- Find list of readers of each modality
    DRModality mod = ite->first;
    DicomVReaderImage* dri = ite->second;
    // G4cout << " DicomReaderMgr::CreateImages( " << mod << G4endl; //GDEB
    if( mod == DRM_CT ) {
      //----- Check and merge DICOM image readers
      //--- Order readers by Z
      //      G4cout << dri << " ADD ZOrderedReadersCT " << dri->GetMaxZ() << " : " << dri->GetName() << G4endl; //GDEB
      if( ZOrderedReadersCT.find(dri->GetMaxZ()) != ZOrderedReadersCT.end() ) {
	G4Exception(" DicomReaderMgr::CreateImages ",
		    "",
		    FatalException,
		    ("Adding two CT slices with same Z " + GmGenUtils::ftoa(dri->GetMaxZ())).c_str());
      }
      ZOrderedReadersCT[dri->GetMaxZ()] = dri;
    } else if( mod == DRM_NM ) {
      //----- Check and merge DICOM image readers
      //--- Order readers by Z
      if( ZOrderedReadersNM.find(dri->GetMaxZ()) != ZOrderedReadersNM.end() ) {
	G4Exception(" DicomReaderMgr::CreateImages ",
		    "",
		    FatalException,
		    ("Adding two NM slices with same Z " + GmGenUtils::ftoa(dri->GetMaxZ())).c_str());
      }
      ZOrderedReadersNM[dri->GetMaxZ()] = dri;
      //	  G4cout << " ADD ZOrderedReaders " << vireaders[ii]->GetMaxZ() << " " << ZOrderedReaders.size() << G4endl; //GDEB
    } else if( mod == DRM_RTDose ) {
      ReadersRTDose.push_back( dri );
    }
  } 

  mdri::iterator itemv;
  //----- CT
  if( ZOrderedReadersCT.size() != 0 ) {
    DicomReaderCT* imgMergedReaderCT = 0;
    itemv = ZOrderedReadersCT.begin();
    if( ZOrderedReadersCT.size() > 1 ) {
      imgMergedReaderCT = MergeSlicesCT( ZOrderedReadersCT );
      //--- delete readers (keep only the merged one )
      for( ; itemv != ZOrderedReadersCT.end(); itemv++ ) {
	delete itemv->second;
      }
    } else {
      imgMergedReaderCT = (DicomReaderCT*)(itemv->second);
    }

    if( imgMergedReaderCT ) CreateImagesCT( imgMergedReaderCT );
    
  }

  //----- Interfile
  if( ZOrderedReadersCT.size() != 0 ) {
    /*   copy dicomReaderInterfile to DicomReaderCT 
    DicomReaderCT* imgMergedReaderCT = 0;
    if( imgMergedReaderCT ) CreateImagesCT( imgMergedReaderCT );*/
  }
  
  //----- NM
  if( ZOrderedReadersNM.size() != 0 ) {
    DicomReaderNM* imgMergedReaderNM;
    itemv = ZOrderedReadersNM.begin();
    if( ZOrderedReadersNM.size() > 1 ) {
      imgMergedReaderNM = MergeSlicesNM( ZOrderedReadersNM );
      //--- delete readers (keep only the merged one )
      for( ; itemv != ZOrderedReadersNM.end(); itemv++ ) {
	delete itemv->second;
      }
    } else {
      imgMergedReaderNM = (DicomReaderNM*)(itemv->second);
    }
    imgMergedReaderNM->DICOMCreateImages();
  }
  
  //----- RTDose
  if( ReadersRTDose.size() != 0 ) {
    if( bMergeRTReaders ) {
      DicomReaderRTDose* imgMergedReaderRTDose;
      if( ReadersRTDose.size() > 1 ) {
	imgMergedReaderRTDose = MergeRTDose(ReadersRTDose);
	//--- delete readers (keep only the merged one )
	for( size_t ii = 0 ; ii < ReadersRTDose.size(); ii++ ) {
	  delete ReadersRTDose[ii];
	}
      } else {
	imgMergedReaderRTDose = (DicomReaderRTDose*)(ReadersRTDose[0]);
      }
      imgMergedReaderRTDose->DICOMCreateImages();
    } else {
      for( size_t ii = 0 ; ii < ReadersRTDose.size(); ii++ ) {
	ReadersRTDose[ii]->DICOMCreateImages();
      }
    }
  }
  
}


//------------------  ----------------------------------------------------------
void DicomReaderMgr::CreateImagesCT( DicomReaderCT* imgMergedReaderCT )
{
  imgMergedReaderCT->DICOMCreateImages();
  
  //----- Special case for DICOM CT and DICOM RTStruct (create structure image in g4dcm)
  //--- First resize g4dcm density image and then redo g4dcm mate ID image
  DicomVOperator* operResize = theDicomMgr->FindOperator("DicomOperResize");
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << " operResize " << operResize << std::endl; 
#endif
  DicomVImage* imageDens = imgMergedReaderCT->GetMateDensImage();
  if( operResize ) {
    if( imageDens != 0 ) {  // NOT IF drawDICOM -fCT
      operResize->Operate(imageDens);
      imgMergedReaderCT->RecalculateMateIDFromMateDens();
    } else {
      operResize->Operate(imgMergedReaderCT->GetImage());
    }
  }
  //--- Calculate g4dcm structure image
  std::vector<DicomVReader*> structReaders = GetReaders(DRM_RTStruct,false);
  size_t nSt = structReaders.size();
  if( nSt != 0 ) {
    //--- Only one RTStruct is allowed to build g4dcm CT
    if( nSt != 1 ) {
      G4Exception("DicomReaderMgr::CreateImages",
		  "",
		  FatalException,
		  "Only one RTStruct is allowed to build g4dcm CT");
    }
    DicomVImageStr* imageDicomStructures = new DicomVImageStr( imageDens, "RTStruct", DIM_RTStruct );
    DicomReaderRTStruct* structRead = (DicomReaderRTStruct*)(structReaders[0]);
    DicomPolygonSet* polygonSet = structRead->GetPolygonSet();
    std::vector<DicomVLineList*> lineLists = polygonSet->GetLineLists();
    /*      for( size_t ipl = 0; ipl < lineLists.size(); ipl++ ) {
	    G4cout << ipl << " Mgr: " << lineLists.size() << " DicomReaderMgr ineList" << lineLists[ipl]->GetName() << " N " << lineLists[ipl]->GetLines().size() << G4endl; //GDEB
	    }*/
    
    G4bool bVoxelsInUseClosest = DicomParameterMgr::GetInstance()->GetNumericValue("closestPolygon",1);
     if( bVoxelsInUseClosest ) {
      DicomPolygonSet* polygonSetNew = new DicomPolygonSet(polygonSet, imageDicomStructures, polygonSet->GetOrientation() );
      structRead->FindVoxelsInXY( imageDicomStructures, polygonSetNew );
      delete polygonSetNew;
    } else {
      structRead->FindVoxelsInXY( imageDicomStructures, polygonSet );
    }	  
  }
}

//-----------------------------------------------------------------------------
void DicomReaderMgr::CheckSlices(std::map<G4double,DicomVReaderImage*> files)
{
  size_t nSlices = files.size();
  if( DicomVerb(warningVerb) ) G4cout << " DicomReaderMgr::Checking CT slices: " << nSlices << G4endl;

  G4bool uniformSliceThickness = true;
  
  if(nSlices > 1) {
    if(nSlices == 2) {
      mdri::const_iterator ite = files.begin();
      DicomVReaderImage* one = (*ite).second;
      ite++;
      DicomVReaderImage* two = (*ite).second;
      
      G4double real_distance = (two->GetLocation()-one->GetLocation())/2.;
      
      if(one->GetMaxZ() != two->GetMinZ()) {
        one->SetMaxZ(one->GetLocation()+real_distance);
        two->SetMinZ(two->GetLocation()-real_distance);
        //one->SetMinZ(one->GetLocation()-real_distance);
        //two->SetMaxZ(two->GetLocation()+real_distance);
        if(uniformSliceThickness) {
          one->SetMinZ(one->GetLocation()-real_distance);
          two->SetMaxZ(two->GetLocation()+real_distance);
        }
      }
    } else {
      mdri::iterator ite0 = files.begin();
      mdri::iterator ite1 = ite0; ite1++;
      mdri::iterator ite2 = ite1; ite2++;
      for(; ite2 != files.end(); ++ite0, ++ite1, ++ite2) {	
        DicomVReaderImage* prev = (DicomVReaderImage*)((*ite0).second);
        DicomVReaderImage* slice = (DicomVReaderImage*)((*ite1).second);
        DicomVReaderImage* next = (DicomVReaderImage*)((*ite2).second);
	//      G4cout << " CHECKING SLICE " << slice->GetLocation() << " prev " << prev->GetLocation() << " next " << next->GetLocation() << G4endl;
        G4double real_up_distance = (next->GetLocation() -
                                      slice->GetLocation())/2.;
        G4double real_down_distance = (slice->GetLocation() -
                                      prev->GetLocation())/2.;
        G4double real_distance = real_up_distance + real_down_distance;
        G4double stated_distance = slice->GetMaxZ()-slice->GetMinZ();
	//	G4cout  << prev->GetLocation() << " " << slice->GetLocation() << " " << next->GetLocation() << " <> Slice width = " << real_distance << " =? " << stated_distance <<G4endl;//GDEB

        if(fabs(real_distance - stated_distance) > 1.E-9) {
          unsigned int sliceNum = std::distance(files.begin(),ite1);
          G4cerr << "\ttDicomReaderMgr::CheckSlices - Slice Distance Error in slice [" << sliceNum 
                 << "]: Distance between this slice and slices up and down = "
                 << real_distance
                 << " <> Slice width = " << stated_distance 
		 << " Slice locations " <<prev->GetLocation() << " : " << slice->GetLocation() << " : " << next->GetLocation()
		 << " DIFFERENCE= " << real_distance - stated_distance
		 << G4endl;
	  //	  G4cerr << "!! WARNING: Geant4 will reset all slices widths to be the same (maxZ-minZ)/NoVoxelsZ " << G4endl;
	  G4cerr << "!! WARNING: Geant4 will reset slice width so that it extends between lower and upper slice " << G4endl;

          slice->SetMinZ(slice->GetLocation()-real_down_distance);
          slice->SetMaxZ(slice->GetLocation()+real_up_distance);
          
          if(ite0 == files.begin()) {
            prev->SetMaxZ(slice->GetMinZ());
            // Using below would make all slice same thickness
            //prev->SetMinZ(prev->GetLocation()-real_min_distance);
            if(uniformSliceThickness) {
              prev->SetMinZ(prev->GetLocation()-real_down_distance);
	    }            
          }
          if((unsigned int)(std::distance(files.begin(),ite2))+1 == nSlices) {
            next->SetMinZ(slice->GetMaxZ());
            // Using below would make all slice same thickness
            //next->SetMaxZ(next->GetLocation()+real_max_distance);
            if(uniformSliceThickness) {
              next->SetMaxZ(next->GetLocation()+real_up_distance); }
          }
        }
      }
    }
  }
  
}

//-----------------------------------------------------------------------------
DicomReaderCT* DicomReaderMgr::MergeSlicesCT(mdri files)
{
  if( DicomVerb(warningVerb) ) G4cout << " DicomReaderMgr::MergeSlicesCT Merging CT Files " << files.size() << G4endl;
  DicomReaderCT* fileAll = new DicomReaderCT();
  //  ite++;
  //---- Check if all slices have a different thickness than slice separation
  std::map<G4double,G4int> countSliceThickness;
  std::map<G4double,G4int> countSliceSeparation;
  mdri::const_iterator ite2 = files.begin();
  countSliceThickness[ite2->second->GetMaxZ()-ite2->second->GetMinZ()] += 1;
  ite2++;
  G4int nMergedSlices = 1;
  for ( mdri::const_iterator itePrev = files.begin(); ite2 != files.end(); itePrev++, ite2++ ) {
    G4double separ = fabs((itePrev->second->GetMinZ()+itePrev->second->GetMaxZ())/2.-(ite2->second->GetMinZ()+ite2->second->GetMaxZ())/2); // separation between the center of this slice and the previous one
    if( itePrev != files.begin() ) {
      if( fabs(separ-countSliceSeparation.begin()->first) < 1.e-6 ) separ = countSliceSeparation.begin()->first;
    }
    countSliceSeparation[ separ ] += 1;
    G4double thick = ite2->second->GetMaxZ()-ite2->second->GetMinZ();
    if( fabs(thick-countSliceThickness.begin()->first) < 1.e-6 ) thick = countSliceThickness.begin()->first;
    countSliceThickness[thick] += 1;
    //    G4cout << " SLICE_SEPARATION " << separ << " THICKNESS " << ite2->second->GetMaxZ()-ite2->second->GetMinZ() << "  NSEPAR=" << countSliceSeparation.size() << " NTHICK=" << countSliceThickness.size() << G4endl; //GDEB
    nMergedSlices++;
  }
  if( countSliceSeparation.size() == 1 && countSliceThickness.size() == 1 ) {
    G4double separ = countSliceSeparation.begin()->first;
    G4double thick = countSliceThickness.begin()->first;
    if( separ != thick ) {
      G4Exception("DicomReaderMgr::MergeSlicesCT",
		  "",
		  JustWarning,
		  ("For all previous slices thickness = "+GmGenUtils::ftoa(thick)+" is different than slice separation = "+GmGenUtils::ftoa(separ)).c_str());
      G4double diff = (thick-separ)/2.;
      // change slice thickness = separation, else make slice thickness = separation
      for ( mdri::iterator ite = files.begin(); ite != files.end(); ite++ ) {
	ite->second->SetMinZ( ite->second->GetMinZ()+diff );
	ite->second->SetMaxZ( ite->second->GetMaxZ()-diff );
      }
    }
  }
  for ( mdri::const_iterator ite = files.begin(); ite != files.end(); ite++ ) {
    (*fileAll) += *((*ite).second);
    if( ite == files.begin() ) {
      DicomReaderCT* rct = dynamic_cast<DicomReaderCT*>(ite->second);
      fileAll->SetOnlyHU( rct->IsOnlyHU() );
    } 
  }
  fileAll->SetNoVoxelsZ(nMergedSlices); // use only number of slices read, not all in set
  
  return fileAll;

}

//-----------------------------------------------------------------------------
DicomReaderNM* DicomReaderMgr::MergeSlicesNM(mdri files)
{
  if( DicomVerb(warningVerb) ) G4cout << " DicomReaderMgr::MergeSlicesNM Merging Files " << files.size() << G4endl;
  mdri::const_iterator ite = files.begin();
  DicomReaderNM* fileAll = new DicomReaderNM();
  //  ite++;
  for( ; ite != files.end(); ite++ ) {
    (*fileAll) += *((*ite).second);
  }
  return fileAll;

}

//-----------------------------------------------------------------------------
DicomReaderRTDose* DicomReaderMgr::MergeRTDose(vdri files)
{
  if( DicomVerb(warningVerb) ) G4cout << " DicomReaderMgr::MergeRTDose Merging Files " << files.size() << G4endl;
  vdri::const_iterator ite = files.begin();
  DicomReaderRTDose* fileAll = new DicomReaderRTDose();
  //  ite++;
  for( ; ite != files.end(); ite++ ) {
    fileAll->AddData(*ite);
  }
  return fileAll;

}


//-----------------------------------------------------------------------------
/*
std::vector<DicomVReader*> DicomReaderMgr::GetReaders(G4String name, G4bool bMustExist ) const
{
  std::vector<DicomVReader*> vfr;
  std::pair<mmstrfi::const_iterator,mmstrfi::const_iterator> itep = theFileReaders.equal_range(name);
  mmstrfi::const_iterator ite;
  for( ite = itep.first; ite != itep.second; ite++ ) {
    vfr.push_back( (*ite).second );
  }
  if( vfr.size() == 0 ) {
    G4ExceptionSeverity excSev = FatalException;
    if( ! bMustExist ) excSev = JustWarning;
    for( ite = theFileReaders.begin(); ite != theFileReaders.end(); ite++ ) {
      G4cerr << " FILE READER: " << (*ite).first << G4endl;
    }
    G4Exception("DicomReaderMgr::GetFileReaders",
		"",
		excSev,
		("NO IMAGE FILE READER FOUND WITH NAME " + name).c_str());
  }

  return vfr;
} */

//-----------------------------------------------------------------------------
std::vector<DicomVReaderImage*> DicomReaderMgr::GetImageReaders(DRModality mod, G4bool bMustExist ) const
{
  if( DicomVerb(infoVerb) ) {
    G4cout << " DicomReaderMgr::GetImageReaders Modality: " << DicomVReader::GetModalityStr(mod) << " bMustExist "<< bMustExist << G4endl;
  }
  std::vector<DicomVReaderImage*> vfr;
  /*  for( mmmdri::const_iterator itep1 = theImageReaders.begin(); itep1 != theImageReaders.end(); itep1++ ) {
    G4cout << " DicomReaderMgr::GetImageReaders " << DicomVReader::GetModalityStr((*itep1).first) << " : " << (*itep1).second->GetName() << " " << (*itep1).second << G4endl; //GDEB
    } */
  std::pair<mmmdri::const_iterator,mmmdri::const_iterator> itep = theImageReaders.equal_range(mod);
  G4cout <<  DicomVReader::GetModalityStr(mod) << " N READERS " << theImageReaders.size() << G4endl;
  mmmdri::const_iterator ite;
  for( ite = itep.first; ite != itep.second; ite++ ) {
    vfr.push_back( (*ite).second );
    if( DicomVerb(infoVerb) ) {
      G4cout << (*ite).second  << " GetImageReaders READER Modality: " << DicomVReader::GetModalityStr((*ite).first) << G4endl;
    }
  }
  if( vfr.size() == 0 ) {
    //    G4ExceptionSeverity excSev = FatalException;
    for( ite = theImageReaders.begin(); ite != theImageReaders.end(); ite++ ) {
      G4cout << " FILE READER Modality: " << DicomVReader::GetModalityStr((*ite).first) << G4endl; 
    }
    if( bMustExist ) { 
    G4Exception("DicomReaderMgr::GetImageReaders",
		"",
		FatalException,
		("NO IMAGE READER FOUND WITH Modality " + DicomVReader::GetModalityStr(mod)).c_str());
    /*    } else {
      if( DicomVerb(debugVerb) ) {
	G4Exception("DicomReaderMgr::GetImageReaders",
		"",
		    JustWarning,
		("NO IMAGE READER FOUND WITH Modality " + DicomVReader::GetModalityStr(mod)).c_str());
		}*/
    }
  }

  return vfr;
}

//-----------------------------------------------------------------------------
std::vector<DicomVReader*> DicomReaderMgr::GetReaders(DRModality mod, G4bool bMustExist ) const
{
  std::vector<DicomVReader*> vfr;
  std::pair<mmmdr::const_iterator,mmmdr::const_iterator> itep = theReaders.equal_range(mod);
  //  G4cout << "DicomReaderMgr::GetReaders( FOUND " <<std::distance(itep.first,itep.second) << G4endl; //GDEB
  mmmdr::const_iterator ite;
  for( ite = itep.first; ite != itep.second; ite++ ) {
    vfr.push_back( (*ite).second );
  }
  if( vfr.size() == 0 && !bMustExist ) {
    G4ExceptionSeverity excSev = FatalException;
    //    if( ! bMustExist ) excSev = JustWarning;
    if( bMustExist ) {
      for( ite = theReaders.begin(); ite != theReaders.end(); ite++ ) {
	G4cerr << " READER Modality: " << DicomVReader::GetModalityStr((*ite).first) << G4endl; 
      }
      G4Exception("DicomReaderMgr::GetReaders",
		  "",
		  excSev,
		  ("NO READER FOUND WITH Modality " + DicomVReader::GetModalityStr(mod)).c_str());
    }
  }
  
  if( DicomVerb(infoVerb) ) {
    for(mmmdr::const_iterator ite1 = theReaders.begin(); ite1 != theReaders.end(); ite1++ )  {
      G4cout << this << " " << theReaders.size() << " DicomReaderMgr::GetReaders " << DicomVReader::GetModalityStr(ite1->first) << " " << ite1->second << G4endl;
    }
  }
  
  return vfr;
}

//-----------------------------------------------------------------------------
std::vector<DicomVImage*> DicomReaderMgr::GetImages(G4bool bAll) const
{
  std::vector<DicomVImage*> images;
  mmmdr::const_iterator ite;
  //  G4cout << " GETTING IMAGE FROM N READERS " << theReaders.size() << G4endl;//GDEB
  for( ite = theReaders.begin(); ite != theReaders.end(); ite++ ) {
    DicomVReader* reader = (*ite).second;
    //    G4cout << " GETTING IMAGE FROM READER MOD " << reader->GetModalityStr(reader->GetModality()) << G4endl;//GDEB
    if( reader->GetModality() == DRM_CT ) {
      //      images.push_back( ((DicomReaderCT*)(reader))->GetMateDensImage() );
      images.push_back( ((DicomReaderCT*)(reader))->GetImage() ); // HU image
    } else if( reader->GetModality() == DRM_NM ) {
      images.push_back( ((DicomReaderNM*)(reader))->GetImage() );
    } else if( reader->GetModality() == DRM_RTDose ) {
      images.push_back( ((DicomReaderRTDose*)(reader))->GetImage() );
    } else if( reader->GetModality() == DRM_G4dcmCT ) {
      images.push_back( ((DicomReaderG4dcmCT*)(reader))->GetMateDensImage() );
      if( bAll ) {
	images.push_back( ((DicomReaderG4dcmCT*)(reader))->GetMateIDImage() );
	images.push_back( ((DicomReaderG4dcmCT*)(reader))->GetStructIDImage() );
      }
    } else if( reader->GetModality() == DRM_G4dcmNM ) {
      images.push_back( ((DicomReaderG4dcmNM*)(reader))->GetImage() );
    } else if( reader->GetModality() == DRM_G4dcmDose ) {
      images.push_back( ((DicomReaderG4dcmDose*)(reader))->GetImage() );
    } else if( reader->GetModality() == DRM_Sqdose ) {
      images.push_back( ((DicomReaderSqdose*)(reader))->GetImage() );
      if( bAll ) {
	images.push_back( ((DicomReaderSqdose*)(reader))->GetDoseSqImage() );
	images.push_back( ((DicomReaderSqdose*)(reader))->GetDoseErrImage() );
      }
    } else if( reader->GetModality() == DRM_Interfile ) {
      images.push_back( ((DicomReaderInterfile*)(reader))->GetImage() );
    }
  }

  return images;
}

//-----------------------------------------------------------------------------
size_t DicomReaderMgr::GetNofImageReaders(DRModality mod)
{
  if( mod == DRM_OTHER ) {
    return theImageReaders.size();
  } else {
    std::pair<mmmdri::const_iterator,mmmdri::const_iterator> itep = theImageReaders.equal_range(mod);
    return std::distance(itep.first,itep.second);
  }

  return 0;
  
}

//-----------------------------------------------------------------------------
size_t DicomReaderMgr::GetNofReaders(DRModality mod)
{
  std::pair<mmmdr::const_iterator,mmmdr::const_iterator> itep = theReaders.equal_range(mod);
  return std::distance(itep.first,itep.second);

  return 0;
  
}

//-----------------------------------------------------------------------------
size_t DicomReaderMgr::GetNofReaders()
{
  return theReaders.size();
  
}

//-----------------------------------------------------------------------------
std::map<G4int,G4String> DicomReaderMgr::GetStructIDsNames()
{
  std::map<G4int,G4String> stIDsNames;
  std::vector<DicomVReaderImage*> vireaders = GetImageReaders(DRM_G4dcmCT,false);
  if( vireaders.size() != 0 ) {
    std::vector<DicomVReaderImage*>::const_iterator ite;
    for( ite = vireaders.begin(); ite != vireaders.end(); ite++ ) {
      DicomReaderG4dcmCT* reader = dynamic_cast<DicomReaderG4dcmCT*>(*ite);
      if( stIDsNames.size() == 0 ) {
	stIDsNames = reader->GetStructNames();
      } else {
	if( reader->GetStructNames().size() != 0 ) {
	  G4Exception("DicomReaderMgr::GetStructIDsNames",
		      "",
		      JustWarning,
		      "More than one G4dcmCT files contain a structure, taking the last one");
	}
	stIDsNames == reader->GetStructNames();
      }
    }
   }
  
  vireaders = GetImageReaders(DRM_RTStruct,false);
  if( vireaders.size() != 0 ) {
    std::vector<DicomVReaderImage*>::const_iterator ite;
    for( ite = vireaders.begin(); ite != vireaders.end(); ite++ ) {
      DicomReaderRTStruct* reader = dynamic_cast<DicomReaderRTStruct*>(*ite);
      DicomPolygonSet* polySet = reader->GetPolygonSet();
      if( stIDsNames.size() != 0 ) {
	G4Exception("DicomReaderMgr::GetStructIDsNames",
		    "",
		    JustWarning,
		    "More than one RTStruct or G4dcmCT files contain a structure, taking the last one");
      }
      stIDsNames == polySet->GetLineListNames();
    }
  }
  
  return stIDsNames;

}


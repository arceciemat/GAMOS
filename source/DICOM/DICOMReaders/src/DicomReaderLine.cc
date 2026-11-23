#include "DicomReaderLine.hh"
#include "DICOM/DICOMBase/include/DicomLine.hh"
#include "DICOM/DICOMBase/include/DicomLineList.hh"
#include "DICOM/DICOMBase/include/DicomLineSet.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include <vector>
#include "G4ThreeVector.hh"

//-----------------------------------------------------------------------------
DicomReaderLine::DicomReaderLine(G4String fName)
{
  theFileName = fName; 
  
  GmFileIn fin = GmFileIn::GetInstance(fName);
  
  std::vector<G4String> wl;
  fin.GetWordsInLine(wl);

  theLineSet = new DicomLineSet("LINES",DPOrientNone);
  DicomLineList* lineList = new DicomLineList("LINES",theLineSet->GetLineLists().size()+1,DPOrientNone);
  theLineSet->AddLineList(lineList);
  DicomLine* line = 0;
  std::vector<G4ThreeVector> points;
  for( ;; ){
    if( ! fin.GetWordsInLine(wl) ) break;    
    if ( wl[0]!="DATA:" ) {
      //----- Read and save the name of each  in the file    
      if ( wl.size() != 2 ) {
	G4Exception("DicomReaderLine::DicomReaderLine",
		    "",
		    FatalException,
		    ("Line starting with ':DATA' should have two words: ':DATA' DATA_NAME, it has "+GmGenUtils::itoa(wl.size())).c_str());   
      }
      if( line != 0 ) line->SetPoints(points);
      points.clear();
      line = new DicomLine(wl[0],DPOrientNone);
      lineList->AddLine(line);
      //    line = new DicomLine(wl[0],lineList->GetLines().size()+1);
    } else {
      if ( wl.size() != 3 ) {
	G4Exception("DicomReaderLine::DicomReaderLine",
		    "",
		    FatalException,
		    ("Line should have 3 words: POS_X POS_Y POS_Z, it has "+GmGenUtils::itoa(wl.size())).c_str());
      }
      if ( line == 0 ) {
	G4Exception("DicomReaderLine::DicomReaderLine",
		    "",
		    FatalException,
		    "First line should be 'DATA' DATA_NAME");
      }
      points.push_back(G4ThreeVector(GmGenUtils::GetValue(wl[0]),GmGenUtils::GetValue(wl[1]),GmGenUtils::GetValue(wl[2])));      
    }
  }
  if( line != 0 ) line->SetPoints(points);
  
  fin.Close();
  
}


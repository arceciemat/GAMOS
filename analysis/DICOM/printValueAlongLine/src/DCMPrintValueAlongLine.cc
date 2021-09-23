#include "DCMPrintValueAlongLine.hh"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TFile.h"

#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMBase/include/DicomLineSet.hh"
#include "DICOM/DICOMBase/include/DicomLineList.hh"
#include "DICOM/DICOMBase/include/DicomLine.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMReaders/include/DicomReaderMgr.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

#include "ROOTUtilities/PlotData/Definitions.C"
#include "ROOTUtilities/PlotData/PlotUtils.C"
#include "ROOTUtilities/GenUtils.C"

using namespace std;
//---------------------------------------------------------------------------
DCMPrintValueAlongLine::DCMPrintValueAlongLine()
{
  Initialise();
  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  theReaderMgr = DicomReaderMgr::GetInstance();
  theDMgr = DicomMgr::GetInstance();
 
}

//---------------------------------------------------------------------------
void DCMPrintValueAlongLine::Initialise()
{
  theExeName = "printValueAlongLine";

  theLineListFN = "";

  theAlongLineInput = 0;

}

//---------------------------------------------------------------------------
void DCMPrintValueAlongLine::ProcessArguments(int argc,char** argv)
{
  //----- PROCESS PARAMETERS
  if( argc == 1 ) {
    G4Exception(theExeName.c_str(),
		  "Error",
		FatalErrorInArgument,
		"NO ARGUMENT GIVEN ");
  } else if( argc == 2 ) {
    if( G4String(argv[1]) == "-h" || G4String(argv[1]) == "-help" ) {
       PrintHelp();
       exit(1);
    } else {
      DicomException(G4String("ONLY ONE ARGUMENT: ") + argv[1]);
    }
  } else {
    if(argc%2 != 1) { 
      G4Exception(theExeName.c_str(),
		  "Error",
		  FatalErrorInArgument,
		  "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
    }
    for( G4int ii = 1; ii < argc; ii++ ){
      G4String argvstr = argv[ii];
      G4String argvstr1 = argv[ii+1];
      G4String argvName = argvstr.substr(1,999);
      if( argvstr == "-doseAlongLine" ) {
	if( argc < ii+7 ) {
	  G4Exception(theExeName,
		      "Error",
		      FatalErrorInArgument,
		      "WRONG NUMBER OF ARGUMENTS, THERE MUST BE AT LEAST 6 AFTER -doseAlongLine");
	}
	theLinePos = G4ThreeVector(GmGenUtils::GetValue(argv[ii+1]),GmGenUtils::GetValue(argv[ii+2]),GmGenUtils::GetValue(argv[ii+3]));
	theLineDir = G4ThreeVector(GmGenUtils::GetValue(argv[ii+4]),GmGenUtils::GetValue(argv[ii+5]),GmGenUtils::GetValue(argv[ii+6]));
	theAlongLineInput = 1;
	ii += 5;
      } else if( argvstr == "-zeroOfAlongLine" ) {
	if( argc < ii+3 ) {
	  G4Exception(theExeName,
		      "Error",
		      FatalErrorInArgument,
		      ("WRONG NUMBER OF ARGUMENTS, THERE MUST BE AT LEAST 3 AFTER -zeroOfAlongLine, while there are "+GmGenUtils::itoa(argc-ii-1)).c_str());
	}
	theLinePos0 = G4ThreeVector(GmGenUtils::GetValue(argv[ii+1]),GmGenUtils::GetValue(argv[ii+2]),GmGenUtils::GetValue(argv[ii+3]));
	ii += 3;
        
      } else if( argvstr == "-bHistos" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
	ii++;
      } else if( argvstr == "-fLineList" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTstring );
	theAlongLineInput = 2;
	ii++;
      } else if( argvstr == "-lineStep" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
	ii++;
      } else {
	G4int iAddPar = theParamMgr->ReadParameter( argv, ii );
	if( iAddPar == -1 ) {
	  PrintHelp();
	  G4Exception(theExeName.c_str(),
		      "Wrong argument",
		      FatalErrorInArgument,
		      (G4String("ARGUMENT: ") + argv[ii]).c_str());
	}
	ii += iAddPar;
      }
    }
  }
  
  bHistos = G4bool(theParamMgr->GetNumericValue("bHistos",0));
  theLineListFN = theParamMgr->GetStringValue("fLineList","");
  theLineStep = theParamMgr->GetNumericValue("lineStep",-DBL_MAX);
}

//---------------------------------------------------------------------------
void DCMPrintValueAlongLine::CheckArguments()
{
  //----- CHECK CORRECT ARGUMENTS
  if( theAlongLineInput == 0 ) {
    DicomException("NO INPUT LINE: -fLineList  or -valueAlongLine");
  }
  
  //--- THERE MUST BE AT LEAST ONE IMAGE FILE
  G4int nFiles = theParamMgr->GetNofImageFiles();
  if( nFiles == 0 ) {
    DicomException("NO IMAGE FILE GIVEN");
  }

}

//---------------------------------------------------------------------------
void DCMPrintValueAlongLine::ReadFilesAndGetImages()
{
  //--- READ AND BUILD IMAGES
  theReaderMgr->ProcessData();

  theImages = theReaderMgr->GetImages();

  theLineSet = new DicomLineSet("DCMPrintValueAlongLine",DPOrientNone);
  if( theAlongLineInput == 1 ) {
    DicomLineList* lineList1 = new DicomLineList("inputLine",0,DPOrientNone);
    theLineSet->AddLineList(lineList1);
    DicomLine* line1 = new DicomLine(theLinePos,theLineDir,"valueInLine",DPOrientNone);
    //    G4cout << "1 BUILD DicomLine " << theLinePos << " " << theLineDir << " valueInLine " << G4endl; //GDEB
    line1->SetPos0( theLinePos0 );
    lineList1->AddLine(line1);
  }
  
  if( theAlongLineInput == 2 ) {
    DicomLineList* lineList = new DicomLineList("fileLines",0,DPOrientNone);
    theLineSet->AddLineList(lineList);
    
    GmFileIn fin = GmFileIn::GetInstance(theLineListFN,true);
    std::vector<G4String> wl;
    for( ;; ) {
      if( !fin.GetWordsInLine(wl) ) break;
      if( wl[0] != ":LINE" ) {
	G4Exception(theExeName,
		    "DCMPrintValueAlongLine",
		    FatalException,
		    (G4String("First word in file has to be ':LINE', while it is "+wl[0])).c_str());
      }
      if( wl.size() != 7 &&  wl.size() != 8 &&  wl.size() != 10 &&  wl.size() != 11 ) {
	for(size_t iw = 0; iw < wl.size(); iw++ ) {
	  G4cerr << " WORD " << iw << " : " << wl[iw] << G4endl;
	}
	G4Exception(theExeName,
		    "",
		    FatalException,
		    (G4String("Number of words in line has to be 7,8,10 or 11, while it is"+GmGenUtils::itoa(wl.size()))).c_str());
      }
      G4ThreeVector pos( GmGenUtils::GetValue(wl[1]), GmGenUtils::GetValue(wl[2]), GmGenUtils::GetValue(wl[3]));
      G4ThreeVector dir( GmGenUtils::GetValue(wl[4]), GmGenUtils::GetValue(wl[5]), GmGenUtils::GetValue(wl[6]));
      G4ThreeVector pos0 = pos;
      G4String lineName = "";
      if( wl.size() == 8 ) {
	lineName = wl[7];
      }
      if( lineName == "" ) lineName = "valueInLine";
      if( wl.size() >= 10) {
	pos0 = G4ThreeVector( GmGenUtils::GetValue(wl[7]), GmGenUtils::GetValue(wl[8]), GmGenUtils::GetValue(wl[9]));
      }
      if( wl.size() == 11 ) {
	lineName = wl[10];
      }
      DicomLine* line = new DicomLine(pos,dir,lineName,DPOrientNone);
      line->SetPos0( pos0 );
      lineList->AddLine(line);
      //      G4cout << "2 BUILD DicomLine " << pos << " " << dir << " " << lineName << G4endl; //GDEB
    }
  }
}

//---------------------------------------------------------------------------
void DCMPrintValueAlongLine::GetInfoFromImages()
{
  for( size_t iimg = 0; iimg < theImages.size(); iimg++ ) {
    DicomVImage* image = theImages[iimg];
    G4String imageName = image->GetName();
    if( theImages.size() != 1 ) imageName += GmGenUtils::itoa(iimg); // figures of the same modality have the same name
    if( theImages.size() == 0 ) {
      imageName = "";
    }
    std::vector<DicomVLineList*> lineLists = theLineSet->GetLineLists();
    for( size_t ill = 0; ill < lineLists.size(); ill++ ) {
      std::vector<DicomVLine*> lines = lineLists[ill]->GetLines();
      for( size_t il = 0; il < lines.size(); il++ ) {
	DicomLine* line = static_cast<DicomLine*>(lines[il]);
	std::map<G4double,G4double> intersValues = line->FindValues(image, theLineStep); 
	G4double distBefore0;
	G4double distToOut = DBL_MAX;
	
	//----- Histogram name: build chaining dir and pos coordinates
	G4ThreeVector dir = line->GetDirection(0);
	G4ThreeVector pos = line->GetPoint(0);
	G4ThreeVector pos0 = line->GetPos0();
	
	G4String lineName = imageName;
	lineName += "_valueAL_";
	lineName += line->GetName();
	/*	for( size_t ii=0; ii < 3; ii++ ){
	  lineName += GmGenUtils::ftoa(dir[ii])+"_";
	}
	for( size_t ii=0; ii < 3; ii++ ){
	  lineName += GmGenUtils::ftoa(pos[ii])+"_";
	  } */
	
	//----- create histogram
	TH1F* histo;
	TCanvas* canvas;
	std::map<G4double,G4double>::const_iterator iteiv;
	if( bHistos ) {
	//--- Get minimum and maximum XY for histogram
	  G4double xMin = DBL_MAX;
	  G4double xMax = -DBL_MAX;
	  G4double yMax = -DBL_MAX;
	  for( iteiv = intersValues.begin(); iteiv != intersValues.end(); iteiv++ ) {
	    G4double distHis =(*iteiv).first;
	    G4double value =(*iteiv).second;
	    xMin = min(xMin,distHis);
	    xMax = max(xMax,distHis);
	    yMax = max(yMax,value);
	  }
	    
	  G4double dist;
	  std::vector<G4double> fMinV = image->GetMinV();
	  std::vector<G4double> fMaxV = image->GetMaxV();
	  for( size_t ii=0; ii < 3; ii++ ) {
	    dist = DBL_MAX;
	    if( dir[ii] >= 0 ) {
	      if( fabs(dir[ii]) > image->GetPrecision() ) dist = (-pos[ii] + fMaxV[ii]) / dir[ii];
	      if( DicomVerb(infoVerb) ) G4cout << "@@@@ DCMPrintValueAlongLine distToOut POS_DIR " << dist << G4endl;
	    } else {
	      if( fabs(dir[ii]) > image->GetPrecision() ) dist = (-pos[ii] + fMinV[ii]) / dir[ii];
	      if( DicomVerb(infoVerb) ) G4cout << "@@@@ DCMPrintValueAlongLine distToOut NEG_DIR " << dist << G4endl;
	    }
	    distToOut = min(dist,distToOut);
	  }
	  distBefore0 = (pos+dir*distToOut-pos0).mag();
	  if( DicomVerb(infoVerb) ) G4cout << "@@@@ DCMPrintValueAlongLine HISTO " << distBefore0 << " == " << pos << " + " << dir << " * " << distToOut << " - " << pos0 << " " << (pos+dir*distToOut-pos0) << distBefore0+distToOut << G4endl;
	  	  if( DicomVerb(infoVerb) ) G4cout << "@@@@ DCMPrintValueAlongLine HISTO " << distBefore0-distToOut << " TO " << distBefore0 << G4endl;
	  histo = new TH1F(lineName.c_str(), lineName.c_str(), 10000, distBefore0-distToOut,distBefore0);
	  canvas = new TCanvas;
	  canvas->Draw();
	  histo->SetMaximum(yMax*1.1);
	  histo->Draw();
	}

	//----- Write to file intersections-values
	G4String valueAlongLineFN = lineName+".val";
	std::ofstream fout(valueAlongLineFN.c_str());
	if( DicomVerb(infoVerb) ) G4cout << " ALONG LINE CREATING FILE " << lineName+".val" << std::endl; 
	fout << "DATA: " << lineName << G4endl;
	if( DicomVerb(infoVerb) ) G4cout << "@@@@ DCMPrintValueAlongLine n intersections " << intersValues.size() << G4endl;
	G4double xvalold, yvalold;
	for( iteiv = intersValues.begin(); iteiv != intersValues.end(); iteiv++ ) {
	  G4double distHis =(*iteiv).first;
	  G4double value =(*iteiv).second;
	  fout << distHis << " " << value << G4endl;
	  if( bHistos ) {
	    //	    G4cout << " DISTHIS " << distHis << " " << value << " BIN " << int((distHis-histo->GetXaxis()->GetXmin())/histo->GetBinWidth(2)) << " " << distHis << " - " << histo->GetXaxis()->GetXmin() << " / " << histo->GetBinWidth(2) << G4endl; //GDEB
	    //	    histo->Draw();
	    TMarker* mark = new TMarker(distHis, value,21);
	    mark->SetMarkerSize(0.03);
	    if( iteiv != intersValues.begin() && value != 0. ) {
	      TLine* lineDraw = new TLine(xvalold,yvalold,distHis,value);
	      //      G4cout << "XVAL " << xvalold << " " << yvalold << " " << xval << " " << yval << G4endl; //GDEB
	      lineDraw->Draw();  
	    }
	    mark->Draw();
	    if( distHis != 0. ) {
	      xvalold = distHis;
	      yvalold = value;      
	    }
	    mark->Draw();
	    //	    histo->Fill( distHis, value ); // fill value at center of bin (accumulated dist + 0.5*voxel_dist)
	  }
	}
	fout.close();

	//--- Draw Histos
	if( bHistos ) {
	  //	  canvas->Print((lineName+"."+theImageFormat).c_str());
	  canvas->Print((lineName+".gif").c_str());
	  TFile* foutR = new TFile(("his"+lineName+".root").c_str(),"RECREATE");
	  foutR->cd();
	  histo->Write();
	  foutR->Close();

	  /* histo->Draw();
	     TH1F* histo2 = new TH1F(lineName.c_str(), lineName.c_str(), 10000, distBefore0-distToOut,distBefore0);
	  histo2->SetMaximum(histo->GetMaximum());
	  canvas->cd();
	  histo2->Draw();
	  TAxis* axis = histo->GetXaxis();
	  float xvalold = 0.;
	  float yvalold = 0.;
	  for( int ii = 0; ii < axis->GetNbins(); ii++ ) {
	    float xval = axis->GetBinCenter(ii);
	    float yval = histo->GetBinContent(ii);
	    TMarker* mark = new TMarker(xval,yval,21);
	    mark->SetMarkerSize(0.03);
	    if( ii != 0 && yval != 0. ) {
	      TLine* lineDraw = new TLine(xvalold,yvalold,xval,yval);
	      //      G4cout << "XVAL " << xvalold << " " << yvalold << " " << xval << " " << yval << G4endl; //GDEB
	      lineDraw->Draw();  
	    }
	    mark->Draw();
	    if( yval != 0. ) {
	      xvalold = xval;
	      yvalold = yval;      
	    }
	  }
	  //	  canvas->Print((lineName+"_ML."+theImageFormat).c_str());
	  canvas->Print((lineName+"_ML.gif").c_str());
	  */

	}
      }
    }
    
  }
}

//-------------------------------------------------------------------
void DCMPrintValueAlongLine::PrintHelp()
{
  ((DicomParameterMgr*)(DicomParameterMgr::GetInstance()))->PrintHelpInputFiles();
  G4cout << " -doseAlongLine POSX POSY POSZ DIRX DIRY DIRZ   calculate values along this line " << G4endl
	 << " -zeroOfAlongLine POS0X POS0Y POS0Z   Define the '0' of a line so that distances are given relative to it " << G4endl
	 << " -fLineList     write a file list of lines " << G4endl
    //	 << " -bHistos    " << G4endl
	 << " -lineStep VALUE    calculate line values at intervals (if not provided calculate it at voxel intersections " << G4endl;
}

//---------------------------------------------------------------------------
void DCMPrintValueAlongLine::DicomException(G4String msg, G4ExceptionSeverity sever )
{
  if( sever == FatalException || sever == FatalErrorInArgument ) {
    PrintHelp();
  }
  G4Exception(theExeName.c_str(),
	      "",
	      sever,
	      msg.c_str());
}
  

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4Geantino.hh"
#include "G4VPhysicalVolume.hh"

#ifndef GAMOS_NO_ROOT
#include "TMarker.h"
#include "TStyle.h"
#include "TText.h"
#include "TLine.h"
#endif

#include "GmGeneratorScanVisUA.hh"
#include "GmGeneratorScanVis.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

//------------------------------------------------------------------------
GmGeneratorScanVisUA::GmGeneratorScanVisUA()
{
  theFileOut = 0;
}


//------------------------------------------------------------------------
GmGeneratorScanVisUA::~GmGeneratorScanVisUA()
{
}

//------------------------------------------------------------------------
void GmGeneratorScanVisUA::UserSteppingAction(const G4Step* aStep)
{
#ifndef GAMOS_NO_ROOT
  if( aStep->GetPostStepPoint()->GetPhysicalVolume() ) {
    G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();
    G4String borderName;

    if( bColorByVolumeIndex ) {
      G4VPhysicalVolume* prePV = aStep->GetPreStepPoint()->GetPhysicalVolume();
      G4VPhysicalVolume* postPV = aStep->GetPostStepPoint()->GetPhysicalVolume();
      if( theColorByVolumeIndexType == CBVIT_LV ) {
	borderName = prePV->GetLogicalVolume()->GetName();
	  + ".-." +  postPV->GetLogicalVolume()->GetName();
      } else if( theColorByVolumeIndexType == CBVIT_PV ) {
	borderName = prePV->GetLogicalVolume()->GetName()
	  + ":" + GmGenUtils::itoa(prePV->GetCopyNo())
	  + ".-." +  postPV->GetLogicalVolume()->GetName()
	  + ":" + GmGenUtils::itoa(postPV->GetCopyNo());
      } else if( theColorByVolumeIndexType == CBVIT_T ) {
	borderName = theGeomUtils->GetG4TouchableLongName((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))
	  + ".-." + theGeomUtils->GetG4TouchableLongName((G4TouchableHistory*)(aStep->GetPostStepPoint()->GetTouchable()));
      }
    } else {
      G4String vol1Name = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
      G4String vol2Name = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
      if( vol1Name > vol2Name ) {
	borderName = vol2Name + ".-." + vol1Name;
      } else {
	borderName = vol1Name + ".-." + vol2Name;
      }
    }
      //-    G4cout << " BORDER NAME " << borderName << " " <<  (vol1Name > vol2Name) << G4endl; //GDEB
    switch (thePlaneFig) {
    case PF_XY:
      if( pos.x() >= theDrawMinH && pos.x() <= theDrawMaxH && pos.y() >= theDrawMinV && pos.y() <= theDrawMaxV ) {
	thePoints[borderName].push_back(G4ThreeVector(pos.x(),pos.y(),0.));
      }
      break;
    case PF_XZ:
      if( pos.x() >= theDrawMinH && pos.x() <= theDrawMaxH && pos.z() >= theDrawMinV && pos.z() <= theDrawMaxV ) {
	thePoints[borderName].push_back(G4ThreeVector(pos.x(),pos.z(),0.));
      }
      break;
    case PF_YZ:
      //      G4cout << " CHECK POS YZ " << pos.y() << " >= " << theDrawMinV << " && " << pos.y() << " <= " << theDrawMaxV <<" && " << pos.z()<< " >= " << theDrawMinZ << "&& " << pos.z() << " <= " << theDrawMaxZ <<G4endl;//GDEB
      if( pos.y() >= theDrawMinH && pos.y() <= theDrawMaxH && pos.z() >= theDrawMinV && pos.z() <= theDrawMaxV ) {
	thePoints[borderName].push_back(G4ThreeVector(pos.y(),pos.z(),0.));
      }
      break;
    }
    //    G4cout << "UserSteppingAction ADD POINT " << thePoints.size() << G4endl; //GDEB

    ColPoint pt;
    pt.x  = pos.x();
    pt.y  = pos.y();
    pt.z  = pos.z();
    strncpy(pt.tag, aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().c_str(), 64);

    if( theFileOut ) {
      fwrite(&pt, sizeof(ColPoint), 1, theFileOut);
    }
    
    theLastPos = pos; // to know in which plane it is 
  }
#endif
}


#ifndef GAMOS_NO_ROOT

//------------------------------------------------------------------------
void GmGeneratorScanVisUA::BeginOfRunAction(const G4Run* )
{
  theGenerator->Initialise();
  
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();

  std::vector<G4String> params;
  params = parmgr->GetVStringValue("GmGeneratorScanVis:ScanParams",params);
  if( params.size() == 0 ) {
    G4Exception("GmGeneratorScanVisUA",
		"",
		FatalException,
		"No command found: /gamos/setParam GmGeneratorScanVisUA::ScanParams XY/XZ/YZ STEP_FIGURE STEP_SCAN ANGLES ");
  }
  if( params.size() < 4 ) {
    G4Exception("GmGeneratorScanVisUA",
		"",
		FatalException,
		("Number of parameters has to be at least 4, while they are " + GmGenUtils::itoa(params.size())).c_str());
  }

  theStepFig = GmGenUtils::GetValue(params[1]);
  if( theStepFig == 0 ) {
    G4Exception("GmGeneratorScanVisUA",
		"",
		FatalException,
		"Second argument (Step of figures) in /gamos/setParam GmGeneratorScanVisUA::ScanParams  cannot be 0");
  }
  if( params[0] == "XY" ) {
    thePlaneFig = PF_XY;
    thePlaneFigName = "XY";
  } else if( params[0] == "XZ" ) {
    thePlaneFig = PF_XZ;
    thePlaneFigName = "XZ";
  } else if( params[0] == "YZ" ) {
    thePlaneFig = PF_YZ;
    thePlaneFigName = "YZ";
  } else {
        G4Exception("GmGeneratorScanVisUA",
		"",
		    FatalErrorInArgument,
		    ("First argument has to be XY, XZ or YZ, while it is " + params[0]).c_str());
  }

  G4String linesFN = parmgr->GetStringValue("GmGeneratorScanVis:LinesFileName","");
  if( linesFN != "" ) ReadLinesFile(linesFN);

  bPrintAllTogether = G4bool(parmgr->GetNumericValue("GmGeneratorScanVis:bPrintAllTogether", 0));


  SetColors();
   
  theGeomUtils = GmGeometryUtils::GetInstance();
  G4LogicalVolume* worldLV = theGeomUtils->GetTopLV();
  G4Box* world = static_cast<G4Box*>(worldLV->GetSolid());
  if( !world ) {
      G4Exception("GmGeneratorScanVisUA::BeginOfRunAction",
		  "",
		  FatalException,
		  (G4String("World has to be of type  Box, while it is ") + worldLV->GetSolid()->GetEntityType()).c_str());
  }
      
  theScanWorldMinX = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMinX", -world->GetXHalfLength());
  theScanWorldMaxX = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMaxX", world->GetXHalfLength());
  theScanWorldMinY = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMinY", -world->GetYHalfLength());
  theScanWorldMaxY = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMaxY", world->GetYHalfLength());
  theScanWorldMinZ = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMinZ", -world->GetZHalfLength());
  theScanWorldMaxZ = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMaxZ", world->GetZHalfLength());
  
  theCanvas = new TCanvas;
  gStyle->SetOptStat(0);
  theCanvasNPixels = G4int(parmgr->GetNumericValue("GmGeneratorScanVis:NPixels", 500));
  theCanvas->SetCanvasSize(theCanvasNPixels,theCanvasNPixels);

  theMarkerStyle = G4int(parmgr->GetNumericValue("GmGeneratorScanVis:MarkerStyle", 1));
  theMarkerSize = parmgr->GetNumericValue("GmGeneratorScanVis:MarkerSize", 0.25);
  if( theMarkerSize < 0.25 ) {
    G4Exception("GmGeneratorScanVisUA::EndOfEventAction",
		"",
		JustWarning,
		"Too small marker size, it will be reset to 0.25. You may increase the NPixels to increase precision");
    theMarkerSize = 0.25;
  }

  switch (thePlaneFig) {
  case PF_XY:
    theHisto = new TH2F("","",100,theScanWorldMinX,theScanWorldMaxX,100,theScanWorldMinY,theScanWorldMaxY);
    theWorldNDigits = log10(fabs(std::max(theScanWorldMinZ,theScanWorldMaxZ)));
    theDrawMinH = theScanWorldMinX;
    theDrawMaxH = theScanWorldMaxX;
    theDrawMinV = theScanWorldMinY;
    theDrawMaxV = theScanWorldMaxY;
    break;
  case PF_XZ:
    theHisto = new TH2F("","",100,theScanWorldMinX,theScanWorldMaxX,100,theScanWorldMinZ,theScanWorldMaxZ);
    theWorldNDigits = log10(fabs(std::max(theScanWorldMinY,theScanWorldMaxY)));
    theDrawMinH = theScanWorldMinX;
    theDrawMaxH = theScanWorldMaxX;
    theDrawMinV = theScanWorldMinZ;
    theDrawMaxV = theScanWorldMaxZ;
    break;
  case PF_YZ:
    theHisto = new TH2F("","",100,theScanWorldMinY,theScanWorldMaxY,100,theScanWorldMinZ,theScanWorldMaxZ);
    theWorldNDigits = log10(fabs(std::max(theScanWorldMinX,theScanWorldMaxX)));
    theDrawMinH = theScanWorldMinY;
    theDrawMaxH = theScanWorldMaxY;
    theDrawMinV = theScanWorldMinZ;
    theDrawMaxV = theScanWorldMaxZ;
    break;
  }
  //  G4cout << " INITIAL thePosTransv " << thePosTransv << " " << theStepFig << G4endl; //GDEB
  
  G4int nd = log10(theStepFig);
  if( nd > 0 ) nd = 0;
  if( nd < 0 ) nd--; //take into account "."
  theNameNDigits = theWorldNDigits - nd;

  G4String fOutName = parmgr->GetStringValue("GmGeneratorScanVis:FileOutName","scanVis.out");
  if( fOutName != "" ) {
    theFileOut = fopen(fOutName.c_str(), "wb");
  }

  bColorByVolumeIndex = false;
  theColorByVolumeIndexFN = parmgr->GetStringValue("GmGeneratorScanVis:ColorByVolumeIndexFileName","");
  if( theColorByVolumeIndexFN != "" ) {
    bColorByVolumeIndex = true;

    G4String colorByVolumeIndexTypeStr = parmgr->GetStringValue("GmGeneratorScanVis:ColorByVolumeIndexType","LogicalVolume");
    if( colorByVolumeIndexTypeStr == "LogicalVolume " ) {
      theColorByVolumeIndexType = CBVIT_LV;
    } else if( colorByVolumeIndexTypeStr == "PhysicalVolume " ) {
      theColorByVolumeIndexType = CBVIT_PV;
    } else if( colorByVolumeIndexTypeStr == "Touchable " ) {
      theColorByVolumeIndexType = CBVIT_T;
    } else {
      G4Exception("GmGeneratorScanVisUA::GmGeneratorScanVisUA",
		  "",
		  FatalException,
		  ("Parameter GmGeneratorScanVisUA:ColorByVolumeIndexType must be: LogicalVolume, PhysicalVolume or Touchable, while it is: " + colorByVolumeIndexTypeStr).c_str());
    }

    std::vector<G4String> wl;
    GmFileIn fin = GmFileIn::GetInstance(theColorByVolumeIndexFN);
    for(;;) {
      if( ! fin.GetWordsInLine(wl) ) break;
      if( wl.size() < 2 ) {
	G4Exception("GmGeneratorScanVisUA::GmGeneratorScanVisUA",
		    "",
		    FatalException,
		    ("Error reading file " + theColorByVolumeIndexFN + " lines must have two columns ").c_str());
      }
      theColorVolumeIdx[wl[0]] = GmGenUtils::GetInt(wl[1]);
    }
  }

  bDrawBorderNames = G4bool(parmgr->GetNumericValue("GmGeneratorScanVis:DrawBorderNames",0));

  thePrecision = std::min( std::min(theScanWorldMaxX-theScanWorldMinX,theScanWorldMaxY-theScanWorldMinY),theScanWorldMaxZ-theScanWorldMinZ)*1e-4;

}

//------------------------------------------------------------------------
void GmGeneratorScanVisUA::EndOfRunAction(const G4Run* )
{
  if( theFileOut ) {
    fclose(theFileOut);
  }
}

//------------------------------------------------------------------------
void GmGeneratorScanVisUA::EndOfEventAction(const G4Event* event)
{
  G4double posTransv = theGenerator->GetPosTransv();
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(warningVerb) ) G4cout << "EndOfEventAction " << posTransv << " : " << thePoints.size() << G4endl; 
#endif

  if( thePoints.size() == 0 ) {
    G4Exception("GmGeneratorScanVisUA",
		"GSV001",
		JustWarning,
		"No intersection point found, please check your dimensions");
    return;
  }
  
  //----- DRAW FIGURE
  if( !bPrintAllTogether || event->GetEventID() == 0 ) {
    theCanvas->Draw();
    theHisto->Draw();
  }
  /*  G4double isoc = 300;
  TLine* line = new TLine(0.,0., 0., isoc);
  line->Draw();
  TLine* line2 = new TLine(0.,0., 25, isoc);
  line2->Draw();
  TLine* line3 = new TLine(0.,0., -25, isoc);
  line3->Draw(); */
  std::map<G4String, std::vector<G4ThreeVector> >::const_iterator itep;
  G4int icol = 0;
  std::map<G4String,EColor> colorMapUsed;
  
  for( itep = thePoints.begin(); itep != thePoints.end(); itep++, icol++ ) {
    G4String borderName = (*itep).first;
    EColor color = kBlack;
    if( bColorByVolumeIndex ) {
      /*      std::map<G4String,EColor>::const_iterator itec = theColorMap.find(borderName);
      if( itec == theColorMap.end() ) {
	theColorMap[borderName] = GetColor( theColorMap.size() );
	itec = theColorMap.find(borderName);
      }
      color = (*itec).second;
      */
    } else {
      color = GetColor(icol);
    } 
    colorMapUsed[borderName] = color;
    std::vector<G4ThreeVector> points = (*itep).second;
    
    for( size_t ii = 0; ii < points.size(); ii++ ) {
      TMarker* mark = new TMarker(points[ii].x(),points[ii].y(),theMarkerStyle);
      mark->SetMarkerSize(theMarkerSize);
      mark->SetMarkerColor(color);
      mark->Draw();
      //  G4cout << " DRAW MARKER " <<mark->GetX() << " " << mark->GetY() << G4endl;//GDEB 
      //    delete mark;
    }
  }
  //---- Draw border names
  if( bDrawBorderNames ) {
    double XPos = theDrawMinH+(theDrawMaxH-theDrawMinH)*0.05;
    double YPos = theDrawMaxV-(theDrawMaxV-theDrawMinV)*0.02;
    //G4cout << " 0TEXT POS  " << XPos << "," << YPos << " " << theDrawMinH << " " << theDrawMaxV << G4endl; //GDEB
    UInt_t tx = 1;
    UInt_t ty = 1;
    UInt_t txPrev = 0;
    UInt_t tyPrev = 0;
    std::map<G4String,EColor>::const_iterator itec;
    for( itec = colorMapUsed.begin(); itec != colorMapUsed.end(); itec++ ) {
      XPos += (txPrev/0.75)*(theDrawMaxH-theDrawMinH)/theCanvasNPixels;
      YPos += 0.;
      TText* text = new TText( XPos, YPos, (*itec).first );
      text->SetTextColor((*itec).second);
      text->SetTextSize(0.015);
      text->GetTextExtent(tx,ty,(*itec).first);
      double XPosEnd = XPos + (tx/0.75)*(theDrawMaxH-theDrawMinH)/theCanvasNPixels;
      if( XPosEnd > theDrawMaxH ) {
	XPos = theDrawMinH+(theDrawMaxH-theDrawMinH)*0.05;
	YPos -= (ty/0.8)*(theDrawMaxV-theDrawMinV)/theCanvasNPixels;
	//      G4cout << XPosEnd << " TEXT POS CORR " << XPos << "," << YPos << " " << tx << " * " << theCanvasNPixels << " / ( " << theDrawMaxV << " - " << theDrawMinV<< " " << YPos << G4endl; //GDEB
      }
      text->SetX(XPos);
      text->SetY(YPos);
      //    G4cout << " TEXT POS  " << XPos << "," << YPos << " " << tx << " * " << theCanvasNPixels << " / ( " << theDrawMaxV << " - " << theDrawMinV<< " " << YPos << G4endl; //GDEB
      
      text->Draw();
      text->GetTextExtent(txPrev,tyPrev,(*itec).first.c_str());
      //    G4cout << " TEXT EXTENT " << (*itec).first << " " << tx << " " << ty << " : " << (*itec).first.length() << G4endl;//GDEB
    }
  }
  
  G4String posTransvStr = GmGenUtils::ftoa(posTransv);
  G4int posNDigits = log10(fabs(posTransv));
  if( posTransv == 0 ) posNDigits = 1;
  if( posNDigits < 0 ) posNDigits = 0;
  //  G4cout << " posNDigits " << posNDigits << " " << theWorldNDigits << G4endl; //GDEB
  for( int ii = posNDigits; ii < theWorldNDigits; ii++ ) {
    if( posTransv > 0. ) {
      posTransvStr = "0" + posTransvStr;
    } else {
      posTransvStr = "-0" + posTransvStr.substr(1,posTransvStr.length());
    }
  }
  int nDigits = theNameNDigits - int((GmGenUtils::sign(posTransv)-1)/2) +1;
  //  G4cout << "NDIG " << nDigits << " = " << theNameNDigits << " - " << int(GmGenUtils::sign(posTransv)-1)/2 << " LENGTH " << posTransvStr.length() << G4endl; //GDEB

  if( int(posTransvStr.length()) < nDigits ) {
    if( posTransvStr.find(".") == std::string::npos ) posTransvStr += ".";
    G4int nptd = posTransvStr.length();
    for( int ii = 0; ii < nDigits-nptd; ii++ ) {
      posTransvStr += "0";
    }
  }
  //  G4cout << " theWorldNDigits " << theWorldNDigits << " " << posNDigits << " : " << posTransvStr << G4endl;//GDEB

  // DRAW EXTRA LINES FROM FILE
  theCanvas->cd();
  switch(thePlaneFig) {
  case PF_XY:
    for( size_t ii = 0; ii < theLinesXY.size(); ii++ ) {
      pvv line = theLinesXY[ii];
      if( fabs(theLastPos.z()-line.first.z())< thePrecision ) {
	TLine* tline = new TLine(line.first.x(),line.first.y(),line.second.x(),line.second.y());
	tline->Draw();
      }
    }
    break;
  case PF_XZ:
    for( size_t ii = 0; ii < theLinesXZ.size(); ii++ ) {
      pvv line = theLinesXZ[ii];
      //      G4cout << " GmGeneratorScanVisUA USE XZ " << line.first << " " << line.second << " " << theLastPos.y()-line.first.y() << " <? " << thePrecision << G4endl; //GDEB
      if( fabs(theLastPos.y()-line.first.y())< thePrecision ) {
	TLine* tline = new TLine(line.first.x(),line.first.z(),line.second.x(),line.second.z());
	//	G4cout << " DRAW LINE XZ " <<  tline->GetX1() << "," << tline->GetY1() << " " << tline->GetX2() << "," << tline->GetY2() << G4endl; //GDEB
	//	tline->SetLine
	tline->Draw();
      }
    }
    break;
  case PF_YZ:
    for( size_t ii = 0; ii < theLinesYZ.size(); ii++ ) {
      pvv line = theLinesYZ[ii];
      if( fabs(theLastPos.x()-line.first.x())< thePrecision ) {
	TLine* tline = new TLine(line.first.y(),line.first.z(),line.second.y(),line.second.z());
	tline->Draw();
      }
    }
    break;
  }
  
  G4String figName = "ScanVis"+thePlaneFigName+"."+posTransvStr+".gif";
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " EndOfEventAction " << figName << G4endl;
#endif
  if( !bPrintAllTogether || event->GetEventID() == G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed()-1 ) {
    theCanvas->Print(figName.c_str());
  }
  
  thePoints.clear();
}
  
//--------------------------------------------------------------------------
void GmGeneratorScanVisUA::SetColors()
{
  theGoodColors[0] = EColor(kBlack+0);
  theGoodColors[1] = EColor(kBlack+1);
  theGoodColors[2] = EColor(kBlack+2);
  theGoodColors[3] = EColor(kBlack+3);
  theGoodColors[4] = EColor(kBlack+5);
  theGoodColors[5] = EColor(kBlack+6);
  theGoodColors[6] = EColor(kBlack+7);
  theGoodColors[7] = EColor(kBlack+8);
  theGoodColors[8] = EColor(kBlack+10);
  theGoodColors[9] = EColor(kBlack+11);
  theGoodColors[10] = EColor(kBlack+43);
  theGoodColors[11] = EColor(kBlack+19);
  theGoodColors[12] = EColor(kBlack+27);
  theGoodColors[13] = EColor(kBlack+28);
  theGoodColors[14] = EColor(kBlack+31);
  theGoodColors[15] = EColor(kBlack+15);
  theGoodColors[16] = EColor(kBlack+34);
  theGoodColors[17] = EColor(kBlack+40);
  theGoodColors[18] = EColor(kBlack+37);
  theGoodColors[19] = EColor(kBlack+23);
  theGoodColors[20] = EColor(kBlack+41);
  theGoodColors[21] = EColor(kBlack+32);
  theGoodColors[22] = EColor(kBlack+45);
  theGoodColors[23] = EColor(kBlack+29);
  theGoodColors[24] = EColor(kBlack+38);
  theGoodColors[25] = EColor(kBlack+47);
}

//--------------------------------------------------------------------------
EColor GmGeneratorScanVisUA::GetColor( int icol )
{
  icol = icol%25;

  return theGoodColors[icol];
}


//--------------------------------------------------------------------------
void GmGeneratorScanVisUA::ReadLinesFile(G4String linesFN)
{
  GmFileIn fin = GmFileIn::GetInstance(linesFN);
  std::vector<G4String> wl;
  for(;;) {
    if( ! fin.GetWordsInLine(wl) ) break;
    if( wl.size() != 7 ) {
      G4Exception("GmGeneratorScanVis::ReadLinesFile","",
		  FatalException,
		  "Line does not have 7 lines: 'XY/XZ/YZ PT1_x PT1_y PT1_z PT2_x PT2_y PT2_z");
    }
    pvv line = pvv(G4ThreeVector(GmGenUtils::GetValue(wl[1]),GmGenUtils::GetValue(wl[2]),GmGenUtils::GetValue(wl[3])),
		  G4ThreeVector(GmGenUtils::GetValue(wl[4]),GmGenUtils::GetValue(wl[5]),GmGenUtils::GetValue(wl[6]))) ;

    if( wl[0] == "XY" ) {
      theLinesXY.push_back(line);
    } else if( wl[0] == "XZ" ) {
      //      G4cout << " GmGeneratorScanVisUA add line XZ " << line.first << " " << line.second << G4endl; //GDEB
      theLinesXZ.push_back(line);
    } else if( wl[0] == "YZ" ) {
      theLinesYZ.push_back(line);
    }
  } 
}


#endif

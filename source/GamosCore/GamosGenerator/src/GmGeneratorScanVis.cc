#include "GmGeneratorScanVis.hh"
#include "GmGeneratorScanVisUA.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4RunManager.hh"
#include "G4Geantino.hh"
#include "G4Box.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "GamosCore/GamosBase/Base/include/GmFilterFactory.hh"
#endif

//------------------------------------------------------------------------
GmGeneratorScanVis::GmGeneratorScanVis()
{
#ifdef GAMOS_NO_ROOT
  G4Exception("GmGeneratorScanVis::GmGeneratorScanVis",
	      "",
	      FatalException,
	      "GmGeneratorScanVis cannot be used without ROOT");
#endif
  
  //--  bPrintAllTogether = G4bool(parmgr->GetNumericValue("GmGeneratorScanVis:bPrintAllTogether", 0));

  
  //----- Create user action that will draw the intersection points
#ifndef GAMOS_NO_ROOT
  theGeneratorUA = new GmGeneratorScanVisUA();
  theGeneratorUA->SetGenerator(this);
#endif  
}


//------------------------------------------------------------------------
GmGeneratorScanVis::~GmGeneratorScanVis()
{
}



//------------------------------------------------------------------------
void GmGeneratorScanVis::Initialise()
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();

  std::vector<G4String> params;
  params = parmgr->GetVStringValue("GmGeneratorScanVis:ScanParams",params);
  if( params.size() == 0 ) {
    G4Exception("GmGeneratorScanVis",
		"",
		FatalException,
		"No command found: /gamos/setParam GmGeneratorScanVis::ScanParams XY/XZ/YZ STEP_FIGURE STEP_SCAN ANGLES ");
  }
  if( params.size() < 4 ) {
    G4Exception("GmGeneratorScanVis",
		"",
		FatalException,
		("Number of parameters has to be at least 4, while they are " + GmGenUtils::itoa(params.size())).c_str());
  }

  theStepFig = GmGenUtils::GetValue(params[1]);
  if( theStepFig == 0 ) {
    G4Exception("GmGeneratorScanVis",
		"",
		FatalException,
		"Second argument (Step of figures) in /gamos/setParam GmGeneratorScanVis::ScanParams  cannot be 0");
  }
  theStepScan = GmGenUtils::GetValue(params[2]);
  for( size_t ii = 3; ii < params.size(); ii++ ) {
    theAngles.push_back(GmGenUtils::GetValue(params[ii]));
  }
  if( theStepScan == 0. ) {
    G4Exception("GmGeneratorScanVis",
		"",
		FatalException,
		"Third argument (Step of scanning) in /gamos/setParam GmGeneratorScanVis::ScanParams  cannot be 0");
  }
  if( params[0] == "XY" ) {
    thePlaneFig = PF_XY;
  } else if( params[0] == "XZ" ) {
    thePlaneFig = PF_XZ;
  } else if( params[0] == "YZ" ) {
    thePlaneFig = PF_YZ;
  } else {
        G4Exception("GmGeneratorScanVis",
		"",
		    FatalErrorInArgument,
		    ("First argument has to be XY, XZ or YZ, while it is " + params[0]).c_str());
  }


  //--- Set world dimensions
  G4LogicalVolume* worldLV = GmGeometryUtils::GetInstance()->GetTopLV();
  G4Box* world = static_cast<G4Box*>(worldLV->GetSolid());
  theScanWorldMinX = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMinX", -world->GetXHalfLength());
  theScanWorldMaxX = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMaxX", world->GetXHalfLength());
  theScanWorldMinY = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMinY", -world->GetYHalfLength());
  theScanWorldMaxY = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMaxY", world->GetYHalfLength());
  theScanWorldMinZ = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMinZ", -world->GetZHalfLength());
  theScanWorldMaxZ = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMaxZ", world->GetZHalfLength());

  //--- Set first transversal position
  switch (thePlaneFig) {
  case PF_XY:
    thePosTransv = theScanWorldMinZ;
    break;
  case PF_XZ:
    thePosTransv = theScanWorldMinY;
    break;
  case PF_YZ:
    thePosTransv = theScanWorldMinX;
    break;
  }
  thePosTransv -= theStepFig;

  //--- Set filter
  G4String filterName = parmgr->GetStringValue("GmGeneratorScanVis:Filter","");
  if( filterName != "" ) {
    /*    GmVFilter* filter = 0;
#ifdef ROOT5
    filter = Reflex::PluginService::Create<GmVFilter*>(filterName,filterName);
#else
    filter = GmFilterFactory::get()->create(filterName,filterName);
    #endif */ 
    theGeneratorUA->SetFilterOrClassifier(filterName);
  }
  
}

//------------------------------------------------------------------------
void GmGeneratorScanVis::GeneratePrimaries(G4Event* evt)
{
#ifndef GAMOS_NO_ROOT
  if( !IncrementPosTransv() ) return;
  
  //  G4cout << " thePlaneFig " << thePlaneFig << " theAngles " << theAngles.size() << G4endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) )   G4cout << "GmGeneratorScanVis::GeneratePrimaries  PosTransv " << thePosTransv << G4endl; //GDEB
#endif
  
  double sstep;
  if( thePlaneFig == PF_XY ) {
    for( size_t ii = 0; ii < theAngles.size(); ii++ ) {
      G4double angle = theAngles[ii];
      SetDir( angle );
      double cosAng = cos(angle);
      double sinAng = sin(angle);
      if( angle < 90*CLHEP::deg ) {
	//        G4cout << " ANGLE " << angle/CLHEP::deg << G4endl; //GDEB
	//----- SCAN ALONG Y AXIS AT NEGATIVE X
	sstep = fabs(theStepScan/cosAng);
	double posY = theScanWorldMinY + sstep;
	for( ; posY < theScanWorldMaxY; posY += sstep ) {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) G4cout << "PF_XY posY " << posY << " " <<G4ThreeVector(theScanWorldMinX, posY, thePosTransv) << G4endl; //GDEB
#endif
	  GenerateParticle( G4ThreeVector(theScanWorldMinX, posY, thePosTransv), evt ); 
	}
	if( angle != 0*CLHEP::deg ) {	
	  //----- SCAN ALONG X AXIS AT NEGATIVE Y
	  sstep = fabs(theStepScan/sinAng);
	  double posX = theScanWorldMinX + sstep;
	  for( ; posX < theScanWorldMaxX; posX += sstep ) {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) G4cout << "PF_XY posX " << posX << " " <<G4ThreeVector(posX, theScanWorldMinY, thePosTransv) << G4endl; //GDEB
#endif
	    GenerateParticle( G4ThreeVector(posX, theScanWorldMinY, thePosTransv), evt ); 
	  }
	}
      } else if( angle < 180*CLHEP::deg ) {
	//----- SCAN ALONG X AXIS AT NEGATIVE Y
	sstep = fabs(theStepScan/sinAng);
	double posX = theScanWorldMinX + sstep;
	//	G4cout << " 0posY " << posY << G4endl; //GDEB
	for( ; posX < theScanWorldMaxX; posX += sstep ) {
	  //	  G4cout << " posY " << posY << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(posX, theScanWorldMinY, thePosTransv), evt ); 
	}
	if( angle != 90*CLHEP::deg ) {	
	  //----- SCAN ALONG Y AXIS AT POSITIVE X
	  sstep = fabs(theStepScan/cosAng);
	  double posY = theScanWorldMinY - sstep;
	  for( ; posY < theScanWorldMaxY; posY -= sstep ) {
	    GenerateParticle( G4ThreeVector(theScanWorldMaxX, posY, thePosTransv), evt ); 
	  }
	}
      } else if( angle < 270*CLHEP::deg ) {
	//----- SCAN ALONG Y AXIS AT POSITIVE X
	sstep = fabs(theStepScan/cosAng);
	double posY = theScanWorldMinY + sstep;
	//	G4cout << " 0posY " << posY << G4endl; //GDEB
	for( ; posY < theScanWorldMaxY; posY += sstep ) {
	  //	  G4cout << " posY " << posY << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(theScanWorldMaxX, posY, thePosTransv), evt ); 
	}
	if( angle != 180*CLHEP::deg ) {	
	  //----- SCAN ALONG X AXIS AT POSITIVE Y
	  sstep = fabs(theStepScan/sinAng);
	  double posX = theScanWorldMinX + sstep;
	  sstep = fabs(theStepScan/sinAng);
	  for( ; posX < theScanWorldMaxX; posX += sstep ) {
	    //	    G4cout << " posX " << posX << G4endl; //GDEB
	    GenerateParticle( G4ThreeVector(posX, theScanWorldMaxY, thePosTransv), evt ); 
	  }
	}
      } else { // < 360
	//----- SCAN ALONG X AXIS AT POSITIVE Y
	sstep = fabs(theStepScan/sinAng);
	double posX = theScanWorldMinX + sstep;
	//	G4cout << " 0posY " << posY << G4endl; //GDEB
	for( ; posX < theScanWorldMaxX; posX += sstep ) {
	  //	  G4cout << " posY " << posY << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(posX, theScanWorldMaxY, thePosTransv), evt ); 
	}
	if( angle != 270*CLHEP::deg ) {	
	  //----- SCAN ALONG Y AXIS AT NEGATIVE X
	  sstep = fabs(theStepScan/cosAng);
	  double posY = theScanWorldMinY + sstep;
	  for( ; posY < theScanWorldMaxY; posY += sstep ) {
	    GenerateParticle( G4ThreeVector(theScanWorldMinX, posY, thePosTransv), evt ); 
	  }
	}
      }
    }
    //---------------------------------------    
  } else if( thePlaneFig == PF_XZ ) {
 #ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) G4cout << "PF_XZ NAngles " <<theAngles.size() << G4endl; //GDEB
#endif
   for( size_t ii = 0; ii < theAngles.size(); ii++ ) {
      G4double angle = theAngles[ii]; 
      //      G4cout << " ANGLE " << angle/CLHEP::deg << G4endl; //GDEB
      SetDir( angle );
 #ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << ii << " Angle " << angle << G4endl; //GDEB
#endif
      double cosAng = cos(angle);
      double sinAng = sin(angle);
      if( angle < 90*CLHEP::deg ) {
	//----- SCAN ALONG Z AXIS AT NEGATIVE X
	sstep = fabs(theStepScan/cosAng);
	double posZ = theScanWorldMinZ + sstep;
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(testVerb) ) G4cout << "PF_XZ 0posZ " << posZ << " theScanWorldMaxZ " << theScanWorldMaxZ << " sstep " << sstep << G4endl; //GDEB
#endif
	for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	  //	  G4cout << " 0posZ " << posZ << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(theScanWorldMinX, thePosTransv, posZ), evt ); 
	}
	if( angle != 0*CLHEP::deg ) {	
	  //----- SCAN ALONG X AXIS AT NEGATIVE Z
	  sstep = fabs(theStepScan/sinAng);
	  double posX = theScanWorldMinX + sstep;
	  for( ; posX < theScanWorldMaxX; posX += sstep ) {
	    GenerateParticle( G4ThreeVector(posX, thePosTransv, theScanWorldMinZ), evt ); 
	  }
	}
      } else if( angle < 180*CLHEP::deg ) {
	//----- SCAN ALONG X AXIS AT NEGATIVE Z
	sstep = fabs(theStepScan/sinAng);
	double posX = theScanWorldMinX + sstep;
	//	G4cout << " 1-90posX " << posX << " theScanWorldMaxX " << theScanWorldMaxX << " sstep " << sstep << G4endl; //GDEB
	for( ; posX < theScanWorldMaxX; posX += sstep ) {
	  //	  G4cout << " 90posZ " << theScanWorldMinZ << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(posX, thePosTransv, theScanWorldMinZ), evt ); 
	}
	if( angle != 90*CLHEP::deg ) {	
	  //----- SCAN ALONG Z AXIS AT POSITIVE X
	  sstep = fabs(theStepScan/cosAng);
	  double posZ = theScanWorldMinZ + sstep;
	  for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	    GenerateParticle( G4ThreeVector(theScanWorldMaxX, thePosTransv, posZ), evt ); 
	  }
	}
      } else if( angle < 270*CLHEP::deg ) {
	//----- SCAN ALONG Z AXIS AT POSITIVE X
	sstep = fabs(theStepScan/cosAng);
	double posZ = theScanWorldMinZ + sstep;
	//	G4cout << " 0posZ " << posZ << G4endl; //GDEB
	for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	  //	  G4cout << " posZ " << posZ << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(theScanWorldMaxX, thePosTransv, posZ), evt ); 
	}
	if( angle != 180*CLHEP::deg ) {	
	  //----- SCAN ALONG X AXIS AT POSITIVE Z
	  sstep = fabs(theStepScan/sinAng);
	  double posX = theScanWorldMinX + sstep;
	  for( ; posX < theScanWorldMaxX; posX += sstep ) {
	    GenerateParticle( G4ThreeVector(posX, thePosTransv, theScanWorldMaxZ), evt ); 
	  }
	}
      } else { // < 360
	//----- SCAN ALONG X AXIS AT POSITIVE Z
	sstep = fabs(theStepScan/sinAng);
	double posX = theScanWorldMinX + sstep;
	//	G4cout << " 0posZ " << posZ << G4endl; //GDEB
	for( ; posX < theScanWorldMaxX; posX += sstep ) {
	  //	  G4cout << " posZ " << posZ << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(posX, thePosTransv, theScanWorldMaxZ), evt ); 
	}
	if( angle != 270*CLHEP::deg ) {	
	  //----- SCAN ALONG Z AXIS AT NEGATIVE X
	  sstep = fabs(theStepScan/cosAng);
	  double posZ = theScanWorldMinZ + sstep;
	  for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	    GenerateParticle( G4ThreeVector(theScanWorldMinX, thePosTransv, posZ), evt ); 
	  }
	}
      }
    }
        //---------------------------------------    
  } else if( thePlaneFig == PF_YZ ) {
    for( size_t ii = 0; ii < theAngles.size(); ii++ ) {
      G4double angle = theAngles[ii];
      SetDir( angle );
      double cosAng = cos(angle);
      double sinAng = sin(angle);
      //      G4cout << " ANGLE " << angle/CLHEP::deg << G4endl; //GDEB
      if( angle < 90*CLHEP::deg ) {
	//----- SCAN ALONG Z AXIS AT NEGATIVE Y
	sstep = fabs(theStepScan/cosAng);
	double posZ = theScanWorldMinZ + sstep;
	//       	G4cout << " 0posZ " << posZ << G4endl; //GDEB
	for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	  //	  G4cout << " posZ " << posZ << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(thePosTransv, theScanWorldMinY, posZ), evt ); 
	}
	if( angle != 0*CLHEP::deg ) {	
	  //----- SCAN ALONG Y AXIS AT NEGATIVE Z
	  sstep = fabs(theStepScan/sinAng);
	  double posY = theScanWorldMinY + sstep;
	  for( ; posY < theScanWorldMaxY; posY += sstep ) {
	    GenerateParticle( G4ThreeVector(thePosTransv, posY, theScanWorldMinZ), evt ); 
	  }
	}
      } else if( angle < 180*CLHEP::deg ) {
	//----- SCAN ALONG Y AXIS AT NEGATIVE Z
	sstep = fabs(theStepScan/sinAng);
	double posY = theScanWorldMinY + sstep;
	//	G4cout << " 0posZ " << posZ << G4endl; //GDEB
	for( ; posY < theScanWorldMaxY; posY += sstep ) {
	  //	  G4cout << " posZ " << posZ << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(thePosTransv, posY, theScanWorldMinZ), evt ); 
	}
	if( angle != 90*CLHEP::deg ) {	
	  //----- SCAN ALONG Z AXIS AT POSITIVE Y
	  sstep = fabs(theStepScan/cosAng);
	  double posZ = theScanWorldMinZ + sstep;
	  for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	    GenerateParticle( G4ThreeVector(thePosTransv, theScanWorldMaxY, posZ), evt ); 
	  }
	}
      } else if( angle < 270*CLHEP::deg ) {
	//----- SCAN ALONG Z AXIS AT POSITIVE Y
	sstep = fabs(theStepScan/cosAng);
	double posZ = theScanWorldMinZ + sstep;
	//	G4cout << " 0posZ " << posZ << G4endl; //GDEB
	//	G4cout << "<270 ANGLE " << angle-180*CLHEP::deg << " " << sstep << " " << theStepScan << " / " << cosAng << G4endl; //GDEB
	for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	  // G4cout << " posZ " << posZ << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(thePosTransv, theScanWorldMaxY, posZ), evt ); 
	}
	if( angle != 180*CLHEP::deg ) {	
	  //----- SCAN ALONG Y AXIS AT POSITIVE Z
	  sstep = fabs(theStepScan/sinAng);
	  double posY = theScanWorldMinY + sstep;
	  for( ; posY < theScanWorldMaxY; posY += sstep ) {
	    GenerateParticle( G4ThreeVector(thePosTransv, posY, theScanWorldMaxZ), evt ); 
	  }
	}
      } else { // < 360
	//----- SCAN ALONG Y AXIS AT POSITIVE Z
	sstep = fabs(theStepScan/sinAng);
	double posY = theScanWorldMinY + sstep;
	//	G4cout << " 0posZ " << posZ << G4endl; //GDEB
	//	G4cout << "<360 ANGLE " << angle-270*CLHEP::deg << " " << sstep << " " << theStepScan << " / " << cosAng << G4endl; //GDEB
	for( ; posY < theScanWorldMaxY; posY += sstep ) {
	  GenerateParticle( G4ThreeVector(thePosTransv, posY, theScanWorldMaxZ), evt ); 
	}
	if( angle != 270*CLHEP::deg ) {	
	  //----- SCAN ALONG Z AXIS AT NEGATIVE Y
	  sstep = fabs(theStepScan/cosAng);
	  double posZ = theScanWorldMinZ + sstep;
	  for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	    GenerateParticle( G4ThreeVector(thePosTransv, theScanWorldMinY, posZ), evt ); 
	  }
	}
      }
    }
  }
#endif  
}

//------------------------------------------------------------------------
void GmGeneratorScanVis::GenerateParticle(G4ThreeVector pos, G4Event* evt )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << "  GmGeneratorScanVis::GenerateParticle " << pos << " dir " << theDir << G4endl;
#endif
  //  G4cout << " GenerateParticle pos " << pos << G4endl; //GDEB
  G4PrimaryVertex* vtx = new G4PrimaryVertex( pos, 0. );
  evt->AddPrimaryVertex( vtx );
  G4ParticleDefinition* partDef = G4Geantino::Geantino();
  G4PrimaryParticle* particle = new G4PrimaryParticle( partDef, theDir.x(), theDir.y(), theDir.z());

  vtx->SetPrimary(particle);

}

//------------------------------------------------------------------------
G4bool GmGeneratorScanVis::IncrementPosTransv()
{
  thePosTransv += theStepFig;
  G4bool bOK = true;
  switch (thePlaneFig) {
  case PF_XY:
    if( thePosTransv > theScanWorldMaxZ ) {
      bOK = false;
    }
    break;
  case PF_XZ:
    if( thePosTransv > theScanWorldMaxY ) {
      bOK = false;
    }
    break;
  case PF_YZ:
    if( thePosTransv > theScanWorldMaxX ) {
      bOK = false;
    }
    break;
  }

  if( !bOK ) {
    G4Exception("GmGeneratorScanVis::IncrementPosTransv",
		"",
		JustWarning,
		("TooBig transversal position"+GmGenUtils::ftoa(thePosTransv)
		 +" plane orientation "+GmGenUtils::ftoa(thePlaneFig)).c_str());
    G4RunManager::GetRunManager()->AbortRun();  //  /abortrun
  }

  return bOK;
}

//------------------------------------------------------------------------
void GmGeneratorScanVis::SetDir( G4double angle ) 
{
  switch (thePlaneFig ) {
  case PF_XY:
    theDir = G4ThreeVector(cos(angle),sin(angle),0.);
    break;
  case PF_XZ:
    theDir = G4ThreeVector(cos(angle),0.,sin(angle));
    break;
  case PF_YZ:
    theDir = G4ThreeVector(0.,cos(angle),sin(angle));
  }

}


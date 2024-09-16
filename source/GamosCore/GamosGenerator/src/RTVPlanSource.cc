#include "RTVPlanSource.hh"
#include "RTIonPlanBeam.hh"
#include "GmGenerDistTimeConstant.hh"
#include "GmGenerDistTimeConstantChange.hh"
#include "GmGenerDistDirectionConst.hh"
#include "GmGenerDistDirectionCone.hh"
#include "GmGenerDistDirectionCone2D.hh"
#include "GmGenerDistDirectionConeGaussian.hh"
#include "GmGenerDistDirectionCone2DGaussian.hh"
#include "GmGenerDistPositionDirection2DCorrelGaussian.hh"
#include "GmGenerDistPositionDirection2DCorrelDoubleGaussian.hh"
#include "GmGenerDistPositionDisc.hh" 
#include "GmGenerDistPositionDiscGaussian.hh" 
#include "GmGenerDistEnergyConstant.hh"
#include "GmGenerDistEnergyGaussian.hh"
#include "GmGeneratorMgr.hh"
#include "RTPlanMessenger.hh"
#include "RTPlanMgr.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmVModule.hh"
#include "GamosCore/GamosGeometry/include/GmModuleJaws.hh"
#include "GamosCore/GamosGeometry/include/GmModuleMLC.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmModuleMgr.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosGeometry/include/GmReadPhantomGeometry.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosData/Distributions/include/GmNumericDistributionLinLin.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryMessenger.hh"

#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "G4tgrUtils.hh"
#include "G4tgrVolumeMgr.hh"
#include "G4tgrVolume.hh"
#include "G4tgrPlace.hh"
#include "G4GeometryManager.hh"
#include "G4VVisManager.hh"
#include "G4Box.hh"
#include "G4UImanager.hh"

#include "CLHEP/Random/RandFlat.h"
#include <math.h>

//-----------------------------------------------------------------------
RTVPlanSource::RTVPlanSource( const G4String& name): GmParticleSource( name )
{
  new RTPlanMessenger(this);

  bInitializedGeom = false;
  thePositionDistribution = 0;
  theDirectionDistribution = 0;
  theEnergyDistribution = 0;
  theTimeDistribution = 0;
  theAcceleratorVolume = "";  
  theAcceleratorPV = 0;
  theLateralSpreadingDevice = "";  
  //  bPhantomMoved = false;

}

//-----------------------------------------------------------------------
void RTVPlanSource::Initialize( const G4String& partName)
{

  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  theParticleDef = partTable->FindParticle( partName);
  if( !theParticleDef ) {
    theParticleDef = (G4ParticleDefinition*)(GmGetParticleMgr::GetInstance()->CreateIon(partName));

  }

  if( !theParticleDef ) {
    G4Exception("RTVPlanSource::RTVPlanSource",
		"Wrong argument",
		FatalErrorInArgument,
		G4String(GetName() + "  particle does not exist: " + partName).c_str());
  }

//  bCoordSystemPatient = G4bool(theParamMgr->GetNumericValue(theName+":CoordinateSystemPatient",1));

  theG4RunManager = G4RunManager::GetRunManager();

  InitializeDistributions();

  theRTPlanMgr = RTPlanMgr::GetInstance();
  theRTPlanMgr->SetRTSource(this);

  bRTHistoControlPoint = G4bool(theParamMgr->GetNumericValue(theName+":RTHistoControlPoint",0.));
    
}

//-----------------------------------------------------------------------
void RTVPlanSource::InitializeDistributions()
{

  SetDistributionTime(new GmGenerDistTimeConstant);

  theParamMgr = GmParameterMgr::GetInstance();
  theActivity = 1.*CLHEP::becquerel;
  thePositionDistribution2D = 0;  
  if( !thePositionDistribution ) {
    SetDistributionPosition(new GmGenerDistPositionDisc);
    static_cast<GmGenerDistPositionDisc*>(thePositionDistribution)->SetRadius(1.);
    G4Exception("RTVPlanSource::RTVPlanSource",
		"",
		JustWarning,
		G4String(GetName() + " No position distribution defined, using GmGenerDistPositionDisc with radius=ScanSpotSize/2.").c_str());    

  }
  thePositionDistribution2D = static_cast<GmVGenerDistPosition2D*>(thePositionDistribution);
  if( !thePositionDistribution2D ) {
    G4Exception("RTVPlanSource::GenerateVertex",
		  "",
		  JustWarning,
		  ("Position distribution must be a 2D one (Disc, DiscGaussian, Square, Rectangle) while it is of type " + thePositionDistribution->GetName()).c_str() );
  }
  if( !theDirectionDistribution ) {
    SetDistributionDirection(new GmGenerDistDirectionConst); // set also ParticleSource pointer
    std::vector<G4String> paramsD;
    paramsD.push_back("0.");
    paramsD.push_back("0.");
    paramsD.push_back("-1.");
    theDirectionDistribution->SetParams( paramsD );
    G4Exception("RTVPlanSource::RTVPlanSource",
		"",
		JustWarning,
		G4String(GetName() + " No direction distribution defined, using GmGenerDistDirectionConst with direction (0,0,-1)").c_str());    
  }
  
  theEnergySigma = theParamMgr->GetNumericValue(theName+":EnergySigma",0.);
  if( theEnergySigma == 0. ) {
    bEGauss = false;
    //dummy distribution, not used
    SetDistributionEnergy(new GmGenerDistEnergyConstant);
  } else {
    bEGauss = true;
    SetDistributionEnergy(new GmGenerDistEnergyGaussian);
  }

  bIsocenterAtZero = G4bool(theParamMgr->GetNumericValue(theName+":IsocenterAtZero",1));
  // bZeroAtIsocentre = G4bool(theParamMgr->GetNumericValue(theName+":ZeroAtIsocentre",(bIsocenterAtZero==0)));
  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " bIsocenterAtZero " << bIsocenterAtZero << G4endl; 
#endif
  
  CheckDistributionsExist();

}

//-----------------------------------------------------------------------
void RTVPlanSource::InitializeGeometry()
{
  theGeomUtils = GmGeometryUtils::GetInstance();
  theModuleMgr = GmModuleMgr::GetInstance();

#ifndef WIN32
    G4tgrUtils::SetAllowRepetitions(true);
#else
    G4tgrUtils tu; tu.SetAllowRepetitions(true);
#endif
  BuildRTPlan2G4Associations();
  BuildRTBeam2G4Associations();
  
  //--- Store original phantom translation and rotation 
  std::vector<G4VPhysicalVolume*> PVs = theGeomUtils->GetPhysicalVolumes("phantomContainer",0);
  if( PVs.size() > 0 ) {
    G4VPhysicalVolume* pv = PVs[0];
    theOrigPhantomTranslation = pv->GetTranslation();
    //      G4cout << pv << " Move  CENTER PHANTOM CONTAINER " << pv->GetTranslation() << pv << G4endl; //GDEB
    //      G4cout << "1RTPlanMgr::MovePhantom theOrigPhantomRotMat " << theOrigPhantomRotMat  << G4endl;   //GDEB
    theOrigPhantomRotMat = *(pv->GetRotation());
    //   G4cout << "2RTPlanMgr::MovePhantom theOrigPhantomRotMat " << theOrigPhantomRotMat  << G4endl;   //GDEB
    bMovePhantom = true;
  } else {
    bMovePhantom = false;
  }
}   

//-----------------------------------------------------------------------
G4PrimaryVertex* RTVPlanSource::GenerateVertex( G4double time ) 
{
  if( !bInitializedGeom ) {
    InitializeGeometry();
    bInitializedGeom = true;
    theRTPlanMgr->CalculateMetersetEvts();
    bInitProtonsPerMU = theRTPlanMgr->IsInitProtonsPerMU();
    if( bInitProtonsPerMU ) {
      theTimeEnergyDataDistribution = theRTPlanMgr->GetTimeEnergyDataDistribution();
    }
  }
  
  G4bool bOK = true;
  RTBeamStateData bsdata = theRTPlanMgr->GetRTBeamStateData(time, 0, bOK);
  //  G4cout << "  theRTPlanMgr->IsChangedBeamState() " <<  theRTPlanMgr->IsChangedBeamState() << " " << bInitProtonsPerMU << G4endl; //GDEB
    
  if( theRTPlanMgr->IsChangedBeamState() ) {
    //---- New BeamState data
    std::vector<G4String> paramsE;
    //    G4ThreeVector position = G4ThreeVector(bsdata.PositionX,bsdata.PositionY,bsdata.PositionZ);
    
    theEnergy = bsdata.Energy;    
    if( bEGauss ) { 
      paramsE.push_back(GmGenUtils::ftoa(theEnergy));
      paramsE.push_back(GmGenUtils::ftoa(theEnergySigma));
    } else {
      paramsE.push_back(GmGenUtils::ftoa(theEnergy));
    }
    theEnergyDistribution->SetParams( paramsE );

    //    G4cout << " CHANGED BEAM STATE " << bsdata.Energy << G4endl; //GDEB
    if ( dynamic_cast< GmGenerDistPositionDirection2DCorrelDoubleGaussian*>(thePositionDistribution) != 0 ) {
      GmGenerDistPositionDirection2DCorrelDoubleGaussian* posDist = dynamic_cast< GmGenerDistPositionDirection2DCorrelDoubleGaussian*>(theDirectionDistribution);
      posDist->SetParamsEnergy(GmGenUtils::ftoa(theEnergy));
    }
      
      
    MoveBeam( bsdata );
    SetSpotSize( thePositionDistribution2D, bsdata );
    
    MoveAccelerator(bsdata);
    if( bMovePhantom ) MovePhantom( bsdata ); // it is not necessary to move it each CP, because it only uses the IsocentrePos

    //change the time interval according to protons per MU for new energy
    if( bInitProtonsPerMU ) {
      //-  if( theTimeEnergyDataDistribution != 0 ) {
      GmGenerDistTimeConstantChange* timeDistCC = dynamic_cast<GmGenerDistTimeConstantChange*>(theTimeDistribution);
      if( ! timeDistCC ) {
	G4Exception("",
		    "",
		    FatalException,
		    ("Time distribution : "+theTimeDistribution->GetName()+" must be of type GmGenerDistTimeConstantChange if '/P PTBeamProtonsPerMU:FileName' is used").c_str());
      }
      timeDistCC->SetInterval(CLHEP::second/theTimeEnergyDataDistribution->GetNumericValueFromIndex(bsdata.Energy));
      time += GenerateTime();
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << this << "@@!!  RTPlanMgr::GenerateVertex Interval " << timeDistCC->GetInterval() << "= 1.e9/" << theTimeEnergyDataDistribution->GetNumericValueFromIndex(bsdata.Energy) << " E= " << bsdata.Energy << " new time " << time << G4endl; 
#endif
    }
  }    

  if( !bOK ) return (G4PrimaryVertex*)0;

  thePosition = thePositionDistribution->GeneratePosition( this );
  theDirection = theDirectionDistribution->GenerateDirection(this);
  theEnergy = theEnergyDistribution->GenerateEnergy(this);

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << "RTVPlanSource::GenerateVertex pos " << thePosition << G4endl;
#endif
  
  G4PrimaryVertex* vtx = new G4PrimaryVertex( thePosition, time );
  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << "RTVPlanSource::GenerateVertex ener " << theEnergy << G4endl;
#endif
  
  G4ThreeVector mom = theDirection * std::sqrt(theEnergy*theEnergy + 2.*theParticleDef->GetPDGMass()*theEnergy);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " RTVPlanSource::GenerateVertex mom " << mom << " theEnergy " << theEnergy << G4endl;
#endif
  G4PrimaryParticle* particle = new G4PrimaryParticle( theParticleDef, mom.x(), mom.y(), mom.z() );

  particle->SetWeight( theWeight );

  vtx->SetPrimary(particle);

  if( bRTHistoControlPoint ) {
    //    theRTPlanMgr->FillRTHistoControlPoint();
    /*    theAnaMgr->GetHisto2D(theHCurrentNumber)->Fill(1e9,1e9); // just increment the number of entries
    theAnaMgr->GetHisto2D(870000)->Fill(1e9,1e9); // just increment the number of entries
    G4int hCurrentNumberBeam = theHCurrentNumber- (theHCurrentNumber-870000)%theMaxNCP;
    theAnaMgr->GetHisto2D(hCurrentNumberBeam)->Fill(1e9,1e9); // just increment the number of entries
    G4cout << "FILL theHCurrentPos " << theHCurrentNumber << G4endl; //GDEB
    */
  } 

  return vtx;

}

//------------------------------------------------------------------------
void RTVPlanSource::AssociatePlanVolumes( std::vector<G4String> wl )
{
  if( wl.size() != 3 ) {
    G4Exception("RTVPlanSource::AssociatePlanVolumes",
		"",
		FatalException,
		("Command should have three words: SOURCE_NAME RTPLAN_VOLUME_NAME GEOMETRY_VOLUME_NAME , while it has " + GmGenUtils::itoa(wl.size())).c_str());
  }

  theRTPlanVoluAssoc[wl[1]] = wl[2];
  
}

//------------------------------------------------------------------------
void RTVPlanSource::AssociateBeamVolumes( std::vector<G4String> wl )
{
  if( wl.size() < 1 ) {
    G4Exception("RTVPlanSource::AssociateBeamVolumes",
		"",
		FatalException,
		("Command should have more than one word: SOURCE_NAME GEOMETRY_VOLUME_NAME , while it has " + GmGenUtils::itoa(wl.size())).c_str());
  }

  for( size_t ii = 1; ii < wl.size(); ii++ ) {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " RTVPlanSource::AssociateBeamVolumes " << wl[ii] << G4endl;
#endif
    theRTBeamVoluAssoc.push_back(wl[ii]);
  }

}

//------------------------------------------------------------------------
void RTVPlanSource::AssociatePlanLimitingDeviceAngle( std::vector<G4String> wl )
{
  if( wl.size() != 2 ) {
    G4Exception("RTVPlanSource::AssociatePlanLimitingDeviceAngle",
		"",
		FatalException,
		("Command should have twowords: SOURCE_NAME GEOMETRY_VOLUME_NAME , while it has " + GmGenUtils::itoa(wl.size())).c_str());
  }

  theLDAVolu = wl[1];
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " RTVPlanSource::AssociatePlanLimitingDeviceAngle" << theLDAVolu << G4endl;
#endif
  
}

//------------------------------------------------------------------------
void RTVPlanSource::DefineAcceleratorVolume( std::vector<G4String> wl )
{
  if( wl.size() != 2 ) {
    G4Exception("RTVPlanSource::DefineAcceleratorVolume",
		"",
		FatalException,
		("Command should have two words: SOURCE_NAME VOLUME_NAME , while it has " + GmGenUtils::itoa(wl.size())).c_str());
  }

  theAcceleratorVolume = wl[1];
}


//------------------------------------------------------------------------
void RTVPlanSource::BuildRTPlan2G4Associations()
{
  for( size_t iib = 0; iib < theBeams.size(); iib++ ) {
    //---- Look for Geant4 volumes
    std::map<G4String,G4double> beamParams = theBeams[iib]->GetParams();
    std::map<G4String,G4double>::const_iterator itep;
    theGeomUtils = GmGeometryUtils::GetInstance();
    for( itep = beamParams.begin() ; itep != beamParams.end(); itep++ ) {
      G4String pname = (*itep).first;
      if( pname.substr(0,4) == "BLD_" && pname.substr(pname.length()-2,2) == "_Z" ) {
	G4String rtVName = pname.substr(4,pname.length()-6);
	std::map<G4String,G4String>::const_iterator itepg = theRTPlanVoluAssoc.find(rtVName);
	G4String g4VName = "";
	if( itepg == theRTPlanVoluAssoc.end() ) {
	  G4Exception("RTVPlanSource::BuildRTPlan2G4Associations",
		      "",
		      JustWarning,
		      ("No association of RT volume " + rtVName + " through an associatePlanVolumes user command, this volume will not be moved").c_str());
	  continue;
	} else {
	  g4VName = (*itepg).second;
	}
#ifdef GAMOS_NO_VERBOSE
	if( GenerVerb(infoVerb) ) G4cout << " theGeomUtils->GetPhysicalVolumes(g4VName " << g4VName << G4endl; 
#endif
	std::vector<G4VPhysicalVolume*> PVs = theGeomUtils->GetPhysicalVolumes(g4VName+"*", 1);
	G4double nLeafJaws = theBeams[iib]->GetParam("BLD_"+rtVName+"_NumberOfLeafJawPairs",1) * 2;
	if( PVs.size() != nLeafJaws ) {
	  G4Exception("RTVPlanSource::BuildRTPlan2G4Associations",
		      "",
		      FatalException,
		      ("Number of G4VPhysicalVolume's for volume " + g4VName + " = "
		       + GmGenUtils::itoa(PVs.size()) + " != number of volumes in RTPlan = "
		       + GmGenUtils::itoa(nLeafJaws) + "Please contact GAMOS authors").c_str());
	}
	theRTPlanG4PVsAssoc[rtVName] = PVs;
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::AssociatePlanVolumes  Beam " << theBeams[iib]->GetIndex() << " " << rtVName << " associated to N PVs " << PVs.size() << G4endl;
#endif
	GmVModule* module = theModuleMgr->GetModule(g4VName,0);
	if( !module ) {
	  G4Exception("RTVPlanSource::BuildRTPlan2G4Associations",
		      "",
		      FatalException,
		      ("Volume must be a GmModule (i.e. constructed with :MODULE in an ASCII geometry file: " + g4VName).c_str());
	}
	theRTPlanGmModuleAssoc[rtVName] = module;
	CheckParamInAllControlPoints(rtVName);
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::AssociatePlanVolumes  Beam. " << theBeams[iib]->GetIndex() << " " << rtVName << " associated to module " << module->GetName() << G4endl;
#endif
      }
    }
  }

  //----- Accerator volume
  if( theAcceleratorVolume != "" && bIsocenterAtZero ) {
    //--- Get volume
    std::vector<G4VPhysicalVolume*> PVAs = theGeomUtils->GetPhysicalVolumes(theAcceleratorVolume, 1);
    if( PVAs.size() > 1 ) {
      G4Exception("RTVPlanSource::BuildRTPlan2G4Associations",
		  "",
		  FatalException,
		  ("There are more than one accelerator volume "+theAcceleratorVolume).c_str());
    }
    G4VPhysicalVolume* PVA = PVAs[0];
    G4LogicalVolume* motherAccel = PVA->GetMotherLogical();
    std::vector<G4VPhysicalVolume*> PVMAs = theGeomUtils->GetPhysicalVolumes(motherAccel->GetName(), 1);
    if( PVMAs[0] == 0 || PVMAs[0]->GetMotherLogical() != 0 ) {
      G4Exception("RTVPlanSource::BuildRTPlan2G4Associations",
		  "",
		  FatalException,
		  ("Accelerator should be placed directly in the world volume, it is in "+motherAccel->GetName()).c_str());
    }
    
    G4ThreeVector position = PVA->GetTranslation();
    if( position.x() > 1.e-6 || position.y() > 1.e-6 ) {
      G4Exception("RTVPlanSource::BuildRTPlan2G4Associations",
		  "",
		  FatalException,
		  "Accelerator initial X and Y position must be 0.");
    }
    if( position.z() < 0. ) {
      G4Exception("RTVPlanSource::BuildRTPlan2G4Associations",
		  "",
		    FatalException,
		  "Accelerator initial Z cannot be negative: isocentre is at zero and beam travels along -Z");
    }
    if( !PVA->GetRotation()->isIdentity() ) {
      G4cerr << " Accelerator Rotation " << *( PVA->GetRotation() ) << G4endl; 
      G4Exception("RTVPlanSource::BuildRTPlan2G4Associations",
		  "",
		  FatalException,
		  "Accelerator initial rotation must be identity");
    }
    
    theAcceleratorPV = PVA;
    theAcceleratorInitialPos = position;
    /*  } else if( bIsocenterAtZero ) {
    G4Exception("RTVPlanSource::BuildRTPlan2G4Associations",
		"",
		FatalException,
		"It is requested to move accelerator, but theres is no accelerator volume. Please use /gamos/generator/RTPlan/defineAcceleratorVolume");
    */
  }


  if( theLDAVolu != "" ) {
    std::vector<G4VPhysicalVolume*> PVs = GmGeometryUtils::GetInstance()->GetPhysicalVolumes(theLDAVolu);
    if( PVs.size() != 1 ) {
      G4Exception("RTVPlanSource::AssociatePlanVolumes",
		  "",
		  FatalException,
		  ("There can only be one physical volume associated to LimitingDeviceAngle "+theLDAVolu).c_str());
    }
    theLDAG4PV = PVs[0];
  } else {
    G4Exception("RTVPlanSource::AssociatePlanVolumes",
		"",
		JustWarning,
		"No volume associated to LimitingDeviceAngle, use /gamos/generator/RTPlan/associatePlanLimitingDeviceAngle <VOLUME_NAME> ");
  }
  
}

//------------------------------------------------------------------------
void RTVPlanSource::BuildRTBeam2G4Associations()
{

  for( std::vector<G4String>::const_iterator ite = theRTBeamVoluAssoc.begin(); ite != theRTBeamVoluAssoc.end(); ite++ ) {
    G4String g4VName = *ite;
    std::vector<G4VPhysicalVolume*> PVs = theGeomUtils->GetPhysicalVolumes(g4VName, 1);
    if( PVs.size() != 1 ) {
      G4Exception("RTVPlanSource::AssociatePlanVolumes",
		  "",
		  FatalException,
		  ("There can only be one physical volume associated to LimitingDeviceAngle "+theLDAVolu).c_str());
    }
    theRTBeamG4PVs.push_back(PVs[0]);
    theRTBeamPosition.push_back(PVs[0]->GetTranslation());
    theRTBeamRotation.push_back(*(PVs[0]->GetRotation()));
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::BuildRTBeam2G4Associations " << g4VName << " position " << PVs[0]->GetTranslation() << " rotation " << *(PVs[0]->GetRotation()) << G4endl;
#endif

  }
}

//------------------------------------------------------------------------
void RTVPlanSource::MoveAccelerator(const RTBeamStateData& bsdata )
{
  //--- Only if  BuildRTPlan2G4Associations() has been called (else no MLC, no JAWS, no theAcceleratorPV
  if( theRTPlanG4PVsAssoc.size() == 0 ) return;
  
  G4GeometryManager* geomMgr = G4GeometryManager::GetInstance();
  geomMgr->OpenGeometry();

  G4VPhysicalVolume* pv;
  //----- MOVE BLD'S (JAWS and MLC's)
  //--- Loop to all RTPlan BLD's found
  std::map<G4String,std::vector<G4VPhysicalVolume*> >::const_iterator itespv;
  for( itespv = theRTPlanG4PVsAssoc.begin(); itespv != theRTPlanG4PVsAssoc.end(); itespv++ ) {
    G4String rtVol2G4Name = (*itespv).first;
    std::vector<G4VPhysicalVolume*> PVs = (*itespv).second;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) {
      for( size_t ipv = 0; ipv < PVs.size(); ipv++ ) {
	G4cout << "MoveAccelerator PVs TO MOVE " << ipv << " = " << PVs[ipv]->GetName() << G4endl; 
      }
    }
#endif
    //--- Get position from beam and control point
    G4double posZ = 0; // Z position of center
    
    std::map<G4String,G4double> beamParams = bsdata.Beam->GetParams();
    //    std::map<G4String,G4double> cpParams = bsdata.ControlPoint->GetParams();
    std::map<G4String,G4double>::const_iterator itep;

    //--- LOOK FOR THE posZ IN RTBeam PARAMETERS
    for( itep = beamParams.begin() ; itep != beamParams.end(); itep++ ) {
      G4String pname = (*itep).first;
      if( pname.substr(0,4) == "BLD_" && pname.substr(pname.length()-2,2) == "_Z" ) {
	G4String rtVName = pname.substr(4,pname.length()-6);
	if( rtVName == rtVol2G4Name ) {
	  posZ = -(*itep).second; // DICOM gives Positions of botomm of JAWS/MLC as a positive number  (distance to origin)
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator " << rtVol2G4Name << " position Z " << posZ << G4endl;
#endif
	  break;
	}
      }
    }
    if( itep == beamParams.end() ) {
      G4Exception("RTVPlanSource::MoveAccelerator",
		  "",
		  FatalException,
		  ("VOLUME " + rtVol2G4Name + " NOT FOUND IN CURRENT BEAM: THIS SHOULD NOT HAPPEN, PLEASE CONTACT GAMOS AUTHORS").c_str());
    }

    //--- NOW LOOK FOR posX/Y IN RTPlanControlPoint PARAMETERS
    std::map<G4String,GmVModule*>::const_iterator itemod = theRTPlanGmModuleAssoc.find(rtVol2G4Name);
    GmVModule* gmModule = (*itemod).second;
    G4String g4PVName = gmModule->GetName();
    if( GenerVerb(debugVerb) ) G4cout << " gmModule->GetName() " << g4PVName << G4endl;
     
    G4double nLeafJaws = bsdata.Beam->GetParam("BLD_"+rtVol2G4Name+"_NumberOfLeafJawPairs",1) * 2;
    if( dynamic_cast<GmModuleJaws*>(gmModule) ) {
      if( nLeafJaws != 2 ) {
	G4Exception("RTVplanSource::MoveAccelerator",
		    "",
		    FatalException,
		    ("Error processing volume of type GmModuleJaws: there should be two leafs in RTPlan, while there are "
		     + GmGenUtils::itoa(nLeafJaws)).c_str());
      }

      //      G4cout << " CHECH PARAM IN PC " <<  bsdata.ControlPoint << " " << rtVol2G4Name+"_1_POS" << G4endl; //GDEB
      G4double posN_1 = bsdata.ControlPoint->GetParam(rtVol2G4Name+"_1_POS",1);
      G4double posP_2 = bsdata.ControlPoint->GetParam(rtVol2G4Name+"_2_POS",1);
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator " << rtVol2G4Name << " position leaf1 " << posN_1 << " leaf2 " << posP_2 << G4endl;
#endif
      /*      // check that parameters in RTPlan have correct sign
      if( posN_1 > 0 || posP_2 < 0) {
	G4Exception("RTVplanSource::MoveAccelerator",
		    "",
		    FatalException,
		    ("Error processing volume of type GmModuleJaws: first position should be negative and second positive, while they are "
		     + GmGenUtils::ftoa(posN_1) + " , " + GmGenUtils::ftoa(posP_2)).c_str());
		     } */
      //      G4int ngmParams = gmModule->GetParams().size();
      gmModule->ReplaceParam("FIELD_NEG", GmGenUtils::ftoa(posN_1));
      gmModule->ReplaceParam("FIELD_POS", GmGenUtils::ftoa(posP_2));
      posZ += GmGenUtils::GetValue(gmModule->GetParam("ZHDIM"));// add half_length_Z (position in RTPlanBeam is of bottom=farthest to beam)
      gmModule->ReplaceParam("Z_CENTRE", GmGenUtils::ftoa(posZ)); 
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator " << rtVol2G4Name << " position Z " << posZ << G4endl;
#endif
      //      G4cout << " posZ-gmModule->GetParam(5)) " << gmModule->GetParam("ZHDIM") << " -> " << GmGenUtils::GetValue(gmModule->GetParam("ZHDIM")) << G4endl; //GDEB
      gmModule->BuildObjects(); // BUILD G4tgr OBJECTS

      //find new G4tgrPlace
      /*      std::vector<G4tgrVolume*> tgrVolumes =  G4tgrVolumeMgr::GetInstance()->GetVolumeList();
      for( size_t itv = 0; itv < tgrVolumes.size(); itv++ ) {
	if( tgrVolumes[itv]->GetName() == g4PVName ) {
	  std::vector<G4tgrPlace*> tgrPlaces = tgrVolumes[itv]->GetPlacements();
	  G4cout << " FOUND " << g4PVName << " = " << tgrVolumes[itv] << " NPLACES " << tgrPlaces.size() << G4endl; //GDEB
	  for( size_t ip = 0; ip < tgrPlaces.size(); ip++ ) {
	    G4cout << " PLACE COPYNO " << tgrPlaces[ip]->GetCopyNo() << G4endl;
	  }
	}
	}*/ 
      std::vector<G4tgrVolume*> tgrVolumes = G4tgrVolumeMgr::GetInstance()->GetVolumeList();
      for( size_t itv = 0; itv < tgrVolumes.size(); itv++ ) {
#ifndef GAMOS_NO_VERBOSE
	//	if( GenerVerb(testVerb) ) G4cout << " tgrVolumes[itv]->GetName() == g4PVName " << tgrVolumes[itv]->GetName()<< " == " << g4PVName << G4endl;
#endif
	if( tgrVolumes[itv]->GetName().find(g4PVName) != std::string::npos ) {
	  std::vector<G4tgrPlace*> tgrPlaces = tgrVolumes[itv]->GetPlacements();
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator " << rtVol2G4Name << " Number of tgrPlaces " << tgrPlaces.size() << G4endl;
#endif
	  //--- Get last two
	  G4tgrPlace* placeN = tgrPlaces[tgrPlaces.size()-2];
	  G4tgrPlace* placeP = tgrPlaces[tgrPlaces.size()-1];
	  //--- Get unique two
	  if( tgrPlaces.size() != 2 ) {
	    G4Exception("RTVPlanSource::MoveAccelerator",
		       "",
		       FatalException,
		       ("Not only two placement found for volume " + g4PVName
			+ "  Number of placements found= " + GmGenUtils::itoa(tgrPlaces.size())).c_str());
	  }
	  //--- Now change placements in G4VPhysicalVolume's
	  //--- Move volume
	  //t	  geomMgr->OpenGeometry( PVs[0]);
	  //	  G4cout << " MOVING PV0 " << PVs[0] << G4endl; //GDEB
	  PVs[0]->SetTranslation(placeN->GetPlacement());
	  //t	  geomMgr->CloseGeometry(true,false,PVs[0]);
	  //t	  geomMgr->OpenGeometry( PVs[1]);
	  //   G4cout << " MOVING PV1 " << PVs[1] << G4endl; //GDEB
	  PVs[1]->SetTranslation(placeP->GetPlacement());
	  //t	  geomMgr->CloseGeometry(true,false,PVs[1]);

	  std::vector<GmTouchable*> touchs2 = GmGeometryUtils::GetInstance()->GetTouchables( PVs[0]->GetName(), true );
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(testVerb) ) {
	    G4cout << " PRINTING TOUCHABLES " << PVs[0]->GetName() << " : " << touchs2.size() << G4endl; 
	    for( unsigned int jj = 0; jj < touchs2.size(); jj++ ) {
	      G4cout << " TOUCHABLE " << *(touchs2[jj]) << G4endl;
	    }
	  }
#endif

	}
      }
      
    } else if( dynamic_cast<GmModuleMLC*>(gmModule) ) {

      std::map<G4String,G4double> mlcParams = bsdata.ControlPoint->GetParams();
      for( std::map<G4String,G4double>::const_iterator itemp = mlcParams.begin(); itemp != mlcParams.end(); itemp++ ) {
	//-	G4cout << " mlcParams " << (*itemp).first << " = " << (*itemp).second << G4endl; //GDEB
      }
      //----- STORE NEGATIVE LEAFS AND POSITIVE LEAFS POSITIONS FROM RTControlPoint
      std::vector<G4double> posPos, posNeg;
      for( size_t ipv = 1; ipv <= PVs.size()/2; ipv++ ) { //FIRST HALF POSITIONS ARE NEGATIVE LEAFS
	//	G4cout << " SEARCH mlcParam " << rtVol2G4Name+"_"+GmGenUtils::itoa(ipv)+"_POS" << G4endl; //GDEB
	posNeg.push_back(bsdata.ControlPoint->GetParam(rtVol2G4Name+"_"+GmGenUtils::itoa(ipv)+"_POS",1)); //Numbering starts at 1 in DICOM
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator position leaf negative " << rtVol2G4Name+"_"+GmGenUtils::itoa(ipv)+"_POS  = " << posNeg[posNeg.size()-1] << G4endl;
#endif
      }
      for( size_t ipv = PVs.size()/2+1; ipv <= PVs.size(); ipv++ ) { //SECOND HALF POSITIONS ARE POSITIVE LEAFS
	posPos.push_back(bsdata.ControlPoint->GetParam(rtVol2G4Name+"_"+GmGenUtils::itoa(ipv)+"_POS",1));
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator position leaf positive " << rtVol2G4Name+"_"+GmGenUtils::itoa(ipv)+"_POS = " << posPos[posPos.size()-1] << G4endl;
#endif
      }

      //----- REPLACE LEAF POSITIONS IN GmModuleMLC
      for( size_t ii = 1; ii < posNeg.size()+1; ii++ ) {
	gmModule->ReplaceParam("OPEN_POSITION_NEG["+GmGenUtils::itoa(ii)+"]", GmGenUtils::ftoa(posNeg[ii-1]));
	gmModule->ReplaceParam("OPEN_POSITION_POS["+GmGenUtils::itoa(ii)+"]", GmGenUtils::ftoa(posPos[ii-1]));
      }

      //----- REPLACE Z POSITION IN GmModuleMLC
      posZ -= GmGenUtils::GetValue(gmModule->GetParam("Z_GAP"));// substract length_Z (Z_GAP is interpreted to be = leaf length !!! (Z_GAP is negative and position in RTPlanBeam is of bottom=farthest to beam)
      
      gmModule->ReplaceParam("Z_TOP", GmGenUtils::ftoa(posZ)); 
      
      //----- BUILD G4tgr OBJECTS
      gmModule->BuildObjects(); 

      //find new G4tgrPlace
      std::vector<G4tgrVolume*> tgrVolumes =  G4tgrVolumeMgr::GetInstance()->GetVolumeList();
      //	:PLACE MLC_X_80_POS 1 ACCEL RM90Y180Z_MLC 95.1766 0 311.77FF
      for( size_t ii = 1; ii <= posNeg.size()+posPos.size(); ii++ ) {
	G4String suffix = "";
	G4int iip = ii;
	if( ii <= posNeg.size() ) {
	  suffix = "_NEG";
	} else {
	  suffix = "_POS";
	  iip -= posNeg.size();
	}
	G4String g4PVNameLeaf = g4PVName+"_Leaf_"+GmGenUtils::itoa(iip)+suffix;
	for( size_t itv = 0; itv < tgrVolumes.size(); itv++ ) {
#ifndef GAMOS_NO_VERBOSE
	  //	  if( GenerVerb(testVerb) ) G4cout << itv << " tgrVolumes[itv]->GetName() " << tgrVolumes[itv]->GetName()<< " == " << g4PVNameLeaf << G4endl;
#endif
	  if( tgrVolumes[itv]->GetName() == G4String(g4PVNameLeaf) ) {
	    std::vector<G4tgrPlace*> tgrPlaces = tgrVolumes[itv]->GetPlacements();
#ifndef GAMOS_NO_VERBOSE
	    if( GenerVerb(testVerb) ) {
	      G4cout << " FOUND " << g4PVNameLeaf << " = " << tgrVolumes[itv] << " NPLACES " << tgrPlaces.size() << G4endl; 
	      for( size_t ip = 0; ip < tgrPlaces.size(); ip++ ) {
		G4cout << " PLACE COPYNO " << tgrPlaces[ip]->GetCopyNo() << G4endl;
	      }
	    }
	    if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator " << rtVol2G4Name << " Number of tgrPlaces " << tgrPlaces.size() << G4endl;
#endif
	    //--- Get unique one
	    if( tgrPlaces.size() != 1 ) {
	      G4Exception("RTVPlanSource::MoveAccelerator",
			  "",
			  FatalException,
			  ("Not only one placement found for volume " + g4PVNameLeaf
			   + "  Number of placements found= " + GmGenUtils::itoa(tgrPlaces.size())).c_str());
	    }
	    G4tgrPlace* place = tgrPlaces[tgrPlaces.size()-1];
	    //--- Now change placements in G4VPhysicalVolume's
	    //--- Move volume
	    //--- Check volume name
	    pv = PVs[iip*2-1-(ii<=posNeg.size())]; // FIRST IS X-2 (NEG), not X-1 (POS)
	    if( pv->GetName() != G4String(g4PVNameLeaf) ) {
	      G4Exception("RTVPlanSource::MoveAccelerator",
			  "",
			  FatalException,
			  ("Trying to move wrong volume: " + pv->GetName() +
			   " instead of " + g4PVNameLeaf).c_str());
	    }
	    //t	    geomMgr->OpenGeometry( pv );
	    pv->SetTranslation(place->GetPlacement());
	    //t	    geomMgr->CloseGeometry(true,false,pv);
#ifndef GAMOS_NO_VERBOSE
	    if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator VOLUME MOVED " << pv->GetName() << " -> " << pv->GetTranslation() << G4endl;
#endif
	      
#ifndef GAMOS_NO_VERBOSE
	    if( GenerVerb(testVerb) ) {
	      std::vector<GmTouchable*> touchs2 = GmGeometryUtils::GetInstance()->GetTouchables(pv->GetName(), true);
   	      G4cout << " PRINTING TOUCHABLES " << pv->GetName() << " : " << touchs2.size() << G4endl; 
	      for( unsigned int jj = 0; jj < touchs2.size(); jj++ ) {
	        G4cout << " TOUCHABLE " << *(touchs2[jj]) << G4endl;
	      }
	    }
#endif
	  }
	}
      }

    } else {
      G4Exception("RTVplanSource::MoveAccelerator",
		  "",
		  FatalException,
		  ("VOLUME " + rtVol2G4Name + " NOT FOUND IN CURRENT BEAM: THIS SHOULD NOT HAPPEN, PLEASE CONTACT GAMOS AUTHORS").c_str());
    }

  }

  //----- MOVE ACCELERATOR
  //  G4ThreeVector position(0.,0.,sourceAxisDist); // if not moved it is in Z axis (IEC 61217)
  /*  G4RotationMatrix* rotation = new G4RotationMatrix;
  if( !bIsocenterAtZero ) {
    rotation->rotateZ(-bsdata.RotAngleZ); // GantryPitchAngle
#ifndef GAMOS_NO_VERBOSE


    //  position.rotateY(bsdata.RotAngleY); // GantryAngle
    rotation->rotateY(-bsdata.RotAngleY); // GantryAngle
#ifndef GAMOS_NO_VERBOSE
    //   if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator Accelerator position after GantryAngle " << position << " GantryAngle " << bsdata.RotAngleY/CLHEP::deg  << " Rotation " << *rotation << G4endl;
#endif
}  */
  

  //-- Rotate and move Accelerator
  if( bIsocenterAtZero && theAcceleratorPV ) { // beam is rotated, rotate also accelerator
    G4double angleGantry = bsdata.RotAngleY;
    G4double pitchAngleGantry = bsdata.RotAngleX;
    G4double limitingDeviceAngle = bsdata.RotAngleZ;
    G4ThreeVector accelPos = G4ThreeVector(0.,0.,bsdata.SourceAxisDistanceX);
    //??    if( angleGantry*pitchAngleGantry != 0 ) {
      accelPos += theAcceleratorInitialPos;
      G4RotationMatrix* accelRM = new G4RotationMatrix();
      if( angleGantry != 0 ) {
	accelPos.rotateY(angleGantry);
	accelRM->rotateY(-angleGantry);
      }
      // ??     if( angleGantry*pitchAngleGantry != 0 ) {
      accelPos.rotateX(pitchAngleGantry);
      accelRM->rotateX(-pitchAngleGantry);
      //  }
      //??  if( angleGantry*pitchAngleGantry != 0 ) {
      accelPos.rotateZ(limitingDeviceAngle-theLastLimitingDeviceAngle);
      accelRM->rotateZ(-limitingDeviceAngle+theLastLimitingDeviceAngle);
      theLastLimitingDeviceAngle = limitingDeviceAngle;
      // }
      theAcceleratorPV->SetRotation(accelRM);
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator initial Accelerator new rotation " << *accelRM << G4endl;
#endif
      //??    }
    theAcceleratorPV->SetTranslation(accelPos);
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator initial Accelerator position " << theAcceleratorInitialPos << " + " << G4ThreeVector(0.,0.,bsdata.SourceAxisDistanceX) << " new " << accelPos << G4endl;
#endif

  }

  //---- Rotate around accelerator axis
  G4double limitingDeviceAngle = bsdata.LimitingDeviceAngle; //??*CLHEP::deg;
  if( theAcceleratorPV && limitingDeviceAngle != 0. ) {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(-debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator limitingDeviceAngle " << limitingDeviceAngle/CLHEP::deg << G4endl;
#endif
    G4ThreeVector accelAxis(0.,0.,1.);
    G4RotationMatrix* accelRM = theAcceleratorPV->GetRotation();
    accelAxis *= *accelRM;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator initial accelAxis after accelRM " << accelAxis << " " << *accelRM << G4endl;
#endif
    accelRM->rotate(-limitingDeviceAngle+theLastLimitingDeviceAngle,accelAxis);
    theLastLimitingDeviceAngle = limitingDeviceAngle;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator accelRM after limitingDeviceAngle " << *accelRM << G4endl;
#endif
    theAcceleratorPV->SetRotation(accelRM);
  }
  

  G4RotationMatrix* rotmat = const_cast<G4RotationMatrix*>(theLDAG4PV->GetFrameRotation());
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << " theLDAG4PV rotmat " << *rotmat << G4endl; 
#endif
  //-  G4double limitingDeviceAngle = bsdata.ControlPoint->GetParam("LimitingDeviceAngle");
  rotmat->rotateZ(limitingDeviceAngle-theLastLimitingDeviceAngle);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << " theLDAG4PV rotmat AFTER " << (limitingDeviceAngle-theLastLimitingDeviceAngle)/CLHEP::deg << " " << *rotmat << G4endl;
#endif
  theLastLimitingDeviceAngle = limitingDeviceAngle;
  theLDAG4PV->SetRotation(rotmat);

  geomMgr->CloseGeometry();

  // Notify the VisManager as well
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) pVVisManager->GeometryHasChanged();

  return;
  
}

//------------------------------------------------------------------------
void RTVPlanSource::MovePhantom(const RTBeamStateData& bsdata )
{
  std::vector<G4VPhysicalVolume*> PVs = theGeomUtils->GetPhysicalVolumes("phantomContainer",0); // you may use RTPlan with other geometry not phantoms, and then "phantom" cannot be moved
  if( PVs.size() == 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(warningVerb) ) {
        G4Exception("RTVPlanSource::MovePhantom",
		    "",
		    JustWarning,
		    "No physical volume found with name  phantomContainer, phantom will not be moved");
    }
#endif
    return;
  }
  
  G4GeometryManager* geomMgr = G4GeometryManager::GetInstance();
  geomMgr->OpenGeometry();

  //  if( bIsocenterAtZero && bPhantomMoved ) return;
  PredictInitialDisplacement( bsdata );

  //-- Get center of phantomContainer
  GmReadPhantomGeometry* phGeom = static_cast<GmReadPhantomGeometry*>(const_cast<G4VUserDetectorConstruction*>(theG4RunManager->GetUserDetectorConstruction()));
  if( !phGeom ) {
    G4Exception("RTVPlanSource::MovePhantom",
		"",
		FatalException,
		"Geometry constructor has to be of type GmReadPhantomGeometry");
  }
  G4ThreeVector pcCenter((phGeom->GetMaxX()+phGeom->GetMinX())/2.,
			 (phGeom->GetMaxY()+phGeom->GetMinY())/2.,
			 (phGeom->GetMaxZ()+phGeom->GetMinZ())/2.);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << "RTVPlanSource::MovePhantom phantomCenter " <<pcCenter << G4endl;
#endif

  //--- TRANSLATE AND ROTATE AS GIVEN BY PARAMETERS IN .in FILE (Initial)
  G4VPhysicalVolume* pv = PVs[0];
  //  G4RotationMatrix* phantomRotMat = new G4RotationMatrix(CLHEP::inverseOf(theOrigPhantomRotMat));

  // G4cout << "2RTVPlanSource::MovePhantom theOrigPhantomTranslation " << theOrigPhantomTranslation  << G4endl;  //GDEB

  G4ThreeVector isocenter( bsdata.IsocenterPosition.x(), bsdata.IsocenterPosition.y(), bsdata.IsocenterPosition.z());
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MovePhantom isocenter before origRot " << isocenter << G4endl;
#endif
  G4ThreeVector isocLocalPoint = isocenter - pcCenter;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MovePhantom isocenter local " << isocLocalPoint << "=  isocenter " << isocenter << " - phantomCenter(noOrigDisp/Rot) " << pcCenter << G4endl;
#endif

  /*  isocenter *= *phantomRotMat;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MovePhantom isocenter after origRot " << isocenter << " rotMat " << *phantomRotMat << G4endl;
#endif
  */

  G4RotationMatrix* phantomRotMat = new G4RotationMatrix(theOrigPhantomRotMat);
  isocLocalPoint *= CLHEP::inverseOf(*(phantomRotMat)); //displace to rotate around isocenter
  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MovePhantom isocenter local after inverse phantom rotation " << isocLocalPoint
				    << " theOrigPhantomRotMat " << theOrigPhantomRotMat << "  phantomRotMat " <<  phantomRotMat<< G4endl;
#endif
  //-- isocenter placed at (0,0,-bsdata.SourceAxisDistance)
  G4ThreeVector phantomTranslation;
  if( !bIsocenterAtZero ) phantomTranslation -= G4ThreeVector(0.,0.,bsdata.PositionZ);
  
#ifndef GAMOS_NO_VERBOSE
  //  if( GenerVerb(testVerb) ) G4cout << "RTVPlanSource::MovePhantom phantomTranslation at (0,0,-bsdata.SourceAxisDistance) " << phantomTranslation << " SourceAxisDistance " << bsdata.SourceAxisDistance << G4endl;
#endif

  //--- Rotate GantryAngle, around isocenter  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MovePhantom rotmat " << *phantomRotMat << G4endl;
#endif
  G4RotationMatrix rotMatGA;

  //-  isocLocalPoint.rotateY(bsdata.RotAngleY); //displace to rotate around isocenter
  isocLocalPoint *= CLHEP::inverseOf(rotMatGA); //displace to rotate around isocenter

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MovePhantom isocenter local after rotation " << isocLocalPoint << G4endl;
#endif
  phantomTranslation -= isocLocalPoint;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << "RTVPlanSource::MovePhantom phantomTranslation -isocLocalPoint " <<phantomTranslation  << G4endl;
#endif

#ifndef GAMOS_NO_VERBOSE
  G4RotationMatrix invFinalRot;
  invFinalRot = CLHEP::inverseOf(*(phantomRotMat));
  if( GenerVerb(testVerb) ) G4cout << "RTVPlanSource::MovePhantom FINAL phantomTranslation " <<phantomTranslation  << G4endl
				   << " FINAL phantomRotation " << *phantomRotMat << " INVERSE " << invFinalRot << G4endl;
#endif
  pv->SetTranslation(phantomTranslation);
  pv->SetRotation(phantomRotMat);
  //  G4cout << pv->GetName() << " phantomContainer FINAL " << *(pv->GetRotation()) << G4endl; //GDEB

  /*  GmGeometryUtils::GetInstance()->DumpG4PVLVTree( G4cout, 23 );
  std::vector<GmTouchable*> touchs2 = GmGeometryUtils::GetInstance()->GetTouchables( pv->GetName(), true );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) {
    G4cout << " PRINTING TOUCHABLES " << pv->GetName() << " : " << touchs2.size() << G4endl; 
    for( unsigned int jj = 0; jj < touchs2.size(); jj++ ) {
      G4cout << " TOUCHABLE " << *(touchs2[jj]) << G4endl;
    }
  }
#endif
  
  */
  // Notify the VisManager as well
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) pVVisManager->GeometryHasChanged();

  /*  G4cout << "RTVPlanSource::MovePhantom VIS " << G4endl;
  G4UImanager * UI = G4UImanager::GetUIpointer(); //GDEB
  UI->ApplyCommand("/control/execute visVRML2FILE.in"); //GDEB
  */

  geomMgr->CloseGeometry();
    
  // bPhantomMoved = true;
}

//-----------------------------------------------------------------------
RTVPlanBeam* RTVPlanSource::GetBeam( G4int ii )
{
  if( ii < 0 || ii >= G4int(theBeams.size()) ) {
    G4Exception("RTVPlanSource::GetBeam",
		"",
		FatalErrorInArgument,
		G4String("Asking for beam " + GmGenUtils::itoa(ii) + " while there are only " + GmGenUtils::itoa(theBeams.size())).c_str());
  }

  return theBeams[ii];
}

//------------------------------------------------------------------------
void RTVPlanSource::MoveBeam(const RTBeamStateData& bsdata)
{

  GmVGenerDistPosition2D* pos2D = dynamic_cast<GmVGenerDistPosition2D*>(thePositionDistribution);
  if ( !pos2D ) {
    G4Exception("RTVPlanSource::MoveBeam",
		"",
		FatalException,
		G4String(" Position distribution is not of type GmGenerDistPositionDisc/GmGenerDistPositionDiscFromFile/GmGenerDistPositionDiscGaussian/GmGenerDistPositionRectangle/GmGenerDistPositionSquare").c_str());
  }

  //@@@@@ SET BEAM POSITION
  // direction is by default (0.,0.,-1. ) 
  G4ThreeVector direction(0.,0.,-1.); 
  G4RotationMatrix rotation;
  
  //@@@ Position X,Y in file RTIonControlPoint is at isocentre, i.e. after travelling SourceAxisDistance, put where beam starts: bsdata.PositionZ=pos_Snout. Only relevant for RTIon, for RT posX=posY=0
  //-  G4double distProp = (bsdata.SourceAxisDistance-bsdata.PositionZ)/bsdata.SourceAxisDistance; // positionZ = SnoutPosition
  //-  G4double distPropX = fabs(bsdata.PositionZ/bsdata.SourceAxisDistanceX); // positionZ = SnoutPosition
  //-  G4double distPropY = fabs(bsdata.PositionZ/bsdata.SourceAxisDistanceY); // positionZ = SnoutPosition
  G4double distPropX = fabs((bsdata.SourceAxisDistanceX-bsdata.PositionZ)/bsdata.SourceAxisDistanceX); // positionZ = SnoutPosition
  G4double distPropY = fabs((bsdata.SourceAxisDistanceY-bsdata.PositionZ)/bsdata.SourceAxisDistanceY); // positionZ = SnoutPosition
  G4ThreeVector position(bsdata.PositionX*distPropX,bsdata.PositionY*distPropY,bsdata.PositionZ);
  //  G4cout << "@@@@  RTVPlanSource::MoveBeam bsdata.Position= " << bsdata.PositionX << ", " << bsdata.PositionY << ", " << bsdata.PositionZ << G4endl;   //GDEB
  //  G4cout << " distProp= " << distProp << " SourceAxisDistance= " << bsdata.SourceAxisDistance << " => position " << position << G4endl;  //GDEB
    
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam position at bsdata " << " " << position << " (at isoc) "
				   << G4ThreeVector(bsdata.PositionX,bsdata.PositionY,0.) << " SourceAxisDistanceX " << bsdata.SourceAxisDistanceX << " distPropX " << distPropX 
				   << " SourceAxisDistanceY " << bsdata.SourceAxisDistanceY << " distPropY " << distPropY << G4endl;
#endif
    //    position += G4ThreeVector(0.,0.,0.); // No gantry angle = pos (0.,0.,0.)
    
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam position at Z=0 " << " " << position << G4endl;
#endif

  //@@@ If beam is rotated (not along Z) posX/Y are not anymore in XY plane
  if( bIsocenterAtZero ) { // only if beam must be moved
    position.rotateY(bsdata.RotAngleY);
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam position rotated Gantry Angle " << bsdata.RotAngleY/CLHEP::deg << " " << position << G4endl;
#endif
    if( bsdata.RotAngleZ != 0. ) { 
      G4ThreeVector direcPerp = direction.cross(G4ThreeVector(0.,1.,0.));
      position.rotate(bsdata.RotAngleZ,direcPerp); // GantryPitchAngle
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam position rotated Gantry Pitch Angle " << bsdata.RotAngleZ/CLHEP::deg << " " << position << G4endl;
#endif
    }

  }

  pos2D->SetCentre( position );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam centre " << position << G4endl;
#endif
  
  //@@@@@ SET BEAM DIRECTION
  //--- Beam divergence: it starts at PosX/Y = 0 and reaches bsdata.PositionX/Y after SourceAxisDistance
  G4double rotDivergY = -(bsdata.PositionX/bsdata.SourceAxisDistanceX); 
  G4double rotDivergZ = -(bsdata.PositionY/bsdata.SourceAxisDistanceY);
  //  G4cout << "@@@@  RTVPlanSource::MoveBeam rotDivergY " << rotDivergY << " rotDivergZ " << rotDivergZ << G4endl;  //GDEB

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam direction as IEC 61217 " << direction << " rotDivergY " << rotDivergY << " rotDivergZ " << rotDivergZ << G4endl<< G4endl;
#endif

  //@@@ rotate Gantry_angle+divergence
  //@@@ FIRST: ROTATE DIVERGENCE
  //@@@ SECOND: ROTATE GANTRY ANGLE AROUND Y
  //@@@ THIRD: ROTATE GANTRY PITCH ANGLE AROUND AN AXIS PERPENDICULAR TO Y AND THE DIRECTION OF (0,0,-1) ROTATED GANTRY ANGLE (AS IF THERE WERE NO DIVERGENCE)
  //-  G4ThreeVector direction(-sin(gantryAngle),0.,-cos(gantryAngle));
  //  G4RotationMatrix rotMatPosition = G4RotationMatrix(); // rotation matrix for position GenerDist
  //  rotMatPosition.rotateY(180.*CLHEP::deg); // if not rotation goes along -Z (IEC 61217)  
  if( rotDivergY != 0 || rotDivergZ != 0 ) {  
    G4double dirz = -1./(1+sqr(rotDivergY)+sqr(rotDivergZ));
    G4double dirx = rotDivergY*dirz; // advance dirx for each dirz
    G4double diry = rotDivergZ*dirz;
    direction = G4ThreeVector(dirx,diry,dirz).unit();
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam direction rotated divergence Y & Z " << rotDivergY << " " << rotDivergZ << " dir= " << direction << G4endl;
#endif
  }

  if( bIsocenterAtZero ) {
    if( bsdata.RotAngleY != 0 ) {
      direction.rotateY(bsdata.RotAngleY);
      rotation.rotateY(bsdata.RotAngleY); 
      //      rotMatPosition.rotateY(bsdata.RotAngleY);
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam direction rotated divergence+GantryAngle " << bsdata.RotAngleY/CLHEP::deg << " " << direction << G4endl;
#endif
    }
    
    if( bsdata.RotAngleX != 0. ) { 
      G4ThreeVector dirGantry = G4ThreeVector(0.,0.,-1.).rotateY(bsdata.RotAngleY);
      G4ThreeVector direcPerp = dirGantry.cross(G4ThreeVector(0.,1.,0.)); 
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam direction Perp " << direcPerp << G4endl;
#endif
      direction.rotate(bsdata.RotAngleX,direcPerp); // GantryPitchAngle
      rotation.rotate(bsdata.RotAngleX,direcPerp); 
      //      rotMatPosition.rotateZ(angleZ);
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam direction rotated divergence+GantryPitchAngle " << bsdata.RotAngleX/CLHEP::deg << " " << direction << G4endl;
#endif
    }
    if( bsdata.RotAngleZ != 0. ) { 
      G4ThreeVector dirGantry = G4ThreeVector(0.,0.,-1.);
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam direction Perp " << dirGantry << G4endl;
#endif
      direction.rotate(bsdata.RotAngleZ,dirGantry); // LimitingDeviceAngle
      rotation.rotate(bsdata.RotAngleZ,dirGantry); 
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam direction rotated divergence+LimitingDeviceAngle " << bsdata.RotAngleZ/CLHEP::deg << " " << direction << G4endl;
#endif
    }
  }

  pos2D->SetRotation( rotation );
  
  //@@@ Set generator direction of direction distribution
  //  G4bool bDirOK = false;
  if( dynamic_cast< GmGenerDistDirectionConst*>(theDirectionDistribution) != 0 ||
      dynamic_cast< GmGenerDistDirectionCone*>(theDirectionDistribution) != 0 ||
      dynamic_cast< GmGenerDistDirectionCone2D*>(theDirectionDistribution) != 0 || 
      dynamic_cast< GmGenerDistDirectionConeGaussian*>(theDirectionDistribution) != 0 || 
      dynamic_cast< GmGenerDistDirectionCone2DGaussian*>(theDirectionDistribution) != 0 ||
      dynamic_cast< GmGenerDistDirectionCone*>(theDirectionDistribution) != 0 ||
      dynamic_cast< GmGenerDistPositionDirection2DCorrelGaussian*>(theDirectionDistribution) != 0 || 
      dynamic_cast< GmGenerDistPositionDirection2DCorrelDoubleGaussian*>(theDirectionDistribution) ) { 
    GmVGenerDistDirection* dirDist = dynamic_cast< GmVGenerDistDirection*>(theDirectionDistribution);
    dirDist->SetDirection( direction );
  } else {
    G4Exception("RTVPlanSource::MoveBeam",
		"",
		FatalException,
		G4String(" Direction distribution is not of type GmGenerDistDirectionConst/GmGenerDistDirectionCone/GmGenerDistDirectionCone2D/GmGenerDistDirectionConeGaussian/GmGenerDistDirectionCone2DGaussian/GmGenerDistPositionDirection2DCorrelGaussian/GmGenerDistPositionDirection2DCorrelDoubleGaussian").c_str());
  }

  if( theRTBeamG4PVs.size() != 0 ) {
    G4GeometryManager* geomMgr = G4GeometryManager::GetInstance();
    geomMgr->OpenGeometry();
  }
  //--- Move volumes associated to beam
  for( size_t ii = 0; ii < theRTBeamG4PVs.size(); ii++ ) {
    G4VPhysicalVolume* pv = theRTBeamG4PVs[ii];
    G4ThreeVector positionBV = theRTBeamPosition[ii];
    G4RotationMatrix rotationBV = theRTBeamRotation[ii];
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam BeamPV rotation original " << rotationBV << G4endl;
#endif
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam BeamPV position orig " << positionBV << G4endl;
#endif
    if( bsdata.RotAngleY != 0. ) {
      positionBV.rotateY(bsdata.RotAngleY);
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam BeamPV position rotated Gantry Angle " << bsdata.RotAngleY/CLHEP::deg << " " << positionBV << G4endl;
#endif
      rotationBV.rotateY(-bsdata.RotAngleY);
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam BeamPV rotation Gantry Angle " << bsdata.RotAngleY/CLHEP::deg << " " << rotationBV << G4endl;
#endif
    }
    if( bsdata.RotAngleZ != 0. ) { 
      G4ThreeVector direcPerp = direction.cross(G4ThreeVector(0.,1.,0.)); //??
      positionBV.rotate(bsdata.RotAngleZ,direcPerp); // GantryPitchAngle
      rotationBV.rotate(-bsdata.RotAngleZ,direcPerp); 
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam BeamPV position rotated Gantry Pitch Angle " << bsdata.RotAngleZ/CLHEP::deg << " " << positionBV << G4endl;
#endif
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::MoveBeam BeamPV rotation Gantry Pitch Angle " << bsdata.RotAngleZ/CLHEP::deg << " " << rotationBV << G4endl;
#endif
    }
    pv->SetTranslation(positionBV);
    G4RotationMatrix* rotationPV = pv->GetRotation();
    *rotationPV = rotationBV;
   //    G4RotationMatrix* rotationBVnew = new G4RotationMatrix(rotationBV);
    //    pv->SetRotation(rotationBVnew);
  }

  if( theRTBeamG4PVs.size() != 0 ) {
    G4GeometryManager* geomMgr = G4GeometryManager::GetInstance();
    geomMgr->CloseGeometry();
  }

}

//------------------------------------------------------------------------
void RTVPlanSource::PredictInitialDisplacement(const RTBeamStateData& bsdata )
{
  std::vector<G4double> finalPosPar = GmParameterMgr::GetInstance()->GetVNumericValue("RTVPlanSource::PredictInitialDisplacement FinalPosition", std::vector<G4double>());
  if( finalPosPar.size() != 3 ) return;
  
  G4ThreeVector finalPosition( finalPosPar[0] ,finalPosPar[1] ,finalPosPar[2] );


  //-- Get center of phantomContainer
  GmReadPhantomGeometry* phGeom = static_cast<GmReadPhantomGeometry*>(const_cast<G4VUserDetectorConstruction*>(theG4RunManager->GetUserDetectorConstruction()));
  if( !phGeom ) {
    G4Exception("RTVPlanSource::PredictInitialDisplacement",
		"",
		FatalException,
		"Geometry constructor has to be of type GmReadPhantomGeometry");
  }
  G4ThreeVector pcCenter((phGeom->GetMaxX()+phGeom->GetMinX())/2.,
			 (phGeom->GetMaxY()+phGeom->GetMinY())/2.,
			 (phGeom->GetMaxZ()+phGeom->GetMinZ())/2.);
  
  std::vector<G4VPhysicalVolume*> PVs = theGeomUtils->GetPhysicalVolumes("phantomContainer",1);
  G4VPhysicalVolume* pv = PVs[0];
  G4RotationMatrix initialRotMat = *(pv->GetRotation());

  G4ThreeVector isocenter( bsdata.IsocenterPosition.x(), bsdata.IsocenterPosition.y(), bsdata.IsocenterPosition.z());

  G4ThreeVector posSAD(0.,0.,bsdata.SourceAxisDistanceX);
  G4RotationMatrix rotMatGA;
  if( !bIsocenterAtZero ) {
    rotMatGA.rotateY(bsdata.RotAngleY);
  }
  
  G4ThreeVector isocLocal = (isocenter-pcCenter);
  isocLocal *= CLHEP::inverseOf(initialRotMat);
  isocLocal *= rotMatGA;
  //G4ThreeVector initialDispPREDICT = finalPosition - pcCenter - posSAD - (isocenter-pcCenter)*CLHEP::inverseOf(initialRotMat)*rotMatGA;
  G4ThreeVector initialDispPREDICT = finalPosition - pcCenter - posSAD - isocLocal; 
    
  //  G4cout << " initialDispPREDICT " << initialDispPREDICT << " TO MATCH " << finalPosition << " - " << pcCenter << " - " << posSAD << " - " << isocLocal << G4endl; //GDEB
  
}

//------------------------------------------------------------------------
void RTVPlanSource::CheckParamInAllControlPoints(G4String rtVName)
{
  std::vector<G4double> lastPos(2,0.);
  for( size_t ib = 0; ib < theBeams.size(); ib++ ) {
    RTVPlanBeam* beam = GetBeam( ib );
    //    G4cout << beam << " NB=" << theBeams.size() << " CheckParamInAllControlPoints " << ib << G4endl; //GDEB
    std::vector<RTVPlanControlPoint*> CPs = beam->GetControlPoints();
    for( size_t iicp = 0; iicp < CPs.size(); iicp++ ) {
      RTVPlanControlPoint* CP = CPs[iicp];
      //      G4cout << CP << " NB=" << theBeams.size() << " NCP=" << CPs.size() << " CheckParamInAllControlPoints " << ib << " " << iicp << G4endl; //GDEB
      for( int ipos = 1; ipos <= 2; ipos++ ) {
	G4String posName = "_"+GmGenUtils::itoa(ipos)+"_POS";
	if( CP->ParamExists(rtVName+posName) ) {
	  lastPos[ipos] = CP->GetParam(rtVName+posName,1);
	} else {
	  if( iicp != 0 ) {
	    G4Exception("RTVPlanSource::CheckParamInAllControlPoints",
			"",
			JustWarning,
			("Parameter  "+rtVName+posName+"  does not exist in Beam: "
			 +GmGenUtils::itoa(beam->GetIndex())+" ControlPoint: "+GmGenUtils::itoa(CP->GetIndex())+
			 " , taking its value from previous ControlPoint").c_str());
	    CP->SetParam(rtVName+posName,lastPos[ipos],false);
	  } else {
	    G4Exception("RTVPlanSource::CheckParamInAllControlPoints",
			"",
			FatalException,
			("Parameter  "+rtVName+posName+"  does not exist in Beam: "
			 +GmGenUtils::itoa(beam->GetIndex())+" ControlPoint: "+GmGenUtils::itoa(CP->GetIndex())+
			 " , taking its value from previous ControlPoint").c_str());
	  }
	}
      }
    }
  }

}

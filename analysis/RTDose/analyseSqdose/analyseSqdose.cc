//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  GAMOS software  is  copyright of the Copyright  Holders  of *
// * the GAMOS Collaboration.  It is provided  under  the  terms  and *
// * conditions of the GAMOS Software License,  included in the  file *
// * LICENSE and available at  http://fismed.ciemat.es/GAMOS/license .*
// * These include a list of copyright holders.                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GAMOS collaboration.                       *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the GAMOS Software license.           *
// ********************************************************************
//
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "G4GeometryManager.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"
#include "GamosCore/GamosScoring/Scorers/include/GmG4PSDoseDeposit.hh"
#include "GamosCore/GamosScoring/Scorers/include/GmG4PSDoseDepositVector.hh"
#include "RadioTherapy/include/RTPSPDoseHistos.hh"
#include "RadioTherapy/include/RTPSPDoseHistosVector.hh"
#include "RadioTherapy/include/RTVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "GamosCore/GamosRunManager/include/GmRunManager.hh"

#include "G4THitsMap.hh"
#include "G4UImanager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include <vector>
#include "G4HCofThisEvent.hh"

int main(int argc,char** argv) 
{
  if(argc < 2) { 
    G4Exception("analyseSqdose",
		"Wrong argument",
		FatalErrorInArgument,
		"YOU MUST SUPPLY AT LEAST ONE ARGUMENT: SQDOSE_FILE");
  }
  
  GmRunManager* runManager = new GmRunManager();
  runManager->SelectVerbosity("GmBaseVerbosity silent");

  G4String theFileName = "";
  G4String hisDoseMin = "-17";
  G4String hisDoseMax = "-12";
  G4String theFileNameOut = "dose_analyseSqdose";
  G4int nRead = -1;
  G4int theContainerType = 1;
  G4String theAllHistos2D;
  G4int nTotalVoxels = -1;

  if( argc == 2 ) {
    if( G4String(argv[1]) == "-help" ) {
      G4cout << "  -f    phase space file name (in this case do not use the file name alone as first argument as before)" << G4endl
	     << "  -NRead   number of particles to be read from the phase space file" << G4endl
	     << "  -fOut    output file name " << G4endl
	     << "  -fHistos    name of file with list of histograms " << G4endl
	     << "  -DoseMin    minimum limit of dose histograms" << G4endl
	     << "  -DoseMax    maximum limit of the dose histograms" << G4endl
	     << "  -cont    type of the STL container that will be used to store the doses and dose errors. It can be MAP or map, that uses a std::map; it is the default one, the one used by the Geant4 scorers. "
	     << " It can also be VECTOR or vector, that uses a std::vector; it occupies about ten times less than the std::map. The std::map container occupies a big amount of space, about 500 Mb for 10 million voxels, so we recommend you that you use std::vector if your phantom is big" << G4endl
	     << "  -NVoxels   total number of voxels in phantom (argument needed if sqdose file is of type FILLED" << G4endl
	     << "  -verb    verbosity: it sets the RTVerbosity. Default is warning, that will print the above lines; debug, that will print each particle read form the phase space file" << G4endl
	     << "  -help    prints the set of arguments" << G4endl;
      return 0;
    } else {
      theFileName = argv[1];
    }

  } else {
    if(argc%2 != 1) { 
    G4Exception("analyseSqdose",
		"Wrong argument",
		FatalErrorInArgument,
		"WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
    }
    for( G4int ii = 1; ii < argc; ii+=2 ){
      G4String argvstr = argv[ii];
      if( argvstr == "-f" ) {
	theFileName = argv[ii+1];
      } else if( argvstr == "-cont" ) {
	if( G4String(argv[ii+1]) == "MAP" || G4String(argv[ii+1]) == "map" ) {
	  theContainerType = 1;
	} else if( G4String(argv[ii+1]) == "VECTOR" || G4String(argv[ii+1]) == "vector" ) {
	  theContainerType = 2;
	} else {
	  G4Exception("analyseSqdose",
		      "Wrong container type",
		      FatalErrorInArgument,
		      G4String("Argument can be map or vector, it is"+G4String(argv[ii+1])).c_str());
	}

      } else if( argvstr == "-fOut" ) {
	theFileNameOut = argv[ii+1];
      } else if( argvstr == "-fHistos" ) {
	G4UImanager::GetUIpointer()->ApplyCommand("/gamos/setParam analyseSqdose:HistosFileName "+G4String(argv[ii+1]));

      } else if( argvstr == "-AllHistos2D" ) {
	theAllHistos2D += " " + G4String(argv[ii+1]);

      } else if( argvstr == "-DoseMin" ) {
	hisDoseMin = argv[ii+1];
  
      } else if( argvstr == "-DoseMax" ) {
	hisDoseMax = argv[ii+1];

      } else if( argvstr == "-NRead" ) {
	nRead = GmGenUtils::GetInt(argv[ii+1]);

      } else if( argvstr == "-NVoxels" ) {
	nTotalVoxels = GmGenUtils::GetInt(argv[ii+1]);

      } else if( argvstr == "-verb" ) {
	G4String verbstr = "RTVerbosity " + G4String(argv[ii+1]);
	runManager->SelectVerbosity(verbstr);

      } else {
	G4Exception("analyseSqdose",
		    "Wrong argument",
		    FatalErrorInArgument,
		    G4String("Argument can be -f -hf -t, it is"+argvstr).c_str());
      }
    }
  }

  G4UImanager::GetUIpointer()->ApplyCommand("/gamos/setParam analyseSqdose:FileName "+theFileNameOut);
  G4UImanager::GetUIpointer()->ApplyCommand("/gamos/setParam "+theFileNameOut+":NormalizeToNEvents 0");
  if( theAllHistos2D != "" ) G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/setParam RTPSPDoseHistos:AllHistos2D ")+theAllHistos2D);
   G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/analysis/histo1Min *RTPSPDoseHistos:Dose*  ")+hisDoseMin);
   G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/analysis/histo1Max *RTPSPDoseHistos:Dose*  ")+hisDoseMax);
  G4UImanager::GetUIpointer()->ApplyCommand("/gamos/setParam dose_analyseSqdose:NormalizeToNEvents 0");
  //  GmParameterMgr::GetInstance()->AddParam("RTPSPDoseHistos::NormalizeToNEvents 0",PTdouble); 

  if( RTVerb(warningVerb) ) G4cout << "READING FILE " << theFileName << G4endl;
  GmSqdose dose;
  dose = GmSqdose();
  dose.Read(theFileName);
  
  if(nRead == -1 ) nRead = dose.GetDoses().size();

  G4cout << " NREAD " << nRead << G4endl;
  //--- create RunMap and PrimitiveScorer with dose squared
  if( theContainerType == 1 ) {
    if( RTVerb(warningVerb) ) G4cout << " USING std::map to store doses" << G4endl;
    G4THitsMap<G4double>* theRunMap = new G4THitsMap<G4double>;
    //----- Create primitive scorer
    GmG4PSDoseDeposit* doseScorer = new GmG4PSDoseDeposit("analyseSqdose_DoseDepositScorer");
    //  doseScorer->SetSumV2( theSumV2 );
    
    std::map<G4int,G4double>* theSumV2 = doseScorer->GetSumV2();
    SqdoseType sqdoseType = dose.GetType();
    GmSqdoseHeader* doseHeader = dose.GetHeader();
    G4int nVoxelsX = doseHeader->GetNoVoxelsX();
    G4int nVoxelsY = doseHeader->GetNoVoxelsY();
    G4int nVoxelsXHalf = nVoxelsX/2;
    G4int nVoxelsYHalf = nVoxelsY/2;
    G4int nVoxelsXY = nVoxelsX * nVoxelsY;
    //    G4int nVoxelsZ = doseHeader->GetNoVoxelsZ();
    G4int nVoxelsInXYPlane = nVoxelsX + nVoxelsY - 1;
    for( G4int ii = 0; ii < nRead; ii++){
      if( sqdoseType == SqTCROSS_PHANTOM ) {
	G4int id = 0;
	G4int nx = 0;
	G4int ny = 0;
	G4int nz = ii / nVoxelsInXYPlane;
	if( ii%nVoxelsInXYPlane < nVoxelsYHalf ) { // first YN
	  nx = nVoxelsXHalf;
	  ny = ii%nVoxelsInXYPlane % nVoxelsYHalf;
	} else if( ii%nVoxelsInXYPlane < nVoxelsYHalf+nVoxelsX ) { // second X
	  nx = (ii%nVoxelsInXYPlane-nVoxelsYHalf) % nVoxelsX;
	  ny = nVoxelsYHalf;
	} else { // third YP
	  nx = nVoxelsXHalf;
	  ny = (ii%nVoxelsInXYPlane-nVoxelsYHalf-nVoxelsX) % nVoxelsYHalf + nVoxelsYHalf+1;
	}
	id = nx + nVoxelsX * ny + nVoxelsXY * nz;
	//	G4cout << "VOXEL READ " << id << " " << nx << " " << ny << " " << nz << G4endl; //GDEB
	G4double val  = dose.GetDose(ii);
	if( val != 0. ){
	  //	  val *= 1.E-12;
	  theRunMap->add(id,val);
	  (*theSumV2)[id] = dose.GetDoseSq(ii) * 1;
	}	
      } else {
	G4double val  = dose.GetDose(ii);
	if( val != 0. ){
	  //	  val *= 1.E-12;
	  theRunMap->add(ii,val);
	  (*theSumV2)[ii] = dose.GetDoseSq(ii) * 1;
	  if( ii%1000000 == 1 )  G4cout << ii << " voxel read. val= " << val << " val2 " << (*theSumV2)[ii] << G4endl;
	  //    *((*theRunMap)[ii]) = dose.GetDose(ii);
	  //    theRunMap->push_back(EvtMap);
	}
      }
    }
    
    if( RTVerb(warningVerb) ) G4cout << " Number of voxels= " << theRunMap->entries() << G4endl;
    
    GmNumberOfEvent::SetNumberOfEvent( dose.GetHeader()->GetNumberOfEvents() );
    //--- calculate errors
    doseScorer->CalculateErrors( theRunMap );
    
    //----- Create PS printer
    RTPSPDoseHistos* doseHistos = new RTPSPDoseHistos("analyseSqdose",dose.GetHeader());
    //  doseHistos->SetScorer( doseScorer );
    //----- Call PS printer
    doseHistos->DumpAll( theRunMap, doseScorer );
    
  } else {
    if( RTVerb(warningVerb) ) G4cout << " USING std::vector to store doses " << G4endl;
    std::vector<G4double>* theRunMap = new std::vector<G4double>;
    //----- Create primitive scorer
    GmG4PSDoseDepositVector* doseScorer = new GmG4PSDoseDepositVector("analyseSqdose_DoseDepositScorer");
    std::vector<G4double>* theSumV2 = doseScorer->GetSumV2();

   
    if( theContainerType == 2 && nTotalVoxels == -1 ) {
    G4Exception("analyseSqdose",
		"Wrong argument",
		FatalErrorInArgument,
		"Container type is 'vector' and number of voxels is not set. Use -NVoxels to reserve a number of voxels equal or bigger than the number of voxels filled in the sqdose file"); 
    }
    for( G4int ii = 0; ii < nTotalVoxels; ii++ ) {
      theRunMap->push_back(0.);
    }
    doseScorer->SetNIndices(nTotalVoxels);
    doseScorer->Initialize((G4HCofThisEvent*)(0));
  
    for( G4int ii = 0; ii < nRead; ii++){
      G4double val  = dose.GetDose(ii);
      if( val != 0. ){
	//	val *= 1.E-12;
	(*theRunMap)[ii] = val;
	(*theSumV2)[ii] = dose.GetDoseSq(ii) * 1.;
	if( ii%1000000 == 1 )  if( RTVerb(debugVerb) ) G4cout << ii << " fill theRunMap val " << val << " VAL2 " << (*theSumV2)[ii] << " " << theRunMap->size() <<G4endl;
	//    *((*theRunMap)[ii]) = dose.GetDose(ii);
	//    theRunMap->push_back(EvtMap);
      }
    }

    if( RTVerb(warningVerb) ) G4cout << " Number of entries= " << theRunMap->size() << G4endl;
    //    std::map<G4int,G4double*>::iterator ite;
  //  for(ite = theRunMap->GetMap()->begin(); ite != theRunMap->GetMap()->end(); ite++){
  //       G4double sumX = (*(ite->second));
  //       G4cout << ite->first << " anaDOSE " << sumX << G4endl;
  //}
    
    GmNumberOfEvent::SetNumberOfEvent( dose.GetHeader()->GetNumberOfEvents() );
    //--- calculate errors
    doseScorer->CalculateErrors( theRunMap );
    
    //----- Create PS printer
    RTPSPDoseHistosVector* doseHistos = new RTPSPDoseHistosVector("analyseSqdose",dose.GetHeader());
    //  doseHistos->SetScorer( doseScorer );
    //----- Call PS printer
    doseHistos->DumpAllVector( theRunMap, doseScorer );

  }

  GmAnalysisMgr::DeleteInstances();
  GmParameterMgr::GetInstance()->CheckParametersUsage();

  //  if( RTVerb(warningVerb) ) G4cout << " N EVENTS IN SOURCE " <<  dose.GetHeader()->GetNumberOfEvents()  << G4endl;

  return 0;
}


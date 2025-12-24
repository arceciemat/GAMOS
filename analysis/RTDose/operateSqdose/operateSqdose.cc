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

#include "G4GeometryManager.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"

#include "G4tgrUtils.hh"

G4String theOperation;
std::vector<G4String> theFileInNames;
G4String theFileOutName;

void DoFilteringOut();
void DoArithmeticOperation();
bool applyOperator(G4double lhs, G4String op, G4double rhs);
int verbose = 1;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv) 
{
  G4tgrUtils::bAllowErrors = true;
  
  theOperation = argv[1];
  theFileOutName =  argv[2];
  if( verbose >= 1 ) G4cout << " theOperation " << theOperation << G4endl;

  for( G4int ii = 3; ii < argc; ii++){
    theFileInNames.push_back(argv[ii]);
  }

  if( theOperation.substr(0,10) != "filterOut=" ) {
    if(argc < 5 ) {    
      G4Exception("sumSqdose",
		  "DoArithmeticOperation ",
		  FatalErrorInArgument,
		  "YOU MUST SUPPLY AT LEAST FOUR ARGUMENTS: OPERATION SQDOSE_OUT SQDOSE1 SQDOSE2 ... SQDOSEn");
    }
    DoArithmeticOperation();
    
  } else {
    if(argc != 4 ) {    
      G4Exception("sumSqdose",
		  "DoFilteringOut",
		  FatalErrorInArgument,
		  "YOU MUST SUPPLY THREE ARGUMENTS: OPERATION SQDOSE_OUT SQDOSE1 ");
    }
    theOperation = theOperation.substr(10);
    DoFilteringOut();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DoArithmeticOperation()
{
  G4int nFilesInOper = 0;
  for( G4int ii=1; ; ii++ ) {
    if( theOperation.find("X"+GmGenUtils::itoa(ii)) != std::string::npos ) {
      nFilesInOper ++;
      if( verbose >= 1 ) G4cout << ii << " operateSqdose " << nFilesInOper << " " << theFileInNames.size() << G4endl; 
    } else {
      if( verbose >= 1 ) G4cout << ii << " " << "X"+GmGenUtils::itoa(ii) << " operateSqdose " << nFilesInOper << " " << theFileInNames.size() << G4endl;
      break;
    }
  }
  G4int nFilesInArguments = theFileInNames.size();
  if( nFilesInOper != nFilesInArguments ) { 
	  G4Exception("operateSqdose",
		      "Wrong type of output file (third argument)",
		      FatalErrorInArgument,
		      "Number of sqdose files must be the same that number of Xi in operation");   
  }


  FILE* fout  = fopen(theFileOutName,"wb");

  std::vector<GmSqdose*> theSqdoseFiles;
  std::vector<std::vector<float>> theDoseValues;
  std::vector<float> theDoseSqs;
  size_t nVoxels = 0;
  SqdoseType sqtype = SqTALL;

  for( size_t ii = 0; ii < theFileInNames.size(); ii++){
    if( verbose >= 1 ) G4cout << "operateSqdose: Reading file " << theFileInNames[ii] << G4endl;
    GmSqdose* sqdose = new GmSqdose();
    sqdose->Read(theFileInNames[ii]);
    theSqdoseFiles.push_back(sqdose);
    std::vector<float> doses = sqdose->GetDoses();
    if( verbose >= 1 ) G4cout << "operateSqdose: Read file " << theFileInNames[ii] << " NDOSES " << doses.size() << G4endl;
    theDoseValues.push_back(doses);
    if( ii == 0 ) {
      nVoxels = doses.size();
      theDoseSqs = sqdose->GetDoseSqs(); 
      sqtype = sqdose->GetSqdoseType();
    } else {
      if( nVoxels != doses.size() ) {
	G4ExceptionDescription msg;
	msg << "Two sqdose files with different number of voxels: "
	    << theFileInNames[0] <<": " << GmGenUtils::itoa(nVoxels)
	    << theFileInNames[ii] <<": " << GmGenUtils::itoa(doses.size()) << G4endl;
	G4Exception("operateSqdose",
		    "",
		    FatalErrorInArgument,
		    msg);
      }
      if( sqtype != sqdose->GetSqdoseType() ) {
	G4ExceptionDescription msg;
	msg << "Two sqdose files with different type: "
	    << theFileInNames[0] <<": " << theSqdoseFiles[0]->GetTypeStr()
	    << theFileInNames[ii] <<": " << sqdose->GetTypeStr() << G4endl;
	G4Exception("operateSqdose",
		    "",
		    FatalErrorInArgument,
		    msg);
      }
    }
  }

  //---- Initialize sqdose out with 1st sqdose (any would serve) 
  GmSqdose sqdoseOut;
  GmSqdose* sqdose1st = theSqdoseFiles[0];
  GmSqdoseHeader* sqdoseOutHeader = sqdose1st->GetHeader();
  sqdoseOut.SetHeader(sqdoseOutHeader);
  std::vector<float> doses1st = sqdose1st->GetDoses(); 
  sqdoseOut.SetDoses( doses1st );
  std::vector<float> doseSqs1st = sqdose1st->GetDoses(); 
  sqdoseOut.SetDoseSqs( doseSqs1st );
  std::vector<float> dosesOut = sqdoseOut.GetDoses();
  SqdoseType sqtypeOut = sqdose1st->GetSqdoseType();
  sqdoseOut.SetSqdoseType(sqtypeOut);
    
  //----- Loop to voxels and fill using the operation
  for( size_t ii =0; ii < nVoxels; ii++ ) {
    G4String operSubs = theOperation;
    for( G4int jj=0; jj < nFilesInOper; jj++ ) {
      G4String xstr = "X"+GmGenUtils::itoa(jj+1);
      G4String doseStr = GmGenUtils::ftoa(theDoseValues[jj][ii]);
      operSubs = GmGenUtils::ReplaceAll(operSubs, xstr, doseStr);
      // G4cout << ii << jj << " operSubs.replace " << operSubs << " " << xstr << " -> " << doseStr <<G4endl;
    }
    G4double newDose = GmGenUtils::GetValue(operSubs);
    /*    if (std::isnan(newDose)) {
      newDose = 0.0;  
    }
    if (std::isinf(newDose)) {
      newDose = 0.0;  
      } */
    dosesOut[ii] = newDose;
    if( verbose >= 3 && dosesOut[ii] != 0. ) G4cout << ii << " " <<operSubs << " dosesOut " << dosesOut[ii] << G4endl;
  }

  sqdoseOut.SetDoses(dosesOut);
  sqdoseOut.Print(fout);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DoFilteringOut()
{

  //--- Separate filters
  std::vector<G4String> theFilters;
  /*  std::stringstream ss(theOperation);
  std::string item;
  while (std::getline(ss, item, '&')) {
    theFilters(item);
    }*/
  size_t istart = 0;
  size_t ipos;
  std::vector<G4String> theOperatorsBetweenFilters;
  while ((ipos = theOperation.find('&', istart)) != std::string::npos || (ipos = theOperation.find('|', istart)) != std::string::npos) {
        theFilters.push_back(theOperation.substr(istart, ipos - istart));
        istart = ipos + 1;
	theOperatorsBetweenFilters.push_back(theOperation.substr(ipos,1));
	if( verbose >= 1 ) G4cout<< "DoFilteringOut theOperatorsBetweenFilters " << theOperation.substr(ipos,1) << " N=" << theOperatorsBetweenFilters.size() << G4endl;
  }
  theFilters.push_back(theOperation.substr(istart)); // last part

  std::vector<G4String> theFilterTexts;
  std::vector<G4String> theFilterOperators;
  std::vector<G4double> theFilterLimits;
  //--- Each filter must have a >, < or =
  for( G4String filt : theFilters ) {
    if( verbose >= 1 ) G4cout << " FILTER LOOP " << filt << G4endl;
    G4int operLength = -1;
    size_t iop = filt.find("<=");;
    if( iop != std::string::npos ) {
      operLength = 2;
    } else {
      iop = filt.find("<");
      if( iop != std::string::npos ) {
	operLength = 1;
      } else {
	iop = filt.find("<");
	if( iop != std::string::npos ) {
	  operLength = 1;
	} else {
	  iop = filt.find("=");
	  if( iop != std::string::npos ) {
	    operLength = 1;
	  } else {
	    iop = filt.find("==");
	    if( iop != std::string::npos ) {
	      operLength = 2;
	    } else {
	      iop = filt.find(">");
	      if( iop != std::string::npos ) {
		operLength = 1;
	      } else {
		iop = filt.find(">=");
		if( iop != std::string::npos ) {
		  operLength = 2;
		} else {
		  G4Exception("operateSqdose",
			      "DoFilteringOut",
			      FatalErrorInArgument,
			      ("Operation does not contain an operator <, <=, =, ==, >=, > "+theOperation).c_str());
		}
	      }
	    }
	  }
	}
      }
    }
    theFilterTexts.push_back(filt.substr(0,iop));
    theFilterOperators.push_back(filt.substr(iop,operLength));
    theFilterLimits.push_back(GmGenUtils::GetValue(filt.substr(iop+operLength,9999)));
    if( verbose >= 1 ) G4cout<< "DoFilteringOut  theFilterTexts " << filt.substr(0,iop) << " theFilterOperators " << filt.substr(iop,operLength)
	  << " theFilterLimits " << GmGenUtils::GetValue(filt.substr(iop+operLength,9999)) << " N=" << theFilterLimits.size() << G4endl;
  }
    
  G4int nFilesInArguments = theFileInNames.size();
  if ( nFilesInArguments != 1 ){
    G4Exception("operateSqdose",
		"DoFilteringOut",
		FatalErrorInArgument,
		"Filtering can only be done for one file");   
  }
  FILE* fout  = fopen(theFileOutName,"wb");

  if( verbose >= 1 ) G4cout << "operateSqdose: Reading file " << theFileInNames[0] << G4endl;
  GmSqdose* sqdose = new GmSqdose();
  sqdose->Read(theFileInNames[0]);
  std::vector<float> doses = sqdose->GetDoses();
  std::vector<float> doseSqs = sqdose->GetDoseSqs(); 
  if( verbose >= 1 ) G4cout << "operateSqdose: Read file " << theFileInNames[0] << " NDOSES " << doses.size() << G4endl;

  //---- Initialize sqdose out with 1st, and unique, sqdose 
  GmSqdose sqdoseOut;
  GmSqdoseHeader* sqdoseOutHeader = sqdose->GetHeader();
  sqdoseOut.SetHeader(sqdoseOutHeader);
  sqdoseOut.SetDoses( doses );
  sqdoseOut.SetDoseSqs( doseSqs );
  std::vector<float> dosesOut = sqdoseOut.GetDoses();
  sqdoseOut.SetSqdoseType(sqdose->GetSqdoseType());
    
  //----- Loop to voxels and fill using the filters 
  size_t nVoxels = doses.size();
  for( size_t ii =0; ii < nVoxels; ii++ ) {
    G4double dose = doses[ii];
    for( size_t jj = 0; jj < theFilterLimits.size(); jj++ ) {
      G4String operSubs = theFilterTexts[jj];
      G4String xstr = "X1";
      G4String doseStr = GmGenUtils::ftoa(dose);
      operSubs = GmGenUtils::ReplaceAll(operSubs, xstr, doseStr);
      if( verbose >= 3 ) G4cout << ii << ":" << jj << " operSubs.replace " << operSubs << " " << xstr << " -> " << doseStr <<G4endl;
      G4double newDose = GmGenUtils::GetValue(operSubs);
      G4String op = theFilterOperators[jj];
      G4double limit = theFilterLimits[jj];
      if( applyOperator( newDose, op, limit ) ) {
	newDose = 0.;
      }
      dosesOut[ii] = newDose;
      G4double newDose2 = GmGenUtils::GetValue(operSubs); 
      if( verbose >= 3  &&  newDose2 != 0. ) G4cout << ii << ":" << jj <<" " << newDose << " " << operSubs << " op " << op << " limit " << limit << " ORIG " << newDose2 << G4endl;
      
    }
    if( dosesOut[ii] != 0. ) G4cout << ii << " dosesOut " << dosesOut[ii] << G4endl;
  }

  sqdoseOut.SetDoses(dosesOut);
  sqdoseOut.Print(fout);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool applyOperator(G4double lhs, G4String op, G4double rhs) {
    if (op == "<")  return lhs < rhs;
    if (op == ">")  return lhs > rhs;
    if (op == "<=") return lhs <= rhs;
    if (op == ">=") return lhs >= rhs;
    if (op == "=" || op == "==") return lhs == rhs;
    return false; // unknown operator
}



#include "globals.hh"
#include <fstream>
#include "RadioTherapy/include/iaea_header.hh"
#include "RadioTherapy/include/iaea_record.hh"
#include "RadioTherapy/include/RTVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosRunManager/include/GmRunManager.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

using namespace CLHEP;

#include <random>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DumpHeaderStats( iaea_header_type *p_iaea_header1 );
void GetError( G4double& rat, G4double& err, G4double val1, G4double val2 );
void DumpParticle( iaea_record_type* part, std::ostream& out = G4cout );

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
int main(int argc,char** argv) 
{
  if(argc < 2) { 
    G4Exception("randomizePS",
		"Error",
		FatalErrorInArgument,
		"YOU MUST SUPPLY AT LEAST ONE ARGUMENT: PHASESPACE_FILE");
  }

  GmRunManager* runManager = new GmRunManager();
  runManager->SelectVerbosity("GmBaseVerbosity silent");

  G4String theFileName;
  G4String theFileNameOut = "phaseSpace";
  G4int nRead = -1;

  if( argc == 2 ) {
    if( G4String(argv[1]) == "-help" ) {
       G4cout << " -f    phase space file name (in this case do not use the file name alone as first argument as before)" << G4endl
	      << " -NRead   number of particles to be read from the phase space file" << G4endl
	      << " -fOut    output file name " << G4endl
	      << " -verb    verbosity: it sets the RTVerbosity" << G4endl
	      << " -help    prints the set of arguments " << G4endl;
       return 0;

     } else {
       theFileName = argv[1];
     }
  } else {
    if(argc%2 != 1) { 
      G4Exception("randomizePS",
		  "Error",
		  FatalErrorInArgument,
		  "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
     }
    for( G4int ii = 1; ii < argc; ii+=2 ){
      G4String argvstr = argv[ii];
      if( argvstr == "-f" ) {
	theFileName = argv[ii+1];
      } else if( argvstr == "-fOut" ) {
	theFileNameOut = argv[ii+1];
      } else if( argvstr == "-NRead" ) {
	nRead = GmGenUtils::GetInt(argv[ii+1]);
      } else if( argvstr == "-verb" ) {
	G4String verbstr = "RTVerbosity " + G4String(argv[ii+1]);
	runManager->SelectVerbosity(verbstr);
      }
    }
  }

  iaea_header_type *p_iaea_header;
  p_iaea_header = (iaea_header_type *) calloc(1, sizeof(iaea_header_type));
  if( theFileName.length() > 11 ) {
    if( theFileName.substr(theFileName.length()-11,11) == ".IAEAheader" ) theFileName = theFileName.substr(0,theFileName.length()-11);
  }
  if( RTVerb(warningVerb) ) G4cout << " READING FILE " << theFileName << G4endl;
  //---- Open header file
  G4String headerName = theFileName + ".IAEAheader";
  p_iaea_header->fheader = fopen(headerName,"rb");

  if( !p_iaea_header->fheader ) {
    G4Exception("randomizePS",
		"Error",
		FatalErrorInArgument,
		G4String("File not found :  " + theFileName + ".IAEAheader").c_str());
  }
  if( p_iaea_header->read_header() ) {
    G4Exception("randomizePS",
		"Error",
		FatalErrorInArgument,
		G4String("Error reading phase space file:  " + theFileName).c_str());
  }

  //----- Open record file
  FILE* p_rfile;
  G4String recordName = theFileName + ".IAEAphsp";
  p_rfile =  fopen(recordName,"rb");
  if( !p_rfile ) {
    G4Exception("randomizePS",
		"Error",
		FatalErrorInArgument,
		G4String("Error file not found :  " + recordName).c_str());
  }

  G4int theNParticlesInFile = p_iaea_header->nParticles;
  if( nRead == -1 ) nRead = theNParticlesInFile;
  if ( RTVerb(warningVerb) ) G4cout << " NPARTICLES TO BE READ " << nRead << std::endl;

  // Creating IAEA record and allocating memory for it
  iaea_record_type * p_iaea_record = (iaea_record_type *) calloc(1, sizeof(iaea_record_type));
  p_iaea_record->p_file = p_rfile;

  //set record data
  p_iaea_header->get_record_contents(p_iaea_record);

  if( RTVerb(warningVerb) ) DumpHeaderStats( p_iaea_header );
  std::vector<iaea_record_type *> theReadRecords;
  for( G4int ii = 0; ii < nRead; ii++ ){
    if( ii%1000000 == 0 ) G4cout << "Reading PHSP track " << ii << G4endl;
    int fileEnd = p_iaea_record->read_particle();
    // Creating IAEA record and allocating memory for it
    iaea_record_type *p_iaea_recordNew = (iaea_record_type *) calloc(1, sizeof(iaea_record_type));
    p_iaea_recordNew->copy(p_iaea_record);
    theReadRecords.push_back(p_iaea_recordNew);
    if( RTVerb(debugVerb) ) DumpParticle( p_iaea_record );
    if( fileEnd == FAIL) {
      fclose(p_rfile);
      break;
    }
    //    G4cout << ii << " READ RECORD " << p_iaea_record->energy << G4endl; //GDEB
 } 

  // open new header
  iaea_header_type * p_iaea_headerRAND = (iaea_header_type *) calloc(1, sizeof(iaea_header_type));
  char L_fnameOut[120];
  G4String fnameOut = theFileNameOut;
  if( fnameOut.length() > 11 ) {
    if( fnameOut.substr(fnameOut.length()-11,11) == ".IAEAheader" ) fnameOut = fnameOut.substr(0,fnameOut.length()-11);
  }
  strcpy(L_fnameOut,fnameOut.c_str());
  p_iaea_headerRAND->fheader = open_file(L_fnameOut,(char *)(".IAEAheader"),(char *)("wb"));
  // copy from old header
  p_iaea_headerRAND->copy( p_iaea_header );
  /*  for( int ii = 0; ii < 9; ii++ ) {
    G4cout << " REDCONT " << p_iaea_header->record_contents[ii] << " = " << p_iaea_headerRAND->record_contents[ii] << G4endl; //GDEB
  }
  G4cout << " NEW HEADER OPENED " << G4endl; //GDEB
  */
  if( RTVerb(warningVerb) ) DumpHeaderStats( p_iaea_headerRAND ); 
  
  //----- Open record file
  iaea_record_type *p_iaea_recordRAND = (iaea_record_type *) calloc(1, sizeof(iaea_record_type));
  char fiaeaphsp[81]={'\0'};;
  //strcpy(fiaeaphsp,"test");
  strcpy(fiaeaphsp,fnameOut.c_str());
  p_iaea_recordRAND->p_file = open_file(fiaeaphsp,(char *)(".IAEAphsp"),(char *)("wb"));
  // if(p_iaea_recordRAND->initialize() != OK) return(FALSE);

  // Store read/write logical block in the PHSP header
  //  if( p_iaea_headerRAND->set_record_contents(p_iaea_record) == FALSE) return FALSE;
  p_iaea_headerRAND->write_header();

  std::random_device rd;
  std::mt19937 grand(rd());
  std::shuffle(theReadRecords.begin(), theReadRecords.end(), grand);
  for( size_t ii = 0; ii < theReadRecords.size(); ii++ ){
    p_iaea_recordRAND->copy( theReadRecords[ii] );
    //    G4cout << " RECORD COPIED " <<p_iaea_recordRAND->energy << G4endl; //GDEB
    //    DumpParticle( p_iaea_recordRAND );
    p_iaea_recordRAND->write_particle();    
  }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DumpHeaderStats( iaea_header_type *p_iaea_header1 )
{
  long theNParticlesInFile = p_iaea_header1->orig_histories;
  G4cout << "ORIGINAL HISTORIES= " << theNParticlesInFile << G4endl;
  GmNumberOfEvent::SetNumberOfEvent(theNParticlesInFile);

  G4double val1, err1;
  GetError( val1, err1, p_iaea_header1->nParticles, theNParticlesInFile);
  G4cout << "N PARTICLES= " << val1*theNParticlesInFile << " PER EVENT= " << val1 << " +- " << err1 << G4endl;

  GetError( val1, err1, p_iaea_header1->particle_number[0], theNParticlesInFile);
  G4cout << "N GAMMAS= " << val1*theNParticlesInFile << " PER EVENT= " << val1 << " +- " << err1 << G4endl;

  GetError( val1, err1, p_iaea_header1->particle_number[1], theNParticlesInFile);
  G4cout << "N ELECTRONS= " << val1*theNParticlesInFile << " PER EVENT " << val1 << " +- " << err1 << G4endl;

  GetError( val1, err1, p_iaea_header1->particle_number[2], theNParticlesInFile);
  G4cout << "N POSITRONS= " << val1*theNParticlesInFile << " PER EVENT= " << val1 << " +- " << err1 << G4endl;

  for( int ii = 0; ii < MAX_NUM_PARTICLES; ii++ ) {
    G4cout << ii << " averageKineticEnergy " << p_iaea_header1->averageKineticEnergy[ii] << G4endl;
    G4cout << ii << " sumParticleWeight " << p_iaea_header1->sumParticleWeight[ii] << G4endl;
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GetError( G4double& rat, G4double& err, G4double val1, G4double val2 )
{

  rat = val1/val2;
  if( val1 == 0 ) {
    err = 0.; 
  } else {
    err = rat*sqrt(1./val1 + 1./val2 );
  }
  // err *= sqrt( err * (1-err) );

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DumpParticle( iaea_record_type* part, std::ostream& out )
{
  out << "NEW PARTICLE: "<< G4endl;
  out << " PARTICLE_TYPE= " << part->particle<< G4endl;
  out << " ENERGY= " << part->energy<< G4endl; 
  out << " IS_NEWH_ISTORY= " << part->IsNewHistory<< G4endl;
  out << " POSITION X/Y/Z= " << part->x << " " << part->y << " " << part->z<< G4endl;
  out << " DIRECTION U/V/W= " << part->u << " " << part->v << " " << part->w<< G4endl;
  out << " IWEIGHT= " << part->iweight << G4endl;
  out << " WEIGHT= " << part->weight<< G4endl;
  out << "N_EXTRAFLOAT = " << part->iextrafloat<< G4endl;
  for( int ii = 0; ii < part->iextrafloat; ii++ ){
    out << "EXTRAFLOAT " << ii << "= " << part->extrafloat[ii]<< G4endl;
  }
  out << "N_EXTRALONG = " << part->iextralong << G4endl;  
  for( int ii = 0; ii < part->iextralong; ii++ ){
    out << "EXTRALONG " << ii << "= " << part->extralong[ii]<< G4endl;
  }
}

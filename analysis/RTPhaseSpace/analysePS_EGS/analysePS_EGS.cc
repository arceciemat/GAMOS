#include "globals.hh"
#include <fstream>
#include "RadioTherapy/include/iaea_header.hh"
#include "RadioTherapy/include/iaea_record.hh"
//#include "RadioTherapy/include/iaeaUtilities.hh"
#include "RadioTherapy/include/RTPhaseSpaceHistos.hh"
#include "RadioTherapy/include/EGSPhspParticle.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4UImanager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void printStat( iaea_header_type *p_iaea_header1 );
void GetError( G4double& rat, G4double& err, G4double val1, G4double val2 );

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
int main(int argc,char** argv) 
{

  if(argc<3) { 
    G4Exception("analysePS_EGS",
		"Error",
		FatalErrorInArgument,
		"!!! FATAL ERROR: YOU MUST SUPPLY AT LEAST TWO ARGUMENTS: PHASESPACE_FILE PHASESPACE_Z");
  }


  G4String theFileName;
  G4String hisEMax = "10.*MeV";
  G4String hisRMax = "100.*mm";
  G4String hisNbins = "100";
  G4String theFileNameOut = "phaseSpace";
  G4int nPartLoop = -1;
  G4double thePosZ = DBL_MAX;

  if( argc == 2 ) {
    theFileName = argv[1];
  } else {
    if(argc%2 != 1) { 
      G4Exception("analysePS_EGS",
		  "Error",
		  FatalErrorInArgument,
		  "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
    }
    for( G4int ii = 1; ii < argc; ii+=2 ){
      G4String argvstr = argv[ii];
      if( argvstr == "-f" ) {
	theFileName = argv[ii+1];
      } else if( argvstr == "-posZ" ) {
        thePosZ = GmGenUtils::GetValue(argv[ii+1]);
      } else if( argvstr == "-EMax" ) {
        hisEMax = argv[ii+1];
      } else if( argvstr == "-RMax" ) {
        hisRMax = argv[ii+1];
      } else if( argvstr == "-NBins" ) {
        hisNbins = argv[ii+1];
      } else if( argvstr == "-fOut" ) {
        theFileNameOut = argv[ii+1];
      } else if( argvstr == "-NPart" ) {
        nPartLoop = GmGenUtils::GetInt(argv[ii+1]);
      }
    }
  }
  G4cout << " READING FILE " << theFileName << G4endl;
  FILE* phspfile = fopen(theFileName.c_str(), "rb");	

  if( thePosZ == DBL_MAX) { 
    G4Exception("analysePS_EGS",
		"Error",
		FatalErrorInArgument,
		"YOU MUST SUPPLY ALWAYS -posZ ARGUMENT");
  }

  char mode[6];
  fread(mode,5*sizeof(char),1,phspfile);
  mode[5]='\0';
  
  int mode_n;
  if(mode[4] == '0')
    mode_n = 0;
  else
    mode_n = 2;

  std::cout << " MODE = " << mode[4] << " => " << mode_n << std::endl;

  //  int matrixInd1, matrixInd2;  //these are the matrix indexes
  iaea_header_type *p_iaea_header;
  p_iaea_header = (iaea_header_type *) calloc(1, sizeof(iaea_header_type));
  
  fread(&(p_iaea_header->nParticles),sizeof(int),1,phspfile);
  fread(&(p_iaea_header->particle_number[0]),sizeof(int),1,phspfile);
  std::cout << " N_PARTICLES " << p_iaea_header->nParticles
	    << " N_PHOTONS " << p_iaea_header->particle_number[0]
	    << std::endl;
  fread(&(p_iaea_header->maximumKineticEnergy[0]),sizeof(float),1,phspfile);
  fread(&(p_iaea_header->minimumKineticEnergy[0]),sizeof(float),1,phspfile);
  float p_incident;
  fread(&p_incident,sizeof(float),1,phspfile);
  std::cout << " MAX_KINE " << p_iaea_header->maximumKineticEnergy[0] 
	    << " MIN_KINE " << p_iaea_header->minimumKineticEnergy[0] 
	    << " P_INCIDENT " << p_incident 
	    << std::endl;
  /*  fread(&n_of_particles,sizeof(int),1,phspfile);
  fread(&n_of_photons,sizeof(int),1,phspfile);
  fread(&energy_max,sizeof(float),1,phspfile);
  fread(&energy_min,sizeof(float),1,phspfile);
  fread(&p_incident,sizeof(float),1,phspfile);
  */

  //  long file_pointer = 5*sizeof(char)+2*sizeof(int)+3*sizeof(float)+3;

  int count_particles = 0;

  RTPhaseSpaceHistos* theHistos = 0;

  GmParameterMgr::GetInstance()->PrintParametersUsage(2);

  G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/analysis/histo1Max PhaseSpace*Energy* ")+hisEMax);
  G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/analysis/histo2MaxY PhaseSpace*Energy* ")+hisEMax);
  G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/analysis/histo1Max PhaseSpace*X*at* ")+hisRMax);
  G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/analysis/histo1Max PhaseSpace*Y*at* ")+hisRMax);
  G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/analysis/histo1Max PhaseSpace*Z*at* ")+hisRMax);
  G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/analysis/histo2MaxX PhaseSpace*XY*at* ")+hisRMax);
  G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/analysis/histo2MaxY PhaseSpace*XY*at* ")+hisRMax);
  G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/analysis/histo2MaxX PhaseSpace*R*vs* ")+hisRMax);
  G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/analysis/histo2MaxX PhaseSpace*X*vs* ")+hisRMax);
  G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/analysis/histo1Nbins PhaseSpace* ")+hisNbins);
  G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/analysis/histo2NbinsX PhaseSpace* ")+hisNbins);
  G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/analysis/histo2NbinsY PhaseSpace* ")+hisNbins);
  G4UImanager::GetUIpointer()->ApplyCommand(G4String("/gamos/setParam RTPhaseSpaceHistos:FileName ")+theFileNameOut);
  G4UImanager::GetUIpointer()->ApplyCommand("/gamos/setParam phaseSpace:NormalizeToNEvents 0");

  EGSPhspParticle* particle = new EGSPhspParticle(phspfile,mode);
  int nParticles = p_iaea_header->nParticles;
  if( nPartLoop == 1 ) nPartLoop = nParticles;
  std::cout << " NPARTICLES TO BE READ " << nPartLoop << std::endl;

  for(long irecord=0; irecord<nPartLoop; irecord++){
    if( irecord%1000000 == 0 ) std::cout << "Reading PHSP track " << irecord << std::endl;
    int iError = particle->RefreshParticle(phspfile,irecord+1);
    //    std::cout << irecord << " PARTICLE " << particle->theLatch 
    //	      << " " << particle->theEnergy << " " << particle->thePosX << " " << particle->thePosY << " " << particle->theDirU << " " << particle->theDirV << " " << particle->theWeight << std::endl;
    if(!iError){
      /*  float xcorr = (particle->thePosX - theXmin)/theDeltaX;
	  float ycorr = (particle->thePosY - theYmin)/theDeltaY;
	  if(particle->theCharge == 0){
	  count_photons++;
	  }*/
      count_particles++;
    } else {
      break;
    }
    
    if( !theHistos ) theHistos  = new RTPhaseSpaceHistos(thePosZ ); 
    theHistos->FillHistos( particle, thePosZ );

    }
    
  G4cout << " NPARTICLES READ " << count_particles << G4endl;
  /*
    if(write==1){
    fclose(phspfile);
    n_of_particles=count_particles;
    n_of_photons=count_photons;
    p_incident=n;
    }
    
    if(write==0){
    mat -> save_matrix(firstname+"MATRIX");
    mat -> colorscale("colors");
    mat -> plot_matrix(firstname+"MATRIX","colors");
  }
  
  */

//  printStat( p_iaea_header );


  GmAnalysisMgr::DeleteInstances();
  GmParameterMgr::GetInstance()->CheckParametersUsage();

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void printStat( iaea_header_type *p_iaea_header1 )
{
  long theNParticlesInFile = p_iaea_header1->orig_histories;
  G4cout << "ORIGINAL HISTORIES= " << theNParticlesInFile << G4endl;
  GmNumberOfEvent::SetNumberOfEvent(theNParticlesInFile);

  G4double val1, err1;
  GetError( val1, err1, p_iaea_header1->nParticles, theNParticlesInFile);
  G4cout << "PARTICLES PER EVENT= " << val1 << " +- " << err1 << G4endl;


  GetError( val1, err1, p_iaea_header1->particle_number[0], theNParticlesInFile);
  G4cout << "GAMMAS PER EVENT= " << val1 << " +- " << err1 << G4endl;

  GetError( val1, err1, p_iaea_header1->particle_number[1], theNParticlesInFile);
  G4cout << "ELECTRONS PER EVENT " << val1 << " +- " << err1 << G4endl;

  GetError( val1, err1, p_iaea_header1->particle_number[2], theNParticlesInFile);
  G4cout << "POSITRONS PER EVENT= " << val1 << " +- " << err1 << G4endl;

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



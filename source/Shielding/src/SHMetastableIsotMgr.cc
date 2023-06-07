#include "SHMetastableIsotMgr.hh"
#include "ShieldingVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosGenerator/include/GmGeneratorMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#include "G4Track.hh"
#include "G4Gamma.hh"
#include "G4Ions.hh"

//----------------------------------------------------------------------
SHMetastableIsotMgr* SHMetastableIsotMgr::theInstance = 0;

//----------------------------------------------------------------------
SHMetastableIsotMgr* SHMetastableIsotMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new SHMetastableIsotMgr;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
SHMetastableIsotMgr::SHMetastableIsotMgr()
{
  ReadData();

}

 //-----------------------------------------------------------------------
void SHMetastableIsotMgr::ReadData()
{
  G4String fileName = GmParameterMgr::GetInstance()->GetStringValue("SHMetastableIsotMgr:FileName","metastableIsotopes.lis");

  fileName = GmGenUtils::FileInPath( fileName );
  GmFileIn fin = GmFileIn::GetInstance(fileName);

  //check that it corresponds to a level in G4RADIOACTIVEDATA 
  char* pathrd = getenv( "G4RADIOACTIVEDATA" );
  if( !pathrd ) {
    G4cerr << "Radioactive decay data are needed."<<G4endl
	   << "Please set the environmental variable G4RADIOACTIVEDATA"<<G4endl
	   << "to point to your RadiativeDecay directory."<<G4endl
	   << "Data are available from the Geant4 download page."<<G4endl<<G4endl;
    G4Exception("SHMetastableIsotMgr::ReadData",
		"Error",
		FatalException,
		"");
  }
  
  std::vector<G4String> wl;
#ifndef WIN32
	GmGenUtils::SetCheckTimeUnits(1);
#endif
  for( ;; ){
    G4cout << " SHMetastableIsotMgr::ReadData() " << fileName << G4endl;
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl.size() != 4 ) {
      G4Exception("SHMetastableIsotMgr::ReadData",
                  "Error",
		  FatalErrorInArgument,
                  G4String("Wrong number of words in file " + fileName + " line number = " + GmGenUtils::itoa(fin.Nline()) + ". There must be 4 words, there are "+ GmGenUtils::itoa(wl.size())).c_str());
    } 
    // Sc44m 270.95*keV 58.61*hour 0.988

    MetastableData md;
    md.name = wl[0];
    md.energy = GmGenUtils::GetValue(wl[1]);
    md.halflife = GmGenUtils::GetValue(wl[2]);
    md.branchingRatio = GmGenUtils::GetValue(wl[3]);
    
    // convert name to Z and and A
    G4int nl = md.name.length();
    if( md.name[nl-1] != 'm' ) {
      G4Exception("SHMetastableIsotMgr::ReadData",
		  "Wrong argument",
		  FatalErrorInArgument,
		  G4String("Name must finish with 'm', it is " + md.name).c_str());
    }

    int inum = 0;
    G4int ii = 0; 
    for( ; ii < nl-1; ii++ ){
      if( GmGenUtils::IsNumber(md.name.substr(ii,1)) ) {
	inum = ii;
	break;
      }
    }
    if( ii == nl ) {
      G4Exception("SHMetastableIsotMgr::ReadData",
		  "Wrong argument",
		  FatalErrorInArgument,
		  G4String("Name must have the atomic mass, it is " + md.name).c_str());
    }
    G4int Z = GmG4Utils::GetElementZFromName( md.name.substr(0,inum) );
    G4int A = GmGenUtils::GetInteger( md.name.substr(inum,nl-3) );

    //-- Read file looking for line starting with P:   z25.a52: P     377.7490   1.2660e+03
    //-- find closest isomere
    GmFileIn finrd = GmFileIn::GetInstance(G4String(pathrd)+"/z"+GmGenUtils::itoa(Z)+".a"+GmGenUtils::itoa(A));
    G4double closestEnergy = 0.;
    for( ;; ){
      if(! finrd.GetWordsInLine( wl ) ) break;
      if( wl.size() == 3 && wl[0] == "P" ) {
	G4double energy = GmGenUtils::GetValue(wl[1])*CLHEP::keV;
	if( fabs(energy-md.energy) < fabs(closestEnergy-md.energy) ){
	  closestEnergy = energy;
	}
      }
    }
    if( closestEnergy - md.energy != 0. ) {
      G4Exception("SHMetastableIsotMgr::ReadData",
                  "Warning",
		  JustWarning,
		  G4String("Excitation energy not found in G4RADIOACTIVEDATA, please check it \n. Looking at isotope " + md.name + " with energy " + GmGenUtils::ftoa(md.energy) + " closest energy is " + GmGenUtils::ftoa(closestEnergy) + " ; difference is " + GmGenUtils::ftoa(closestEnergy-md.energy)).c_str());
    }

    G4Ions* ionNew = GmGetParticleMgr::GetInstance()->CreateIon( Z, A, md.energy, 0 );
    
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) ) G4cout << " SHMetastableIsotMgr::ReadData() New ion created " << ionNew->GetParticleName() << " Z= " << ionNew->GetAtomicNumber() << " A= " << ionNew->GetAtomicMass() << " ExcitE= " << ionNew->GetExcitationEnergy() << G4endl;
#endif

    theMetastableIsotopes[md.name] = ionNew;
    theMetastableIsotopesData[md.name] = md;
  }

}

//-----------------------------------------------------------------------
std::pair<G4Ions*,G4double> SHMetastableIsotMgr::FindIsomere( G4ParticleDefinition* nucleus, std::vector<ppdd> tracks )
{
  G4Ions* ion = 0;
  std::pair<G4Ions*,G4double> ionW(ion,0.);

  G4String metaName = nucleus->GetParticleName();
  int inum = 0;
  G4int nl = metaName.length();
  // Find last number
  for( G4int ii = 0; ii < nl; ii++ ){
    if( metaName[ii] == '[' ) {
      inum = ii;
      break;
    }
  }
  metaName = metaName.substr(0,inum)+"m";
  std::map<G4String, G4Ions* >::const_iterator itemi = theMetastableIsotopes.find(metaName);
  if( itemi == theMetastableIsotopes.end() ) return ionW;

#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) ) G4cout << " SHMetastableIsotMgr::FindIsomere FOUND " << metaName << G4endl;
#endif

  std::map<G4String, MetastableData >::const_iterator itemid = theMetastableIsotopesData.find(metaName);

  // loop to tracks to match nucleus mass + track energy with mass of some of the isomeres stored
  std::vector<ppdd>::const_iterator itet;
  if( SHVerb(debugVerb) ) G4cout << " SHMetastableIsotMgr::FindIsomere N TRACKS " << tracks.size() << G4endl;
  for( itet = tracks.begin(); itet != tracks.end(); itet++ ) {
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) ) G4cout << " SHMetastableIsotMgr::FindIsomere particle " << (*itet).first->GetParticleName() << G4endl;
#endif
    if( (*itet).first == G4Gamma::Gamma() ) {
      G4double energy = (*itet).second.first;
#ifndef GAMOS_NO_VERBOSE
      if( SHVerb(debugVerb) ) G4cout << " SHMetastableIsotMgr::FindIsomere testing gamma " << energy << G4endl;
#endif
      if( fabs((*itemid).second.energy - energy) < energy/1000. ) {
	return std::pair<G4Ions*,G4double>((*itemi).second,1./(*itemid).second.branchingRatio);
      }
    }
  }
  
  return ionW;

}

//-----------------------------------------------------------------------
SHMetastableIsotMgr::~SHMetastableIsotMgr()
{
}

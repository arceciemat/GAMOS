#include "GmLivermoreRayleighModel_XSChangeNEW.hh"
#include "G4SystemOfUnits.hh"
#include "G4RayleighAngularGenerator.hh"

using namespace std;

#include "GmPhysicsVerbosity.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4int GmLivermoreRayleighModel_XSChangeNEW::maxZ = 100;
G4LPhysicsFreeVector* GmLivermoreRayleighModel_XSChangeNEW::dataCS[] = {0};

GmLivermoreRayleighModel_XSChangeNEW::GmLivermoreRayleighModel_XSChangeNEW()
  : G4LivermoreRayleighModel(), GmXSChange_DistributionUser("XS_Change")
{
  fParticleChange = 0;
  lowEnergyLimit  = 10 * eV; 
  
  SetAngularDistribution(new G4RayleighAngularGenerator());
  
  verboseLevel= 2;
  // Verbosity scale for debugging purposes:
  // 0 = nothing 
  // 1 = calculation of cross sections, file openings...
  // 2 = entering in methods

  if(verboseLevel > 0) 
  {
    G4cout << "GmLivermoreRayleighModel_XSChangeNEW is constructed " << G4endl;
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmLivermoreRayleighModel_XSChangeNEW::~GmLivermoreRayleighModel_XSChangeNEW()
{
  if(IsMaster()) {
    for(G4int i=0; i<maxZ; ++i) {
      if(dataCS[i]) { 
	delete dataCS[i];
	dataCS[i] = 0;
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmLivermoreRayleighModel_XSChangeNEW::Initialise(const G4ParticleDefinition* particle,
					  const G4DataVector& cuts)
{
  if (verboseLevel > 1) 
  {
    G4cout << "Calling Initialise() of GmLivermoreRayleighModel_XSChangeNEW." << G4endl
	   << "Energy range: "
	   << LowEnergyLimit() / eV << " eV - "
	   << HighEnergyLimit() / GeV << " GeV"
	   << G4endl;
  }

  if(IsMaster()) {

    // Initialise element selector
    InitialiseElementSelectors(particle, cuts);

    // Access to elements
    char* path = getenv("G4LEDATA");
    G4ProductionCutsTable* theCoupleTable =
      G4ProductionCutsTable::GetProductionCutsTable();
    G4int numOfCouples = theCoupleTable->GetTableSize();
  
    for(G4int i=0; i<numOfCouples; ++i) 
      {
	const G4MaterialCutsCouple* couple = 
	  theCoupleTable->GetMaterialCutsCouple(i);
	const G4Material* material = couple->GetMaterial();
	const G4ElementVector* theElementVector = material->GetElementVector();
	G4int nelm = material->GetNumberOfElements();
    
	for (G4int j=0; j<nelm; ++j) 
	  {
	    G4int Z = G4lrint((*theElementVector)[j]->GetZ());
	    if(Z < 1)          { Z = 1; }
	    else if(Z > maxZ)  { Z = maxZ; }
	    if( (!dataCS[Z]) ) { ReadData(Z, path); }
	  }
      }
  }

  if(isInitialised) { return; }
  fParticleChange = GetParticleChangeForGamma();
  isInitialised = true;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmLivermoreRayleighModel_XSChangeNEW::InitialiseLocal(const G4ParticleDefinition*,
					       G4VEmModel* masterModel)
{
  if (verboseLevel > 1) 
  {
    G4cout << "Calling GmLivermoreRayleighModel_XSChangeNEW::InitialiseLocal()" 
	   << G4endl;
  } //GDEB
  SetElementSelectors(masterModel->GetElementSelectors());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmLivermoreRayleighModel_XSChangeNEW::ReadData(size_t Z, const char* path)
{
  if (verboseLevel > 1) 
  {
    G4cout << "Calling ReadData() of GmLivermoreRayleighModel_XSChangeNEW" 
	   << G4endl;
  }

  if(dataCS[Z]) { return; }
  
  const char* datadir = path;

  if(!datadir) 
  {
    datadir = getenv("G4LEDATA");
    if(!datadir) 
    {
      G4Exception("GmLivermoreRayleighModel_XSChangeNEWModel::ReadData()","em0006",
		  FatalException,
		  "Environment variable G4LEDATA not defined");
      return;
    }
  }

  //
  
  dataCS[Z] = new G4LPhysicsFreeVector();
  
  // Activation of spline interpolation
  //dataCS[Z] ->SetSpline(true);
  
  std::ostringstream ostCS;
  ostCS << datadir << "/livermore/rayl/re-cs-" << Z <<".dat";
  std::ifstream finCS(ostCS.str().c_str());
  
  if( !finCS .is_open() ) 
  {
    G4ExceptionDescription ed;
    ed << "GmLivermoreRayleighModel_XSChangeNEW data file <" << ostCS.str().c_str()
       << "> is not opened!" << G4endl;
    G4Exception("GmLivermoreRayleighModel_XSChangeNEW::ReadData()","em0003",FatalException,
		ed,"G4LEDATA version should be G4EMLOW6.27 or later.");
    return;
  } 
  else 
  {
    if(verboseLevel > 3) { 
      G4cout << "File " << ostCS.str() 
	     << " is opened by GmLivermoreRayleighModel_XSChangeNEW" << G4endl;
    }
    dataCS[Z]->Retrieve(finCS, true);
  } 
}


G4double GmLivermoreRayleighModel_XSChangeNEW::ComputeCrossSectionPerAtom(
                                       const G4ParticleDefinition*,
                                             G4double GammaEnergy,
                                             G4double Z, G4double,
                                             G4double, G4double)
{
  if (verboseLevel > 1) 
  {
    G4cout << "G4LivermoreRayleighModel::ComputeCrossSectionPerAtom()" 
	   << G4endl;
  }

  if(GammaEnergy < lowEnergyLimit) { return 0.0; }
  
  G4double xs = 0.0;
  
  G4int intZ = G4lrint(Z);

  if(intZ < 1 || intZ > maxZ) { return xs; }

  G4LPhysicsFreeVector* pv = dataCS[intZ];

  // if element was not initialised
  // do initialisation safely for MT mode
  if(!pv) { 
    InitialiseForElement(0, intZ);
    pv = dataCS[intZ];
    if(!pv) { return xs; }
  }

  G4int n = pv->GetVectorLength() - 1;
  G4double e = GammaEnergy/MeV;
  if(e >= pv->Energy(n)) {
    xs = (*pv)[n]/(e*e);  
  } else if(e >= pv->Energy(0)) {
    xs = pv->Value(e)/(e*e);  
  }

  if(verboseLevel > 0)
  {
    G4cout  <<  "****** DEBUG: tcs value for Z=" << Z << " at energy (MeV)=" 
	    << e << G4endl;
    G4cout  <<  "  cs (Geant4 internal unit)=" << xs << G4endl;
    G4cout  <<  "    -> first E*E*cs value in CS data file (iu) =" << (*pv)[0] 
	    << G4endl;
    G4cout  <<  "    -> last  E*E*cs value in CS data file (iu) =" << (*pv)[n] 
	    << G4endl;
    G4cout  <<  "*********************************************************" 
	    << G4endl;
  }
  return xs;
}
/*
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4double GmLivermoreRayleighModel_XSChangeNEW::ComputeCrossSectionPerAtom(const G4ParticleDefinition* part,
						G4double energy,
						G4double Z, G4double A,
						G4double cutEnergy, G4double maxEnergy)
{
  if(PhysicsVerb(testVerb)) G4cout << "CHECK_XSChange START GmLivermoreRayleighModel_XSChangeNEW::ComputeCrossSectionPerAtom " << G4endl; 

  G4double XS = G4LivermoreRayleighModel::ComputeCrossSectionPerAtom(part, energy, Z, A, cutEnergy, maxEnergy);
  G4double XStransformation = GetDistributionValue(energy);

  if(PhysicsVerb(debugVerb)) G4cout << " GmPEEffectFluoModel_XSChangeNEW::ComputeCrossSectionPerAtom " <<  XS*XStransformation << " = " << XS << "*" << XStransformation << " energy= " << energy << G4endl; 
  if(PhysicsVerb(testVerb)) G4cout << "CHECK_XSChange END GmLivermoreRayleighModel_XSChangeNEW::ComputeCrossSectionPerAtom " << G4endl; 

  return XS*XStransformation;

}
*/
/*//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4double 
GmLivermoreRayleighModel_XSChange::CrossSectionPerVolume(const G4Material* material,
					   const G4ParticleDefinition* part,
					   G4double energy,
					   G4double cutEnergy, G4double maxEnergy)
{
  if(PhysicsVerb(testVerb)) G4cout << "CHECK_XSChange START GmLivermoreRayleighModel_XSChange::CrossSectionPerVolume " << G4endl; 
  G4double XS = G4LivermoreRayleighModel::CrossSectionPerVolume(material, part, energy, cutEnergy, maxEnergy);
  G4double XStransformation = GetDistributionValue(energy);

  if(PhysicsVerb(debugVerb)) G4cout << " GmLivermoreRayleighModel_XSChange::CrossSectionPerVolume " <<  XS*XStransformation << " = " << XS << "*" << XStransformation << " energy= " << energy << G4endl; 
  if(PhysicsVerb(testVerb)) G4cout << "CHECK_XSChange END GmLivermoreRayleighModel_XSChange::CrossSectionPerVolume " << G4endl; 

  return XS*XStransformation;

  }*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmLivermoreRayleighModel_XSChangeNEW::SampleSecondaries(
                          std::vector<G4DynamicParticle*>*,
			  const G4MaterialCutsCouple* couple,
			  const G4DynamicParticle* aDynamicGamma,
			  G4double, G4double)
{
  if (verboseLevel > 1) {
    G4cout << "Calling SampleSecondaries() of GmLivermoreRayleighModel_XSChangeNEW" 
	   << G4endl;
  }
  G4double photonEnergy0 = aDynamicGamma->GetKineticEnergy();

  // absorption of low-energy gamma  
  /*
  if (photonEnergy0 <= lowEnergyLimit)
    {
      fParticleChange->ProposeTrackStatus(fStopAndKill);
      fParticleChange->SetProposedKineticEnergy(0.);
      fParticleChange->ProposeLocalEnergyDeposit(photonEnergy0);
      return ;
    }
  */
  // Select randomly one element in the current material
  const G4ParticleDefinition* particle =  aDynamicGamma->GetDefinition();
  const G4Element* elm = SelectRandomAtom(couple,particle,photonEnergy0);
  G4int Z = G4lrint(elm->GetZ());

  // Sample the angle of the scattered photon
  
  G4ThreeVector photonDirection = 
    GetAngularDistribution()->SampleDirection(aDynamicGamma, 
					      photonEnergy0, 
					      Z, couple->GetMaterial());
  fParticleChange->ProposeMomentumDirection(photonDirection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4AutoLock.hh"
namespace { G4Mutex LivermoreRayleighModelMutex = G4MUTEX_INITIALIZER; }

void 
GmLivermoreRayleighModel_XSChangeNEW::InitialiseForElement(const G4ParticleDefinition*, 
					       G4int Z)
{
  G4AutoLock l(&LivermoreRayleighModelMutex);
  //  G4cout << "GmLivermoreRayleighModel_XSChangeNEW::InitialiseForElement Z= " 
  //   << Z << G4endl;
  if(!dataCS[Z]) { ReadData(Z); }
  l.unlock();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef GmXrayRefraction_h
#define GmXrayRefraction_h 1

#include "globals.hh"
#include "templates.hh"
#include "geomdefs.hh"
#include "Randomize.hh"
#include "G4Step.hh"
#include "G4VDiscreteProcess.hh"
#include "G4DynamicParticle.hh"
#include "G4Material.hh"
#include "G4TransportationManager.hh"

class GmXrayRefraction : public G4VDiscreteProcess 
{

private:

public: 

    GmXrayRefraction(const G4String& processName = "XrayRefraction",
                                     G4ProcessType type = fOptical);

	~GmXrayRefraction();

public: 

    G4bool IsApplicable(const G4ParticleDefinition& aParticleType);

	G4double GetMeanFreePath(const G4Track& ,
				 G4double ,
				 G4ForceCondition* condition);

	G4VParticleChange* PostStepDoIt(const G4Track& aTrack,
				       const G4Step&  aStep);

private:

	void Snell();

	void G4Swap(G4double* a, G4double* b) const;

	void G4Swap(G4Material* a, G4Material* b) const;

	void G4VectorSwap(G4ThreeVector* vec1, G4ThreeVector* vec2) const;

private:

	G4Material* Material1;
	G4Material* Material2;

	G4double Rindex1;
	G4double Rindex2;

	G4double cost1, cost2, sint1, sint2;
	
	G4double thePhotonMomentum;
	G4ThreeVector OldMomentum;
	G4ThreeVector NewMomentum;

	G4ThreeVector theGlobalNormal;
	G4ThreeVector theFacetNormal;

        G4double kCarTolerance;
};

inline
G4bool GmXrayRefraction::IsApplicable(const G4ParticleDefinition& 
					               aParticleType)
{
   return  ( &aParticleType == G4Gamma::Gamma() );
}

inline
void GmXrayRefraction::G4Swap(G4double* a, G4double* b) const
{


  G4double temp;

  temp = *a;
  *a = *b;
  *b = temp;
}

inline
void GmXrayRefraction::G4Swap(G4Material* a, G4Material* b) const
{

   G4Material* temp = a;

   a = b;
   b = temp;
}

inline
void GmXrayRefraction::G4VectorSwap(G4ThreeVector* vec1,
				       G4ThreeVector* vec2) const
{

  G4ThreeVector temp;

  temp = *vec1;
  *vec1 = *vec2;
  *vec2 = temp;
}

#endif /* GmXrayRefraction_h */

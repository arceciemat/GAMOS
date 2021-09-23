#include "GmClassifierCrossPhantom.hh"
#include "GmRegularParamUtils.hh"
#include "GmReadDICOMVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Step.hh"
#include "G4PhantomParameterisation.hh"
#include "G4TouchableHistory.hh"

//---------------------------------------------------------------
GmClassifierCrossPhantom::GmClassifierCrossPhantom(G4String name) : GmVClassifier( name )
{
#ifdef WIN32
  theMaxIndex = int64_t(std::pow(double(2),int(32)-1)-1); // sizeof(int64_t) = 32 gives error 
#else
  theMaxIndex = int64_t(pow(double(2.),int(sizeof(int64_t)*8)-2)-1);
#endif
}

//---------------------------------------------------------------
void GmClassifierCrossPhantom::SetParameters( std::vector<G4String>& params )
{
  if( params.size() != 0 ){
    G4Exception("GmClassifierCrossPhantom::GmClassifierCrossPhantom",
		"",
		FatalErrorInArgument,
		" Number of parameters different than 0");
  }

  std::vector<G4PhantomParameterisation*> regparams = GmGeometryUtils::GetInstance()->GetPhantomParams(1);
  if( regparams.size() != 3 ) {
        G4Exception("GmClassifierCrossPhantom::GmClassifierCrossPhantom",
		"",
		FatalErrorInArgument,
		    ("There must be 3 G4PhantomParameterisation, while there is " + GmGenUtils::itoa(regparams.size())).c_str()); 
  }
  theNVoxelsX = regparams[0]->GetNoVoxelsX();
  theYPhantomNVoxels = regparams[1]->GetNoVoxelsY();
  theNVoxelsY = regparams[1]->GetNoVoxelsY()*2+1;
  theNVoxelsZ = regparams[0]->GetNoVoxelsZ();
  theNVoxelsXY = theNVoxelsX * theNVoxelsY;
#ifndef GAMOS_NO_VERBOSE 
  if( ReadDICOMVerb(debugVerb) ) G4cout << " GmClassifierCrossPhantom::SetParameters= " << theNVoxelsX << " " << theNVoxelsY << " " << theNVoxelsZ << " YPHANTOM " << theYPhantomNVoxels << G4endl;
#endif
  
}

//---------------------------------------------------------------
int64_t GmClassifierCrossPhantom::GetIndexFromStep(const G4Step* aStep)
{
  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4TouchableHistory* th = (G4TouchableHistory*)(preStep->GetTouchable());
  //  if( th->GetHistoryDepth() <= 2 ) {
  if( th->GetHistoryDepth() < 2 ) {
    std::cerr << " NDEPTH " <<   th->GetHistoryDepth() << std::endl; 
    for( int ii = 0; ii < th->GetHistoryDepth(); ii++ ) {
      std::cerr << " DEPTH " << ii << " " << th->GetVolume(ii)->GetName() << std::endl;
    } 
    G4Exception("GmClassifierCrossPhantom::GetIndexFromStep",
		"",
		FatalErrorInArgument,
		("The volume is not a phantom voxel : " + th->GetVolume(0)->GetName()).c_str());
  }

  return GetIndex( th );

}

//---------------------------------------------------------------
int64_t GmClassifierCrossPhantom::GetIndexFromTrack(const G4Track* aTrack)
{
  const G4Step* aStep = aTrack->GetStep();
  if( !aStep ) return 0;

  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4TouchableHistory* th = (G4TouchableHistory*)(preStep->GetTouchable());

  return GetIndex( th ) ;

}

//---------------------------------------------------------------
int64_t GmClassifierCrossPhantom::GetIndex(G4TouchableHistory* th)
{
  // Get NEW COPY NO
  int64_t index0 = th->GetReplicaNumber(0);
  int64_t index1 = th->GetReplicaNumber(1);
  G4int nx = 0;
  G4int ny = 0;
  G4int nz = 0;
  if( index1 == 1 ) { // X phantom param
    nx = index0 % theNVoxelsX;
    nz = index0 / theNVoxelsX; 
    ny = theYPhantomNVoxels;
  }else if( index1 == 2) { // X phantom param
    nx = theNVoxelsX/2;
    nz = index0 / theYPhantomNVoxels; 
    ny = index0 % theYPhantomNVoxels;
  }else if( index1 == 3 ) { // X phantom param
    nx = theNVoxelsX/2;
    nz = index0 / theYPhantomNVoxels; 
    ny = index0 % theYPhantomNVoxels + theYPhantomNVoxels+1;
  } else {
    G4Exception("GmClassifierCrossPhantom::GetIndexFromStep",
		"",
		FatalErrorInArgument,
		("The voxel is not in a CrossPhantom geometry, because voxel mother ID is not 0, 1 or 2, but " + GmGenUtils::itoa(index1)).c_str());
  }
      
  int64_t copyNo = nx + theNVoxelsX*ny + theNVoxelsXY*nz;

#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) G4cout << " GmClassifierCrossPhantom::GetIndex indices= " << index0 << " " << index1 << " ->copyNo= " << copyNo << " nx,ny,nz " << nx << " " << ny << " " << nz << G4endl;
#endif
  return copyNo; 
  
}

//-------------------------------------------------------------
void GmClassifierCrossPhantom::SetIndices( std::vector<G4String> wl )
{

  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
    int64_t index = int64_t(GmGenUtils::GetValue(wl[ii+1]));
    theIndexMap[wl[ii]] = index;
    if( theMaxIndex < index) theMaxIndex = index;
    theIndexNames[int64_t(GmGenUtils::GetValue(wl[ii]))] = wl[ii+1];
  }

  theMaxIndex -= theIndexMap.size()+1;  

}

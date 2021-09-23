#include "GmClassifierByParallelLogicalVolume.hh"
#include "GamosCore/GamosBase/Classifiers/include/GmClassifierVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4Step.hh"
#include "G4LogicalVolume.hh"
#include "G4ParallelWorldScoringProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4VUserParallelWorld.hh"
#include "G4tgbParallelGeomMgr.hh"
#include "G4PhysicalVolumeStore.hh"

//------------------------------------------------------------------
GmClassifierByParallelLogicalVolume::GmClassifierByParallelLogicalVolume(G4String name) : GmVClassifier( name )
{
  theNShift = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":NShift",10));

  GmGetParticleMgr* particleMgr = GmGetParticleMgr::GetInstance();
  std::vector<G4ParticleDefinition*> particles = particleMgr->GetG4ParticleList("*");

  for( size_t ip = 0; ip < particles.size(); ip++ ){
    G4ParticleDefinition* particle = particles[ip];
    std::vector<G4VProcess*> processes = particleMgr->GetG4ProcessList(particle->GetParticleName(),"ParaWorldProc*",1);
    std::vector<G4ParallelWorldScoringProcess*> pwsProcesses;
    for( size_t ii = 0; ii < processes.size(); ii++ ) { 
      pwsProcesses.push_back((G4ParallelWorldScoringProcess*)(processes[ii]));
    }
    theParallelWorldProcesses[particle] = pwsProcesses; 
  }

  mvpwsp::const_iterator itepw = theParallelWorldProcesses.begin();
  size_t nParallelWorlds = (*itepw).second.size();
  size_t nMaxWorld = sizeof(int64_t)*8 * log(2) / log(theNShift);
  if( nParallelWorlds >= nMaxWorld ){
    G4int NShiftMax = sizeof(int64_t)*8*log(2)/log(nParallelWorlds);
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(testVerb) ) G4cout << " NSHIFTMAX " << NShiftMax << " nParallelWorlds " << nParallelWorlds << " nMaxWorld " << nMaxWorld << G4endl;
#endif
    G4Exception("GmClassifierByParallelLogicalVolume::GmClassifierByParallelLogicalVolume",
		"",
		FatalException,
		("Too many worlds for NShift value = " + GmGenUtils::itoa(theNShift) +
		 " change it with command /gamos/setParam "+theName+":NShift VALUE, where VALUE < " + GmGenUtils::itoa(NShiftMax)).c_str());
  }  
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(testVerb) ) {
    G4int NShiftMax = sizeof(int64_t)*8*log(2)/log(nParallelWorlds);
    G4cout << " NSHIFTMAX " << NShiftMax << " nParallelWorlds " << nParallelWorlds << " nMaxWorld " << nMaxWorld << G4endl;
  }
#endif

   bInitialized = false;

}

//------------------------------------------------------------------
void GmClassifierByParallelLogicalVolume::SetParameters( std::vector<G4String>& params)
{
  if( params.size() != 0 ) {
    G4String parastr = "PARAMS: ";
    for( unsigned int ii=0; ii > params.size(); ii++ ){
      parastr += " " + params[ii];
    }
    G4Exception("GmClassifierByParallelLogicalVolume::SetParameters"
		,G4String("Error in number of parameters at classifier"+theName).c_str()
  		,FatalErrorInArgument
		,G4String("There should be 0, and they are "+parastr).c_str());
  }
}


//------------------------------------------------------------------
int64_t GmClassifierByParallelLogicalVolume::GetIndexFromStep(const G4Step* aStep )
{
  if( !bInitialized ) InitializeMaps();

  int64_t index = 0;
  vpwsp parallelWorldProcesses = GetParticleParallelWorldProcesses(aStep->GetTrack()->GetDefinition());
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(testVerb) )  G4cout << " GmClassifierByParallelLogicalVolume::GetIndexFromStep NWORLDS " << parallelWorldProcesses.size() << G4endl;
#endif
  for( size_t ii = 0; ii < parallelWorldProcesses.size(); ii++ ) {    
    const G4StepPoint* preSP = parallelWorldProcesses[ii]->fGhostPreStepPoint;
    G4LogicalVolume* lv = preSP->GetPhysicalVolume()->GetLogicalVolume();
    std::map<G4LogicalVolume*,int64_t>::const_iterator ite = theIndexMap.find(lv);
    if( ite == theIndexMap.end() ){
      G4Exception("GmClassifierByParallelLogicalVolume::GetIndexFromStep",
		  "",
		  FatalException,
		  ("Logical volume in parallel world " + lv->GetName() + " is not in classifier list, please contact GAMOS author's").c_str());
    }
    //    index += pow(theNShift,ii)*(*ite).second;
    index += (*ite).second;    
#ifndef GAMOS_NO_VERBOSE
    if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByParallelLogicalVolume::GetIndexFromStep " << index << " lv " << lv->GetName() << G4endl;
#endif
  }
  
  return index;
}

//------------------------------------------------------------------
int64_t GmClassifierByParallelLogicalVolume::GetIndexFromTrack(const G4Track* aTrack)
{
  if( !bInitialized ) InitializeMaps();
  
  //--- No classification for G4Track's
  int64_t index;
  G4VPhysicalVolume* pv = aTrack->GetVolume();
  if( !pv ) return 0;
  G4LogicalVolume* lv = pv->GetLogicalVolume();
  std::map<G4LogicalVolume*,int64_t>::const_iterator ite = theIndexMap.find(lv);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[lv] = index;
  } else {
    index = (*ite).second;
  }
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByParallelLogicalVolume::GetIndex " << index << " lv " << lv->GetName() << G4endl;
#endif

  return index;
}


//--------------------------------------------------------------
G4String GmClassifierByParallelLogicalVolume::GetIndexName(int64_t index)
{
  G4String name = "NOT_FOUND";
  /*  std::map<G4LogicalVolume*,int64_t>::const_iterator ite;
  size_t nWorlds = log(index)/log(theNShift)+1;
  for( size_t ii = 0; ii < nWorlds; ii++ ) {
    size_t index1 = size_t(index/pow(theNShift,ii)) % theNShift;
    size_t indexT = pow(theNShift,ii)*ii + index1;
    std::map<int64_t,G4String>::const_iterator iten = theIndexNameMap.find( indexT );
    if( iten == theIndexNameMap.end() ) {
      theIndexNameMap[indexT] = "NOT_FOUND";
      iten = theIndexNameMap.find( indexT );
      G4Exception("GmClassifierByParallelLogicalVolume::GetIndexName",
		  "",
		  JustWarning,
		  ("Index not found in index name map, please contact GAMOS authors. Index name assigned is 'NOT_FOUND'"));
    }
    name= (*iten).second;
  }
  */

  std::map<int64_t,G4String>::const_iterator iten = theIndexNameMap.find( index );
  if( iten == theIndexNameMap.end() ) {
    theIndexNameMap[index] = "NOT_FOUND";
    iten = theIndexNameMap.find( index );
    G4Exception("GmClassifierByParallelLogicalVolume::GetIndexName",
		"",
		JustWarning,
		("Index not found in index name map, please contact GAMOS authors. Index name assigned is 'NOT_FOUND'"));
  }
  name= (*iten).second;
  
  return name;
}

//---------------------------------------------------------------
GmClassifierByParallelLogicalVolume::~GmClassifierByParallelLogicalVolume()
{
  //print names of each index 
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) {
    G4cout << "%%%%% Table of indices for GmClassifierByParallelLogicalVolume " << theName << G4endl;
    std::map<int64_t,G4String>::const_iterator ite;
    for( ite = theIndexNameMap.begin(); ite != theIndexNameMap.end(); ite++ ) {
      G4cout << theName << ": " << (*ite).second << " = " << (*ite).first << G4endl;
    }
  }
#endif
}


//-------------------------------------------------------------
void GmClassifierByParallelLogicalVolume::SetIndices( std::vector<G4String> )
{
  theMaxIndex -= theIndexMap.size()+1;  
}

//-------------------------------------------------------------
void GmClassifierByParallelLogicalVolume::InitializeMaps()
{
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) )  G4cout <<" GmClassifierByParallelLogicalVolume::InitializeMaps " << G4endl;
#endif

  std::vector<G4VUserParallelWorld*> parallelWorlds = G4tgbParallelGeomMgr::GetInstance()->GetParallelWorlds();
  //  std::set<G4int> parallelIndices = G4tgbParallelGeomMgr::GetInstance()->GetIndices();
  std::set<G4VPhysicalVolume*> theGhostWorlds;

  for( size_t ii = 0; ii < parallelWorlds.size(); ii++ ) {
    G4int pwID = 0;
    G4VPhysicalVolume* pvGhostWorld = parallelWorlds[ii]->GetWorld();
    G4String ghostWorldName = parallelWorlds[ii]->GetName();
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(testVerb) ) G4cout << ii << "InitializeMaps  ghostWorldName " << ghostWorldName << G4endl;
#endif
    //    G4VPhysicalVolume* ghostWorldPV = parallelWorlds[ii]->GetWorld();
    //    theGhostWorlds.insert(ghostWorldPV);

    //--- Find all the LV that belong to parallel world, and set the index in this ghost world
    G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
    std::vector<G4VPhysicalVolume*>::iterator citepv;
    for( citepv = pvs->begin(); citepv != pvs->end(); citepv++ ) {
      G4VPhysicalVolume* pv = *citepv;
      SetIndexMaps( pv->GetLogicalVolume(), pv, pvGhostWorld, pwID, ii );
    }

  }    

  //--- build names merging parallel worlds (classifier always returns an indexin each world volume, even if it is the top volume)
  std::vector<G4int> nIndices;
  G4int nIndicesTot = 1;
  for( size_t ii = 0; ii < parallelWorlds.size(); ii++ ) {
    int64_t idx = pow(theNShift,G4int(ii));
    G4int jj = 1;
    for(; jj < theNShift; jj++ ) {
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(testVerb) )  G4cout << "COMBINE INDICES search IndexNameMap " << idx*jj  << G4endl;
#endif
      if( theIndexNameMap.find(idx*jj) == theIndexNameMap.end() ) break;
    }
#ifndef GAMOS_NO_VERBOSE
    if( ClassifierVerb(testVerb) ) {
      std::map<int64_t,G4String>::const_iterator ite;
      for( ite = theIndexNameMap.begin(); ite != theIndexNameMap.end(); ite++ ) {
#ifndef GAMOS_NO_VERBOSE
	if( ClassifierVerb(testVerb) ) G4cout << "COMBINE INDICES IndexNameMap " << (*ite).first << " : " << (*ite).second  << G4endl;
#endif
      }
    }
#endif
    nIndices.push_back( jj-1 ); // number of volumes in world ii
    nIndicesTot *= jj-1;
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(testVerb) )  G4cout << ii << " COMBINE INDICES : nIndices " << jj-1 << " tot " << nIndicesTot << G4endl; 
#endif
  }

  std::vector<G4int> nIndices1;
  for( G4int ii = 0; ii < nIndicesTot; ii++ ) {
    size_t idxNew = 0;
    G4String nameNew = "";
    for( size_t jj = 0; jj < nIndices.size(); jj++ ) {
      G4int fmult = 1;
      for( size_t kk = jj; kk > 0; kk-- ) {
	fmult *= nIndices[kk];
      }
      G4int nii = (ii/fmult) % nIndices[jj];
      int64_t idx = pow(theNShift,G4int(jj));
      idxNew += (nii + 1)*idx;
      if( nameNew != "" ) nameNew += "__";
      nameNew += theIndexNameMap[(nii + 1)*idx];
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(testVerb) )  G4cout << ii << " " << jj << " COMBINE INDICES : idxNew " << idxNew << " nameNew " << nameNew << " nii " << nii << " idx " << idx << " fmult " << fmult << G4endl;
#endif
    }
    theIndexNameMap[idxNew] = nameNew;
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(testVerb) )  G4cout << ii << " COMBINE INDICES : FINAL idxNew " << idxNew << " nameNew " << nameNew << G4endl;
#endif
  }
  
  bInitialized = true;

}

//-------------------------------------------------------------
void GmClassifierByParallelLogicalVolume::SetIndexMaps( G4LogicalVolume* childLV, G4VPhysicalVolume* PV, G4VPhysicalVolume* PVGhostWorld, G4int& pwID, G4int idWorld )
{
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " ENTER SetIndexMaps " << PV->GetName() << " " << PVGhostWorld->GetName() << " " << pwID << " " << idWorld << G4endl;
#endif
  if( PV == PVGhostWorld ) {
    ++pwID;
    if( pwID == theNShift ) {
      G4Exception("GmClassifierByParallelLogicalVolume::GmClassifierByParallelLogicalVolume",
		  "",
		  FatalException,
		  ("Too many volumes in one parallel world, it has to be smaller than NShift value = " + GmGenUtils::itoa(theNShift) +
		   " change it with command /gamos/setParam "+theName+":NShift VALUE, where VALUE > " + GmGenUtils::itoa(theNShift)).c_str());
    }
    int64_t idx = pow(theNShift,G4int(idWorld))*pwID;
    theIndexMap[childLV] = idx;
    theIndexNameMap[idx] = childLV->GetName();
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << "SetIndexMaps FOUND " << childLV->GetName() << " : " << pwID << " NAME " << pow(theNShift,G4int(idWorld))*pwID << " : " << childLV->GetName() << G4endl;
#endif
  } else {
    //--- look for PVs of mother
    G4LogicalVolume* lvMother = PV->GetMotherLogical();
    std::set<G4VPhysicalVolume*> pvset; // list of PV's of lvMother
    G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
    std::vector<G4VPhysicalVolume*>::iterator citepv;
    for( citepv = pvs->begin(); citepv != pvs->end(); citepv++ ) {
      G4VPhysicalVolume* pv = *citepv;
      if( pv->GetLogicalVolume() == lvMother ) pvset.insert(pv);
    }
    std::set<G4VPhysicalVolume*>::const_iterator itepv;
    for( itepv = pvset.begin(); itepv != pvset.end(); itepv++ ){
      SetIndexMaps( childLV, *itepv, PVGhostWorld, pwID, idWorld );
    }
  }
}

//-------------------------------------------------------------------------
std::vector<G4ParallelWorldScoringProcess*> GmClassifierByParallelLogicalVolume::GetParticleParallelWorldProcesses(G4ParticleDefinition* particle )
{
  std::map< G4ParticleDefinition*, std::vector<G4ParallelWorldScoringProcess*> >::const_iterator ite = theParallelWorldProcesses.find(particle);
  if( ite == theParallelWorldProcesses.end() ) {
    G4Exception("GmVParallelGeomConditionGetParticleParallelWorldProcesses",
		"",
		FatalException,
		("No G4ParallelWorldScoringProcess found for particle " + particle->GetParticleName()).c_str());
  }

  return (*ite).second;
}

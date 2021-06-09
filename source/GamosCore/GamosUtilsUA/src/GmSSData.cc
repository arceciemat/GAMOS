#include "GmSSData.hh"
#include "GmUtilsUAVerbosity.hh"

//----------------------------------------------------------------
GmSSData::GmSSData( G4int showerID, const G4Step* aStep, TSPUse tspuse )
{
  theTSPUse = tspuse; // store it for other steps

  //--- First track is special 
  theSteps.push_back(new GmTrajStep( aStep, TSPUPost ));

  theShowerID = showerID;
  theFirstTrackID = aStep->GetTrack()->GetTrackID();

#ifndef GAMOS_NO_VERBOSE
  if( UtilsUAVerb(debugVerb) ) G4cout << " GmSSData::GmSSData " 
	 << " showerID " << theShowerID 
	 << " theFirstTrackID " << theFirstTrackID 
	 << G4endl;
#endif
}


//----------------------------------------------------------------
void GmSSData::AddStep( const G4Step* aStep )
{
#ifndef GAMOS_NO_VERBOSE
  if( UtilsUAVerb(debugVerb) ) G4cout << " GmSSData::AddPoint "
	 << " showerID " << theShowerID 
	 << G4endl;
#endif
  theSteps.push_back(new GmTrajStep(aStep, theTSPUse));

}


//----------------------------------------------------------------
G4double GmSSData::GetTotalEnergy() const
{

  std::vector<GmTrajStep*>::const_iterator ite;
  G4double tener = 0.;
  for( ite = theSteps.begin(); ite != theSteps.end(); ite++ ){
    tener = (*ite)->GetDepositedEnergy();
  }

  return tener;

}

//----------------------------------------------------------------
G4ThreeVector GmSSData::GetFirstTrackPos() const
{

  return theSteps[0]->GetPositionPost();

}


//----------------------------------------------------------------
G4ThreeVector GmSSData::GetFirstTrackDir() const
{

  return theSteps[0]->GetMomentumDirectionPost();

}

//----------------------------------------------------------------
void GmSSData::Dump( std::ostream& fout )
{
  if( theTSPUse == TSPUPre ) {
    fout << " Pre ";
  } else if( theTSPUse == TSPUPost ) {
    fout << " Post ";
  } else if( theTSPUse == TSPUPrePost ) {
    fout << " PrePost ";
  } 
  fout << theShowerID << " ";
  fout << theFirstTrackID << " ";
  fout << theClassifierIndex << G4endl;

  fout << theSteps.size() << " ";
  std::vector<GmTrajStep*>::const_iterator ite;
  for( ite = theSteps.begin(); ite != theSteps.end(); ite++ ){
    GmTrajStep* step = *ite;
    if( ite == theSteps.begin() || theTSPUse == TSPUPost ) {
      DumpStepPost(step, fout);

    } else if( theTSPUse == TSPUPre ) {
      DumpStepPre(step, fout);

    } else if( theTSPUse == TSPUPrePost ) {
      DumpStepPre(step, fout);
      DumpStepPost(step, fout);

    }

    fout << step->GetDepositedEnergy() << " ";
    fout << G4endl;
  }
}

//----------------------------------------------------------------
void GmSSData::DumpStepPost( GmTrajStep* step, std::ostream& fout )
{
  fout << step->GetPositionPost().x() << " " 
       << step->GetPositionPost().y() << " " 
       << step->GetPositionPost().z() << " "; 	
  fout << step->GetMomentumPost().x() << " " 
       << step->GetMomentumPost().y() << " " 
       << step->GetMomentumPost().z() << " "; 
  fout << step->GetKineticEnergyPost() << " ";
   
}


//----------------------------------------------------------------
void GmSSData::DumpStepPre( GmTrajStep* step, std::ostream& fout )
{
  fout << step->GetPositionPre().x() << " " 
       << step->GetPositionPre().y() << " " 
       << step->GetPositionPre().z() << " "; 	
  fout << step->GetMomentumPre().x() << " " 
       << step->GetMomentumPre().y() << " " 
       << step->GetMomentumPre().z() << " "; 
  fout << step->GetKineticEnergyPre() << " ";
   
}


//----------------------------------------------------------------
void GmSSData::Dump( FILE* fout )
{
  const char* datastr = new char[7];
  if( theTSPUse == TSPUPre ) {
    datastr = "Pre    ";
  } else if( theTSPUse == TSPUPost ) {
    datastr = "Post   ";
  } else if( theTSPUse == TSPUPrePost ) {
    datastr = "PrePost";
  } 
  fwrite (&datastr, sizeof(char),7,fout);

  fwrite (&theShowerID, sizeof(G4int),1,fout);

  fwrite (&theFirstTrackID, sizeof(G4int),1,fout);

  fwrite (&theClassifierIndex, sizeof(G4int),1,fout);

  G4int datai = theSteps.size();
  fwrite (&datai, sizeof(G4int),1,fout);

  std::vector<GmTrajStep*>::const_iterator ite;
  for( ite = theSteps.begin(); ite != theSteps.end(); ite++ ){
    GmTrajStep* step = *ite;
    if( ite == theSteps.begin() || theTSPUse == TSPUPost ) {
      DumpStepPost(step, fout);

    } else if( theTSPUse == TSPUPre ) {
      DumpStepPre(step, fout);

    } else if( theTSPUse == TSPUPrePost ) {
      DumpStepPre(step, fout);
      DumpStepPost(step, fout);

    }

    float dataf = step->GetDepositedEnergy();
    fwrite (&dataf, sizeof(G4int),1,fout);
  
  }
}

//----------------------------------------------------------------
void GmSSData::DumpStepPost( GmTrajStep* step, FILE* fout )
{
  float dataf = step->GetPositionPost().x();
  fwrite (&dataf, sizeof(G4int),1,fout);
  dataf = step->GetPositionPost().y();
  fwrite (&dataf, sizeof(G4int),1,fout);
  dataf = step->GetPositionPost().z();
  fwrite (&dataf, sizeof(G4int),1,fout);
  dataf = step->GetMomentumPost().x();
  fwrite (&dataf, sizeof(G4int),1,fout);
  dataf = step->GetMomentumPost().y();
  fwrite (&dataf, sizeof(G4int),1,fout);
  dataf = step->GetMomentumPost().z();
  fwrite (&dataf, sizeof(G4int),1,fout);
  dataf = step->GetKineticEnergyPost();
  fwrite (&dataf, sizeof(G4int),1,fout);
   
}


//----------------------------------------------------------------
void GmSSData::DumpStepPre( GmTrajStep* step, FILE* fout )
{
  float dataf = step->GetPositionPre().x();
  fwrite (&dataf, sizeof(G4int),1,fout);
  dataf = step->GetPositionPre().y();
  fwrite (&dataf, sizeof(G4int),1,fout);
  dataf = step->GetPositionPre().z();
  fwrite (&dataf, sizeof(G4int),1,fout);
  dataf = step->GetMomentumPre().x();
  fwrite (&dataf, sizeof(G4int),1,fout);
  dataf = step->GetMomentumPre().y();
  fwrite (&dataf, sizeof(G4int),1,fout);
  dataf = step->GetMomentumPre().z();
  fwrite (&dataf, sizeof(G4int),1,fout);
  dataf = step->GetKineticEnergyPre();
  fwrite (&dataf, sizeof(G4int),1,fout);
   
}

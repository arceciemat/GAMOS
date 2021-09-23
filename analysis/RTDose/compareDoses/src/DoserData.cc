#include "DoserData.hh"
#include "DoseData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <set>

//-----------------------------------------------------------------
DoserData::DoserData()
{
  theNumberOfFiles = 1;
  theNDoses = 0;
}

//-----------------------------------------------------------------
void DoserData::AddDoseData(DoseData* ed )
{
  theDoseData.push_back(ed);
  ed->SetDoserData( this );
  //  if( bSetDoser) ed->MultiplyByNEvents();
  if( DoseData::verbosity >= 2 ) G4cout << " DoserData::AddDoseData " << theName << " SIZE= " << theDoseData.size() << " : " << ed->GetName() << G4endl;

}

//-----------------------------------------------------------------
void DoserData::SetNEvents(G4double evts) 
{
  //  G4cout << " SetNEvents  " << theName << " = " << theNEvents << " nevts " << evts << G4endl;
  theNEvents = evts;
}

//-----------------------------------------------------------------
DoserData DoserData::operator+=( const DoserData& sd2 )
{
  if( DoseData::verbosity >= 1 ) G4cout << "  DoserData::operator+=: ADDING DOSER " << sd2.GetName() << "  N= " << sd2.GetNDoses() << "  TO  " << theName << "  N= " << theNDoses << G4endl;

  G4double NEvents2 = sd2.GetNEvents();
  /*t  for( int ii = 0; ii < GetNDoses()+1; ii++ ){
    DoseData* id = GetDoseData(ii);
    (*id) *= theNEvents;
    } */

  // loop to dose data of 2 and check if same dose data is found in 1: if yes, add data, if not create new data
  /* if( sd2.GetNDoses() == 0 ) {
    G4Exception("DoserData::operator+=",
		"",
		JustWarning,
		G4String("Adding empty doser " + sd2.GetName()).c_str());
    return *this;
    }*/

  //  G4cout << sd2.GetName() << " N DOSES " <<  sd2.GetNDoses() << G4endl; //GDEB
  if( DoseData::verbosity >= 2 ) G4cout << "@@@ CHECK IF NEW DOSER ADDS SOME NEW DOSE " << G4endl; 
  for( int ii2 = 0; ii2 < sd2.GetNDoses()+1; ii2++ ){
    DoseData* id2 = sd2.GetDoseData(ii2);
    bool bFound1 = false;
    DoseData* id1 = 0;
    for( int ii1 = 0; ii1 < theNDoses+1; ii1++ ){
      id1 = theDoseData[ii1];
      if( *id1 == *id2 ) {
	bFound1 = true;
	break;
      }
    }

    //-    G4cout << ii2 << " CHECK DATA 2 " << id2->GetName() << " " << id2->GetDose() << " bFound1 " << bFound1 << G4endl; //GDEB

    if( bFound1 ) {
      *id1 += *id2;
      if( DoseData::verbosity >= 2 ) G4cout << "  DoserData::operator+=: " << theName << " ADDED DATA " << id2->GetName() << " to " << id1->GetName() << G4endl; 
    } else {
      AddDoseData( id2 );
      //-      (*id2) *= NEvents2;
      theNDoses += 1;
      if( DoseData::verbosity >= 2 ) G4cout << "  DoserData::operator+=: " << theName << " CREATED NEW DATA " << id2->GetName() << G4endl; 
    }

  }
      
  theNumberOfFiles++;

  /*t  for( int ii = 0; ii < GetNDoses()+1; ii++ ){
    DoseData* id = GetDoseData(ii);
    (*id) *= 1./(theNEvents+NEvents2);
    if( DoseData::verbosity >= 2 ) G4cout << "  DoserData::operator+=: " << theName << " SCALE TO PER EVENT " << id->GetName() << " = " << id->GetDose() << G4endl; 
    } */

  theNEvents += NEvents2;

  return *this;
  
}

//-----------------------------------------------------------------
bool DoserData::operator!=( const DoserData& sd2 )
{

  if (GetName() != sd2.GetName() ) {
    return true;
  }

  return false;

}

//-----------------------------------------------------------------
DoseData* DoserData::GetDoseData(unsigned int ii) const
{
  if( ii >= theDoseData.size() ) {
    G4cerr << theName << " Asking for dose " << ii << " >= N doses = " << theDoseData.size() << G4endl;
    G4Exception("DoserData::GetDoseData",
		"Asking for an dose that does not exists",
		FatalException,
		"You are trying to add an doser with ' Number of entries= ' wrong");
  }
  return theDoseData[ii];
}

//-----------------------------------------------------------------
mdpdd* DoserData::GetPlotData( G4bool bLogX, G4double multFactor, G4double dispFactor, G4double dispYFactor )
{
  mdpdd* plotData = new mdpdd;
  //-  G4cout << "  GETPLOTDATA " << theDoseData.size() << G4endl;
  for( unsigned int ii = 0; ii < theDoseData.size(); ii++ ) {
    DoseData* doseData = theDoseData[ii];
    //    G4cerr << " GETPLOTDATA " << doseData<< G4endl;    //GDEB

    // Index must represent a numeric value
    G4String doseName = doseData->GetName();
    if( !GmGenUtils::IsNumber(doseName) ) {
      if( doseName == "SUM_ALL:" ) continue;
      /*      G4Exception("GAMOSDoserData::GetPlotData",
		  "Non numeric dose index",
		  FatalException,
		  (G4String("Dose index is not numeric = ")+doseName).c_str()); */
    } 
    //    G4cout << this <<  "  GETPLOTDATA doseName " << doseName << " bLogX " << bLogX << G4endl; //GDEB
    G4double indexVal = GetIndexVal( doseName, bLogX ) + dispFactor;
    //    G4cout << "  GETPLOTDATA doseName " << doseName << " indexVal "  << indexVal << " dispFactor " << dispFactor << G4endl; //GDEB
    pdd pd(doseData->GetDose() * multFactor+dispYFactor,doseData->GetDoseError() * multFactor);
    //    G4cout << " GETPLOTDATA " << indexVal << " " << doseData->GetDose() * multFactor+dispYFactor << " +- " << doseData->GetDoseError() * multFactor << G4endl;//GDEB
    (*plotData)[indexVal] = pd;
  }

  return plotData;

}

//-----------------------------------------------------------------
void DoserData::InvertX()
{
  for( size_t ii = 0; ii < theDoseData.size(); ii++ ) {
    G4String doseName = theDoseData[ii]->GetName();
    if( doseName.substr(0,1) == "-" ) {
      theDoseData[ii]->SetName(doseName.substr(1,doseName.length()));
    } else {
      theDoseData[ii]->SetName("-"+doseName);
    }
    //    G4cout << this << " "<<  GetName() << " " << ii << " DoserData::InvertX " <<  theDoseData[ii]->GetName() << G4endl; //GDEB
  }

}

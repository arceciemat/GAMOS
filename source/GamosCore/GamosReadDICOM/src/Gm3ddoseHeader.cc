#include "Gm3ddoseHeader.hh"
#include "GmSqdoseHeader.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-----------------------------------------------------------------------
G4bool Gm3ddoseHeader::operator==(const Gm3ddoseHeader& right) const
{
  G4bool equ = 1;
  if( theNoVoxelsX != right.GetNoVoxelsX() ||
      theNoVoxelsX != right.GetNoVoxelsX() ||
      theNoVoxelsX != right.GetNoVoxelsX() ||
      theVoxelLimitsX != right.GetVoxelLimitsX() ||
      theVoxelLimitsX != right.GetVoxelLimitsX() ||
      theVoxelLimitsX != right.GetVoxelLimitsX() ) {
    equ = 0;
  }
  return equ;

}


//-----------------------------------------------------------------------
G4bool Gm3ddoseHeader::operator!=(const Gm3ddoseHeader& right) const
{ 
  return !( *this == right );
}

//-----------------------------------------------------------------------
Gm3ddoseHeader::Gm3ddoseHeader(const GmSqdoseHeader& sqdh)
{
  //  G4cout << " reading header file " << G4endl;
  theNoEvent = sqdh.GetNumberOfEvents();
  G4cout << " N events " << theNoEvent << G4endl;

  theNoVoxelsX = sqdh.GetNoVoxelsX();
  theNoVoxelsY = sqdh.GetNoVoxelsY();
  theNoVoxelsZ = sqdh.GetNoVoxelsZ();

  for( size_t ii = 0; ii < sqdh.GetVoxelLimitsX().size(); ii++ ) {
    theVoxelLimitsX.push_back(sqdh.GetVoxelLimitsX()[ii]/10.);
  }
  for( size_t ii = 0; ii < sqdh.GetVoxelLimitsY().size(); ii++ ) {
    theVoxelLimitsY.push_back(sqdh.GetVoxelLimitsY()[ii]/10.);
  }
  for( size_t ii = 0; ii < sqdh.GetVoxelLimitsZ().size(); ii++ ) {
    theVoxelLimitsZ.push_back(sqdh.GetVoxelLimitsZ()[ii]/10.);
  }

}

//-----------------------------------------------------------------------
void Gm3ddoseHeader::Read( GmFileIn& fin )
{
  //  G4cout << " reading header file " << G4endl;
  std::vector<G4String> wl;
  /*t
  fin.GetWordsInLine(wl);
  if( wl.size() != 1 ){
    G4Exception("Gm3ddoseHeader::Read"," first line must have 1 words: number of events",FatalErrorInArgument,G4String("It has "+GmGenUtils::itoa(wl.size())).c_str());
  }
  theNoEvent = (unsigned long long)(GmGenUtils::GetValue( wl[0] ));
  G4cout << " N events " << theNoEvent << G4endl;
  */
  theNoEvent = 1E6;
  G4cout << " N events set to " << theNoEvent << G4endl;

  fin.GetWordsInLine(wl);
  if( wl.size() != 3 ){
    G4Exception("Gm3ddoseHeader::Read"," second line must have 3 words: number of voxels",FatalErrorInArgument,G4String("It has "+GmGenUtils::itoa(wl.size())).c_str());
  }

  theNoVoxelsX = GmGenUtils::GetInteger( wl[0] );
  theNoVoxelsY = GmGenUtils::GetInteger( wl[1] );
  theNoVoxelsZ = GmGenUtils::GetInteger( wl[2] );

  fin.GetWordsInLine(wl);
  if( wl.size() != theNoVoxelsX+1 ){
    G4Exception("Gm3ddoseHeader::Read",G4String(" third line must have number of words = " + GmGenUtils::itoa(theNoVoxelsX)).c_str(),FatalErrorInArgument,G4String("It has "+GmGenUtils::itoa(wl.size())).c_str());
  }
  for( size_t ii = 0; ii < theNoVoxelsX+1; ii++ ){    
    theVoxelLimitsX.push_back( GmGenUtils::GetValue(wl[ii]) );
  }

  fin.GetWordsInLine(wl);
  if( wl.size() != theNoVoxelsY+1 ){
    G4Exception("Gm3ddoseHeader::Read",G4String(" fourth line must have number of words = " + GmGenUtils::itoa(theNoVoxelsY)).c_str(),FatalErrorInArgument,G4String("It has "+GmGenUtils::itoa(wl.size())).c_str());
  } 
  for( size_t ii = 0; ii < theNoVoxelsY+1; ii++ ){    
    theVoxelLimitsY.push_back( GmGenUtils::GetValue(wl[ii]) );
  }

  fin.GetWordsInLine(wl);
  if( wl.size() != theNoVoxelsZ+1 ){
    G4Exception("Gm3ddoseHeader::Read",G4String(" fifth line must have number of words = " + GmGenUtils::itoa(theNoVoxelsZ)).c_str(),FatalErrorInArgument,G4String("It has "+GmGenUtils::itoa(wl.size())).c_str());
  }
  for( size_t ii = 0; ii < theNoVoxelsZ+1; ii++ ){    
    theVoxelLimitsZ.push_back( GmGenUtils::GetValue(wl[ii]) );
  }

}


//-----------------------------------------------------------------------
std::ostream& operator<<(std::ostream &out, 
			 const Gm3ddoseHeader &dh)
{

  out << dh.GetNumberOfEvents() << G4endl;
  //  G4cout << "DOSE N EV " << dh.GetNumberOfEvents() << G4endl;

  out << dh.GetNoVoxelsX() << " " << dh.GetNoVoxelsY() << " " << dh.GetNoVoxelsZ() << G4endl;
  std::vector<float>::const_iterator ite;

  std::vector<float> vlim = dh.GetVoxelLimitsX();
  //  out << " size x " << dh.GetVoxelLimitsX().size() << G4endl;
  for( ite = vlim.begin(); ite != vlim.end(); ite++ ){
    out << *ite << " ";
  }
  out << G4endl;
 
  vlim = dh.GetVoxelLimitsY();
  for( ite = vlim.begin(); ite != vlim.end(); ite++ ){
    out << *ite << " ";
  }
  out << G4endl;
  
  vlim = dh.GetVoxelLimitsZ();
  for( ite = vlim.begin(); ite != vlim.end(); ite++ ){
    out << *ite << " ";
  }
  out << G4endl;

  return out;

}

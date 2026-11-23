#include "RTVExtraInfoProviderFloat.hh"
#include "RTExtraInfoMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
RTVExtraInfoProviderFloat::RTVExtraInfoProviderFloat()
{ 
 
}

//----------------------------------------------------------------
void RTVExtraInfoProviderFloat::InitVariables()
{
  theWord = new float;  

  RTExtraInfoMgr::GetInstance()->AddExtraInfoProviderFloat(this);
  theFileName = GmParameterMgr::GetInstance()->GetStringValue("RTExtraInfoProviderFloat:FileName","RTExtraInfoProvider.summ");
}


//----------------------------------------------------------------
void RTVExtraInfoProviderFloat::DumpIndices(std::ostream& out)
{
  out << GetName() 
      << " FLOAT INDEX " << theOrder << G4endl;
}

#include"ParametrizedFieldCreator.hh"
#include"G4MagneticField.hh"

////////////////////////////////////////////////////////////////////////////////
//
ParametrizedFieldCreator::ParametrizedFieldCreator(G4String ModelName): 
theModelName(ModelName)
{ nb_fields_of_same_model=0;
} 
////////////////////////////////////////////////////////////////////////////////
//
ParametrizedFieldCreator::~ParametrizedFieldCreator()
{;
}








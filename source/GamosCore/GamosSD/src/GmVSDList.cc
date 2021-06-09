#include "GamosCore/GamosSD/include/GmVSDList.hh"

GmVSDList* GmVSDList::theInstance = 0;

GmVSDList* GmVSDList::GetInstance()
{

  if( theInstance == 0 ){
    theInstance = new GmVSDList;
  }

  return theInstance;
}

GmVSDList::GmVSDList()
{
}

GmVSDList::~GmVSDList()
{
}

void GmVSDList::AddSD( GmVSD* sd )
{
  push_back( sd );
  theSDTypeList.insert( sd->GetType() );
}

// #define  GmVerbosity_CC
#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include <iostream>
#include <fstream>
#include <algorithm>

GmVerbosity::GmVerbosity(int il) : level_(testVerb) 
{
  filter_ = il;
  verbose_=true;
}

const FilterLevel * GmVerbosity::levels() {
  static const FilterLevel local[nFilterLevel+1] = {silentVerb,errorVerb,warningVerb,infoVerb,debugVerb,testVerb};
  return local;
}

const G4String * GmVerbosity::levelNames() {
  static const G4String local[nFilterLevel+1] = {"silent", "error", "warning", "info", "debug", "test"};
  return local;
}
const FilterLevel & GmVerbosity::level(const G4String & name) {
  const G4String * p = std::find(levelNames(),levelNames()+nFilterLevel,name);
  return levels()[p-levelNames()];
}

const G4String & GmVerbosity::levelName(int il) {
  const FilterLevel * p = std::lower_bound(levels(),levels()+nFilterLevel,il);
  return levelNames()[p-levels()];
}
 
static std::ofstream devnull("/dev/null");

std::ostream & GmVerbosity::out = devnull;

void GmVerbosity::Verbose() {verbose_=true;}

void GmVerbosity::Silent() {verbose_=false;}


void GmVerbosity::SetFilterLevel(int il) { filter_=il;}

int GmVerbosity::GetFilterLevel() const { return filter_;}


//GmVerbosity infoV, testV, debugV;

//----------------------------------------------------------------------------

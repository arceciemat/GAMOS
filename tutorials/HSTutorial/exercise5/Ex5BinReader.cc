#include "globals.hh"
#include <fstream>

int main()
{

  FILE* fin = fopen("track.bin.out","rb");
  char* particle = new char[10];
  char* process = new char[20];
  float energy, elost;
  G4cout << " READING FILE 'track.bin.out'" << G4endl;
  for( G4int ii = 0;; ii++ ) {
    fread(particle, sizeof(char), 10, fin);
    fread(&elost, sizeof(float), 1, fin);
    G4cout << ii << " : " << particle << " E= " << elost << G4endl; 
    if( feof(fin) ) break;
  }

  fin = fopen("step.bin_GmPrimaryFilter_noTransportFilter.out","rb");
  G4cout << " READING FILE 'step.bin_GmPrimaryFilter_noTransportFilter.out'" << G4endl;
  for( G4int ii = 0;; ii++ ) {
    fread(particle, sizeof(char), 10, fin);
    fread(process, sizeof(char), 20, fin);
    fread(&energy, sizeof(float), 1, fin);
    fread(&elost, sizeof(float), 1, fin);
    G4cout << ii << " : " << particle
	   << " process= " << process 
	   << " E= " << energy 
	   << " Elost= " << elost 
	   << G4endl; 
    if( feof(fin) ) break;
  }
 //

  return 0;
}

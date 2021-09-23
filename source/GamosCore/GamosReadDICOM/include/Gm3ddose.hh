#ifndef Gm3ddose__HH
#define Gm3ddose__HH

#include <vector>
#include "Gm3ddoseHeader.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
class GmSqdose;

class Gm3ddose
{
public:
  Gm3ddose();
  Gm3ddose(const GmSqdose& sqd);
  ~Gm3ddose(){};

  Gm3ddose operator+=( const Gm3ddose& dose );

  void Read( G4String& fileName );
  void Read(GmFileIn& fin);
  //  void Write3ddose(std::ofstream& fout);

public:

  Gm3ddoseHeader* GetHeader() const {
    return theHeader; }
  std::vector<float> GetDoses() const {
    return theDoses; }
  std::vector<float> GetDoseErrors() const {
    return theDoseErrors; }

private:
  Gm3ddoseHeader* theHeader;
  std::vector<float> theDoses;
  std::vector<float> theDoseErrors;

};

std::ostream& operator<<(std::ostream &out, 
                           const Gm3ddose &dose);


#endif

#ifndef GmParameter_hh
#define GmParameter_hh
//
//
//
#include<string>
#include<sstream>

class SimpleConfiguration;

/**
 */
template<class TYP>
class GmParameter 
{
private:
  typedef GmParameter<TYP> par;  
public:
  GmParameter() {} //?? NEEDED??
  GmParameter(const TYP& par) : theValue(par) {
    //  G4cout << " new GmParameter " << theValue << G4endl; 
  }

  ///
  GmParameter & operator=(const par& rh) {
    theValue=rh.theValue;
    return *this;
  }
  
  virtual ~GmParameter(){}

  operator const TYP&() const { return theValue;}
  TYP& operator()() { return theValue;}
  const TYP& operator()() const { return theValue;}

  TYP& value() { return theValue;}
  const TYP& value() const { return theValue;}

  /*
  std::string Type() const {
    return ClassName<T>::name();
  }
  */

  void Set(const TYP& val){ theValue = val; }
  void Set(const std::string & input);
  
  //  virtual std::string Get() const;
  
  
protected:
  
  TYP theValue;
  
  
};

#include<iomanip>
#include<algorithm>

/*
template<class T>
inline std::ostream & operator<<(std::ostream & out, const GmParameter<TYP>& c) {
  return out <<  c.value();
}
*/

template<class TYP>
inline void GmParameter<TYP>::Set(const std::string & input) {
  std::istringstream in(input.c_str());
  in >> theValue;
}  // for vectors???

/*
template<class TYP>
inline std::string GmParameter<TYP>::Get() const{
  std::ostringstream out;
  out << theValue;
#ifdef CMS_CHAR_STREAM
  out << ends;
#endif
  return out.str();
}
*/

#endif // GmParameter_hh

#ifndef CompareStats_C
#define CompareStats_C
#include "Definitions.C"
#include "TText.h"
#include "TMarker.h"
#include "TH1F.h"
#include "TMath.h"
#include "ROOTUtilities/Verbosity.C"
#include "ROOTUtilities/PlotData/Definitions.C"

#include <sstream>

struct CompStatData {
  double chi2;
  int ndof;
  double pValue;
};

/*//--------------------------------------------------------------------------
CompStatData CompareStats( mdpdd* data1, mdpdd* data2 )
{
  double PREC_MATCH = 1.e-4;
  CompStatData cs;
  mdpdd::iterator ite1,ite2;
  std::set<double> valAll;
  std::set<double>::const_iterator ites;
 
  for( ite1 = data1->begin(); ite1 != data1->end(); ite1++ ) {
    valAll.insert( (*ite1).first );
    if( verbose >= 4 )  G4cout << "CS insert val1 " << (*ite1).first <<  G4endl;
  }

  //build new data2, matching energies with those of data1 very close
  mdpdd* data2b = new mdpdd;
  for( ite2 = data2->begin(); ite2 != data2->end(); ite2++ ) {    
    G4bool bFound = false;
    for( ites = valAll.begin(); ites != valAll.end(); ites++ ) {
      if( fabs((*ites)-(*ite2).first/(*ites)) < PREC_MATCH ) {
	bFound = true;
	break;
      }      
    }      
    if( !bFound ) {
      (*data2b)[(*ite2).first] = (*ite2).second;
      if( verbose >= 3 )  G4cout << "CS insert val2 " << (*ite2).first <<  G4endl;
    } else {
      (*data2b)[*ites] = (*ite2).second;
      if( verbose >= 3 )  G4cout << "CS close found " << fabs((*ites)-(*ite2).first/(*ites)) << " < " << PREC_MATCH << " " << *ites << " - " << (*ite2).first <<  G4endl;
    }
  }

  for( ite2 = data2b->begin(); ite2 != data2b->end(); ite2++ ) {    
    valAll.insert( (*ite2).first );
  }
  
  double chi2 = 0.;
  for( ites = valAll.begin(); ites != valAll.end(); ites++ ) {
    ite1 = data1->find(*ites);
    ite2 = data2b->find(*ites);
    if( verbose >= 2 ) G4cout << "CS look " << *ites <<  G4endl;
    if( ite1 != data1->end() && ite2 != data2b->end() ) {
      double err1 = (*ite1).second.second;
      double err2 = (*ite2).second.second;
      if( err1 != 0. || err2 != 0 ) {
	double err = sqrt(err1*err1+err2*err2);
	chi2 += sqr(((*ite1).second.first-(*ite2).second.first)/err);
      }
      if( verbose >= 2 ) G4cout << " CS CHI2:0 " << chi2 << " " << (*ite1).second.first << " - " << (*ite2).second.first << " +- " << err1 << " " << err2 << G4endl;
    } else if( ite1 != data1->end() ) { // how far is from 0.
      double err1 = (*ite1).second.second;
      if( err1 != 0. ) {
	chi2 += sqr((*ite1).second.first/err1);
      }
      if( verbose >= 2 ) G4cout << " CS CHI2:1 " << chi2 << " " << (*ite1).second.first << " +- " << " " << err1 << G4endl;
    } else if( ite2 != data2b->end() ) {
      double err2 = (*ite2).second.second;
      if( err2 != 0 ) {
	chi2 += sqr((*ite2).second.first/err2);
      }
      if( verbose >= 2 ) G4cout << " CS CHI2:2 " << chi2 << " " << (*ite2).second.first << " +- " << " " << err2 << G4endl;
    }
  }

  cs.chi2 = chi2;
  cs.ndof = valAll.size();
  cs.pValue = TMath::Prob(chi2,cs.ndof);

  return cs;
}
*/

//--------------------------------------------------------------------------
CompStatData CompareStatsChi2( mdpdd* data1, mdpdd* data2, mdpdd& dataDiv)
{
  if( data1->size() == 0 || data2->size() == 0 ) {
    std::cerr << " !!! ERROR IN CompareStats, one of the data is empty: data1) " << data1->size() << " data2= " << data2->size() << std::endl; 
    exit(1);
  }
  double PREC_MATCH = 1.e-4;
  CompStatData cs;
  mdpdd::iterator itec1,itec2; // loop to doses in doser
  std::set<double> valAll;
  std::set<double>::const_iterator ites;
  double chi2 = 0.;
  G4int ndof = 0;
  /*  for( itec1 = data1->begin(); itec1 != data1->end(); itec1++ ) {
    std::cout << "CC_1 " << (*itec1).first << " : " <<(*itec1).second.first << " +- " << (*itec1).second.second << std::endl; //GDEB
  }
  for( itec2 = data2->begin(); itec2 != data2->end(); itec2++ ) {
    std::cout << "CC_2 " << (*itec2).first << " : " <<(*itec2).second.first << " +- " << (*itec2).second.second << std::endl; //GDEB
    }*/

  for( itec1 = data1->begin(); itec1 != data1->end(); itec1++ ) {
    G4double valX = (*itec1).first;
    //    if( fabs(valX) > 10 ) continue; //GDEB
    //    std::cout << " GOING TO COMPARE " << valX << std::endl; //GDEB
    itec2 = data2->find(valX); // LOOK FOR VALUE WITH SAME X
    G4bool bFound = false;
    if( itec2 != data2->end() ) bFound = true;
    
    if( ! bFound ) {  // LOOK FOR VALUE WITH VERY SIMILAR X
      for( itec2 = data2->begin(); itec2 != data2->end(); itec2++ ) {
	if( fabs((valX-(*itec2).first)/valX) < PREC_MATCH ) {
	  bFound = true;
	  break;
	}
      }
    }
    if( ! bFound ) {
      // interpolate linearly between values
      itec2 = data2->lower_bound(valX);
      if( itec2 == data2->begin() ) continue;// VALUES BELOW MINIMUM data2
      if( valX > (*(data2->rbegin())).first ) continue;// VALUES ABOVE MAXIMUM data2
      if( itec2 != data2->begin() ){
	mdpdd::iterator itec2b = itec2; 
	itec2b--;
	G4double slopeVal = ((*itec2b).second.first - (*itec2).second.first ) / ( (*itec2b).first - (*itec2).first );
	G4double newVal = (*itec2).second.first + slopeVal * ( valX - (*itec2).first );
	G4double slopeErr = ((*itec2b).second.second - (*itec2b).second.second ) / ( (*itec2b).first - (*itec2).first );
	G4double newErr = (*itec2).second.second + slopeErr * ( valX - (*itec2).first );
 	if( verbose >= 4 ) {
	  std::cout << valX << " COMPARE INTERP " << newVal << " +- " << newErr 
		    << " between X= " << (*itec2b).first << " - " << (*itec2).first 
		    << " valX= " << (*itec2b).second.first << " - " << (*itec2).second.first 
		    << " errX " << (*itec2b).second.second << " - " << (*itec2).second.second     
		    << std::endl; //GDEB
	}
	if( newVal != 0.  && (*itec1).second.first != 0. ) {
	  newErr = (*itec1).second.first/newVal * sqrt( sqr(newErr/newVal) + sqr((*itec1).second.second/(*itec1).second.first));
	  newVal = (*itec1).second.first/newVal;
	  dataDiv[valX] = pdd(newVal,newErr);
	  //	  std::cout << "dataDIV1 " << valX << " : " << newVal << "+-" << newErr << std::endl; //GDEB
	  if( newErr != 0. && valX < 25e9 ) {
	    chi2 += sqr((newVal-1.)/newErr);
	    if( verbose >= 2 ) G4cout << " CS CHI2:1 " << chi2 << " " << newVal-1. << " +- " << newErr << G4endl;
	    //	    G4cout<< " CHI2 " << chi2 << " X " << valX << " " << newErr << G4endl; //GDEB
	    ndof++;
	  }
	  //	  G4cout << " DATADIV " << valX << " : " << newErr << " " << newVal << " * " << sqr(newErr/newVal) << " + " << sqr((*itec1).second.second/(*itec1).second.first) << G4endl; //GDEB
	}
      }
    } else {
      //t	    if( itec2 != data2->begin() ) {
      //      std::cout << " valX FOUND in data2 " << valX << std::endl; //GDEB    
      if( (*itec1).second.first != 0. && (*itec2).second.first != 0. ) {
	G4double dose = (*itec1).second.first / (*itec2).second.first;
	G4double doseErr = (*itec1).second.second/(*itec1).second.first + (*itec2).second.second/(*itec2).second.first;
	doseErr *= dose;
	dataDiv[valX] = pdd(dose,doseErr);
	//	std::cout << "dataDIV " << valX << " : " << dose << "+-" << doseErr << std::endl; //GDEB
	if( doseErr != 0.  && valX < 25e9 ) {
	  chi2 += sqr((dose-1.)/doseErr);
	  if( verbose >= 2 ) G4cout << valX << " CS CHI2:2 " << chi2 << " " << dose-1. << " +- " << doseErr << G4endl;
	  ndof++;
	} 
      } else {
	dataDiv[valX] = pdd(0.,0.);
      }
    }

  } 
  
  cs.chi2 = chi2;
  cs.ndof = ndof;
  cs.pValue = TMath::Prob(chi2,cs.ndof);

  return cs;
}

#endif


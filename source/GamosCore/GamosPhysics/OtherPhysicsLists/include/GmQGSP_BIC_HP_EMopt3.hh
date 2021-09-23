//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: GmQGSP_BIC_HP_EMopt3.hh,v 1.5 2018/06/28 15:27:34 arce Exp $
//
//---------------------------------------------------------------------------
//
// ClassName:   GmQGSP_BIC_HP_EMopt3
//
// Author: 2006 G.Folger
//
// based on QGSP_BIC
// Modified:
//
//----------------------------------------------------------------------------
//
#ifndef TGmQGSP_BIC_HP_EMopt3_h
#define TGmQGSP_BIC_HP_EMopt3_h 1

#include <CLHEP/Units/SystemOfUnits.h>

#include "globals.hh"
#include "G4VModularPhysicsList.hh"
#include "CompileTimeConstraints.hh"

template<class T>
class TGmQGSP_BIC_HP_EMopt3: public T
{
public:
  TGmQGSP_BIC_HP_EMopt3(G4int ver = 1);
  virtual ~TGmQGSP_BIC_HP_EMopt3();
  
public:
  // SetCuts() 
  virtual void SetCuts();

private:
  enum {ok = CompileTimeConstraints::IsA<T, G4VModularPhysicsList>::ok };
};
#include "GmQGSP_BIC_HP_EMopt3.icc"
typedef TGmQGSP_BIC_HP_EMopt3<G4VModularPhysicsList> GmQGSP_BIC_HP_EMopt3;

// 2002 by J.P. Wellisch

#endif




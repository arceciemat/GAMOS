#include "GmDummyPhysics.hh"
#include "GmG4HadronicPhysicsLists.hh"
#include "GmG4PhysicsLists.hh"
#include "GmQGSP_BIC_HP_EMopt3.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(GmDummyPhysics,G4VUserPhysicsList*())

PLUGINSVC_FACTORY(G4FTFP_BERT,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_BERT_ATL,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_BERT_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_BERT_TRV,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_INCLXX,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_INCLXX_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFQGSP_BERT,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTF_BIC,G4VUserPhysicsList*())
#ifndef WIN32
PLUGINSVC_FACTORY(G4G4GenericPhysicsList,G4VUserPhysicsList*())
#endif
PLUGINSVC_FACTORY(G4LBE,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(NuBeam,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QBBC,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BERT,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BERT_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BIC,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BIC_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_FTFP_BERT,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_INCLXX,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_INCLXX_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGS_BIC,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4Shielding,G4VUserPhysicsList*())

PLUGINSVC_FACTORY(G4QGSP_BIC_AllHP,G4VUserPhysicsList*())

PLUGINSVC_FACTORY(GmQGSP_BIC_HP_EMopt3,G4VUserPhysicsList*())

PLUGINSVC_FACTORY(GmG4HadronicPhysicsLists,G4VUserPhysicsList*())

#else

#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsFactory.hh"

DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_PHYSICS(GmDummyPhysics);

DEFINE_GAMOS_PHYSICS(G4FTFP_BERT);
DEFINE_GAMOS_PHYSICS(G4FTFP_BERT_HP);
DEFINE_GAMOS_PHYSICS(G4FTFP_BERT_TRV);
DEFINE_GAMOS_PHYSICS(G4FTFP_INCLXX);
DEFINE_GAMOS_PHYSICS(G4FTFP_INCLXX_HP);
DEFINE_GAMOS_PHYSICS(G4FTF_BIC);
#ifndef WIN32
DEFINE_GAMOS_PHYSICS(G4GenericPhysicsList);
#endif
DEFINE_GAMOS_PHYSICS(G4LBE);
DEFINE_GAMOS_PHYSICS(G4QBBC);
DEFINE_GAMOS_PHYSICS(G4QGSP_BERT);
DEFINE_GAMOS_PHYSICS(G4QGSP_BERT_HP);
DEFINE_GAMOS_PHYSICS(G4QGSP_BIC);
DEFINE_GAMOS_PHYSICS(G4QGSP_BIC_HP);
DEFINE_GAMOS_PHYSICS(G4QGSP_FTFP_BERT);
DEFINE_GAMOS_PHYSICS(G4QGSP_INCLXX);
DEFINE_GAMOS_PHYSICS(G4QGSP_INCLXX_HP);
DEFINE_GAMOS_PHYSICS(G4QGS_BIC);
DEFINE_GAMOS_PHYSICS(G4Shielding);
DEFINE_GAMOS_PHYSICS(G4QGSP_BIC_AllHP);
DEFINE_GAMOS_PHYSICS(GmQGSP_BIC_HP_EMopt3);
DEFINE_GAMOS_PHYSICS(G4FTFP_BERT_ATL);
DEFINE_GAMOS_PHYSICS(G4FTFQGSP_BERT);
DEFINE_GAMOS_PHYSICS(NuBeam);

DEFINE_GAMOS_PHYSICS(GmG4HadronicPhysicsLists);

#endif

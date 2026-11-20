# - GamosCore_GamosPhysics_OtherPhysicsLists module build definition

gamos_add_module(GamosCore_GamosPhysics_OtherPhysicsLists
  PUBLIC_HEADERS
    GmDummyPhysics.hh
    GmG4HadronicPhysicsLists.hh
    GmG4HadronicPhysicsListsMessenger.hh
    GmG4PhysicsLists.hh
    GmQGSP_BIC_HP_EMopt3.hh
    GmQGSP_BIC_HP_EMopt3.icc
    PhysListEmStandardSS.hh
  SOURCES
    GmDummyPhysics.cc
    GmG4HadronicPhysicsLists.cc
    GmG4HadronicPhysicsListsMessenger.cc
    PhysListEmStandardSS.cc
    plugin.cc
)

gamos_module_link_libraries(GamosCore_GamosPhysics_OtherPhysicsLists
  PUBLIC
    GamosCore_GamosPhysics_PhysicsList
    lcg_PluginManager
    lcg_SealBase
)

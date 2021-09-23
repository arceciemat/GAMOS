#ifndef ROOT5
#ifndef GmGeneratorDistributionFactories_H
#define GmGeneratorDistributionFactories_H

#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"

// Generator energy distribution factory
#include "GmVGenerDistEnergy.hh"

class GmGenerDistEnergyFactory : public seal::PluginFactory< GmVGenerDistEnergy*()>
{
public:
    static GmGenerDistEnergyFactory *get (void);

private:
    GmGenerDistEnergyFactory (void);
    static GmGenerDistEnergyFactory s_instance;
};

#define DEFINE_GAMOS_GENER_DIST_ENERGY(type) \
  DEFINE_SEAL_PLUGIN(GmGenerDistEnergyFactory, type,#type)
#define DEFINE_GAMOSGENERDISTENERGY(type) \
  DEFINE_SEAL_PLUGIN(GmGenerDistEnergyFactory, type,#type)


// Generator time distribution factory
#include "GmVGenerDistTime.hh"

class GmGenerDistTimeFactory : public seal::PluginFactory< GmVGenerDistTime*()>
{
public:
    static GmGenerDistTimeFactory *get (void);

private:
    GmGenerDistTimeFactory (void);
    static GmGenerDistTimeFactory s_instance;
};


#define DEFINE_GAMOS_GENER_DIST_TIME(type) \
  DEFINE_SEAL_PLUGIN(GmGenerDistTimeFactory, type,#type)
#define DEFINE_GAMOSGENERDISTTIME(type) \
  DEFINE_SEAL_PLUGIN(GmGenerDistTimeFactory, type,#type)


// Generator position distribution factory
#include "GmVGenerDistPosition.hh"

class GmGenerDistPositionFactory : public seal::PluginFactory< GmVGenerDistPosition*()>
{
public:
    static GmGenerDistPositionFactory *get (void);

private:
    GmGenerDistPositionFactory (void);
    static GmGenerDistPositionFactory s_instance;
};

#define DEFINE_GAMOS_GENER_DIST_POSITION(type) \
  DEFINE_SEAL_PLUGIN(GmGenerDistPositionFactory, type,#type)
#define DEFINE_GAMOSGENERDISTPOSITION(type) \
  DEFINE_SEAL_PLUGIN(GmGenerDistPositionFactory, type,#type)
 

// Generator direction distribution factory
#include "GmVGenerDistDirection.hh"

class GmGenerDistDirectionFactory : public seal::PluginFactory< GmVGenerDistDirection*()>
{
public:
    static GmGenerDistDirectionFactory *get (void);

private:
    GmGenerDistDirectionFactory (void);
    static GmGenerDistDirectionFactory s_instance;
};

#define DEFINE_GAMOS_GENER_DIST_DIRECTION(type) \
  DEFINE_SEAL_PLUGIN(GmGenerDistDirectionFactory, type,#type)
#define DEFINE_GAMOSGENERDISTDIRECTION(type) \
  DEFINE_SEAL_PLUGIN(GmGenerDistDirectionFactory, type,#type)
 
#endif // GmGeneratorDisttributionFactories_H

#endif

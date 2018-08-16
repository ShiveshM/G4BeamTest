/**
 * Copyright (C) 2009
 * The IceCube collaboration
 * ID: $Id: G4BeamTestTank.cxx 158930 2017-10-20 01:17:41Z cweaver $
 *
 * @file G4BeamTestTank.cxx
 * @version $Rev: 158930 $
 * @date $Date: 2017-10-20 02:17:41 +0100 (Fri, 20 Oct 2017) $
 * @author Tilo Waldenmaier, Thomas Melzig
 */


#include <g4-tankresponse/g4classes/G4BeamTestTank.h>
#include <g4-tankresponse/g4classes/G4TankIceSD.h>
#include <icetray/I3Units.h>

#include <icetray/OMKey.h>
#include <dataclasses/TankKey.h>
#include <dataclasses/geometry/I3TankGeo.h>
#include <dataclasses/geometry/I3Geometry.h>
#include <dataclasses/I3Position.h>

#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Material.hh>
#include <G4Tubs.hh>
#include <G4Sphere.hh>
#include <G4Box.hh>
#include <G4SDManager.hh>
#include <G4ThreeVector.hh>

#include <G4VisAttributes.hh>

#include <G4UserLimits.hh>

#include <boost/foreach.hpp>

//prevent gcc to make something stupid with pretended unused variables
#ifdef __GNUC__
#define ATTRIBUTE_UNUSED __attribute__((unused))
#else
#define ATTRIBUTE_UNUSED
#endif


G4BeamTestTank::G4BeamTestTank(const TankKey& tankKey, const I3Geometry& geometry):
tankKey_(tankKey), geometry_(geometry)
{
  const I3StationGeoMap& stationMap = geometry.stationgeo;
  unsigned int tankID = tankKey.tank==TankKey::TankA?0:1;
  I3StationGeoMap::const_iterator station_iter = stationMap.find(tankKey.string);
  
  if(station_iter==stationMap.end())
  {
    log_fatal("The requested station %d in not in the geometry!", tankKey.string);
    return;
  }
  
  if(station_iter->second.size()<tankID)
  {
    log_fatal("The number of tanks in station %d is not correct!", tankKey.string);
    return;
  }
  const I3TankGeo& tankGeo = station_iter->second.at(tankID);
  
  // Get tank dimensions
  tankThickness_ = 0.5*CLHEP::cm;
  tankHeight_  = (tankGeo.tankheight / I3Units::m) * CLHEP::m + tankThickness_;
  innerRadius_ = (tankGeo.tankradius / I3Units::m) * CLHEP::m;
  outerRadius_ = innerRadius_ + tankThickness_;
  
  // Get fill and snow heights
  fillHeight_    = (tankGeo.fillheight / I3Units::m) * CLHEP::m;
  snowHeight_    = (tankGeo.snowheight / I3Units::m) * CLHEP::m;
  perliteHeight_ = tankHeight_ - tankThickness_ - fillHeight_;
  
  // Set DOM dimensions
  glassOuterRadius_ = 6.5 * 2.54 * CLHEP::cm; // 6.5" outer glass sphere radius
  glassThickness_   = 0.5 * 2.54 * CLHEP::cm; // 0.5" glass sphere thickness
  
  // Calculate tank position (tank center)
  // tankGeo.position corresponds to the average position of the two DOMs in a tank
  position_.set((tankGeo.position.GetX() / I3Units::m) * CLHEP::m,
                (tankGeo.position.GetY() / I3Units::m) * CLHEP::m,
                (tankGeo.position.GetZ() / I3Units::m) * CLHEP::m - fillHeight_ + 0.5*tankHeight_);
  
  // Get positions of the doms relativ to tank center
  BOOST_FOREACH(const OMKey& omKey, tankGeo.omKeyList_)
  {
    I3OMGeoMap::const_iterator omGeo_iter = geometry_.omgeo.find(omKey);
    if(omGeo_iter==geometry_.omgeo.end())
    {
      log_error_stream(omKey << " is missing in Tank " << tankKey_);
      continue;
    }
    
    G4ThreeVector relPos(omGeo_iter->second.position.GetX() - tankGeo.position.GetX(),
                         omGeo_iter->second.position.GetY() - tankGeo.position.GetY(),
                         omGeo_iter->second.position.GetZ() - tankGeo.position.GetZ());
    
    relDomPositions_[omKey] = (relPos / I3Units::m) * CLHEP::m;
  }
  
  //
  // Calculate Delaunay points
  //
  G4ThreeVector triangleDir(NAN, NAN, NAN);
  switch(station_iter->second.size())
  {
    case 1: // Single tank 
    {
      // Vector orthogonal to DOM positions
      triangleDir.set(relDomPositions_.begin()->second.y(),
                      -relDomPositions_.begin()->second.x(),
                      0.0);
      break;
    }
    case 2: // Two tanks
    {
      const I3TankGeo& neighborGeo = station_iter->second.at(tankID==0?1:0);
      G4ThreeVector neighborPos(neighborGeo.position.GetX(),
                                neighborGeo.position.GetY(),
                                neighborGeo.position.GetZ());
      
      // Convert to G4 units
      neighborPos *= CLHEP::m/I3Units::m;
      
      // Same z position same as other tank
      neighborPos.setZ(position_.z());
      
      triangleDir = position_ - neighborPos;
      break;
    }
    default:
    {
      log_fatal("Invalid number of tanks (%zu) in station %d!",
                station_iter->second.size(), tankKey_.string);
      break;
    }
  }
  
  // side length
  double triangleLength = 10.0 * CLHEP::m;
  
  triangleDir.setMag(0.5*triangleLength/cos(CLHEP::pi/6.0));
  delaunayPoint1_ = position_ + triangleDir;
  
  // Rotate by 120 deg
  triangleDir.rotateZ(CLHEP::pi/1.5);
  delaunayPoint2_ = position_ + triangleDir;
  
  // Rotate by 120 deg
  triangleDir.rotateZ(CLHEP::pi/1.5);
  delaunayPoint3_ = position_ + triangleDir;
}


G4BeamTestTank::~G4BeamTestTank()
{
  
}


G4VPhysicalVolume* G4BeamTestTank::InstallTank(G4VPhysicalVolume* mother, const G4ThreeVector& origin)
{
  // User limits (energy cutoffs)
  // Do not create photons or electrons below cherenkov threshold
  // See also corresponding UserSpecialCuts in Physicslist !!!!
  // Maybe do all of this as stepping action ??????
  G4UserLimits* energyLimit = new G4UserLimits();
  energyLimit->SetUserMinEkine(280.0 * CLHEP::keV);  // Cherenkov threshold of electrons in ice
  
  std::string tankName=boost::lexical_cast<std::string>(tankKey_);
  
  // Define plastic frame
  G4Material* plastic = G4Material::GetMaterial("Plastic");
  G4Tubs* solidTank = new G4Tubs(("solid_tank_" + tankName).c_str(),
                                 0.0 * CLHEP::m, outerRadius_, 0.5 * tankHeight_,
                                 0.0 * CLHEP::deg, 360.0 * CLHEP::deg);
  tankLog_ = new G4LogicalVolume(solidTank, plastic,
                                 ("log_tank_" + tankName).c_str(), 0, 0, 0);
  
  // Define ice volume
  G4Material* ice = G4Material::GetMaterial("Ice");
  G4Tubs* solidIce = new G4Tubs(("solid_ice_" + tankName).c_str(),
                                0.0 * CLHEP::m, innerRadius_, 0.5 * fillHeight_,
                                0.0 * CLHEP::deg, 360.0 * CLHEP::deg);
  G4LogicalVolume* logIce =
      new G4LogicalVolume(solidIce, ice, ("log_ice_" + tankName).c_str(), 0, 0, 0);
  G4ThreeVector physIcePosition(0, 0, -0.5*tankHeight_ + tankThickness_ + 0.5*fillHeight_);
  G4VPhysicalVolume* physIce ATTRIBUTE_UNUSED =
      new G4PVPlacement(0, physIcePosition, logIce,
                        ("ice_" + tankName).c_str(), tankLog_, false, 0);
  
  // Define perlite volume
  G4Material* perlite = G4Material::GetMaterial("Perlite");
  G4Tubs* solidPerlite = new G4Tubs(("solid_perlite_" + tankName).c_str(),
                                    0.0 * CLHEP::m, innerRadius_, 0.5 * perliteHeight_,
                                    0.0 * CLHEP::deg, 360.0 * CLHEP::deg);
  G4LogicalVolume* logPerlite =
      new G4LogicalVolume(solidPerlite, perlite, ("log_perlite_" + tankName).c_str(), 0, 0, 0);
  G4ThreeVector physPerlitePosition(0, 0, -0.5 * tankHeight_ + 0.5 * CLHEP::cm + fillHeight_ +
                                    0.5 * perliteHeight_);
  G4VPhysicalVolume* physPerlite ATTRIBUTE_UNUSED =
      new G4PVPlacement(0, physPerlitePosition, logPerlite,
                        ("perlite_" + tankName).c_str(), tankLog_, false, 0);
  
  // Define glass sphere & effective DOM material splitted in upper and lower part
  G4Material* glass        = G4Material::GetMaterial("Glass");
  G4Material* effectiveDOM = G4Material::GetMaterial("effectiveDOM");
  
  std::map<OMKey, G4ThreeVector> domPosIce;
  std::map<OMKey, G4ThreeVector>::const_iterator om_iter;
  for(om_iter=relDomPositions_.begin(); om_iter!=relDomPositions_.end(); ++om_iter)
  {
    const OMKey& omKey = om_iter->first;
    
    G4ThreeVector upperDOMpos(om_iter->second.x(), om_iter->second.y(), -0.5 * perliteHeight_);
    G4ThreeVector lowerDOMpos(om_iter->second.x(), om_iter->second.y(),  0.5 * fillHeight_);
    
    domPosIce[omKey] = lowerDOMpos;
    
    std::string omName=boost::lexical_cast<std::string>(omKey);
    
    G4Sphere *upperglasssphere = new G4Sphere (("solid_dom_up_" + omName).c_str(),
                                               0.0 * CLHEP::m, glassOuterRadius_,
                                               0.0 * CLHEP::deg, 360.0 * CLHEP::deg,
                                               0.0 * CLHEP::deg,  90.0 * CLHEP::deg);
    G4Sphere *lowerglasssphere = new G4Sphere (("solid_dom_lo_" + omName).c_str(),
                                               0.0  * CLHEP::m, glassOuterRadius_,
                                               0.0  * CLHEP::deg, 360.0 * CLHEP::deg,
                                               90.0 * CLHEP::deg, 180.0 * CLHEP::deg);
    
    G4double domInnerRadius = glassOuterRadius_ - glassThickness_;
    G4Sphere *upperdomsphere = new G4Sphere (("solid_inside_dom_up_" + omName).c_str(),
                                             0.0 * CLHEP::m, domInnerRadius,
                                             0.0 * CLHEP::deg, 360.0 * CLHEP::deg,
                                             0.0 * CLHEP::deg,  90.0 * CLHEP::deg);
    G4Sphere *lowerdomsphere = new G4Sphere (("solid_inside_dom_lo_" + omName).c_str(),
                                             0.0  * CLHEP::m, domInnerRadius,
                                             0.0  * CLHEP::deg, 360.0 * CLHEP::deg,
                                             90.0 * CLHEP::deg, 180.0 * CLHEP::deg);
    
    G4LogicalVolume* logUpperGlass =
        new G4LogicalVolume(upperglasssphere, glass,
                            ("log_dom_up_" + omName).c_str(), 0, 0, 0);
    G4LogicalVolume* logLowerGlass =
        new G4LogicalVolume(lowerglasssphere, glass,
                            ("log_dom_lo_" + omName).c_str(), 0, 0, 0);
    G4LogicalVolume* logUpperDOM =
        new G4LogicalVolume(upperdomsphere, effectiveDOM,
                            ("log_inside_dom_up_" + omName).c_str(), 0, 0, 0);
    G4LogicalVolume* logLowerDOM =
        new G4LogicalVolume(lowerdomsphere, effectiveDOM,
                            ("log_inside_dom_lo_" + omName).c_str(), 0, 0, 0);
    G4VPhysicalVolume* physUpperGlass ATTRIBUTE_UNUSED =
        new G4PVPlacement(0, upperDOMpos, logUpperGlass,
                          ("dom_up_" + omName).c_str(), logPerlite, false, 0);
    G4VPhysicalVolume* physLowerGlass ATTRIBUTE_UNUSED =
        new G4PVPlacement(0, lowerDOMpos, logLowerGlass,
                          ("dom_lo_" + omName).c_str(), logIce, false, 0);
    G4VPhysicalVolume* physUpperDOM ATTRIBUTE_UNUSED =
        new G4PVPlacement(0, G4ThreeVector(0,0,0), logUpperDOM,
                          ("inside_dom_up_" + omName).c_str(), logUpperGlass, false, 0);
    G4VPhysicalVolume* physLowerDOM ATTRIBUTE_UNUSED =
        new G4PVPlacement(0, G4ThreeVector(0,0,0), logLowerDOM,
                          ("inside_dom_lo_" + omName).c_str(), logLowerGlass, false, 0);
    
    // apply energy limits
    logUpperGlass->SetUserLimits(energyLimit);
    logLowerGlass->SetUserLimits(energyLimit);
    logUpperDOM->SetUserLimits(energyLimit);
    logLowerDOM->SetUserLimits(energyLimit);
  }
  
  // Define sensitive detector
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  iceSD_ = new G4TankIceSD(("ice_SD_" + tankName).c_str(), domPosIce);
  sdManager->AddNewDetector(iceSD_);
  logIce->SetSensitiveDetector(iceSD_);
  
  // Instantiation of a set of visualization attributes with red colour
  G4VisAttributes * tankVisAtt = new G4VisAttributes(G4Colour(1,0,0));
  // Set the forced wireframe style 
  //snowVisAtt->SetForceWireFrame(true);
  // Assignment of the visualization attributes to the logical volume
  tankLog_->SetVisAttributes(tankVisAtt);
  
  G4ThreeVector tankPos = position_ - origin - mother->GetTranslation();
  
  G4VPhysicalVolume* tankPhys = new G4PVPlacement(0, tankPos, tankLog_,
                                                  ("tank_" + tankName).c_str(),
                                                  mother->GetLogicalVolume(), false, 0);
  
  // apply energy limits
  tankLog_->SetUserLimits(energyLimit);
  logPerlite->SetUserLimits(energyLimit);
  logIce->SetUserLimits(energyLimit);
  
  return tankPhys;
}


double G4BeamTestTank::GetNumCherenkov(const OMKey& omKey)
{
  return std::max(iceSD_->GetNumCherenkov(omKey), 0.);
}


double G4BeamTestTank::GetNumCherenkovWeight(const OMKey& omKey)
{
  return std::max(iceSD_->GetNumCherenkovWeight(omKey), 0.);
}


double G4BeamTestTank::GetEDep_G4(const OMKey& omKey)
{
  return std::max(iceSD_->GetEDep(omKey), 0.);
}


double G4BeamTestTank::GetTime_G4(const OMKey& omKey)
{
  return iceSD_->GetTime(omKey);
}


double G4BeamTestTank::GetEDep_I3(const OMKey& omKey)
{
  return std::max(iceSD_->GetEDep(omKey), 0.) / CLHEP::keV * I3Units::keV;
}


double G4BeamTestTank::GetTime_I3(const OMKey& omKey)
{
  return (iceSD_->GetTime(omKey) / CLHEP::s) * I3Units::s;
}


I3Position G4BeamTestTank::GetPos_I3()
{
  I3Position pos((position_.x() / CLHEP::m) * I3Units::m,
                 (position_.y() / CLHEP::m) * I3Units::m,
                 (position_.z() / CLHEP::m) * I3Units::m);
  return pos;
}


double G4BeamTestTank::GetX_I3()
{
  return (position_.x() / CLHEP::m) * I3Units::m;
}


double G4BeamTestTank::GetY_I3()
{
  return (position_.y() / CLHEP::m) * I3Units::m;
}


double G4BeamTestTank::GetZ_I3()
{
  return (position_.z() / CLHEP::m) * I3Units::m;
}


double G4BeamTestTank::GetTankHeight_I3()
{
  return (tankHeight_ / CLHEP::m) * I3Units::m;
}


double G4BeamTestTank::GetTankRadius_I3()
{
  return (outerRadius_ / CLHEP::m) * I3Units::m;
}


double G4BeamTestTank::GetSnowHeight_I3()
{
  return (snowHeight_ / CLHEP::m) * I3Units::m;
}

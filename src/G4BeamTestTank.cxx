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

/* #include <boost/foreach.hpp> */

#include "G4BeamTestTank.h"
#include "G4TankIceSD.h"

//prevent gcc to make something stupid with pretended unused variables
#ifdef __GNUC__
#define ATTRIBUTE_UNUSED __attribute__((unused))
#else
#define ATTRIBUTE_UNUSED
#endif


G4BeamTestTank::G4BeamTestTank()
{
  // Get tank dimensions
  tankThickness_ = 0.5*CLHEP::cm; // TODO(shivesh) : check thickness
  tankHeight_  = 76.83 * 2.54 * CLHEP::cm;
  innerRadius_ = 32 * 2.54 * CLHEP::cm;
  outerRadius_ = innerRadius_ + tankThickness_;

  // Get fill height
  fillHeight_ = 55.1 * 2.54 * CLHEP::cm;

  // Set DOM dimensions
  glassOuterRadius_ = 6.5 * 2.54 * CLHEP::cm; // 6.5" outer glass sphere radius
  glassThickness_   = 0.5 * 2.54 * CLHEP::cm; // 0.5" glass sphere thickness
}


G4BeamTestTank::~G4BeamTestTank()
{

}


G4VPhysicalVolume* G4BeamTestTank::InstallTank(G4VPhysicalVolume* mother, const G4ThreeVector& origin)
{
  // User limits (energy cutoffs)
  // Do not create photons or electrons below cherenkov threshold
  // See also corresponding UserSpecialCuts in Physicslist !!!!
  // TODO(shivesh): Maybe do all of this as stepping action ??????
  G4UserLimits* energyLimit = new G4UserLimits();
  energyLimit->SetUserMinEkine(264.1 * CLHEP::keV);  // Cherenkov threshold of electrons in water

  // std::string tankName=boost::lexical_cast<std::string>(tankKey_);
  std::string tankName = "BTT";

  // Define plastic frame TODO(shivesh): plastic or polyethelene?
  G4Material* plastic = G4Material::GetMaterial("Plastic");
  G4Tubs* solidTank = new G4Tubs(("solid_tank_" + tankName).c_str(),
                                 0.0 * CLHEP::m, outerRadius_, 0.5 * tankHeight_,
                                 0.0 * CLHEP::deg, 360.0 * CLHEP::deg);
  tankLog_ = new G4LogicalVolume(solidTank, plastic,
                                 ("log_tank_" + tankName).c_str(), 0, 0, 0);

  // Define water volume
  G4Material* water = G4Material::GetMaterial("Water");
  G4Tubs* solidWater = new G4Tubs(("solid_water_" + tankName).c_str(),
                                0.0 * CLHEP::m, innerRadius_, 0.5 * fillHeight_,
                                0.0 * CLHEP::deg, 360.0 * CLHEP::deg);
  G4LogicalVolume* logWater =
      new G4LogicalVolume(solidWater, water, ("log_water_" + tankName).c_str(), 0, 0, 0);
  G4ThreeVector physWaterPosition(0, 0, -0.5*tankHeight_ + tankThickness_ + 0.5*fillHeight_);
  G4VPhysicalVolume* physWater ATTRIBUTE_UNUSED =
      new G4PVPlacement(0, physWaterPosition, logWater,
                        ("water_" + tankName).c_str(), tankLog_, false, 0);

  // Define air volume
  G4Material* air = G4Material::GetMaterial("Air");
  G4Tubs* solidAir = new G4Tubs(("solid_air_" + tankName).c_str(),
                                    0.0 * CLHEP::m, innerRadius_, 0.5 * airHeight_,
                                    0.0 * CLHEP::deg, 360.0 * CLHEP::deg);
  G4LogicalVolume* logAir =
      new G4LogicalVolume(solidAir, air, ("log_air_" + tankName).c_str(), 0, 0, 0);
  G4ThreeVector physAirPosition(0, 0, -0.5 * tankHeight_ + 0.5 * CLHEP::cm + fillHeight_ +
                                    0.5 * airHeight_);
  G4VPhysicalVolume* physAir_UNUSED =
      new G4PVPlacement(0, physAirPosition, logAir,
                        ("air_" + tankName).c_str(), tankLog_, false, 0);

  // Define glass sphere & effective DOM material splitted in upper and lower part
  G4Material* glass        = G4Material::GetMaterial("Glass");
  G4Material* effectiveDOM = G4Material::GetMaterial("effectiveDOM");

  // std::map<OMKey, G4ThreeVector> domPosIce;
  // std::map<OMKey, G4ThreeVector>::const_iterator om_iter;
  // for(om_iter=relDomPositions_.begin(); om_iter!=relDomPositions_.end(); ++om_iter)
  // {
  /* const OMKey& omKey = om_iter->first; */

  G4ThreeVector upperDOMpos(0, 0, -0.5 * airHeight_);
  G4ThreeVector lowerDOMpos(0, 0,  0.5 * fillHeight_);

  // domPosIce[omKey] = lowerDOMpos;

  // std::string omName=boost::lexical_cast<std::string>(omKey);
  std::string omName="BTOM";

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
                        ("dom_up_" + omName).c_str(), logAir, false, 0);
  G4VPhysicalVolume* physLowerGlass ATTRIBUTE_UNUSED =
      new G4PVPlacement(0, lowerDOMpos, logLowerGlass,
                        ("dom_lo_" + omName).c_str(), logWater, false, 0);
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
  // }

  // Define sensitive detector TODO(shivesh): make the PMT the SD
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  iceSD_ = new G4TankIceSD(("ice_SD_" + tankName).c_str());
  sdManager->AddNewDetector(iceSD_);
  logWater->SetSensitiveDetector(iceSD_);

  // Instantiation of a set of visualization attributes with red colour
  G4VisAttributes * tankVisAtt = new G4VisAttributes(G4Colour(1,0,0));
  // Set the forced wireframe style
  //snowVisAtt->SetForceWireFrame(true);
  // Assignment of the visualization attributes to the logical volume
  tankLog_->SetVisAttributes(tankVisAtt);

  G4ThreeVector tankPos = position_ - origin - mother->GetTranslation();
  G4cout << "tankPos = " << tankPos << G4endl;

  G4VPhysicalVolume* tankPhys = new G4PVPlacement(0, tankPos, tankLog_,
                                                  ("tank_" + tankName).c_str(),
                                                  mother->GetLogicalVolume(), false, 0);

  // apply energy limits
  tankLog_->SetUserLimits(energyLimit);
  logAir->SetUserLimits(energyLimit);
  logWater->SetUserLimits(energyLimit);

  return tankPhys;
}


double G4BeamTestTank::GetNumCherenkov(/* const OMKey& omKey */)
{
  return std::max(iceSD_->GetNumCherenkov(/* omKey */), 0.);
}


double G4BeamTestTank::GetNumCherenkovWeight(/* const OMKey& omKey */)
{
  return std::max(iceSD_->GetNumCherenkovWeight(/* omKey */), 0.);
}


double G4BeamTestTank::GetEDep_G4(/* const OMKey& omKey */)
{
  return std::max(iceSD_->GetEDep(/* omKey */), 0.);
}


double G4BeamTestTank::GetTime_G4(/* const OMKey& omKey */)
{
  return iceSD_->GetTime(/* omKey */);
}


/* double G4BeamTestTank::GetEDep_I3(const OMKey& omKey) */
/* { */
/*   return std::max(iceSD_->GetEDep(omKey), 0.) / CLHEP::keV * I3Units::keV; */
/* } */


/* double G4BeamTestTank::GetTime_I3(const OMKey& omKey) */
/* { */
/*   return (iceSD_->GetTime(omKey) / CLHEP::s) * I3Units::s; */
/* } */


/* I3Position G4BeamTestTank::GetPos_I3() */
/* { */
/*   I3Position pos((position_.x() / CLHEP::m) * I3Units::m, */
/*                  (position_.y() / CLHEP::m) * I3Units::m, */
/*                  (position_.z() / CLHEP::m) * I3Units::m); */
/*   return pos; */
/* } */


/* double G4BeamTestTank::GetX_I3() */
/* { */
/*   return (position_.x() / CLHEP::m) * I3Units::m; */
/* } */
/*  */
/*  */
/* double G4BeamTestTank::GetY_I3() */
/* { */
/*   return (position_.y() / CLHEP::m) * I3Units::m; */
/* } */
/*  */
/*  */
/* double G4BeamTestTank::GetZ_I3() */
/* { */
/*   return (position_.z() / CLHEP::m) * I3Units::m; */
/* } */
/*  */

/* double G4BeamTestTank::GetTankHeight_I3() */
/* { */
/*   return (tankHeight_ / CLHEP::m) */
/* } */
/*  */
/*  */
/* double G4BeamTestTank::GetTankRadius_I3() */
/* { */
/*   return (outerRadius_ / CLHEP::m) */
/* } */
/*  */
/*  */
/* double G4BeamTestTank::GetFillHeight_I3() */
/* { */
/*   return (fillHeight_ / CLHEP::m) */
/* } */

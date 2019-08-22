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
#include "G4BeamTestSiSD.h"
/* #include "G4TankIceSD.h" */

//prevent gcc to make something stupid with pretended unused variables
#ifdef __GNUC__
#define ATTRIBUTE_UNUSED __attribute__((unused))
#else
#define ATTRIBUTE_UNUSED
#endif


G4BeamTestTank::G4BeamTestTank()
{
  // Get tank dimensions
  // tankThickness_ = 0.0*CLHEP::cm;
  tankThickness_ = 0.44 * 2.54 *CLHEP::cm;
  tankHeight_  = 76.83 * 2.54 * CLHEP::cm;
  innerRadius_ = 32 * 2.54 * CLHEP::cm;
  outerRadius_ = innerRadius_ + tankThickness_;

  // Get fill height
  fillHeight_ = 55.1 * 2.54 * CLHEP::cm;
  airHeight_ = tankHeight_ - fillHeight_;

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
  G4UserLimits* energyLimit = new G4UserLimits();
  energyLimit->SetUserMinEkine(2.26 * CLHEP::eV);  // Lower threshold of PMT - 550nm

  G4Material* plastic = G4Material::GetMaterial("Plastic");
  G4Tubs* solidTank = new G4Tubs(
          "solid_tank_",
          0.0 * CLHEP::m, outerRadius_, 0.5 * tankHeight_,
          0.0 * CLHEP::deg, 360.0 * CLHEP::deg
          );
  tankLog_ = new G4LogicalVolume(
          solidTank, plastic, "log_tank_", 0, 0, 0
          );

  // Define water volume
  G4Material* water = G4Material::GetMaterial("Water");
  G4Tubs* solidWater = new G4Tubs(
          "solid_water_",
          0.0 * CLHEP::m, innerRadius_, 0.5 * fillHeight_,
          0.0 * CLHEP::deg, 360.0 * CLHEP::deg
          );
  G4LogicalVolume* logWater =
      new G4LogicalVolume(solidWater, water, "log_water_", 0, 0, 0);
  G4ThreeVector physWaterPosition(
          0, 0, -0.5*tankHeight_ + tankThickness_ + 0.5*fillHeight_
          );
  G4VPhysicalVolume* physWater ATTRIBUTE_UNUSED =
      new G4PVPlacement(
              0, physWaterPosition, logWater, "water_", tankLog_, false, 0
              );

  // Define air volume
  G4Material* air = G4Material::GetMaterial("Air");
  G4Tubs* solidAir = new G4Tubs(
          "solid_air_",
          0.0 * CLHEP::m, innerRadius_, 0.5 * airHeight_,
          0.0 * CLHEP::deg, 360.0 * CLHEP::deg
          );
  G4LogicalVolume* logAir =
      new G4LogicalVolume(solidAir, air, "log_air_", 0, 0, 0);
  G4ThreeVector physAirPosition(0, 0,
          -0.5 * tankHeight_ + 0.5 * CLHEP::cm + fillHeight_ + 0.5 * airHeight_
          );
  G4VPhysicalVolume* physAir_UNUSED =
      new G4PVPlacement(
              0, physAirPosition, logAir, "air_", tankLog_, false, 0
              );

  // Define glass sphere & effective DOM material splitted in upper and lower part
  G4Material* glass        = G4Material::GetMaterial("Glass");
  G4Material* effectiveDOM = G4Material::GetMaterial("effectiveDOM");

  G4ThreeVector upperDOMpos(0, 0, -0.5 * airHeight_);
  G4ThreeVector lowerDOMpos(0, 0,  0.5 * fillHeight_);

  G4Sphere *upperglasssphere = new G4Sphere ("solid_dom_up_",
                                             0.0 * CLHEP::m, glassOuterRadius_,
                                             0.0 * CLHEP::deg, 360.0 * CLHEP::deg,
                                             0.0 * CLHEP::deg,  90.0 * CLHEP::deg);
  G4Sphere *lowerglasssphere = new G4Sphere ("solid_dom_lo_",
                                             0.0  * CLHEP::m, glassOuterRadius_,
                                             0.0  * CLHEP::deg, 360.0 * CLHEP::deg,
                                             90.0 * CLHEP::deg, 180.0 * CLHEP::deg);

  G4double domInnerRadius = glassOuterRadius_ - glassThickness_;
  G4Sphere *upperdomsphere = new G4Sphere ("solid_inside_dom_up_",
                                           0.0 * CLHEP::m, domInnerRadius,
                                           0.0 * CLHEP::deg, 360.0 * CLHEP::deg,
                                           0.0 * CLHEP::deg,  90.0 * CLHEP::deg);
  G4Sphere *lowerdomsphere = new G4Sphere ("solid_inside_dom_lo_",
                                           0.0  * CLHEP::m, domInnerRadius,
                                           0.0  * CLHEP::deg, 360.0 * CLHEP::deg,
                                           90.0 * CLHEP::deg, 180.0 * CLHEP::deg);

  G4LogicalVolume* logUpperGlass =
      new G4LogicalVolume(upperglasssphere, glass,
                          "log_dom_up_", 0, 0, 0);
  G4LogicalVolume* logLowerGlass =
      new G4LogicalVolume(lowerglasssphere, glass,
                          "log_dom_lo_", 0, 0, 0);
  G4LogicalVolume* logUpperDOM =
      new G4LogicalVolume(upperdomsphere, effectiveDOM,
                          "log_inside_dom_up_", 0, 0, 0);
  G4LogicalVolume* logLowerDOM =
      new G4LogicalVolume(lowerdomsphere, effectiveDOM,
                          "log_inside_dom_lo_", 0, 0, 0);
  G4VPhysicalVolume* physUpperGlass ATTRIBUTE_UNUSED =
      new G4PVPlacement(0, upperDOMpos, logUpperGlass,
                        "dom_up_", logAir, false, 0);
  G4VPhysicalVolume* physLowerGlass ATTRIBUTE_UNUSED =
      new G4PVPlacement(0, lowerDOMpos, logLowerGlass,
                        "dom_lo_", logWater, false, 0);
  G4VPhysicalVolume* physUpperDOM ATTRIBUTE_UNUSED =
      new G4PVPlacement(0, G4ThreeVector(0,0,0), logUpperDOM,
                        "inside_dom_up_", logUpperGlass, false, 0);
  G4VPhysicalVolume* physLowerDOM ATTRIBUTE_UNUSED =
      new G4PVPlacement(0, G4ThreeVector(0,0,0), logLowerDOM,
                        "inside_dom_lo_", logLowerGlass, false, 0);

  // apply energy limits
  logUpperGlass->SetUserLimits(energyLimit);
  logLowerGlass->SetUserLimits(energyLimit);
  logUpperDOM->SetUserLimits(energyLimit);
  logLowerDOM->SetUserLimits(energyLimit);
  // }

  // Define sensitive detector
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  iceSD_ = new G4BeamTestSiSD("ice_SD_", "HitsCollection");
  sdManager->AddNewDetector(iceSD_);
  // logLowerDOM->SetSensitiveDetector(iceSD_);
  logLowerGlass->SetSensitiveDetector(iceSD_);

  // Instantiation of a set of visualization attributes with red colour
  G4VisAttributes * tankVisAtt = new G4VisAttributes(G4Colour(1,0,0));
  // Assignment of the visualization attributes to the logical volume
  tankLog_->SetVisAttributes(tankVisAtt);

  // G4ThreeVector tankPos = position_ - origin - mother->GetTranslation();
  G4ThreeVector tankPos = origin;
  G4VPhysicalVolume* tankPhys = new G4PVPlacement(
          0, tankPos, tankLog_, "tank_", mother->GetLogicalVolume(), false, 0
          );

  // apply energy limits
  tankLog_->SetUserLimits(energyLimit);
  logAir->SetUserLimits(energyLimit);
  logWater->SetUserLimits(energyLimit);

  return tankPhys;
}

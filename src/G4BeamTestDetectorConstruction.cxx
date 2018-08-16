#include <G4NistManager.hh>
#include <G4Material.hh>

#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>

#include <G4Box.hh>

#include <G4VisAttributes.hh>
#include <G4UserLimits.hh>

#include "G4BeamTestDetectorConstruction.h"
#include "G4BeamTestTank.h"


G4BeamTestDetectorConstruction::G4BeamTestDetectorConstruction():
  origin_(NAN, NAN, NAN), verboseLevel_(0)/* , tankList_(0) */
{
}


G4BeamTestDetectorConstruction::~G4BeamTestDetectorConstruction()
{
}


G4VPhysicalVolume* G4BeamTestDetectorConstruction::Construct()
{
  /* if(tankList_.empty()) return NULL; */

  CreateMaterials();

  // Determine bottom z position of snow volume
  G4double zSnowBottom(tankList_.at(0)->GetPos().z());
  BOOST_FOREACH(G4BeamTestTank* tank, tankList_)
  {
    // z position of bottom of tank
    G4double z = tank->GetPos().z() - 0.5*tank->GetTankHeight_G4();
    zSnowBottom = std::min(z, zSnowBottom);
  }

  // Subtract safety margin
  zSnowBottom -= 1.0*CLHEP::m;

  // Triangulate snow surface
  G4Delaunay delaunay;
  BOOST_FOREACH(G4BeamTestTank* tank, tankList_)
  {
    // z position of snow surface
    G4double z = tank->GetPos().z() + 0.5 * tank->GetTankHeight_G4() + tank->GetSnowHeight_G4();

    delaunay.AddPoint(tank->GetDelaunayPoint1().x(),
                      tank->GetDelaunayPoint1().y(),
                      z - zSnowBottom);
    delaunay.AddPoint(tank->GetDelaunayPoint2().x(),
                      tank->GetDelaunayPoint2().y(),
                      z - zSnowBottom);
    delaunay.AddPoint(tank->GetDelaunayPoint3().x(),
                      tank->GetDelaunayPoint3().y(),
                      z - zSnowBottom);
  }

  // Create tesselated snow volume
  G4TessellatedSolid* solidSnow = new G4TessellatedSolid("solid_snow");
  delaunay.BuildSolid(solidSnow, 50.0*CLHEP::m, 100.0*CLHEP::m);

  // Determine World dimensions
  G4double xHalfLength = 0.5 * (delaunay.GetXmax() - delaunay.GetXmin());
  G4double yHalfLength = 0.5 * (delaunay.GetYmax() - delaunay.GetYmin());
  G4double zHalfLength = 0.5 * (delaunay.GetZmax() + 20.0 * CLHEP::m); // 20 m of atmosphere

  // World origin in IceCube coordinates
  origin_.set(delaunay.GetOrigin().x(), delaunay.GetOrigin().y(), zSnowBottom + zHalfLength);

  // Create world volume
  G4Box* world_box = new G4Box("solid_world", xHalfLength, yHalfLength, zHalfLength);
  G4LogicalVolume* worldLog =
      new G4LogicalVolume(world_box, G4Material::GetMaterial("Air"), "log_world", 0, 0, 0);
  G4VPhysicalVolume* worldPhys =
      new G4PVPlacement(0, G4ThreeVector(), worldLog, "world", 0, false, 0);

  // Snow layer
  G4LogicalVolume* snowLog =
      new G4LogicalVolume(solidSnow, G4Material::GetMaterial("Snow"), "log_snow", 0, 0, 0);
  G4VPhysicalVolume* snowPhys =
      new G4PVPlacement(0, G4ThreeVector(0, 0, -zHalfLength), snowLog, "snow", worldLog, false, 0);

  // Instantiation of a set of visualization attributes with cyan colour
  G4VisAttributes * snowVisAtt = new G4VisAttributes(G4Colour(0., 1., 1.));
  // Assignment of the visualization attributes to the logical volume
  snowLog->SetVisAttributes(snowVisAtt);

  // Install tanks
  BOOST_FOREACH(G4BeamTestTank* tank, tankList_)
  {
    tank->InstallTank(snowPhys, origin_);
  }

  // User limits (energy cutoffs)
  // Do not create photons or electrons below cherenkov threshold
  // See also corresponding UserSpecialCuts in Physicslist !!!!
  G4UserLimits* energyLimit = new G4UserLimits();
  energyLimit->SetUserMinEkine(280.0 * CLHEP::keV);  // Cherenkov threshold of electrons in ice TODO(shivesh)
  worldLog->SetUserLimits(energyLimit);
  snowLog->SetUserLimits(energyLimit);
  
  return worldPhys;
}

/*****************************************************************/

void G4BeamTestDetectorConstruction::CreateMaterials()
{
  CreateAir();
  /* CreateIce(); */
  /* CreateSnow(); */
  CreateWater();
  CreatePlastic();
  /* CreatePerlite(); */
  CreateGlassSphere();
  CreateEffectiveDOMMaterial();

  //if(verboseLevel_>0) G4cout << *G4Material::GetMaterialTable() << G4endl;
}

/*****************************************************************/

void G4BeamTestDetectorConstruction::CreateAir()
{
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->ConstructNewGasMaterial("Air","G4_AIR"/* , (273.15 - 40.0)*CLHEP::kelvin, 670.0E-3*CLHEP::bar */);
}

/*****************************************************************/

void G4BeamTestDetectorConstruction::CreateWater()
{
  G4NistManager* nistManager = G4NistManager::Instance();
  // G4Material* ice = new G4Material("Water", 1.0 * CLHEP::g / CLHEP::cm3, 2, kStateLiquid);
  // ice->AddElement(nistManager->FindOrBuildElement("H"), 2);
  // ice->AddElement(nistManager->FindOrBuildElement("O"), 1);
  nistManager->ConstructNewGasMaterial("Water","G4_WATER");
}

/*****************************************************************/

void G4BeamTestDetectorConstruction::CreatePlastic()
{
  G4NistManager* nistManager = G4NistManager::Instance();
  /*
  G4Material* plastic = new G4Material("Plastic", 1.5 * CLHEP::g / CLHEP::cm3, 3, kStateSolid);
  plastic->AddElement(nistManager->FindOrBuildElement("H"), 20);
  plastic->AddElement(nistManager->FindOrBuildElement("C"), 10);
  plastic->AddElement(nistManager->FindOrBuildElement("O"), 5);
  */
  
  // POM
  G4Material* plastic = new G4Material("Plastic", 1.425 * CLHEP::g / CLHEP::cm3, 3, kStateSolid);
  plastic->AddElement(nistManager->FindOrBuildElement("H"), 2);
  plastic->AddElement(nistManager->FindOrBuildElement("C"), 1);
  plastic->AddElement(nistManager->FindOrBuildElement("O"), 1);
  
  //nistManager->FindOrBuildMaterial("G4_POLYOXYMETHYLENE");
  
}

/*****************************************************************/

void G4BeamTestDetectorConstruction::CreateGlassSphere()
{
  // Elemental composition not exact for detailed line-up look at
  // docushare collection 690 -> XRF and ICP Analysis of Pressure Sphere Glass

  // 20 lbs. weight = 9072 g
  // 6.5" outer radius & 0.5" thickness = 4024 cm3
  G4NistManager* nistManager = G4NistManager::Instance();
  G4Material* glass = new G4Material("Glass", 2.254 * CLHEP::g / CLHEP::cm3, 2, kStateSolid);
  glass->AddElement(nistManager->FindOrBuildElement("Si"), 1);
  glass->AddElement(nistManager->FindOrBuildElement("O"),  2);
}

/*****************************************************************/

void G4BeamTestDetectorConstruction::CreateEffectiveDOMMaterial()
{
  // Mass of the a complete DOM: 12050 g
  // Mass without glass sphere: 2978 g
  // 6" inner glass radius: volume = 14830 cm3
  G4NistManager* nistManager = G4NistManager::Instance();
  G4Material* glass = new G4Material("effectiveDOM", 0.2 * CLHEP::g / CLHEP::cm3, 2, kStateSolid);
  glass->AddElement(nistManager->FindOrBuildElement("Si"), 1);
  glass->AddElement(nistManager->FindOrBuildElement("O"),  2);
}

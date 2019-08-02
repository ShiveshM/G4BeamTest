#include <G4NistManager.hh>
#include <G4Material.hh>

#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>

#include <G4Box.hh>

#include <G4VisAttributes.hh>
#include <G4UserLimits.hh>

#include "G4BeamTestDetectorConstruction.h"
#include "G4BeamTestTank.h"


G4BeamTestDetectorConstruction::G4BeamTestDetectorConstruction():
  origin_(0, 0, 0), verboseLevel_(0)/* , tankList_(0) */
{
}


G4BeamTestDetectorConstruction::~G4BeamTestDetectorConstruction()
{
}


G4VPhysicalVolume* G4BeamTestDetectorConstruction::Construct()
{
  /* if(tankList_.empty()) return NULL; */

  CreateMaterials();

  /* // World origin in IceCube coordinates */
  /* origin_.set(delaunay.GetOrigin().x(), delaunay.GetOrigin().y(), zSnowBottom + zHalfLength); */

  // Determine World dimensions
  G4double xWorld = 4.0 * CLHEP::m;
  G4double yWorld = 4.0 * CLHEP::m;
  G4double zWorld = 4.0 * CLHEP::m;

  // SC4 dimensions
  G4double scinHeight_ = 1 * 2.54 * CLHEP::cm;
  G4double scinWidth_ = 1 * 2.54 * CLHEP::cm;
  G4double scinThickness_ = 1 * 2.54 * CLHEP::cm;

  // Create world volume
  G4Box* world_box = new G4Box("solid_world", xWorld*0.5, yWorld*0.5, zWorld*0.5);
  G4LogicalVolume* worldLog =
      new G4LogicalVolume(world_box, G4Material::GetMaterial("Air"), "log_world", 0, 0, 0);
  G4VPhysicalVolume* worldPhys =
      new G4PVPlacement(0, G4ThreeVector(), worldLog, "world", 0, false, 0);

  /* // Instantiation of a set of visualization attributes with cyan colour */
  /* G4VisAttributes * snowVisAtt = new G4VisAttributes(G4Colour(0., 1., 1.)); */
  /* // Assignment of the visualization attributes to the logical volume */
  /* snowLog->SetVisAttributes(snowVisAtt); */

  // Install tank
  /* BOOST_FOREACH(G4BeamTestTank* tank, tankList_) */
  /* { */
  tank_->InstallTank(worldPhys, origin_);
  // }

  // Define SC4
  G4Box* sc4_box = new G4Box("sc4",scinHeight_*0.5, scinWidth_*0.5, scinThickness_*0.5);
  G4LogicalVolume* sc4Log =
      new G4LogicalVolume(sc4_box, G4Material::GetMaterial("SC4"), "log_sc4", 0, 0, 0);
  G4VPhysicalVolume* sc4Phys =
      new G4PVPlacement(0, G4ThreeVector(1.2*CLHEP::m,0,0), sc4Log, "sc4", worldLog, false, 0);

  // User limits (energy cutoffs)
  // Do not create photons or electrons below cherenkov threshold
  // See also corresponding UserSpecialCuts in Physicslist !!!!
  G4UserLimits* energyLimit = new G4UserLimits();
  /* energyLimit->SetUserMinEkine(280.0 * CLHEP::keV);  // Cherenkov threshold of electrons in ice */
  energyLimit->SetUserMinEkine(2.26 * CLHEP::eV);  // Lower threshold of PMT - 550nm
  // energyLimit->SetUserMaxEkine(3.55 * CLHEP::eV); //upper threshold of PMT - 350nm
  worldLog->SetUserLimits(energyLimit);
  /* snowLog->SetUserLimits(energyLimit); */
  sc4Log->SetUserLimits(energyLimit);
  
  // G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  // sc4SD_ = new G4BeamTestSC4SD("sc4_SD_", "HitsCollection");
  // sdManager->AddNewDetector(sc4SD_);
  // sc4Log->SetSensitiveDetector(sc4SD_);

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
  CreateSC4();

  //if(verboseLevel_>0) G4cout << *G4Material::GetMaterialTable() << G4endl;
}

/*****************************************************************/

void G4BeamTestDetectorConstruction::CreateAir()
{
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->ConstructNewGasMaterial("Air","G4_AIR" , (273.15 + 20.0)*CLHEP::kelvin, 1.0*CLHEP::atmosphere);
}

/*****************************************************************/

void G4BeamTestDetectorConstruction::CreateWater()
{
  G4NistManager* nistManager = G4NistManager::Instance();
  G4Material* water = new G4Material("Water", 1.0 * CLHEP::g / CLHEP::cm3, 2, kStateLiquid);
  water->AddElement(nistManager->FindOrBuildElement("H"), 2);
  water->AddElement(nistManager->FindOrBuildElement("O"), 1);

  // pmt spectral response 300-650nm
  // https://docushare.icecube.wisc.edu/dsweb/Get/Document-6637/R7081-02%20data%20sheet.pdf<Paste>
  // TODO(shivesh): add more properties?
  const G4int water_bins = 2;
  // G4double water_ephot[water_bins] = {1.91 * CLHEP::eV, 4.13 * CLHEP::eV};
  G4double water_ephot[water_bins] = {0.1 * CLHEP::eV, 10 * CLHEP::eV};
  G4double water_refr[water_bins] = {1.33, 1.33};
  G4MaterialPropertiesTable *mpt_water = new G4MaterialPropertiesTable ();
  mpt_water->AddProperty ("RINDEX", water_ephot, water_refr, water_bins);
  water->SetMaterialPropertiesTable(mpt_water);
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
  // G4Material* glass = new G4Material("Glass", 2.254 * CLHEP::g / CLHEP::cm3, 2, kStateSolid);
  // glass->AddElement(nistManager->FindOrBuildElement("Si"), 1);
  // glass->AddElement(nistManager->FindOrBuildElement("O"),  2);

  // http://hypernews.slac.stanford.edu/HyperNews/geant4/get/AUX/2013/03/11/12.39-85121-chDetectorConstruction.cc
  // Define elements for all materials not found in the NIST database
  G4Element* Si = nistManager->FindOrBuildElement("Si");
  G4Element* B = nistManager->FindOrBuildElement("B");
  G4Element* O = nistManager->FindOrBuildElement("O");
  G4Element* Na = nistManager->FindOrBuildElement("Na");
  G4Element* Al = nistManager->FindOrBuildElement("Al");
  G4Element* K = nistManager->FindOrBuildElement("K");  

  G4double density;
  G4int ncomponents;
  G4double fractionmass;
  G4Material* glass = new G4Material("Glass", density= 2.23*CLHEP::g/CLHEP::cm3, ncomponents=6);
  glass->AddElement(B, fractionmass=0.040064);
  glass->AddElement(O, fractionmass=0.539562); 
  glass->AddElement(Na, fractionmass=0.028191);
  glass->AddElement(Al, fractionmass=0.011644);
  glass->AddElement(Si, fractionmass=0.377220);
  glass->AddElement(K, fractionmass=0.003321);

  // pmt spectral response 300-650nm
  // https://docushare.icecube.wisc.edu/dsweb/Get/Document-6637/R7081-02%20data%20sheet.pdf<Paste>
  // TODO(shivesh): add more properties?
  const G4int glass_bins = 2;
  // G4double glass_ephot[glass_bins] = {1.91 * CLHEP::eV, 4.13 * CLHEP::eV};
  G4double glass_ephot[glass_bins] = {0.1 * CLHEP::eV, 10 * CLHEP::eV};
  G4double glass_refr[glass_bins] = {1.47, 1.47};
  G4MaterialPropertiesTable *mpt_glass = new G4MaterialPropertiesTable ();
  mpt_glass->AddProperty ("RINDEX", glass_ephot, glass_refr, glass_bins);
  glass->SetMaterialPropertiesTable(mpt_glass);
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

  // const G4int glass_bins = 2;
  // G4double glass_ephot[glass_bins] = {1.91 * CLHEP::eV, 4.13 * CLHEP::eV};
  // glass 
  // G4double glass_ephot[glass_bins] = {0.1 * CLHEP::eV, 10 * CLHEP::eV};
  // G4double glass_refr[glass_bins] = {1.47, 1.47};
  // G4MaterialPropertiesTable *mpt_glass = new G4MaterialPropertiesTable ();
  // mpt_glass->AddProperty ("RINDEX", glass_ephot, glass_refr, glass_bins);
  // glass->SetMaterialPropertiesTable(mpt_glass);
}

void G4BeamTestDetectorConstruction::CreateSC4()
{
  G4NistManager* nistManager = G4NistManager::Instance();
  // POM
  G4Material* plastic = new G4Material("SC4", 1.425 * CLHEP::g / CLHEP::cm3, 3, kStateSolid);
  plastic->AddElement(nistManager->FindOrBuildElement("H"), 2);
  plastic->AddElement(nistManager->FindOrBuildElement("C"), 1);
  plastic->AddElement(nistManager->FindOrBuildElement("O"), 1);
}

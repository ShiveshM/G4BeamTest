#include <G4NistManager.hh>
#include <G4Material.hh>

#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include "G4SystemOfUnits.hh"

#include <G4Box.hh>

#include <G4VisAttributes.hh>
#include <G4UserLimits.hh>

#include "G4BeamTestDetectorConstruction.h"
#include "G4BeamTestTank.h"

#include "G4BeamTestSC4SD.h"

G4BeamTestDetectorConstruction::G4BeamTestDetectorConstruction():
  origin_(0, 0, 0), verboseLevel_(0)/* , tankList_(0) */
{
}


G4BeamTestDetectorConstruction::~G4BeamTestDetectorConstruction()
{
}


G4VPhysicalVolume* G4BeamTestDetectorConstruction::Construct()
{
  CreateMaterials();

  // Determine World dimensions
  // G4double xWorld = 20.0 * CLHEP::m;
  G4double xWorld = 4.0 * CLHEP::m;
  G4double yWorld = 4.0 * CLHEP::m;
  G4double zWorld = 4.0 * CLHEP::m;

  // SC4 dimensions
  G4double scinX_ = 0.25 * 2.54 * CLHEP::cm;
  G4double scinY_ = 1 * 2.54 * CLHEP::cm;
  G4double scinZ_ = 1 * 2.54 * CLHEP::cm;

  // Create world volume
  G4Box* world_box = new G4Box("solid_world", xWorld*0.5, yWorld*0.5, zWorld*0.5);
  G4LogicalVolume* worldLog =
      new G4LogicalVolume(world_box, G4Material::GetMaterial("Air"), "log_world", 0, 0, 0);
  G4VPhysicalVolume* worldPhys =
      new G4PVPlacement(0, G4ThreeVector(), worldLog, "world", 0, false, 0);

  // Location calculations
  G4ThreeVector mwpc1Vec = G4ThreeVector(198.8*CLHEP::cm-xWorld*0.5, 0, 0);
  G4ThreeVector sc1Vec   = mwpc1Vec + G4ThreeVector(-50.0*CLHEP::cm, 0, 0);
  G4ThreeVector sc2Vec   = mwpc1Vec + G4ThreeVector( 80.0*CLHEP::cm, 0, 0);
  G4ThreeVector sc3Vec   = mwpc1Vec + G4ThreeVector(180.0*CLHEP::cm, 0, 0);
  G4ThreeVector mwpc2Vec = mwpc1Vec + G4ThreeVector(275.3*CLHEP::cm, 0, 0);
  G4ThreeVector mwpc3Vec = mwpc2Vec + G4ThreeVector(284.6*CLHEP::cm, 0, 0);
  G4ThreeVector mwpc4Vec = mwpc3Vec + G4ThreeVector(771.8*CLHEP::cm, 0, 0);
  // G4ThreeVector tankVec  = mwpc4Vec + G4ThreeVector(300.0*CLHEP::cm, 0, 0);
  G4ThreeVector tankVec  = G4ThreeVector(0, 0, 0);
  G4ThreeVector sc4Vec   = tankVec +  G4ThreeVector(120.0*CLHEP::cm, 0, 0);
  G4ThreeVector steelVec = tankVec +  G4ThreeVector(-120.0*CLHEP::cm, 0, 0);

  tank_->InstallTank(worldPhys, tankVec);

  // // Define SC1
  // G4Box* sc1_box = new G4Box("sc1",scinX_*0.5, scinY_*2.0, scinZ_*2.0);
  // G4LogicalVolume* sc1Log =
  //     new G4LogicalVolume(sc1_box, G4Material::GetMaterial("SC4"), "log_sc1", 0, 0, 0);
  // G4VPhysicalVolume* sc1Phys =
  //     new G4PVPlacement(0, sc1Vec, sc1Log, "sc1", worldLog, false, 0);

  // // Define SC2
  // G4Box* sc2_box = new G4Box("sc2",scinX_*0.5, scinY_*2.0, scinZ_*2.0);
  // G4LogicalVolume* sc2Log =
  //     new G4LogicalVolume(sc2_box, G4Material::GetMaterial("SC4"), "log_sc2", 0, 0, 0);
  // G4VPhysicalVolume* sc2Phys =
  //     new G4PVPlacement(0, sc2Vec, sc2Log, "sc2", worldLog, false, 0);

  // // Define SC3
  // G4Box* sc3_box = new G4Box("sc3",scinX_*0.5, scinY_*2.0, scinZ_*2.0);
  // G4LogicalVolume* sc3Log =
  //     new G4LogicalVolume(sc3_box, G4Material::GetMaterial("SC4"), "log_sc3", 0, 0, 0);
  // G4VPhysicalVolume* sc3Phys =
  //     new G4PVPlacement(0, sc3Vec, sc3Log, "sc3", worldLog, false, 0);

  // Define SC4
  G4Box* sc4_box = new G4Box("sc4",scinX_, scinY_*0.5, scinZ_*0.5);
  G4LogicalVolume* sc4Log =
      new G4LogicalVolume(sc4_box, G4Material::GetMaterial("SC4"), "log_sc4", 0, 0, 0);
  G4VPhysicalVolume* sc4Phys =
      new G4PVPlacement(0, sc4Vec, sc4Log, "sc4", worldLog, false, 0);

  // Define steel
  // G4Box* steel_box = new G4Box("steel", scinX_, scinY_*0.5, scinZ_*2.0);
  // G4LogicalVolume* steelLog =
  //     new G4LogicalVolume(steel_box, G4Material::GetMaterial("Steel"), "log_steel", 0, 0, 0);
  // G4VPhysicalVolume* steelPhys =
  //     new G4PVPlacement(0, steelVec, steelLog, "steel", worldLog, false, 0);

  // User limits (energy cutoffs)
  G4UserLimits* energyLimit = new G4UserLimits();
  energyLimit->SetUserMinEkine(2.26 * CLHEP::eV);  // Lower threshold of PMT - 550nm
  worldLog->SetUserLimits(energyLimit);
  // sc1Log->SetUserLimits(energyLimit);
  // sc2Log->SetUserLimits(energyLimit);
  // sc3Log->SetUserLimits(energyLimit);
  sc4Log->SetUserLimits(energyLimit);
  // steelLog->SetUserLimits(energyLimit);

  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  sc4SD_ = new G4BeamTestSC4SD("sc4_SD_", "SDHitsCollection");
  sdManager->AddNewDetector(sc4SD_);
  sc4Log->SetSensitiveDetector(sc4SD_);

  return worldPhys;
}

/*****************************************************************/

void G4BeamTestDetectorConstruction::CreateMaterials()
{
  CreateAir();
  CreateWater();
  CreatePlastic();
  CreateGlassSphere();
  CreateEffectiveDOMMaterial();
  CreateSC4();
  CreateSteel();
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

  // const G4int water_bins = 2;
  // G4double water_ephot[water_bins] = {0.1 * CLHEP::eV, 10 * CLHEP::eV};
  // G4double water_refr[water_bins] = {1.33, 1.33};

   //From SFDETSIM water absorption
   const G4int NUMENTRIES_water=60;

   G4double ENERGY_water[NUMENTRIES_water] =
     { 1.56962e-09*GeV, 1.58974e-09*GeV, 1.61039e-09*GeV, 1.63157e-09*GeV, 
       1.65333e-09*GeV, 1.67567e-09*GeV, 1.69863e-09*GeV, 1.72222e-09*GeV, 
       1.74647e-09*GeV, 1.77142e-09*GeV,1.7971e-09*GeV, 1.82352e-09*GeV, 
       1.85074e-09*GeV, 1.87878e-09*GeV, 1.90769e-09*GeV, 1.93749e-09*GeV, 
       1.96825e-09*GeV, 1.99999e-09*GeV, 2.03278e-09*GeV, 2.06666e-09*GeV,
       2.10169e-09*GeV, 2.13793e-09*GeV, 2.17543e-09*GeV, 2.21428e-09*GeV, 
       2.25454e-09*GeV, 2.29629e-09*GeV, 2.33962e-09*GeV, 2.38461e-09*GeV, 
       2.43137e-09*GeV, 2.47999e-09*GeV, 2.53061e-09*GeV, 2.58333e-09*GeV, 
       2.63829e-09*GeV, 2.69565e-09*GeV, 2.75555e-09*GeV, 2.81817e-09*GeV, 
       2.88371e-09*GeV, 2.95237e-09*GeV, 3.02438e-09*GeV, 3.09999e-09*GeV,
       3.17948e-09*GeV, 3.26315e-09*GeV, 3.35134e-09*GeV, 3.44444e-09*GeV, 
       3.54285e-09*GeV, 3.64705e-09*GeV, 3.75757e-09*GeV, 3.87499e-09*GeV, 
       3.99999e-09*GeV, 4.13332e-09*GeV, 4.27585e-09*GeV, 4.42856e-09*GeV, 
       4.59258e-09*GeV, 4.76922e-09*GeV, 4.95999e-09*GeV, 5.16665e-09*GeV, 
       5.39129e-09*GeV, 5.63635e-09*GeV, 5.90475e-09*GeV, 6.19998e-09*GeV }; 

   // M Fechner : new ; define the water refraction index using refsg.F 
   //from skdetsim using the whole range.   
    G4double RINDEX1[NUMENTRIES_water] = 
     {1.32885, 1.32906, 1.32927, 1.32948, 1.3297, 1.32992, 1.33014, 
      1.33037, 1.3306, 1.33084, 1.33109, 1.33134, 1.3316, 1.33186, 1.33213,
      1.33241, 1.3327, 1.33299, 1.33329, 1.33361, 1.33393, 1.33427, 1.33462,
      1.33498, 1.33536, 1.33576, 1.33617, 1.3366, 1.33705, 1.33753, 1.33803,
      1.33855, 1.33911, 1.3397, 1.34033, 1.341, 1.34172, 1.34248, 1.34331,
      1.34419, 1.34515, 1.3462, 1.34733, 1.34858, 1.34994, 1.35145, 1.35312,
      1.35498, 1.35707, 1.35943, 1.36211, 1.36518, 1.36872, 1.37287, 1.37776,
      1.38362, 1.39074, 1.39956, 1.41075, 1.42535};

    G4double ABWFF = 1.0;
    //T. Akiri: Values from Skdetsim 
    G4double ABSORPTION_water[NUMENTRIES_water] =
      {
        16.1419*cm*ABWFF,  18.278*cm*ABWFF, 21.0657*cm*ABWFF, 24.8568*cm*ABWFF, 30.3117*cm*ABWFF, 
        38.8341*cm*ABWFF, 54.0231*cm*ABWFF, 81.2306*cm*ABWFF, 120.909*cm*ABWFF, 160.238*cm*ABWFF, 
        193.771*cm*ABWFF, 215.017*cm*ABWFF, 227.747*cm*ABWFF,  243.85*cm*ABWFF, 294.036*cm*ABWFF, 
        321.647*cm*ABWFF,  342.81*cm*ABWFF, 362.827*cm*ABWFF, 378.041*cm*ABWFF, 449.378*cm*ABWFF,
        739.434*cm*ABWFF, 1114.23*cm*ABWFF, 1435.56*cm*ABWFF, 1611.06*cm*ABWFF, 1764.18*cm*ABWFF, 
        2100.95*cm*ABWFF,  2292.9*cm*ABWFF, 2431.33*cm*ABWFF,  3053.6*cm*ABWFF, 4838.23*cm*ABWFF, 
        6539.65*cm*ABWFF, 7682.63*cm*ABWFF, 9137.28*cm*ABWFF, 12220.9*cm*ABWFF, 15270.7*cm*ABWFF, 
        19051.5*cm*ABWFF, 23671.3*cm*ABWFF, 29191.1*cm*ABWFF, 35567.9*cm*ABWFF,   42583*cm*ABWFF,
        49779.6*cm*ABWFF, 56465.3*cm*ABWFF,   61830*cm*ABWFF, 65174.6*cm*ABWFF, 66143.7*cm*ABWFF,   
          64820*cm*ABWFF,   61635*cm*ABWFF, 57176.2*cm*ABWFF, 52012.1*cm*ABWFF, 46595.7*cm*ABWFF, 
        41242.1*cm*ABWFF, 36146.3*cm*ABWFF, 31415.4*cm*ABWFF, 27097.8*cm*ABWFF, 23205.7*cm*ABWFF, 
        19730.3*cm*ABWFF, 16651.6*cm*ABWFF, 13943.6*cm*ABWFF, 11578.1*cm*ABWFF, 9526.13*cm*ABWFF
      };

   G4double RAYFF = 0.625;
   //T. Akiri: Values from Skdetsim 
   G4double RAYLEIGH_water[NUMENTRIES_water] = {
      386929*cm*RAYFF,  366249*cm*RAYFF,  346398*cm*RAYFF,  327355*cm*RAYFF,  309097*cm*RAYFF,  
      291603*cm*RAYFF,  274853*cm*RAYFF,  258825*cm*RAYFF,  243500*cm*RAYFF,  228856*cm*RAYFF,  
      214873*cm*RAYFF,  201533*cm*RAYFF,  188816*cm*RAYFF,  176702*cm*RAYFF,  165173*cm*RAYFF,
      154210*cm*RAYFF,  143795*cm*RAYFF,  133910*cm*RAYFF,  124537*cm*RAYFF,  115659*cm*RAYFF,  
      107258*cm*RAYFF, 99318.2*cm*RAYFF, 91822.2*cm*RAYFF,   84754*cm*RAYFF, 78097.3*cm*RAYFF, 
     71836.5*cm*RAYFF,   65956*cm*RAYFF, 60440.6*cm*RAYFF, 55275.4*cm*RAYFF, 50445.6*cm*RAYFF,
       45937*cm*RAYFF, 41735.2*cm*RAYFF, 37826.6*cm*RAYFF, 34197.6*cm*RAYFF, 30834.9*cm*RAYFF, 
     27725.4*cm*RAYFF, 24856.6*cm*RAYFF, 22215.9*cm*RAYFF, 19791.3*cm*RAYFF, 17570.9*cm*RAYFF,   
       15543*cm*RAYFF, 13696.6*cm*RAYFF, 12020.5*cm*RAYFF, 10504.1*cm*RAYFF, 9137.15*cm*RAYFF,
     7909.45*cm*RAYFF,  6811.3*cm*RAYFF, 5833.25*cm*RAYFF,  4966.2*cm*RAYFF, 4201.36*cm*RAYFF, 
     3530.28*cm*RAYFF, 2944.84*cm*RAYFF, 2437.28*cm*RAYFF, 2000.18*cm*RAYFF,  1626.5*cm*RAYFF, 
     1309.55*cm*RAYFF, 1043.03*cm*RAYFF, 821.016*cm*RAYFF,  637.97*cm*RAYFF, 488.754*cm*RAYFF
   };

  // G4MaterialPropertiesTable *mpt_water = new G4MaterialPropertiesTable ();
  // mpt_water->AddProperty ("RINDEX", water_ephot, water_refr, water_bins);
  // water->SetMaterialPropertiesTable(mpt_water);
  G4MaterialPropertiesTable *mpt_water = new G4MaterialPropertiesTable ();
  mpt_water->AddProperty ("RINDEX", ENERGY_water, RINDEX1, NUMENTRIES_water);
  mpt_water->AddProperty("ABSLENGTH",ENERGY_water, ABSORPTION_water, NUMENTRIES_water);
  mpt_water->AddProperty("RAYLEIGH",ENERGY_water,RAYLEIGH_water,NUMENTRIES_water);
  water->SetMaterialPropertiesTable(mpt_water);

}

/*****************************************************************/

void G4BeamTestDetectorConstruction::CreatePlastic()
{
  G4NistManager* nistManager = G4NistManager::Instance();
  G4Material* plastic = new G4Material("Plastic", 1.425 * CLHEP::g / CLHEP::cm3, 3, kStateSolid);
  plastic->AddElement(nistManager->FindOrBuildElement("H"), 2);
  plastic->AddElement(nistManager->FindOrBuildElement("C"), 1);
  plastic->AddElement(nistManager->FindOrBuildElement("O"), 1);
}

/*****************************************************************/

void G4BeamTestDetectorConstruction::CreateGlassSphere()
{
  // http://hypernews.slac.stanford.edu/HyperNews/geant4/get/AUX/2013/03/11/12.39-85121-chDetectorConstruction.cc
  G4NistManager* nistManager = G4NistManager::Instance();
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
  G4Material* plastic = new G4Material("SC4", 1.425 * CLHEP::g / CLHEP::cm3, 3, kStateSolid);
  plastic->AddElement(nistManager->FindOrBuildElement("H"), 2);
  plastic->AddElement(nistManager->FindOrBuildElement("C"), 1);
  plastic->AddElement(nistManager->FindOrBuildElement("O"), 1);
}

void G4BeamTestDetectorConstruction::CreateSteel()
{
  //---Steel
  G4double a= 12.01*g/mole;
  G4Element* elC = new G4Element("Carbon","C", 6,a);
  
  a = 55.85*g/mole;
  G4Element* elFe = new G4Element("Iron","Fe", 26,a);
  
  G4double density = 7.8*g/cm3;
  G4Material* Steel = new G4Material("Steel",density,2);
  Steel->AddElement(elC, 1.*perCent);
  Steel->AddElement(elFe, 99.*perCent);
}

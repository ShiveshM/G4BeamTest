#ifndef _TOPSIMULATOR_G4BEAMTESTDETECTORCONSTRUCTION_H_
#define _TOPSIMULATOR_G4BEAMTESTDETECTORCONSTRUCTION_H_

#include <G4VUserDetectorConstruction.hh>
#include <G4ThreeVector.hh>

#include "G4BeamTestTank.h"

class G4BeamTestDetectorConstruction: public G4VUserDetectorConstruction
{
 public:
  G4BeamTestDetectorConstruction();

  ~G4BeamTestDetectorConstruction();

  G4VPhysicalVolume* Construct();

  void SetVerboseLevel(G4int level) {verboseLevel_=level;}

  void InstallTank(G4BeamTestTank* tank) {tank_ = tank;}

  const G4ThreeVector& GetWorldOrigin() const {return origin_;}

 private:

  void CreateMaterials();

  void CreateAir();
  /* void CreateIce(); */
  /* void CreateSnow(); */
  void CreateWater();
  void CreatePlastic();
  /* void CreateTyvek(); */
  /* void CreatePerlite(); */
  void CreateGlassSphere();
  void CreateEffectiveDOMMaterial();

  G4ThreeVector origin_;

  G4int verboseLevel_;

  G4BeamTestTank* tank_;
};

#endif

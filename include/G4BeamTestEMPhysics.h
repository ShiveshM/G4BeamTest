#ifndef G4TANKRESPONSE_G4BEAMTESTEMPHYSICS_H_INCLUDED
#define G4TANKRESPONSE_G4BEAMTESTEMPHYSICS_H_INCLUDED

#include <globals.hh>
#include <G4VPhysicsConstructor.hh>
#include <G4PhotoElectricEffect.hh>
#include <G4ComptonScattering.hh>
#include <G4GammaConversion.hh>
#include <G4eMultipleScattering.hh>
#include <G4eIonisation.hh>
#include <G4eBremsstrahlung.hh>
#include <G4eplusAnnihilation.hh>

#include <G4Cerenkov.hh>
#include <G4Scintillation.hh>
#include <G4OpAbsorption.hh>
#include <G4OpRayleigh.hh>
#include <G4OpMieHG.hh>
#include <G4OpBoundaryProcess.hh>

#include <G4LossTableManager.hh>
#include <G4EmSaturation.hh>


/**
   @class G4BeamTestEMPhysics
   @brief Electromagnetic physics. Used only if Geant4 version is earlier than 4.10.

   This class implements the electromagnetic interactions
   - Photoelectric effect
   - Compton scattering
   - Gamma conversion
   - Multiple scattering
   - Ionisation/Bremsstrahlung for electrons
   - Positron annihilation
*/
class G4BeamTestEMPhysics : public G4VPhysicsConstructor {
public:
  G4BeamTestEMPhysics();
  ~G4BeamTestEMPhysics();

  void ConstructParticle();
  void ConstructProcess();
  void ConstructOp();

private:
  // Gamma physics
  G4PhotoElectricEffect photoEffect;
  G4ComptonScattering comptonEffect;
  G4GammaConversion pairProduction;

  // Electron physics
  G4eMultipleScattering electronMultipleScattering;
  G4eIonisation electronIonisation;
  G4eBremsstrahlung electronBremsStrahlung;
  
  //Positron physics
  G4eMultipleScattering positronMultipleScattering;
  G4eIonisation positronIonisation; 
  G4eBremsstrahlung positronBremsStrahlung;  
  G4eplusAnnihilation annihilation;

  // Cerenkov physics
  G4Cerenkov cerenkov;
};

#endif // G4TANKRESPONSE_G4BEAMTESTEMPHYSICS_H_INCLUDED

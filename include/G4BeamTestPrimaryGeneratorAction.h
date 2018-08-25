#ifndef G4BeamTestPrimaryGeneratorAction_h
#define G4BeamTestPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;
class G4BeamTestPrimaryGeneratorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4BeamTestPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    G4BeamTestPrimaryGeneratorAction();
    virtual ~G4BeamTestPrimaryGeneratorAction();

  public:
    virtual void GeneratePrimaries(G4Event*);

    void SetOptPhotonPolar();
    void SetOptPhotonPolar(G4double);

  private:
    G4GeneralParticleSource* fParticleGun;
    G4BeamTestPrimaryGeneratorMessenger* fGunMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*G4BeamTestPrimaryGeneratorAction_h*/


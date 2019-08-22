#ifndef G4BeamTestPrimaryGeneratorMessenger_h
#define G4BeamTestPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class G4BeamTestPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4BeamTestPrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    G4BeamTestPrimaryGeneratorMessenger(G4BeamTestPrimaryGeneratorAction* );
    virtual ~G4BeamTestPrimaryGeneratorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    G4BeamTestPrimaryGeneratorAction* fG4BeamTestAction;
    G4UIdirectory*                  fGunDir;
    G4UIcmdWithADoubleAndUnit*      fPolarCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


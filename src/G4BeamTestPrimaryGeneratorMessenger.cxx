#include "G4BeamTestPrimaryGeneratorMessenger.h"
#include "G4BeamTestPrimaryGeneratorAction.h"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4BeamTestPrimaryGeneratorMessenger::
  G4BeamTestPrimaryGeneratorMessenger(G4BeamTestPrimaryGeneratorAction* G4BeamTestGun)
  : G4UImessenger(),
    fG4BeamTestAction(G4BeamTestGun)
{
  fGunDir = new G4UIdirectory("/G4BeamTest/gun/");
  fGunDir->SetGuidance("PrimaryGenerator control");

  fPolarCmd =
           new G4UIcmdWithADoubleAndUnit("/G4BeamTest/gun/optPhotonPolar",this);
  fPolarCmd->SetGuidance("Set linear polarization");
  fPolarCmd->SetGuidance("  angle w.r.t. (k,n) plane");
  fPolarCmd->SetParameterName("angle",true);
  fPolarCmd->SetUnitCategory("Angle");
  fPolarCmd->SetDefaultValue(-360.0);
  fPolarCmd->SetDefaultUnit("deg");
  fPolarCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4BeamTestPrimaryGeneratorMessenger::~G4BeamTestPrimaryGeneratorMessenger()
{
  delete fPolarCmd;
  delete fGunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4BeamTestPrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{
  if( command == fPolarCmd ) {
      G4double angle = fPolarCmd->GetNewDoubleValue(newValue);
      if ( angle == -360.0*deg ) {
         fG4BeamTestAction->SetOptPhotonPolar();
      } else {
         fG4BeamTestAction->SetOptPhotonPolar(angle);
      }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


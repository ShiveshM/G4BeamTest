#ifndef G4BeamTestRunAction_h
#define G4BeamTestRunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Timer;
class G4Run;

class G4BeamTestRunAction : public G4UserRunAction
{
  public:
    G4BeamTestRunAction();
    virtual ~G4BeamTestRunAction();

  public:
    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run* aRun);
    virtual void EndOfRunAction(const G4Run* aRun);

  private:
    G4Timer* fTimer;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*G4BeamTestRunAction_h*/


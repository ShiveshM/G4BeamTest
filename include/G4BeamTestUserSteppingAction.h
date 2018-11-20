#ifndef G4BEAMTESTUSERSTEPPINGACTION_H_INCLUDED
#define G4BEAMTESTUSERSTEPPINGACTION_H_INCLUDED
#include "G4UserSteppingAction.hh"
#include "globals.hh"

/**
 * Implementation of G4UserSteppingAction. This class kills gammas below threshold (set by G4BeamTestTank).
 */
class G4BeamTestUserSteppingAction : public G4UserSteppingAction {

  public:
    G4BeamTestUserSteppingAction();
   ~G4BeamTestUserSteppingAction() {}

    void UserSteppingAction(const G4Step*);

  private:
    G4int fScintillationCounter;
    G4int fCerenkovCounter;
    G4int fEventNumber;
};

#endif  // G4BEAMTESTUSERSTEPPINGACTION_H_INCLUDED

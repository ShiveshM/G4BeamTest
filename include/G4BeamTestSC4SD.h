#ifndef G4BeamTestSC4SD_h
#define G4BeamTestSC4SD_h 1

#include "G4VSensitiveDetector.hh"

#include "G4BeamTestSiHit.h"

#include <vector>

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// B2Tracker sensitive detector class
///
/// The hits are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step. A hit is created with each step with non zero
/// energy deposit.

class G4BeamTestSC4SD : public G4VSensitiveDetector
{
public:
  G4BeamTestSC4SD(const G4String& name, const G4String& hitsCollectionName);
  virtual ~G4BeamTestSC4SD();

  // methods from base class
  virtual void   Initialize(G4HCofThisEvent* hitCollection);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);


private:
  G4BeamTestSiHitsCollection* fHitsCollection;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif



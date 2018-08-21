#ifndef _G4TANKRESPONSE_G4TankIceSD_H
#define _G4TANKRESPONSE_G4TankIceSD_H

/* #include <icetray/OMKey.h> */

#include <G4VSensitiveDetector.hh>
#include <G4ThreeVector.hh>

/* #include <map> */

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;


/**
 * An "ice sensitive detector". This sensitive detector is meant to be associated with the ice logical volume in a tank. TODO(shivesh): make the PMT the SD
 *
 * This class keeps track of the energy losses and number Cherenkov photons produced in the ice of each tank.
 * The Cherenkov photons are counted in two ways. One is a simple count and the other is a weighted count
 * where the weight depends on the distance from the photon emission point an the OM.
 */
class G4TankIceSD : public G4VSensitiveDetector
{
 public:
   G4TankIceSD(G4String name/* , const std::map<OMKey, G4ThreeVector>& domPositions */);
  ~G4TankIceSD();

  /// Methods called by Geant4 framework
  void Initialize(G4HCofThisEvent *HCE);
  G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist);
  void EndOfEvent(G4HCofThisEvent *HCE);

  /// Get total energy deposit for a given OM (same for both OMs in a tank)
  G4double GetEDep(/* const OMKey& omKey */) {return sumEdep_/* [omKey] */;}
  /// Get average emission time weighted by deposited energy (same for both OMs in a tank)
  G4double GetTime(/* const OMKey& omKey */) {return cogTime_/* [omKey] */;}
  /// Get number of Cherenkov photons for a given OM (same for both OMs in a tank)
  G4double GetNumCherenkov(/* const OMKey& omKey */) {return cherenkovCounter_/* [omKey] */;}
  /// Get number of Cherenkov photons for a given OM weighted relative to emission point
  G4double GetNumCherenkovWeight(/* const OMKey& omKey */) {return cherenkovCounterWeight_/* [omKey] */;}

 private:
  //ExN04TrackerHitsCollection *trackerCollection;
  const G4ThreeVector domPositions_;

  /// Cherenkov production. See technical note
  G4double GetCerenkovNumber(G4Step* aStep);
  G4double GetProbability(G4double radius, G4double height);

  G4double sumEdep_;
  G4double cogTime_;
  G4double cherenkovCounter_;
  G4double cherenkovCounterWeight_;
};

#endif

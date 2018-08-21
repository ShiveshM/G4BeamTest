#include "G4BeamTestUserSteppingAction.h"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4SteppingManager.hh"

G4BeamTestUserSteppingAction::G4BeamTestUserSteppingAction(){}

void G4BeamTestUserSteppingAction::UserSteppingAction(const G4Step*)
{
  G4Track* track = fpSteppingManager->GetTrack();
  if(track)
  {
    const G4LogicalVolume *volume = track->GetLogicalVolumeAtVertex();
    G4UserLimits *limit = volume->GetUserLimits();
    if(!limit) G4cout << "----> G4LogicalVolume: " << volume->GetName() << " has no defined G4UserLimit" << G4endl;
    G4double threshold = limit->GetUserMinEkine(*track);
    //check if particle is a gamma
    if(track->GetDefinition()->GetParticleName() == "gamma")
    {
      //check if particle energy is below threshold; if true, kill the particle
      G4double energy = track->GetTotalEnergy();
      if(energy < threshold) track->SetTrackStatus(fStopAndKill);
    }
  }
}

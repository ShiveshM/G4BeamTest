#include "G4BeamTestUserSteppingAction.h"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4SteppingManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"

G4BeamTestUserSteppingAction::G4BeamTestUserSteppingAction(){}

void G4BeamTestUserSteppingAction::UserSteppingAction(const G4Step* step)
{
  G4int eventNumber = G4RunManager::GetRunManager()->
                                              GetCurrentEvent()->GetEventID();

  if (eventNumber != fEventNumber) {
     /* G4cout << " Number of Scintillation Photons in previous event: " */
     /*       << fScintillationCounter << G4endl; */
     /* G4cout << " Number of Cerenkov Photons in previous event: " */
     /*        << fCerenkovCounter << G4endl; */
     fEventNumber = eventNumber;
     fScintillationCounter = 0;
     fCerenkovCounter = 0;
  }

  G4Track* track = fpSteppingManager->GetTrack();
  if(track)
  {
    const G4LogicalVolume *volume = track->GetLogicalVolumeAtVertex();
    G4UserLimits *limit = volume->GetUserLimits();
    if(!limit) G4cout << "----> G4LogicalVolume: " << volume->GetName() << " has no defined G4UserLimit" << G4endl;
    G4double threshold = limit->GetUserMinEkine(*track);
    //check if particle is a gamma
    G4String particle = track->GetDefinition()->GetParticleName();
    /* G4cout << "particle = " << particle << G4endl; */
    if(particle == "gamma")
    {
      //check if particle energy is below threshold; if true, kill the particle
      G4double energy = track->GetTotalEnergy();
      if(energy < threshold){
          if (energy > 0) G4cout << "SteppingAction: killing particle " << particle << " with energy " << energy << " < " << threshold << G4endl;
          track->SetTrackStatus(fStopAndKill);
      }
    }
  }

  const std::vector<const G4Track*>* secondaries =
                                            step->GetSecondaryInCurrentStep();

  if (secondaries->size()>0) {
     for(unsigned int i=0; i<secondaries->size(); ++i) {
        if (secondaries->at(i)->GetParentID()>0) {
           if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition()
               == G4OpticalPhoton::OpticalPhotonDefinition()){
             if (secondaries->at(i)->GetCreatorProcess()->GetProcessName()
              == "Scintillation")fScintillationCounter++;
              if (secondaries->at(i)->GetCreatorProcess()->GetProcessName()
               == "Cerenkov")fCerenkovCounter++;
           }
        }
     }
  }
}

#include "G4BeamTestUserTrackingAction.h"

#include "G4Track.hh"
#include "G4UserLimits.hh"
#include "G4TrackVector.hh"
#include "G4TrackingManager.hh"

G4BeamTestUserTrackingAction::G4BeamTestUserTrackingAction(){}

void G4BeamTestUserTrackingAction::PreUserTrackingAction(const G4Track*){}

void G4BeamTestUserTrackingAction::PostUserTrackingAction(const G4Track* track)
{
  const G4LogicalVolume *volume = track->GetLogicalVolumeAtVertex();
  G4UserLimits *limit = volume->GetUserLimits();
  if(!limit) G4cout << "----> G4LogicalVolume: " << volume->GetName() << " has no defined G4UserLimit" << G4endl;
  G4double threshold = limit->GetUserMinEkine(*track);
  G4double max_threshold = 3.54;
  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
  if(secondaries)
  {
    size_t nSeco = secondaries->size();
    if(nSeco>0)
    {
      for(size_t i=0;i<nSeco;i++)
      {
        //check if secondary particle is a gamma
        G4String particle = (*secondaries)[i]->GetDefinition()->GetParticleName();
        if(particle == "gamma" || particle == "opticalphoton")
        {
          //check if particle energy is below threshold; if true, kill the particle
          G4double energy = (*secondaries)[i]->GetTotalEnergy();
          // if(energy < threshold){
          //     G4cout << "TrackingAction: killing particle " << particle << " with energy " << energy << " < " << threshold << G4endl;
          //     (*secondaries)[i]->SetTrackStatus(fStopAndKill);
          // }
	  // if (energy >  max_threshold * CLHEP::eV){
	      // G4cout << "TrackingAction: killing particle " << particle << " with energy " << energy << " > " << max_threshold << G4endl;
	      // (*secondaries)[i]->SetTrackStatus(fStopAndKill); 
	    
	  // }
        }
      }
    }
  }
}

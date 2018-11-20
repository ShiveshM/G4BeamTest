#include "G4BeamTestUserStackingAction.h"

#include "G4VProcess.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4ios.hh"

#include "G4Event.hh"

#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VDigitizerModule.hh"
#include "G4DigiManager.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
//#include "TH1F.h"
//#include "TH2F.h"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4BeamTestUserStackingAction::G4BeamTestUserStackingAction()
  : G4UserStackingAction(),
    fScintillationCounter(0), 
    fCerenkovCounter(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4BeamTestUserStackingAction::~G4BeamTestUserStackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
G4BeamTestUserStackingAction::ClassifyNewTrack(const G4Track * aTrack)
{
  if(aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
  { // particle is optical photon
    if(aTrack->GetParentID()>0)
    { // particle is secondary
      if(aTrack->GetCreatorProcess()->GetProcessName() == "Scintillation")
       fScintillationCounter++;
      if(aTrack->GetCreatorProcess()->GetProcessName() == "Cerenkov")
        fCerenkovCounter++;
    }
  }
  return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4BeamTestUserStackingAction::NewStage()
{
  G4cout << "Number of Scintillation photons produced in this event : "
        << fScintillationCounter << G4endl;
  G4cout << "Number of Cerenkov photons produced in this event : "
         << fCerenkovCounter << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4BeamTestUserStackingAction::PrepareNewEvent()
{
 fScintillationCounter = 0;
  fCerenkovCounter = 0;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


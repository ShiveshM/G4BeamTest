
#include <sstream>

#include "G4UnitsTable.hh"

#include "G4BeamTestSC4SD.h"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4BeamTestSC4SD::G4BeamTestSC4SD(const G4String& name, const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name)
      , fHitsCollection(NULL)
{
   collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4BeamTestSC4SD::~G4BeamTestSC4SD()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4BeamTestSC4SD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection

  fHitsCollection
    = new G4BeamTestSiHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce

  G4int hcID
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G4BeamTestSC4SD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  //  G4double stepl = 10;
  // aStep->SetStepLength(stepl);

  G4String name=aStep->GetTrack()->GetDefinition()->GetParticleName();


  if (name == "pi-" || name == "e-") {
    G4cout << " Particle_name hit SC4 = " <<  name  << G4endl;
    // total energy
    G4double etot = aStep->GetTrack()->GetTotalEnergy();
    // energy deposit
    G4double edep = aStep->GetTotalEnergyDeposit();

    G4BeamTestSiHit* newHit = new G4BeamTestSiHit();
    // G4StepPoint* preStepPoint = aStep->GetPreStepPoint();

    newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
    newHit->SetEdep(edep);
    newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
    newHit-> SetTime (aStep->GetPreStepPoint()->GetProperTime());
    fHitsCollection->insert( newHit );

    return true;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4BeamTestSC4SD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) {
    G4int nofHits = fHitsCollection->entries();
    G4cout << G4endl
	   << "-------->Hits Collection: in this event they are " << nofHits
	   << " hits in the tracker chambers: " << G4endl;
      for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();

  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


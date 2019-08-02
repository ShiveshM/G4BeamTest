
#include <sstream>

#include "G4UnitsTable.hh"

#include "G4BeamTestSiSD.h"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

std::ofstream testdata("EVENTDATA.txt");


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4BeamTestSiSD::G4BeamTestSiSD(const G4String& name,
	   const G4String& hitsCollectionName) 
  : G4VSensitiveDetector(name)
      , fHitsCollection(NULL)
{
   collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4BeamTestSiSD::~G4BeamTestSiSD() 
{ // testdata  << "Arrival time"   << "  "  << "Energy "  <<  "  "  << "Distance"  <<  std::endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4BeamTestSiSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection

  testdata  <<  "*"    <<  std::endl;

  fHitsCollection 
    = new G4BeamTestSiHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce

  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  G4cout << "hcID " << hcID << G4endl;
  hce->AddHitsCollection( hcID, fHitsCollection ); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G4BeamTestSiSD::ProcessHits(G4Step* aStep, 
				G4TouchableHistory*)
{  
  //  G4double stepl = 10;
  // aStep->SetStepLength(stepl);

  G4String name=aStep->GetTrack()->GetDefinition()->GetParticleName();

  /* G4cout << " Particle_name = " <<  name  << G4endl;   */

  if (name == "opticalphoton" || name == "gamma") {
  // G4cout << " Particle_name = " <<  name  << G4endl;

// total energy
  G4double etot = aStep->GetTrack()->GetTotalEnergy();
// energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit();

  if (etot < 2.26 * CLHEP::eV) { // Lower threshold of PMT - 550nm
  // if (etot < 2.48 * CLHEP::eV) { // Lower threshold of PMT - 500nm
      // G4cout << "particle " << name << " under threshold with energy " << etot << G4endl;
      return true;
  }
  if (etot > 3.55 * CLHEP::eV) { // Upper threshold of PMT - 350nm
  // if (etot > 3.10 * CLHEP::eV) { // Upper threshold of PMT - 400nm
      // G4cout << "particle " << name << " over threshold with energy " << etot << G4endl;
      return true;
  }
  // G4cout << "inserting particle " << name << " with energy " << etot << " into record" << G4endl;

// if (edep==0.) return false;

  /* G4cout << " Particle_name_after_edep = " <<  name  << G4endl;   */

  G4BeamTestSiHit* newHit = new G4BeamTestSiHit();
  // G4StepPoint* preStepPoint = aStep->GetPreStepPoint();

  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetEdep(edep);
  newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
  newHit-> SetTime (aStep->GetPreStepPoint()->GetProperTime());
  fHitsCollection->insert( newHit );
  

  G4double zbef =aStep->GetPreStepPoint()->GetPosition().z();
  G4double zaft =aStep->GetPostStepPoint()->GetPosition().z();
  G4double zdelta = zaft-zbef;
  //G4double zintial = aStep

  G4double zfix =  (zdelta + (zbef- 4.7))*1000;

  //  G4double zfix =  (zdelta + (zbef- 375));

  //  G4cout << " Z VALUE = " <<  zfix  << G4endl;
  G4double time = aStep->GetPreStepPoint()->GetProperTime();
  G4double gtime = aStep->GetPreStepPoint()->GetGlobalTime();
  //  G4cout << "Global Time " << gtime << G4endl;
 G4double timefix = time - 0.288861775183071;

/*  G4cout << "TIME == " << time << G4endl; */
 
 G4double vel =  aStep->GetPreStepPoint()->GetVelocity();
 
 G4double vel2 = zfix/time;

 // G4cout << G4BestUnit(vel, "Speed")<< G4endl; 

/*

 testdata << timefix << "  "   << edep  << "  "  <<  zfix  <<  std::endl;
   
 h_bragg9->Fill(zfix,edep);
 enetime->Fill(timefix, edep);
 // newHit->Print();
 sally->Fill(zfix,vel);

 G4cout << "DISTANCE = " << zfix << " Velocity = " << vel2 <<  " Velocityold = " << vel <<G4endl; 


*/
  return true;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4BeamTestSiSD::EndOfEvent(G4HCofThisEvent*)
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

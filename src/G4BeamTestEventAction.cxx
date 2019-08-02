#include "G4Event.hh"

#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VDigitizerModule.hh"
#include "G4DigiManager.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4BeamTestSiSD.h"
#include "G4BeamTestSiHit.h"
#include "G4BeamTestEventAction.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4BeamTestEventAction::G4BeamTestEventAction()
  : G4UserEventAction(),
 
    SiCollID(0)
{
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4BeamTestEventAction::~G4BeamTestEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4BeamTestEventAction::BeginOfEventAction(const G4Event* event )
{    
 
  
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  SDman->ListTree();
  if(SiCollID<0)
    {
      G4String colNam;
      SiCollID = SDman->GetCollectionID(colNam="G4BeamTestSiSDCollection");
     
    }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4BeamTestEventAction::EndOfEventAction(const G4Event* event)
{   

  G4cout << ">>> Summary of Event " << event->GetEventID() << G4endl;
  // testnew << ">>> Summary of Event " << event->GetEventID() << G4endl;

  G4cout << SiCollID << G4endl;

  if(SiCollID<0) return;

  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  G4BeamTestSiHitsCollection* SiHC = 0;

  if(HCE)
    {
      SiHC = (G4BeamTestSiHitsCollection*)(HCE->GetHC(SiCollID));

    }



  if(SiHC)
    {
      int n_hit = SiHC->entries();
      // testnew << std::flush;
      G4cout << G4endl;
      // G4cout << "Si hits " <<
      // "--------------------------------------------------------------"
      //    << G4endl;
      G4cout << n_hit << " hits are stored in G4BeamTestSiHitsCollection."
	     << G4endl;
      /* G4cout << "List of hits in tracker" << G4endl; */
      // testnew << G4endl;
      // testnew << "Si hits " <<
      // "--------------------------------------------------------------"
      testnew << n_hit << " hits are stored in G4BeamTestSiHitsCollection."
	     << G4endl;
      /* testnew << "List of hits in tracker" << G4endl; */
      for(int i=0;i<n_hit;i++)
      {
          /* (*SiHC)[i]->Print(); */
          (*SiHC)[i]->Dataout();
      }

      //      G4cout << "sid + " << SiCollID << G4endl;
      //      testnew << "sid + " << SiCollID << G4endl;
      testnew << std::flush;

    } 

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


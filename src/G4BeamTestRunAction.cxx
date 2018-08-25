// Make this appear first!
#include "G4Timer.hh"

#include "G4BeamTestRunAction.h"

#include "G4Run.hh"

#include "G4RunManager.hh"
#include "G4ScoringManager.hh"
#include "G4UnitsTable.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4PSNofCollision.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"

#include "G4Run.hh"

//#include "TH1F.h"  //This file seems to be ....

//extern TH1F*  h_bragg9;
//#include "B3PrimaryGeneratorAction.hh"
//#include "B3Run.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4BeamTestRunAction::G4BeamTestRunAction()
 : G4UserRunAction(),
   fTimer(0)
{
  fTimer = new G4Timer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4BeamTestRunAction::~G4BeamTestRunAction()
{
  delete fTimer;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* G4BeamTestRunAction::GenerateRun()
{
  return new G4Run;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4BeamTestRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  fTimer->Start();

  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//

void G4BeamTestRunAction::EndOfRunAction(const G4Run* aRun)
{
  fTimer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent()
         << " " << *fTimer << G4endl;

/*
  int binmax = h_bragg9->GetMaximumBin(); double x = h_bragg9->GetXaxis()->GetBinCenter(binmax);

  G4cout << " bin max JFGDSLKJF ==== " << binmax << "X VALUE DISTANCE SAUJDFHALSDF ==== "<< x << G4endl
*/

}

//
/*
G4SDManager* SDman = G4SDManager::GetSDMpointer();
G4cout << " Print out hit information" << G4endl;
  G4SDManager* SDManager= G4SDManager::GetSDMpointer();
  G4String detName = "inside_dom_lo_wintery_mix";
  G4MultiFunctionalDetector* mfd=
    (G4MultiFunctionalDetector*)SDManager-> FindSensitiveDetector(detName);
  // std::cout << "number of mfd " << muonSD->GetNumberOfPrimitives() << std::endl;
  // G4VPrimitiveScorer* scorer = muonSD->GetPrimitive(0);
  // std::cout << muonSD->GetPrimitive(0) << std::endl;
  // scorer-> PrintAll();
  // muonSD-> DrawAll();
  // G4cout << G4endl;
  // G4PSNofCollision* muonSD2=
  //   (G4PSNofCollision*)SDManager-> FindSensitiveDetector("inside_dom_up_winterymix");
  // muonSD2-> PrintAll();
    if ( mfd ){
    //--- Loop over the registered primitive scorers.
        for (G4int icol = 0; icol < mfd->GetNumberOfPrimitives(); icol++){
    // Get Primitive Scorer object.
            G4VPrimitiveScorer* scorer=mfd->GetPrimitive(icol);
    // collection name and collectionID for HitsCollection,
    // where type of HitsCollection is G4THitsMap in case of primitive scorer.
    // The collection name is given by <MFD name>/<Primitive Scorer name>.
            G4String collectionName = scorer->GetName();
            G4String fullCollectionName = detName+"/"+collectionName;
            G4int    collectionID = SDman->GetCollectionID(fullCollectionName);

            if ( collectionID >= 0 ){
                G4cout << "++ "<<fullCollectionName<< " id " << collectionID 
                       << G4endl;
    // Store obtained HitsCollection information into data members.
    // And, creates new G4THitsMap for accumulating quantities during RUN.
                std::cout << "fullCollectionName " << fullCollectionName << std::endl;
                std::cout <<"collectionID " << collectionID << std::endl;
                G4THitsMap<G4double> * hits =
                    new G4THitsMap<G4double>(detName ,collectionName);
                hits->PrintAllHits();
            }else{
                G4cout << "** collection " << fullCollectionName 
                       << " not found. "<<G4endl;
            }
        }
    }
*/


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




#include <iostream>
#include <iomanip>

#include "G4BeamTestSiHit.h"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4ThreadLocal G4Allocator<G4BeamTestSiHit>* G4BeamTestSiHitAllocator=0;
static std::fstream testnew("./testnew.txt", std::ofstream::out);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4BeamTestSiHit::G4BeamTestSiHit()
  : G4VHit(),
    fTrackID(-1),
  
    fEdep(0.),
              fTime(0.),
    fPos(G4ThreeVector())
{
  /* G4cout << "opening file" << G4endl; */
  /* testnew.open(testnew_out); */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4BeamTestSiHit ::~G4BeamTestSiHit() {
  /* G4cout << "closing file" << G4endl; */
  /* testnew.close(); */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4BeamTestSiHit::G4BeamTestSiHit(const G4BeamTestSiHit& right)
  : G4VHit()
{
  fTrackID   = right.fTrackID;
  // fChamberNb = right.fChamberNb;
  fEdep      = right.fEdep;
  fPos       = right.fPos;
  fTime = right.fTime;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4BeamTestSiHit& G4BeamTestSiHit::operator=(const G4BeamTestSiHit& right)
{
  fTrackID   = right.fTrackID;
  //  fChamberNb = right.fChamberNb;
  fEdep      = right.fEdep;
  fPos       = right.fPos;
  fTime = right.fTime;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int G4BeamTestSiHit::operator==(const G4BeamTestSiHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4BeamTestSiHit::Draw()
{
  /* G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance(); */
  /* if(pVVisManager) */
  /*   { */
  /*     G4Circle circle(fPos); */
  /*     circle.SetScreenSize(4.); */
  /*     circle.SetFillStyle(G4Circle::filled); */
  /*     G4Colour colour(1.,0.,0.); */
  /*     G4VisAttributes attribs(colour); */
  /*     circle.SetVisAttributes(attribs); */
  /*     pVVisManager->Draw(circle); */
  /*   } */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void G4BeamTestSiHit::Print()
{
 G4cout
    << " trackID: " << fTrackID
    << "Edep: "
    << std::setw(7) << G4BestUnit(fEdep,"Energy")
    << " Position: "
    << std::setw(7) << G4BestUnit(fPos ,"Length")
    << "Time: "
    << std::setw(7) << G4BestUnit(fTime,"Time")
    << G4endl;
}

void G4BeamTestSiHit::Dataout()
{
testnew
  << " trackID: " << fTrackID
  << "Edep: "
  << std::setw(7) << G4BestUnit(fEdep,"Energy")
  << " Position: "
  << std::setw(7) << G4BestUnit( fPos ,"Length")
  << "Time: "
  << std::setw(7) << G4BestUnit( fTime,"Time")
  << G4endl;

 }


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


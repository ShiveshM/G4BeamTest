#ifndef G4BeamTestSiHit_h
#define G4BeamTestSiHit_h 1
#include <iostream>
#include <fstream>

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// Tracker hit class
///
/// It defines data members to store the trackID, chamberNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fChamberNB, fEdep, fPos

class G4BeamTestSiHit : public G4VHit
{
public:
  G4BeamTestSiHit();
  G4BeamTestSiHit(const G4BeamTestSiHit&);
  virtual ~G4BeamTestSiHit();

  // operators
  const G4BeamTestSiHit& operator=(const G4BeamTestSiHit&);
  G4int operator==(const G4BeamTestSiHit&) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  // methods from base class
  virtual void Draw();
  virtual void Print();
  virtual void Dataout();

  // Set methods
  void SetTrackID  (G4int track)      { fTrackID = track; };
  void SetEdep     (G4double de)      { fEdep = de; };
  void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
  void SetTime(G4double val) { fTime = val; };


  // Get methods
  G4int GetTrackID() const     { return fTrackID; };
  G4double GetEdep() const     { return fEdep; };
  G4ThreeVector GetPos() const { return fPos; };
  G4double GetTime() const { return fTime; }

  /* const std::string testnew_out = "./testnew.txt"; */

private:

  G4ThreeVector position;
  G4ThreeVector momentum;
  G4double      energy;
  G4double time;
  // std::ofstream testnew;


public:
  inline void SetPosition(G4ThreeVector pos)
  { position=pos; }
  inline G4ThreeVector GetPosition()
  { return position; }
  inline void SetEnergy(G4double ene)
  { energy = ene; }
  inline G4double GetEnergy()
  { return energy; }
 

private:
  G4double    fTime;
  G4int         fTrackID;
  G4double      fEdep;
  G4ThreeVector fPos;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<G4BeamTestSiHit> G4BeamTestSiHitsCollection;

extern G4ThreadLocal G4Allocator<G4BeamTestSiHit>* G4BeamTestSiHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* G4BeamTestSiHit::operator new(size_t)
{
  if(!G4BeamTestSiHitAllocator)
    G4BeamTestSiHitAllocator = new G4Allocator<G4BeamTestSiHit>;
  return (void *) G4BeamTestSiHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void G4BeamTestSiHit::operator delete(void *hit)
{
  G4BeamTestSiHitAllocator->FreeSingle((G4BeamTestSiHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


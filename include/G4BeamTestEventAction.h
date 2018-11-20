#ifndef G4BeamTestEventAction_h
#define G4BeamTestEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

/// Event action class
///

class G4BeamTestEventAction : public G4UserEventAction
{
  public:
    G4BeamTestEventAction();
    virtual ~G4BeamTestEventAction();
    
    virtual void BeginOfEventAction(const G4Event* );
    virtual void EndOfEventAction(const G4Event* );

    void AddEdep(G4double edep) { fEdep += edep; }
    void AddPath(G4double path) { fIntegralZ +=path; }
    G4double GetPath(){return fIntegralZ;}
    void SetXY (G4double xhit, G4double yhit) {fXIn=xhit;fYIn=yhit;}
	G4double GetX()const {return fXIn;}


	G4double GetY()const {return fYIn;}
  private:
  G4double  fEdep;
  G4double  fIntegralZ;
  G4double  fXIn;
  G4int SiCollID;
  G4int hcID;

	G4double  fYIn;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    


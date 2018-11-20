#ifndef TOPSIMULATOR_G4BEAMTESTRUNMANAGER_H
#define TOPSIMULATOR_G4BEAMTESTRUNMANAGER_H

#include <G4RunManager.hh>

class G4ParticleGun;

/**
 * Implementation of G4RunManager
 */
class G4BeamTestRunManager: public G4RunManager
{
 public:
  G4BeamTestRunManager();

  static G4BeamTestRunManager* GetIceTopRunManager() {return (G4BeamTestRunManager*)GetRunManager();}

  // Disable BeamOn
  void BeamOn(G4int n_event,const char* macroFile=0,G4int n_select=-1);

  void InitializeRun();
  void InjectParticle(G4ParticleGun* particleGun);
  void TerminateRun();

 protected:
  G4Event* GenerateEvent(G4int i_event);

 private:
  // This method is an exact copy of UpdateScoring which is private in the G4RunManager
  void Update_Scoring();

};

#endif

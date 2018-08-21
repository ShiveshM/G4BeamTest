#ifndef _TOPSIMULATOR_G4BEAMTESTPHYSICSLIST_H
#define _TOPSIMULATOR_G4BEAMTESTPHYSICSLIST_H

#include <G4VModularPhysicsList.hh>

/**
 * Implementation of G4VModularPhysicsList. The top-level physics list. It combines all the other physics lists:
 *
 *  \li G4BeamTestGeneralPhysics
 *  \li G4BeamTestHadronPhysics
 *  \li G4BeamTestIonPhysics
 *  \li G4BeamTestMuonPhysics
 *  \li G4BeamTestEMPhysics
 */
class G4BeamTestPhysicsList: public G4VModularPhysicsList
{
public:
  G4BeamTestPhysicsList();
  ~G4BeamTestPhysicsList();
private:
  void SetCuts();
};

#endif

#ifndef G4TANKRESPONSE_G4BEAMTESTGENERALPHYSICS_H_INCLUDED
#define G4TANKRESPONSE_G4BEAMTESTGENERALPHYSICS_H_INCLUDED

#include <globals.hh>
#include <G4VPhysicsConstructor.hh>
#include <G4Decay.hh>

/**
 * Implementation of G4VPhysicsConstructor for decay processes and geantino.
 */
class G4BeamTestGeneralPhysics : public G4VPhysicsConstructor
{
public:
  G4BeamTestGeneralPhysics();
  virtual ~G4BeamTestGeneralPhysics();

  void ConstructParticle();
  void ConstructProcess();

private:
  G4Decay decay;
};

#endif // G4TANKRESPONSE_G4BEAMTESTGENERALPHYSICS_H_INCLUDED

/*
 * copyright  (C) 2010
 * The Icecube Collaboration
 *
 * $Id: G4BeamTestGeneralPhysics.h 149388 2016-08-18 21:50:04Z jgonzalez $
 *
 * @version $Revision: 149388 $
 * @date $LastChangedDate: 2016-08-18 22:50:04 +0100 (Thu, 18 Aug 2016) $
 * @author Fabian Kislat <fabian.kislat@desy.de> Last changed by: $LastChangedBy: jgonzalez $
 */

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

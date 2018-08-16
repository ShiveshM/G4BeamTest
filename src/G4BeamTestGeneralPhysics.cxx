/*
 * copyright  (C) 2010
 * The Icecube Collaboration
 *
 * $Id: G4BeamTestGeneralPhysics.cxx 152849 2017-01-20 21:44:25Z jgonzalez $
 *
 * @version $Revision: 152849 $
 * @date $LastChangedDate: 2017-01-20 21:44:25 +0000 (Fri, 20 Jan 2017) $
 * @author Fabian Kislat <fabian.kislat@desy.de> Last changed by: $LastChangedBy: jgonzalez $
 */

#include "G4BeamTestGeneralPhysics.h"

#include <iomanip>   

#include <globals.hh>
#include <G4ios.hh>
#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
// Bosons
#include <G4ChargedGeantino.hh>
#include <G4Geantino.hh>
#include <G4Version.hh>

G4BeamTestGeneralPhysics::G4BeamTestGeneralPhysics()
  :  G4VPhysicsConstructor("general")
{}


G4BeamTestGeneralPhysics::~G4BeamTestGeneralPhysics()
{}


void G4BeamTestGeneralPhysics::ConstructParticle()
{
  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();  
}


void G4BeamTestGeneralPhysics::ConstructProcess()
{
  //AddTransportation();

// Decay processes are set somewhere else now
#if G4VERSION_NUMBER < 1000
  // Add Decay Process
  theParticleIterator->reset();
  while ((*theParticleIterator)()) {
    G4ParticleDefinition *particle = theParticleIterator->value();
    G4ProcessManager *pmanager = particle->GetProcessManager();
    if (decay.IsApplicable(*particle)) { 
      pmanager->AddProcess(&decay);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager->SetProcessOrdering(&decay, idxPostStep);
      pmanager->SetProcessOrdering(&decay, idxAtRest);
    }
  }
#endif
}


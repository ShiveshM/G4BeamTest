/*
 * copyright  (C) 2010
 * The Icecube Collaboration
 *
 * $Id: G4BeamTestIonPhysics.cxx 86420 2012-03-20 16:00:37Z kislat $
 *
 * @version $Revision: 86420 $
 * @date $LastChangedDate: 2012-03-20 16:00:37 +0000 (Tue, 20 Mar 2012) $
 * @author Fabian Kislat <fabian.kislat@desy.de>, Peter Nießen (tanktop)
 * Last changed by: $LastChangedBy: kislat $
 */

#include <iomanip>   

#include <globals.hh>
#include <G4ios.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>
#include <G4IonConstructor.hh>
#include <G4ProcessManager.hh>

#include "G4BeamTestIonPhysics.h"

G4BeamTestIonPhysics::G4BeamTestIonPhysics()
  :  G4VPhysicsConstructor ("ion")
{}

/********************************************************************/

G4BeamTestIonPhysics::~G4BeamTestIonPhysics()
{}


/********************************************************************/

void G4BeamTestIonPhysics::ConstructParticle()
{
  //  Construct light ions
  G4IonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

/********************************************************************/




void G4BeamTestIonPhysics::ConstructProcess()
{
  G4ProcessManager *pManager = 0;
  
  // Elastic Process
  elasticModel_ = new G4LElastic;
  elasticProcess_.RegisterMe(elasticModel_);

  // Generic Ion
  pManager = G4GenericIon::GenericIon()->GetProcessManager();
  // add process
  pManager->AddDiscreteProcess(&elasticProcess_);

  pManager->AddProcess(&ionMultipleScattering_, -1, 1, 1);
  pManager->AddProcess(&ionIonisation_,         -1, 2, 2);

  // Deuteron 
  pManager = G4Deuteron::Deuteron()->GetProcessManager();
  // add process
  pManager->AddDiscreteProcess(&elasticProcess_);

  deuteronModel_ = new G4LEDeuteronInelastic;
  deuteronProcess_.RegisterMe(deuteronModel_);
  pManager->AddDiscreteProcess(&deuteronProcess_);

  pManager->AddProcess(&deuteronMultipleScattering_, -1, 1, 1);
  pManager->AddProcess(&deuteronIonisation_,        -1, 2, 2);

  // Triton 
  pManager = G4Triton::Triton()->GetProcessManager();
  // add process
  pManager->AddDiscreteProcess(&elasticProcess_);

  tritonModel_ = new G4LETritonInelastic;
  tritonProcess_.RegisterMe(tritonModel_);
  pManager->AddDiscreteProcess(&tritonProcess_);

  pManager->AddProcess(&tritonMultipleScattering_, -1, 1, 1);
  pManager->AddProcess(&tritonIonisation_,         -1, 2, 2);
 
  // Alpha 
  pManager = G4Alpha::Alpha()->GetProcessManager();
  // add process
  pManager->AddDiscreteProcess(&elasticProcess_);

  alphaModel_ = new G4LEAlphaInelastic;
  alphaProcess_.RegisterMe(alphaModel_);
  pManager->AddDiscreteProcess(&alphaProcess_);

  pManager->AddProcess(&alphaMultipleScattering_, -1, 1, 1);
  pManager->AddProcess(&alphaIonisation_,         -1, 2, 2);

  // He3
  pManager = G4He3::He3()->GetProcessManager();
  // add process
  pManager->AddDiscreteProcess(&elasticProcess_);

  pManager->AddProcess(&he3MultipleScattering_, -1, 1, 1);
  pManager->AddProcess(&he3Ionisation_,         -1, 2, 2);

   
}




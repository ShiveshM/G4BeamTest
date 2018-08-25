#include <globals.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4Positron.hh>
#include <G4NeutrinoE.hh>
#include <G4AntiNeutrinoE.hh>
#include <G4ProcessManager.hh>

#include "G4BeamTestEMPhysics.h"


G4BeamTestEMPhysics::G4BeamTestEMPhysics()
  : G4VPhysicsConstructor("standard")
{}


G4BeamTestEMPhysics::~G4BeamTestEMPhysics()
{}


void G4BeamTestEMPhysics::ConstructParticle()
{
  // gamma
  G4Gamma::GammaDefinition();
 
  // electron
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
}


void G4BeamTestEMPhysics::ConstructProcess()
{
  G4ProcessManager *pManager = 0;
  
  // Gamma Physics
  pManager = G4Gamma::Gamma()->GetProcessManager();
  pManager->AddDiscreteProcess(&photoEffect);
  pManager->AddDiscreteProcess(&comptonEffect);
  pManager->AddDiscreteProcess(&pairProduction);

  // Electron Physics
  pManager = G4Electron::Electron()->GetProcessManager();
  pManager->AddProcess(&electronMultipleScattering, -1, 1, 1);
  pManager->AddProcess(&electronIonisation,         -1, 2, 2);
  pManager->AddProcess(&electronBremsStrahlung,     -1, 3, 3);  

  //Positron Physics
  pManager = G4Positron::Positron()->GetProcessManager();
  pManager->AddProcess(&positronMultipleScattering, -1, 1, 1);
  pManager->AddProcess(&positronIonisation,         -1, 2, 2);
  pManager->AddProcess(&positronBremsStrahlung,     -1, 3, 3);  
  pManager->AddProcess(&annihilation,                0,-1, 4);  

  // Cerenkov Physics
  ConstructOp();
}

void G4BeamTestEMPhysics::ConstructOp()
{
  G4Cerenkov* cerenkovProcess = &cerenkov;
  G4int fMaxNumPhotonStep = 20;

  cerenkovProcess->SetMaxNumPhotonsPerStep(fMaxNumPhotonStep);
  G4cout << "MaxNumPhoton" << fMaxNumPhotonStep << G4endl; 

  cerenkovProcess->SetMaxBetaChangePerStep(10.0);
  cerenkovProcess->SetTrackSecondariesFirst(true);
 G4Scintillation* scintillationProcess = new G4Scintillation("Scintillation");
 scintillationProcess->SetScintillationYieldFactor(1.);
 scintillationProcess->SetTrackSecondariesFirst(true);
  G4OpAbsorption* absorptionProcess = new G4OpAbsorption();
  G4OpRayleigh* rayleighScatteringProcess = new G4OpRayleigh();
  G4OpMieHG* mieHGScatteringProcess = new G4OpMieHG();
  G4OpBoundaryProcess* boundaryProcess = new G4OpBoundaryProcess();

/*   cerenkovProcess->SetVerboseLevel(fVerboseLebel); */
/*  // scintillationProcess->SetVerboseLevel(fVerboseLebel); */
/*   absorptionProcess->SetVerboseLevel(fVerboseLebel); */
/*   rayleighScatteringProcess->SetVerboseLevel(fVerboseLebel); */
/*   mieHGScatteringProcess->SetVerboseLevel(fVerboseLebel); */
/*   boundaryProcess->SetVerboseLevel(fVerboseLebel); */
  
  // Use Birks Correction in the Scintillation process

 if(!G4Threading::IsWorkerThread())
 {
   G4EmSaturation* emSaturation =
             G4LossTableManager::Instance()->EmSaturation();
     scintillationProcess->AddSaturation(emSaturation);
 }

  auto theParticleIterator = GetParticleIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    if (cerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(cerenkovProcess);
      pmanager->SetProcessOrdering(cerenkovProcess,idxPostStep);
    }
   if (scintillationProcess->IsApplicable(*particle)) {
     pmanager->AddProcess(scintillationProcess);
     pmanager->SetProcessOrderingToLast(scintillationProcess, idxAtRest);
     pmanager->SetProcessOrderingToLast(scintillationProcess, idxPostStep);
   }
    if (particleName == "opticalphoton") {
      G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(absorptionProcess);
      pmanager->AddDiscreteProcess(rayleighScatteringProcess);
      pmanager->AddDiscreteProcess(mieHGScatteringProcess);
      pmanager->AddDiscreteProcess(boundaryProcess);
    }
  }
}

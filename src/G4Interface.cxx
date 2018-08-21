/* #include <dataclasses/physics/I3Particle.h> */

#include "G4Interface.h"
#include "G4BeamTestDetectorConstruction.h"
#include "G4BeamTestPhysicsList.h"
#include "G4BeamTestUserTrackingAction.h"
#include "G4BeamTestUserSteppingAction.h"

/* #include <icetray/I3Logging.h> */

#ifdef G4VIS_USE
#include <G4VisExecutive.hh>
#endif

#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
/* #include <G4UImanager.hh> */


G4Interface* G4Interface::g4Interface_ = NULL;

G4Interface::G4Interface(const std::string& visMacro):
  detector_(NULL), initialized_(false),
  eventInitialized_(false), visMacro_(visMacro)
{
  g4Interface_ = this;

  // Visualization manager
#ifdef G4VIS_USE
  visManager_ = NULL;
  visManager_ = new G4VisExecutive();
  visManager_->Initialize();
#endif
}


G4Interface::~G4Interface()
{
  g4Interface_ = NULL;

#ifdef G4VIS_USE
  if(visManager_) delete visManager_;
#endif
}


void G4Interface::InstallTank(G4BeamTestTank* tank)
{
  if(initialized_)
  {
    /* log_fatal("G4Interface aleady initialized. Cannot install tank!"); */
    G4cout << "G4Interface aleady initialized. Cannot install tank!" << G4endl;
    return;
  }

  if(!detector_) detector_ = new G4BeamTestDetectorConstruction();
  detector_->InstallTank(tank);
}


void G4Interface::InitializeEvent()
{
  ///
  /// An IceTray EVENT corresponds to a G4RUN
  /// where each injected particle initiates an G4EVENT
  ///

  if(!initialized_)
  {
    Initialize();
  }

  if(!eventInitialized_)
  {
    runManager_.InitializeRun();
    eventInitialized_ = true;
  }
}


void G4Interface::InjectParticle(
        const std::string& particleName, const G4ThreeVector& particlePosition,
        const G4ThreeVector& particleDirection, const G4double particleEnergy
        )
{
  if(!eventInitialized_)
  {
    /* log_fatal("No event initialized. Cannot inject particle!"); */
    G4cout << "No event initialized. Cannot inject particle!" << G4endl;
    return;
  }

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particleDef = NULL;
  particleDef = particleTable->FindParticle(particleName);

  // Transform coordinates to world system
  G4ThreeVector position = particlePosition - detector_->GetWorldOrigin();

  G4ParticleGun gun(1);
  gun.SetParticleDefinition(particleDef);
  gun.SetParticleEnergy(particleEnergy);
  gun.SetParticlePosition(position);
  gun.SetParticleMomentumDirection(particleDirection);

  G4cout << "Injecting %s: x=%.2f m, y=%.2f m, z=%.2f m, E=%.3f MeV",
            particleName.c_str(),
            position.x() / CLHEP::m,
            position.y() / CLHEP::m,
            position.z() / CLHEP::m,
            gun.GetParticleEnergy() / CLHEP::MeV;

  runManager_.InjectParticle(&gun);
}


void G4Interface::TerminateEvent()
{
  ///
  /// An IceTray EVENT corresponds to a G4RUN
  /// where each injected particle initiates an G4EVENT
  ///

  if(eventInitialized_)
  {
    runManager_.TerminateRun();
    eventInitialized_ = false;
  }
}


void G4Interface::Initialize()
{
  if(initialized_)
  {
    /* log_error("G4Interface has already been initialized. Ignoring this call!"); */
    G4cout << "G4Interface has already been initialized. Ignoring this call!" << G4endl;
    return;
  }

  /* log_debug("Init geometry ..."); */
  G4cout << "Init geometry ..." << G4endl;
  runManager_.SetUserInitialization(detector_);

  /* log_debug("Init physics list ..."); */
  G4cout << "Init physics list ..." << G4endl;
  runManager_.SetUserInitialization(new G4BeamTestPhysicsList());

  /* log_debug("Init UserTrackingAction ..."); */
  G4cout << "Init UserTrackingAction ..." << G4endl;
  runManager_.SetUserAction(new G4BeamTestUserTrackingAction());

  /* log_debug("Init UserSteppingAction ..."); */
  G4cout << "Init UserSteppingAction ..." << G4endl;
  runManager_.SetUserAction(new G4BeamTestUserSteppingAction());

  // Initialize G4 kernel
  /* log_debug("Init run manager ..."); */
  G4cout << "Init run manager ..." << G4endl;
  runManager_.Initialize();

  /* // Set verbosity */
  /* int verboseLevel = 0; */
  /* switch (GetIcetrayLogger()->LogLevelForUnit("G4Interface")) */
  /* { */
  /*   case I3LOG_FATAL: */
  /*   case I3LOG_ERROR: */
  /*   case I3LOG_WARN: */
  /*   case I3LOG_INFO: */
  /*   case I3LOG_NOTICE: */
  /*   default: */
  /*     verboseLevel = 0; */
  /*     break; */
  /*   case I3LOG_DEBUG: */
  /*     verboseLevel = 1; */
  /*     break; */
  /*   case I3LOG_TRACE: */
  /*     verboseLevel = 2; */
  /*     break; */
  /* } */

  // TODO(shivesh): verbosity
/*   runManager_.SetVerboseLevel(verboseLevel); */
/*   G4EventManager::GetEventManager()->SetVerboseLevel(verboseLevel); */
/*   G4EventManager::GetEventManager()->GetStackManager()->SetVerboseLevel(verboseLevel); */
/*   G4EventManager::GetEventManager()->GetTrackingManager()->SetVerboseLevel(verboseLevel); */
/* #ifdef G4VIS_USE */
/*   if(visManager_) visManager_->SetVerboseLevel(verboseLevel); */
/* #endif */

  // Execute visualization macro (if specified)
  /* if(!visMacro_.empty()) */
  /* { */
  /*   G4UImanager* uim = G4UImanager::GetUIpointer(); */
  /*  */
  /*   // Checking geometry */
  /*   uim->ApplyCommand("/geometry/test/grid_test"); */
  /*  */
  /*   // Execute visualization macro */
  /*   std::string visCmd = "/control/execute " + visMacro_; */
  /*   uim->ApplyCommand(visCmd.c_str()); */
  /* } */

  initialized_ = true;
}

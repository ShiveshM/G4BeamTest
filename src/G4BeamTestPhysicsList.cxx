#include <globals.hh>
#include <G4Version.hh>
#include "G4BeamTestPhysicsList.h"
// #include "G4BeamTestGeneralPhysics.h"
/* #if G4VERSION_NUMBER < 1000 */
/* #include "G4BeamTestEMPhysics.h" */
/* #include "G4BeamTestMuonPhysics.h" */
/* #include "G4BeamTestHadronPhysics.h" */
/* #include "G4BeamTestIonPhysics.h" */
/* #else */
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
//#include "G4NeutronTrackingCut.hh"
#include "G4DataQuestionaire.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include <FTFP_BERT.hh>
/* #endif */

#include <G4OpticalPhysics.hh>

#include <G4ProcessManager.hh>
#include <G4ParticleTypes.hh>
#include <G4UserSpecialCuts.hh>

#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsFTFP_BERT_TRV.hh"
#include "G4HadronPhysicsFTF_BIC.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_FTFP_BERT.hh"
#include "G4HadronPhysicsQGS_BIC.hh"
#include "G4RadioactiveDecayPhysics.hh"

G4BeamTestPhysicsList::G4BeamTestPhysicsList()
  : G4VUserPhysicsList()
{
  defaultCutValue = 0.7*CLHEP::mm;
  SetVerboseLevel(1);

  // RegisterPhysics(new G4BeamTestGeneralPhysics);
/* #if G4VERSION_NUMBER < 1000 */
/*   RegisterPhysics(new G4BeamTestEMPhysics); */
/*   RegisterPhysics(new G4BeamTestMuonPhysics); */
/*   RegisterPhysics(new G4BeamTestHadronPhysics); */
/*   RegisterPhysics(new G4BeamTestIonPhysics); */
/* #else */
  // The following is basically Geant4's FTFP_BERT physics list
  G4DataQuestionaire it(photon); // this checks that G4LEVELGAMMADATA is there

  RegisterPhysics(new G4OpticalPhysics());

  RegisterPhysics(new G4EmStandardPhysics());
  RegisterPhysics(new G4EmExtraPhysics());
  RegisterPhysics(new G4DecayPhysics());
  RegisterPhysics(new G4HadronElasticPhysics());
  // RegisterPhysics(new G4HadronPhysicsFTFP_BERT());
  RegisterPhysics(new G4StoppingPhysics());
  RegisterPhysics(new G4IonPhysics());
  RegisterPhysics(new G4HadronPhysicsQGSP_BERT());
  // RegisterPhysics(new G4HadronInelasticQBBC());
  //RegisterPhysics(new G4NeutronTrackingCut());
/* #endif */
}


G4BeamTestPhysicsList::~G4BeamTestPhysicsList()
{
}


void G4BeamTestPhysicsList::SetCuts()
{
  //G4VUserPhysicsList::SetCutsWithDefault method sets
  //the default cut value for all particle types
  //
  SetCutsWithDefault();
  if (verboseLevel>0) DumpCutValuesTable();
}


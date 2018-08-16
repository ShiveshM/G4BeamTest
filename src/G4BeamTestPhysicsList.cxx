/**
 * Copyright (C) 2009
 * The IceCube collaboration
 * ID: $Id: G4BeamTestPhysicsList.cxx 152849 2017-01-20 21:44:25Z jgonzalez $
 *
 * @file G4BeamTestPhysicsList.cxx
 * @version $Rev: 152849 $
 * @date $Date: 2017-01-20 21:44:25 +0000 (Fri, 20 Jan 2017) $
 * @author Tilo Waldenmaier, Thomas Melzig, Javier Gonzalez
 */


#include <globals.hh>
#include <G4Version.hh>
#include "G4BeamTestPhysicsList.h"
#include "G4BeamTestGeneralPhysics.h"
#if G4VERSION_NUMBER < 1000
#include "G4BeamTestEMPhysics.h"
#include "G4BeamTestMuonPhysics.h"
#include "G4BeamTestHadronPhysics.h"
#include "G4BeamTestIonPhysics.h"
#else
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
#endif

#include <G4ProcessManager.hh>
#include <G4ParticleTypes.hh>
#include <G4UserSpecialCuts.hh>


G4BeamTestPhysicsList::G4BeamTestPhysicsList()
  : G4VUserPhysicsList()
{
  defaultCutValue = 0.7*CLHEP::mm;
  SetVerboseLevel(1);

  RegisterPhysics(new G4BeamTestGeneralPhysics);
#if G4VERSION_NUMBER < 1000
  RegisterPhysics(new G4BeamTestEMPhysics);
  RegisterPhysics(new G4BeamTestMuonPhysics);
  RegisterPhysics(new G4BeamTestHadronPhysics);
  RegisterPhysics(new G4BeamTestIonPhysics);
#else
  // The following is basically Geant4's FTFP_BERT physics list
  G4DataQuestionaire it(photon); // this checks that G4LEVELGAMMADATA is there

  RegisterPhysics(new G4EmStandardPhysics());
  RegisterPhysics(new G4EmExtraPhysics());
  RegisterPhysics(new G4DecayPhysics());
  RegisterPhysics(new G4HadronElasticPhysics());
  RegisterPhysics(new G4HadronPhysicsFTFP_BERT());
  RegisterPhysics(new G4StoppingPhysics());
  RegisterPhysics(new G4IonPhysics());
  //RegisterPhysics(new G4NeutronTrackingCut());
#endif
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


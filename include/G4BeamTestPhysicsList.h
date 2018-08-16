/**
 * Copyright (C) 2009
 * The IceCube collaboration
 * ID: $Id: G4BeamTestPhysicsList.h 149388 2016-08-18 21:50:04Z jgonzalez $
 *
 * @file G4BeamTestPhysicsList.h
 * @version $Rev: 149388 $
 * @date $Date: 2016-08-18 22:50:04 +0100 (Thu, 18 Aug 2016) $
 * @author Tilo Waldenmaier, Thomas Melzig, Fabian Kislat
 */

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

#ifndef G4BEAMTESTUSERSTEPPINGACTION_H_INCLUDED
#define G4BEAMTESTUSERSTEPPINGACTION_H_INCLUDED
/**
 * Copyright (C) 2011
 * The IceCube collaboration
 * ID: $Id$
 *
 * @file G4BeamTestUserSteppingAction.h
 * @version $Revision$
 * @date $Date$
 * @author Thomas Melzig
 *
 * $LastChangedBy$
 */


#include "G4UserSteppingAction.hh"

/**
 * Implementation of G4UserSteppingAction. This class kills gammas below threshold (set by G4BeamTestTank).
 */
class G4BeamTestUserSteppingAction : public G4UserSteppingAction {

  public:
    G4BeamTestUserSteppingAction();
   ~G4BeamTestUserSteppingAction() {}

    void UserSteppingAction(const G4Step*);
};

#endif  // G4BEAMTESTUSERSTEPPINGACTION_H_INCLUDED

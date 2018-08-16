#ifndef G4BEAMTESTUSERTRACKINGACTION_H_INCLUDED
#define G4BEAMTESTUSERTRACKINGACTION_H_INCLUDED
/**
 * Copyright (C) 2011
 * The IceCube collaboration
 * ID: $Id$
 *
 * @file G4BeamTestUserTrackingAction.h
 * @version $Revision$
 * @date $Date$
 * @author Thomas Melzig
 *
 * $LastChangedBy$
 */


#include "G4UserTrackingAction.hh"

/**
 * Implementation of G4UserTrackingAction. This class kills gammas below threshold (set by G4BeamTestTank).
 */
class G4BeamTestUserTrackingAction : public G4UserTrackingAction {

  public:
    G4BeamTestUserTrackingAction();
   ~G4BeamTestUserTrackingAction() {}

    void PreUserTrackingAction(const G4Track*);
    void PostUserTrackingAction(const G4Track*);
};

#endif  // G4BEAMTESTUSERTRACKINGACTION_H_INCLUDED

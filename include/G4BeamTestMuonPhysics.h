/*
 * copyright  (C) 2010
 * The Icecube Collaboration
 *
 * $Id: G4BeamTestMuonPhysics.h 154687 2017-04-05 15:46:57Z jgonzalez $
 *
 * @version $Revision: 154687 $
 * @date $LastChangedDate: 2017-04-05 16:46:57 +0100 (Wed, 05 Apr 2017) $
 * @author Fabian Kislat <fabian.kislat@desy.de>, Peter Nießen (tanktop)
 * Last changed by: $LastChangedBy: jgonzalez $
 */

#ifndef G4TANKRESPONSE_G4BEAMTESTMUONPHYSICS_H_INCLUDED
#define G4TANKRESPONSE_G4BEAMTESTMUONPHYSICS_H_INCLUDED

#include <globals.hh>
#include <G4VPhysicsConstructor.hh>
#include <G4MuMultipleScattering.hh>
#include <G4MuBremsstrahlung.hh>
#include <G4MuPairProduction.hh>
#include <G4MuIonisation.hh>
#include <G4hIonisation.hh>
#include <G4MuonMinusCaptureAtRest.hh>

/**
   @class G4BeamTestMuonPhysics
   @brief Muon/tau Physics. Used only if Geant4 version is earlier than 4.10.

   This class implements the physics processes for the muons. For
   muons, it contains
   - Ionisation
   - Multiple scattering
   - Bremsstrahlung
   - Pair production
   - Capture at rest (mu-)
   For taus, it does
   - Multiple scattering
   - Ionisation
*/
class G4BeamTestMuonPhysics : public G4VPhysicsConstructor
{
public: 
  G4BeamTestMuonPhysics();
  ~G4BeamTestMuonPhysics();
  
  void ConstructParticle();
  void ConstructProcess();

private:
  // Muon physics
  G4MuIonisation         muPlusIonisation_;
  G4MuMultipleScattering   muPlusMultipleScattering_;
  G4MuBremsstrahlung     muPlusBremsstrahlung_;
  G4MuPairProduction     muPlusPairProduction_;

  G4MuIonisation         muMinusIonisation_;
  G4MuMultipleScattering   muMinusMultipleScattering_;
  G4MuBremsstrahlung     muMinusBremsstrahlung_;
  G4MuPairProduction     muMinusPairProduction_;

  G4MuonMinusCaptureAtRest muMinusCaptureAtRest_;

  // Tau physics
  G4MuMultipleScattering   tauPlusMultipleScattering_;
  G4hIonisation          tauPlusIonisation_;

  G4MuMultipleScattering   tauMinusMultipleScattering_;
  G4hIonisation          tauMinusIonisation_;
};

#endif // G4TANKRESPONSE_G4BEAMTESTMUONPHYSICS_H_INCLUDED

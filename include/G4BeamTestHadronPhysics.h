/*
 * copyright  (C) 2010
 * The Icecube Collaboration
 *
 * $Id: G4BeamTestHadronPhysics.h 154687 2017-04-05 15:46:57Z jgonzalez $
 *
 * @version $Revision: 154687 $
 * @date $LastChangedDate: 2017-04-05 16:46:57 +0100 (Wed, 05 Apr 2017) $
 * @author Fabian Kislat <fabian.kislat@desy.de> Last changed by: $LastChangedBy: jgonzalez $
 */

#ifndef G4TANKRESPONSE_G4BEAMTESTHADRONPHYSICS_H_INCLUDED
#define G4TANKRESPONSE_G4BEAMTESTHADRONPHYSICS_H_INCLUDED

#include <vector>

#include <globals.hh>

#include <G4VPhysicsConstructor.hh>

#include <G4hMultipleScattering.hh>
#include <G4hIonisation.hh>

#include <G4HadronElasticProcess.hh>
#include <G4HadronFissionProcess.hh>
#include <G4HadronCaptureProcess.hh>

#include <G4PionPlusInelasticProcess.hh>
#include <G4PionMinusInelasticProcess.hh>
#include <G4KaonPlusInelasticProcess.hh>
#include <G4KaonZeroSInelasticProcess.hh>
#include <G4KaonZeroLInelasticProcess.hh>
#include <G4KaonMinusInelasticProcess.hh>
#include <G4ProtonInelasticProcess.hh>
#include <G4AntiProtonInelasticProcess.hh>
#include <G4NeutronInelasticProcess.hh>
#include <G4AntiNeutronInelasticProcess.hh>
#include <G4LambdaInelasticProcess.hh>
#include <G4AntiLambdaInelasticProcess.hh>
#include <G4SigmaPlusInelasticProcess.hh>
#include <G4SigmaMinusInelasticProcess.hh>
#include <G4AntiSigmaPlusInelasticProcess.hh>
#include <G4AntiSigmaMinusInelasticProcess.hh>
#include <G4XiZeroInelasticProcess.hh>
#include <G4XiMinusInelasticProcess.hh>
#include <G4AntiXiZeroInelasticProcess.hh>
#include <G4AntiXiMinusInelasticProcess.hh>
#include <G4DeuteronInelasticProcess.hh>
#include <G4TritonInelasticProcess.hh>
#include <G4AlphaInelasticProcess.hh>
#include <G4OmegaMinusInelasticProcess.hh>
#include <G4AntiOmegaMinusInelasticProcess.hh>

// Low-energy Models
#include <G4LElastic.hh>   
#include <G4LFission.hh>
#include <G4LCapture.hh>

#include <G4LEPionPlusInelastic.hh>
#include <G4LEPionMinusInelastic.hh>
#include <G4LEKaonPlusInelastic.hh>
#include <G4LEKaonZeroSInelastic.hh>
#include <G4LEKaonZeroLInelastic.hh>
#include <G4LEKaonMinusInelastic.hh>
#include <G4LEProtonInelastic.hh>
#include <G4LEAntiProtonInelastic.hh>
#include <G4LENeutronInelastic.hh>
#include <G4LEAntiNeutronInelastic.hh>
#include <G4LELambdaInelastic.hh>
#include <G4LEAntiLambdaInelastic.hh>
#include <G4LESigmaPlusInelastic.hh>
#include <G4LESigmaMinusInelastic.hh>
#include <G4LEAntiSigmaPlusInelastic.hh>
#include <G4LEAntiSigmaMinusInelastic.hh>
#include <G4LEXiZeroInelastic.hh>
#include <G4LEXiMinusInelastic.hh>
#include <G4LEAntiXiZeroInelastic.hh>
#include <G4LEAntiXiMinusInelastic.hh>
#include <G4LEDeuteronInelastic.hh>
#include <G4LETritonInelastic.hh>
#include <G4LEAlphaInelastic.hh>
#include <G4LEOmegaMinusInelastic.hh>
#include <G4LEAntiOmegaMinusInelastic.hh>

// High-energy Models

#include <G4HEPionPlusInelastic.hh>
#include <G4HEPionMinusInelastic.hh>
#include <G4HEKaonPlusInelastic.hh>
#include <G4HEKaonZeroInelastic.hh>
#include <G4HEKaonZeroInelastic.hh>
#include <G4HEKaonMinusInelastic.hh>
#include <G4HEProtonInelastic.hh>
#include <G4HEAntiProtonInelastic.hh>
#include <G4HENeutronInelastic.hh>
#include <G4HEAntiNeutronInelastic.hh>
#include <G4HELambdaInelastic.hh>
#include <G4HEAntiLambdaInelastic.hh>
#include <G4HESigmaPlusInelastic.hh>
#include <G4HESigmaMinusInelastic.hh>
#include <G4HEAntiSigmaPlusInelastic.hh>
#include <G4HEAntiSigmaMinusInelastic.hh>
#include <G4HEXiZeroInelastic.hh>
#include <G4HEXiMinusInelastic.hh>
#include <G4HEAntiXiZeroInelastic.hh>
#include <G4HEAntiXiMinusInelastic.hh>
#include <G4HEOmegaMinusInelastic.hh>
#include <G4HEAntiOmegaMinusInelastic.hh>

// Stopping processes
#include <G4AntiProtonAnnihilationAtRest.hh>
#include <G4AntiNeutronAnnihilationAtRest.hh>

#ifdef TRIUMF_STOP_PIMINUS
#include <G4PionMinusAbsorptionAtRest.hh>
#else
#include <G4PiMinusAbsorptionAtRest.hh>
#endif
#ifdef TRIUMF_STOP_KMINUS
#include <G4KaonMinusAbsorption.hh>
#else
#include <G4KaonMinusAbsorptionAtRest.hh>
#endif

// quark gluon string model with chips afterburner.
#include <G4TheoFSGenerator.hh>
#include <G4ExcitationHandler.hh>
#include <G4PreCompoundModel.hh>
#include <G4GeneratorPrecompoundInterface.hh>
#include <G4QGSModel.hh>
#include <G4QGSParticipants.hh>
#include <G4QGSMFragmentation.hh>
#include <G4ExcitedStringDecay.hh>

/**
   @brief Hadron physics. Used only if Geant4 version is earlier than 4.10.
   @author GEANT4/Peter Niessen
   @date Sat Jul 24 23:53:47 EDT 2004

   Many processes. You're encouraged to check the source.
*/
class G4BeamTestHadronPhysics : public G4VPhysicsConstructor {
public: 

  /**
   * The constructor
   */
  G4BeamTestHadronPhysics();

  /**
   * The virtual destructor
   */
  virtual ~G4BeamTestHadronPhysics();

  /**
   * This method will be invoked in the Construct() method. 
   * each particle type will be instantiated
   */
  virtual void ConstructParticle();
 
  /**
   * This method will be invoked in the Construct() method.
   * each physics process will be instantiated and
   * registered to the process manager of each particle type 
   */
  virtual void ConstructProcess();

protected:

  // Elastic Process
  G4HadronElasticProcess elasticProcess_;
  G4LElastic *elasticModel_;
  
  // Pi + 
  G4PionPlusInelasticProcess pionPlusInelastic_;
  G4LEPionPlusInelastic *lePionPlusModel_;
  G4hMultipleScattering pionPlusMult_;
  G4hIonisation pionPlusIonisation_;

  // Pi -
  G4PionMinusInelasticProcess pionMinusInelastic_;
  G4LEPionMinusInelastic *lePionMinusModel_;
  G4hMultipleScattering pionMinusMult_;
  G4hIonisation pionMinusIonisation_;
#ifdef TRIUMF_STOP_PIMINUS
  G4PionMinusAbsorptionAtRest pionMinusAbsorption_;
#else
  G4PiMinusAbsorptionAtRest pionMinusAbsorption_;
#endif

  // pi+ and pi-
  G4TheoFSGenerator theoModel_;
  G4ExcitationHandler excitationHandler_;
  G4PreCompoundModel *preEquilib_;
  G4GeneratorPrecompoundInterface cascade_;
  G4QGSModel< G4QGSParticipants > stringModel_;
  G4QGSMFragmentation fragmentation_;
  G4ExcitedStringDecay *stringDecay_;

  // K + 
  G4KaonPlusInelasticProcess kaonPlusInelastic_;
  G4LEKaonPlusInelastic *leKaonPlusModel_;
  G4HEKaonPlusInelastic *heKaonPlusModel_;
  G4hMultipleScattering kaonPlusMult_;
  G4hIonisation kaonPlusIonisation_;
	
  // K -
  G4KaonMinusInelasticProcess kaonMinusInelastic_;
  G4LEKaonMinusInelastic *leKaonMinusModel_;
  G4HEKaonMinusInelastic *heKaonMinusModel_;
  G4hMultipleScattering kaonMinusMult_;
  G4hIonisation kaonMinusIonisation_;
#ifdef TRIUMF_STOP_KMINUS
  G4KaonMinusAbsorption kaonMinusAbsorption_;
#else
  G4PiMinusAbsorptionAtRest kaonMinusAbsorption_;
#endif

  // K0L
  G4KaonZeroLInelasticProcess kaonZeroLInelastic_;
  G4LEKaonZeroLInelastic *leKaonZeroLModel_;
  G4HEKaonZeroInelastic *heKaonZeroLModel_;

  // K0S
  G4KaonZeroSInelasticProcess kaonZeroSInelastic_;
  G4LEKaonZeroSInelastic *leKaonZeroSModel_;
  G4HEKaonZeroInelastic *heKaonZeroSModel_;

  // Proton
  G4ProtonInelasticProcess protonInelastic_;
  G4LEProtonInelastic *leProtonModel_;
  G4HEProtonInelastic *heProtonModel_;
  G4hMultipleScattering protonMult_;
  G4hIonisation protonIonisation_;
 
  // anti-proton
  G4AntiProtonInelasticProcess antiProtonInelastic_;
  G4LEAntiProtonInelastic *leAntiProtonModel_;
  G4HEAntiProtonInelastic *heAntiProtonModel_;
  G4hMultipleScattering antiProtonMult_;
  G4hIonisation antiProtonIonisation_;
  G4AntiProtonAnnihilationAtRest antiProtonAnnihilation_;
    
  // neutron
  G4NeutronInelasticProcess neutronInelastic_;
  G4LENeutronInelastic *leNeutronModel_;
  G4HENeutronInelastic *heNeutronModel_;
  G4HadronFissionProcess neutronFission_;
  G4LFission *neutronFissionModel_;
  G4HadronCaptureProcess neutronCapture_;
  G4LCapture *neutronCaptureModel_;


  // anti-neutron
  G4AntiNeutronInelasticProcess antiNeutronInelastic_;
  G4LEAntiNeutronInelastic *leAntiNeutronModel_;
  G4HEAntiNeutronInelastic *heAntiNeutronModel_;
  G4AntiNeutronAnnihilationAtRest antiNeutronAnnihilation_;
   
  // Lambda
  G4LambdaInelasticProcess lambdaInelastic_;
  G4LELambdaInelastic *leLambdaModel_;
  G4HELambdaInelastic *heLambdaModel_;
  
  // AntiLambda
  G4AntiLambdaInelasticProcess antiLambdaInelastic_;
  G4LEAntiLambdaInelastic *leAntiLambdaModel_;
  G4HEAntiLambdaInelastic *heAntiLambdaModel_;
  
  // SigmaMinus
  G4SigmaMinusInelasticProcess sigmaMinusInelastic_;
  G4LESigmaMinusInelastic *leSigmaMinusModel_;
  G4HESigmaMinusInelastic *heSigmaMinusModel_;
  G4hMultipleScattering sigmaMinusMult_;
  G4hIonisation sigmaMinusIonisation_;
  
  // AntiSigmaMinus
  G4AntiSigmaMinusInelasticProcess antiSigmaMinusInelastic_;
  G4LEAntiSigmaMinusInelastic *leAntiSigmaMinusModel_;
  G4HEAntiSigmaMinusInelastic *heAntiSigmaMinusModel_;
  G4hMultipleScattering antiSigmaMinusMult_;
  G4hIonisation antiSigmaMinusIonisation_;
   
  // SigmaPlus
  G4SigmaPlusInelasticProcess sigmaPlusInelastic_;
  G4LESigmaPlusInelastic *leSigmaPlusModel_;
  G4HESigmaPlusInelastic *heSigmaPlusModel_;
  G4hMultipleScattering sigmaPlusMult_;
  G4hIonisation sigmaPlusIonisation_;
  
  // AntiSigmaPlus
  G4AntiSigmaPlusInelasticProcess antiSigmaPlusInelastic_;
  G4LEAntiSigmaPlusInelastic *leAntiSigmaPlusModel_;
  G4HEAntiSigmaPlusInelastic *heAntiSigmaPlusModel_;
  G4hMultipleScattering antiSigmaPlusMult_;
  G4hIonisation antiSigmaPlusIonisation_;
  
  // XiZero
  G4XiZeroInelasticProcess xiZeroInelastic_;
  G4LEXiZeroInelastic *leXiZeroModel_;
  G4HEXiZeroInelastic *heXiZeroModel_;
  
  // AntiXiZero
  G4AntiXiZeroInelasticProcess  antiXiZeroInelastic_;
  G4LEAntiXiZeroInelastic*  leAntiXiZeroModel_;
  G4HEAntiXiZeroInelastic*  heAntiXiZeroModel_;
  
  // XiMinus
  G4XiMinusInelasticProcess xiMinusInelastic_;
  G4LEXiMinusInelastic *leXiMinusModel_;
  G4HEXiMinusInelastic *heXiMinusModel_;
  G4hMultipleScattering xiMinusMult_;
  G4hIonisation xiMinusIonisation_;

  // AntiXiMinus
  G4AntiXiMinusInelasticProcess antiXiMinusInelastic_;
  G4LEAntiXiMinusInelastic *leAntiXiMinusModel_;
  G4HEAntiXiMinusInelastic *heAntiXiMinusModel_;
  G4hMultipleScattering antiXiMinusMult_;
  G4hIonisation antiXiMinusIonisation_;
  
  // OmegaMinus
  G4OmegaMinusInelasticProcess omegaMinusInelastic_;
  G4LEOmegaMinusInelastic *leOmegaMinusModel_;
  G4HEOmegaMinusInelastic *heOmegaMinusModel_;
  G4hMultipleScattering omegaMinusMult_;
  G4hIonisation omegaMinusIonisation_;
   
  // AntiOmegaMinus
  G4AntiOmegaMinusInelasticProcess antiOmegaMinusInelastic_;
  G4LEAntiOmegaMinusInelastic *leAntiOmegaMinusModel_;
  G4HEAntiOmegaMinusInelastic *heAntiOmegaMinusModel_;
  G4hMultipleScattering antiOmegaMinusMult_;
  G4hIonisation antiOmegaMinusIonisation_;
};


#endif // G4TANKRESPONSE_G4BEAMTESTHADRONPHYSICS_H_INCLUDED

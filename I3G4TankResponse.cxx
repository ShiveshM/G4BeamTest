/**
 * Copyright (C) 2009
 * The IceCube collaboration
 * ID: $Id: I3G4TankResponse.cxx 158929 2017-10-20 00:58:24Z cweaver $
 *
 * @file I3G4TankResponse.cxx
 * @version $Revision: 158929 $
 * @date $Date: 2017-10-20 01:58:24 +0100 (Fri, 20 Oct 2017) $
 * @author Tilo Waldenmaier <tilo.waldenmaier@desy.de>, Thomas Melzig <thomas.melzig@desy.de>
 *
 * $LastChangedBy: cweaver $
 */


#include <topsimulator/interface/I3IceTopResponseFactory.h>
#include <topsimulator/GeoFunctions.h>
#include <g4-tankresponse/g4classes/G4IceTopTank.h>
#include <g4-tankresponse/g4classes/G4Interface.h>
#include <g4-tankresponse/I3G4TankResponse.h>
//#include <pmt-simulator/I3PMTConstants.h>
#include <phys-services/I3GSLRandomService.h>
#include <dataclasses/physics/I3Particle.h>
#include <dataclasses/I3Direction.h>
#include <simclasses/I3MCPE.h>

#include <cmath>

const double I3G4TankResponse::PHOTONS_PER_VEM = 32880.;
const double I3G4TankResponse::WEIGHTED_PHOTONS_PER_VEM = 1435.;
const double I3G4TankResponse::MEV_PER_VEM = 186.6 * I3Units::MeV;
const double I3G4TankResponse::VEM_THRESHOLD = 2000.0;

I3G4TankResponse::I3G4TankResponse(I3Configuration& config, const I3Context& context, const TankKey& tankKey):
  I3TankResponse(config, context, tankKey),  tauZirco_(7.94 * I3Units::m / I3Constants::c),
  tauTyvek_(12.6 * I3Units::m / I3Constants::c),
  decayTime_(0), safetyMargin_(0.3*I3Units::m), chargeScale_(1),
  cherenkovMethod_(true), cherenkovWeighting_(false),
  particlesBeforeThreshold_(0), particlesAfterThreshold_(0),
  vemCounter_(0), visMacroFile_(""), pePerVEM_(), randomServiceName_(""),
  g4Interface_(NULL), g4Tank_(NULL)
{
  AddParameter("TimeConstantZirco",
               "Time constant for tanks with zirconium coating.",
               tauZirco_);
  AddParameter("TimeConstantTyvek",
               "Time constant for tanks with a tykec liner.",
               tauTyvek_);
  AddParameter("VisMacro", "Geant4 visualization macro file",
               visMacroFile_);
  AddParameter("RandomServiceName", "Name of random service",
               randomServiceName_);
  AddParameter("SafetyMargin",
               "Check if a particle hits the tank within safety margin.",
               safetyMargin_);
  AddParameter("ChargeScale",
               "Scale all charges by this factor",
               chargeScale_);
  AddParameter("CherenkovMethod",
               "Use the number of created cherenkov photons instead of energy deposition",
               cherenkovMethod_);
  AddParameter("CherenkovWeighting",
               "Use position dependent weighting of the number of created cherenkov photons",
               cherenkovWeighting_);
}


I3G4TankResponse::~I3G4TankResponse()
{
  if (G4Interface::GetInstance()) {
    delete g4Interface_;
  }
}


void I3G4TankResponse::Configure()
{
  log_info("Configuring Tank Response:");
  
  GetParameter("TimeConstantZirco", tauZirco_);
  log_info(" + Time Constant (Zirconium): %.1f ns", tauZirco_ / I3Units::ns);
  
  GetParameter("TimeConstantTyvek", tauTyvek_);
  log_info(" + Time Constant (Tyvek)    : %.1f ns", tauTyvek_ / I3Units::ns);
  
  GetParameter("VisMacro", visMacroFile_);
#ifdef G4VIS_USE
  log_info(" + Visualization macro      : %s", (visMacroFile_.empty() ? "DISABLED" : visMacroFile_.c_str()));
#else
  if (!visMacroFile_.empty()) {
    log_fatal("VisMacro non-empty, but no Geant4 visualization in this build");
  }
#endif

  GetParameter("SafetyMargin", safetyMargin_);
  log_info(" + Safety margin            : %.1f m", safetyMargin_ / I3Units::m);
  
  GetParameter("ChargeScale", chargeScale_);
  log_info(" + Charge scale             : %.2f", chargeScale_);
  
  GetParameter("CherenkovMethod", cherenkovMethod_);
  log_info(" + Cherenkov method         : %s", cherenkovMethod_ ? "PHOTON COUNTING" : "ENERGY DEPOSIT");
  
  GetParameter("CherenkovWeighting", cherenkovWeighting_);
  log_info(" + Cherenkov weighting      : %s", cherenkovWeighting_ ? "ENABLED" : "DISABLED");
  
  log_info(" + Saturation threshold     : %.1f VEM", VEM_THRESHOLD);
  
  GetParameter("RandomServiceName", randomServiceName_);
  
  // Look for a random generator service
  if (randomServiceName_.empty())
  {
    randomService_ = I3RandomServicePtr(new I3GSLRandomService(0));
    log_info("+ Random service           : I3GSLRandomService  (default)");
  }
  else
  {
    randomService_ = GetContext().Get<I3RandomServicePtr>(randomServiceName_);
    log_info("+ Random service           : %s  (EXTERNAL)",  randomServiceName_.c_str());
  }
  if(!randomService_) log_fatal("Missing random service!");
}


double I3G4TankResponse::GetX() const
{
  return (g4Tank_ ? g4Tank_->GetX_I3() : NAN);
}


double I3G4TankResponse::GetY() const
{
  return (g4Tank_ ? g4Tank_->GetY_I3() : NAN);
}


double I3G4TankResponse::GetZ() const
{
  return (g4Tank_ ? g4Tank_->GetZ_I3() : NAN);
}


double I3G4TankResponse::GetTankRadius() const
{
  return (g4Tank_ ? g4Tank_->GetTankRadius_I3() : NAN);
}


double I3G4TankResponse::GetTankHeight() const
{
  return (g4Tank_ ? g4Tank_->GetTankHeight_I3() : NAN);
}


double I3G4TankResponse::GetSnowHeight() const
{
  return (g4Tank_ ? g4Tank_->GetSnowHeight_I3() : NAN);
}


void I3G4TankResponse::Initialize(const I3Geometry& geometry,
                                  const I3Calibration& calib,
                                  const I3DetectorStatus& status)
{
  // Get the stations geometry
  const I3StationGeoMap& stationMap = geometry.stationgeo;
  
  // Get the dom status
  const std::map<OMKey, I3DOMStatus>& domStatusMap = status.domStatus;
  
  // Get the VEM calibration
  const std::map<OMKey, I3VEMCalibration>& vemCalMap = calib.vemCal;
  
  I3StationGeoMap::const_iterator station_iter = stationMap.find(tankKey_.string);
  if (station_iter==stationMap.end()) {
    log_fatal("Station %d doesn't exist in geometry!", tankKey_.string);
    return;
  }
  
  unsigned int tankID = tankKey_.tank == TankKey::TankA ? 0 : 1;
  const I3TankGeo& tankGeo = station_iter->second.at(tankID);
  
  // Set decay time for specific tank coating
  switch (tankGeo.tanktype) 
  {
    case I3TankGeo::Tyvek_Lined:
      decayTime_ = tauTyvek_;
      break;
    case I3TankGeo::Zirconium_Lined:
      decayTime_ = tauZirco_;
      break;
    default:
      log_fatal_stream("Unknown type of tank " << tankKey_ << "! Is your GCD-file up-to-date?");
      return;
  }
  
  // Loop over all DOMs in the tank
  pePerVEM_.clear();
  I3Vector<OMKey>::const_iterator dom_iter;
  for (dom_iter = tankGeo.omKeyList_.begin();
       dom_iter != tankGeo.omKeyList_.end(); dom_iter++)
  {
    // Get the DOMStatus of this DOM to see if it is in the configuration
    std::map<OMKey, I3DOMStatus>::const_iterator status_iter = domStatusMap.find(*dom_iter);
    if(status_iter==domStatusMap.end()) continue;
    
    // Check if PMT is powered up
    if(!(status_iter->second.pmtHV>0)) {
      log_warn_stream("HV of " << *dom_iter << " is down!");
    }
    
    // Get the VEM calibration
    std::map<OMKey, I3VEMCalibration>::const_iterator vem_iter = vemCalMap.find(*dom_iter);
    if (vem_iter==vemCalMap.end()) {
      log_warn_stream("Missing VEM calibration of module " << *dom_iter << ". Skipping it!");
      continue;
    }
    // Fill the map of pePerVEM values for each DOM
    // The actual pePerVEM value is scaled down by the mean PE charge (0.85) to account for the
    // asymmetric single photoelectron distribution --> This only works together with the
    // pmt-simulator option "TreatIceTopAsInIce"=True!!!!
    //pePerVEM_[*dom_iter] = (vem_iter->second.pePerVEM / vem_iter->second.corrFactor) /
    //                        I3PMTConstants::MEAN_NORMALIZED_PE;
    pePerVEM_[*dom_iter] = (vem_iter->second.pePerVEM / vem_iter->second.corrFactor) / MEAN_NORMALIZED_PE;
  }
  
  g4Interface_ = G4Interface::GetInstance();
  if (!g4Interface_) g4Interface_ = new G4Interface(visMacroFile_);
  
  g4Tank_ = new G4IceTopTank(tankKey_, geometry);
  
  g4Interface_->InstallTank(g4Tank_);
}


void I3G4TankResponse::BeginEvent(const I3Particle& primary)
{
  g4Interface_->InitializeEvent();
  // Get the primary direction
  I3Direction dir = primary.GetDir();
  double x_dir = dir.GetX();
  double y_dir = dir.GetY();
  double z_dir = dir.GetZ();
  // Get the vector pointing from tank to particle
  double x_diff = primary.GetX() - g4Tank_->GetX_I3();
  double y_diff = primary.GetY() - g4Tank_->GetY_I3();
  double z_diff = primary.GetZ() - g4Tank_->GetZ_I3();
  // Get the closest distance of this tank to primary track
  double d = sqrt(pow(-x_dir * y_diff + x_diff * y_dir, 2) +
                  pow( x_dir * z_diff - x_diff * z_dir, 2) +
                  pow(-y_dir * z_diff + y_diff * z_dir, 2));
  
  double distToCore = 20.0 * I3Units::m;
  if (d < distToCore) {
    log_debug_stream("+ Tank " << tankKey_ << " lies closer than "
                     << distToCore << " m to the primary: " << d << " m");
  }
  
  // Set the VEM and particle counter to zero
  vemCounter_ = 0.0;
  particlesBeforeThreshold_ = 0;
  particlesAfterThreshold_ = 0;
}


bool I3G4TankResponse::TrackParticle(const ExtendedI3Particle& particle, HitHistoCollection& hitHC, HitHistoCollection& cherHitCollection)
{
  if (GeoFunctions::IntersectCylinder(GetX(), GetY(), GetZ(), GetVirtualTankHeight(),
                                      GetVirtualTankRadius(), particle))
  {
    if (vemCounter_ < VEM_THRESHOLD) {
      particlesBeforeThreshold_++;
    } else {
      particlesAfterThreshold_++;
      return true;
    }
    
    g4Interface_->InjectParticle(particle);
    
    // Loop over all active DOMs in tank
    double sum_vem = 0.;
    int num_vem = 0;
    std::map<OMKey, double>::const_iterator pePerVEM_iter;
    for (pePerVEM_iter=pePerVEM_.begin(); pePerVEM_iter!=pePerVEM_.end(); ++pePerVEM_iter)
    {
      const OMKey& omKey = pePerVEM_iter->first;
      ExtendedI3Particle iceTrack(particle); // copy constructor preserves ParticleID
      iceTrack.SetTime(particle.GetTime() + g4Tank_->GetTime_I3(omKey));
      iceTrack.SetShape(I3Particle::MCTrack);

      double vem_mean = 0.;
      double cher_mean = 0.;
      if (cherenkovMethod_) {
        if (cherenkovWeighting_) {
          cher_mean = g4Tank_->GetNumCherenkovWeight(omKey);
          vem_mean = cher_mean / WEIGHTED_PHOTONS_PER_VEM;
        } else {
          cher_mean = g4Tank_->GetNumCherenkov(omKey);
          vem_mean = cher_mean / PHOTONS_PER_VEM;
        }
      } else {
        cher_mean = g4Tank_->GetEDep_I3(omKey);
        vem_mean = cher_mean / MEV_PER_VEM;
      }
      sum_vem += vem_mean;
      if (vem_mean > 0) num_vem++;
      
      // Calculate mean number of photoelectrons according to actual VEM calibration
      double npe_mean = vem_mean * pePerVEM_iter->second * chargeScale_;
      
      // Dial npe according to Poisson distribution
      int num_hits = randomService_->Poisson(npe_mean);

      cherHitCollection.GetHitHisto(omKey).Fill(iceTrack.GetStartTime(), int(cher_mean), iceTrack);
      GenerateHits(num_hits, iceTrack, hitHC.GetHitHisto(omKey));
      //log_warn("DOM: %s, VEM: %.2f, ",omKey.str().c_str(), vem_mean);
    }
    if (num_vem > 0) vemCounter_ += sum_vem / num_vem;
    else return false;

    return true;
  }
  log_trace("No cylinder intersection. Tank position (%f, %f, %f), Particle position (%f, %f, %f) and direction (%f, %f, %f). Height: %f. Radius: %f.",
	    GetX(), GetY(), GetZ(),
	    particle.GetPos().GetX(), particle.GetPos().GetY(), particle.GetPos().GetZ(),
	    particle.GetDir().GetX(), particle.GetDir().GetY(), particle.GetDir().GetZ(),
	    GetVirtualTankHeight(), GetVirtualTankRadius());
  return false;
}


void I3G4TankResponse::GenerateHits(int npe, const ExtendedI3Particle& p, HitHisto& hitHisto)
{
  if (npe == 0)
    return;

  // draw individual npe with time delay from exponential distribution
  const double t0 = p.GetStartTime();

  if (npe == 1) {
    // no hokus pokus for single pe
    hitHisto.Fill(t0 + randomService_->Exp(decayTime_), 1, p);
  } else {
    // fill times into a vector and pass that to HitHisto
    std::vector<double> times(npe);
    for (int j = 0; j < npe; ++j)
      times[j] = t0 + randomService_->Exp(decayTime_);
    hitHisto.Fill(times, p);
  }
}


void I3G4TankResponse::EndEvent(HitHistoCollection &hitHC, HitHistoCollection& cherHitCollection)
{
  g4Interface_->TerminateEvent();
  if (particlesAfterThreshold_ > 0) {
    double scalingFactor = static_cast<double>(particlesAfterThreshold_) /
                           static_cast<double>(particlesBeforeThreshold_) + 1.0;

    std::map<OMKey, double>::const_iterator pePerVEM_iter;
    for (pePerVEM_iter = pePerVEM_.begin(); pePerVEM_iter != pePerVEM_.end(); ++pePerVEM_iter)
    {
      hitHC.GetHitHisto(pePerVEM_iter->first).Scale(scalingFactor);
      cherHitCollection.GetHitHisto(pePerVEM_iter->first).Scale(scalingFactor);
    }
    log_trace_stream(tankKey_ << " reached VEM-threshold, particles before: "
                              << particlesBeforeThreshold_ << ", after: "
                              << particlesAfterThreshold_ << ", scaling-factor: "
                              << scalingFactor);
  }
}


I3_SERVICE_FACTORY(I3IceTopResponseFactory<I3G4TankResponse>);

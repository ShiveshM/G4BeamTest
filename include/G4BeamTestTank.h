/**
 * Copyright (C) 2009
 * The IceCube collaboration
 * ID: $Id: G4BeamTestTank.h 149431 2016-08-19 17:38:06Z jgonzalez $
 *
 * @file G4BeamTestTank.h
 * @version $Rev: 149431 $
 * @date $Date: 2016-08-19 18:38:06 +0100 (Fri, 19 Aug 2016) $
 * @author Tilo Waldenmaier, Thomas Melzig
 */


#ifndef _TOPSIMULATOR_G4BEAMTESTTANK_H_
#define _TOPSIMULATOR_G4BEAMTESTTANK_H_

class OMKey;
struct TankKey;
class I3Geometry;
class I3Position;

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4TankIceSD;

#include <G4Types.hh>
#include <G4ThreeVector.hh>
#include <map>

#include <icetray/I3Logging.h>

/**
 * This class constructs the physical volume of single tanks, serves as bridge to the sensitive detector in the tank, and computes the location of the point that define the snow surface. It also stores the basic tank dimensions (height, radius, snow).
 *
 * The sensitive detectors are not PMTs. Instead, the ice itself is the sensitive detector through G4TankIceSD. In this way, time is saved by not tracking photons.
 *
 * The points that are used to interpolate the snow surface are chosen so the snow surface is essentially flat on top of the tanks. This is done by determining the three vertices of a 10-meter equilateral triangle centered at the tank. The orientation of the triangle depends on whether the tank is part of a station with one or two tanks (if there are more tanks in a station, a fatal error occurs). In the single-tank case, the first vertex lies along the line pointing toward the first DOM. In the two-tank case, it lies along the line pointing toward it's neighboring tank.
 *
 */
class G4BeamTestTank
{
 public:
  G4BeamTestTank(const TankKey& tankKey, const I3Geometry& geometry);

  ~G4BeamTestTank();

  const G4ThreeVector& GetPos() const {return position_;}

  /// Position of center of the tank
  I3Position GetPos_I3();
  double GetX_I3();
  double GetY_I3();
  double GetZ_I3();

  /// Delaunay points around tank
  const G4ThreeVector& GetDelaunayPoint1() const {return delaunayPoint1_;}
  const G4ThreeVector& GetDelaunayPoint2() const {return delaunayPoint2_;}
  const G4ThreeVector& GetDelaunayPoint3() const {return delaunayPoint3_;}

  /// Tank dimensions in Geant4 units
  G4double GetTankHeight_G4() {return tankHeight_;}
  G4double GetTankRadius_G4() {return outerRadius_;}
  G4double GetSnowHeight_G4() {return snowHeight_;}

  /// Tank dimensions in IceCube units and reference system
  double   GetTankHeight_I3();
  double   GetTankRadius_I3();
  double   GetSnowHeight_I3();

  /// Energy deposit for a given OM, in Geant4 units (from G4TankIceSD)
  G4double GetEDep_G4(const OMKey& omKey);
  /// Emission time for a given OM, in Geant4 units (from G4TankIceSD)
  G4double GetTime_G4(const OMKey& omKey);

  /// Energy deposit for a given OM, in Geant4 units (from G4TankIceSD)
  double   GetEDep_I3(const OMKey& omKey);
  /// Emission time for a given OM, in Geant4 units (from G4TankIceSD)
  double   GetTime_I3(const OMKey& omKey);

  /// Number of Cherenkovs for a given OM (from G4TankIceSD)
  double GetNumCherenkov(const OMKey& omKey);
  /// Number of Cherenkovs weighted by emission point (from G4TankIceSD)
  double GetNumCherenkovWeight(const OMKey& omKey);

  const TankKey& GetTankKey() const {return tankKey_;}

  /// Method to call when constructing detector
  G4VPhysicalVolume* InstallTank(G4VPhysicalVolume* mother, const G4ThreeVector& origin);

 private:

  G4double tankThickness_;
  G4double tankHeight_;
  G4double fillHeight_;
  G4double innerRadius_;
  G4double outerRadius_;

  G4double snowHeight_;
  G4double perliteHeight_;

  G4double glassOuterRadius_;
  G4double glassThickness_;

  G4ThreeVector position_;

  G4ThreeVector delaunayPoint1_;
  G4ThreeVector delaunayPoint2_;
  G4ThreeVector delaunayPoint3_;

  std::map<OMKey, G4ThreeVector> relDomPositions_;

  G4LogicalVolume* tankLog_;
  G4TankIceSD* iceSD_;

  const TankKey& tankKey_;
  const I3Geometry& geometry_;

  SET_LOGGER("G4BeamTestTank");
};

#endif

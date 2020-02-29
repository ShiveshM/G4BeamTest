# G4BeamTest

![C++ Version](https://img.shields.io/badge/c++-98+-green.svg)
[![license](https://img.shields.io/github/license/ShiveshM/G4BeamTest 'license')](https://github.com/ShiveshM/G4BeamTest/blob/master/LICENSE)

G4BeamTest is a C++ simulation of an Optical Module Beam Test at the [Fermilab
Test Beam Facility](https://ftbf.fnal.gov/) (FTBF).

![G4BeamTest Logo](logo.png)

## Installation
G4BeamTest can be installed using `cmake`
```
mkdir build && cd build
cmake -DGEANT4_USE_QT=ON ../
make
```

### Dependencies

G4BeamTest has the following dependencies:
* [`gcc-c++`](https://gcc.gnu.org/)
* [`CMake`](https://cmake.org/) >= 2.6
* [`Geant4`](https://geant4.web.cern.ch/) >= 10.4

For installation instructions of Geant4 see the [Geant4
Installation
Guide](https://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/InstallationGuide/html/).
All G4 data files including hadron cross-sections are required to be
downloaded. Here is a typical example of the environment variables which need
to be set in your `.bash_profile`:
```
export GEANTTOPDIR="/opt/geant4"
export G4LEVELGAMMADATA="$GEANTTOPDIR/share/Geant4-10.4.2/data/PhotonEvaporation5.2"
export G4NEUTRONXSDATA="$GEANTTOPDIR/share/Geant4-10.4.2/data/G4NEUTRONXS1.4"
export G4LEDATA="$GEANTTOPDIR/share/Geant4-10.4.2/data/G4EMLOW7.3"
export G4NEUTRONHPDATA="$GEANTTOPDIR/share/Geant4-10.4.2/data/G4NDL4.5"
export G4ENSDFSTATEDATA="$GEANTTOPDIR/share/Geant4-10.4.2/data/G4ENSDFSTATE2.2"
export G4RADIOACTIVEDATA="$GEANTTOPDIR/share/Geant4-10.4.2/data/RadioactiveDecay5.2"
export G4ABLADATA="$GEANTTOPDIR/share/Geant4-10.4.2/data/G4ABLA3.1"
export G4PIIDATA="$GEANTTOPDIR/share/Geant4-10.4.2/data/G4PII1.3"
export G4SAIDXSDATA="$GEANTTOPDIR/share/Geant4-10.4.2/data/G4SAIDDATA1.1"
export G4REALSURFACEDATA="$GEANTTOPDIR/share/Geant4-10.4.2/data/RealSurface2.1.1"
export LD_LIBRARY_PATH=$GEANTTOPDIR/lib64:$LD_LIBRARY_PATH
export PATH=$GEANTTOPDIR/bin:$PATH
```

Additional dependencies:
* [`Qt`](https://www.qt.io/) >= 4

Be sure to install with `Qt` support for the GUI interface shown in the
animation above.

## License

[MIT License](LICENSE)

Copyright (c) 2020 Shivesh Mandalia

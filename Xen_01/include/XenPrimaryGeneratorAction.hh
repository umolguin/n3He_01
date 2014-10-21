#ifndef XenPrimaryGeneratorAction_h
#define XenPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class XenDetectorConstruction;

/// The primary generator action class with particle gum.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued
/// in front of the phantom across 80% of the (X,Y) phantom size.

class XenPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    XenPrimaryGeneratorAction();
    virtual ~XenPrimaryGeneratorAction();

    // static access method
    static const XenPrimaryGeneratorAction* Instance();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);

    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

  private:
    static XenPrimaryGeneratorAction* fgInstance;

    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    G4int myEventCounter;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

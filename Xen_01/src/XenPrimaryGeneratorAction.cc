#include "XenPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

XenPrimaryGeneratorAction* XenPrimaryGeneratorAction::fgInstance=0;
const XenPrimaryGeneratorAction* XenPrimaryGeneratorAction::Instance()
{
    return fgInstance;
}
XenPrimaryGeneratorAction::XenPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
    myEventCounter =0;
    fParticleGun= new G4ParticleGun(1);//Number of particles

    // default particle kinematic
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    G4ParticleDefinition* particle= particleTable->FindParticle(particleName="neutron");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));

    fParticleGun->SetParticleEnergy(5.0e-3*eV);

    G4cout<<"Particle used: "<<particleName<<G4endl;

}

XenPrimaryGeneratorAction::~XenPrimaryGeneratorAction()
{
  delete fParticleGun;
}

void XenPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    //std::cout<<"XenPrimaryAction"<<std::endl;
  G4double envSizeXY = 0;
  G4double envSizeZ = 0;
  //---------------------------------------------------------------------
  G4LogicalVolume* envLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Box* envBox = NULL;
  if ( envLV ) envBox = dynamic_cast<G4Box*>(envLV->GetSolid());
  if ( envBox ) {
    envSizeXY = envBox->GetXHalfLength()*2.;
    envSizeZ = envBox->GetZHalfLength()*2.;
  }
  else  {
    G4cerr << "Envelope volume of box shape not found." << G4endl;
    G4cerr << "Perhaps you have changed geometry." << G4endl;
    G4cerr << "The gun will be place in the center." << G4endl;
  }

//--------------------------------------------------------------------------
    G4double size = 5*cm;
    G4double x0 = size* (G4UniformRand()-0.5);
    G4double y0 = size* (G4UniformRand()-0.5);
    G4double z0 = -0.5 * envSizeZ;
    //z0=4.7*cm;
    z0=100*cm;


  //fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0+180));
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

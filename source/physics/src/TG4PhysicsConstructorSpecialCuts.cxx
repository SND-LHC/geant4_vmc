// $Id: TG4PhysicsConstructorSpecialCuts.cxx,v 1.3 2004/11/10 11:39:28 brun Exp $
// Category: physics
//
// Class TG4PhysicsConstructorSpecialCuts
// --------------------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4PhysicsConstructorSpecialCuts.h"
#include "TG4G3PhysicsManager.h"
#include "TG4G3PhysicsManager.h"
#include "TG4ProcessMCMap.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4VProcess.hh>

#include <TMCProcess.h>


//_____________________________________________________________________________
TG4PhysicsConstructorSpecialCuts::TG4PhysicsConstructorSpecialCuts(
                                     const G4String& name)
  : TG4VPhysicsConstructor(name),
    fSpecialCutsForGamma("specialCutForGamma"),
    fSpecialCutsForElectron("specialCutForElectron"),
    fSpecialCutsForEplus("specialCutForEplus"),
    fSpecialCutsForChargedHadron("specialCutForChargedHadron"),
    fSpecialCutsForNeutralHadron("specialCutForNeutralHadron"),
    fSpecialCutsForMuon("specialCutForMuon"),
    fSpecialCutsForOther("specialCutForMuon")
{
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorSpecialCuts::TG4PhysicsConstructorSpecialCuts(
                                     G4int verboseLevel,
				     const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel),
    fSpecialCutsForGamma("specialCutForGamma"),
    fSpecialCutsForElectron("specialCutForElectron"),
    fSpecialCutsForEplus("specialCutForEplus"),
    fSpecialCutsForChargedHadron("specialCutForChargedHadron"),
    fSpecialCutsForNeutralHadron("specialCutForNeutralHadron"),
    fSpecialCutsForMuon("specialCutForMuon"),
    fSpecialCutsForOther("specialCutForMuon")
{
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorSpecialCuts::~TG4PhysicsConstructorSpecialCuts() {
//
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4PhysicsConstructorSpecialCuts::ConstructParticle()
{
/// The particles are constructed in the physics list.
}

//_____________________________________________________________________________
void TG4PhysicsConstructorSpecialCuts::ConstructProcess()
{
/// Add TG4SpecialCuts "process" that activates
/// the kinetic energy cuts defined in 
/// the vector of cuts (PhysicsManager::fCutVector) or in TG4Limits.

  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();

  theParticleIterator->reset();
  while ((*theParticleIterator)())
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    TG4G3ParticleWSP particleWSP 
      = g3PhysicsManager->GetG3ParticleWSP(particle);
    G4String name =
      g3PhysicsManager->GetG3ParticleWSPName(particleWSP);
      
    // uncomment this to see all particles "WSP"
    //G4cout << "Iterating particle: " 
    //       << particle->GetParticleName() << " " << particleWSP << " "
    //       << name << G4endl;

    if ( (particleWSP !=kNofParticlesWSP) ) {
      // special process is created in any case
   
      G4ProcessManager* pManager = particle->GetProcessManager();
      switch (particleWSP) {
        case kGamma:
          pManager->AddDiscreteProcess(&fSpecialCutsForGamma);
	      break;
        case kElectron:
          pManager->AddDiscreteProcess(&fSpecialCutsForElectron);
	      break;
        case kEplus:  
          pManager->AddDiscreteProcess(&fSpecialCutsForEplus);
	      break;
        case kChargedHadron:  
          pManager->AddDiscreteProcess(&fSpecialCutsForChargedHadron);
	      break;
        case kNeutralHadron:  
          pManager->AddDiscreteProcess(&fSpecialCutsForNeutralHadron);
	      break;
        case kMuon:  
          pManager->AddDiscreteProcess(&fSpecialCutsForMuon);
	      break;
        case kAny:
          pManager->AddDiscreteProcess(&fSpecialCutsForOther);
	      break;
        case kNofParticlesWSP:
	      // cannot happen
	      break;
      }
    }    
  }

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fSpecialCutsForGamma, kPStop);
  mcMap->Add(&fSpecialCutsForElectron, kPStop);
  mcMap->Add(&fSpecialCutsForEplus, kPStop);
  mcMap->Add(&fSpecialCutsForChargedHadron, kPStop);
  mcMap->Add(&fSpecialCutsForNeutralHadron, kPStop);
  mcMap->Add(&fSpecialCutsForMuon, kPStop);
  mcMap->Add(&fSpecialCutsForOther, kPStop);

  if (VerboseLevel() > 0) 
    G4cout << "### Special Cuts constructed. " << G4endl;
}



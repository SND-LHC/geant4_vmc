// $Id: $
//
// Classes TG4pecialCutsFor*
// --------------------------------------
// See the class descriptions in the header file.
//
// Author: I. Hrivnacova

#include "TG4SpecialCuts.h"
#include "TG4Limits.h"

//
//  Class TG4SpecialCutsForChargedHadron implementation
//

//_____________________________________________________________________________
TG4SpecialCutsForChargedHadron::TG4SpecialCutsForChargedHadron(
					         const G4String& processName)
  : TG4VSpecialCuts(processName) 
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForChargedHadron::TG4SpecialCutsForChargedHadron()
  : TG4VSpecialCuts()
{
/// Protected default constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForChargedHadron::~TG4SpecialCutsForChargedHadron() {
//
}

//_____________________________________________________________________________
G4double TG4SpecialCutsForChargedHadron::GetMinEkine(const TG4Limits& limits,
                                                     const G4Track& track) const
{					     
/// Return the min kinetic energy cut from limits.
// --- 

  return limits.GetMinEkineForChargedHadron(track);
}  

//
//  Class TG4SpecialCutsForElectron implementation
//

//_____________________________________________________________________________
TG4SpecialCutsForElectron::TG4SpecialCutsForElectron(const G4String& processName)
  : TG4VSpecialCuts(processName)
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForElectron::TG4SpecialCutsForElectron() 
  : TG4VSpecialCuts()
{
/// Protected default constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForElectron::~TG4SpecialCutsForElectron() {
//
}

//_____________________________________________________________________________
G4double TG4SpecialCutsForElectron::GetMinEkine(const TG4Limits& limits,
                                                const G4Track& track) const
{					     
/// Return the min kinetic energy cut from limits.

  return limits.GetMinEkineForElectron(track);
}  

//
//  Class TG4SpecialCutsForEplus implementation
//

//_____________________________________________________________________________
TG4SpecialCutsForEplus::TG4SpecialCutsForEplus(const G4String& processName)
  : TG4VSpecialCuts(processName) 
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForEplus::TG4SpecialCutsForEplus()
  : TG4VSpecialCuts()
{
/// Protected default constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForEplus::~TG4SpecialCutsForEplus() {
//
}

//_____________________________________________________________________________
G4double TG4SpecialCutsForEplus::GetMinEkine(const TG4Limits& limits,
                                             const G4Track& track) const
{					     
/// Return the min kinetic energy cut from limits.

  return limits.GetMinEkineForEplus(track);
}  

//
//  Class TG4SpecialCutsForGamma implementation
//

//_____________________________________________________________________________
TG4SpecialCutsForGamma::TG4SpecialCutsForGamma(const G4String& processName)
  : TG4VSpecialCuts(processName)
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForGamma::TG4SpecialCutsForGamma() 
  : TG4VSpecialCuts()
{
/// Protected default constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForGamma::~TG4SpecialCutsForGamma() {
//
}

//_____________________________________________________________________________
G4double TG4SpecialCutsForGamma::GetMinEkine(const TG4Limits& limits,
                                             const G4Track& track) const
{					     
/// Return the min kinetic energy cut from limits.

  return limits.GetMinEkineForGamma(track);
}  

//
//  Class TG4SpecialCutsForMuon implementation
//

//_____________________________________________________________________________
TG4SpecialCutsForMuon::TG4SpecialCutsForMuon(const G4String& processName)
  : TG4VSpecialCuts(processName) 
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForMuon::TG4SpecialCutsForMuon() 
  : TG4VSpecialCuts()
{
/// Protected default constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForMuon::~TG4SpecialCutsForMuon() {
//
}

//_____________________________________________________________________________
G4double TG4SpecialCutsForMuon::GetMinEkine(const TG4Limits& limits,
                                            const G4Track& track) const
{					     
/// Return the min kinetic energy cut from limits.

  return limits.GetMinEkineForMuon(track);
}  

//
//  Class TG4SpecialCutsForNeutralHadron implementation
//

//_____________________________________________________________________________
TG4SpecialCutsForNeutralHadron::TG4SpecialCutsForNeutralHadron(
                                              const G4String& processName)
  : TG4VSpecialCuts(processName)
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForNeutralHadron::TG4SpecialCutsForNeutralHadron()
  : TG4VSpecialCuts()
{
// Protected default constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForNeutralHadron::~TG4SpecialCutsForNeutralHadron() {
//
}

//_____________________________________________________________________________
G4double TG4SpecialCutsForNeutralHadron::GetMinEkine(const TG4Limits& limits,
                                                     const G4Track& track) const
{					     
/// Return the min kinetic energy cut from limits.

  return limits.GetMinEkineForNeutralHadron(track);
}  

//
//  Class TG4SpecialCutsForOther implementation
//

//_____________________________________________________________________________
TG4SpecialCutsForOther::TG4SpecialCutsForOther(const G4String& processName)
  : TG4VSpecialCuts(processName)
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForOther::TG4SpecialCutsForOther()
{
// Protected default constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForOther::~TG4SpecialCutsForOther() {
//
}

//_____________________________________________________________________________
G4double TG4SpecialCutsForOther::GetMinEkine(const TG4Limits& limits,
                                             const G4Track& track) const
{					     
/// Return the min kinetic energy cut from limits.

  return limits.GetMinEkineForOther(track);
}  

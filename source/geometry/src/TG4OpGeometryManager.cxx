// $Id: TG4OpGeometryManager.cxx,v 1.17 2006/04/12 10:37:23 brun Exp $
// Category: geometry
//
// Class TG4OpGeometryManager
// ------------------------
// See the class description in the header file.
//
// Author: V. Berejnoi, I. Hrivnacova

#include "TG4OpGeometryManager.h"
#include "TG4GeometryServices.h"
#include "TG4MediumMap.h"
#include "TG4Medium.h"
#include "TG4Limits.h"
#include "TG4G3Units.h"
#include "TG4G3CutVector.h"
#include "TG4G3ControlVector.h"
#include "TG4Globals.h"

#include <G3MatTable.hh>

#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4OpticalSurface.hh>
#include <G4ReflectedSolid.hh>
#include <G4Box.hh>
#include <G4Cons.hh>
#include <G4EllipticalTube.hh>
#include <G4Hype.hh>
#include <G4Para.hh>
#include <G4Polycone.hh>
#include <G4Polyhedra.hh>
#include <G4Sphere.hh>
#include <G4Torus.hh>
#include <G4Trap.hh>
#include <G4Trd.hh>
#include <G4Tubs.hh>
#include <G4TwistedTrap.hh>
#include <G4Transform3D.hh>

#include <TArrayD.h>
#include <TString.h>
#include <Riostream.h>

//_____________________________________________________________________________
TG4OpGeometryManager::TG4OpGeometryManager() 
  : TG4Verbose("geometryManager"),
    fGeometryServices(0),
    fOpSurfaceMap(0)
{
/// Standard constructor

  fGeometryServices = TG4GeometryServices::Instance();
  if ( !fGeometryServices ) {
    TG4Globals::Exception(
      "TG4OpGeometryManager", "TG4OpGeometryManager",
      "TG4GeometryServices have to be defined first"); 
  }
  fOpSurfaceMap = fGeometryServices->GetOpSurfaceMap();

}

//_____________________________________________________________________________
TG4OpGeometryManager::~TG4OpGeometryManager() 
{
/// Destructor

}

//
// public methods - TVirtualMC implementation
//


//_____________________________________________________________________________
void  TG4OpGeometryManager::SetCerenkov(Int_t itmed, Int_t npckov, 
                             Float_t* ppckov, Float_t* absco, Float_t* effic, 
                             Float_t* rindex)
{
///
///  Geant3 desription:                                                     \n
///  ==================
///
///    Stores the tables for UV photon tracking in medium ITMED 
///    Please note that it is the user's responsability to 
///    provide all the coefficients:
///
///
///     - ITMED       Tracking medium number
///     - NPCKOV      Number of bins of each table
///     - PPCKOV      Value of photon momentum (in GeV)
///     - ABSCO       Absorbtion coefficients 
///                   dielectric: absorbtion length in cm
///                   metals    : absorbtion fraction (0<=x<=1)
///     - EFFIC       Detection efficiency for UV photons 
///     - RINDEX      Refraction index (if=0 metal)

  G4double* ppckovDbl = fGeometryServices->CreateG4doubleArray(ppckov, npckov); 
  G4double* abscoDbl  = fGeometryServices->CreateG4doubleArray(absco, npckov); 
  G4double* efficDbl  = fGeometryServices->CreateG4doubleArray(effic, npckov); 
  G4double* rindexDbl = fGeometryServices->CreateG4doubleArray(rindex, npckov); 
  
  SetCerenkov(itmed, npckov, ppckovDbl, abscoDbl, efficDbl, rindexDbl);

  delete ppckovDbl;
  delete abscoDbl;
  delete efficDbl;
  delete rindexDbl;         
}                         

//_____________________________________________________________________________
void  TG4OpGeometryManager::SetCerenkov(Int_t itmed, Int_t npckov, 
                             Double_t* ppckov, Double_t* absco, Double_t* effic, 
                             Double_t* rindex)
{
///
///  Geant3 desription:                                                     \n
///  ==================
///
///    Stores the tables for UV photon tracking in medium ITMED 
///    Please note that it is the user's responsability to 
///    provide all the coefficients:
///
///
///     - ITMED       Tracking medium number
///     - NPCKOV      Number of bins of each table
///     - PPCKOV      Value of photon momentum (in GeV)
///     - ABSCO       Absorbtion coefficients 
///                   dielectric: absorbtion length in cm
///                   metals    : absorbtion fraction (0<=x<=1)
///     - EFFIC       Detection efficiency for UV photons 
///     - RINDEX      Refraction index (if=0 metal)

  // add units
  // add units
  G4double* ppckov2  = fGeometryServices->CreateG4doubleArray(ppckov, npckov); 
  G4double* absco2  = fGeometryServices->CreateG4doubleArray(absco, npckov); 
  G4int i;
  for (i=0; i<npckov; i++) {
    ppckov2[i] *= TG4G3Units::Energy();
    absco2[i]  *= TG4G3Units::Length();
  }  

  // get material of medium from table
  TG4Medium* medium 
    = fGeometryServices->GetMediumMap()->GetMedium(itmed);
  if (!medium) {
    TString text = "Medium ";
    text += itmed;
    text += " not found." ;
    TG4Globals::Exception(
      "TG4OpGeometryManager", "SetCerenkov", text); 
  }  
  G4Material* material = medium->GetMaterial();
  
  // get materials properties table or create it if it does not yet
  // exist
  G4MaterialPropertiesTable* table
    = material->GetMaterialPropertiesTable();
  if (!table) {
    table = new G4MaterialPropertiesTable(); 
    material->SetMaterialPropertiesTable(table);
  }  

  // add material properties into the table 
  table->AddProperty("ABSLENGTH", ppckov2, absco2, npckov);
                    // used in G4OpAbsorption process
  table->AddProperty("EFFICIENCY", ppckov2, effic, npckov);
                    // used in G4OpBoundary process
  table->AddProperty("RINDEX", ppckov2, rindex, npckov);
                    // used in G4Cerenkov, G4OpRayleigh, G4OpBoundary

  delete [] ppckov2;           
  delete [] absco2;           

  // verbose
  if (VerboseLevel() > 0) {
    G4cout << "The tables for UV photon tracking set for "
           << material->GetName() << G4endl;
  }           
}                         


//_____________________________________________________________________________
void TG4OpGeometryManager::DefineOpSurface(const char *name,
                               EMCOpSurfaceModel model,
                               EMCOpSurfaceType surfaceType,
                               EMCOpSurfaceFinish surfaceFinish,
                               Double_t sigmaAlpha)
{
/// Define the optical surface

  G4OpticalSurface* surface = new G4OpticalSurface(name);
  surface->SetModel(fGeometryServices->SurfaceModel(model));
  surface->SetType(fGeometryServices->SurfaceType(surfaceType));
  surface->SetFinish(fGeometryServices->SurfaceFinish(surfaceFinish));
  surface->SetSigmaAlpha(sigmaAlpha);
  
  // Store the surface in the map
  (*fOpSurfaceMap)[name] = surface;
}                               
                               
//_____________________________________________________________________________
void TG4OpGeometryManager::SetBorderSurface(const char *name,
                               const char* vol1Name, int vol1CopyNo,
                               const char* vol2Name, int vol2CopyNo,
                               const char* opSurfaceName)
{
/// Define the optical border surface

  // Get physical volumes
  G4VPhysicalVolume* pv1
    = fGeometryServices->FindPhysicalVolume( vol1Name, vol1CopyNo, false); 
  G4VPhysicalVolume* pv2
    = fGeometryServices->FindPhysicalVolume( vol2Name, vol2CopyNo, false); 

  if (! pv1 || ! pv2) {
    TString text;
    if ( ! pv1 ) {
      text = TString("Cannot find physical volume: ") + vol1Name;
      text += TG4Globals::Endl();
    }  
    if ( ! pv2 ) 
      text += TString("Cannot find physical volume: ") + vol2Name;
    TG4Globals::Exception("TG4OpGeometryManager", "SetBorderSurface", text);
  }  
  
  // Get the optical surface
  TG4OpSurfaceMap::iterator it = fOpSurfaceMap->find(opSurfaceName);
  if ( it == fOpSurfaceMap->end() ) {
    TG4Globals::Exception(
      "TG4OpGeometryManager", "SetBorderSurface",
      TString("Cannot find optical surface: ") + opSurfaceName);
    return;
  }  
  G4OpticalSurface* surface = (*it).second;
  
  // Create the border surface
  new G4LogicalBorderSurface(name, pv1, pv2, surface);
}
                               
//_____________________________________________________________________________
void TG4OpGeometryManager::SetSkinSurface(const char *name,
                         const char* volName,
                         const char* opSurfaceName)
{
/// Define the optical skin surface

  // Get logical volume
  G4LogicalVolume* lv
    = fGeometryServices->FindLogicalVolume(volName, false); 

  if ( !lv ) {
    TG4Globals::Exception(
      "TG4OpGeometryManager", "SetSkinSurface:",
      "Cannot find logical volume: " + TString(volName));
  }  
  
  // Get the optical surface
  TG4OpSurfaceMap::iterator it = fOpSurfaceMap->find(opSurfaceName);
  if (it == fOpSurfaceMap->end()) {
    TG4Globals::Exception(
      "TG4OpGeometryManager", "SetBorderSurface",
      "Cannot find optical surface: " + TString(opSurfaceName));
  }  
  G4OpticalSurface* surface = (*it).second;
 
  // Create the skin surface
  new G4LogicalSkinSurface(name, lv, surface);
}
                         
//_____________________________________________________________________________
void TG4OpGeometryManager::SetMaterialProperty(
                            Int_t itmed, const char* propertyName, 
                            Int_t np, Double_t* pp, Double_t* values)
{
/// Set the material property specified by propertyName to the tracking medium

  // create material properties table
  // get material of medium from table
  TG4Medium* medium 
    = fGeometryServices->GetMediumMap()->GetMedium(itmed);
  if (!medium) {
    TString text = "Medium ";
    text += itmed;
    text += "not found.";
    TG4Globals::Exception(
      "TG4OpGeometryManager", "SetMaterialProperty", text); 
  }  
  G4Material* material = medium->GetMaterial();
  
  // get materials properties table or create it if it does not yet
  // exist
  G4MaterialPropertiesTable* table
    = material->GetMaterialPropertiesTable();
  if (!table) {
    table = new G4MaterialPropertiesTable(); 
    material->SetMaterialPropertiesTable(table);
  }  

  // add units
  G4double* pp2  = fGeometryServices->CreateG4doubleArray(pp, np); 
  G4double* val2 = fGeometryServices->CreateG4doubleArray(values, np); 
  G4int i;
  for (i=0; i<np; i++) {
    pp2[i] = pp2[i]*TG4G3Units::Energy();
    val2[i]  = values[i];
    if (G4String(propertyName) == "ABSLENGTH") 
      val2[i]  = val2[i]*TG4G3Units::Length();
  }  
  table->AddProperty(propertyName, pp2, val2, np);

  delete [] pp2;           
  delete [] val2;           

  // verbose
  if (VerboseLevel() > 0) {
    G4cout << "The material properties " << propertyName << " set for "
           << material->GetName() << G4endl;
  }  
}
                         
//_____________________________________________________________________________
void TG4OpGeometryManager::SetMaterialProperty(
                            Int_t itmed, const char* propertyName, 
                            Double_t value)
{
/// Set the material property specified by propertyName to the tracking medium

  // create material properties table
  // get material of medium from table
  TG4Medium* medium 
    = fGeometryServices->GetMediumMap()->GetMedium(itmed);
  if (!medium) {
    TString text = "Medium ";
    text += itmed;
    text += "not found.";
    TG4Globals::Exception(
      "TG4OpGeometryManager", "SetMaterialProperty", text); 
  }  
  G4Material* material = medium->GetMaterial();
  
  // get materials properties table or create it if it does not yet
  // exist
  G4MaterialPropertiesTable* table
    = material->GetMaterialPropertiesTable();
  if (!table) {
    table = new G4MaterialPropertiesTable(); 
    material->SetMaterialPropertiesTable(table);
  }  

  // add units
  if ( G4String(propertyName) == "SCINTILLATIONYIELD") {
       value  = value/TG4G3Units::Energy();
  }  
  if ( G4String(propertyName) == "FASTTIMECONSTANT" || 
       G4String(propertyName) == "SLOWTIMECONSTANT" ) { 
       value  = value*TG4G3Units::Time();
  }  
  table->AddConstProperty(propertyName, value);

  // verbose
  if (VerboseLevel() > 0) {
    G4cout << "The material property " << propertyName << " set for "
           << material->GetName() << G4endl;
  }           
}
                         
//_____________________________________________________________________________
void TG4OpGeometryManager::SetMaterialProperty(
                            const char* surfaceName, const char* propertyName, 
                            Int_t np, Double_t* pp, Double_t* values)
{
/// Set the material property specified by propertyName to the optical surface

  // create material properties table
  // get optical surface from the map
  // Get the optical surface
  TG4OpSurfaceMap::iterator it = fOpSurfaceMap->find(surfaceName);
  if (it == fOpSurfaceMap->end()) {
    TG4Globals::Exception(
      "TG4OpGeometryManager", "SetMaterialProperty",
      "Cannot find optical surface: " + TString(surfaceName));
    return;
  }  
  G4OpticalSurface* surface = (*it).second;
 
  // get materials properties table or create it if it does not yet
  // exist
  G4MaterialPropertiesTable* table
    = surface->GetMaterialPropertiesTable();
  if (!table) {
    table = new G4MaterialPropertiesTable(); 
    surface->SetMaterialPropertiesTable(table);
  }  

  // add units
  G4double* pp2  = fGeometryServices->CreateG4doubleArray(pp, np); 
  G4int i;
  for (i=0; i<np; i++) {
    pp2[i] = pp2[i]*TG4G3Units::Energy();
  }  
  table->AddProperty(propertyName, pp2, values, np);
 
  delete [] pp2;  

  // verbose
  if (VerboseLevel() > 0) {
    G4cout << "The material properties " << propertyName 
           << " set for optical surface "
           << surface->GetName() << G4endl;
  }           
}                             
 
//_____________________________________________________________________________
void TG4OpGeometryManager::Gfmate(Int_t imat, char *name, Float_t &a, 
          Float_t &z, Float_t &dens, Float_t &radl, Float_t &absl,
          Float_t* /*ubuf*/, Int_t& nbuf) 
{ 
/// Return parameters for material specified by material number imat 

  G4double da, dz, ddens, dradl, dabsl;  
  Gfmate(imat, name, da, dz, ddens, dradl, dabsl, 0, nbuf);
  
  a = da;
  z = dz;
  dens = ddens;
  radl = dradl;
  absl = dabsl; 
} 

 
//_____________________________________________________________________________
void TG4OpGeometryManager::Gfmate(Int_t imat, char *name, Double_t &a, 
          Double_t &z, Double_t &dens, Double_t &radl, Double_t &absl,
          Double_t* ubuf, Int_t& nbuf) 
{ 
///  Return parameters for material specified by material number imat 


  // TO DO: change to use G4Material instead
  G4Material* material = G3Mat.get(imat);
  
  if (material) {
    // to do: change this correctly 
    // !! unsafe conversion
    const char* chName = material->GetName();
    name = (char*)chName;
    a = fGeometryServices->GetEffA(material);
    z = fGeometryServices->GetEffZ(material);
    
    dens = material->GetDensity();
    dens /= TG4G3Units::MassDensity();

    radl = material->GetRadlen();
    radl /= TG4G3Units::Length();

    // the following parameters are not defined in Geant4
    absl = 0.; 
    ubuf = 0;
    nbuf = 0;
  }
  else {
    TString text = "Material ";
    text += imat;
    text += "has not been found.";
    TG4Globals::Exception("TG4OpGeometryManager", "Gfmate", text);
  }
} 

//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: ExN03DetectorConstruction.hh,v 1.6 2003/09/15 15:38:14 maire Exp $
// GEANT4 tag $Name: geant4-07-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ExN03DetectorConstruction_h
#define ExN03DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include <vector>

class G4Box;
class G4Sphere;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UniformMagField;
class ExN03DetectorMessenger;

class G4VIStore;
class G4IStore;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN03DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    ExN03DetectorConstruction();
   ~ExN03DetectorConstruction();

  public:
     
     void SetAbsorberMaterial (G4String);     
     void SetAbsorberThickness(G4double);     

     void SetGapMaterial (G4String);     
     void SetGapThickness(G4double);
     
     void SetCalorSizeYZ(G4double);          
     void SetNbOfLayers (G4int);   
      
     void SetMagField(G4double);
     
     G4VPhysicalVolume* Construct();

     void UpdateGeometry();
     
  public:
  
     void PrintCalorParameters(); 
                    
     G4double GetWorldSizeX()           {return WorldSizeX;}; 
     G4double GetWorldSizeYZ()          {return WorldSizeYZ;};
     
     G4double GetCalorThickness()       {return CalorThickness;}; 
     G4double GetCalorSizeYZ()          {return CalorSizeYZ;};
      
     G4int GetNbOfLayers()              {return NbOfLayers;}; 
     G4double GetLayerThickness()       {return LayerThickness;}; 
     
     G4Material* GetAbsorberMaterial()  {return AbsorberMaterial;};
     G4double    GetAbsorberThickness() {return AbsorberThickness;};      
     
     G4Material* GetGapMaterial()       {return GapMaterial;};
     G4double    GetGapThickness()      {return GapThickness;};
     
     G4VPhysicalVolume* GetphysWorld() {return (G4VPhysicalVolume*) physiWorld;};           
     const G4VPhysicalVolume* GetAbsorber()   {return physiAbsorber;};
     const G4VPhysicalVolume* GetGap()        {return physiGap;};

  private:

     G4Material*        targetMaterial;
     
     G4Material*        AbsorberMaterial;
     G4double           AbsorberThickness;
     
     G4Material*        GapMaterial;
     G4double           GapThickness;
     
     G4int              NbOfLayers;
     G4double           LayerThickness;
          
     G4double           CalorSizeYZ;
     G4double           CalorThickness;
     
     G4Material*        defaultMaterial;
     G4double           WorldSizeYZ;
     G4double           WorldSizeX;
            
     G4Box*             solidWorld;    //pointer to the solid World 
     G4LogicalVolume*   logicWorld;    //pointer to the logical World
     G4VPhysicalVolume* physiWorld;    //pointer to the physical World

     G4Box*             solidCalor;    //pointer to the solid Calor 
     G4LogicalVolume*   logicCalor;    //pointer to the logical Calor
     G4VPhysicalVolume* physiCalor;    //pointer to the physical Calor
     
     G4Box*             solidLayer;    //pointer to the solid Layer 
     G4LogicalVolume*   logicLayer;    //pointer to the logical Layer
     G4VPhysicalVolume* physiLayer;    //pointer to the physical Layer
         
     G4Box*             solidAbsorber; //pointer to the solid Absorber
     G4LogicalVolume*   logicAbsorber; //pointer to the logical Absorber
     G4VPhysicalVolume* physiAbsorber; //pointer to the physical Absorber
     
     G4Box*             solidGap;      //pointer to the solid Gap
     G4LogicalVolume*   logicGap;      //pointer to the logical Gap
     G4VPhysicalVolume* physiGap;      //pointer to the physical Gap
     
     G4UniformMagField* magField;      //pointer to the magnetic field
     
     ExN03DetectorMessenger* detectorMessenger;  //pointer to the Messenger
      
  private:
    
     void DefineMaterials();
     void ComputeCalorParameters();
     G4VPhysicalVolume* ConstructCalorimeter();     


// SATIF
   public:
      //G4VIStore* CreateImportanceStore();
      //void UseBiassing() { useBiassing = TRUE; }; 
      G4bool UseBiassing() { return useBiassing; };

      const G4VPhysicalVolume* GetphysiWorld() {return physiWorld;};           
      const G4VPhysicalVolume* GetphysiCalor() {return physiCalor;};           
      const G4VPhysicalVolume* GetphysiLayer() {return physiLayer;};           

      G4LogicalVolume* GetlogiLayer() {return logicLayer;};           

   private:
      G4bool useBiassing; 
      std::vector< G4VPhysicalVolume* > vPhysicalVolume;
      G4IStore* istore;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void ExN03DetectorConstruction::ComputeCalorParameters()
{
  // Compute derived parameters of the calorimeter
     LayerThickness = AbsorberThickness + GapThickness;
     CalorThickness = NbOfLayers*LayerThickness;
     
     WorldSizeX = 1.2*CalorThickness; WorldSizeYZ = 1.2*CalorSizeYZ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


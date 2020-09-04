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
// $Id: ExN03PrimaryGeneratorAction.hh,v 1.6 2003/09/15 15:38:14 maire Exp $
// GEANT4 tag $Name: geant4-07-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ExN03PrimaryGeneratorAction_h
#define ExN03PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include <vector>

class G4ParticleGun;
class G4Event;
class ExN03DetectorConstruction;
class ExN03PrimaryGeneratorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN03PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    ExN03PrimaryGeneratorAction(ExN03DetectorConstruction*);    
   ~ExN03PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);
    void SetRndmFlag(G4String val) { rndmFlag = val;}

  private:
    G4ParticleGun*                particleGun;	  //pointer a to G4  class
    ExN03DetectorConstruction*    ExN03Detector;  //pointer to the geometry
    
    ExN03PrimaryGeneratorMessenger* gunMessenger; //messenger of this class
    G4String                      rndmFlag;	  //flag for a rndm impact point

   public:
      void SetGunType( G4String type ) { gun_type = type; }; 

   private:
      G4String gun_type;
      G4double energy_from_a();
      G4double energy_from_b();
      G4double energy_from_c();
      G4double energy_from_d();

      G4double energy_from_200MeVpFe90();
      G4double energy_from_500MeVpFe90();
      G4double energy_from_1GeVpFe90();
      G4double energy_from_3GeVpFe90();
      G4double energy_from_5GeVpFe90();

      G4double energy_from_3GeVpHgM000to015();
      G4double energy_from_3GeVpHgM045to060();
      G4double energy_from_3GeVpHgM090to105();
      G4double energy_from_3GeVpHgM135to150();
      G4double energy_from_24GeVpHgM090to105();

      std::vector < G4double > e_bin; 
      std::vector < G4double > e_FeT_bin; 
      std::vector < G4double > v_d200MeVpFe90; 
      std::vector < G4double > v_p200MeVpFe90; 

      std::vector < G4double > v_d500MeVpFe90; 
      std::vector < G4double > v_p500MeVpFe90; 

      std::vector < G4double > v_d1GeVpFe90; 
      std::vector < G4double > v_p1GeVpFe90; 

      std::vector < G4double > v_d3GeVpFe90; 
      std::vector < G4double > v_p3GeVpFe90; 

      std::vector < G4double > v_d5GeVpFe90; 
      std::vector < G4double > v_p5GeVpFe90; 

      std::vector < G4double > v_d3GeVpHgM000to015; 
      std::vector < G4double > v_p3GeVpHgM000to015; 

      std::vector < G4double > v_d3GeVpHgM045to060; 
      std::vector < G4double > v_p3GeVpHgM045to060; 

      std::vector < G4double > v_d3GeVpHgM090to105; 
      std::vector < G4double > v_p3GeVpHgM090to105; 

      std::vector < G4double > v_d3GeVpHgM135to150; 
      std::vector < G4double > v_p3GeVpHgM135to150; 

      std::vector < G4double > v_d24GeVpHgM090to105; 
      std::vector < G4double > v_p24GeVpHgM090to105; 

      void create_energy_dist_alleys();
      void create_p_alley( std::vector < G4double >* , std::vector < G4double >* );
      void create_FeT_p_alley( std::vector < G4double >* , std::vector < G4double >* );
      G4int search_bin_no ( std::vector < G4double >* , G4double );
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif



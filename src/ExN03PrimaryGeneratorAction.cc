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
// $Id: ExN03PrimaryGeneratorAction.cc,v 1.7 2003/09/15 15:38:18 maire Exp $
// GEANT4 tag $Name: geant4-07-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN03PrimaryGeneratorAction.hh"

#include "ExN03DetectorConstruction.hh"
#include "ExN03PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN03PrimaryGeneratorAction::ExN03PrimaryGeneratorAction(
                                             ExN03DetectorConstruction* ExN03DC)
:ExN03Detector(ExN03DC),rndmFlag("off")
{
  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  
  //create a messenger for this class
  gunMessenger = new ExN03PrimaryGeneratorMessenger(this);

  // default particle kinematic

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
                    = particleTable->FindParticle(particleName="e-");
  particleGun->SetParticleDefinition(particle);
  //particleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  particleGun->SetParticleEnergy(50.*MeV);
  //G4double position = -0.5*(ExN03Detector->GetWorldSizeX());
  //particleGun->SetParticlePosition(G4ThreeVector(position,0.*cm,0.*cm));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->SetParticlePosition(G4ThreeVector(0*cm,0.*cm,0.*cm));

  gun_type = "default";

  create_energy_dist_alleys();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN03PrimaryGeneratorAction::~ExN03PrimaryGeneratorAction()
{
  delete particleGun;
  delete gunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN03PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  // 
  //G4double x0 = -0.5*(ExN03Detector->GetWorldSizeX());
/*
  G4double y0 = 0.*cm, z0 = 0.*cm;
  if (rndmFlag == "on")
  {   
      y0 = (ExN03Detector->GetCalorSizeYZ())*(G4UniformRand()-0.5);
      z0 = (ExN03Detector->GetCalorSizeYZ())*(G4UniformRand()-0.5);
  } 
*/
  //particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  //particleGun->SetParticlePosition(G4ThreeVector(0*cm,0.*cm,0.*cm));

   G4double sigmaX = 3.98*cm/2.354826;
   G4double sigmaY = 1.90*cm/2.354826;
   G4double meanX = 1.1*cm;
   G4double meanY = -1.2*cm;
   if ( particleGun->GetParticleEnergy() > 23*GeV ) // 24GeV
   {
      sigmaX = 2.68*cm/2.354826;
      sigmaY = 2.79*cm/2.354826;
      meanX = 0.6*cm;
      meanY = -1.0*cm;
   }
   G4double xp = G4RandGauss::shoot( meanX, sigmaX );
   G4double yp = G4RandGauss::shoot( meanY, sigmaY );

  //particleGun->SetParticlePosition(G4ThreeVector(0*cm,0.*cm,-1.*cm));
  particleGun->SetParticlePosition(G4ThreeVector(xp,yp,-1.*cm));
  if ( gun_type != "default" )
  {
     if ( gun_type == "a" )
     {
        particleGun->SetParticleEnergy( energy_from_a() );
     }
     else if ( gun_type == "b" )
     {
        particleGun->SetParticleEnergy( energy_from_b() );
     } 
     else if ( gun_type == "c" )
     {
        particleGun->SetParticleEnergy( energy_from_c() );
     } 
     else if ( gun_type == "d" )
     {
        particleGun->SetParticleEnergy( energy_from_d() );
     } 
     else if ( gun_type == "200MeVpFe90" )
     {
        particleGun->SetParticleEnergy( energy_from_200MeVpFe90() );
     } 
     else if ( gun_type == "500MeVpFe90" )
     {
        particleGun->SetParticleEnergy( energy_from_500MeVpFe90() );
     } 
     else if ( gun_type == "1GeVpFe90" )
     {
        //particleGun->SetParticleEnergy( energy_from_1GeVpFe90() );
        G4double e = energy_from_1GeVpFe90();
        //std::cout << "SHOKOSHOKO " << e / MeV << std::endl;
        particleGun->SetParticleEnergy( e );
     } 
     else if ( gun_type == "3GeVpFe90" )
     {
        particleGun->SetParticleEnergy( energy_from_3GeVpFe90() );
     } 
     else if ( gun_type == "5GeVpFe90" )
     {
        particleGun->SetParticleEnergy( energy_from_5GeVpFe90() );
     } 
     else if ( gun_type == "3GeVpHgM000to015" )
     {
        particleGun->SetParticleEnergy( energy_from_3GeVpHgM000to015() );
     } 
     else if ( gun_type == "3GeVpHgM045to060" )
     {
        particleGun->SetParticleEnergy( energy_from_3GeVpHgM045to060() );
     } 
     else if ( gun_type == "3GeVpHgM090to105" )
     {
        particleGun->SetParticleEnergy( energy_from_3GeVpHgM090to105() );
     } 
     else if ( gun_type == "3GeVpHgM135to150" )
     {
        particleGun->SetParticleEnergy( energy_from_3GeVpHgM135to150() );
     } 
     else if ( gun_type == "24GeVpHgM090to105" )
     {
        //particleGun->SetParticleEnergy( energy_from_24GeVpHgM090to105() );
        G4double e = energy_from_24GeVpHgM090to105();
        //std::cout << "SHOKOSHOKO " << e / MeV << std::endl;
        particleGun->SetParticleEnergy( e );
     } 
  }
  particleGun->GeneratePrimaryVertex(anEvent);
}



G4double ExN03PrimaryGeneratorAction::energy_from_a()
{
   return ( 50*MeV - 45*MeV ) * G4UniformRand() + 45*MeV;
}
G4double ExN03PrimaryGeneratorAction::energy_from_b()
{
   return ( 100*MeV - 90*MeV ) * G4UniformRand() + 90*MeV;
}
G4double ExN03PrimaryGeneratorAction::energy_from_c()
{
   return ( 200*MeV - 180*MeV ) * G4UniformRand() + 180*MeV;
}
G4double ExN03PrimaryGeneratorAction::energy_from_d()
{
   return ( 400*MeV - 375*MeV ) * G4UniformRand() + 375*MeV;
}

G4double ExN03PrimaryGeneratorAction::energy_from_200MeVpFe90()
{
   G4int i = search_bin_no ( &v_p200MeVpFe90 , G4UniformRand() );
   return ( e_bin [ i ] - e_bin [ i-1 ] ) * G4UniformRand() + e_bin [ i-1 ];
}

G4double ExN03PrimaryGeneratorAction::energy_from_500MeVpFe90()
{
   G4int i = search_bin_no ( &v_p500MeVpFe90 , G4UniformRand() );
   return ( e_bin [ i ] - e_bin [ i-1 ] ) * G4UniformRand() + e_bin [ i-1 ];
}

G4double ExN03PrimaryGeneratorAction::energy_from_1GeVpFe90()
{
   G4int i = search_bin_no ( &v_p1GeVpFe90 , G4UniformRand() );
   //return ( e_FeT_bin [ i + 1 ] - e_FeT_bin [ i ] ) * G4UniformRand() + e_FeT_bin [ i ];
   return ( e_FeT_bin [ i ] - e_FeT_bin [ i-1 ] ) * G4UniformRand() + e_FeT_bin [ i-1 ];
}

G4double ExN03PrimaryGeneratorAction::energy_from_3GeVpFe90()
{
   G4int i = search_bin_no ( &v_p3GeVpFe90 , G4UniformRand() );
   return ( e_bin [ i ] - e_bin [ i-1 ] ) * G4UniformRand() + e_bin [ i-1 ];
}

G4double ExN03PrimaryGeneratorAction::energy_from_5GeVpFe90()
{
   G4int i = search_bin_no ( &v_p5GeVpFe90 , G4UniformRand() );
   return ( e_bin [ i ] - e_bin [ i-1 ] ) * G4UniformRand() + e_bin [ i-1 ];
}

G4double ExN03PrimaryGeneratorAction::energy_from_3GeVpHgM000to015()
{
   G4int i = search_bin_no ( &v_p3GeVpHgM000to015 , G4UniformRand() );
   return ( e_bin [ i ] - e_bin [ i-1 ] ) * G4UniformRand() + e_bin [ i-1 ];
}

G4double ExN03PrimaryGeneratorAction::energy_from_3GeVpHgM045to060()
{
   G4int i = search_bin_no ( &v_p3GeVpHgM045to060 , G4UniformRand() );
   return ( e_bin [ i ] - e_bin [ i-1 ] ) * G4UniformRand() + e_bin [ i-1 ];
}

G4double ExN03PrimaryGeneratorAction::energy_from_3GeVpHgM090to105()
{
   G4int i = search_bin_no ( &v_p3GeVpHgM090to105 , G4UniformRand() );
   return ( e_bin [ i ] - e_bin [ i-1 ] ) * G4UniformRand() + e_bin [ i-1 ];
}

G4double ExN03PrimaryGeneratorAction::energy_from_3GeVpHgM135to150()
{
   G4int i = search_bin_no ( &v_p3GeVpHgM135to150 , G4UniformRand() );
   return ( e_bin [ i ] - e_bin [ i-1 ] ) * G4UniformRand() + e_bin [ i-1 ];
}

G4double ExN03PrimaryGeneratorAction::energy_from_24GeVpHgM090to105()
{
   G4int i = search_bin_no ( &v_p24GeVpHgM090to105 , G4UniformRand() );
   return ( e_bin [ i ] - e_bin [ i-1 ] ) * G4UniformRand() + e_bin [ i-1 ];
}

void ExN03PrimaryGeneratorAction::create_energy_dist_alleys()
{

   e_bin.resize( 44 );
   e_bin [  0 ] =   19.95;
   e_bin [  1 ] =   22.39;
   e_bin [  2 ] =   25.12;
   e_bin [  3 ] =   28.18;
   e_bin [  4 ] =   31.62;
   e_bin [  5 ] =   35.48;
   e_bin [  6 ] =   39.81;
   e_bin [  7 ] =   44.67;
   e_bin [  8 ] =   50.12;
   e_bin [  9 ] =   56.23;
   e_bin [ 10 ] =   63.1;
   e_bin [ 11 ] =   70.79;
   e_bin [ 12 ] =   79.4;
   e_bin [ 13 ] =   89.1;
   e_bin [ 14 ] =  100;
   e_bin [ 15 ] =  112;
   e_bin [ 16 ] =  126;
   e_bin [ 17 ] =  141;
   e_bin [ 18 ] =  159;
   e_bin [ 19 ] =  178;
   e_bin [ 20 ] =  200;
   e_bin [ 21 ] =  224;
   e_bin [ 22 ] =  251;
   e_bin [ 23 ] =  282;
   e_bin [ 24 ] =  316;
   e_bin [ 25 ] =  355;
   e_bin [ 26 ] =  398;
   e_bin [ 27 ] =  447;
   e_bin [ 28 ] =  501;
   e_bin [ 29 ] =  562;
   e_bin [ 30 ] =  631;
   e_bin [ 31 ] =  708;
   e_bin [ 32 ] =  794;
   e_bin [ 33 ] =  891;
   e_bin [ 34 ] = 1000;
   e_bin [ 35 ] = 1120;
   e_bin [ 36 ] = 1260;
   e_bin [ 37 ] = 1410;
   e_bin [ 38 ] = 1590;
   e_bin [ 39 ] = 1780;
   e_bin [ 40 ] = 2000;
   e_bin [ 41 ] = 2240;
   e_bin [ 42 ] = 2510;
   e_bin [ 43 ] = 2820;

   e_FeT_bin.resize( 38 );
   e_FeT_bin [  0 ] =    19.6;
   e_FeT_bin [  1 ] =    22.5;
   e_FeT_bin [  2 ] =    25.0;
   e_FeT_bin [  3 ] =    27.5;
   e_FeT_bin [  4 ] =    30.0;
   e_FeT_bin [  5 ] =    35.0;
   e_FeT_bin [  6 ] =    40.0;
   e_FeT_bin [  7 ] =    45.0;
   e_FeT_bin [  8 ] =    50.0;
   e_FeT_bin [  9 ] =    55.0;
   e_FeT_bin [ 10 ] =    60.0;
   e_FeT_bin [ 11 ] =    65.0;
   e_FeT_bin [ 12 ] =    70.0;
   e_FeT_bin [ 13 ] =    80.0;
   e_FeT_bin [ 14 ] =    90.0;
   e_FeT_bin [ 15 ] =   100. ;
   e_FeT_bin [ 16 ] =   110. ;
   e_FeT_bin [ 17 ] =   120. ;
   e_FeT_bin [ 18 ] =   140. ;
   e_FeT_bin [ 19 ] =   160. ;
   e_FeT_bin [ 20 ] =   180. ;
   e_FeT_bin [ 21 ] =   200. ;
   e_FeT_bin [ 22 ] =   225. ;
   e_FeT_bin [ 23 ] =   250. ;
   e_FeT_bin [ 24 ] =   275. ;
   e_FeT_bin [ 25 ] =   300. ;
   e_FeT_bin [ 26 ] =   325. ;
   e_FeT_bin [ 27 ] =   350. ;
   e_FeT_bin [ 28 ] =   375. ;
   e_FeT_bin [ 29 ] =   400. ;
   e_FeT_bin [ 30 ] =   450. ;
   e_FeT_bin [ 31 ] =   500. ;
   e_FeT_bin [ 32 ] =   550. ;
   e_FeT_bin [ 33 ] =   600. ;
   e_FeT_bin [ 34 ] =   650. ;
   e_FeT_bin [ 35 ] =   700. ;
   e_FeT_bin [ 36 ] =   750. ;
   e_FeT_bin [ 37 ] =   800. ;

   v_d200MeVpFe90.resize(21);
   v_d200MeVpFe90 [  0 ] = 0;
   v_d200MeVpFe90 [  1 ] = 0.115;
   v_d200MeVpFe90 [  2 ] = 0.0262;
   v_d200MeVpFe90 [  3 ] = 0.0258;
   v_d200MeVpFe90 [  4 ] = 0.0226;
   v_d200MeVpFe90 [  5 ] = 0.0199;
   v_d200MeVpFe90 [  6 ] = 0.0161;
   v_d200MeVpFe90 [  7 ] = 0.0122;
   v_d200MeVpFe90 [  8 ] = 0.0104;
   v_d200MeVpFe90 [  9 ] = 0.00795;
   v_d200MeVpFe90 [ 10 ] = 0.00573;
   v_d200MeVpFe90 [ 11 ] = 0.00459;
   v_d200MeVpFe90 [ 12 ] = 0.0037;
   v_d200MeVpFe90 [ 13 ] = 0.00302;
   v_d200MeVpFe90 [ 14 ] = 0.0019;
   v_d200MeVpFe90 [ 15 ] = 0.00111;
   v_d200MeVpFe90 [ 16 ] = 0.000605;
   v_d200MeVpFe90 [ 17 ] = 0.000488;
   v_d200MeVpFe90 [ 18 ] = 0.000207;
   v_d200MeVpFe90 [ 19 ] = 0.0000617;
   v_d200MeVpFe90 [ 20 ] = 0.0000117;

   //create_p_alley ( &v_d200MeVpFe90 , &v_p200MeVpFe90 );
   create_FeT_p_alley ( &v_d200MeVpFe90 , &v_p200MeVpFe90 );

   v_d500MeVpFe90.resize(28);
   v_d500MeVpFe90 [ 0 ] = 0;
   v_d500MeVpFe90 [ 1 ] = 0.026;
   v_d500MeVpFe90 [ 2 ] = 0.0263;
   v_d500MeVpFe90 [ 3 ] = 0.0238;
   v_d500MeVpFe90 [ 4 ] = 0.0219;
   v_d500MeVpFe90 [ 5 ] = 0.019;
   v_d500MeVpFe90 [ 6 ] = 0.0161;
   v_d500MeVpFe90 [ 7 ] = 0.0144;
   v_d500MeVpFe90 [ 8 ] = 0.0129;
   v_d500MeVpFe90 [ 9 ] = 0.0112;
   v_d500MeVpFe90 [ 10 ] = 0.00916;
   v_d500MeVpFe90 [ 11 ] = 0.00843;
   v_d500MeVpFe90 [ 12 ] = 0.00732;
   v_d500MeVpFe90 [ 13 ] = 0.00591;
   v_d500MeVpFe90 [ 14 ] = 0.00449;
   v_d500MeVpFe90 [ 15 ] = 0.0036;
   v_d500MeVpFe90 [ 16 ] = 0.00267;
   v_d500MeVpFe90 [ 17 ] = 0.00203;
   v_d500MeVpFe90 [ 18 ] = 0.00142;
   v_d500MeVpFe90 [ 19 ] = 0.000834;
   v_d500MeVpFe90 [ 20 ] = 0.000471;
   v_d500MeVpFe90 [ 21 ] = 0.000251;
   v_d500MeVpFe90 [ 22 ] = 0.000134;
   v_d500MeVpFe90 [ 23 ] = 0.0000496;
   v_d500MeVpFe90 [ 24 ] = 0.0000205;
   v_d500MeVpFe90 [ 25 ] = 0.00000325;
   v_d500MeVpFe90 [ 26 ] = 0.00000286;
   v_d500MeVpFe90 [ 27 ] = 0.000000642;

   //create_p_alley ( &v_d500MeVpFe90 , &v_p500MeVpFe90 );
   create_FeT_p_alley ( &v_d500MeVpFe90 , &v_p500MeVpFe90 );

   v_d1GeVpFe90.resize( 31 );
   v_d1GeVpFe90 [ 0 ] = 0;
   v_d1GeVpFe90 [ 1 ] = 0.0216;
   v_d1GeVpFe90 [ 2 ] = 0.022;
   v_d1GeVpFe90 [ 3 ] = 0.0207;
   v_d1GeVpFe90 [ 4 ] = 0.0189;
   v_d1GeVpFe90 [ 5 ] = 0.016;
   v_d1GeVpFe90 [ 6 ] = 0.014;
   v_d1GeVpFe90 [ 7 ] = 0.0135;
   v_d1GeVpFe90 [ 8 ] = 0.0115;
   v_d1GeVpFe90 [ 9 ] = 0.00976;
   v_d1GeVpFe90 [ 10 ] = 0.00895;
   v_d1GeVpFe90 [ 11 ] = 0.00832;
   v_d1GeVpFe90 [ 12 ] = 0.00711;
   v_d1GeVpFe90 [ 13 ] = 0.00602;
   v_d1GeVpFe90 [ 14 ] = 0.00522;
   v_d1GeVpFe90 [ 15 ] = 0.00418;
   v_d1GeVpFe90 [ 16 ] = 0.00314;
   v_d1GeVpFe90 [ 17 ] = 0.0027;
   v_d1GeVpFe90 [ 18 ] = 0.00212;
   v_d1GeVpFe90 [ 19 ] = 0.00151;
   v_d1GeVpFe90 [ 20 ] = 0.000925;
   v_d1GeVpFe90 [ 21 ] = 0.000628;
   v_d1GeVpFe90 [ 22 ] = 0.000387;
   v_d1GeVpFe90 [ 23 ] = 0.000235;
   v_d1GeVpFe90 [ 24 ] = 0.000106;
   v_d1GeVpFe90 [ 25 ] = 0.0000568;
   v_d1GeVpFe90 [ 26 ] = 0.0000253;
   v_d1GeVpFe90 [ 27 ] = 0.0000126;
   v_d1GeVpFe90 [ 28 ] = 0.00000467;
   v_d1GeVpFe90 [ 29 ] = 0.0000051;
   v_d1GeVpFe90 [ 30 ] = 0.00000123;

   //create_p_alley ( &v_d1GeVpFe90 , &v_p1GeVpFe90 );
   create_FeT_p_alley ( &v_d1GeVpFe90 , &v_p1GeVpFe90 );

   v_d3GeVpFe90.resize( 38 );
   v_d3GeVpFe90 [ 0 ] = 0;
   v_d3GeVpFe90 [ 1 ] = 0.0258;
   v_d3GeVpFe90 [ 2 ] = 0.0219;
   v_d3GeVpFe90 [ 3 ] = 0.0198;
   v_d3GeVpFe90 [ 4 ] = 0.0189;
   v_d3GeVpFe90 [ 5 ] = 0.0157;
   v_d3GeVpFe90 [ 6 ] = 0.0146;
   v_d3GeVpFe90 [ 7 ] = 0.0129;
   v_d3GeVpFe90 [ 8 ] = 0.0113;
   v_d3GeVpFe90 [ 9 ] = 0.0101;
   v_d3GeVpFe90 [ 10 ] = 0.00941;
   v_d3GeVpFe90 [ 11 ] = 0.00812;
   v_d3GeVpFe90 [ 12 ] = 0.00748;
   v_d3GeVpFe90 [ 13 ] = 0.0062;
   v_d3GeVpFe90 [ 14 ] = 0.00508;
   v_d3GeVpFe90 [ 15 ] = 0.00403;
   v_d3GeVpFe90 [ 16 ] = 0.00332;
   v_d3GeVpFe90 [ 17 ] = 0.00263;
   v_d3GeVpFe90 [ 18 ] = 0.00196;
   v_d3GeVpFe90 [ 19 ] = 0.00121;
   v_d3GeVpFe90 [ 20 ] = 0.000836;
   v_d3GeVpFe90 [ 21 ] = 0.000531;
   v_d3GeVpFe90 [ 22 ] = 0.000336;
   v_d3GeVpFe90 [ 23 ] = 0.000223;
   v_d3GeVpFe90 [ 24 ] = 0.000127;
   v_d3GeVpFe90 [ 25 ] = 0.0000699;
   v_d3GeVpFe90 [ 26 ] = 0.0000512;
   v_d3GeVpFe90 [ 27 ] = 0.000038;
   v_d3GeVpFe90 [ 28 ] = 0.0000181;
   v_d3GeVpFe90 [ 29 ] = 0.0000113;
   v_d3GeVpFe90 [ 30 ] = 0.00000489;
   v_d3GeVpFe90 [ 31 ] = 0.000000675;
   v_d3GeVpFe90 [ 32 ] = 0.00000116;
   v_d3GeVpFe90 [ 33 ] = 0;
   v_d3GeVpFe90 [ 34 ] = 0;
   v_d3GeVpFe90 [ 35 ] = 0.000000577;
   v_d3GeVpFe90 [ 36 ] = 0;
   v_d3GeVpFe90 [ 37 ] = 0;

   //create_p_alley ( &v_d3GeVpFe90 , &v_p3GeVpFe90 );
   create_FeT_p_alley ( &v_d3GeVpFe90 , &v_p3GeVpFe90 );

   v_d5GeVpFe90.resize( 38 );
   v_d5GeVpFe90 [ 0 ] = 0;
   v_d5GeVpFe90 [ 1 ] = 0.0207;
   v_d5GeVpFe90 [ 2 ] = 0.0176;
   v_d5GeVpFe90 [ 3 ] = 0.0159;
   v_d5GeVpFe90 [ 4 ] = 0.0144;
   v_d5GeVpFe90 [ 5 ] = 0.013;
   v_d5GeVpFe90 [ 6 ] = 0.0112;
   v_d5GeVpFe90 [ 7 ] = 0.0102;
   v_d5GeVpFe90 [ 8 ] = 0.00895;
   v_d5GeVpFe90 [ 9 ] = 0.00885;
   v_d5GeVpFe90 [ 10 ] = 0.00802;
   v_d5GeVpFe90 [ 11 ] = 0.00756;
   v_d5GeVpFe90 [ 12 ] = 0.00669;
   v_d5GeVpFe90 [ 13 ] = 0.00604;
   v_d5GeVpFe90 [ 14 ] = 0.00521;
   v_d5GeVpFe90 [ 15 ] = 0.00452;
   v_d5GeVpFe90 [ 16 ] = 0.00393;
   v_d5GeVpFe90 [ 17 ] = 0.00327;
   v_d5GeVpFe90 [ 18 ] = 0.00267;
   v_d5GeVpFe90 [ 19 ] = 0.00202;
   v_d5GeVpFe90 [ 20 ] = 0.00153;
   v_d5GeVpFe90 [ 21 ] = 0.00122;
   v_d5GeVpFe90 [ 22 ] = 0.000835;
   v_d5GeVpFe90 [ 23 ] = 0.000572;
   v_d5GeVpFe90 [ 24 ] = 0.000378;
   v_d5GeVpFe90 [ 25 ] = 0.000281;
   v_d5GeVpFe90 [ 26 ] = 0.000219;
   v_d5GeVpFe90 [ 27 ] = 0.000155;
   v_d5GeVpFe90 [ 28 ] = 0.000107;
   v_d5GeVpFe90 [ 29 ] = 0.0000844;
   v_d5GeVpFe90 [ 30 ] = 0.0000428;
   v_d5GeVpFe90 [ 31 ] = 0.0000119;
   v_d5GeVpFe90 [ 32 ] = 0.00000715;
   v_d5GeVpFe90 [ 33 ] = 0.00000576;
   v_d5GeVpFe90 [ 34 ] = 0.00000294;
   v_d5GeVpFe90 [ 35 ] = 0.00000109;
   v_d5GeVpFe90 [ 36 ] = 0.000000992;
   v_d5GeVpFe90 [ 37 ] = 0.0000000281;

   //create_p_alley ( &v_d5GeVpFe90 , &v_p5GeVpFe90 );
   create_FeT_p_alley ( &v_d5GeVpFe90 , &v_p5GeVpFe90 );

   v_d3GeVpHgM000to015.resize( 45 );
   v_d3GeVpHgM000to015 [ 0 ] = 0;
   v_d3GeVpHgM000to015 [ 1 ] = 0.0107;
   v_d3GeVpHgM000to015 [ 2 ] = 0.00942;
   v_d3GeVpHgM000to015 [ 3 ] = 0.00839;
   v_d3GeVpHgM000to015 [ 4 ] = 0.00698;
   v_d3GeVpHgM000to015 [ 5 ] = 0.00658;
   v_d3GeVpHgM000to015 [ 6 ] = 0.00597;
   v_d3GeVpHgM000to015 [ 7 ] = 0.00547;
   v_d3GeVpHgM000to015 [ 8 ] = 0.00493;
   v_d3GeVpHgM000to015 [ 9 ] = 0.00464;
   v_d3GeVpHgM000to015 [ 10 ] = 0.00411;
   v_d3GeVpHgM000to015 [ 11 ] = 0.00405;
   v_d3GeVpHgM000to015 [ 12 ] = 0.00352;
   v_d3GeVpHgM000to015 [ 13 ] = 0.00324;
   v_d3GeVpHgM000to015 [ 14 ] = 0.00299;
   v_d3GeVpHgM000to015 [ 15 ] = 0.00239;
   v_d3GeVpHgM000to015 [ 16 ] = 0.00231;
   v_d3GeVpHgM000to015 [ 17 ] = 0.00201;
   v_d3GeVpHgM000to015 [ 18 ] = 0.00185;
   v_d3GeVpHgM000to015 [ 19 ] = 0.00182;
   v_d3GeVpHgM000to015 [ 20 ] = 0.00145;
   v_d3GeVpHgM000to015 [ 21 ] = 0.00122;
   v_d3GeVpHgM000to015 [ 22 ] = 0.00104;
   v_d3GeVpHgM000to015 [ 23 ] = 0.000861;
   v_d3GeVpHgM000to015 [ 24 ] = 0.000726;
   v_d3GeVpHgM000to015 [ 25 ] = 0.000581;
   v_d3GeVpHgM000to015 [ 26 ] = 0.000443;
   v_d3GeVpHgM000to015 [ 27 ] = 0.000372;
   v_d3GeVpHgM000to015 [ 28 ] = 0.000319;
   v_d3GeVpHgM000to015 [ 29 ] = 0.000231;
   v_d3GeVpHgM000to015 [ 30 ] = 0.000225;
   v_d3GeVpHgM000to015 [ 31 ] = 0.000199;
   v_d3GeVpHgM000to015 [ 32 ] = 0.000163;
   v_d3GeVpHgM000to015 [ 33 ] = 0.000159;
   v_d3GeVpHgM000to015 [ 34 ] = 0.000155;
   v_d3GeVpHgM000to015 [ 35 ] = 0.000162;
   v_d3GeVpHgM000to015 [ 36 ] = 0.000152;
   v_d3GeVpHgM000to015 [ 37 ] = 0.000138;
   v_d3GeVpHgM000to015 [ 38 ] = 0.000134;
   v_d3GeVpHgM000to015 [ 39 ] = 0.0000948;
   v_d3GeVpHgM000to015 [ 40 ] = 0.0000742;
   v_d3GeVpHgM000to015 [ 41 ] = 0.0000529;
   v_d3GeVpHgM000to015 [ 42 ] = 0.0000308;
   v_d3GeVpHgM000to015 [ 43 ] = 0.00000922;
   v_d3GeVpHgM000to015 [ 44 ] = 0.00000251;

   create_p_alley ( &v_d3GeVpHgM000to015 , &v_p3GeVpHgM000to015 );

   v_d3GeVpHgM045to060.resize ( 43 );
   v_d3GeVpHgM045to060 [ 0 ] = 0;
   v_d3GeVpHgM045to060 [ 1 ] = 0.0173;
   v_d3GeVpHgM045to060 [ 2 ] = 0.0148;
   v_d3GeVpHgM045to060 [ 3 ] = 0.0129;
   v_d3GeVpHgM045to060 [ 4 ] = 0.0117;
   v_d3GeVpHgM045to060 [ 5 ] = 0.0104;
   v_d3GeVpHgM045to060 [ 6 ] = 0.00925;
   v_d3GeVpHgM045to060 [ 7 ] = 0.0083;
   v_d3GeVpHgM045to060 [ 8 ] = 0.00783;
   v_d3GeVpHgM045to060 [ 9 ] = 0.00696;
   v_d3GeVpHgM045to060 [ 10 ] = 0.00633;
   v_d3GeVpHgM045to060 [ 11 ] = 0.00589;
   v_d3GeVpHgM045to060 [ 12 ] = 0.00547;
   v_d3GeVpHgM045to060 [ 13 ] = 0.00507;
   v_d3GeVpHgM045to060 [ 14 ] = 0.00432;
   v_d3GeVpHgM045to060 [ 15 ] = 0.00383;
   v_d3GeVpHgM045to060 [ 16 ] = 0.00327;
   v_d3GeVpHgM045to060 [ 17 ] = 0.00282;
   v_d3GeVpHgM045to060 [ 18 ] = 0.00242;
   v_d3GeVpHgM045to060 [ 19 ] = 0.00209;
   v_d3GeVpHgM045to060 [ 20 ] = 0.00165;
   v_d3GeVpHgM045to060 [ 21 ] = 0.00127;
   v_d3GeVpHgM045to060 [ 22 ] = 0.000958;
   v_d3GeVpHgM045to060 [ 23 ] = 0.000737;
   v_d3GeVpHgM045to060 [ 24 ] = 0.000516;
   v_d3GeVpHgM045to060 [ 25 ] = 0.000329;
   v_d3GeVpHgM045to060 [ 26 ] = 0.000247;
   v_d3GeVpHgM045to060 [ 27 ] = 0.000157;
   v_d3GeVpHgM045to060 [ 28 ] = 0.00011;
   v_d3GeVpHgM045to060 [ 29 ] = 0.000066;
   v_d3GeVpHgM045to060 [ 30 ] = 0.0000441;
   v_d3GeVpHgM045to060 [ 31 ] = 0.0000262;
   v_d3GeVpHgM045to060 [ 32 ] = 0.0000134;
   v_d3GeVpHgM045to060 [ 33 ] = 0.00000737;
   v_d3GeVpHgM045to060 [ 34 ] = 0.00000684;
   v_d3GeVpHgM045to060 [ 35 ] = 0.00000305;
   v_d3GeVpHgM045to060 [ 36 ] = 0.00000167;
   v_d3GeVpHgM045to060 [ 37 ] = 0.00000139;
   v_d3GeVpHgM045to060 [ 38 ] = 0.000000665;
   v_d3GeVpHgM045to060 [ 39 ] = 0.0000000741;
   v_d3GeVpHgM045to060 [ 40 ] = 0.000000263;
   v_d3GeVpHgM045to060 [ 41 ] = 0.000000176;
   v_d3GeVpHgM045to060 [ 42 ] = 0.0000000524;

   create_p_alley ( &v_d3GeVpHgM045to060 , &v_p3GeVpHgM045to060 );

   v_d3GeVpHgM090to105.resize( 36 );
   v_d3GeVpHgM090to105 [ 0 ] = 0;
   v_d3GeVpHgM090to105 [ 1 ] = 0.0218;
   v_d3GeVpHgM090to105 [ 2 ] = 0.0193;
   v_d3GeVpHgM090to105 [ 3 ] = 0.0169;
   v_d3GeVpHgM090to105 [ 4 ] = 0.0152;
   v_d3GeVpHgM090to105 [ 5 ] = 0.0133;
   v_d3GeVpHgM090to105 [ 6 ] = 0.0118;
   v_d3GeVpHgM090to105 [ 7 ] = 0.011;
   v_d3GeVpHgM090to105 [ 8 ] = 0.0103;
   v_d3GeVpHgM090to105 [ 9 ] = 0.00887;
   v_d3GeVpHgM090to105 [ 10 ] = 0.00765;
   v_d3GeVpHgM090to105 [ 11 ] = 0.00702;
   v_d3GeVpHgM090to105 [ 12 ] = 0.00641;
   v_d3GeVpHgM090to105 [ 13 ] = 0.00564;
   v_d3GeVpHgM090to105 [ 14 ] = 0.00494;
   v_d3GeVpHgM090to105 [ 15 ] = 0.00423;
   v_d3GeVpHgM090to105 [ 16 ] = 0.00321;
   v_d3GeVpHgM090to105 [ 17 ] = 0.00248;
   v_d3GeVpHgM090to105 [ 18 ] = 0.00186;
   v_d3GeVpHgM090to105 [ 19 ] = 0.00141;
   v_d3GeVpHgM090to105 [ 20 ] = 0.000955;
   v_d3GeVpHgM090to105 [ 21 ] = 0.000619;
   v_d3GeVpHgM090to105 [ 22 ] = 0.000347;
   v_d3GeVpHgM090to105 [ 23 ] = 0.000217;
   v_d3GeVpHgM090to105 [ 24 ] = 0.000104;
   v_d3GeVpHgM090to105 [ 25 ] = 0.0000556;
   v_d3GeVpHgM090to105 [ 26 ] = 0.0000298;
   v_d3GeVpHgM090to105 [ 27 ] = 0.0000136;
   v_d3GeVpHgM090to105 [ 28 ] = 0.00000368;
   v_d3GeVpHgM090to105 [ 29 ] = 0.00000562;
   v_d3GeVpHgM090to105 [ 30 ] = 0.0000025;
   v_d3GeVpHgM090to105 [ 31 ] = 0.000000372;
   v_d3GeVpHgM090to105 [ 32 ] = 0.00000199;
   v_d3GeVpHgM090to105 [ 33 ] = 0.000000295;
   v_d3GeVpHgM090to105 [ 34 ] = 0.000000263;
   v_d3GeVpHgM090to105 [ 35 ] = 0.000000235;

   create_p_alley ( &v_d3GeVpHgM090to105 , &v_p3GeVpHgM090to105 );

   v_d3GeVpHgM135to150.resize( 30 );
   v_d3GeVpHgM135to150 [ 0 ] = 0;
   v_d3GeVpHgM135to150 [ 1 ] = 0.0302;
   v_d3GeVpHgM135to150 [ 2 ] = 0.0247;
   v_d3GeVpHgM135to150 [ 3 ] = 0.0209;
   v_d3GeVpHgM135to150 [ 4 ] = 0.0194;
   v_d3GeVpHgM135to150 [ 5 ] = 0.0157;
   v_d3GeVpHgM135to150 [ 6 ] = 0.0132;
   v_d3GeVpHgM135to150 [ 7 ] = 0.0126;
   v_d3GeVpHgM135to150 [ 8 ] = 0.011;
   v_d3GeVpHgM135to150 [ 9 ] = 0.00865;
   v_d3GeVpHgM135to150 [ 10 ] = 0.00793;
   v_d3GeVpHgM135to150 [ 11 ] = 0.00688;
   v_d3GeVpHgM135to150 [ 12 ] = 0.00638;
   v_d3GeVpHgM135to150 [ 13 ] = 0.00574;
   v_d3GeVpHgM135to150 [ 14 ] = 0.00445;
   v_d3GeVpHgM135to150 [ 15 ] = 0.0034;
   v_d3GeVpHgM135to150 [ 16 ] = 0.00267;
   v_d3GeVpHgM135to150 [ 17 ] = 0.00176;
   v_d3GeVpHgM135to150 [ 18 ] = 0.00126;
   v_d3GeVpHgM135to150 [ 19 ] = 0.000777;
   v_d3GeVpHgM135to150 [ 20 ] = 0.000503;
   v_d3GeVpHgM135to150 [ 21 ] = 0.000315;
   v_d3GeVpHgM135to150 [ 22 ] = 0.000136;
   v_d3GeVpHgM135to150 [ 23 ] = 0.0000847;
   v_d3GeVpHgM135to150 [ 24 ] = 0.0000565;
   v_d3GeVpHgM135to150 [ 25 ] = 0.0000264;
   v_d3GeVpHgM135to150 [ 26 ] = 0.00000641;
   v_d3GeVpHgM135to150 [ 27 ] = 0.0000019;
   v_d3GeVpHgM135to150 [ 28 ] = 0.0000017;
   v_d3GeVpHgM135to150 [ 29 ] = 0.00000151;

   create_p_alley ( &v_d3GeVpHgM135to150 , &v_p3GeVpHgM135to150 );

   v_d24GeVpHgM090to105.resize( 37 );
   v_d24GeVpHgM090to105 [ 0 ] = 0;
   v_d24GeVpHgM090to105 [ 1 ] = 0.0258;
   v_d24GeVpHgM090to105 [ 2 ] = 0.0223;
   v_d24GeVpHgM090to105 [ 3 ] = 0.0204;
   v_d24GeVpHgM090to105 [ 4 ] = 0.0171;
   v_d24GeVpHgM090to105 [ 5 ] = 0.014;
   v_d24GeVpHgM090to105 [ 6 ] = 0.0121;
   v_d24GeVpHgM090to105 [ 7 ] = 0.011;
   v_d24GeVpHgM090to105 [ 8 ] = 0.00898;
   v_d24GeVpHgM090to105 [ 9 ] = 0.00872;
   v_d24GeVpHgM090to105 [ 10 ] = 0.00766;
   v_d24GeVpHgM090to105 [ 11 ] = 0.00631;
   v_d24GeVpHgM090to105 [ 12 ] = 0.00578;
   v_d24GeVpHgM090to105 [ 13 ] = 0.0046;
   v_d24GeVpHgM090to105 [ 14 ] = 0.00413;
   v_d24GeVpHgM090to105 [ 15 ] = 0.0031;
   v_d24GeVpHgM090to105 [ 16 ] = 0.00226;
   v_d24GeVpHgM090to105 [ 17 ] = 0.00199;
   v_d24GeVpHgM090to105 [ 18 ] = 0.00154;
   v_d24GeVpHgM090to105 [ 19 ] = 0.0012;
   v_d24GeVpHgM090to105 [ 20 ] = 0.000962;
   v_d24GeVpHgM090to105 [ 21 ] = 0.00063;
   v_d24GeVpHgM090to105 [ 22 ] = 0.000569;
   v_d24GeVpHgM090to105 [ 23 ] = 0.000381;
   v_d24GeVpHgM090to105 [ 24 ] = 0.000275;
   v_d24GeVpHgM090to105 [ 25 ] = 0.000155;
   v_d24GeVpHgM090to105 [ 26 ] = 0.000153;
   v_d24GeVpHgM090to105 [ 27 ] = 0.0000874;
   v_d24GeVpHgM090to105 [ 28 ] = 0.0000554;
   v_d24GeVpHgM090to105 [ 29 ] = 0.0000478;
   v_d24GeVpHgM090to105 [ 30 ] = 0.0000341;
   v_d24GeVpHgM090to105 [ 31 ] = 0.0000186;
   v_d24GeVpHgM090to105 [ 32 ] = 0.0000121;
   v_d24GeVpHgM090to105 [ 33 ] = 0.00001;
   v_d24GeVpHgM090to105 [ 34 ] = 0.00000731;
   v_d24GeVpHgM090to105 [ 35 ] = 0.00000172;
   v_d24GeVpHgM090to105 [ 36 ] = 0.00000195;

   create_p_alley ( &v_d24GeVpHgM090to105 , &v_p24GeVpHgM090to105 );

}

void ExN03PrimaryGeneratorAction::create_p_alley ( std::vector < G4double >* d_alley , std::vector < G4double >* p_alley )
{
   p_alley->resize ( d_alley->size() );  
   G4int i;   
   G4double x = 0.0;
   (*p_alley) [ 0 ] = x;
   for ( i = 1 ; i < (G4int)d_alley->size() ; i++ )
   {
      x += (*d_alley) [ i ] * ( e_bin [ i ] - e_bin [ i-1 ] );
      //std::cout << i << " " << x << " "  << e_bin [ i ] << " " << e_bin [ i-1 ] << std::endl;
      (*p_alley) [ i ] = x;
   }
}

void ExN03PrimaryGeneratorAction::create_FeT_p_alley ( std::vector < G4double >* d_alley , std::vector < G4double >* p_alley )
{
   //p_alley->resize ( d_alley->size() + 1 );  
   p_alley->resize ( d_alley->size() );  
   G4int i;   
   G4double x = 0.0;
   (*p_alley) [ 0 ] = x;
   for ( i = 1 ; i < (G4int)d_alley->size() ; i++ )
   {
      x += (*d_alley) [ i ] * ( e_FeT_bin [ i ] - e_FeT_bin [ i-1 ] );
      (*p_alley) [ i ] = x;
   }
}

G4int ExN03PrimaryGeneratorAction::search_bin_no ( std::vector< G4double >* alley , G4double x )
{
   G4int i;    
   while ( x > (*alley).back() ) 
      x = G4UniformRand();
      //return i = alley->size() - 1;

   for ( i = 0 ; i < (G4int)alley->size(); i++ )
   {
      //std::cout << i << " " << x << " " << (*alley)[ i ] << std::endl;
      if ( x - (*alley)[ i ] < 0 ) return i;
   }
   return -1;
}

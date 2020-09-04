#include "ExN03DetectorConstruction.hh"
#include "ExN03DetectorMessenger.hh"

#include "G4Material.hh"
//080109 
// BNL
#include "G4Sphere.hh"
#include "G4Tubs.hh"
//080109 
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

// SATIF
// for importance biasing
#include "G4IStore.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN03DetectorConstruction::ExN03DetectorConstruction()
:AbsorberMaterial(0),GapMaterial(0),defaultMaterial(0),
 solidWorld(0),logicWorld(0),physiWorld(0),
 solidCalor(0),logicCalor(0),physiCalor(0),
 solidLayer(0),logicLayer(0),physiLayer(0),
 solidAbsorber(0),logicAbsorber(0),physiAbsorber(0),
 solidGap (0),logicGap (0),physiGap (0),
 magField(0),
 istore ( NULL )
{
  // default parameter values of the calorimeter
  AbsorberThickness = 1.*mm;
  GapThickness      =  0.*mm;
  NbOfLayers        = 1;
  CalorSizeYZ       = 5.*m;
  ComputeCalorParameters();
  
  // materials
  DefineMaterials();
  SetAbsorberMaterial("Concrete");
  SetGapMaterial("liquidArgon");
  
  // create commands for interactive definition of the calorimeter
  detectorMessenger = new ExN03DetectorMessenger(this);

   // SATIF
   //useBiassing = FALSE;
   useBiassing = TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN03DetectorConstruction::~ExN03DetectorConstruction()
{ delete detectorMessenger;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* ExN03DetectorConstruction::Construct()
{
  return ConstructCalorimeter();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN03DetectorConstruction::DefineMaterials()
{ 
 //This function illustrates the possible ways to define materials
 
G4String symbol;             //a=mass of a mole;
G4double a, z, density;      //z=mean number of protons;  
G4int iz, n;                 //iz=number of protons  in an isotope; 
                             // n=number of nucleons in an isotope;

G4int ncomponents, natoms;
G4double abundance, fractionmass;

//
// define Elements
//

// SATIF7 
G4Element* H  = new G4Element ( "Hydrogen" , "H" , z = 1. , a = 1.00794*g/mole );
G4Element* C  = new G4Element ( "Carbon" , "C" , z = 6. , a = 12.0107*g/mole );

G4Element* O  = new G4Element ( "Oxygen" , "O" , z = 8. , a = 15.9994*g/mole );

G4Element* Mg = new G4Element ( "Magnesium"  , "Mg" , z = 12. , a = 24.3056*g/mole );
G4Element* Al = new G4Element ( "Aluminium"  , "Al" , z = 13. , a = 26.981538*g/mole );
G4Element* Si = new G4Element ( "Silicon" , "Si" , z = 14. , a = 28.0855*g/mole );
G4Element* K  = new G4Element ( "Potassium" , "K" , z = 19. , a = 39.0983*g/mole );
G4Element* Ca = new G4Element ( "Calcium" , "Ca" , z = 20. , a = 40.078*g/mole );
G4Element* Fe = new G4Element ( "Iron"  , "Fe" , z = 26. , a = 55.845*g/mole );
//

G4Element* N  = new G4Element ( "Nitrogen",symbol="N" , z= 7., a= 14.01*g/mole);
G4Element* Mn = new G4Element ( "Manganese" , "Mn" , z = 25 , a = 54.938045*g/mole );
G4Element* Na = new G4Element ( "Sodium" , "Na" , z = 11 , a = 22.98976928*g/mole );
G4Element* P  = new G4Element ( "Phosphorus" , "P" , z = 15 , a = 30.973762*g/mole );
G4Element* S  = new G4Element ( "Sulfur" , "S" , z = 16 , a = 32.065*g/mole );
G4Element* Ti = new G4Element ( "Titanium" , "Ti" , z = 22 , a = 47.867*g/mole );
G4Element* V  = new G4Element ( "Vanadium" , "V" , z = 23 , a = 50.9415*g/mole );
G4Element* Cr  = new G4Element ( "Chromium" , "Cr" , z = 24 , a = 51.9961*g/mole );
G4Element* Ni  = new G4Element ( "Nickel" , "Ni" , z = 28 , a = 58.6934*g/mole );
G4Element* Mo  = new G4Element ( "Molybden" , "Mo" , z = 42 , a = 95.94*g/mole );

/*
G4Element* Gd  = new G4Element("Gadolinium",symbol="Gd" , z= 64., a= 157.25*g/mole);

G4Element* Hg = new G4Element("Mercury",symbol="Hg" , z= 80., a= 200.59*g/mole);
*/

//
// define an Element from isotopes, by relative abundance 
//

G4Isotope* U5 = new G4Isotope("U235", iz=92, n=235, a=235.01*g/mole);
G4Isotope* U8 = new G4Isotope("U238", iz=92, n=238, a=238.03*g/mole);

G4Element* U  = new G4Element("enriched Uranium",symbol="U",ncomponents=2);
U->AddIsotope(U5, abundance= 90.*perCent);
U->AddIsotope(U8, abundance= 10.*perCent);

//
// define simple materials
//

new G4Material("Aluminium", z=13., a=26.98*g/mole, density=2.700*g/cm3);
new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);

// SATIF7 
new G4Material ( "Iron" , z = 26., a = 55.845*g/mole , density = 7.87 *g/cm3);


//new G4Material("Gadolinium"     , z=64., a= 157.25*g/mole, density= 7.901*g/cm3);
new G4Material("Lead"     , z=82., a= 207.19*g/mole, density= 11.35*g/cm3);

//G4Material* Vacuum =
new G4Material("Uranium", z=92., a= 238.03*g/mole, density= 19.05*g/cm3);

//
// define a material from elements.   case 1: chemical molecule

/*
G4Material* GdH = 
new G4Material("GdH", density= 8.0*g/cm3, ncomponents=2);
GdH->AddElement(Gd, natoms=1);
GdH->AddElement(H, natoms=1);

G4Material* HgH = 
new G4Material("HgH", density= 13.6*g/cm3, ncomponents=2);
HgH->AddElement(H, natoms=1);
HgH->AddElement(Hg, natoms=1);
*/

G4Material* UH = 
new G4Material("UH", density= 19.05*g/cm3, ncomponents=2);
UH->AddElement(H, natoms=1);
UH->AddElement(U, natoms=1);


G4Material* H2O = 
new G4Material("Water", density= 1.000*g/cm3, ncomponents=2);
H2O->AddElement(H, natoms=2);
H2O->AddElement(O, natoms=1);
// overwrite computed meanExcitationEnergy with ICRU recommended value 
H2O->GetIonisation()->SetMeanExcitationEnergy(75.0*eV);

G4Material* Sci = 
new G4Material("Scintillator", density= 1.032*g/cm3, ncomponents=2);
Sci->AddElement(C, natoms=9);
Sci->AddElement(H, natoms=10);

G4Material* Myl = 
new G4Material("Mylar", density= 1.397*g/cm3, ncomponents=3);
Myl->AddElement(C, natoms=10);
Myl->AddElement(H, natoms= 8);
Myl->AddElement(O, natoms= 4);

G4Material* SiO2 = 
new G4Material("quartz",density= 2.200*g/cm3, ncomponents=2);
SiO2->AddElement(Si, natoms=1);
SiO2->AddElement(O , natoms=2);

//
// define a material from elements.   case 2: mixture by fractional mass
//

// SATIF7 
   G4Material* Concrete = 
   new G4Material( "Concrete" , density = 2.27*g/cm3 , ncomponents = 9 );
   Concrete->AddElement( H  , fractionmass = 0.0102 );
   Concrete->AddElement( C  , fractionmass = 0.0010 );
   Concrete->AddElement( O  , fractionmass = 0.5385 );
   Concrete->AddElement( Mg , fractionmass = 0.0022 );
   Concrete->AddElement( Al , fractionmass = 0.0344 );
   Concrete->AddElement( Si , fractionmass = 0.3422 );
   Concrete->AddElement( K  , fractionmass = 0.0132 );
   Concrete->AddElement( Ca , fractionmass = 0.0441 );
   Concrete->AddElement( Fe , fractionmass = 0.0141 );
//
// BNL
   G4Material* iron_BNL = new G4Material ( "Iron_BNL" , density = 7.74 *g/cm3 , ncomponents = 3 );
   iron_BNL->AddElement ( C  , fractionmass = 0.0025 );
   iron_BNL->AddElement ( Mn , fractionmass = 0.0024 );
   iron_BNL->AddElement ( Fe , fractionmass = 0.9951 );

   G4Material* concrete_BNL = 
   new G4Material( "Concrete_BNL" , density = 2.45*g/cm3 , ncomponents = 14 );
   concrete_BNL->AddElement( H  , fractionmass = 0.00881 );
   concrete_BNL->AddElement( C  , fractionmass = 0.01273 );
   concrete_BNL->AddElement( O  , fractionmass = 0.48204 );
   concrete_BNL->AddElement( Na , fractionmass = 0.00317 );
   concrete_BNL->AddElement( Mg , fractionmass = 0.01093 );
   concrete_BNL->AddElement( Al , fractionmass = 0.01736 );
   concrete_BNL->AddElement( Si , fractionmass = 0.29698 );
   concrete_BNL->AddElement( P  , fractionmass = 0.00029 );
   concrete_BNL->AddElement( S  , fractionmass = 0.00369 );
   concrete_BNL->AddElement( K  , fractionmass = 0.00170 );
   concrete_BNL->AddElement( Ca , fractionmass = 0.14001 );
   concrete_BNL->AddElement( Ti , fractionmass = 0.00219 );
   concrete_BNL->AddElement( Mn , fractionmass = 0.00042 );
   concrete_BNL->AddElement( Fe , fractionmass = 0.01968 );

   G4Material* heavy_concrete_BNL = 
   new G4Material( "HeavyConcrete_BNL" , density = 4.41*g/cm3 , ncomponents = 9 );
   heavy_concrete_BNL->AddElement( H  , fractionmass = 0.00237 );
   heavy_concrete_BNL->AddElement( O  , fractionmass = 0.13768 );
   heavy_concrete_BNL->AddElement( Al , fractionmass = 0.00367 );
   heavy_concrete_BNL->AddElement( Si , fractionmass = 0.01575 );
   heavy_concrete_BNL->AddElement( S  , fractionmass = 0.01036 );
   heavy_concrete_BNL->AddElement( Ca , fractionmass = 0.05568 );
   heavy_concrete_BNL->AddElement( Ti , fractionmass = 0.01597 );
   heavy_concrete_BNL->AddElement( V  , fractionmass = 0.00065 );
   heavy_concrete_BNL->AddElement( Fe , fractionmass = 0.75788 );

   G4Material* stainless_BNL = new G4Material ( "Stainless_BNL" , density = 7.85 *g/cm3 , ncomponents = 5 );
   stainless_BNL->AddElement ( C  , fractionmass = 0.0005 );
   stainless_BNL->AddElement ( Cr , fractionmass = 0.1800 );
   stainless_BNL->AddElement ( Fe , fractionmass = 0.6970 );
   stainless_BNL->AddElement ( Ni , fractionmass = 0.1000 );
   stainless_BNL->AddElement ( Mo , fractionmass = 0.0225 );
//

G4Material* Air = 
new G4Material("Air"  , density= 1.290*mg/cm3, ncomponents=2);
Air->AddElement(N, fractionmass=0.7);
Air->AddElement(O, fractionmass=0.3);

//
// define a material from elements and/or others materials (mixture of mixtures)
//

G4Material* Aerog = 
new G4Material("Aerogel", density= 0.200*g/cm3, ncomponents=3);
Aerog->AddMaterial(SiO2, fractionmass=62.5*perCent);
Aerog->AddMaterial(H2O , fractionmass=37.4*perCent);
Aerog->AddElement (C   , fractionmass= 0.1*perCent);

//
// examples of gas in non STP conditions
//

G4Material* CO2 = 
new G4Material("CarbonicGas", density= 27.*mg/cm3, ncomponents=2,
                              kStateGas, 325.*kelvin, 50.*atmosphere);
CO2->AddElement(C, natoms=1);
CO2->AddElement(O, natoms=2);
 
G4Material* steam = 
new G4Material("WaterSteam", density= 0.3*mg/cm3, ncomponents=1,
                             kStateGas, 500.*kelvin, 2.*atmosphere);
steam->AddMaterial(H2O, fractionmass=1.);

//
// examples of vacuum
//

G4Material* Vacuum =
new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                           kStateGas, 3.e-18*pascal, 2.73*kelvin);

G4Material* beam = 
new G4Material("Beam", density= 1.e-5*g/cm3, ncomponents=1,
                       kStateGas, STP_Temperature, 2.e-2*bar);
beam->AddMaterial(Air, fractionmass=1.);

G4cout << *(G4Material::GetMaterialTable()) << G4endl;

//default materials of the World
defaultMaterial  = Vacuum;

//080109
//BNL 
   G4Material* Hg = new G4Material( "Mercury" , z = 80. , a = 200.59*g/mole, density = 13.6*g/cm3 );
   targetMaterial = Hg;
//080109
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* ExN03DetectorConstruction::ConstructCalorimeter()
{

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // complete the Calor parameters definition
  ComputeCalorParameters();
   
  //     
  // World
  //
//080110
//BML
//  solidWorld = new G4Box("World",				//its name
//                   WorldSizeX/2,WorldSizeYZ/2,WorldSizeYZ/2);	//its size

  solidWorld = new G4Box("World",				//its name
                   10*m/2,10*m/2,10*m/2);	//its size
//080110
                         
  logicWorld = new G4LogicalVolume(solidWorld,		//its solid
                                   defaultMaterial,	//its material
                                   "World");		//its name
                                   
  physiWorld = new G4PVPlacement(0,			//no rotation
  				 G4ThreeVector(),	//at (0,0,0)
                                 logicWorld,		//its logical volume				 
                                 "World",		//its name
                                 0,			//its mother  volume
                                 false,			//no boolean operation
                                 0);			//copy number
  //                               
  // Calorimeter
  //  
  solidCalor=0; logicCalor=0; physiCalor=0;
  solidLayer=0; logicLayer=0; physiLayer=0;
  
  if (CalorThickness > 0.)  
    { solidCalor = new G4Box("Calorimeter",		//its name
    		       CalorThickness/2,CalorSizeYZ/2,CalorSizeYZ/2);//size
    			     
      logicCalor = new G4LogicalVolume(solidCalor,	//its solid
      				       defaultMaterial,	//its material
      				       "Calorimeter");	//its name
    				       
/*
      physiCalor = new G4PVPlacement(0,			//no rotation
                                     G4ThreeVector(),	//at (0,0,0)
                                     logicCalor,	//its logical volume
                                     "Calorimeter",	//its name
                                     logicWorld,	//its mother  volume
                                     false,		//no boolean operation
                                     0);		//copy number
  
*/
  //                                 
  // Layer
  //
      solidLayer = new G4Box("Layer",			//its name
                       LayerThickness/2,CalorSizeYZ/2,CalorSizeYZ/2); //size
                       
      logicLayer = new G4LogicalVolume(solidLayer,	//its solid
                                       //defaultMaterial,	//its material
      			               AbsorberMaterial, //its material
                                       "Layer");	//its name
      if (NbOfLayers > 1)                                      
        physiLayer = new G4PVReplica("Layer",		//its name
      				     logicLayer,	//its logical volume
      				     logicCalor,	//its mother
                                     kXAxis,		//axis of replication
                                     NbOfLayers,	//number of replica
                                     LayerThickness);	//witdth of replica
      else
        physiLayer = new G4PVPlacement(0,		//no rotation
                                     G4ThreeVector(),	//at (0,0,0)
                                     logicLayer,	//its logical volume				     
                                     "Layer",		//its name
                                     logicCalor,	//its mother  volume
                                     false,		//no boolean operation
                                     0);		//copy number     
    }                                   
  
  //                               
  // Absorber
  //
  solidAbsorber=0; logicAbsorber=0; physiAbsorber=0;  
  
  if (AbsorberThickness > 0.) 
    { solidAbsorber = new G4Box("Absorber",		//its name
                          AbsorberThickness/2,CalorSizeYZ/2,CalorSizeYZ/2); 
                          
      logicAbsorber = new G4LogicalVolume(solidAbsorber,    //its solid
      			                  AbsorberMaterial, //its material
      			                  AbsorberMaterial->GetName()); //name
      			                  
/*
      physiAbsorber = new G4PVPlacement(0,		   //no rotation
      		    G4ThreeVector(-GapThickness/2,0.,0.),  //its position
                                        logicAbsorber,     //its logical volume		    
                                        AbsorberMaterial->GetName(), //its name
                                        logicLayer,        //its mother
                                        false,             //no boulean operat
                                        0);                //copy number
*/
                                        
    }
  
  //                                 
  // Gap
  //
  solidGap=0; logicGap=0; physiGap=0; 
  
  if (GapThickness > 0.)
    { solidGap = new G4Box("Gap",
    			   GapThickness/2,CalorSizeYZ/2,CalorSizeYZ/2);
    			   
      logicGap = new G4LogicalVolume(solidGap,
      				     GapMaterial,
      				     GapMaterial->GetName());
/*
      physiGap = new G4PVPlacement(0,                      //no rotation
               G4ThreeVector(AbsorberThickness/2,0.,0.),   //its position
                                   logicGap,               //its logical volume	       
                                   GapMaterial->GetName(), //its name
                                   logicLayer,             //its mother
                                   false,                  //no boulean operat
                                   0);                     //copy number
*/
    }
    
  PrintCalorParameters();     

//080109
// BNL
// Target

  G4Sphere* solidTargetS = new G4Sphere( "target_S" , 0.0 , 10.0*cm , 0 , 2*pi , 0.5*pi , pi );

  G4LogicalVolume* logicTargetS = new G4LogicalVolume( solidTargetS ,  //its solid
      			                             targetMaterial ,  //its material
      			                                  "TargetS" ); //name

  new G4PVPlacement ( 0 ,		//no rotation
                      G4ThreeVector( 0 , 0 , +10*cm ) ,	//at (0,0,0)
                      logicTargetS ,	//its logical volume				     
                      "TargetS" ,	//its name
                      logicWorld ,	//its mother  volume
                      false,		//no boolean operation
                      0 );		//copy number     
 

  G4Tubs* solidTargetC = new G4Tubs( "target_C" , 0.0 , 10.0*cm , 120*cm/2 , 0.0 , 2*pi );

  G4LogicalVolume* logicTargetC = new G4LogicalVolume( solidTargetC ,  //its solid
      			                             targetMaterial ,  //its material
      			                                  "TargetC" ); //name

  new G4PVPlacement ( 0 ,		//no rotation
                      G4ThreeVector( 0 , 0 , +10*cm+60*cm ) ,	//at (0,0,0)
                      logicTargetC ,	//its logical volume				     
                      "TargetC" ,	//its name
                      logicWorld ,	//its mother  volume
                      false,		//no boolean operation
                      0 );		//copy number     

  G4Sphere* solidTargetSCase = new G4Sphere( "target_S_Case" , 10.0*cm , 10.25*cm , 0 , 2*pi , 0.5*pi , pi );
  G4LogicalVolume* logicTargetSCase = new G4LogicalVolume( solidTargetSCase ,  //its solid
                                                           G4Material::GetMaterial( "Stainless_BNL" ) ,  //its material
      			                                  "TargetSCase" ); //name
  new G4PVPlacement ( 0 ,		//no rotation
                      G4ThreeVector( 0 , 0 , +10*cm ) ,	//at (0,0,0)
                      logicTargetSCase ,	//its logical volume				     
                      "TargetSCase" ,	//its name
                      logicWorld ,	//its mother  volume
                      false,		//no boolean operation
                      0 );		//copy number     

  G4Tubs* solidTargetCCase = new G4Tubs( "target_C_Case" , 10.0*cm , 10.25*cm , 120*cm/2 , 0.0 , 2*pi );
  G4LogicalVolume* logicTargetCCase = new G4LogicalVolume( solidTargetCCase ,  //its solid
                                                           G4Material::GetMaterial( "Stainless_BNL" ) ,  //its material
      			                                  "TargetCCase" ); //name
  new G4PVPlacement ( 0 ,		//no rotation
                      G4ThreeVector( 0 , 0 , +10*cm+60*cm ) ,	//at (0,0,0)
                      logicTargetCCase ,	//its logical volume				     
                      "TargetCCase" ,	//its name
                      logicWorld ,	//its mother  volume
                      false,		//no boolean operation
                      0 );		//copy number     


// Shield 

  G4double r0 = 101.6*cm; 
  G4double dr0 = 22.9*cm;
  G4double dr = 29.2*cm/2;
  G4double rmin_outer = 328.9*cm;

  if ( GetAbsorberMaterial()->GetName() == "Concrete_BNL" ) 
  {
     dr0 = 44.8*cm;
     dr = 45.8*cm/2;
     rmin_outer = 467.0*cm;
  } 


  G4double r = r0 + dr0;
  G4Tubs* solidShield0 = new G4Tubs( "Shield0" , r0 , r , 305*cm/2 , 0.0 , 2*pi );

  G4LogicalVolume* logicShield0 = new G4LogicalVolume( solidShield0 ,  //its solid
      			                           AbsorberMaterial ,  //its material
      			                                  "Shield0" ); //name

  new G4PVPlacement ( 0 ,		//no rotation
                      G4ThreeVector() ,	//at (0,0,0)
                      logicShield0 ,	//its logical volume				     
                      "Shield0" ,	//its name
                      logicWorld ,	//its mother  volume
                      false,		//no boolean operation
                      0 );		//copy number     


  G4int irmax = 15;
  for ( G4int ir = 1 ; ir < irmax; ir++ )  
  {
     std::ostringstream os;
     os << "Shield" << ir;
     G4String name = os.str();

     //std::cout << name << std::endl;

     r0 = r;
     r += dr;
     G4Tubs* solidShieldi = new G4Tubs( name , r0 , r , 305*cm/2 , 0.0 , 2*pi );

     G4LogicalVolume* logicShieldi = new G4LogicalVolume( solidShieldi ,  //its solid
      			                           AbsorberMaterial ,  //its material
      			                                  name ); //name

     new G4PVPlacement ( 0 ,		//no rotation
                         G4ThreeVector() ,	//at (0,0,0)
                         logicShieldi ,	//its logical volume				     
                         name ,	//its name
                         logicWorld ,	//its mother  volume
                         false,		//no boolean operation
                         ir );		//copy number     

  }

// Outer Shield
  r = rmin_outer + 100*cm;
  G4Tubs* solidOuterShield = new G4Tubs( "OuterShield" , rmin_outer , r , 305*cm/2 , 0.0 , 2*pi );
  G4LogicalVolume* logicOuterShield = new G4LogicalVolume( solidOuterShield ,  //its solid
                                                           G4Material::GetMaterial( "HeavyConcrete_BNL" ) ,  //its material
      			                                   "OuterShield" ); //name

  new G4PVPlacement ( 0 ,		//no rotation
                      G4ThreeVector() ,	//at (0,0,0)
                      logicOuterShield ,	//its logical volume				     
                      "OuterShield" ,	//its name
                      logicWorld ,	//its mother  volume
                      false,		//no boolean operation
                      0 );		//copy number     

// Front and Back Shield
  r = rmin_outer + 100*cm;
  G4Tubs* solidFrontShield = new G4Tubs( "FrontShield" , r0 , r , 305*cm/2 , 0.0 , 2*pi );
  G4LogicalVolume* logicFrontShield = new G4LogicalVolume( solidFrontShield ,  //its solid
                                                           G4Material::GetMaterial( "HeavyConcrete_BNL" ) ,  //its material
      			                                   "FrontShield" ); //name

  new G4PVPlacement ( 0 ,		//no rotation
                      G4ThreeVector( 0 , 0 , -305*cm ) ,	//at 
                      logicFrontShield ,	//its logical volume				     
                      "FrontShield" ,	//its name
                      logicWorld ,	//its mother  volume
                      false,		//no boolean operation
                      0 );		//copy number     

  new G4PVPlacement ( 0 ,		//no rotation
                      G4ThreeVector( 0 , 0 , 305*cm ) ,	//at 
                      logicFrontShield ,	//its logical volume				     
                      "BackShield" ,	//its name
                      logicWorld ,	//its mother  volume
                      false,		//no boolean operation
                      0 );		//copy number     

// End Shield
  G4Tubs* solidEndShield = new G4Tubs( "EndShield" , 0 , r , 305*cm/2 , 0.0 , 2*pi );
  G4LogicalVolume* logicEndShield = new G4LogicalVolume( solidEndShield ,  //its solid
                                                           G4Material::GetMaterial( "HeavyConcrete_BNL" ) ,  //its material
      			                                   "EndShield" ); //name
  new G4PVPlacement ( 0 ,		//no rotation
                      G4ThreeVector( 0 , 0 , 305*cm*2  ) ,	//at 
                      logicEndShield ,	//its logical volume				     
                      "EndShield" ,	//its name
                      logicWorld ,	//its mother  volume
                      false,		//no boolean operation
                      0 );		//copy number     
//080109
  
  //                                        
  // Visualization attributes
  //
  logicWorld->SetVisAttributes (G4VisAttributes::Invisible);

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  logicCalor->SetVisAttributes(simpleBoxVisAtt);

 /*
  // Below are vis attributes that permits someone to test / play 
  // with the interactive expansion / contraction geometry system of the
  // vis/OpenInventor driver :
 {G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  simpleBoxVisAtt->SetVisibility(true);
  delete logicCalor->GetVisAttributes();
  logicCalor->SetVisAttributes(simpleBoxVisAtt);}

 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  logicLayer->SetVisAttributes(atb);}
  
 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  atb->SetForceSolid(true);
  logicAbsorber->SetVisAttributes(atb);}
  
 {//Set opacity = 0.2 then transparency = 1 - 0.2 = 0.8
  G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.2));
  atb->SetForceSolid(true);
  logicGap->SetVisAttributes(atb);}
  */

// SATIF
/*
   if ( useBiassing == TRUE )
   { 
      //CreateImportanceStore();
   }
  //
  //always return the physical World
  //
*/
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN03DetectorConstruction::PrintCalorParameters()
{
  G4cout << "\n------------------------------------------------------------"
         << "\n---> The calorimeter is " << NbOfLayers << " layers of: [ "
         << AbsorberThickness/mm << "mm of " << AbsorberMaterial->GetName() 
         << " + "
         << GapThickness/mm << "mm of " << GapMaterial->GetName() << " ] " 
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN03DetectorConstruction::SetAbsorberMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) AbsorberMaterial = pttoMaterial;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN03DetectorConstruction::SetGapMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  if (pttoMaterial) GapMaterial = pttoMaterial;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN03DetectorConstruction::SetAbsorberThickness(G4double val)
{
  // change Absorber thickness and recompute the calorimeter parameters
  AbsorberThickness = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN03DetectorConstruction::SetGapThickness(G4double val)
{
  // change Gap thickness and recompute the calorimeter parameters
  GapThickness = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN03DetectorConstruction::SetCalorSizeYZ(G4double val)
{
  // change the transverse size and recompute the calorimeter parameters
  CalorSizeYZ = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN03DetectorConstruction::SetNbOfLayers(G4int val)
{
  NbOfLayers = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

void ExN03DetectorConstruction::SetMagField(G4double fieldValue)
{
  //apply a global uniform magnetic field along Z axis
  G4FieldManager* fieldMgr
   = G4TransportationManager::GetTransportationManager()->GetFieldManager();

  if(magField) delete magField;		//delete the existing magn field

  if(fieldValue!=0.)			// create a new one if non nul
  { magField = new G4UniformMagField(G4ThreeVector(0.,0.,fieldValue));
    fieldMgr->SetDetectorField(magField);
    fieldMgr->CreateChordFinder(magField);
  } else {
    magField = 0;
    fieldMgr->SetDetectorField(magField);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"

void ExN03DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructCalorimeter());
}

//SATIF
/*
G4VIStore* ExN03DetectorConstruction::CreateImportanceStore()
{
;
} 
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

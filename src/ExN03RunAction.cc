#include "ExN03RunAction.hh"

#ifdef G4ANALYSIS_USE
#include "A01AnalysisManager.hh"
#endif // G4ANALYSIS_USE

#include "G4Run.hh"
#include "G4RunManager.hh"

#include "G4SystemOfUnits.hh"

// a score table
//071207
//#include "G4ScoreTable.hh"
#include "G4IStore.hh"
//#include "B01ScoreTable.hh"
//071207

#include <sstream>
//#include <fstream>

//071207
// For Primitive Scorers
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDParticleFilter.hh"
#include "G4PSNofCollision.hh"
#include "G4PSPopulation.hh"
#include "G4PSTrackCounter.hh"
#include "G4PSTrackLength.hh"
//071207
//071210
#include "G4THitsMap.hh"
//071210
//080110
//BNL 
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
//


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ExN03RunAction::ExN03RunAction()
{
}

ExN03RunAction::ExN03RunAction( ExN03DetectorConstruction* det , G4String pl )
//071210
: FieldName(15),
  FieldValue(14)
//071210
{
   setLogBins ( 0 , 4 , 10 );   
   init_neutron_flux2dose_conversion_factor ();
   init_neutron_activation_XS ();

   detector = det;
   istore = NULL;

   mgs = new G4GeometrySampler( detector->GetphysWorld() , "neutron" );

   #ifdef G4ANALYSIS_USE
   //A01AnalysisManager* analysisManager = A01AnalysisManager::getInstance();
   G4String name = pl + ".aida";
   A01AnalysisManager::getInstance( name );
   //A01AnalysisManager::getInstance( );
   A01AnalysisManager::getInstance()->getDataPointSetFactory();
   #endif // G4ANALYSIS_USE

   ofile = new std::ofstream();
   //G4String fname = pl + ".dat";
   //(*ofile).open ( fname.c_str() , std::fstream::out ); 

//071210
  theSDName.push_back(G4String("ConcreteSD"));
  std::cout << " SetSensitive " << std::endl;
  SetSensitive();
//071210
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN03RunAction::~ExN03RunAction()
{

#ifdef G4ANALYSIS_USE
   A01AnalysisManager::dispose();
#endif // G4ANALYSIS_USE
   //(*ofile).close (); 
   delete ofile;

//071210
  theSDName.clear();
//071210
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* ExN03RunAction::GenerateRun()
{
  std::cout << " ExN03RunAction::GenerateRun " << std::endl;
  // Generate new RUN object, which is specially
  // dedicated for MultiFunctionalDetector scheme.
  //  Detail description can be found in B01Run.hh/cc.
  return new B01Run(theSDName);
}

void ExN03RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

   G4int n = detector->GetNbOfLayers();
   v_neutron_spectra.clear();
   v_neutron_spectra.resize( n );
   v_neutron_dose.clear();
   v_neutron_dose.resize( n );
   
   //v_neutron_dose_tally.resize( n );
#ifndef G4V93OROLD
   v_neutron_dose_convergence.resize( n );
   for ( G4int i = 0 ; i < n ; i ++ ) 
   {
      //v_neutron_dose_tally [ i ] = new TKTally(); // will be delete EndOfRunAction
      v_neutron_dose_convergence [ i ] = new G4ConvergenceTester(); 
   }
#endif

   std::vector< std::vector < G4double >* >::iterator it; 
   for ( it = v_neutron_spectra.begin() ; it != v_neutron_spectra.end() ; it++ )
   {
       *it = new std::vector< G4double >; 
       //(*it)->resize ( v_energy_ch.size() ); 
       (*it)->resize ( neutron_flux2dose_conversion_factor.size() ); 
   }
   v_neutron_data.clear();

   v_neutron_data2.clear();

//080110
//BNL 
   v_ar_204Bi.resize(14);
   v_ar_206Bi.resize(14);

   for ( G4int i = 0 ; i < 14 ; i++ ) 
   {
      v_ar_204Bi [i] = 0;
      v_ar_206Bi [i] = 0;
   }


   G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
   //Until geant4.9.6.p02 (geant4.10.00.b01)
   //istore = new G4IStore( *(detector->GetphysiWorld()) );
   //After geant4.10.00
#ifdef BEFOREG4V10
   istore = new G4IStore( *(detector->GetphysiWorld()) );
#else
   istore = G4IStore::GetInstance();
#endif

   G4double imp = 1.0;
   istore->AddImportanceGeometryCell( imp , *(detector->GetphysiWorld()) );
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "TargetS" ) ) );
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "TargetSCase" ) ) );
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "TargetC" ) ) );
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "TargetCCase" ) ) );

   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "Shield0" ) ) , 0 );
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "OuterShield" ) ) , 0 );
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "FrontShield" ) ) , 0 );
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "BackShield" ) ) , 0 );
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "EndShield" ) ) , 0 );

   imp *= 2.0;
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "Shield1" ) ) , 1 );
   imp *= 2.0;
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "Shield2" ) ) , 2 );
   imp *= 2.0;
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "Shield3" ) ) , 3 );
   imp *= 2.0;
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "Shield4" ) ) , 4 );
   imp *= 2.0;
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "Shield5" ) ) , 5 );
   imp *= 2.0;
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "Shield6" ) ) , 6 );
   imp *= 2.0;
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "Shield7" ) ) , 7 );
   imp *= 2.0;
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "Shield8" ) ) , 8 );
   imp *= 2.0;
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "Shield9" ) ) , 9 );
   imp *= 2.0;
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "Shield10" ) ) , 10 );
   imp *= 2.0;
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "Shield11" ) ) , 11 );
   imp *= 2.0;
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "Shield12" ) ) , 12 );
   imp *= 2.0;
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "Shield13" ) ) , 13 );
   imp *= 2.0;
   istore->AddImportanceGeometryCell( imp , *(pvs->GetVolume( "Shield14" ) ) , 14 );


//080110
/*
   istore = new G4IStore( *(detector->GetphysiWorld()) );

   G4double imp = 1.0;
   istore->AddImportanceGeometryCell( imp , *(detector->GetphysiWorld()) );
   istore->AddImportanceGeometryCell( imp , *(detector->GetphysiCalor()) );

   for ( n = 0 ; n < detector->GetNbOfLayers() ; n++ )
   {
      imp = std::pow( 2., n);
      //std::cout << "Layer " << n << std::endl;
      istore->AddImportanceGeometryCell ( imp , *(detector->GetphysiLayer()) , n );
      //istore->AddImportanceGeometryCell ( 1.0 , *(detector->GetphysiLayer()) , n );
   }
*/

//071207
   mgs->PrepareImportanceSampling( istore , 0 );
   mgs->Configure();
//071207
   //detector->GetlogiLayer()->SetSensitiveDetector(MFDet);
//080110
//BNL 
   G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
   lvs->GetVolume( "TargetC" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "TargetS" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "Shield0" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "Shield1" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "Shield2" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "Shield3" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "Shield4" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "Shield5" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "Shield6" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "Shield7" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "Shield8" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "Shield9" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "Shield10" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "Shield11" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "Shield12" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "Shield13" )->SetSensitiveDetector( MFDet );
   lvs->GetVolume( "Shield14" )->SetSensitiveDetector( MFDet );
//080110

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN03RunAction::EndOfRunAction(const G4Run* aRun ) 
{
//080110
   std::cout << "EndOfRunAction" << std::endl;
   std::cout << (G4int)theSDName.size() << std::endl;
//080110

//071210
  //- B01Run object.
  B01Run* re02Run = (B01Run*)aRun;
  //--- Dump all socred quantities involved in B01Run.
  //re02Run->DumpAllScorer();
  //---
  //G4RunManager* mgr = G4RunManager::GetRunManager();
  //

  for ( G4int i = 0; i < (G4int)theSDName.size(); i++ ){
 //   const G4VUserDetectorConstruction* vdet = mgr->GetUserDetectorConstruction();
 //   B01DetectorConstruction* bdet = (B01DetectorConstruction*)vdet;
    //

    //---------------------------------------------
    // Dump accumulated quantities for this RUN.
    //  (Display only central region of x-y plane)
    //      0       ConcreteSD/Collisions
    //      1       ConcreteSD/CollWeight
    //      2       ConcreteSD/Population
    //      3       ConcreteSD/TrackEnter
    //      4       ConcreteSD/SL
    //      5       ConcreteSD/SLW
    //      6       ConcreteSD/SLWE
    //      7       ConcreteSD/SLW_V
    //      8       ConcreteSD/SLWE_V
    //---------------------------------------------
    G4THitsMap<G4double>* Collisions = re02Run->GetHitsMap(theSDName[i]+"/Collisions");
      std::cout << "Collisiions " << Collisions << std::endl;
    G4THitsMap<G4double>* CollWeight = re02Run->GetHitsMap(theSDName[i]+"/CollWeight");
    G4THitsMap<G4double>* Population = re02Run->GetHitsMap(theSDName[i]+"/Population");
    G4THitsMap<G4double>* TrackEnter = re02Run->GetHitsMap(theSDName[i]+"/TrackEnter");
    G4THitsMap<G4double>* SL = re02Run->GetHitsMap(theSDName[i]+"/SL");
    G4THitsMap<G4double>* SLW = re02Run->GetHitsMap(theSDName[i]+"/SLW");
    G4THitsMap<G4double>* SLWE = re02Run->GetHitsMap(theSDName[i]+"/SLWE");
    G4THitsMap<G4double>* SLW_V = re02Run->GetHitsMap(theSDName[i]+"/SLW_V");
    G4THitsMap<G4double>* SLWE_V = re02Run->GetHitsMap(theSDName[i]+"/SLWE_V");


    G4cout << "=============================================================" <<G4endl;
    G4cout << " Number of event processed : "<< aRun->GetNumberOfEvent() << G4endl;
    G4cout << "=============================================================" <<G4endl;

    std::ostream *myout = &G4cout;
    PrintHeader(myout);

    //G4int n = detector->GetNbOfLayers();
    G4int n = 15;
    for ( G4int iz = 0; iz < n; iz++){
      G4double* SumCollisions = (*Collisions)[iz];
      G4double* SumCollWeight = (*CollWeight)[iz];
      G4double* Populations   = (*Population)[iz];
      G4double* TrackEnters   = (*TrackEnter)[iz];
      G4double* SLs   = (*SL)[iz];
      G4double* SLWs   = (*SLW)[iz];
      G4double* SLWEs   = (*SLWE)[iz];
      G4double* SLW_Vs   = (*SLW_V)[iz];
      G4double* SLWE_Vs   = (*SLWE_V)[iz];
      if ( !SumCollisions ) SumCollisions = new G4double(0.0);
      if ( !SumCollWeight ) SumCollWeight = new G4double(0.0);
      if ( !Populations   ) Populations   = new G4double(0.0);
      if ( !TrackEnters   ) TrackEnters   = new G4double(0.0);
      if ( !SLs   ) SLs   = new G4double(0.0);
      if ( !SLWs   ) SLWs   = new G4double(0.0);
      if ( !SLWEs   ) SLWEs   = new G4double(0.0);
      if ( !SLW_Vs   ) SLW_Vs   = new G4double(0.0);
      if ( !SLWE_Vs   ) SLWE_Vs   = new G4double(0.0);
      G4double NumWeightedEnergy =0.0;
      G4double FluxWeightedEnergy=0.0;
      G4double AverageTrackWeight=0.0;
      if ( *SLW_Vs !=0. ) NumWeightedEnergy  = (*SLWE_Vs)/(*SLW_Vs);
      if ( *SLWs   !=0. ) FluxWeightedEnergy  = (*SLWEs)/(*SLWs);
      if ( *SLs    !=0. ) AverageTrackWeight  = (*SLWs)/(*SLs);
//      G4String cname = bdet->GetCellName(iz);
      G4String cname = GetLayerName( iz );
      G4cout
        << std::setw(FieldValue) << cname << " |"
        << std::setw(FieldValue) << (*TrackEnters) << " |"
        << std::setw(FieldValue) << (*Populations) << " |"
        << std::setw(FieldValue) << (*SumCollisions) << " |"
        << std::setw(FieldValue) << (*SumCollWeight) << " |"
        << std::setw(FieldValue) << NumWeightedEnergy << " |"
        << std::setw(FieldValue) << FluxWeightedEnergy << " |"
        << std::setw(FieldValue) << AverageTrackWeight << " |"
        << std::setw(FieldValue) << (*SLs) << " |"
        << std::setw(FieldValue) << (*SLWs) << " |"
        << std::setw(FieldValue) << (*SLW_Vs) << " |"
        << std::setw(FieldValue) << (*SLWEs) << " |"
        << std::setw(FieldValue) << (*SLWE_Vs) << " |"
        << G4endl;
    }
    G4cout << "============================================="<<G4endl;
  }

//071210
   
   G4double a_factor = 1.0 / aRun->GetNumberOfEvent();
   //std::vector< std::pair< G4int , G4double >* >::iterator it; 
   std::vector< std::pair< G4int , std::pair < G4double , G4double >* >* >::iterator it; 
   for ( it = v_neutron_data.begin() ; it !=  v_neutron_data.end() ; it++ ) 
   {
      //std::cout <<  (*it)->first << " " << (*it)->second->first << " " << (*it)->second->second << std::endl;
      //( *( v_neutron_spectra[ (*it)->first ] ) ) [ search_bin_no ( (*it)->second ) ] +=  1; 
      ( *( v_neutron_spectra[ (*it)->first ] ) ) [ search_bin_no_neutron_flux2dose ( (*it)->second->first ) ] +=  1.0 * a_factor * ( (*it)->second->second ); 

      G4double x = neutron_flux2dose_conversion_factor [ search_bin_no_neutron_flux2dose ( (*it)->second->first ) ];
      //v_neutron_dose [ (*it)->first ] += a_factor * 3600 * x * (*it)->second->second; 
      v_neutron_dose [ (*it)->first ] += a_factor / 3600 * x * (*it)->second->second; 
   }


// Tally
   {
      
   std::cout << std::endl;
   std::cout << "Tally Output  start" << std::endl;
   std::cout << std::endl;

// Tally Output 
   G4int j = 1;
   for ( G4int i = 0 ; i < detector->GetNbOfLayers() ; i++ ) 
   {
      if ( i * detector->GetLayerThickness()/m >= j ) 
      {
         G4cout << "Depth " << j << "m" << G4endl;
/*
         v_neutron_dose_tally [ i ] -> CalStat(); 
         v_neutron_dose_tally [ i ] -> ShowResult(); 
*/

#ifndef G4V93OROLD
         v_neutron_dose_convergence[i]->ShowResult();
         v_neutron_dose_convergence[i]->ShowHistory();
#endif
/*
      G4cout << "EFFICIENCY = " << v_neutron_dose_tally [ i ]->GetEfficiency() << G4endl;
      G4cout << "MEAN = " << v_neutron_dose_tally [ i ]->GetMean() << G4endl;
      G4cout << "VAR = " << v_neutron_dose_tally [ i ]->GetVariance() << G4endl;
      G4cout << "SD = " << v_neutron_dose_tally [ i ]->GetStandardDeviation() << G4endl;
      G4cout << "R = "<< v_neutron_dose_tally [ i ]->GetR() << G4endl;
      G4cout << "SHIFT = "<< v_neutron_dose_tally [ i ]->GetShift() << G4endl;
      G4cout << "VOV = "<< v_neutron_dose_tally [ i ]->GetVOV() << G4endl;
*/
          j++; 
      }
   }

   std::cout << std::endl;
   std::cout << "Tally Output End" << std::endl;
   std::cout << std::endl;
//
   }

//071207
/*
   G4ScoreTable sp(istore);
   sp.Print(scorer->GetMapGeometryCellCellScorer(), &G4cout);
   mgs->ClearSampling();
   delete scorer;
*/
//   B01ScoreTable sp(istore);
   //sp.Print(scorer->GetMapGeometryCellCellScorer(), &G4cout);
   mgs->ClearSampling();
//071207

#ifdef BEFOREG4V10
   delete istore;
#endif

   for ( it = v_neutron_data.begin() ; it !=  v_neutron_data.end() ; it++ ) 
   {
      delete (*it)->second;
      delete *it;
   }
   std::vector< std::vector < G4double >* >::iterator itt; 
   for ( itt = v_neutron_spectra.begin() ; itt != v_neutron_spectra.end() ; itt++ )
   {
      delete *itt;
   }

   for ( G4int i = 0 ; i < detector->GetNbOfLayers() ; i ++ ) 
   {
      //delete v_neutron_dose_tally [ i ]; 
#ifndef G4V93OROLD
      delete v_neutron_dose_convergence [ i ]; 
#endif
   }

//080110
//BNL

   for ( G4int i = 0 ; i < 14 ; i++ ) 
   {
      std::cout << "BNL_RESULT " << i+1 << " " << a_factor*v_ar_206Bi [i] << " " << a_factor*v_ar_204Bi [i] << std::endl; 
   }

//080110
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



void ExN03RunAction::setLogBins( G4int iorder_min , G4int iorder_max , G4int bin_per_order )
{

   G4double energy;
   for ( G4int iorder = iorder_min ; iorder < iorder_max ; iorder++ )
   {
      G4double order = pow ( 10.0 , 1.0 * iorder );
      for ( G4int i = 0 ; i < bin_per_order ; i++ )
      {
         energy = order * pow ( 10.0 , 1.0 /  bin_per_order * i );
         v_energy_ch.push_back(energy);
      }
   }
   energy = pow( 10.0, iorder_max );
   v_energy_ch.push_back(energy);
}



G4int ExN03RunAction::search_bin_no ( G4double energy )
{
    G4int i = 0;
    std::vector<G4double>::iterator iv;
    for ( iv = v_energy_ch.begin() ; iv != v_energy_ch.end() ; iv++ )
    {
        if ( ( energy - *iv ) < 0.0 )
           return i;
        i++;
    };
    return -1;
}

void ExN03RunAction::init_neutron_activation_XS ()
{

   e_bin_of_neutron_activation_XS.resize( 34 );

   e_bin_of_neutron_activation_XS[ 0] = 22.39;
   e_bin_of_neutron_activation_XS[ 1] = 25.12;
   e_bin_of_neutron_activation_XS[ 2] = 28.18;
   e_bin_of_neutron_activation_XS[ 3] = 31.62;
   e_bin_of_neutron_activation_XS[ 4] = 35.49;
   e_bin_of_neutron_activation_XS[ 5] = 39.81;
   e_bin_of_neutron_activation_XS[ 6] = 44.67;
   e_bin_of_neutron_activation_XS[ 7] = 50.12;
   e_bin_of_neutron_activation_XS[ 8] = 56.23;
   e_bin_of_neutron_activation_XS[ 9] = 63.1;
   e_bin_of_neutron_activation_XS[ 10] = 70.79;
   e_bin_of_neutron_activation_XS[ 11] = 79.43;
   e_bin_of_neutron_activation_XS[ 12] = 89.13;
   e_bin_of_neutron_activation_XS[ 13] = 100;
   e_bin_of_neutron_activation_XS[ 14] = 112.2;
   e_bin_of_neutron_activation_XS[ 15] = 125.9;
   e_bin_of_neutron_activation_XS[ 16] = 141.3;
   e_bin_of_neutron_activation_XS[ 17] = 158.5;
   e_bin_of_neutron_activation_XS[ 18] = 177.8;
   e_bin_of_neutron_activation_XS[ 19] = 199.5;
   e_bin_of_neutron_activation_XS[ 20] = 223.9;
   e_bin_of_neutron_activation_XS[ 21] = 251.2;
   e_bin_of_neutron_activation_XS[ 22] = 281.8;
   e_bin_of_neutron_activation_XS[ 23] = 316.2;
   e_bin_of_neutron_activation_XS[ 24] = 354.8;
   e_bin_of_neutron_activation_XS[ 25] = 398.1;
   e_bin_of_neutron_activation_XS[ 26] = 446.7;
   e_bin_of_neutron_activation_XS[ 27] = 501.2;
   e_bin_of_neutron_activation_XS[ 28] = 562.3;
   e_bin_of_neutron_activation_XS[ 29] = 631;
   e_bin_of_neutron_activation_XS[ 30] = 707.9;
   e_bin_of_neutron_activation_XS[ 31] = 794.3;
   e_bin_of_neutron_activation_XS[ 32] = 891.3;
   e_bin_of_neutron_activation_XS[ 33] = 1000;


   neutron_activation_XS_206Bi.resize( 34 );

   neutron_activation_XS_206Bi[ 0] = 0;
   neutron_activation_XS_206Bi[ 1] = 0.005688;
   neutron_activation_XS_206Bi[ 2] = 0.205;
   neutron_activation_XS_206Bi[ 3] = 0.8073;
   neutron_activation_XS_206Bi[ 4] = 1.25;
   neutron_activation_XS_206Bi[ 5] = 1.114;
   neutron_activation_XS_206Bi[ 6] = 0.5228;
   neutron_activation_XS_206Bi[ 7] = 0.2972;
   neutron_activation_XS_206Bi[ 8] = 0.2002;
   neutron_activation_XS_206Bi[ 9] = 0.1538;
   neutron_activation_XS_206Bi[ 10] = 0.1403;
   neutron_activation_XS_206Bi[ 11] = 0.1174;
   neutron_activation_XS_206Bi[ 12] = 0.1016;
   neutron_activation_XS_206Bi[ 13] = 0.08717;
   neutron_activation_XS_206Bi[ 14] = 0.07666;
   neutron_activation_XS_206Bi[ 15] = 0.06479;
   neutron_activation_XS_206Bi[ 16] = 0.05211;
   neutron_activation_XS_206Bi[ 17] = 0.04;
   neutron_activation_XS_206Bi[ 18] = 0.03081;
   neutron_activation_XS_206Bi[ 19] = 0.0251;
   neutron_activation_XS_206Bi[ 20] = 0.02245;
   neutron_activation_XS_206Bi[ 21] = 0.02064;
   neutron_activation_XS_206Bi[ 22] = 0.01922;
   neutron_activation_XS_206Bi[ 23] = 0.01803;
   neutron_activation_XS_206Bi[ 24] = 0.01699;
   neutron_activation_XS_206Bi[ 25] = 0.01605;
   neutron_activation_XS_206Bi[ 26] = 0.0152;
   neutron_activation_XS_206Bi[ 27] = 0.0144;
   neutron_activation_XS_206Bi[ 28] = 0.01364;
   neutron_activation_XS_206Bi[ 29] = 0.01293;
   neutron_activation_XS_206Bi[ 30] = 0.01224;
   neutron_activation_XS_206Bi[ 31] = 0.01158;
   neutron_activation_XS_206Bi[ 32] = 0.01093;
   neutron_activation_XS_206Bi[ 33] = 0.01031;


   neutron_activation_XS_204Bi.resize( 34 );

   neutron_activation_XS_204Bi[ 1] = 0;
   neutron_activation_XS_204Bi[ 2] = 0;
   neutron_activation_XS_204Bi[ 3] = 0;
   neutron_activation_XS_204Bi[ 4] = 0;
   neutron_activation_XS_204Bi[ 5] = 0.0000004929;
   neutron_activation_XS_204Bi[ 6] = 0.02056;
   neutron_activation_XS_204Bi[ 7] = 0.2422;
   neutron_activation_XS_204Bi[ 8] = 0.4666;
   neutron_activation_XS_204Bi[ 9] = 0.48;
   neutron_activation_XS_204Bi[ 10] = 0.34;
   neutron_activation_XS_204Bi[ 11] = 0.22;
   neutron_activation_XS_204Bi[ 12] = 0.16;
   neutron_activation_XS_204Bi[ 13] = 0.13;
   neutron_activation_XS_204Bi[ 14] = 0.1098;
   neutron_activation_XS_204Bi[ 15] = 0.09082;
   neutron_activation_XS_204Bi[ 16] = 0.06607;
   neutron_activation_XS_204Bi[ 17] = 0.05455;
   neutron_activation_XS_204Bi[ 18] = 0.04201;
   neutron_activation_XS_204Bi[ 19] = 0.03423;
   neutron_activation_XS_204Bi[ 20] = 0.03062;
   neutron_activation_XS_204Bi[ 21] = 0.02814;
   neutron_activation_XS_204Bi[ 22] = 0.02621;
   neutron_activation_XS_204Bi[ 23] = 0.02459;
   neutron_activation_XS_204Bi[ 24] = 0.02317;
   neutron_activation_XS_204Bi[ 25] = 0.02189;
   neutron_activation_XS_204Bi[ 26] = 0.02072;
   neutron_activation_XS_204Bi[ 27] = 0.01963;
   neutron_activation_XS_204Bi[ 28] = 0.0186;
   neutron_activation_XS_204Bi[ 29] = 0.01763;
   neutron_activation_XS_204Bi[ 30] = 0.01669;
   neutron_activation_XS_204Bi[ 31] = 0.01579;
   neutron_activation_XS_204Bi[ 32] = 0.01491;
   neutron_activation_XS_204Bi[ 33] = 0.01406;



}

void ExN03RunAction::init_neutron_flux2dose_conversion_factor ()
{
   e_bin_of_neutron_flux2dose_conversion_factor.resize( 72 );
// energy in MeV
   e_bin_of_neutron_flux2dose_conversion_factor [  0 ] = 0.00E+0;
   e_bin_of_neutron_flux2dose_conversion_factor [  1 ] = 2.00E+1;
   e_bin_of_neutron_flux2dose_conversion_factor [  2 ] = 2.25E+1;
   e_bin_of_neutron_flux2dose_conversion_factor [  3 ] = 2.50E+1;
   e_bin_of_neutron_flux2dose_conversion_factor [  4 ] = 2.75E+1;
   e_bin_of_neutron_flux2dose_conversion_factor [  5 ] = 3.00E+1;
   e_bin_of_neutron_flux2dose_conversion_factor [  6 ] = 3.50E+1;
   e_bin_of_neutron_flux2dose_conversion_factor [  7 ] = 4.00E+1;
   e_bin_of_neutron_flux2dose_conversion_factor [  8 ] = 4.50E+1;
   e_bin_of_neutron_flux2dose_conversion_factor [  9 ] = 5.00E+1;
   e_bin_of_neutron_flux2dose_conversion_factor [ 10 ] = 5.50E+1;
   e_bin_of_neutron_flux2dose_conversion_factor [ 11 ] = 6.00E+1;
   e_bin_of_neutron_flux2dose_conversion_factor [ 12 ] = 6.50E+1;
   e_bin_of_neutron_flux2dose_conversion_factor [ 13 ] = 7.00E+1;
   e_bin_of_neutron_flux2dose_conversion_factor [ 14 ] = 8.00E+1;
   e_bin_of_neutron_flux2dose_conversion_factor [ 15 ] = 9.00E+1;
   e_bin_of_neutron_flux2dose_conversion_factor [ 16 ] = 1.00E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 17 ] = 1.10E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 18 ] = 1.20E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 19 ] = 1.40E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 20 ] = 1.60E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 21 ] = 1.80E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 22 ] = 2.00E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 23 ] = 2.25E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 24 ] = 2.50E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 25 ] = 2.75E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 26 ] = 3.00E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 27 ] = 3.25E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 28 ] = 3.50E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 29 ] = 3.75E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 30 ] = 4.00E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 31 ] = 5.00E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 32 ] = 6.00E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 33 ] = 7.00E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 34 ] = 8.00E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 35 ] = 9.00E+2;
   e_bin_of_neutron_flux2dose_conversion_factor [ 36 ] = 1.00E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 37 ] = 1.10E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 38 ] = 1.20E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 39 ] = 1.30E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 40 ] = 1.40E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 41 ] = 1.50E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 42 ] = 1.60E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 43 ] = 1.70E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 44 ] = 1.80E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 45 ] = 1.90E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 46 ] = 2.00E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 47 ] = 2.50E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 48 ] = 3.00E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 49 ] = 3.50E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 50 ] = 4.00E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 51 ] = 4.50E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 52 ] = 5.00E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 53 ] = 6.00E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 54 ] = 7.00E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 55 ] = 8.00E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 56 ] = 9.00E+3;
   e_bin_of_neutron_flux2dose_conversion_factor [ 57 ] = 1.00E+4;
   e_bin_of_neutron_flux2dose_conversion_factor [ 58 ] = 1.20E+4;
   e_bin_of_neutron_flux2dose_conversion_factor [ 59 ] = 1.40E+4;
   e_bin_of_neutron_flux2dose_conversion_factor [ 60 ] = 1.60E+4;
   e_bin_of_neutron_flux2dose_conversion_factor [ 61 ] = 1.80E+4;
   e_bin_of_neutron_flux2dose_conversion_factor [ 62 ] = 2.00E+4;
   e_bin_of_neutron_flux2dose_conversion_factor [ 63 ] = 3.00E+4;
   e_bin_of_neutron_flux2dose_conversion_factor [ 64 ] = 4.00E+4;
   e_bin_of_neutron_flux2dose_conversion_factor [ 65 ] = 5.00E+4;
   e_bin_of_neutron_flux2dose_conversion_factor [ 66 ] = 6.00E+4;
   e_bin_of_neutron_flux2dose_conversion_factor [ 67 ] = 7.00E+4;
   e_bin_of_neutron_flux2dose_conversion_factor [ 68 ] = 8.00E+4;
   e_bin_of_neutron_flux2dose_conversion_factor [ 69 ] = 9.00E+4;
   e_bin_of_neutron_flux2dose_conversion_factor [ 70 ] = 1.00E+5;
   e_bin_of_neutron_flux2dose_conversion_factor [ 71 ] = 1.10E+5; // For Puch Through Neutron 100 GeV

   neutron_flux2dose_conversion_factor.resize( 72 );
// Neutron flux-to-dose conversion factor[(Sv/hr)/(n/sec/cm2)]
   neutron_flux2dose_conversion_factor [  0 ] = 0.00E-0;
   neutron_flux2dose_conversion_factor [  1 ] = 1.52E-6;
   neutron_flux2dose_conversion_factor [  2 ] = 1.53E-6;
   neutron_flux2dose_conversion_factor [  3 ] = 1.54E-6;
   neutron_flux2dose_conversion_factor [  4 ] = 1.55E-6;
   neutron_flux2dose_conversion_factor [  5 ] = 1.56E-6;
   neutron_flux2dose_conversion_factor [  6 ] = 1.57E-6;
   neutron_flux2dose_conversion_factor [  7 ] = 1.58E-6;
   neutron_flux2dose_conversion_factor [  8 ] = 1.59E-6;
   neutron_flux2dose_conversion_factor [  9 ] = 1.60E-6;
   neutron_flux2dose_conversion_factor [ 10 ] = 1.61E-6;
   neutron_flux2dose_conversion_factor [ 11 ] = 1.62E-6;
   neutron_flux2dose_conversion_factor [ 12 ] = 1.63E-6;
   neutron_flux2dose_conversion_factor [ 13 ] = 1.64E-6;
   neutron_flux2dose_conversion_factor [ 14 ] = 1.65E-6;
   neutron_flux2dose_conversion_factor [ 15 ] = 1.67E-6;
   neutron_flux2dose_conversion_factor [ 16 ] = 1.68E-6;
   neutron_flux2dose_conversion_factor [ 17 ] = 1.70E-6;
   neutron_flux2dose_conversion_factor [ 18 ] = 1.72E-6;
   neutron_flux2dose_conversion_factor [ 19 ] = 1.74E-6;
   neutron_flux2dose_conversion_factor [ 20 ] = 1.77E-6;
   neutron_flux2dose_conversion_factor [ 21 ] = 1.79E-6;
   neutron_flux2dose_conversion_factor [ 22 ] = 1.82E-6;
   neutron_flux2dose_conversion_factor [ 23 ] = 1.86E-6;
   neutron_flux2dose_conversion_factor [ 24 ] = 1.93E-6;
   neutron_flux2dose_conversion_factor [ 25 ] = 1.99E-6;
   neutron_flux2dose_conversion_factor [ 26 ] = 2.05E-6;
   neutron_flux2dose_conversion_factor [ 27 ] = 2.10E-6;
   neutron_flux2dose_conversion_factor [ 28 ] = 2.15E-6;
   neutron_flux2dose_conversion_factor [ 29 ] = 2.20E-6;
   neutron_flux2dose_conversion_factor [ 30 ] = 2.25E-6;
   neutron_flux2dose_conversion_factor [ 31 ] = 2.72E-6;
   neutron_flux2dose_conversion_factor [ 32 ] = 3.26E-6;
   neutron_flux2dose_conversion_factor [ 33 ] = 3.78E-6;
   neutron_flux2dose_conversion_factor [ 34 ] = 4.18E-6;
   neutron_flux2dose_conversion_factor [ 35 ] = 4.47E-6;
   neutron_flux2dose_conversion_factor [ 36 ] = 4.72E-6;
   neutron_flux2dose_conversion_factor [ 37 ] = 4.94E-6;
   neutron_flux2dose_conversion_factor [ 38 ] = 5.14E-6;
   neutron_flux2dose_conversion_factor [ 39 ] = 5.34E-6;
   neutron_flux2dose_conversion_factor [ 40 ] = 5.52E-6;
   neutron_flux2dose_conversion_factor [ 41 ] = 5.69E-6;
   neutron_flux2dose_conversion_factor [ 42 ] = 5.84E-6;
   neutron_flux2dose_conversion_factor [ 43 ] = 5.98E-6;
   neutron_flux2dose_conversion_factor [ 44 ] = 6.11E-6;
   neutron_flux2dose_conversion_factor [ 45 ] = 6.22E-6;
   neutron_flux2dose_conversion_factor [ 46 ] = 6.32E-6;
   neutron_flux2dose_conversion_factor [ 47 ] = 6.72E-6;
   neutron_flux2dose_conversion_factor [ 48 ] = 7.02E-6;
   neutron_flux2dose_conversion_factor [ 49 ] = 7.41E-6;
   neutron_flux2dose_conversion_factor [ 50 ] = 6.77E-6;
   neutron_flux2dose_conversion_factor [ 51 ] = 8.11E-6;
   neutron_flux2dose_conversion_factor [ 52 ] = 8.42E-6;
   neutron_flux2dose_conversion_factor [ 53 ] = 9.01E-6;
   neutron_flux2dose_conversion_factor [ 54 ] = 9.55E-6;
   neutron_flux2dose_conversion_factor [ 55 ] = 1.00E-5;
   neutron_flux2dose_conversion_factor [ 56 ] = 1.05E-5;
   neutron_flux2dose_conversion_factor [ 57 ] = 1.09E-5;
   neutron_flux2dose_conversion_factor [ 58 ] = 1.17E-5;
   neutron_flux2dose_conversion_factor [ 59 ] = 1.24E-5;
   neutron_flux2dose_conversion_factor [ 60 ] = 1.30E-5;
   neutron_flux2dose_conversion_factor [ 61 ] = 1.35E-5;
   neutron_flux2dose_conversion_factor [ 62 ] = 1.40E-5;
   neutron_flux2dose_conversion_factor [ 63 ] = 1.58E-5;
   neutron_flux2dose_conversion_factor [ 64 ] = 1.78E-5;
   neutron_flux2dose_conversion_factor [ 65 ] = 1.85E-5;
   neutron_flux2dose_conversion_factor [ 66 ] = 1.90E-5;
   neutron_flux2dose_conversion_factor [ 67 ] = 1.93E-5;
   neutron_flux2dose_conversion_factor [ 68 ] = 1.93E-5;
   neutron_flux2dose_conversion_factor [ 69 ] = 1.96E-5;
   neutron_flux2dose_conversion_factor [ 70 ] = 1.98E-5;
   neutron_flux2dose_conversion_factor [ 71 ] = 1.98E-5;   // For Punch Through Neutron 100 GeV

}



G4int ExN03RunAction::search_bin_no_neutron_flux2dose ( G4double energy )
{
    G4int i = 0;
    std::vector<G4double>::iterator iv;
    for ( iv = e_bin_of_neutron_flux2dose_conversion_factor.begin() ; iv != e_bin_of_neutron_flux2dose_conversion_factor.end() ; iv++ )
    {
        if ( ( energy - *iv ) < 0.0 )
           return i;
        i++;
    };
    return -1;
}


G4int ExN03RunAction::search_bin_no_neutron_activation_XS ( G4double energy )
{
    G4int i = 0;
    std::vector<G4double>::iterator iv;
    for ( iv = e_bin_of_neutron_activation_XS.begin() ; iv != e_bin_of_neutron_activation_XS.end() ; iv++ )
    {
        if ( ( energy - *iv ) < 0.0 )
           return i;
        i++;
    };
    return -1;
}

void ExN03RunAction::AddNeutron2 ( G4int no , G4int th , G4double e , G4double mu , G4double w )
{
   neutron_data* aData = new neutron_data(); // will be delete EndOfRunAction
   aData->event = no;
   aData->layer = th - 1;
   aData->ke = e;
   aData->mu = mu;
   aData->weight = w;
   v_neutron_data2.push_back ( aData );
}



void ExN03RunAction::AddEvent ()
{

//080111
//BNL

   std::vector < G4double > v_event_dose;
   //v_event_dose.resize ( detector->GetNbOfLayers() );
   v_event_dose.resize ( 14 );

   std::vector< neutron_data* >::iterator it; 
   for ( it = v_neutron_data2.begin() ; it !=  v_neutron_data2.end() ; it++ ) 
   {

      G4int ich = search_bin_no_neutron_activation_XS ( (*it)->ke );

      G4double d = 1 / std::abs( (*it)->mu );
      if ( d > 8 ) d = 8;

      G4double x = (*it)->weight * d;

      v_ar_204Bi [ (*it)->layer ] += ( x * neutron_activation_XS_204Bi [ ich ] ); 
      v_ar_206Bi [ (*it)->layer ] += ( x * neutron_activation_XS_206Bi [ ich ] ); 

   }

   for ( G4int j = 0 ; j < detector->GetNbOfLayers() ; j++ ) 
   {
      //v_neutron_dose_tally [ j ] -> AddScore ( v_event_dose [ j ] ); 
#ifndef G4V93OROLD
      v_neutron_dose_convergence [ j ] -> AddScore ( v_event_dose [ j ] ); 
#endif
   }

   for ( it = v_neutron_data2.begin() ; it !=  v_neutron_data2.end() ; it++ ) 
      delete *it;

   v_neutron_data2.clear();
//080111
}


//071207
void ExN03RunAction::SetSensitive(){

  //  -------------------------------------------------
  //   The collection names of defined Primitives are
  //   0       ConcreteSD/Collisions
  //   1       ConcreteSD/CollWeight
  //   2       ConcreteSD/Population
  //   3       ConcreteSD/TrackEnter
  //   4       ConcreteSD/SL
  //   5       ConcreteSD/SLW
  //   6       ConcreteSD/SLWE
  //   7       ConcreteSD/SLW_V
  //   8       ConcreteSD/SLWE_V
  //  -------------------------------------------------

  //================================================
  // Sensitive detectors : MultiFunctionalDetector
  //================================================
  //
  //  Sensitive Detector Manager.
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  //
  // Sensitive Detector Name
  G4String concreteSDname = "ConcreteSD";
  
  //------------------------
  // MultiFunctionalDetector
  //------------------------
  //
  // Define MultiFunctionalDetector with name.
  //G4MultiFunctionalDetector* MFDet = new G4MultiFunctionalDetector(concreteSDname);
  MFDet = new G4MultiFunctionalDetector(concreteSDname);
  SDman->AddNewDetector( MFDet );                 // Register SD to SDManager
  
  G4String fltName,particleName;
  G4SDParticleFilter* neutronFilter =
      new G4SDParticleFilter(fltName="neutronFilter", particleName="neutron");
  
  MFDet->SetFilter(neutronFilter);
  
  G4String psName;
  G4PSNofCollision*   scorer0 = new G4PSNofCollision(psName="Collisions");  
  MFDet->RegisterPrimitive(scorer0);


  G4PSNofCollision*   scorer1 = new G4PSNofCollision(psName="CollWeight");  
  scorer1->Weighted(true);
  MFDet->RegisterPrimitive(scorer1);


  G4PSPopulation*   scorer2 = new G4PSPopulation(psName="Population");  
  MFDet->RegisterPrimitive(scorer2);

  G4PSTrackCounter* scorer3 = new G4PSTrackCounter(psName="TrackEnter",fCurrent_In);  
  MFDet->RegisterPrimitive(scorer3);

  G4PSTrackLength* scorer4 = new G4PSTrackLength(psName="SL");  
  MFDet->RegisterPrimitive(scorer4);

  G4PSTrackLength* scorer5 = new G4PSTrackLength(psName="SLW");  
  scorer5->Weighted(true);
  MFDet->RegisterPrimitive(scorer5);

  G4PSTrackLength* scorer6 = new G4PSTrackLength(psName="SLWE");
  scorer6->Weighted(true);
  scorer6->MultiplyKineticEnergy(true);
  MFDet->RegisterPrimitive(scorer6);

  G4PSTrackLength* scorer7 = new G4PSTrackLength(psName="SLW_V");
  scorer7->Weighted(true);
  scorer7->DivideByVelocity(true);
  MFDet->RegisterPrimitive(scorer7);

  G4PSTrackLength* scorer8 = new G4PSTrackLength(psName="SLWE_V");
  scorer8->Weighted(true);
  scorer8->MultiplyKineticEnergy(true);
  scorer8->DivideByVelocity(true);
  MFDet->RegisterPrimitive(scorer8);

  std::cout << " SetSensitive end" << std::endl;

}
//071207

//071210
void ExN03RunAction::PrintHeader(std::ostream *out)
{
  std::vector<G4String> vecScoreName;
  vecScoreName.push_back("Tr.Entering");
  vecScoreName.push_back("Population");
  vecScoreName.push_back("Collisions");
  vecScoreName.push_back("Coll*WGT");
  vecScoreName.push_back("NumWGTedE");
  vecScoreName.push_back("FluxWGTedE");
  vecScoreName.push_back("Av.Tr.WGT");
  vecScoreName.push_back("SL");
  vecScoreName.push_back("SLW");
  vecScoreName.push_back("SLW_v");
  vecScoreName.push_back("SLWE");
  vecScoreName.push_back("SLWE_v");

  // head line
  //std::string vname = FillString("Volume", ' ', FieldName+1);
  //*out << vname << '|';
  std::string vname;
  *out << std::setw(FieldValue) << "Volume" << " |";
  for (std::vector<G4String>::iterator it = vecScoreName.begin();
       it != vecScoreName.end(); it++) {
      //vname = FillString((*it),
//                     ' ',
//                     FieldValue+1,
//                     false);
//    *out << vname << '|';
      *out << std::setw(FieldValue) << (*it) << " |";
  }
  *out << G4endl;
}

std::string ExN03RunAction::FillString(const std::string &name,
                                       char c, G4int n, G4bool back)
{
  std::string fname("");
  G4int k = n - name.size();
  if (k > 0) {
    if (back) {
      fname = name;
      fname += std::string(k,c);
    }
    else {
      fname = std::string(k,c);
      fname += name;
    }
  }
  else {
    fname = name;
  }
  return fname;
}

G4String ExN03RunAction::GetLayerName(G4int i)
{
  std::ostringstream os;
  os << "Shield";
  if (i<10)
  {
    os << "0";
  }
  os << i ;
  G4String name = os.str();
  return name;
}
//071210

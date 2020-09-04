#ifndef ExN03RunAction_h
#define ExN03RunAction_h 1

#include "ExN03DetectorConstruction.hh"

#include "G4UserRunAction.hh"
#include "globals.hh"

#include <vector>
#include <fstream>

//#include "TKTally.hh"
#ifndef G4V93OROLD
#include "G4ConvergenceTester.hh"
#endif

//071207
/*
#include "G4Scorer.hh"
#include "G4MassGeometrySampler.hh"
*/
#include "G4GeometrySampler.hh"
//071207
#include "G4IStore.hh"
//071210
#include "B01Run.hh"
#include "G4MultiFunctionalDetector.hh"
//071210
#ifdef G4ANALYSIS_USE
#include "AIDA/AIDA.h"
using namespace AIDA;
#include "A01AnalysisManager.hh"
#endif // G4ANALYSIS_USE


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;

struct neutron_data 
{
   G4int event , layer;
   G4double ke , mu , weight;
};


class ExN03RunAction : public G4UserRunAction
{
  public:
    ExN03RunAction();
    ExN03RunAction( ExN03DetectorConstruction* , G4String );
   ~ExN03RunAction();

   public:

//071210
      virtual G4Run* GenerateRun();
      void PrintHeader(std::ostream *out);
      std::string FillString(const std::string &name, char c, G4int n, G4bool back=true);
//071210

    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);

   public:
      // void AddNeutron( G4int i , G4double ke ) { v_neutron_data.push_back( new std::pair< G4int , G4double >( i , ke ) ); }; 
      void AddNeutron( G4int i , G4double ke , G4double weight ) { v_neutron_data.push_back ( new std::pair< G4int , std::pair < G4double , G4double >* > ( i , new std::pair < G4double , G4double > ( ke , weight ) ) ); }; 
//                      i-event  i-shield ke         mu         weight  
      void AddNeutron2 ( G4int ,  G4int , G4double , G4double , G4double ); 
      void AddEvent (); 

   private:

//071207
    void SetSensitive();
//071207

      // std::vector < std::pair < G4int , G4double >* > v_neutron_data;  
      std::vector < std::pair < G4int , std::pair < G4double , G4double >* >* > v_neutron_data;  
      //                        LAYER ,                KE    ,  WEIGHT

      std::vector < neutron_data* > v_neutron_data2;  

      void setLogBins( G4int , G4int , G4int bin_per_order = 10 );
      std::vector<G4double> v_energy_ch;
      G4int search_bin_no ( G4double );

      std::vector < std::vector< G4double >* > v_neutron_spectra;  

      std::vector < G4double > e_bin_of_neutron_flux2dose_conversion_factor;
      std::vector < G4double > neutron_flux2dose_conversion_factor;

//080111
//BNL
      std::vector < G4double > e_bin_of_neutron_activation_XS;
      std::vector < G4double > neutron_activation_XS_206Bi;
      std::vector < G4double > neutron_activation_XS_204Bi;
      void init_neutron_activation_XS();

      std::vector < G4double > v_ar_204Bi;  
      std::vector < G4double > v_ar_206Bi;  
      G4int search_bin_no_neutron_activation_XS ( G4double );
//080111

      G4int search_bin_no_neutron_flux2dose ( G4double );
      void init_neutron_flux2dose_conversion_factor();

      std::vector < G4double > v_neutron_dose;  

   private:
      ExN03DetectorConstruction* detector;

      G4IStore* istore;
//071207
/*
      G4Scorer* scorer;
      G4MassGeometrySampler* mgs;
*/
      G4GeometrySampler* mgs;
//071207
 
//      std::vector < TKTally* >  v_neutron_dose_tally;

#ifndef G4V93OROLD
      std::vector < G4ConvergenceTester* >  v_neutron_dose_convergence;
#endif

      std::ofstream* ofile;

//071210
   std::vector< G4String > theSDName;
   G4int FieldName;
   G4int FieldValue;
   G4MultiFunctionalDetector* MFDet;

   G4String GetLayerName( G4int );
//071210

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


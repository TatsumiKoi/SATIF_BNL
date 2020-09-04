#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4UI_USE_XM
#include "G4UIXm.hh"
#endif

#ifdef G4UI_USE_WIN32
#include "G4UIWin32.hh"
#endif

#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "ExN03DetectorConstruction.hh"

#include "ExN03PhysicsList.hh"
//LHEP no more exist since v10
//#include "LHEP.hh"
//#include "LCPhysicsList.hh"
// Hadronic Lists
#ifndef G4V91OROLD
#include "G4PhysListFactory.hh"
#endif

#ifdef G4V91OROLD

#ifndef G4V90OROLD
#include "FTFP_BERT.hh"
#include "FTF_BIC.hh"
#include "QGSC_BERT.hh"
#include "QGSP_BERT_DIF.hh"
#include "QGSP_DIF.hh"
#include "QGS_BIC.hh"
#endif

/*
> FTFP_BERT
> FTF_BIC
> QGSC_BERT
> QGSP_BERT_DIF
> QGSP_DIF
> QGS_BIC
*/
#include "FTFC.hh"
#include "FTFP.hh"
#include "FTFP_EMV.hh"
#include "LBE.hh"
#include "LHEP.hh"
#include "LHEP_BERT.hh"
#include "LHEP_BERT_HP.hh"
#include "LHEP_EMV.hh"
#include "LHEP_PRECO_HP.hh"
#include "QBBC.hh"
#include "QGSC.hh"
#include "QGSC_EFLOW.hh"
#include "QGSC_EMV.hh"
#include "QGSP.hh"
#include "QGSP_BERT.hh"
#include "QGSP_BERT_EMV.hh"
#include "QGSP_BERT_HP.hh"
#include "QGSP_BERT_NQE.hh"
#include "QGSP_BERT_TRV.hh"
#include "QGSP_BIC.hh"
#include "QGSP_BIC_HP.hh"
#include "QGSP_EMV.hh"
#include "QGSP_EMV_NQE.hh"
#include "QGSP_EMX.hh"
#include "QGSP_NQE.hh"
#include "QGSP_QEL.hh"
#endif

//080108
#include "G4ProcessManager.hh"
#include "G4Neutron.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4NeutronHPJENDLHEInelasticData.hh"
#include "G4BGGNucleonInelasticXS.hh"
#include "G4ParticleTable.hh"

#include "ExN03PrimaryGeneratorAction.hh"
#include "ExN03RunAction.hh"
#include "ExN03EventAction.hh"
#include "ExN03SteppingAction.hh"
#include "ExN03SteppingVerbose.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv) {

  // choose the Random engine
  //CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
  
  //my Verbose output class
  G4VSteppingVerbose::SetInstance(new ExN03SteppingVerbose);
     
  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  // set mandatory initialization classes
  ExN03DetectorConstruction* detector = new ExN03DetectorConstruction;
  runManager->SetUserInitialization(detector);
  //runManager->SetUserInitialization(new ExN03PhysicsList);
  //runManager->SetUserInitialization(new LHEP);

   G4String pl;
   if ( argc > 2 ) pl = argv[2];
   G4String jendl;
   if ( argc > 3 ) jendl = argv[3];
   G4String nrun;
   if ( argc > 4 ) nrun = argv[4];

#ifndef G4V91OROLD
    G4PhysListFactory factory;
    //Dump name of physics list 
    std::vector<G4String> list_of_PL = factory.AvailablePhysLists();
    for ( std::vector<G4String>::iterator 
          it = list_of_PL.begin() ;  it != list_of_PL.end() ; it++ ) {  
        G4cout << *it << G4endl;
    }
    runManager->SetUserInitialization( factory.GetReferencePhysList(pl) );
#endif

#ifdef G4V91OROLD
#ifndef G4V90OROLD
if ( pl == "FTFP_BERT" ) {
   runManager->SetUserInitialization( new FTFP_BERT );
} else if ( pl == "FTF_BIC" ) {
   runManager->SetUserInitialization( new FTF_BIC );
} else if ( pl == "QGSC_BERT" ) {
   runManager->SetUserInitialization( new QGSC_BERT );
} else if ( pl == "QGSP_BERT_DIF") {
   runManager->SetUserInitialization( new QGSP_BERT_DIF );
} else if ( pl == "QGSP_DIF" ) {
   runManager->SetUserInitialization( new QGSP_DIF );
} else if ( pl == "QGS_BIC" ) {
   runManager->SetUserInitialization( new QGS_BIC );
}
#endif
if ( pl == "FTFC" ) {
   runManager->SetUserInitialization( new FTFC );
} else if ( pl == "FTFP") {
   runManager->SetUserInitialization( new FTFP );
} else if ( pl == "FTFP_EMV" ) {
   runManager->SetUserInitialization( new FTFP_EMV );
} else if ( pl == "LBE" ) {
   runManager->SetUserInitialization( new LBE );
} else if ( pl == "LHEP" ) {
   runManager->SetUserInitialization( new LHEP );
} else if ( pl == "LHEP_BERT") {
   runManager->SetUserInitialization( new LHEP_BERT );
} else if ( pl == "LHEP_BERT_HP" ) {
   runManager->SetUserInitialization( new LHEP_BERT_HP );
} else if ( pl == "LHEP_EMV" ) {
   runManager->SetUserInitialization( new LHEP_EMV );
} else if ( pl == "LHEP_PRECO_HP" ) {
   runManager->SetUserInitialization( new LHEP_PRECO_HP );
} else if ( pl == "QBBC" ) {
   runManager->SetUserInitialization( new QBBC );
} else if ( pl == "QGSC" ) {
   runManager->SetUserInitialization( new QGSC );
} else if ( pl == "QGSC_EFLOW" ) {
   runManager->SetUserInitialization( new QGSC_EFLOW );
} else if ( pl == "QGSC_EMV" ) {
   runManager->SetUserInitialization( new QGSC_EMV );
} else if ( pl == "QGSP") {
   runManager->SetUserInitialization( new QGSP ); 
} else if ( pl == "QGSP_BERT" ) {
   runManager->SetUserInitialization( new QGSP_BERT );
} else if ( pl == "QGSP_BERT_EMV") {
   runManager->SetUserInitialization( new QGSP_BERT_EMV);
} else if ( pl == "QGSP_BERT_HP" ) {
   runManager->SetUserInitialization( new QGSP_BERT_HP );
} else if ( pl == "QGSP_BERT_NQE" ) {
   runManager->SetUserInitialization( new QGSP_BERT_NQE );
} else if ( pl == "QGSP_BERT_TRV" ) {
   runManager->SetUserInitialization( new QGSP_BERT_TRV );
} else if ( pl == "QGSP_BIC" ) {
   runManager->SetUserInitialization( new QGSP_BIC );
} else if ( pl == "QGSP_BIC_HP" ) {
   runManager->SetUserInitialization( new QGSP_BIC_HP );
} else if ( pl == "QGSP_EMV") {
   runManager->SetUserInitialization( new QGSP_EMV_NQE);
} else if ( pl == "QGSP_EMX" ) {
   runManager->SetUserInitialization( new QGSP_EMX );
} else if ( pl == "QGSP_NQE" ) {
   runManager->SetUserInitialization( new QGSP_NQE );
} else if ( pl == "QGSP_QEL" ) {
   runManager->SetUserInitialization( new QGSP_QEL );
}
#endif

//

   
   

 G4UIsession* session=0;
  
  if (argc==1)   // Define UI session for interactive mode.
    {
      // G4UIterminal is a (dumb) terminal.
#if defined(G4UI_USE_XM)
      session = new G4UIXm(argc,argv);
#elif defined(G4UI_USE_WIN32)
      session = new G4UIWin32();
#elif defined(G4UI_USE_TCSH)
      session = new G4UIterminal(new G4UItcsh);      
#else
      session = new G4UIterminal();
#endif
    }
  
#ifdef G4VIS_USE
  // visualization manager
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif
    
  // set user action classes
  runManager->SetUserAction(new ExN03PrimaryGeneratorAction(detector));
  //runManager->SetUserAction(new ExN03RunAction);
  //ExN03EventAction* eventaction = new ExN03EventAction;
  //ExN03RunAction* runaction = new ExN03RunAction;
  //ExN03RunAction* runaction = new ExN03RunAction ( detector );
  ExN03RunAction* runaction = new ExN03RunAction ( detector , pl );
  runManager->SetUserAction( runaction );
  ExN03EventAction* eventaction = new ExN03EventAction ( runaction );
  runManager->SetUserAction(eventaction);
  runManager->SetUserAction(new ExN03SteppingAction(detector, eventaction));
  
  //Initialize G4 kernel
  runManager->Initialize();


// 080108

   if ( jendl == "JENDL" || jendl == "BARASHENKOV" || jendl == "JB" )
   {
      std::cout << "Adding JENDL Corss Section " << std::endl;
      G4ParticleTable* pt = G4ParticleTable::GetParticleTable();
      std::cout << "Got Particle Table " << std::endl;
      //G4ProcessManager* pm = pt->FindParticle( G4Neutron::Neutron() )->GetProcessManager();
      G4ProcessManager* pm = pt->FindParticle( "neutron" )->GetProcessManager();
      std::cout << "Got ProcessManager  " << std::endl;
      std::cout << pt << std::endl;
      std::cout <<  pt->FindParticle( G4Neutron::Neutron() ) << std::endl;
      std::cout << pt->FindParticle( "neutron" ) << std::endl;
      std::cout << (pt->FindParticle( "neutron" ) )->GetProcessManager() << std::endl;
      std::cout << pm << std::endl;
      G4ProcessVector* pv = pm->GetProcessList();  
      std::cout <<  pm->GetProcessList() << std::endl;
      for ( G4int ip = 0 ; ip < (G4int)pv->size() ; ip++ )
      {
         std::cout << (*pv)[ip]->GetProcessName() << std::endl;
         if ( (*pv)[ip]->GetProcessName() == "NeutronInelastic" )
         {
            if ( jendl == "JENDL" ) 
               ((G4NeutronInelasticProcess*)(*pv)[ip])->AddDataSet( new G4NeutronHPJENDLHEInelasticData() ); 
            else if ( jendl == "BARASHENKOV" )
               ((G4NeutronInelasticProcess*)(*pv)[ip])->AddDataSet( new G4BGGNucleonInelasticXS( G4Neutron::Neutron() ) );
            else if ( jendl == "JB" )
            {
               ((G4NeutronInelasticProcess*)(*pv)[ip])->AddDataSet( new G4BGGNucleonInelasticXS( G4Neutron::Neutron() ) );
               ((G4NeutronInelasticProcess*)(*pv)[ip])->AddDataSet( new G4NeutronHPJENDLHEInelasticData() ); 
            }          
         }
      }
   }

  // get the pointer to the User Interface manager 
  G4UImanager* UI = G4UImanager::GetUIpointer();  
  G4String rand_seed("/random/resetEngineFrom randseed_"); 
  rand_seed += nrun;
  rand_seed += ".rndm";

      //UI->ApplyCommand("/random/resetEngineFrom randseed_0.rndm");
      UI->ApplyCommand(rand_seed);

  if (session)   // Define UI session for interactive mode.
    {
      // G4UIterminal is a (dumb) terminal.
      UI->ApplyCommand("/control/execute vis.mac");    
#if defined(G4UI_USE_XM) || defined(G4UI_USE_WIN32)
      // Customize the G4UIXm,Win32 menubar with a macro file :
      UI->ApplyCommand("/control/execute visTutor/gui.mac");
#endif
      session->SessionStart();
      delete session;
    }
  else           // Batch mode
    { 
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI->ApplyCommand(command+fileName);
    }

  // job termination
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

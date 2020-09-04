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
// $Id: ExN03EventAction.hh,v 1.8 2003/09/15 15:38:14 maire Exp $
// GEANT4 tag $Name: geant4-07-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ExN03EventAction_h
#define ExN03EventAction_h 1

#include "ExN03RunAction.hh"

#include "G4UserEventAction.hh"
#include "globals.hh"

class ExN03EventActionMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN03EventAction : public G4UserEventAction
{
 public:
   ExN03EventAction();
    ExN03EventAction( ExN03RunAction* );
  ~ExN03EventAction();

 public:
   void  BeginOfEventAction(const G4Event*);
   void    EndOfEventAction(const G4Event*);
    
   void AddAbs(G4double de, G4double dl) {EnergyAbs += de; TrackLAbs += dl;};
   void AddGap(G4double de, G4double dl) {EnergyGap += de; TrackLGap += dl;};
                     
   void SetDrawFlag   (G4String val)  {drawFlag = val;};
   void SetPrintModulo(G4int    val)  {printModulo = val;};
    
 private:
   G4double  EnergyAbs, EnergyGap;
   G4double  TrackLAbs, TrackLGap;
                     
   G4String  drawFlag;
   G4int     printModulo;
                             
   G4int     no;

   ExN03RunAction*  runAction;

   ExN03EventActionMessenger*  eventMessenger;

   public:
      //void AddNeutron ( G4int i , G4double ke ) { runAction->AddNeutron( i , ke ); };  
      //void AddNeutron ( G4int i , G4double ke , G4double weight = 1.0 ) { runAction->AddNeutron( i , ke , weight ); };  
      void AddNeutron ( G4int i , G4double ke , G4double mu , G4double weight = 1.0 ) { 
                                                                          //runAction->AddNeutron( i , ke , weight ); 
                                                                          runAction->AddNeutron2( no , i , ke , mu , weight ); 
                                                                        };  
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    

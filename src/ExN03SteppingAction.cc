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
// $Id: ExN03SteppingAction.cc,v 1.9 2005/02/02 17:11:11 maire Exp $
// GEANT4 tag $Name: geant4-07-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN03SteppingAction.hh"

#include "ExN03DetectorConstruction.hh"
#include "ExN03EventAction.hh"

#include "G4SystemOfUnits.hh"

#include "G4Track.hh"

////#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN03SteppingAction::ExN03SteppingAction(ExN03DetectorConstruction* det,
                                         ExN03EventAction* evt)
:detector(det), eventaction(evt)					 
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN03SteppingAction::~ExN03SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN03SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  const G4Track* track = aStep->GetTrack();
  G4VPhysicalVolume* volume = track->GetVolume();
  
  // collect energy and track length step by step
  G4double edep = aStep->GetTotalEnergyDeposit();
  
  G4double stepl = 0.;
  if (track->GetDefinition()->GetPDGCharge() != 0.)
    stepl = aStep->GetStepLength();
      
  if (volume == detector->GetAbsorber()) eventaction->AddAbs(edep,stepl);
  if (volume == detector->GetGap())      eventaction->AddGap(edep,stepl);
       
 // save the random number seed of this event, under condition
 //// if(condition) G4RunManager::GetRunManager()->rndmSaveThisEvent();

//080110
//BNL
   if ( aStep->GetPostStepPoint()->GetKineticEnergy() < 10*MeV )
   {
      aStep->GetTrack()->SetTrackStatus( fStopAndKill ); 
   }

   if ( aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary && std::abs ( aStep->GetPostStepPoint()->GetPosition().z() ) < 4*cm && aStep->GetPostStepPoint()->GetKineticEnergy()/MeV > 20*MeV )
   {
      if ( volume->GetName() != "TargetS" && volume->GetName() != "World" && volume->GetName() != "OuterShield" && volume->GetName() != "TargetSCase" )
      {
      if ( track->GetDefinition()->GetParticleName() == "neutron" ) 
      {

         G4ThreeVector rd ( aStep->GetPostStepPoint()->GetPosition().x() , aStep->GetPostStepPoint()->GetPosition().y() , 0 ); 

         G4double mu = ( rd.unit() ).dot ( aStep->GetPostStepPoint()->GetMomentumDirection() );

         //G4int i = 0; 
         //if ( mu < 0 ) i=-1; 
/*
         std::cout << volume->GetName() 
                   << " " << volume->GetCopyNo()+1
                   << " " << track->GetDefinition()->GetParticleName() 
                   << " " << aStep->GetPostStepPoint()->GetKineticEnergy()/MeV 
                   << " " << aStep->GetPostStepPoint()->GetPosition()/cm 
                   << " " << aStep->GetPostStepPoint()->GetMomentumDirection() 
                   << " " << aStep->GetPreStepPoint()->GetWeight() 
                   << " " << rd.unit()
                   << " " << mu
                   << " " << rd.mag()
                   << std::endl;
*/

         G4int iShield = volume->GetCopyNo()+1 ;  
         if ( iShield > 0 )  
         {
             eventaction->AddNeutron( iShield , aStep->GetPreStepPoint()->GetKineticEnergy() , mu , 
                                      aStep->GetPreStepPoint()->GetWeight() );
         }
      }
      }

   }
   if ( track->GetDefinition()->GetParticleName() == "neutron" && volume->GetName() == "Layer" )
   {
/*
      if ( aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary  && aStep->GetPostStepPoint()->GetKineticEnergy()/MeV > 20*MeV )
      {
         G4int i=0; 
         if ( aStep->GetPostStepPoint()->GetMomentumDirection().x() < 0.0 ) i = 1;   

         if ( aStep->GetPostStepPoint()->GetTouchable()->GetCopyNumber( 0 ) != aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber( 0 ) ) 
              eventaction->AddNeutron( aStep->GetPostStepPoint()->GetTouchable()->GetCopyNumber( 0 ) + i , aStep->GetPostStepPoint()->GetKineticEnergy() ,  
                                     aStep->GetPreStepPoint()->GetWeight() );  // Useual case 
         else if ( aStep->GetPostStepPoint()->GetTouchable()->GetCopyNumber( 0 ) == 0 ) 
            eventaction->AddNeutron( 0 , aStep->GetPostStepPoint()->GetKineticEnergy() ,  
                                     aStep->GetPreStepPoint()->GetWeight() );  // Escape from copy No 0
      }
*/
//080110
/*
      if ( aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary  && aStep->GetPreStepPoint()->GetTouchable()->GetHistoryDepth() > 0 && aStep->GetPreStepPoint()->GetKineticEnergy() > 20*MeV )
      {
         G4int i=0; 
         if ( aStep->GetPreStepPoint()->GetMomentumDirection().x() < 0.0 ) i = 1;   

         std::cout 
            << "MOTOKO "  
                   << aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber( 0 ) + i 
            << " " << aStep->GetPreStepPoint()->GetKineticEnergy()/MeV 
            << " " << aStep->GetPreStepPoint()->GetPosition() 
            << " " << aStep->GetPreStepPoint()->GetMomentumDirection() 
            << " " << aStep->GetPreStepPoint()->GetWeight() 
            << std::endl;

         //eventaction->AddNeutron( aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber( 0 ) + i , aStep->GetPreStepPoint()->GetKineticEnergy() );
         eventaction->AddNeutron( aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber( 0 ) + i , aStep->GetPreStepPoint()->GetKineticEnergy() ,  
                                  aStep->GetPreStepPoint()->GetWeight() );
      }
*/
      //if ( aStep->GetPostStepPoint()->GetKineticEnergy() < 100*keV )
      if ( aStep->GetPostStepPoint()->GetKineticEnergy() < 10000*keV )
      {
         aStep->GetTrack()->SetTrackStatus( fStopAndKill ); 
      }
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




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
// $Id: ExN03PrimaryGeneratorMessenger.cc,v 1.8 2002/12/16 16:37:27 maire Exp $
// GEANT4 tag $Name: geant4-07-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN03PrimaryGeneratorMessenger.hh"

#include "ExN03PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN03PrimaryGeneratorMessenger::ExN03PrimaryGeneratorMessenger(
                                          ExN03PrimaryGeneratorAction* ExN03Gun)
:ExN03Action(ExN03Gun)
{
  gunDir = new G4UIdirectory("/N03/gun/");
  gunDir->SetGuidance("PrimaryGenerator control");
   
  RndmCmd = new G4UIcmdWithAString("/N03/gun/rndm",this);
  RndmCmd->SetGuidance("Shoot randomly the incident particle.");
  RndmCmd->SetGuidance("  Choice : on(default), off");
  RndmCmd->SetParameterName("choice",true);
  RndmCmd->SetDefaultValue("on");
  RndmCmd->SetCandidates("on off");
  RndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  gunCmd = new G4UIcmdWithAString("/N03/gun/gunType",this);
  gunCmd->SetGuidance("Select gun type.");
//  gunCmd->SetGuidance("  Choice : default, a , b , c , d");
//  gunCmd->SetParameterName("choice",true);
  gunCmd->SetDefaultValue("defalut");
  gunCmd->SetCandidates("default a b c d 200MeVpFe90 500MeVpFe90 1GeVpFe90 3GeVpFe90 5GeVpFe90 3GeVpHgM000to015 3GeVpHgM045to060 3GeVpHgM090to105 3GeVpHgM135to150 24GeVpHgM090to105 ");
  gunCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN03PrimaryGeneratorMessenger::~ExN03PrimaryGeneratorMessenger()
{
  delete gunCmd;
  delete RndmCmd;
  delete gunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN03PrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{ 
  if( command == RndmCmd )
   { ExN03Action->SetRndmFlag(newValue);}

  if( command == gunCmd )
   { ExN03Action->SetGunType( newValue );}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


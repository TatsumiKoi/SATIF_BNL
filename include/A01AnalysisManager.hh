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
// **********************************************************************
// *                                                                    *
// *                    GEANT 4 tutorial 1                              *
// *                                                                    *
// * MODULE:            A01AnalysisManager.cc                           *
// * -------                                                            *
// *                                                                    *
// * Version:           0.1                                             *
// * Date:              January 28 2002                                 *
// * Author:            T.Johnson                                       *
// * Organisation:      SLAC                                            *
// *                                                                    *
// **********************************************************************
//
// CHANGE HISTORY
// --------------
//
//
// **********************************************************************

#ifndef A01AnalysisManager_h
#define A01AnalysisManager_h 1

#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"

using namespace AIDA;

class AIDA::IAnalysisFactory;
class AIDA::ITree;
class AIDA::IHistogramFactory;
class AIDA::ITupleFactory;
class AIDA::IPlotter;

class G4Track;

class A01AnalysisManager {
public:

  virtual ~A01AnalysisManager();
  static A01AnalysisManager* getInstance();
  static A01AnalysisManager* getInstance( G4String );
  static void dispose();

  IHistogramFactory* getHistogramFactory();
  ITupleFactory* getTupleFactory();
  IPlotter* createPlotter();
  IDataPointSetFactory* getDataPointSetFactory();

private:

  A01AnalysisManager();
  A01AnalysisManager( G4String );
  static A01AnalysisManager* instance;

  G4String file_name;

  IAnalysisFactory* analysisFactory;
  IHistogramFactory* hFactory;
  ITupleFactory* tFactory;
  IDataPointSetFactory* dFactory;
  ITree* tree;
};

#endif


#ifdef G4ANALYSIS_USE

#include <fstream>

#include "G4ios.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4VVisManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"

#include <AIDA/AIDA.h>

#include "A01AnalysisManager.hh"

A01AnalysisManager* A01AnalysisManager::instance = 0;

A01AnalysisManager::A01AnalysisManager()
:analysisFactory(0), hFactory(0), tFactory(0)
{
  // Hooking an AIDA compliant analysis system.
  analysisFactory = AIDA_createAnalysisFactory();
  if(analysisFactory)
  {
    ITreeFactory* treeFactory = analysisFactory->createTreeFactory();
    tree = treeFactory->create("SATIF.aida","xml",false,true,"compress=yes");
    hFactory = analysisFactory->createHistogramFactory(*tree);
    tFactory = analysisFactory->createTupleFactory(*tree);
    dFactory = analysisFactory->createDataPointSetFactory(*tree);
    delete treeFactory; // Will not delete the ITree.
  }
}

A01AnalysisManager::A01AnalysisManager( G4String name )
:analysisFactory(0), hFactory(0), tFactory(0)
{
  // Hooking an AIDA compliant analysis system.
  analysisFactory = AIDA_createAnalysisFactory();
  if(analysisFactory)
  {
    ITreeFactory* treeFactory = analysisFactory->createTreeFactory();
    //tree = treeFactory->create("SATIF.aida","xml",false,true,"compress=yes");
    tree = treeFactory->create(name,"xml",false,true,"compress=yes");
    hFactory = analysisFactory->createHistogramFactory(*tree);
    tFactory = analysisFactory->createTupleFactory(*tree);
    dFactory = analysisFactory->createDataPointSetFactory(*tree);
    delete treeFactory; // Will not delete the ITree.
  }
}

A01AnalysisManager::~A01AnalysisManager()
{
  if (analysisFactory)
  {
    if (!tree->commit()) G4cout << "Commit failed: no AIDA file produced!" << G4endl;
    delete tree;
    delete tFactory;
    delete hFactory;
    G4cout << "Warning: Geant4 will NOT exit unless you close the JAS-AIDA window." << G4endl;
    delete analysisFactory;
  }
}
IHistogramFactory* A01AnalysisManager::getHistogramFactory()
{
  return hFactory;
}
ITupleFactory* A01AnalysisManager::getTupleFactory()
{
  return tFactory;
}
IPlotter* A01AnalysisManager::createPlotter()
{
  if (analysisFactory)
  {
    IPlotterFactory* pf = analysisFactory->createPlotterFactory(0,0);
    if (pf) return pf->create("Plotter");
  }
  return 0;
}
IDataPointSetFactory* A01AnalysisManager::getDataPointSetFactory()
{
  return dFactory;
}

A01AnalysisManager* A01AnalysisManager::getInstance()
{
  if (instance == 0) instance = new A01AnalysisManager();
  return instance;
}

A01AnalysisManager* A01AnalysisManager::getInstance( G4String name )
{
  if (instance == 0) instance = new A01AnalysisManager( name );
  return instance;
}

void A01AnalysisManager::dispose()
{
  if (instance != 0)
  {
    delete instance;
    instance = 0;
  }
}

#endif // G4ANALYSIS_USE


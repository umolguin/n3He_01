//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
/// \file XenEventAction.cc
/// \brief Implementation of the XenEventAction class

#include "XenEventAction.hh"

#include "XenRunAction.hh"
#include "XenSteppingAction.hh"
#include "XenAnalysis.hh"
  // use of stepping action to get and reset accumulated energy

#include "G4RunManager.hh"
#include "G4Event.hh"

#include "TFile.h"
#include "TH1F.h"

#include <cmath>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//XenEventAction* XenEventAction::fgInstance = 0;
//G4double XenEventAction::fEnergyAbs=0;
//G4double XenEventAction::fTrackLAbs=0;
G4double XenEventAction::originZ=0;
G4double XenEventAction::originX=0;
G4double XenEventAction::originY=0;
G4int XenEventAction::lastCellID=0;
G4double XenEventAction::lastTrackID=0;
G4double XenEventAction::accumDelta=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XenEventAction* XenEventAction::Instance()
{
// Static acces function via G4RunManager

 // return fgInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XenEventAction::XenEventAction()
: G4UserEventAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XenEventAction::~XenEventAction()
{


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XenEventAction::BeginOfEventAction(const G4Event* event)
{
    G4int eventNb = event->GetEventID();

    G4cout << "\n---> This is an event with ID: " << eventNb << G4endl;


    originX=0;
    originY=0;
    originX=0;
    lastTrackID=0;
    lastCellID=-1;
    G4cout<<"[EDEP]"<<"STARTEVENT";

}
void XenEventAction::EndOfEventAction(const G4Event* event)
{

//    // get analysis manager
//    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
//
//    // fill histograms
//    analysisManager->FillH1(1, fEnergyAbs);
//    G4cout << "Energy ABS: "<<fEnergyAbs << G4endl;
//
//    G4cout << "Length ABS: "<<fTrackLAbs << G4endl;
//    analysisManager->FillH1(2, fTrackLAbs);
//G4cout<<"origin_reset_endevent"<<G4endl;
    originX=0;
    originY=0;
    originX=0;

}


void XenEventAction::AddAbs(G4double de, G4double dl) {

//  fEnergyAbs += std::abs(de);
 // fTrackLAbs += std::abs(dl);

  //G4cout<<"EnergyABS(XenEventAction): "<<fEnergyAbs<<G4endl;
  //G4cout<<"LengthABS(XenEventAction): "<<fTrackLAbs<<G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XenEventAction::Reset()
{
  //reset cumulative quantities
  //

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

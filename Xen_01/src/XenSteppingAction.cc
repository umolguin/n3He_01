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
// * regarding  this  software system or assume any liaility for its *
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
/// \file XenSteppingAction.cc
/// \brief Implementation of the XenSteppingAction class

#include "XenSteppingAction.hh"
#include "XenEventAction.hh"
#include "XenDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4VProcess.hh"

#include "XenAnalysis.hh"
#include <cmath>

#include "CellManager.hh"

XenSteppingAction* XenSteppingAction::Instance()
{

}
XenSteppingAction::XenSteppingAction(
                      XenDetectorConstruction* detectorConstruction,
                      XenEventAction* eventAction)
  : G4UserSteppingAction(),
    fEventAction(eventAction)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XenSteppingAction::XenSteppingAction()
: G4UserSteppingAction()

{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XenSteppingAction::~XenSteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XenSteppingAction::UserSteppingAction(G4Step* step)
{
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4TouchableHandle theTouchable = preStepPoint->GetTouchableHandle();
    G4int _copyNo = theTouchable->GetCopyNumber();
    //XenEventAction::lastTrackID=track->GetParentID();
    G4LogicalVolume* volume= step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    G4Track* track     = step->GetTrack();
    const G4ParticleDefinition* part = track->GetDefinition();
    G4cout << "Track #"
    << track->GetTrackID() << " of " << part->GetParticleName()
    << " E(MeV)= " << track->GetKineticEnergy()/MeV
    << " produced by Track ID= " << track->GetParentID()
    << G4endl;

    // Kinetic energy
    G4double ken = track->GetKineticEnergy()/MeV;

    // energy deposit
    G4double edep = step->GetTotalEnergyDeposit();
    //G4cout <<"edep: "<<edep<<G4endl;
    G4cout<<"ProcessNamePost: "<<step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()<<", with edep:"<<edep/MeV<<G4endl;

    // step length
    G4double stepLength = 0.;

    stepLength=track->GetPosition().z();

    //XenEventAction::AddAbs(edep,stepLength);
    G4double _path=0;
    G4double realEDep=step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit();

    G4double _weight=.01;
    G4int _nIons=0;
    if(step -> GetTrack() -> GetDefinition() == G4Neutron::Definition()){
        if(ken==0)
        {
            XenEventAction::originZ=track->GetPosition().z();
            XenEventAction::originX=track->GetPosition().x();
            XenEventAction::originY=track->GetPosition().y();

            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            _path=std::abs(stepLength);
            XenEventAction::accumDelta=0;
            XenEventAction::lastCellID=_copyNo;

         //   analysisManager->FillH1(2, _path);//TODO: Please don't be like this =(
        }
        else{G4cout<<"neutron Ken: "<<ken<<", edep: "<<edep<<", stpLength: "<<_path<<G4endl;}
    }
    else if(step -> GetTrack() -> GetDefinition() == G4Proton::Definition())
    {
        //This is a proton
        if(XenEventAction::originZ!=0){
            G4double currentX=track->GetPosition().x();
            G4double currentY=track->GetPosition().y();
            G4double currentZ=track->GetPosition().z();

            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

            _path=std::abs(stepLength);
            G4double _dist=std::sqrt(std::pow(XenEventAction::originX-currentX,2)+std::pow(XenEventAction::originY-currentY,2)+std::pow(XenEventAction::originZ-currentZ,2));

           // G4int nElectrons=((realEDep)/MeV)/.5*MeV;
            G4double _delta=std::abs((step->GetDeltaEnergy()/MeV));
            G4double _totalEnergy=std::abs((step->GetTotalEnergyDeposit()/MeV));

            G4cout<<"[EDEP]trans:"<<"("<<XenEventAction::originX<<","<<XenEventAction::originY<<","<<XenEventAction::originZ<<")->("<<currentX<<","<<currentY<<","<<currentZ<<")"<<G4endl;
            G4cout<<"[EDEP]delta:"<<_delta<<",total:"<<_totalEnergy<<",stepLength:"<<(step->GetStepLength()/MeV)<<",_dist"<<_dist<<",procName:"<<step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()<<G4endl;
            _nIons=_delta/_weight;
            G4int _discreet=round(_dist);

            analysisManager->FillH1(3, _discreet,_nIons);

            if(XenEventAction::lastCellID!=_copyNo)
            {
                double _mX=track->GetVertexMomentumDirection().x();
                double _mY=track->GetVertexMomentumDirection().y();
                double _mZ=track->GetVertexMomentumDirection().z();

                G4cout<<"[A]EnergyDep:"<<XenEventAction::accumDelta<<",delta:"<<_delta<<"("<<_mX<<","<<_mY<<","<<_mZ<<")"<<G4endl;
                //man->FillH2(1, i,j,CellManager::getGFactor(j,i)*10);
                CellManager::addEnergy(XenEventAction::accumDelta,XenEventAction::lastCellID,track->GetVertexMomentumDirection () ,false);
                XenEventAction::accumDelta=0;
            }
            else
            {
                G4cout<<"[A]LastCell=copyNO"<<_copyNo<<G4endl;
                XenEventAction::accumDelta+=_delta;

            }
            XenEventAction::lastCellID=_copyNo;

//            for(int i=0; i<nElectrons;i++)
//                analysisManager->FillH1(3, _dist);


//            analysisManager->FillNtupleDColumn(0, ken);
//            analysisManager->FillNtupleDColumn(1, 1);
//            analysisManager->AddNtupleRow();
        }

    }


    else if(step -> GetTrack() -> GetDefinition() == G4Triton::Definition())
    {
        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        G4double currentX=track->GetPosition().x();
            G4double currentY=track->GetPosition().y();
            G4double currentZ=track->GetPosition().z();
        _path=std::abs(stepLength);
        //G4cout<<"[EDEP]:gen_"<<part->GetParticleName()<<" Ken: "<<ken<<", edep: "<<edep<<", stpLength: "<<_path<<G4endl;
        G4double _dist=std::sqrt(std::pow(XenEventAction::originX-currentX,2)+std::pow(XenEventAction::originY-currentY,2)+std::pow(XenEventAction::originZ-currentZ,2));

           // G4int nElectrons=((realEDep)/MeV)/.5*MeV;
            G4double _delta=std::abs((step->GetDeltaEnergy()/MeV));
            G4double _totalEnergy=std::abs((step->GetTotalEnergyDeposit()/MeV));

            //G4cout<<"[EDEP]dist:"<<_dist<<"("<<XenEventAction::originX<<","<<XenEventAction::originY<<","<<XenEventAction::originZ<<")->("<<currentX<<","<<currentY<<","<<currentZ<<")"<<",delta:"<<_delta<<G4endl;
            G4cout<<"[EDEP]trans:"<<"("<<XenEventAction::originX<<","<<XenEventAction::originY<<","<<XenEventAction::originZ<<")->("<<currentX<<","<<currentY<<","<<currentZ<<")"<<G4endl;
            G4cout<<"[EDEP]ELECTRON_delta:"<<_delta<<",total:"<<_totalEnergy<<",stepLength:"<<(step->GetStepLength()/MeV)<<",_dist"<<_dist<<G4endl;

            G4int _discreet=std::abs(round(_dist))*-1;
            _nIons=_delta/_weight;
            analysisManager->FillH1(3, _discreet,_nIons);

//             if(XenEventAction::lastCellID!=_copyNo)
//            {
//                G4cout<<"[A]EnergyDep:"<<XenEventAction::accumDelta<<",delta:"<<_delta<<G4endl;
//                //man->FillH2(1, i,j,CellManager::getGFactor(j,i)*10);
//                CellManager::addEnergy(XenEventAction::accumDelta,XenEventAction::lastCellID,track->GetVertexMomentumDirection (),false);
//                XenEventAction::accumDelta=0;
//            }
//            else
//            {
//                G4cout<<"[A]LastCell=copyNO"<<_copyNo<<G4endl;
//                XenEventAction::accumDelta+=_delta;
//
//            }
//            XenEventAction::lastCellID=_copyNo;
    }


    //std::cout<<"beforeDrawTracks"<<std::endl;
    DrawTracks(true, track,fpSteppingManager);

  return;
}

void XenSteppingAction::DrawTracks(G4bool drawFlag, G4Track *theTrack,G4SteppingManager* pSM)
{

//std::cout<<"drawPolyLine"<<std::endl;

      G4ParticleDefinition *particleType = theTrack->GetDefinition();

      G4Colour red      ( 255/255.,   0/255.,   0/255.);
      G4Colour blue     (   0/255.,   0/255., 255/255.);
      G4Colour green    (   0/255., 255/255.,   0/255.);
      G4Colour yellow   ( 255/255., 255/255.,   0/255.);

      G4Colour white    ( 255/255., 255/255., 255/255.);

      G4Colour orange   ( 255/255., 127/255.,   0/255.);
      G4Colour magenta  ( 237/255., 173/255., 255/255.);
      G4Colour magenta1 ( 104/255.,  49/255.,  94/255.);

      G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

      if (pVVisManager) {
          // Declare begininng of visualization




	G4Polyline polyline;

//	G4double charge = pSM->GetTrack()->GetDefinition()->GetPDGCharge();
//
	G4Colour colour;
//
//	if      (charge < 0.) colour = red;
//	else if (charge > 0.) colour = blue;
//	else                  colour = green;

	G4cout<<"ParType:"<<particleType->GetParticleType()<<G4endl;
	G4cout<<"ParSubType:"<<particleType->GetParticleSubType()<<", ParticleName:"<<particleType->GetParticleName()<<G4endl;
	if( particleType == G4Gamma::GammaDefinition()){
	  colour = green;
	  G4cout<<"GREEN_GAMMA!!"<<G4endl;
	}
	if( particleType == G4Triton::TritonDefinition()){
	  colour = yellow;
	  G4cout<<"TRITON_YELLOW!!"<<G4endl;
	}
	if( particleType == G4Neutron ::NeutronDefinition())            {
	    G4cout<<"NEUTRON_WHITE!!"<<G4endl;
	  colour = white; }
	if( particleType == G4Proton  ::ProtonDefinition())
	  { G4cout<<"PROTON_BLUE!!"<<G4endl; colour = blue; }
	if( particleType == G4Electron  ::ElectronDefinition())
	  { colour = red; G4cout<<"ELECTRON_RED!!"<<G4endl;}
    //colour = G4Colour(1., 1., 1.);
	G4VisAttributes attribs(colour);

	polyline.SetVisAttributes(attribs);
	G4cout<<"psmPosPre:"<<pSM->GetStep()->GetPreStepPoint()->GetPosition()<<G4endl;
	G4cout<<"psmPosPos:"<<pSM->GetStep()->GetPostStepPoint()->GetPosition()<<G4endl;
	polyline.push_back(pSM->GetStep()->GetPreStepPoint()->GetPosition());
	polyline.push_back(pSM->GetStep()->GetPostStepPoint()->GetPosition());



	 //const G4String preProsName=proBefore->GetProcessType();
	 //const G4String posProsName=proAfter->GetProcessName();
    //G4cout<<"ProcessPreName:"<<preProsName<<G4endl;
    //G4cout<<"ProcessPosName:"<<posProsName<<G4endl;
//G4cout<<"Gonna draw me sum poleis m8!"<<G4endl;
	pVVisManager -> Draw(polyline);


      }

}

void XenSteppingAction::Reset()
{
  fEnergy = 0.;
}


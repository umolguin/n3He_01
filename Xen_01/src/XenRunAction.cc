#include "XenRunAction.hh"
#include "XenPrimaryGeneratorAction.hh"
#include "XenEventAction.hh"
#include "XenSteppingAction.hh"
#include "XenSteppingAction.hh"
#include "XenAnalysis.hh"
  // use of other actions
  // - primary generator: to get info for printing about the primary
  // - event action: to get and reset accumulated energy sums
  // - stepping action: to get info about accounting volume

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "TFile.h"
#include "TH1F.h"
#include "CellManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


XenRunAction::XenRunAction()
: G4UserRunAction()
{
;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XenRunAction::~XenRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XenRunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/scene/notifyHandlers");
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

    G4SDManager * SDman = G4SDManager::GetSDMpointer();
    G4String colNam;

    G4AnalysisManager* man =
    G4AnalysisManager::Instance();
    // Open an output file
    man->OpenFile("Histograms");
    man->SetFirstHistoId(1);
    // Create histogram(s)
    man->CreateH1("histo1","Energy Deposition", 100, 0., .005*eV);

    man->CreateH1("histo2","Length of deposition", 100, -20, 30*cm);//TODO: change to the appropiate size
    man->CreateH1("histo3","Ionization distribution", 40, -2*cm, 6*cm);//TODO: change to the appropiate size

    man->CreateH2("GeometricFactor","Geometric Factor",16,0,16,9,0,9);


}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XenRunAction::EndOfRunAction(const G4Run* aRun)
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    for(int i=0; i<9;i++)
			for(int j=0;j<16;j++)
				man->FillH2(1, j,i,CellManager::getGFactor(i,j));
//    G4double ken = track->GetKineticEnergy()/MeV;
//    G4cout <<"ken: "<<ken<<G4endl;
//    // energy deposit
//    G4double edep = step->GetTotalEnergyDeposit();
//    G4cout <<"edep: "<<edep<<G4endl;
//    XenEventAction::AddAbs(edep,stepLength);
    G4int nofEvents = aRun->GetNumberOfEvent();
    if (nofEvents == 0) return;

    G4cout << aRun->GetRunID() << G4endl;

    if (G4VVisManager::GetConcreteInstance())
    {
      G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
    }

    man->Write();
    man->CloseFile();
}

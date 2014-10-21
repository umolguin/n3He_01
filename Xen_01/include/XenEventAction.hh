#ifndef XenEventAction_h
#define XenEventAction_h 1

#include "G4UserEventAction.hh"
#include "G4THitsMap.hh"
#include "globals.hh"

class XenSteppingAction;

/// Event action class
///
/// It holds data member fEnergySum and fEnergy2Sum for accumulating
/// the event energy deposit its square event by event.
/// These data are then used in the run action to compute the dose.
/// The accumulated energy and enrgy square sums are reset for each
/// new run via the Reset() function from the run action.

class XenEventAction : public G4UserEventAction
{
  public:
  static G4double  originZ;
  static G4double  originX;
  static G4double  originY;
  static G4int lastCellID;
  static G4double lastTrackID;
  static G4double accumDelta;

    XenEventAction();
    virtual ~XenEventAction();

    // static access method
    static XenEventAction* Instance();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void Reset();
    static void AddAbs(G4double de, G4double dl);



  private:
    static XenEventAction* fgInstance;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// inline functions






#endif

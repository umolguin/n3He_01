#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserLimits.hh"

#include "XenDetectorConstruction.hh"
 #include "GDMLDetectorConstruction.hh"
#include "XenSteppingAction.hh"

#include "G4GDMLParser.hh"
#include "G4UnitsTable.hh"

using namespace std;




XenDetectorConstruction::XenDetectorConstruction(): G4VUserDetectorConstruction()
{
}
XenDetectorConstruction::~XenDetectorConstruction(){}

G4VPhysicalVolume* XenDetectorConstruction::Construct()
{

    G4double atomicNumber = 1.;
    G4double massOfMole = 1.008*g/mole;
    G4double density1 = 1.e-25*g/cm3;
    G4double temperature1 = 2.73*kelvin;
    G4double pressure1 = 3.e-18*pascal;
    G4Material* vacuum =new G4Material("interGalactic", atomicNumber,massOfMole, density1,kStateGas,temperature1, pressure1);
    //Get nist material manager
    G4NistManager* mngr= G4NistManager::Instance();



    G4double density3 = 2.700*g/cm3;
    G4double a3 = 26.98*g/mole;
    G4Material* Al = new G4Material("Aluminum", 13., a3, density3);


    G4int protons=2, neutrons=1, nucleons=protons+neutrons;
    G4double atomicMass = 3.016*g/mole;
    G4Isotope* he3 = new G4Isotope("He3", protons, nucleons, atomicMass);
    G4int isotopes, elements;
    G4Element* He3 = new G4Element("Helium3", "He3", isotopes=1);
    He3->AddIsotope(he3, 100*perCent);

    G4double pressure = 1*atmosphere;
    G4double temperature = 273.15*kelvin;
    G4double molar_constant = Avogadro*k_Boltzmann;  //from clhep
    G4double density = (atomicMass*pressure)/(temperature*molar_constant);// Ideal gas law
    G4Material* matHelium3Gas = new G4Material("Helium3", density, elements=1, kStateGas, temperature, pressure);
    matHelium3Gas->AddElement(He3, 100*perCent);


//    G4Element* elN  = new G4Element("Nitrogen","N" ,  7., 14.01*g/mole);
//    G4Material* matMix = new G4Material("Mix", .1340*mg/cm3, 2,kStateGas,273.15*kelvin,1.*atmosphere);
//    matMix->AddElement(elN, 0.1);
//    matMix->AddElement(elHe3, 0.9);
//
    //Geometries
//    G4Box* worldBox=new G4Box("World",100*cm,100*cm,100*cm);
//
//    G4Box* targetBox= new G4Box("Target",50*cm,50*cm,100*cm);
//
//    //Logical Volumes... Holders, MetaClasses, JavaBeans, Wrapper, etc... but physicists call it logical... cause everything else is not... you know logical
//    G4LogicalVolume* worldLogical = new G4LogicalVolume(worldBox,vacuum,"World");
//
//
//    //Placement
//    G4VPhysicalVolume* worldPhys = new G4PVPlacement(0,G4ThreeVector(0,0,0),worldLogical,"World",
//                                                         0,//parentRef... so this is the root node
//                                                         false,
//                                                         0,
//                                                         true);
//
//    G4LogicalVolume* targetLogical= new G4LogicalVolume(targetBox,matHelium3Gas,"Target");//TODO: change material back to helium3
//    G4double maxStep = 2*mm;
//    targetLogical->SetUserLimits(new G4UserLimits(maxStep));
//    G4VPhysicalVolume* targetPhys =new G4PVPlacement(0, G4ThreeVector(0,0,-75*cm),targetLogical,"Target",worldLogical,false,0,true);
//G4GDMLParser parser;
//parser.Write("output.gdml", worldPhys, true, "/home/umolguin/geant/work/Xen_build");
//return worldPhys;


    G4GDMLParser parser;
    parser.Read("TargetChamber.gdml");
    //parser.Read("TG003macor2.gdml");
    //parser.Read("TG003macor2Dirty.gdml");
    G4VPhysicalVolume* W=parser.GetWorldVolume();

//    for(int i=1; i<387;i++)
//    {
//        stringstream ss;//create a stringstream
//        ss << i;//add number to the stream
//
//        string _name = string("TG003macor_") + ss.str()+string("_vol");
//        G4LogicalVolume* lv = parser.GetVolume( _name);
//        lv->SetVisAttributes (G4VisAttributes::Invisible);
//    }

//    G4LogicalVolume* lv1 = parser.GetVolume( "TG003macor_vol");
//    lv1->SetVisAttributes (G4VisAttributes::Invisible);

//    G4LogicalVolume* lv1 = parser.GetVolume( "TG003macor_vol");
//    G4TessellatedSolid* sol=(G4TessellatedSolid)lv1->GetSolid();

//    lv1->SetMaterial(matHelium3Gas);
    G4LogicalVolume* lv = W->GetLogicalVolume();

//    G4double maxStep = 2*mm;

//    lv->SetUserLimits(new G4UserLimits(maxStep));
    lv->SetMaterial(vacuum);
 //   lv->SetVisAttributes (G4VisAttributes::Invisible);

    //G4Tubs* _cylSol= new G4Tubs("Cylinder #1",0,32.4*cm,32.3*cm,0,twopi);
//    G4Tubs* _cylSol= new G4Tubs("Target",0,32.4*cm,30*cm,0,twopi);
//    G4LogicalVolume* _cylLog= new G4LogicalVolume(_cylSol,matHelium3Gas,"Target");
//    new G4PVPlacement(0, G4ThreeVector(0,0,-35.2*cm),_cylLog,"Target",lv,false,0,true);
    double _cX=16.02*cm; double _cY=1.905*cm; double _cZ=1.905*cm;

    G4Box** _aCells= new G4Box*[144];
    G4LogicalVolume** _aLCells= new G4LogicalVolume*[144];

    //G4Box* _cells= new G4Box("Cell",_cX/2,_cY/2,_cZ/2);
    //G4LogicalVolume* _lCells= new G4LogicalVolume(_cells,matHelium3Gas,"Cell");
    for(int i=0; i<9;i++)
        for(int j=0;j<16;j++)
        {
            int _index=(j+(i*16));
            std::stringstream sstm;
            std::string _name="Cells";
            std::string _id;
            sstm << _name << _index;
            _id = sstm.str();

            _aCells[_index]=new G4Box(_id,_cX/2,_cY/2,_cZ/2);
            _aLCells[_index]=new G4LogicalVolume(_aCells[_index],matHelium3Gas,_id);

            new G4PVPlacement(0, G4ThreeVector(0,((8.57-1.905/2)*cm)-(i*_cY),((15.24-1.905/2)*cm)-(j*_cZ)),_aLCells[_index],_id,lv,false,10+_index,true);
        }

    W->SetLogicalVolume(lv);
    ////parser.Write("output.gdml", worldPhys, true, "/home/umolguin/geant/work/Xen_build");

    return W;
}

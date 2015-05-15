//---------------------------------------------------------------------------

#ifndef s2pH
#define s2pH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Dialogs.hpp>
#include <FileCtrl.hpp>
#include <IniFiles.hpp>
#include <string>
#include "GestionLangue.h"
//---------------------------------------------------------------------------
class TInterfaceS2P : public TForm
{
__published:	// Composants gérés par l'EDI
	TMainMenu *MainMenu1;
	TMenuItem *Quitter1;
	TMenuItem *A_propos;
	TImage *Image1;
	TImage *Image2;
	TButton *lancer_s2p;
	TButton *lancer_scratch2;
	TButton *ScratchEnLigne;
	TMenuItem *Wiki1;
	TMenuItem *Documentation1;
	TMenuItem *PrparerPicaxe;
	TMenuItem *Utilisation1;
	TMenuItem *N3Mmentodescommandes1;
	TLabel *Label4;
	TLabel *Label5;
	TOpenDialog *Open_s2;
	TMenuItem *Ouvrir;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *Aide1;
	TMenuItem *Apropos1;
	TMenuItem *Fichier;
	TMenuItem *N3;
	TMenuItem *ExemplesScratch2;
	TMenuItem *Siteofficieldes2p;
	TMenuItem *N4Principesdeprogrammation1;
	TLabel *Label1;
	TLabel *Label2;
	TComboBox *ComboBox;
	TLabel *choixPicaxe;
	TMenuItem *Outils1;
	TMenuItem *Bibliothque;
	TMenuItem *Gestionnairedepriphriques1;
	TMenuItem *Langue1;
	TMenuItem *N4;
	TMenuItem *N5;
	TMenuItem *Init;
	TMenuItem *N6;
	TMenuItem *Localiserledossierbibliothque1;
	TMenuItem *Localiserledossierprojets1;
	TMenuItem *Localiserledossierdocumentation1;
	TMenuItem *SiteofficielPicaxe1;
	TMenuItem *lchargerlesdrivers1;
	TOpenDialog *OpenDialog;
	TMenuItem *Localiserlesmodles1;
	TComboBox *Popup;
	TMenuItem *InstallDrivers;
	TMenuItem *Locate;
	TMenuItem *Drivers;
	TMenuItem *Links;
	TMenuItem *N7;
	void __fastcall A_proposClick(TObject *Sender);
	void __fastcall PrparerPicaxeClick(TObject *Sender);
	void __fastcall Utilisation1Click(TObject *Sender);
	void __fastcall N3Mmentodescommandes1Click(TObject *Sender);
	void __fastcall Wiki1Click(TObject *Sender);
	void __fastcall ScratchEnLigneClick(TObject *Sender);
	void __fastcall lancer_scratch2Click(TObject *Sender);
	void __fastcall lancer_s2pClick(TObject *Sender);
	void __fastcall Quitter1Click(TObject *Sender);
	void __fastcall OuvrirClick(TObject *Sender);
	void __fastcall Apropos1Click(TObject *Sender);
	void __fastcall Siteofficieldes2pClick(TObject *Sender);
	void __fastcall N4Principesdeprogrammation1Click(TObject *Sender);
	void __fastcall ComboBoxChange(TObject *Sender);
	void __fastcall Gestionnairedepriphriques1Click(TObject *Sender);  
	void __fastcall SearchEx(AnsiString FilePath, TStringList * Extensions, TStrings * ListeFichiers, int RangMenu);
	void __fastcall SearchTemp(AnsiString FilePath, TStringList * Extensions, TComboBox * ListeFichiers);
	void __fastcall SearchDocs(AnsiString FilePath, TStringList * Extensions, TStrings * ListeFichiers, int RangMenu);
	void __fastcall ExempleClick(TObject *Sender);
	void __fastcall DocsClick(TObject *Sender);
	void __fastcall InitINI();
	void __fastcall InitClick(TObject *Sender);
	void __fastcall Localiserledossierbibliothque1Click(TObject *Sender);
	void __fastcall Localiserledossierprojets1Click(TObject *Sender);
	void __fastcall Localiserledossierdocumentation1Click(TObject *Sender);
	void __fastcall SiteofficielPicaxe1Click(TObject *Sender);
	void __fastcall lchargerlesdrivers1Click(TObject *Sender);
	void __fastcall Localiserlesmodles1Click(TObject *Sender);
	void __fastcall InstallDriversClick(TObject *Sender);
	void __fastcall DriversClick(TObject *Sender);
private:	// Déclarations de l'utilisateur
	TLabel * interLabel ;
	TButton * interButton ;
	TInterfaceS2P * interInterfaceS2P ;
	TGroupBox * interGroupeBox ;
	TComboBox * interComboBox ;
	TCheckBox * interCheckBox ;
	TRadioGroup * interRadioGroupe ;
	TMenuItem * interMenuItem ;
	GestionLangue * langue;
public:		// Déclarations de l'utilisateur
	void __fastcall Langue1Click(TObject *Sender);
	__fastcall TInterfaceS2P(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TInterfaceS2P *InterfaceS2P;
//---------------------------------------------------------------------------
#endif

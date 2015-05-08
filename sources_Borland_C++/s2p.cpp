//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#pragma hdrstop

#include <windows.h>

using namespace std;  //pour les traductions
#include <tinyxml.h>

#include <IniFiles.hpp> //pour le fichier INI

#include "s2p.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TInterfaceS2P *InterfaceS2P;
//listes pour récupérer le contenu du fichier INI
int choix_langue;
AnsiString locate_scratch2;
AnsiString locate_librairie;
AnsiString locate_project;
AnsiString locate_doc;
AnsiString locate_template;
//listes pour les menus dynamiques
TStringList *Examples = new TStringList;
TStringList *Biblio = new TStringList;
TStringList *Aides = new TStringList;
TStringList *Docs = new TStringList;
TStringList *Extensions1 = new TStringList;
TStringList *Extensions2 = new TStringList;

char *picaxe_choisi="axe020";

//---------------------------------------------------------------------------
void __fastcall TInterfaceS2P::InitINI()
{

  TIniFile *INI = new TIniFile(ExtractFilePath(Application->ExeName)+ "s2p.ini");
  //écriture dans le fichier INI des variables utiles
  INI->WriteString("locate Scratch2", "locate_scratch2", "Scratch 2.exe"); //chemin par défaut pour initialiser
  INI->WriteInteger("langue", "langue", 0); //dans le fichier label.xml, à la balise <Langues>, cela correspond au n° du rang de la langue
  //dossiers à utiliser pour les documents
  INI->WriteString("locate Librairie", "locate_librairie", ExtractFilePath(Application->ExeName) + "bibliotheque\\"); //chemin par défaut pour initialiser
  INI->WriteString("locate Project", "locate_project", ExtractFilePath(Application->ExeName) + "projets\\"); //chemin par défaut pour initialiser
  INI->WriteString("locate Documentation", "locate_doc", ExtractFilePath(Application->ExeName) + "documentation\\"); //chemin par défaut pour initialiser
  INI->WriteString("locate Template", "locate_template", ExtractFilePath(Application->ExeName) + "s2p\\templates\\"); //chemin par défaut pour initialiser
  Extensions1->Add(".sb2");
  SearchTemp(locate_template, Extensions1, ComboBox);
  delete INI;
}
//---------------------------------------------------------------------------
__fastcall TInterfaceS2P::TInterfaceS2P(TComponent* Owner)
	: TForm(Owner)
{
	//vérification de l'existence du fichier INI, sinon le recréé
	if (!FileExists(ExtractFilePath(Application->ExeName)+ "s2p.ini"))
		InitINI();
	//lecture du fichier INI
	TIniFile *INI = new TIniFile(ExtractFilePath(Application->ExeName)+ "s2p.ini");
	choix_langue=INI->ReadInteger("langue", "langue", 0);
	locate_scratch2=INI->ReadString("locate Scratch2", "locate_scratch2", "Scratch 2.exe");
	locate_librairie=INI->ReadString("locate Librairie", "locate_librairie", ExtractFilePath(Application->ExeName) + "bibliotheque\\");
	locate_project=INI->ReadString("locate Project", "locate_project", ExtractFilePath(Application->ExeName) + "projets\\");
	locate_doc=INI->ReadString("locate Documentation", "locate_doc", ExtractFilePath(Application->ExeName) + "documentation\\");
	locate_template=INI->ReadString("locate Template", "locate_template", ExtractFilePath(Application->ExeName) + "s2p\\templates\\");
	delete INI;

  Extensions1->Add(".sb2");
  // Appel à Search, avec pour liste des fichiers, les items des listes Bibilo & Examples & menu déroulant
  SearchEx(locate_librairie, Extensions1, Biblio,0);
  SearchEx(locate_project, Extensions1, Examples,1);
  SearchTemp(locate_template, Extensions1, ComboBox);
  delete Extensions1;
  ComboBox->ItemIndex=0;
  Extensions2->Add(".pdf");
  Extensions2->Add(".odt");
  Extensions2->Add(".lnk");
  Extensions2->Add(".url");
  Extensions2->Add(".html");
  Extensions2->Add(".htm");
  // Appel à SearchDocs, avec pour liste des fichiers les items de Documentation.
  SearchDocs(locate_doc, Extensions2, Docs,2);
  delete Extensions2;

  //appel à la procédure pour gérer les langues
  AnsiString file = ExtractFilePath(Application->ExeName) + "label.xml";
  langue = new GestionLangue;
  langue->Init(InterfaceS2P->Langue1,file.c_str(),(ptrOnClick)&Langue1Click);
  //après l'initialisation des langues, le système pioche la langue précédemment sélectionnée
  langue->Change(choix_langue);
}
//-------------------------recherche des fichiers sb2 pour les lister dans le menu Fichier--------------------------------------------------
void __fastcall TInterfaceS2P::SearchEx(AnsiString FilePath, TStringList * Extensions, TStrings * ListeFichiers, int RangMenu)
{
  TSearchRec Infos_fichier;
  TMenuItem *NewItem = new TMenuItem(this);
  if (!FindFirst(FilePath+"*.*", faAnyFile, Infos_fichier))
  do
  {
	for (int i = 0 ; i < Extensions->Count ; i++)
	{
	  if (ExtractFileExt(Infos_fichier.Name).LowerCase() == Extensions->Strings[i].LowerCase())
	  {
		//ajout dans la liste d'une ligne avec le chemin+nom du fichier trouvé
		ListeFichiers->Add(ExpandFileName(Infos_fichier.Name));
		//nettoie le nom de fichier de son extension sb2 pour le nom seul du fichier, sans le chemin
		AnsiString CleanName = StringReplace(Infos_fichier.Name, ".sb2", "",TReplaceFlags() << rfReplaceAll);
		//rajout d'entrées dans le menu
		NewItem = new TMenuItem(this);
		NewItem->Caption = CleanName;
		NewItem->Name = CleanName;
		//stockage du chemin complet + nom de fichier pour la fonction ExempleClick
		NewItem->Hint = FilePath + Infos_fichier.Name.c_str();
		NewItem->OnClick=ExempleClick;
		//rajout au bon endroit de l'entrée dans le menu
		MainMenu1->Items->Items[0]->Items[RangMenu]->Add(NewItem);
	  }
	}
  }
  while(!FindNext(Infos_fichier));
  FindClose(Infos_fichier);
}
//-------------------------recherche des fichiers sb2 pour les lister dans la liste déroulante des modèles--------------------------------------------------
void __fastcall TInterfaceS2P::SearchTemp(AnsiString FilePath, TStringList * Extensions, TComboBox * ListeFichiers)
{
  TSearchRec Infos_fichier;
  if (!FindFirst(FilePath+"*.*", faAnyFile, Infos_fichier))
  do
  {
	for (int i = 0 ; i < Extensions->Count ; i++)
	{
	  if (ExtractFileExt(Infos_fichier.Name).LowerCase() == Extensions->Strings[i].LowerCase())
	  {
		//nettoie le nom de fichier de son extension pour le nom seul du fichier, sans le chemin
		AnsiString CleanName = StringReplace(Infos_fichier.Name, Extensions->Strings[i].LowerCase(), "",TReplaceFlags() << rfReplaceAll);
		//rajout d'entrées dans le menu
		ListeFichiers->Items->Add(CleanName);
	  }
	}
  }
  while(!FindNext(Infos_fichier));
  FindClose(Infos_fichier);
}
//---------------------associer chaque entrée dynamique dans Biblio & Exemples à leur lancement-------------------
void __fastcall TInterfaceS2P::ExempleClick(TObject *Sender)
{
AnsiString CheminNomFichier;
ofstream fichier_s2("scratch2.bat", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
		if(fichier_s2)
		{
			   //récupération des infos de l'entrée TMenu qui a déclenché cette fonction
			   CheminNomFichier=static_cast<TMenuItem*>(Sender)->Hint;
			   fichier_s2 << "@echo off\nbreak ON\nrem fichiers BAT et fork créés par Sébastien CANET\ncls\nSET currentpath=%~dp1\nSET dossier_scratch="<< '"' << locate_scratch2.c_str() << '"' << "\nstart %dossier_scratch% " << '"' << CheminNomFichier.c_str() << '"';
			   fichier_s2.close();
		}
		else ShowMessage(Popup->Items->Strings[0]);
ShellExecute(0, 0, "scratch2.bat", 0, 0 , SW_HIDE );
}
//-------------------------recherche des fichiers de docs pour les lister dans les menus Aide & Documentation---------------
void __fastcall TInterfaceS2P::SearchDocs(AnsiString FilePath, TStringList * Extensions, TStrings * ListeFichiers, int RangMenu)
{
  TSearchRec Infos_fichier;
  TMenuItem *NewItem = new TMenuItem(this);
  if (!FindFirst(FilePath+"*.*", faAnyFile, Infos_fichier))
  do
  {
	for (int i = 0 ; i < Extensions->Count ; i++)
	{
	  if (ExtractFileExt(Infos_fichier.Name).LowerCase() == Extensions->Strings[i].LowerCase())
	  {
		//ajout dans la liste d'une ligne avec le chemin+nom du fichier trouvé
		ListeFichiers->Add(ExpandFileName(Infos_fichier.Name));
		//nettoie le nom de fichier de son extension sb2 pour le nom seul du fichier, sans le chemin
		AnsiString CleanName = StringReplace(Infos_fichier.Name, Extensions->Strings[i].LowerCase(), "",TReplaceFlags() << rfReplaceAll);
		//rajout d'entrées dans le menu
		NewItem = new TMenuItem(this);
		NewItem->Caption = Infos_fichier.Name;
		NewItem->Name = CleanName;
		//stockage du chemin complet + nom de fichier pour la fonction ExempleClick
		NewItem->Hint = FilePath + Infos_fichier.Name.c_str();
		NewItem->OnClick=DocsClick;
		//rajout au bon endroit de l'entrée dans le menu
		MainMenu1->Items->Items[RangMenu]->Add(NewItem);
	  }
	}
  }
  while(!FindNext(Infos_fichier));
  FindClose(Infos_fichier);
}
//---------------------associer chaque entrée dynamique dans Aide & Documentation à leur lancement-------------------
void __fastcall TInterfaceS2P::DocsClick(TObject *Sender)
{
//récupération des infos de l'entrée TMenu qui a déclenché cette fonction
AnsiString CheminNomFichier=static_cast<TMenuItem*>(Sender)->Hint;

ShellExecute(0, 0, CheminNomFichier.c_str(), 0, 0 , SW_HIDE );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2P::A_proposClick(TObject *Sender)
{
ShellExecute(0, 0, "https://github.com/technologiescollege/s2p", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2P::Siteofficieldes2pClick(TObject *Sender)
{
ShellExecute(0, 0, "http://www.picaxe.com/Software/Third-Party/Scratch/", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2P::PrparerPicaxeClick(TObject *Sender)
{
ShellExecute(0, 0, ".\\s2p\\datasheets\\1.pdf", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2P::Utilisation1Click(TObject *Sender)
{
ShellExecute(0, 0, ".\\s2p\\datasheets\\2.pdf", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2P::N4Principesdeprogrammation1Click(TObject *Sender)
{
ShellExecute(0, 0, ".\\s2p\\datasheets\\3.pdf", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2P::N3Mmentodescommandes1Click(TObject *Sender)
{
ShellExecute(0, 0, ".\\s2p\\datasheets\\4.pdf", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2P::Wiki1Click(TObject *Sender)
{
ShellExecute(0, 0, "http://tic.technologiescollege.fr/wiki", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2P::ScratchEnLigneClick(TObject *Sender)
{
ShellExecute(0, 0, "http://scratch.mit.edu/projects/editor/", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2P::lancer_scratch2Click(TObject *Sender)
{
ofstream fichier_s2("scratch2.bat", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
		if(fichier_s2)
		{
				fichier_s2 << "@echo off\nbreak ON\nrem fichiers BAT et fork créés par Sébastien CANET\ncls\nSET currentpath=%~dp1\nSET dossier_scratch="<< '"' << locate_scratch2.c_str() << '"'<< "\nstart %dossier_scratch% " << '"' << locate_template.c_str() << picaxe_choisi << ".sb2";
				fichier_s2.close();
		}
		else ShowMessage(Popup->Items->Strings[1]);
ShellExecute(0, 0, "scratch2.bat", 0, 0 , SW_HIDE );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2P::lancer_s2pClick(TObject *Sender)
{
ShellExecute(0, 0, ".\\s2p\\s2p.exe", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2P::Quitter1Click(TObject *Sender)
{
InterfaceS2P->Close();
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2P::OuvrirClick(TObject *Sender)
{
if (Open_s2->Execute()==true) locate_scratch2=Open_s2->FileName.c_str();
//écriture dans le fichier INI
//DeleteKey("locate Scratch2", "locate_scratch2");
TIniFile *INI = new TIniFile(ExtractFilePath(Application->ExeName)+ "s2p.ini");
INI->WriteString("locate Scratch2", "locate_scratch2", locate_scratch2);
delete INI;
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2P::Apropos1Click(TObject *Sender)
{
ShowMessage(Popup->Items->Strings[2]);
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2P::ComboBoxChange(TObject *Sender)
{
strcpy(picaxe_choisi, ComboBox->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2P::Gestionnairedepriphriques1Click(TObject *Sender)
{
ShellExecute(0, 0, "devmgmt.msc", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2P::Langue1Click(TObject *Sender)
{
TIniFile *INI = new TIniFile(ExtractFilePath(Application->ExeName)+ "s2p.ini");
INI->WriteInteger("langue", "langue", ((TMenuItem*)Sender)->Tag);
langue->Change(((TMenuItem*)Sender)->Tag);
delete INI;
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2P::InitClick(TObject *Sender)
{
DeleteFile(ExtractFilePath(Application->ExeName)+ "s2p.ini");
InitINI();
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2P::Localiserledossierbibliothque1Click(
      TObject *Sender)
{
if (OpenDialog->Execute()==true) locate_librairie=ExtractFilePath(OpenDialog->FileName);
//écriture dans le fichier INI
TIniFile *INI = new TIniFile(ExtractFilePath(Application->ExeName)+ "s2p.ini");
INI->WriteString("locate Librairie", "locate_librairie", locate_librairie);
delete INI;
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2P::Localiserledossierprojets1Click(TObject *Sender)
{
if (OpenDialog->Execute()==true) locate_project=ExtractFilePath(OpenDialog->FileName);
//écriture dans le fichier INI
TIniFile *INI = new TIniFile(ExtractFilePath(Application->ExeName)+ "s2p.ini");
INI->WriteString("locate Project", "locate_project", locate_project);
delete INI;
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2P::Localiserledossierdocumentation1Click(
      TObject *Sender)
{
if (OpenDialog->Execute()==true) locate_doc=ExtractFilePath(OpenDialog->FileName);
//écriture dans le fichier INI
TIniFile *INI = new TIniFile(ExtractFilePath(Application->ExeName)+ "s2p.ini");
INI->WriteString("locate Documentation", "locate_doc", locate_doc);
delete INI;
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2P::SiteofficielPicaxe1Click(TObject *Sender)
{
ShellExecute(0, 0, "http://www.picaxe.com/Software/Third-Party/Scratch/#download", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2P::lchargerlesdrivers1Click(TObject *Sender)
{
ShellExecute(0, 0, "https://github.com/technologiescollege/drivers", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2P::Localiserlesmodles1Click(TObject *Sender)
{
if (OpenDialog->Execute()==true) locate_template=ExtractFilePath(OpenDialog->FileName);
//écriture dans le fichier INI
TIniFile *INI = new TIniFile(ExtractFilePath(Application->ExeName)+ "s2p.ini");
INI->WriteString("locate Template", "locate_template", locate_template);
delete INI;
}
//---------------------------------------------------------------------------


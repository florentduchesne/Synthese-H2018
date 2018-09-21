// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "StatistiquesDuJoueur.h"
#include "EngineUtils.h"
#include "TimerManager.h"
#include "Portail.h"
#include "GestionnaireDeNiveaux.h"
#include "Personnage/Personnage.h"
#include "GameFramework/PlayerStart.h"
#include "InformationsNiveau.h"
#include "Engine/EngineTypes.h"
#include "Blueprint/UserWidget.h"
#include "ModeDeJeu_MenuPrincipal.generated.h"

/**
 * 
 */
UCLASS()
class PORTAILCPP_API AModeDeJeu_MenuPrincipal : public AGameModeBase
{
	GENERATED_BODY()

	AGestionnaireDeNiveaux* GestionnaireDeNiveaux;

	void PlacerUnUniqueJoueur();

	//-----------------------------------------
	//---------OUVERTURE DE LA PARTIE----------
	//-----------------------------------------

	//vide le tableau et le remplis selon le nombre de joueurs
	void InitialiserStatsJoueurs();

	//liste de tous les niveaux qui ont ete choisis par le generateur de carte au lancement d'une partie
	TArray<InformationsNiveau*> NiveauxChoisis;

	//choisis les niveaux qui seront utilises pendant la partie
	void SelectionnerNiveaux(TArray<InformationsNiveau*> ListeCompleteNiveaux, int NbNiveauxVoulus);

	//selectionne quels niveaux seront connectes ensemble (NE CONNECTE PAS LES PORTAILS)
	void RelierNiveaux();

	//charge les niveaux
	void ChargerLesNiveaux();

	//connecte les portails de tous les niveaux de maniere a ce que tous les niveaux soient accessibles
	void ConnecterLesPortails();

	//Trouve tous les acteurs de type APortail charges dans le jeu, et les stock dans NiveauxChoisis
	void TrouverTousLesPortailsCharges();

	//Trouve tous les acteurs de type PointApparition charges dans le jeu, et les stock dans NiveauChoisis
	void ChercherPointsApparition();

	//Supprime tous les objets APersonnage deja present (ex: le personnage par defaut), et en cree un certain nombre, places dans les PointsApparitions selectionnes precedemment
	void PlacerJoueurs();

	//retourne vrai si tous les niveaux ont termine d'etre charges
	bool NiveauxTousCharges();

	//retourne vrai si on trouve autant de portails qu'il est suppose y en avoir
	bool CompterPortails();

	//connecte les portails, va chercher les points d'apparition et fait apparaitre les joueurs
	void InitialiserCarte();

	//-----------------------------------------
	//---------FERMETURE DE LA PARTIE----------
	//-----------------------------------------

	void PartieTerminee(int idNoJoueurGagnant);

	//decharge tous les niveaux
	void DechargerCarte();

	void DetruireTousLesJoueurs();

	//detruit un joueur specifie
	void DetruireJoueur(int NoJoueur);

	int NombreDeJoueursExistants();

	bool AttendreDetruireTousLesJoueurs();

	void RetourMenuPrincipal();

	//-----------------------------------------
	//--------DeROULEMENT DE LA PARTIE---------
	//-----------------------------------------

	//fait apparaitre un joueur sur un point d'apparition aleatoire
	void ReapparitionJoueur(int NoJoueur);

	//fait apparaitre un joueur au point d'apparition specifie avec l'ID du PlayerController specifie
	void FaireApparaitreJoueur(AActor * PointApparition, int NoJoueur);

	void AttendreQueJoueurCharge(APlayerController * Controleur, int NoJoueur, AActor * PointApparition);


	APersonnage * GetJoueurParIndex(int NoJoueur);

	//retourne un point d'apparition aleatoire (a remplacer plus tard par un point d'apparition dans un piece où il n'y a pas de joueur)
	APlayerStart * TrouverPointApparitionAleatoire();

	int NbMeutresRequisPourVictoire;

	//sera plus eleve quand on aura plus de niveaux de faits
	///IMPORTANT!!! DOIT eTRE PLUS PETIT OU eGAL a LA TAILLE DE LA LISTE DE NIVEAUX
	int NbNiveauxVoulus;

	//nombre de joueurs qui participeront dans la partie
	int NbJoueurs;

	//gerent la fin de la partie avec le timer
public:
	void TerminerPartieTimer();
	
private:
	FTimerHandle TimerHandleFinDePartie;

	TArray<float> SensibiliteH;
	TArray<float> SensibiliteV;

	
public:
	AModeDeJeu_MenuPrincipal();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MenuPrincipal")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WidgetPartie")
	TSubclassOf<UUserWidget> WidgetEnPartie;
	
	//le numero du joueur gagnant. si egal a -1, le menu ne l'affichera pas
	UPROPERTY(BlueprintReadOnly, Category = "MenuPrincipal")
	int NoJoueurGagnant = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MenuPrincipal")
	TArray<UStatistiquesDuJoueur*> StatsJoueurs;

	//le temps maximal qu'une partie peut durer
	UPROPERTY(BlueprintReadOnly, Category = "Timer")
	int TempsMaxPartie = 300;

	virtual void BeginPlay() override;

	UPROPERTY()
	UUserWidget* CurrentWidget;
	
	UFUNCTION(BlueprintCallable, Category = "MenuPrincipal")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	//appele lorsqu'un joueur meurt
	void JoueurEnTueUnAutre(int IndexJoueurTueur, int IndexJoueurMort);

	//appelle toutes les fonctions d'ouverture de la partie
	UFUNCTION(BlueprintCallable, Category = "GenerationCarte")
	void GenererCarte(int _NbJoueurs, int nb_pieces, int nb_points_victoire, int duree, UPARAM(ref) TArray<float>& SensibiliteH, UPARAM(ref) TArray<float>& SensibiliteV);
};

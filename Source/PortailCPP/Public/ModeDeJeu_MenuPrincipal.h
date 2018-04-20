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

	//liste de tous les niveaux qui ont été choisis par le générateur de carte au lancement d'une partie
	TArray<InformationsNiveau*> NiveauxChoisis;

	//choisis les niveaux qui seront utilisés pendant la partie
	void SelectionnerNiveaux(TArray<InformationsNiveau*> ListeCompleteNiveaux, int NbNiveauxVoulus);

	//sélectionne quels niveaux seront connectés ensemble (NE CONNECTE PAS LES PORTAILS)
	void RelierNiveaux();

	//charge les niveaux
	void ChargerLesNiveaux();

	//connecte les portails de tous les niveaux de manière à ce que tous les niveaux soient accessibles
	void ConnecterLesPortails();

	//Trouve tous les acteurs de type APortail chargés dans le jeu, et les stock dans NiveauxChoisis
	void TrouverTousLesPortailsCharges();

	//Trouve tous les acteurs de type PointApparition chargés dans le jeu, et les stock dans NiveauChoisis
	void ChercherPointsApparition();

	//Supprime tous les objets APersonnage déjà présent (ex: le personnage par défaut), et en crée un certain nombre, placés dans les PointsApparitions sélectionnés précédemment
	void PlacerJoueurs();

	//retourne vrai si tous les niveaux ont terminé d'être chargés
	bool NiveauxTousCharges();

	//connecte les portails, va chercher les points d'apparition et fait apparaître les joueurs
	void InitialiserCarte();

	//-----------------------------------------
	//---------FERMETURE DE LA PARTIE----------
	//-----------------------------------------

	void PartieTerminee(int idNoJoueurGagnant);

	void DechargerCarte();

	void DetruireTousLesJoueurs();

	//-----------------------------------------
	//--------DÉROULEMENT DE LA PARTIE---------
	//-----------------------------------------

	//fait apparaitre un joueur sur un point d'apparition aléatoire
	void ReapparitionJoueur(int NoJoueur);

	//fait apparaitre un joueur au point d'apparition spécifié avec l'ID du PlayerController spécifié
	void FaireApparaitreJoueur(AActor * PointApparition, int NoJoueur);

	void AttendreQueJoueurCharge(APlayerController * Controleur, int NoJoueur, AActor * PointApparition);

	//détruit un joueur spécifié
	void DetruireJoueur(int NoJoueur);

	APersonnage * GetJoueurParIndex(int NoJoueur);

	//retourne un point d'apparition aléatoire (à remplacer plus tard par un point d'apparition dans un pièce où il n'y a pas de joueur)
	APlayerStart * TrouverPointApparitionAleatoire();

	const int NbMeutresRequisPourVictoire = 3;

	//sera plus élevé quand on aura plus de niveaux de faits
	///IMPORTANT!!! DOIT ÊTRE PLUS PETIT OU ÉGAL À LA TAILLE DE LA LISTE DE NIVEAUX
	int NbNiveauxVoulus = 4;

	int NbJoueurs;

	
public:
	AModeDeJeu_MenuPrincipal();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MenuPrincipal")
	TSubclassOf<UUserWidget> StartingWidgetClass;
	
	//le numéro du joueur gagnant. si égal à -1, le menu ne l'affichera pas
	UPROPERTY(BlueprintReadOnly, Category = "MenuPrincipal")
	int NoJoueurGagnant = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MenuPrincipal")
	TArray<UStatistiquesDuJoueur*> StatsJoueurs;

	virtual void BeginPlay() override;

	UPROPERTY()
	UUserWidget* CurrentWidget;
	
	UFUNCTION(BlueprintCallable, Category = "MenuPrincipal")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	void JoueurEnTueUnAutre(int IndexJoueurTueur, int IndexJoueurMort);

	//appelle toutes les fonctions d'ouverture de la partie
	UFUNCTION(BlueprintCallable, Category = "GenerationCarte")
	void GenererCarte(int _NbJoueurs, int nb_pieces);

};

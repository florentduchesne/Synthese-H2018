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

	StatistiquesDuJoueur * StatsJoueurs[4];

	TArray<InformationsNiveau*> NiveauxChoisis;

	void SelectionnerNiveaux(TArray<InformationsNiveau*> ListeCompleteNiveaux, int NbNiveauxVoulus);

	void RelierNiveaux();

	AGestionnaireDeNiveaux * ChargerLesNiveaux();

	void ConnecterLesPortails();

	void TrouverTousLesPortailsCharges(AGestionnaireDeNiveaux * const GestionnaireDeNiveaux);

	void ChercherPointsApparition();

	void PlacerJoueurs(int NbJoueurs);
	
public:
	AModeDeJeu_MenuPrincipal();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MenuPrincipal")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	virtual void BeginPlay() override;

	UPROPERTY()
	UUserWidget* CurrentWidget;
	
	UFUNCTION(BlueprintCallable, Category = "MenuPrincipal")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	void JoueurEnTueUnAutre(int IndexJoueurTueur, int IndexJoueurMort);

	UFUNCTION(BlueprintCallable, Category = "GenerationCarte")
	void GenererCarte(int nbJoueurs);
};

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

	void SelectionnerNiveaux(TArray<InformationsNiveau*> * const NiveauxChoisis, TArray<InformationsNiveau*> ListeCompleteNiveaux, int NbNiveauxVoulus);

	void RelierNiveaux(TArray<InformationsNiveau*> * const NiveauxChoisis);

	AGestionnaireDeNiveaux * ChargerLesNiveaux(TArray<InformationsNiveau*> NiveauxChoisis);

	void ConnecterLesPortails(TArray<InformationsNiveau*> NiveauxChoisis);

	void TrouverTousLesPortailsCharges(TArray<InformationsNiveau*> * const NiveauxChoisis, AGestionnaireDeNiveaux * const GestionnaireDeNiveaux);
	
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

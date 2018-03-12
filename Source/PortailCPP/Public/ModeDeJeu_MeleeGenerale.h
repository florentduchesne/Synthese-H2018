// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "./Private/Personnage.h"
#include "ATH.h"
#include "Blueprint/UserWidget.h"
#include "EtatDuJeu.h"
#include "EtatDuJoueur.h"
#include "MyPlayerController.h"
#include "Classes/GameFramework/SpectatorPawn.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "ModeDeJeu_MeleeGenerale.generated.h"

/**
 * 
 */
UCLASS()
class PORTAILCPP_API AModeDeJeu_MeleeGenerale : public AGameMode
{
	GENERATED_BODY()

public:
	AModeDeJeu_MeleeGenerale();

	virtual void BeginPlay() override;

	void StartPlay() override;
	
	
	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	/** The widget instance that we are using as our menu. */
	UPROPERTY()
	UUserWidget* CurrentWidget;

	/** Remove the current menu widget and create a new one from the specified class, if provided. */
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
    void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);
};

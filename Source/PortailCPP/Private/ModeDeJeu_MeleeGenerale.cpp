// Fill out your copyright notice in the Description page of Project Settings.

#include "ModeDeJeu_MeleeGenerale.h"

AModeDeJeu_MeleeGenerale::AModeDeJeu_MeleeGenerale()
	:Super()
{
	//les classes par défaut pour les personnages / game mode / game state / hud
	DefaultPawnClass = APersonnage::StaticClass();
	HUDClass = AATH::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	GameStateClass = AEtatDuJeu::StaticClass();
	PlayerStateClass = AEtatDuJoueur::StaticClass();
	SpectatorClass = ASpectatorPawn::StaticClass();

	//delai minimal avant une réapparition
	MinRespawnDelay = 3.0f;
}

void AModeDeJeu_MeleeGenerale::BeginPlay()
{
	Super::BeginPlay();
}

void AModeDeJeu_MeleeGenerale::StartPlay()
{
	//le code est placé ici parce que BeginPlay ne se fait pas appeler pour des raisons inconnues...
	ChangeMenuWidget(StartingWidgetClass);
	UKismetSystemLibrary::PrintString(this, TEXT("partie commencee"), true, true, FColor::Red, 5.0f);
}

void AModeDeJeu_MeleeGenerale::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
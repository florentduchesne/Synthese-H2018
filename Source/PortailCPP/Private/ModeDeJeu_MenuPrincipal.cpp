// Fill out your copyright notice in the Description page of Project Settings.

#include "ModeDeJeu_MenuPrincipal.h"


AModeDeJeu_MenuPrincipal::AModeDeJeu_MenuPrincipal()
	: Super()
{
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = APersonnage::StaticClass();
	UE_LOG(LogTemp, Warning, TEXT("constructeur game mode"));

	for (auto i = 0; i < 4; i++)
	{
		StatsJoueurs[i] = new StatistiquesDuJoueur(i);
	}
}

void AModeDeJeu_MenuPrincipal::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("begin play game mode"));

	ChangeMenuWidget(StartingWidgetClass);
}

void AModeDeJeu_MenuPrincipal::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{

	UE_LOG(LogTemp, Warning, TEXT("change menu widget game mode"));
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

void AModeDeJeu_MenuPrincipal::JoueurEnTueUnAutre(int IndexJoueurTueur, int IndexJoueurMort)
{
	StatsJoueurs[IndexJoueurTueur]->NbMeurtres++;
	StatsJoueurs[IndexJoueurMort]->NbMorts++;

	if (StatsJoueurs[IndexJoueurTueur]->NbMeurtres == 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("JOUEUR %d A GAGNÉ LA PARTIE"), IndexJoueurTueur);
	}
}

void AModeDeJeu_MenuPrincipal::GenererCarte()
{
	AGestionnaireDeNiveaux* GestionnaireDeNiveaux = GetWorld()->SpawnActor<AGestionnaireDeNiveaux>(AGestionnaireDeNiveaux::StaticClass());
	UE_LOG(LogTemp, Warning, TEXT("Generation de la carte en cours"));
	GestionnaireDeNiveaux->ChargerNiveau(FName("Ventilation"));
	GestionnaireDeNiveaux->ChargerNiveau(FName("Metro"));
}
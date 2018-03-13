// Fill out your copyright notice in the Description page of Project Settings.

#include "GestionnaireDeNiveaux.h"


// Sets default values
AGestionnaireDeNiveaux::AGestionnaireDeNiveaux()
{

}

// Called when the game starts or when spawned
void AGestionnaireDeNiveaux::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGestionnaireDeNiveaux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGestionnaireDeNiveaux::ChargerNiveau(FName nomNiveau)
{
	ULevelStreaming *pStreaming = UGameplayStatics::GetStreamingLevel(this, nomNiveau);
	if (pStreaming)
	{
		pStreaming->bShouldBeLoaded = true;
		pStreaming->bShouldBeVisible = true;
	}
}

void AGestionnaireDeNiveaux::DechargerNiveau(FName nomNiveau)
{
	ULevelStreaming *pStreaming = UGameplayStatics::GetStreamingLevel(this, nomNiveau);
	if (pStreaming)
	{
		pStreaming->bShouldBeLoaded = false;
		pStreaming->bShouldBeVisible = false;
	}
}

void AGestionnaireDeNiveaux::ToutDecharger()
{

}

void AGestionnaireDeNiveaux::SelectionnerNiveauxACharger()
{

}
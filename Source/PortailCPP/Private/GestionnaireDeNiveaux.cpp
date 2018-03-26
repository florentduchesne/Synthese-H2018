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

void AGestionnaireDeNiveaux::ChargerNiveau(FName nomNiveau, int IdStream)
{
	FLatentActionInfo info;
	info.UUID = IdStream;
	info.Linkage = 1;
	UGameplayStatics::LoadStreamLevel(this, nomNiveau, true, true, info);
	UE_LOG(LogTemp, Warning, TEXT("on charge un niveau"));
	GetWorld()->UpdateLevelStreaming();
	/*ULevelStreaming *pStreaming = UGameplayStatics::GetStreamingLevel(this, nomNiveau);
	if (pStreaming)
	{
		pStreaming->bShouldBeLoaded = true;
		pStreaming->bShouldBeVisible = true;
	}*/
}

bool AGestionnaireDeNiveaux::NiveauEstCharge(FName nomNiveau)
{
	ULevelStreaming *pStreaming = UGameplayStatics::GetStreamingLevel(this, nomNiveau);
	return pStreaming->IsLevelLoaded();
}

void AGestionnaireDeNiveaux::DechargerNiveau(FName nomNiveau, int IdStream)
{
	FLatentActionInfo info;
	info.UUID = IdStream;
	info.Linkage = 1;
	UGameplayStatics::UnloadStreamLevel(this, nomNiveau, info);
	UE_LOG(LogTemp, Warning, TEXT("on decharge un niveau"));
	/*ULevelStreaming *pStreaming = UGameplayStatics::GetStreamingLevel(this, nomNiveau);
	if (pStreaming)
	{
		pStreaming->bShouldBeLoaded = false;
		pStreaming->bShouldBeVisible = false;
	}*/
	GetWorld()->UpdateLevelStreaming();
}
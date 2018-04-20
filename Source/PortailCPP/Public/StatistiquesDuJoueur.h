// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatistiquesDuJoueur.generated.h"

/**
 * 
 */
UCLASS()
class PORTAILCPP_API UStatistiquesDuJoueur : public UObject
{
	GENERATED_BODY()
	FString nom = "Joueur ";
	UStatistiquesDuJoueur();

public:
	void SetNoJoueur(int NoJoueur);

	UPROPERTY(BlueprintReadOnly, Category = "StatJoueur")
	int NbMeurtres = 0;
	UPROPERTY(BlueprintReadOnly, Category = "StatJoueur")
	int NbMorts = 0;
};

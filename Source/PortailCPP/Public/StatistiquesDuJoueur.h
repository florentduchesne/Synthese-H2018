// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PORTAILCPP_API StatistiquesDuJoueur
{
	
	FString nom = "Joueur ";

public:
	StatistiquesDuJoueur(int NoJoueur);
	~StatistiquesDuJoueur();

	int NbMeurtres = 0;
	int NbMorts = 0;
};

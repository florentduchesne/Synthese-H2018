// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PORTAILCPP_API InformationsNiveau
{
	//nombre de portails possibles dans la piece : devrait etre entre 2 et 4
	int NbPortails;
	//nom du niveau : primordial pour charger le niveau
	FName Nom;

public:
	InformationsNiveau(int NbPortails, FName Nom);
	~InformationsNiveau();
};

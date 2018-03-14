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
	TArray<int> IdPiecesConnectees;
	//nom du niveau : primordial pour charger le niveau
	FName Nom;
	//id unique, permet de savoir avec quel autre niveau celui-ci est connecté
	int Id;

public:
	InformationsNiveau(int NbPortails, FName Nom, int Id);
	~InformationsNiveau();

	int GetNbPortailsNonConnectes();
	void ConnecterNiveau(int IdNiveau);
	TArray<int> GetListeNiveauxConnectes() { return IdPiecesConnectees; };
	int GetId() { return Id; };
	FName GetNom() { return Nom; };
};

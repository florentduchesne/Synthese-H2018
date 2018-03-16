// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Portail.h"

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

	//tous les points d'apparition dans une piece
	TArray<APlayerStart*> ListePointsApparition;

public:
	InformationsNiveau(int NbPortails, FName Nom, int Id);
	~InformationsNiveau();

	//liste de tous les portails du niveau
	TArray<APortail*> listePortails;

	APortail * GetProchainPortailNonConnecte();

	int GetNbPortailsNonConnectes();
	void ConnecterNiveau(int IdNiveau);
	TArray<int> GetListeNiveauxConnectes() { return IdPiecesConnectees; };
	int GetId() { return Id; };
	FName GetNom() { return Nom; };
	void AjouterPointApparition(APlayerStart * PlayerStart);
	TArray<APlayerStart*> GetListePointsApparition() { return ListePointsApparition; };
};

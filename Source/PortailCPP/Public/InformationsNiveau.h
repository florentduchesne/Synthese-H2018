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
	int NbPortailsAConnecter;
	TArray<int> IdPiecesConnectees;
	//nom du niveau : primordial pour charger le niveau
	FName Nom;
	//id unique, permet de savoir avec quel autre niveau celui-ci est connecte
	int Id;

	//tous les points d'apparition dans une piece
	TArray<APlayerStart*> ListePointsApparition;

	

public:
	InformationsNiveau(int NbPortailsAConnecter, FName Nom, int Id);
	~InformationsNiveau();

	//trie le tableau de portails de maniere aleatoire
	void MelangerListePortails();

	//liste de tous les portails du niveau
	TArray<APortail*> listePortails;

	//a MODIFIER POUR eTRE + ALeATOIRE
	APortail * GetProchainPortailNonConnecte();

	int GetNbPortailsNonConnectes() { return NbPortailsAConnecter; };
	void ConnecterNiveau(int IdNiveau);
	TArray<int> GetListeNiveauxConnectes() { return IdPiecesConnectees; };
	int GetId() { return Id; };
	FName GetNom() { return Nom; };
	void AjouterPointApparition(APlayerStart * PlayerStart);
	TArray<APlayerStart*> GetListePointsApparition() { return ListePointsApparition; };
};

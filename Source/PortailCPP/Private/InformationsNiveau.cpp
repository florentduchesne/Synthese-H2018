// Fill out your copyright notice in the Description page of Project Settings.

#include "InformationsNiveau.h"

InformationsNiveau::InformationsNiveau(int NbPortails, FName Nom, int Id)
	:NbPortails{NbPortails}, Nom{Nom},  Id{Id}
{

}

InformationsNiveau::~InformationsNiveau()
{
}

int InformationsNiveau::GetNbPortailsNonConnectes()
{
	return NbPortails - IdPiecesConnectees.Num();
}

void InformationsNiveau::ConnecterNiveau(int IdNiveau)
{
	IdPiecesConnectees.Add(IdNiveau);
}

APortail * InformationsNiveau::GetProchainPortailNonConnecte()
{
	for (APortail * Portail : listePortails)
	{
		if (!Portail->bConnecte)
		{
			Portail->bConnecte = true;
			return Portail;
		}
	}
	return nullptr;
}

void InformationsNiveau::AjouterPointApparition(APlayerStart * PointApparition)
{
	ListePointsApparition.Add(PointApparition);
}
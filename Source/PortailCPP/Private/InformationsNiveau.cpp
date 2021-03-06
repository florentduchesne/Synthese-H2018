// Fill out your copyright notice in the Description page of Project Settings.

#include "InformationsNiveau.h"

InformationsNiveau::InformationsNiveau(int NbPortailsAConnecter, FName Nom, int Id)
	:NbPortailsAConnecter{ NbPortailsAConnecter }, Nom{Nom},  Id{Id}
{

}

InformationsNiveau::~InformationsNiveau()
{
}

void InformationsNiveau::ConnecterNiveau(int IdNiveau)
{
	IdPiecesConnectees.Add(IdNiveau);
	NbPortailsAConnecter--;
	if (NbPortailsAConnecter < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ERREUR NB PORTAILS A CONNECTER < 0"));
	}
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

void InformationsNiveau::MelangerListePortails()
{
	for (int i = listePortails.Num() - 1; i > 0; i--) {
		int j = FMath::FloorToInt(FMath::SRand() * (i + 1)) % listePortails.Num();
		APortail * tempo = listePortails[i];
		listePortails[i] = listePortails[j];
		listePortails[j] = tempo;
	}
}

void InformationsNiveau::AjouterPointApparition(APlayerStart * PointApparition)
{
	ListePointsApparition.Add(PointApparition);
}
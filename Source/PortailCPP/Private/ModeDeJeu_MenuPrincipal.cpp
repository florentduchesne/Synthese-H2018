// Fill out your copyright notice in the Description page of Project Settings.

#include "ModeDeJeu_MenuPrincipal.h"


AModeDeJeu_MenuPrincipal::AModeDeJeu_MenuPrincipal()
	: Super()
{
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = APersonnage::StaticClass();
	UE_LOG(LogTemp, Warning, TEXT("constructeur game mode"));

	for (auto i = 0; i < 4; i++)
	{
		StatsJoueurs[i] = new StatistiquesDuJoueur(i);
	}
}

void AModeDeJeu_MenuPrincipal::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("begin play game mode"));

	ChangeMenuWidget(StartingWidgetClass);
}

void AModeDeJeu_MenuPrincipal::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{

	UE_LOG(LogTemp, Warning, TEXT("change menu widget game mode"));
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void AModeDeJeu_MenuPrincipal::JoueurEnTueUnAutre(int IndexJoueurTueur, int IndexJoueurMort)
{
	StatsJoueurs[IndexJoueurTueur]->NbMeurtres++;
	StatsJoueurs[IndexJoueurMort]->NbMorts++;

	if (StatsJoueurs[IndexJoueurTueur]->NbMeurtres == 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("JOUEUR %d A GAGNÉ LA PARTIE"), IndexJoueurTueur);
	}
}

void AModeDeJeu_MenuPrincipal::GenererCarte(int nbJoueurs)
{
	//contient les informations de tous les niveaux qui peuvent apparaître : nombre de portails, nom, Id
	TArray<InformationsNiveau*> ListeCompleteNiveaux;
	//on rempli avec des valeurs hard-codées...
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Ventilation"), 0));
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Metro"), 1));
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Hall"), 2));

	//les quelques niveaux qui seront choisis pour la partie qui vient
	TArray<InformationsNiveau*> NiveauxChoisis;
	//sera plus élevé quand on aura plus de niveaux de faits
	///IMPORTANT!!! DOIT ÊTRE PLUS PETIT QUE LA TAILLE DE LA LISTE DE NIVEAUX
	const int NbNiveauxVoulus = 3;

	if (NbNiveauxVoulus > ListeCompleteNiveaux.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("ERREUR DANS LE NOMBRE DE NIVEAUX VOULUS"));
		return;
	}

	//on place le nombre de niveaux voulus de la liste des niveaux vers la liste de niveaux choisis
	for (auto i = 0; i < NbNiveauxVoulus; i++)
	{
		bool NiveauTrouve = false;
		while (!NiveauTrouve)
		{
			//on prend un chiffre au hasard
			int IdNiveau = FMath::RandRange(0, ListeCompleteNiveaux.Num() - 1);
			UE_LOG(LogTemp, Warning, TEXT("Id choisi aléatoirement : %d"), IdNiveau);
			//si le niveau n'a pas déjà été choisi, on l'ajoute à la liste. Sinon, on recommence jusqu'à tomber sur un niveau non choisi.
			if (!NiveauxChoisis.Contains(ListeCompleteNiveaux[IdNiveau]))
			{
				NiveauxChoisis.Add(ListeCompleteNiveaux[IdNiveau]);
				NiveauTrouve = true;
			}
		}
	}

	for (auto i = 0; i < NiveauxChoisis.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nom de la carte : %s"), *NiveauxChoisis[i]->GetNom().ToString());
		UE_LOG(LogTemp, Warning, TEXT("Nombre de portails : %d"), NiveauxChoisis[i]->GetNbPortailsNonConnectes());
	}

	//on associe les pièces ensemble de sorte qu'elles soient toutes reliées entre elles d'une manière ou d'une autre
	for (auto i = 0; i < NiveauxChoisis.Num() - 1; i++)
	{
		//si une pièce et sa suivante ont une connexion de libre, on les associe
		if (NiveauxChoisis[i]->GetNbPortailsNonConnectes() && NiveauxChoisis[i + 1]->GetNbPortailsNonConnectes())
		{
			//connecter les deux pieces
			NiveauxChoisis[i]->ConnecterNiveau(i + 1);
			NiveauxChoisis[i + 1]->ConnecterNiveau(i);
		}
	}

	//est vrai si le niveau courant est le seul à avoir des portails non-connectés
	bool NiveauToutSeul;
	int NoRandom;

	//on associe les dernières portes ensemble. Rendu à ce point, toutes les pièces sont accessibles, on arrange donc les portes restantes.
	for (auto i = NiveauxChoisis.Num() - 1; i >= 0; i--)
	{
		while (NiveauxChoisis[i]->GetNbPortailsNonConnectes())
		{
			NiveauToutSeul = true;
			for (auto j = 0; j < NiveauxChoisis.Num(); j++)
			{
				if (NiveauxChoisis[j]->GetNbPortailsNonConnectes() && i != j)
				{
					NiveauToutSeul = false;
				}
			}
			if (!NiveauToutSeul)
			{
				NoRandom = FMath::RandRange(0, NiveauxChoisis.Num() - i);
				if (NiveauxChoisis[NoRandom]->GetNbPortailsNonConnectes() && NoRandom != i)
				{
					//connecter les deux pieces
					NiveauxChoisis[i]->ConnecterNiveau(NoRandom);
					NiveauxChoisis[NoRandom]->ConnecterNiveau(i);
				}
			}
			//si la pièce est la dernière toute seule, on la connecte à elle même avec les deux derniers portails de libres.
			else
			{
				//connecter les deux pieces
				NiveauxChoisis[i]->ConnecterNiveau(i);
				NiveauxChoisis[i]->ConnecterNiveau(i);
			}
		}
	}

	//print
	for (auto i = 0; i < NiveauxChoisis.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nom de la carte : %s"), *NiveauxChoisis[i]->GetNom().ToString());
		UE_LOG(LogTemp, Warning, TEXT("ID de la carte : %d"), NiveauxChoisis[i]->GetId());
		UE_LOG(LogTemp, Warning, TEXT("Nombre de portails libres : %d"), NiveauxChoisis[i]->GetNbPortailsNonConnectes());
		for (auto j = 0; j < NiveauxChoisis[i]->GetListeNiveauxConnectes().Num(); j++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Nom de la carte connectee : %d"), NiveauxChoisis[i]->GetListeNiveauxConnectes()[j]);
		}
	}

	AGestionnaireDeNiveaux* GestionnaireDeNiveaux = GetWorld()->SpawnActor<AGestionnaireDeNiveaux>(AGestionnaireDeNiveaux::StaticClass());
	UE_LOG(LogTemp, Warning, TEXT("Generation de la carte en cours"));

	//on charge tous les niveaux sélectionnés
	for (auto i = 0; i < NiveauxChoisis.Num(); i++)
	{
		GestionnaireDeNiveaux->ChargerNiveau(NiveauxChoisis[i]->GetNom());
	}
}
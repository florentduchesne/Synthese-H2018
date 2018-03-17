// Fill out your copyright notice in the Description page of Project Settings.

#include "ModeDeJeu_MenuPrincipal.h"


AModeDeJeu_MenuPrincipal::AModeDeJeu_MenuPrincipal()
	: Super()
{
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = APersonnage::StaticClass();
	PlayerControllerClass = APlayerController::StaticClass();
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

	//fait apparaître un GestionnaireDeNiveaux
	GestionnaireDeNiveaux = GetWorld()->SpawnActor<AGestionnaireDeNiveaux>(AGestionnaireDeNiveaux::StaticClass());

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

void AModeDeJeu_MenuPrincipal::GenererCarte(int nbJoueurs)
{
	//contient les informations de tous les niveaux qui peuvent apparaître : nombre de portails, nom, Id
	TArray<InformationsNiveau*> ListeCompleteNiveaux;
	//on rempli avec des valeurs hard-codées...
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Ventilation"), 0));
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Metro"), 1));
	ListeCompleteNiveaux.Add(new InformationsNiveau(2, FName("Hall"), 2));
	
	//sera plus élevé quand on aura plus de niveaux de faits
	///IMPORTANT!!! DOIT ÊTRE PLUS PETIT OU ÉGAL À LA TAILLE DE LA LISTE DE NIVEAUX
	const int NbNiveauxVoulus = 3;

	if (NbNiveauxVoulus > ListeCompleteNiveaux.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("ERREUR DANS LE NOMBRE DE NIVEAUX VOULUS"));
		return;
	}

	SelectionnerNiveaux(ListeCompleteNiveaux, NbNiveauxVoulus);

	ChargerLesNiveaux();

	//print
	for (auto i = 0; i < NiveauxChoisis.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nom de la carte : %s"), *NiveauxChoisis[i]->GetNom().ToString());
		UE_LOG(LogTemp, Warning, TEXT("Nombre de portails : %d"), NiveauxChoisis[i]->GetNbPortailsNonConnectes());
	}

	RelierNiveaux();

	//print
	/*for (auto i = 0; i < NiveauxChoisis.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nom de la carte : %s"), *NiveauxChoisis[i]->GetNom().ToString());
		UE_LOG(LogTemp, Warning, TEXT("ID de la carte : %d"), NiveauxChoisis[i]->GetId());
		UE_LOG(LogTemp, Warning, TEXT("Nombre de pieces connectees : %d"), NiveauxChoisis[i]->GetListeNiveauxConnectes().Num());
		for (auto j = 0; j < NiveauxChoisis[i]->GetListeNiveauxConnectes().Num(); j++)
		{
			UE_LOG(LogTemp, Warning, TEXT("ID de la carte connectee : %d"), NiveauxChoisis[i]->GetListeNiveauxConnectes()[j]);
		}
	}*/

	//on va chercher tous les portails de tous les niveaux chargés et on les sauvegarde dans NiveauxChoisis
	TrouverTousLesPortailsCharges(GestionnaireDeNiveaux);

	//print
	/*for (InformationsNiveau* Niveau : NiveauxChoisis)
	{
		UE_LOG(LogTemp, Warning, TEXT("portails du niveau : %s"), *Niveau->GetNom().ToString());
		UE_LOG(LogTemp, Warning, TEXT("nombre de portails : %d"), Niveau->listePortails.Num());
		for (APortail * Portail : Niveau->listePortails)
		{
			if (Portail)
			{
				UE_LOG(LogTemp, Warning, TEXT("nom portail : %s"), *Portail->GetName());
			}
		}
	}*/

	//on connecte les portails pour vrai
	ConnecterLesPortails();

	ChercherPointsApparition();

	PlacerJoueurs(nbJoueurs);
}

void AModeDeJeu_MenuPrincipal::SelectionnerNiveaux(TArray<InformationsNiveau*> ListeCompleteNiveaux, int NbNiveauxVoulus)
{
	//on place le nombre de niveaux voulus de la liste des niveaux complète vers la liste de niveaux choisis
	for (auto i = 0; i < NbNiveauxVoulus; i++)
	{
		bool NiveauTrouve = false;
		while (!NiveauTrouve)
		{
			//on prend un chiffre au hasard
			int IdNiveau = FMath::RandRange(0, ListeCompleteNiveaux.Num() - 1);
			//si le niveau n'a pas déjà été choisi, on l'ajoute à la liste. Sinon, on recommence jusqu'à tomber sur un niveau non choisi.
			if (!NiveauxChoisis.Contains(ListeCompleteNiveaux[IdNiveau]))
			{
				NiveauxChoisis.Add(ListeCompleteNiveaux[IdNiveau]);
				NiveauTrouve = true;
			}
		}
	}
}

AGestionnaireDeNiveaux * AModeDeJeu_MenuPrincipal::ChargerLesNiveaux()
{

	//on charge tous les niveaux sélectionnés
	for (auto i = 0; i < NiveauxChoisis.Num(); i++)
	{
		GestionnaireDeNiveaux->ChargerNiveau(NiveauxChoisis[i]->GetNom(), i);
	}

	return GestionnaireDeNiveaux;
}

void AModeDeJeu_MenuPrincipal::RelierNiveaux()
{
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
}

void AModeDeJeu_MenuPrincipal::TrouverTousLesPortailsCharges(AGestionnaireDeNiveaux* GestionnaireDeNiveaux)
{
	//vérifier si tous les niveaux sont chargés
	bool bTousCharges = true;
	for (InformationsNiveau* Niveau : NiveauxChoisis)
	{
		if (!GestionnaireDeNiveaux->NiveauEstCharge(Niveau->GetNom()))
		{
			bTousCharges = false;
		}
	}

	if (bTousCharges)
	{
		//fait la même chose que l'itérateur
		/*TArray<AActor*> ActeursTrouves;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APortail::StaticClass(), ActeursTrouves);
		for (AActor * ActeurPortail : ActeursTrouves)
		{
			APortail *Portail = Cast<APortail>(ActeurPortail);
			//on vérifie si le portail a un tag qui contient le nom d'un niveau
			for (InformationsNiveau* Niveau : *NiveauxChoisis)
			{
				if (Portail->Tags.Contains(Niveau->GetNom()))
				{
					//on ajoute le portail à la liste de portails du niveau
					Niveau->listePortails.Add(Portail);
					break;
				}
			}
		}*/
		//on va chercher tous les portails de tous les niveaux chargés
		for (TActorIterator<APortail> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			APortail *Portail = *ActorItr;
			//on vérifie si le portail a un tag qui contient le nom d'un niveau
			for (InformationsNiveau* Niveau : NiveauxChoisis)
			{
				if (Portail->Tags.Contains(Niveau->GetNom()))
				{
					//on ajoute le portail à la liste de portails du niveau
					Niveau->listePortails.Add(Portail);
					break;
				}
			}
		}
	}
	else {
		TrouverTousLesPortailsCharges(GestionnaireDeNiveaux);
	}
}

void AModeDeJeu_MenuPrincipal::ConnecterLesPortails()
{
	//on relie les portails 
	for (InformationsNiveau* Niveau : NiveauxChoisis)
	{
		for (int idNiveau : Niveau->GetListeNiveauxConnectes())
		{
			APortail * Portail1 = Niveau->GetProchainPortailNonConnecte();
			APortail * Portail2 = NiveauxChoisis[idNiveau]->GetProchainPortailNonConnecte();
			if (Portail1 && Portail2)
			{
				Portail1->connecterDeuxPortails(Portail2);
			}
			else
			{
				if (Portail1)
				{
					Portail1->bConnecte = false;
				}
				if (Portail2) 
				{
					Portail2->bConnecte = false;
				}
			}
		}
	}
}

void AModeDeJeu_MenuPrincipal::ChercherPointsApparition()
{
	for (TActorIterator<APlayerStart> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		APlayerStart *PointApparition = *ActorItr;
		//on vérifie si le portail a un tag qui contient le nom d'un niveau
		for (InformationsNiveau* Niveau : NiveauxChoisis)
		{
			if (PointApparition->Tags.Contains(Niveau->GetNom()))
			{
				//on ajoute le portail à la liste de portails du niveau
				Niveau->AjouterPointApparition(PointApparition);
				break;
			}
		}
	}
}

void AModeDeJeu_MenuPrincipal::PlacerJoueurs(int NbJoueurs)
{
	//détruit le joueur par defaut
	TArray<AActor*> ActeursTrouves;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), ActeursTrouves);
	for (AActor * Acteur : ActeursTrouves)
	{
		APlayerController * Controleur = Cast<APlayerController>(Acteur);
		if (Controleur)
		{
			UGameplayStatics::RemovePlayer(Controleur, true);
		}
	}
	
	TArray<int> IdNiveauxDejaPris;
	bool bSortir = false;
	for (int i = 0; i < NbJoueurs; i++)
	{
		for (int j = 0; j < NiveauxChoisis.Num(); j++)
		{
			if (!IdNiveauxDejaPris.Contains(j))
			{
				for (APlayerStart * PointApparition : NiveauxChoisis[j]->GetListePointsApparition())
				{
					//on cree un joueur automatiquement et on vérifie si il a bien été créé
					APlayerController * Controleur = UGameplayStatics::CreatePlayer(PointApparition, i, true);
					if (Controleur)
					{
						APawn * Pion = Controleur->GetPawn();
						APersonnage * Personnage = Cast<APersonnage>(Pion);
						Personnage->SetNoJoueur(i);
						IdNiveauxDejaPris.Add(j);
						break;
					}
				}
				//break;
			}
		}
	}
}


void AModeDeJeu_MenuPrincipal::PartieTerminee(int idNoJoueurGagnant)
{

}

void AModeDeJeu_MenuPrincipal::DechargerCarte()
{

}

void AModeDeJeu_MenuPrincipal::DetruireTousLesJoueurs()
{

}



void AModeDeJeu_MenuPrincipal::JoueurEnTueUnAutre(int IndexJoueurTueur, int IndexJoueurMort)
{
	StatsJoueurs[IndexJoueurTueur]->NbMeurtres++;
	StatsJoueurs[IndexJoueurMort]->NbMorts++;

	if (StatsJoueurs[IndexJoueurTueur]->NbMeurtres == 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("JOUEUR %d A GAGNE LA PARTIE!!!!!!!!!!!!!!!!"), IndexJoueurTueur);
	}
}
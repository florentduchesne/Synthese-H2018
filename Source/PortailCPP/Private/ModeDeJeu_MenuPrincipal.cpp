// Fill out your copyright notice in the Description page of Project Settings.

#include "ModeDeJeu_MenuPrincipal.h"

AModeDeJeu_MenuPrincipal::AModeDeJeu_MenuPrincipal()
	: Super()
{
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = APersonnage::StaticClass();
	PlayerControllerClass = APlayerController::StaticClass();

	for (auto i = 0; i < 4; i++)
	{
		StatsJoueurs[i] = new StatistiquesDuJoueur(i);
	}

	NoJoueurGagnant = -1;
}

void AModeDeJeu_MenuPrincipal::BeginPlay()
{
	Super::BeginPlay();

	//fait apparaître un GestionnaireDeNiveaux
	GestionnaireDeNiveaux = GetWorld()->SpawnActor<AGestionnaireDeNiveaux>(AGestionnaireDeNiveaux::StaticClass());
	//affiche le menu principal
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

void AModeDeJeu_MenuPrincipal::GenererCarte(int _NbJoueurs)
{
	NbJoueurs = _NbJoueurs;

	//contient les informations de tous les niveaux qui peuvent apparaître : nombre de portails, nom, Id
	TArray<InformationsNiveau*> ListeCompleteNiveaux;
	//on rempli avec des valeurs hard-codées...
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Ventilation"), 0));
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Metro"), 1));
	ListeCompleteNiveaux.Add(new InformationsNiveau(2, FName("Hall"), 2));

	if (NbNiveauxVoulus > ListeCompleteNiveaux.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("ERREUR DANS LE NOMBRE DE NIVEAUX VOULUS"));
		return;
	}

	//on sélectionne quels niveaux seront utilisés dans la partie,
	SelectionnerNiveaux(ListeCompleteNiveaux, NbNiveauxVoulus);
	//on demande au gestionnaire de niveaux de charger tous les niveaux
	ChargerLesNiveaux();
	//on sélectionne quels niveaux seront reliés avec quels autres
	RelierNiveaux();
	//quand tous les niveaux ont terminé de charger, on connecte les portails, place les joueurs et démarre la partie
	InitialiserCarte();
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

void AModeDeJeu_MenuPrincipal::ChargerLesNiveaux()
{
	//on charge tous les niveaux sélectionnés
	for (auto i = 0; i < NiveauxChoisis.Num(); i++)
	{
		GestionnaireDeNiveaux->ChargerNiveau(NiveauxChoisis[i]->GetNom(), i);
	}
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

void AModeDeJeu_MenuPrincipal::InitialiserCarte()
{
	if (NiveauxTousCharges())
	{
		UE_LOG(LogTemp, Warning, TEXT("niveaux tous charges"));
		//on va chercher tous les portails de tous les niveaux chargés et on les sauvegarde dans NiveauxChoisis
		TrouverTousLesPortailsCharges();

		//on connecte les portails pour vrai
		ConnecterLesPortails();

		//on va chercher tous les points d'apparition
		ChercherPointsApparition();

		//détruit tous les joueurs
		DetruireTousLesJoueurs();

		//on place tous les joueurs
		PlacerJoueurs();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("niveaux pas tous charges"));
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AModeDeJeu_MenuPrincipal::InitialiserCarte, 0.1f, false);
	}
}

void AModeDeJeu_MenuPrincipal::TrouverTousLesPortailsCharges()
{
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
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
	for (AActor* Acteur : FoundActors)
	{
		APlayerStart* PointApparition = Cast<APlayerStart>(Acteur);
		if (PointApparition)
		{
			for (InformationsNiveau* Niveau : NiveauxChoisis)
			{
				if (PointApparition->Tags.Contains(Niveau->GetNom()))
				{
					//on ajoute le portail à la liste de portails du niveau
					Niveau->AjouterPointApparition(PointApparition);
					UE_LOG(LogTemp, Warning, TEXT("Point apparition trouve"));
					break;
				}
			}
		}
	}
}

void AModeDeJeu_MenuPrincipal::DetruireTousLesJoueurs()
{
	//va chercher tous les objets APersonnage
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APersonnage::StaticClass(), FoundActors);
	for (AActor * Acteur : FoundActors)
	{
		//supprime le joueur
		APersonnage * Personnage = Cast<APersonnage>(Acteur);
		APlayerController * Controleur = Cast<APlayerController>(Personnage->GetController());
		UGameplayStatics::RemovePlayer(Controleur, true);
	}
}

void AModeDeJeu_MenuPrincipal::PlacerJoueurs()
{	
	TArray<int> IdNiveauxDejaPris;
	bool bSortir = false;
	for (int NoJoueur = 0; NoJoueur < NbJoueurs; NoJoueur++)
	{
		for (int j = 0; j < NiveauxChoisis.Num(); j++)
		{
			if (!IdNiveauxDejaPris.Contains(j))
			{
				for (APlayerStart * PointApparition : NiveauxChoisis[j]->GetListePointsApparition())
				{
					FaireApparaitreJoueur(PointApparition, NoJoueur);
					break;
				}
				break;
			}
		}
	}
}

void AModeDeJeu_MenuPrincipal::FaireApparaitreJoueur(UObject * PointApparition, int NoJoueur)
{
	//on cree un joueur automatiquement et on vérifie si il a bien été créé
	APlayerController * Controleur = UGameplayStatics::CreatePlayer(PointApparition, NoJoueur, true);
	if (Controleur)
	{
		AttendreQueJoueurChargent(Controleur, NoJoueur);
	}
}

void AModeDeJeu_MenuPrincipal::AttendreQueJoueurChargent(APlayerController * Controleur, int NoJoueur)
{
	APawn * Pion = Controleur->GetPawn();
	APersonnage * Personnage = Cast<APersonnage>(Pion);
	if (Personnage)
	{
		Personnage->SetNoJoueur(NoJoueur);
		Personnage->SetATH(Controleur->GetHUD());
	}
	else
	{
		FTimerDelegate TimerDel;
		FTimerHandle TimerHandle;
		//on rappelle la fonction dans 0.1 seconde
		TimerDel.BindUFunction(this, FName("AttendreQueJoueurChargent"), Controleur, NoJoueur);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.1f, false);
	}
}

void AModeDeJeu_MenuPrincipal::PartieTerminee(int idNoJoueurGagnant)
{
	//décharge tous les niveaux
	DechargerCarte();

	//supprime tous les niveaux sélectionnés pour la dernière partie
	NiveauxChoisis.Empty();

	//réinitialise les stats des joueurs
	for (auto i = 0; i < 4; i++)
	{
		StatsJoueurs[i] = new StatistiquesDuJoueur(i);
	}

	//détruit tous les joueurs
	DetruireTousLesJoueurs();

	PlacerUnUniqueJoueur();

	//retour au menu principal
	ChangeMenuWidget(StartingWidgetClass);
}

void AModeDeJeu_MenuPrincipal::DechargerCarte()
{
	//boucle à travers tous les niveaux et les décharge
	for (auto i = 0; i < NiveauxChoisis.Num(); i++)
	{
		GestionnaireDeNiveaux->DechargerNiveau(NiveauxChoisis[i]->GetNom(), i);
	}
}

void AModeDeJeu_MenuPrincipal::JoueurEnTueUnAutre(int IndexJoueurTueur, int IndexJoueurMort)
{
	//actualise les statistiques des joueurs
	StatsJoueurs[IndexJoueurTueur]->NbMeurtres++;
	StatsJoueurs[IndexJoueurMort]->NbMorts++;

	//si un joueur gagne, met fin à la partie
	if (StatsJoueurs[IndexJoueurTueur]->NbMeurtres == NbMeutresRequisPourVictoire)
	{
		NoJoueurGagnant = IndexJoueurTueur;
		PartieTerminee(IndexJoueurTueur);
	}
	//sinon faire réapparaître le joueur mort
	else {
		ReapparitionJoueur(IndexJoueurMort);
	}
}

void AModeDeJeu_MenuPrincipal::ReapparitionJoueur(int NoJoueur)
{
	APlayerStart * PointApparition = TrouverPointApparitionAleatoire();
	if (PointApparition)
	{
		for (TActorIterator<APlayerController> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			APlayerController * Controleur = *ActorItr;
			APawn * Pion = Controleur->GetPawn();
			if (Pion)
			{
				APersonnage * Personnage = Cast<APersonnage>(Pion);
				if (Personnage)
				{
					if (Personnage->GetNoJoueur() == NoJoueur)
					{
						UE_LOG(LogTemp, Warning, TEXT("1 JOUEUR reapparu"));
						Personnage->ReinitialiserStatistiques();
						FRotator rotation = Personnage->GetControlRotation();
						rotation.Yaw = Personnage->GetActorRotation().Yaw + PointApparition->GetActorRotation().Yaw;
						Personnage->GetController()->SetControlRotation(rotation);

						FVector position = PointApparition->GetActorLocation();


						FHitResult HitResult;
						Personnage->SetActorLocation(position, false, &HitResult, ETeleportType::None);
						return;
					}
				}
			}
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("point apparition manquant"));
}

void AModeDeJeu_MenuPrincipal::DetruireJoueur(int NoJoueur)
{
	for (TActorIterator<APlayerController> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		APlayerController * Controleur = *ActorItr;
		APawn * Pion = Controleur->GetPawn();
		if (Pion)
		{
			APersonnage * Personnage = Cast<APersonnage>(Pion);
			if (Personnage)
			{
				if (Personnage->GetNoJoueur() == NoJoueur)
				{
					UGameplayStatics::RemovePlayer(Controleur, true);
					break;
				}
			}
		}
	}
}

APlayerStart * AModeDeJeu_MenuPrincipal::TrouverPointApparitionAleatoire()
{
	int NoRandom = FMath::RandRange(0, NiveauxChoisis.Num() - 1);
	APlayerStart * PointApparition{ nullptr };
	while (!PointApparition)
	{
		for (APlayerStart * PointApparition : NiveauxChoisis[NoRandom]->GetListePointsApparition())
		{
			return PointApparition;
		}
	}
	return nullptr;
}

bool AModeDeJeu_MenuPrincipal::NiveauxTousCharges()
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
	return bTousCharges;
}

void AModeDeJeu_MenuPrincipal::PlacerUnUniqueJoueur()
{
	//fait réapparaitre un unique joueur pour avoir le controle du menu principal
	APlayerStart * PointApparition = NewObject<APlayerStart>(this);
	FaireApparaitreJoueur(PointApparition, 0);
	PointApparition->Destroy();
}
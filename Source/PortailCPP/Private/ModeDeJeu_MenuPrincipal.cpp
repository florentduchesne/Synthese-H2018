// Fill out your copyright notice in the Description page of Project Settings.

#include "ModeDeJeu_MenuPrincipal.h"

AModeDeJeu_MenuPrincipal::AModeDeJeu_MenuPrincipal()
	: Super()
{
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = APersonnage::StaticClass();
	PlayerControllerClass = APlayerController::StaticClass();

	NoJoueurGagnant = -1;
}

void AModeDeJeu_MenuPrincipal::BeginPlay()
{
	Super::BeginPlay();

	//fait apparaitre un GestionnaireDeNiveaux
	GestionnaireDeNiveaux = GetWorld()->SpawnActor<AGestionnaireDeNiveaux>(AGestionnaireDeNiveaux::StaticClass());
	//affiche le menu principal
	ChangeMenuWidget(StartingWidgetClass);
}

void AModeDeJeu_MenuPrincipal::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	//UE_LOG(LogTemp, Warning, TEXT("change menu widget game mode"));
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

void AModeDeJeu_MenuPrincipal::GenererCarte(int _NbJoueurs, int nb_pieces, int nb_points_victoire, int duree, UPARAM(ref) TArray<float>& _SensibiliteH, UPARAM(ref) TArray<float>& _SensibiliteV)
{
	NbJoueurs = _NbJoueurs;
	NbNiveauxVoulus = nb_pieces;
	NbMeutresRequisPourVictoire = nb_points_victoire;
	TempsMaxPartie = duree;
	SensibiliteH = _SensibiliteH;
	SensibiliteV = _SensibiliteV;

	GetOuter()->GetWorld()->GetTimerManager().SetTimer(TimerHandleFinDePartie, this, &AModeDeJeu_MenuPrincipal::TerminerPartieTimer, TempsMaxPartie, false);

	InitialiserStatsJoueurs();

	//contient les informations de tous les niveaux qui peuvent apparaitre : nombre de portails, nom, Id
	TArray<InformationsNiveau*> ListeCompleteNiveaux;
	//on rempli avec des valeurs hard-codees...
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Ventilation"), 0));
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Metro"), 1));
	ListeCompleteNiveaux.Add(new InformationsNiveau(2, FName("Hall"), 2));
	ListeCompleteNiveaux.Add(new InformationsNiveau(2, FName("SalleD"), 3));
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Appartement"), 4));
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Salle2"), 5));
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Chapelle"), 6));
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Epicerie"), 7));

	if (NbNiveauxVoulus > ListeCompleteNiveaux.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("ERREUR DANS LE NOMBRE DE NIVEAUX VOULUS"));
		return;
	}

	//on selectionne quels niveaux seront utilises dans la partie,
	SelectionnerNiveaux(ListeCompleteNiveaux, NbNiveauxVoulus);

	/*
	for (InformationsNiveau * Niveau : NiveauxChoisis)
	{
		UE_LOG(LogTemp, Warning, TEXT("niveau choisi : %s"), *Niveau->GetNom().ToString());
	}*/

	//on demande au gestionnaire de niveaux de charger tous les niveaux
	ChargerLesNiveaux();
	//on selectionne quels niveaux seront relies avec quels autres
	RelierNiveaux();
	//quand tous les niveaux ont termine de charger, on connecte les portails, place les joueurs et demarre la partie
	InitialiserCarte();
}

void AModeDeJeu_MenuPrincipal::InitialiserStatsJoueurs()
{
	StatsJoueurs.Empty();

	for (auto i = 0; i < NbJoueurs; i++)
	{
		StatsJoueurs.Add(NewObject<UStatistiquesDuJoueur>(this, FName("StatJoueur%s", i)));
		StatsJoueurs[i]->SetNoJoueur(i);
	}
}

void AModeDeJeu_MenuPrincipal::SelectionnerNiveaux(TArray<InformationsNiveau*> ListeCompleteNiveaux, int NbNiveauxVoulus)
{
	//on place le nombre de niveaux voulus de la liste des niveaux complete vers la liste de niveaux choisis
	for (auto i = 0; i < NbNiveauxVoulus; i++)
	{
		bool NiveauTrouve = false;
		while (!NiveauTrouve)
		{
			//on prend un chiffre au hasard
			int IdNiveau = FMath::RandRange(0, ListeCompleteNiveaux.Num() - 1);
			//si le niveau n'a pas deja ete choisi, on l'ajoute a la liste. Sinon, on recommence jusqu'a tomber sur un niveau non choisi.
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
	//on charge tous les niveaux selectionnes
	for (auto i = 0; i < NiveauxChoisis.Num(); i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("on charge un niveau"));
		GestionnaireDeNiveaux->ChargerNiveau(NiveauxChoisis[i]->GetNom(), i);
	}
}

void AModeDeJeu_MenuPrincipal::RelierNiveaux()
{
	//on associe les pieces ensemble de sorte qu'elles soient toutes reliees entre elles d'une maniere ou d'une autre
	for (int i = 0; i < NiveauxChoisis.Num() - 1; i++)
	{
		//si une piece et sa suivante ont une connexion de libre, on les associe
		if (NiveauxChoisis[i]->GetNbPortailsNonConnectes() && NiveauxChoisis[i + 1]->GetNbPortailsNonConnectes())
		{
			//connecter les deux pieces
			NiveauxChoisis[i]->ConnecterNiveau(i + 1);
			NiveauxChoisis[i + 1]->ConnecterNiveau(i);
		}
	}
	//est vrai si le niveau courant est le seul a avoir des portails non-connectes
	bool NiveauToutSeul;
	int NoRandom;

	//on associe les dernieres portes ensemble. Rendu a ce point, toutes les pieces sont accessibles, on arrange donc les portes restantes.
	for (int i = NiveauxChoisis.Num() - 1; i >= 0; i--)
	{
		while (NiveauxChoisis[i]->GetNbPortailsNonConnectes())
		{
			NiveauToutSeul = true;
			//on verifie si le niveau est le dernier a avoir des connexions de libre
			for (auto j = 0; j < NiveauxChoisis.Num(); j++)
			{
				if (NiveauxChoisis[j]->GetNbPortailsNonConnectes() && i != j)
				{
					NiveauToutSeul = false;
				}
			}
			//si non, on le connecte avec une des dernieres pieces
			if (!NiveauToutSeul)
			{
				NoRandom = FMath::RandRange(0, i - 1);
				if (NiveauxChoisis[NoRandom]->GetNbPortailsNonConnectes() && NoRandom != i)
				{
					//connecter les deux pieces
					NiveauxChoisis[i]->ConnecterNiveau(NoRandom);
					NiveauxChoisis[NoRandom]->ConnecterNiveau(i);
				}
			}
			//si la piece est la derniere toute seule, on la connecte a elle meme avec les deux derniers portails de libres.
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
	//UE_LOG(LogTemp, Warning, TEXT("joueurs existants : %d"), NombreDeJoueursExistants());
	DetruireTousLesJoueurs();

	if (NiveauxTousCharges())
	{
		if (CompterPortails())
		{
			//UE_LOG(LogTemp, Warning, TEXT("joueurs existants2 : %d"), NombreDeJoueursExistants());
			if (NombreDeJoueursExistants() <= 1)
			{
				//UE_LOG(LogTemp, Warning, TEXT("joueurs existants3 : %d"), NombreDeJoueursExistants());
				//UE_LOG(LogTemp, Warning, TEXT("niveaux tous charges"));
				//on va chercher tous les portails de tous les niveaux charges et on les sauvegarde dans NiveauxChoisis
				TrouverTousLesPortailsCharges();

				//on connecte les portails pour vrai
				ConnecterLesPortails();

				//on va chercher tous les points d'apparition
				ChercherPointsApparition();

				//on place tous les joueurs
				PlacerJoueurs();

				//on affiche le widget qui contient le timer
				if (NbJoueurs > 1)
				{
					ChangeMenuWidget(WidgetEnPartie);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("personnages pas tous detruits"));
				FTimerHandle UnusedHandle;
				GetWorldTimerManager().SetTimer(UnusedHandle, this, &AModeDeJeu_MenuPrincipal::InitialiserCarte, 0.1f, false);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("portails pas tous charges"));
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &AModeDeJeu_MenuPrincipal::InitialiserCarte, 0.1f, false);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("niveaux pas tous charges"));
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AModeDeJeu_MenuPrincipal::InitialiserCarte, 0.1f, false);
	}
}

bool AModeDeJeu_MenuPrincipal::CompterPortails()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APortail::StaticClass(), FoundActors);
	int nbPortailsTrouves = FoundActors.Num();
	int nbPortailsTheorique = 0;
	for (InformationsNiveau* Niveau : NiveauxChoisis)
	{
		nbPortailsTheorique += Niveau->GetListeNiveauxConnectes().Num();
	}
	//UE_LOG(LogTemp, Warning, TEXT("portails charges : %d"), nbPortailsTrouves);
	//UE_LOG(LogTemp, Warning, TEXT("portails theorique : %d"), nbPortailsTheorique);
	return nbPortailsTrouves == nbPortailsTheorique;
}

void AModeDeJeu_MenuPrincipal::TrouverTousLesPortailsCharges()
{
	//on va chercher tous les portails de tous les niveaux charges (equivalent a GetAllActorsOfClass)
	for (TActorIterator<APortail> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		APortail *Portail = *ActorItr;
		//UE_LOG(LogTemp, Warning, TEXT("portail trouve %s"), *Portail->Tags[0].ToString());

		//FPlatformProcess::Sleep(0.5f);
		//on verifie si le portail a un tag qui contient le nom d'un niveau
		for (InformationsNiveau* Niveau : NiveauxChoisis)
		{
			//UE_LOG(LogTemp, Warning, TEXT("avec niveau %s"), *Niveau->GetNom().ToString());
			if (Portail->Tags.Contains(Niveau->GetNom()))
			{
				//on ajoute le portail a la liste de portails du niveau
				Niveau->listePortails.Add(Portail);
				break;
			}
		}
	}
}

void AModeDeJeu_MenuPrincipal::ConnecterLesPortails()
{
	//on trie tous les portails de tous les niveaux de maniere aleatoire
	for (InformationsNiveau * Niveau : NiveauxChoisis)
	{
		Niveau->MelangerListePortails();
	}

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
	//UE_LOG(LogTemp, Warning, TEXT("nb points apparition total : %d"), FoundActors.Num());
	for (AActor* Acteur : FoundActors)
	{
		APlayerStart* PointApparition = Cast<APlayerStart>(Acteur);
		if (PointApparition)
		{
			for (InformationsNiveau* Niveau : NiveauxChoisis)
			{
				if (PointApparition->Tags.Contains(Niveau->GetNom()))
				{
					//on ajoute le portail a la liste de portails du niveau
					Niveau->AjouterPointApparition(PointApparition);
					break;
				}
			}
		}
	}
}

int AModeDeJeu_MenuPrincipal::NombreDeJoueursExistants()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APersonnage::StaticClass(), FoundActors);
	return FoundActors.Num();
}

void AModeDeJeu_MenuPrincipal::DetruireTousLesJoueurs()
{
	//va chercher tous les objets APersonnage
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), FoundActors);
	//UE_LOG(LogTemp, Warning, TEXT("nb joueurs : %d"), FoundActors.Num());
	for (AActor * Acteur : FoundActors)
	{
		//supprime le joueur
		APlayerController * Controleur = Cast<APlayerController>(Acteur);
		UGameplayStatics::RemovePlayer(Controleur, true);
		//UE_LOG(LogTemp, Warning, TEXT("joueur detruit"));
	}
}

void AModeDeJeu_MenuPrincipal::PlacerJoueurs()
{
	//a modifier pour qu'on ne puisse pas faire apparaitre deux joueurs dans la meme piece (retirer le commentaire)
	TArray<int> IdNiveauxDejaPris;
	for (int NoJoueur = 0; NoJoueur < NbJoueurs; NoJoueur++)
	{
		for (int j = 0; j < NiveauxChoisis.Num(); j++)
		{
			if (!IdNiveauxDejaPris.Contains(j))
			{
				//UE_LOG(LogTemp, Warning, TEXT("on va creer un joueur... %d nb points apparition"), NiveauxChoisis[j]->GetListePointsApparition().Num());
				//UE_LOG(LogTemp, Warning, TEXT("nom niveau : %s"), *NiveauxChoisis[j]->GetNom().ToString());
				for (APlayerStart * PointApparition : NiveauxChoisis[j]->GetListePointsApparition())
				{
					FaireApparaitreJoueur(PointApparition, NoJoueur);
					IdNiveauxDejaPris.Add(j);
					break;
				}
				break;
			}
		}
	}
	//maintenant que les personnages sont parametres, on n'a plus besoin de la sensibilite
	/*SensibiliteH.Empty();
	SensibiliteV.Empty();*/
}

void AModeDeJeu_MenuPrincipal::FaireApparaitreJoueur(AActor * PointApparition, int NoJoueur)
{
	//on cree un joueur automatiquement et on verifie si il a bien ete cree
	APlayerController * Controleur = UGameplayStatics::CreatePlayer(PointApparition, NoJoueur, true);
	if (Controleur)
	{
		//UE_LOG(LogTemp, Warning, TEXT("joueur cree"));
		AttendreQueJoueurCharge(Controleur, NoJoueur, PointApparition);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("controleur n existe pas"));
		APersonnage * Personnage = GetJoueurParIndex(-1);
		if (Personnage)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Personnage 0 existe"));
			Personnage->SetNoJoueur(NoJoueur);
			Controleur = Cast<APlayerController>(Personnage->GetController());
			Personnage->SetATH(Controleur->GetHUD());
			Personnage->SetSensibilite(SensibiliteH[NoJoueur], SensibiliteV[NoJoueur]);
			Personnage->SetActorLocation(PointApparition->GetActorLocation());
			Personnage->ChangerArme(UFusilSemiAuto::StaticClass());
		}
	}
}

void AModeDeJeu_MenuPrincipal::AttendreQueJoueurCharge(APlayerController * Controleur, int NoJoueur, AActor * PointApparition)
{
	APawn * Pion = Controleur->GetPawn();
	APersonnage * Personnage = Cast<APersonnage>(Pion);
	if (Personnage)
	{
		Personnage->SetNoJoueur(NoJoueur);
		Personnage->SetSensibilite(SensibiliteH[NoJoueur], SensibiliteV[NoJoueur]);
		Personnage->SetATH(Controleur->GetHUD());
		Personnage->SetActorLocation(PointApparition->GetActorLocation());
		Personnage->ChangerArme(UFusilSemiAuto::StaticClass());
	}
	else
	{
		FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &AModeDeJeu_MenuPrincipal::AttendreQueJoueurCharge, Controleur, NoJoueur, PointApparition);
		FTimerHandle TimerHandle;
		//UE_LOG(LogTemp, Warning, TEXT("joueur pas charge"));
		//on rappelle la fonction dans 0.1 seconde
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.1f, false);
	}
	
}

void AModeDeJeu_MenuPrincipal::TerminerPartieTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("timer termine"));
	int PlusGrandNombreDeMeurtres = 0;
	NoJoueurGagnant = -1;

	for (auto i = 0; i < NbJoueurs; i++)
	{
		if (StatsJoueurs[i]->NbMeurtres > PlusGrandNombreDeMeurtres)
		{
			PlusGrandNombreDeMeurtres = StatsJoueurs[i]->NbMeurtres;
			NoJoueurGagnant = i;
		}
	}
	PartieTerminee(NoJoueurGagnant);
}

void AModeDeJeu_MenuPrincipal::RetourMenuPrincipal()
{
	//detruit tous les joueurs
	if (NombreDeJoueursExistants() == 1)
	{
		//retour au menu principal
		ChangeMenuWidget(StartingWidgetClass);
		UE_LOG(LogTemp, Warning, TEXT("afficher widget menu"));
	}
	else
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AModeDeJeu_MenuPrincipal::RetourMenuPrincipal, 0.1f, false);
	}
}

void AModeDeJeu_MenuPrincipal::PartieTerminee(int idNoJoueurGagnant)
{
	DetruireTousLesJoueurs();

	PlacerUnUniqueJoueur();

	//arrete le timer de fin de partie
	GetWorldTimerManager().ClearTimer(TimerHandleFinDePartie);

	//decharge tous les niveaux
	DechargerCarte();

	//supprime tous les niveaux selectionnes pour la derniere partie
	NiveauxChoisis.Empty();

	RetourMenuPrincipal();
}

void AModeDeJeu_MenuPrincipal::DechargerCarte()
{
	//boucle a travers tous les niveaux et les decharge
	for (auto i = 0; i < NiveauxChoisis.Num(); i++)
	{
		GestionnaireDeNiveaux->DechargerNiveau(NiveauxChoisis[i]->GetNom(), i);
	}
}

void AModeDeJeu_MenuPrincipal::JoueurEnTueUnAutre(int IndexJoueurTueur, int IndexJoueurMort)
{
	//actualise les statistiques des joueurs
	if(IndexJoueurTueur != IndexJoueurMort)
		StatsJoueurs[IndexJoueurTueur]->NbMeurtres++;
	StatsJoueurs[IndexJoueurMort]->NbMorts++;

	APersonnage * Personnage = GetJoueurParIndex(IndexJoueurTueur);
	if (Personnage)
	{
		Personnage->MiseAJourNbMeurtresATH(StatsJoueurs[IndexJoueurTueur]->NbMeurtres);
	}

	//si un joueur gagne, met fin a la partie
	if (StatsJoueurs[IndexJoueurTueur]->NbMeurtres == NbMeutresRequisPourVictoire)
	{
		NoJoueurGagnant = IndexJoueurTueur;
		PartieTerminee(IndexJoueurTueur);
	}
	//sinon faire reapparaitre le joueur mort
	else if (StatsJoueurs[IndexJoueurTueur]->NbMeurtres < NbMeutresRequisPourVictoire)
	{
		ReapparitionJoueur(IndexJoueurMort);
	}
}

void AModeDeJeu_MenuPrincipal::ReapparitionJoueur(int NoJoueur)
{
	APlayerStart * PointApparition = TrouverPointApparitionAleatoire();
	if (PointApparition)
	{
		APersonnage * Personnage = GetJoueurParIndex(NoJoueur);
		if (Personnage)
		{
			//UE_LOG(LogTemp, Warning, TEXT("1 JOUEUR reapparu"));
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
	else
		UE_LOG(LogTemp, Warning, TEXT("point apparition manquant"));
}

void AModeDeJeu_MenuPrincipal::DetruireJoueur(int NoJoueur)
{
	APersonnage * Personnage = GetJoueurParIndex(NoJoueur);
	if (Personnage)
	{
		APlayerController * Controleur = Cast<APlayerController>(Personnage->GetController());
		UGameplayStatics::RemovePlayer(Controleur, true);
	}
}

APlayerStart * AModeDeJeu_MenuPrincipal::TrouverPointApparitionAleatoire()
{
	int NoRandom;
	APlayerStart * PointApparition{ nullptr };
	while (!PointApparition)
	{
		NoRandom = FMath::RandRange(0, NiveauxChoisis.Num() - 1);
		for (APlayerStart * Point : NiveauxChoisis[NoRandom]->GetListePointsApparition())
		{
			PointApparition = Point;
		}
	}
	return PointApparition;
}

bool AModeDeJeu_MenuPrincipal::NiveauxTousCharges()
{
	//verifier si tous les niveaux sont charges
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
	//fait reapparaitre un unique joueur pour avoir le controle du menu principal
	APlayerStart * PointApparition = NewObject<APlayerStart>(this);
	//FaireApparaitreJoueur(PointApparition, 0);
	UGameplayStatics::CreatePlayer(PointApparition, 0, true);
	PointApparition->Destroy();
}

APersonnage * AModeDeJeu_MenuPrincipal::GetJoueurParIndex(int NoJoueur)
{
	for (TActorIterator<APersonnage> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		APersonnage * Personnage = *ActorItr;
		if (Personnage)
		{
			if (Personnage->GetNoJoueur() == NoJoueur)
			{
				return Personnage;
			}
		}
	}
	return nullptr;
}
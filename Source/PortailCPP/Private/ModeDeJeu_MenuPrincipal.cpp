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

	//fait appara�tre un GestionnaireDeNiveaux
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

void AModeDeJeu_MenuPrincipal::GenererCarte(int _NbJoueurs, int nb_pieces)
{
	NbJoueurs = _NbJoueurs;
	NbNiveauxVoulus = nb_pieces;

	InitialiserStatsJoueurs();

	//contient les informations de tous les niveaux qui peuvent appara�tre : nombre de portails, nom, Id
	TArray<InformationsNiveau*> ListeCompleteNiveaux;
	//on rempli avec des valeurs hard-cod�es...
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Ventilation"), 0));
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Metro"), 1));
	ListeCompleteNiveaux.Add(new InformationsNiveau(2, FName("Hall"), 2));
	ListeCompleteNiveaux.Add(new InformationsNiveau(2, FName("SalleD"), 3));
	ListeCompleteNiveaux.Add(new InformationsNiveau(4, FName("Appartement"), 4));

	if (NbNiveauxVoulus > ListeCompleteNiveaux.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("ERREUR DANS LE NOMBRE DE NIVEAUX VOULUS"));
		return;
	}

	//on s�lectionne quels niveaux seront utilis�s dans la partie,
	SelectionnerNiveaux(ListeCompleteNiveaux, NbNiveauxVoulus);

	for (InformationsNiveau * Niveau : NiveauxChoisis)
	{
		UE_LOG(LogTemp, Warning, TEXT("niveau choisi : %s"), *Niveau->GetNom().ToString());
	}
	//on demande au gestionnaire de niveaux de charger tous les niveaux
	ChargerLesNiveaux();
	//on s�lectionne quels niveaux seront reli�s avec quels autres
	RelierNiveaux();
	//quand tous les niveaux ont termin� de charger, on connecte les portails, place les joueurs et d�marre la partie
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
	//on place le nombre de niveaux voulus de la liste des niveaux compl�te vers la liste de niveaux choisis
	for (auto i = 0; i < NbNiveauxVoulus; i++)
	{
		bool NiveauTrouve = false;
		while (!NiveauTrouve)
		{
			//on prend un chiffre au hasard
			int IdNiveau = FMath::RandRange(0, ListeCompleteNiveaux.Num() - 1);
			//si le niveau n'a pas d�j� �t� choisi, on l'ajoute � la liste. Sinon, on recommence jusqu'� tomber sur un niveau non choisi.
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
	//on charge tous les niveaux s�lectionn�s
	for (auto i = 0; i < NiveauxChoisis.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("on charge un niveau"));
		GestionnaireDeNiveaux->ChargerNiveau(NiveauxChoisis[i]->GetNom(), i);
	}
}

void AModeDeJeu_MenuPrincipal::RelierNiveaux()
{
	//on associe les pi�ces ensemble de sorte qu'elles soient toutes reli�es entre elles d'une mani�re ou d'une autre
	for (int i = 0; i < NiveauxChoisis.Num() - 1; i++)
	{
		//si une pi�ce et sa suivante ont une connexion de libre, on les associe
		if (NiveauxChoisis[i]->GetNbPortailsNonConnectes() && NiveauxChoisis[i + 1]->GetNbPortailsNonConnectes())
		{
			//connecter les deux pieces
			NiveauxChoisis[i]->ConnecterNiveau(i + 1);
			NiveauxChoisis[i + 1]->ConnecterNiveau(i);
		}
	}
	//est vrai si le niveau courant est le seul � avoir des portails non-connect�s
	bool NiveauToutSeul;
	int NoRandom;

	//on associe les derni�res portes ensemble. Rendu � ce point, toutes les pi�ces sont accessibles, on arrange donc les portes restantes.
	for (int i = NiveauxChoisis.Num() - 1; i >= 0; i--)
	{
		while (NiveauxChoisis[i]->GetNbPortailsNonConnectes())
		{
			NiveauToutSeul = true;
			//on v�rifie si le niveau est le dernier � avoir des connexions de libre
			for (auto j = 0; j < NiveauxChoisis.Num(); j++)
			{
				if (NiveauxChoisis[j]->GetNbPortailsNonConnectes() && i != j)
				{
					NiveauToutSeul = false;
				}
			}
			//si non, on le connecte avec une des derni�res pi�ces
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
			//si la pi�ce est la derni�re toute seule, on la connecte � elle m�me avec les deux derniers portails de libres.
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
		//on va chercher tous les portails de tous les niveaux charg�s et on les sauvegarde dans NiveauxChoisis
		TrouverTousLesPortailsCharges();

		//on connecte les portails pour vrai
		ConnecterLesPortails();

		//on va chercher tous les points d'apparition
		ChercherPointsApparition();

		//d�truit tous les joueurs
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
	//on va chercher tous les portails de tous les niveaux charg�s
	for (TActorIterator<APortail> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		APortail *Portail = *ActorItr;
		//on v�rifie si le portail a un tag qui contient le nom d'un niveau
		for (InformationsNiveau* Niveau : NiveauxChoisis)
		{
			if (Portail->Tags.Contains(Niveau->GetNom()))
			{
				//on ajoute le portail � la liste de portails du niveau
				Niveau->listePortails.Add(Portail);
				break;
			}
		}
	}
}

void AModeDeJeu_MenuPrincipal::ConnecterLesPortails()
{
	//on trie tous les portails de tous les niveaux de mani�re al�atoire
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
	UE_LOG(LogTemp, Warning, TEXT("nb points apparition total : %d"), FoundActors.Num());
	for (AActor* Acteur : FoundActors)
	{
		APlayerStart* PointApparition = Cast<APlayerStart>(Acteur);
		if (PointApparition)
		{
			for (InformationsNiveau* Niveau : NiveauxChoisis)
			{
				if (PointApparition->Tags.Contains(Niveau->GetNom()))
				{
					//on ajoute le portail � la liste de portails du niveau
					Niveau->AjouterPointApparition(PointApparition);
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
	//� modifier pour qu'on ne puisse pas faire apparaitre deux joueurs dans la m�me pi�ce (retirer le commentaire)
	TArray<int> IdNiveauxDejaPris;
	for (int NoJoueur = 0; NoJoueur < NbJoueurs; NoJoueur++)
	{
		for (int j = 0; j < NiveauxChoisis.Num(); j++)
		{
			if (!IdNiveauxDejaPris.Contains(j))
			{
				UE_LOG(LogTemp, Warning, TEXT("on va creer un joueur... %d nb points apparition"), NiveauxChoisis[j]->GetListePointsApparition().Num());
				UE_LOG(LogTemp, Warning, TEXT("nom niveau : %s"), *NiveauxChoisis[j]->GetNom().ToString());
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
}

void AModeDeJeu_MenuPrincipal::FaireApparaitreJoueur(AActor * PointApparition, int NoJoueur)
{
	//on cree un joueur automatiquement et on v�rifie si il a bien �t� cr��
	UE_LOG(LogTemp, Warning, TEXT("joueur cree"));
	APlayerController * Controleur = UGameplayStatics::CreatePlayer(PointApparition, NoJoueur, true);
	if (Controleur)
	{
		AttendreQueJoueurCharge(Controleur, NoJoueur, PointApparition);
	}
}

void AModeDeJeu_MenuPrincipal::AttendreQueJoueurCharge(APlayerController * Controleur, int NoJoueur, AActor * PointApparition)
{
	APawn * Pion = Controleur->GetPawn();
	APersonnage * Personnage = Cast<APersonnage>(Pion);
	if (Personnage)
	{
		Personnage->SetNoJoueur(NoJoueur);
		Personnage->SetATH(Controleur->GetHUD());
		Personnage->SetActorLocation(PointApparition->GetActorLocation());
	}
	else
	{
		FTimerDelegate TimerDel;
		FTimerHandle TimerHandle;
		//on rappelle la fonction dans 0.1 seconde
		TimerDel.BindUFunction(this, FName("AttendreQueJoueurCharge"), Controleur, NoJoueur);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.1f, false);
	}
}

void AModeDeJeu_MenuPrincipal::PartieTerminee(int idNoJoueurGagnant)
{
	//d�charge tous les niveaux
	DechargerCarte();

	//supprime tous les niveaux s�lectionn�s pour la derni�re partie
	NiveauxChoisis.Empty();

	//r�initialise les stats des joueurs
	/*for (auto i = 0; i < 4; i++)
	{
		StatsJoueurs.Add(NewObject<UStatistiquesDuJoueur>(this, FName("StatJoueur%s", i)));
		StatsJoueurs[i]->SetNoJoueur(i);
	}*/

	//d�truit tous les joueurs
	DetruireTousLesJoueurs();

	PlacerUnUniqueJoueur();

	//retour au menu principal
	ChangeMenuWidget(StartingWidgetClass);
}

void AModeDeJeu_MenuPrincipal::DechargerCarte()
{
	//boucle � travers tous les niveaux et les d�charge
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

	//si un joueur gagne, met fin � la partie
	if (StatsJoueurs[IndexJoueurTueur]->NbMeurtres == NbMeutresRequisPourVictoire)
	{
		NoJoueurGagnant = IndexJoueurTueur;
		PartieTerminee(IndexJoueurTueur);
	}
	//sinon faire r�appara�tre le joueur mort
	else {
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
	//v�rifier si tous les niveaux sont charg�s
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
	//fait r�apparaitre un unique joueur pour avoir le controle du menu principal
	APlayerStart * PointApparition = NewObject<APlayerStart>(this);
	FaireApparaitreJoueur(PointApparition, 0);
	PointApparition->Destroy();
}

APersonnage * AModeDeJeu_MenuPrincipal::GetJoueurParIndex(int NoJoueur)
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
					return Personnage;
				}
			}
		}
	}

	return nullptr;
}
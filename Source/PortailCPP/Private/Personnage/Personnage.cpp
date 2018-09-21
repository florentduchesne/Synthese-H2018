// Fill out your copyright notice in the Description page of Project Settings.

#include "Personnage.h"
#include "PortailCPP/Public/ModeDeJeu_MenuPrincipal.h"

// Sets default values
APersonnage::APersonnage()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	corps = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshCorps"));
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(TEXT("/Game/Personnage/Ch_Personnage"));
	corps->SetSkeletalMeshWithoutResettingAnimation(MeshObj.Object);
	
	corps->SetRelativeLocation(FVector(0, 0, -100.0f));
	corps->SetRelativeRotation(FRotator(0, -90.0f, 0));

	corps->SetOwnerNoSee(true);

	corps->SetAllMassScale(5.0f);
	
	RootComponent = GetCapsuleComponent();

	corps->SetupAttachment(RootComponent);

	// Cree une camera
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// attache la camera a la capsule
	camera->SetupAttachment(RootComponent);
	camera->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f + BaseEyeHeight));
	// Permet au personnage de controler la rotation de la camera
	camera->bUsePawnControlRotation = true;
	
	//on change es statistiques de deplacement du personnage
	GetCharacterMovement()->MaxWalkSpeed = 1500;
	GetCharacterMovement()->MaxAcceleration = 3000;
	GetCharacterMovement()->AirControl = 1.5f;
	GetCharacterMovement()->JumpZVelocity = 840.0f;
	GetCharacterMovement()->GravityScale = 2.0f;
}

// Called when the game starts or when spawned
void APersonnage::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APersonnage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APersonnage::SetNoJoueur(int _NoJoueur)
{
	NoJoueur = _NoJoueur;
	UE_LOG(LogTemp, Warning, TEXT("no joueur : %d"), NoJoueur);

	//va chercher le chemin du materiau (se termine obligatoirement par 1, 2, 3 ou 4)
	FString CheminMateriau = FString("/Game/Personnage/MT_BodyPlayer");
	CheminMateriau.AppendInt(NoJoueur + 1);

	//set le materiau du corps
	UMaterialInstanceConstant * materiauPtr = Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(), nullptr, *CheminMateriau));
	if (materiauPtr)
	{
		corps->SetMaterial(0, materiauPtr);
	}
}

int APersonnage::GetNoJoueur()
{
	return NoJoueur;
}

// Called to bind functionality to input
void APersonnage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//deplacements
	PlayerInputComponent->BindAxis("Avancer", this, &APersonnage::Avancer);
	PlayerInputComponent->BindAxis("DeplacementLateral", this, &APersonnage::DeplacementLateral);
	//rotation
	PlayerInputComponent->BindAxis("Turn", this, &APersonnage::TournerHorizontalement);
	PlayerInputComponent->BindAxis("LookUp", this, &APersonnage::TournerVerticalement);
	//saut
	PlayerInputComponent->BindAction("Sauter", IE_Pressed, this, &APersonnage::DebuterSaut);
	PlayerInputComponent->BindAction("Sauter", IE_Released, this, &APersonnage::TerminerSaut);

	///pas le choix de passer par une fonction membre de APersonnage, sinon quand on change d'arme ca brise tout...

	//tir
	PlayerInputComponent->BindAction("Tirer", IE_Pressed, this, &APersonnage::CommencerTir);
	PlayerInputComponent->BindAction("Tirer", IE_Released, this, &APersonnage::TerminerTir);
	//tir secondaire
	PlayerInputComponent->BindAction("TirerSecondaire", IE_Pressed, this, &APersonnage::TirSecondaire);
	//recharger
	PlayerInputComponent->BindAction("Recharger", IE_Pressed, this, &APersonnage::Rechargement);

	//retour au menu principal (quitter la partie)
	PlayerInputComponent->BindAction("RetourMenuPrincipal", IE_Pressed, this, &APersonnage::RetourMenuPrincipal);
}

void APersonnage::RetourMenuPrincipal()
{
	UE_LOG(LogTemp, Warning, TEXT("menu"));
	AModeDeJeu_MenuPrincipal * GameMode = Cast<AModeDeJeu_MenuPrincipal>(GetOuter()->GetWorld()->GetAuthGameMode());
	if(GameMode)
		GameMode->TerminerPartieTimer();
}

void APersonnage::Avancer(float Value)
{
	// prend la direction dans laquelle le personnage regarde et le fait avancer sur cet axe
	const FRotator YawOnlyRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	AddMovementInput(FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::X), Value);
}

void APersonnage::DeplacementLateral(float Value)
{
	// prend la direction dans laquelle le personnage regarde et le fait avancer laterallement
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void APersonnage::TournerHorizontalement(float Value)
{
	AddControllerYawInput(Value * GetWorld()->GetDeltaSeconds() * 40.0f * SensibiliteHorizontale);
}

void APersonnage::TournerVerticalement(float Value)
{
	AddControllerPitchInput(Value * GetWorld()->GetDeltaSeconds() * 40.0f * SensibiliteVerticale);
}

//gere par l'engine...
void APersonnage::DebuterSaut()
{
	bPressedJump = true;
}
//gere par l'engine...
void APersonnage::TerminerSaut()
{
	bPressedJump = false;
}

void APersonnage::DebloquerTeleportationFutur()
{
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &APersonnage::DebloquerTeleportation, 0.1f, false);
}

void APersonnage::DebloquerTeleportation()
{
	bPeutSeTeleporter = true;
}

void APersonnage::BloquerTeleportation()
{
	bPeutSeTeleporter = false;
}

bool APersonnage::PeutSeTeleporter()
{
	return bPeutSeTeleporter;
}

void APersonnage::InfligerDegats(int degats, int NoJoueurAttaquant)
{
	if (Armure)
	{
		Armure -= degats;
		if (Armure < 0)
		{
			PointsDeVie += Armure;
			Armure = 0;
		}
	}
	else
	{
		PointsDeVie -= degats;
	}
	if (ATH) 
	{
		ATH->MiseAJourPV(PointsDeVie);
		ATH->MiseAJourArmure(Armure);
	}
	if (PointsDeVie <= 0)
	{
		AModeDeJeu_MenuPrincipal * GameMode = Cast<AModeDeJeu_MenuPrincipal>(GetOuter()->GetWorld()->GetAuthGameMode());
		GameMode->JoueurEnTueUnAutre(NoJoueurAttaquant, NoJoueur);
	}
}

bool APersonnage::Soigner(int NbPointsDeVie)
{
	if (PointsDeVie == 100)
	{
		return false;
	}
	else
	{
		PointsDeVie += NbPointsDeVie;
		if (PointsDeVie > 100)
		{
			PointsDeVie = 100;
		}
		if (ATH)
		{
			ATH->MiseAJourPV(PointsDeVie);
		}
		return true;
	}
}

bool APersonnage::EquiperArmure(int NbArmure)
{
	if (Armure == 100)
	{
		return false;
	}
	else
	{
		Armure += NbArmure;
		if (Armure > 100)
		{
			Armure = 100;
		}
		if (ATH)
		{
			ATH->MiseAJourArmure(Armure);
		}
		return true;
	}
}

void APersonnage::Recharger()
{
	arme->LancerRechargement();
}

void APersonnage::ReinitialiserStatistiques()
{
	ChangerArme(UFusilSemiAuto::StaticClass());
	 
	PointsDeVie = 100;
	Armure = 0;

	ATH->MiseAJourPV(PointsDeVie);
	ATH->MiseAJourArmure(Armure);
	ATH->MiseAJourBallesDansChargeur(arme->GetBallesDansChargeur());
	ATH->MiseAJourBallesMax(arme->GetBallesMax());
}

bool APersonnage::ChangerArme(TSubclassOf<UArme> SousClasseDeArme)
{
	if (!SousClasseDeArme)
	{
		UE_LOG(LogTemp, Warning, TEXT("ERREUR, pas de sous classe d arme!!!"));
		return false;
	}
	if (arme)
	{
		if (SousClasseDeArme == arme->GetClass())
		{
			UE_LOG(LogTemp, Warning, TEXT("meme arme!"));
			return false;
		}
		UE_LOG(LogTemp, Warning, TEXT("detruire arme"));
		arme->DetruireArme();
		arme->UnregisterComponent();
		arme->DestroyComponent();
	}
	arme = nullptr;
	
	if (SousClasseDeArme == UFusilSemiAuto::StaticClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("semi auto"));
		arme = NewObject<UFusilSemiAuto>(this, FName("FusilSemiAuto"));
	}
	else if (SousClasseDeArme == UFusilAuto::StaticClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("auto"));
		arme = NewObject<UFusilAuto>(this, FName("FusilAuto"));
	}
	else if (SousClasseDeArme == UFusilARafales::StaticClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("rafales"));
		arme = NewObject<UFusilARafales>(this, FName("FusilARafales"));
	}
	else if (SousClasseDeArme == UFusilAPompe::StaticClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("pompe"));
		arme = NewObject<UFusilAPompe>(this, FName("FusilAPompe"));
	}
	UE_LOG(LogTemp, Warning, TEXT("nom arme : %s"), *arme->GetName());

	arme->RegisterComponent(); 
	/*arme->SetWorldLocation(Location);
	arme->SetWorldRotation(Rotation);
	*/
	//arme->AttachTo(GetRootComponent(), SocketName, EAttachLocation::KeepWorldPosition);

	
	UE_LOG(LogTemp, Warning, TEXT("attacher arme"));
	/*arme->AttachToComponent(camera, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	arme->SetRelativeLocation(FVector(50.0f, 35.0f, -20.0f));
	*/
	arme->Attacher(camera);
	arme->SetRelativeLocation(FVector(50.0f, 35.0f, -20.0f));
	arme->SetNoJoueur(GetNoJoueur());
	
	if (ATH)
	{
		ATH->MiseAJourBallesMax(arme->GetBallesMax());
		ATH->MiseAJourBallesDansChargeur(arme->GetBallesDansChargeur());
	}
	return true;
}

void APersonnage::CommencerTir()
{
	if(arme)
		arme->CommencerTirSuper();
	else
		UE_LOG(LogTemp, Warning, TEXT("pas arme"));
}

void APersonnage::TerminerTir()
{
	if(arme)
		arme->TerminerTirSuper();
	else
		UE_LOG(LogTemp, Warning, TEXT("pas arme"));
}

void APersonnage::TirSecondaire()
{
	arme->TirSecondaire();
}

void APersonnage::Rechargement()
{
	arme->LancerRechargement();
}

void APersonnage::SetSensibilite(float sensiHorizontale, float sensiVerticale)
{
	SensibiliteHorizontale = sensiHorizontale;
	SensibiliteVerticale = sensiVerticale;
}

void APersonnage::SetATH(AHUD * HUD)
{
	ATH = Cast<AATH>(HUD);
	//ATH->MiseAJourBallesMax(arme->GetBallesMax());
	//ATH->MiseAJourBallesDansChargeur(arme->GetBallesDansChargeur());
}

void APersonnage::MiseAJourNbMeurtresATH(int Meurtres)
{
	ATH->MiseAJourMeutres(Meurtres);
}

void APersonnage::MiseAJourBallesDansChargeur(int NbBalles)
{
	ATH->MiseAJourBallesDansChargeur(NbBalles);
}

void APersonnage::MiseAJourBallesMax(int NbBalles)
{
	ATH->MiseAJourBallesMax(NbBalles);
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "Personnage.h"
#include "PortailCPP/Public/ModeDeJeu_MenuPrincipal.h"

// Sets default values
APersonnage::APersonnage()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	corps = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshCorps"));
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(TEXT("/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP"));
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

	arme = CreateDefaultSubobject<UFusilSemiAuto>(TEXT("Arme"));

	arme->SetupAttachment(camera);

	arme->getMesh()->SetupAttachment(camera);

	arme->getMesh()->SetRelativeLocation(FVector(50.0f, 35.0f, -20.0f));

	arme->getMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	
	//on change es statistiques de déplacement du personnage
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
	arme->SetNoJoueur(NoJoueur);
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

	///pas le choix de passer par une fonction membre de APersonnage, sinon quand on change d'arme ça brise tout...

	//tir
	PlayerInputComponent->BindAction("Tirer", IE_Pressed, this, &APersonnage::CommencerTir);
	PlayerInputComponent->BindAction("Tirer", IE_Released, this, &APersonnage::TerminerTir);
	//tir secondaire
	PlayerInputComponent->BindAction("TirerSecondaire", IE_Pressed, this, &APersonnage::TirSecondaire);
	//recharger
	PlayerInputComponent->BindAction("Recharger", IE_Pressed, this, &APersonnage::Rechargement);
}

void APersonnage::Avancer(float Value)
{
	// prend la direction dans laquelle le personnage regarde et le fait avancer sur cet axe
	const FRotator YawOnlyRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	AddMovementInput(FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::X), Value);
}

void APersonnage::DeplacementLateral(float Value)
{
	// prend la direction dans laquelle le personnage regarde et le fait avancer latérallement
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void APersonnage::TournerHorizontalement(float Value)
{
	AddControllerYawInput(Value);
}

void APersonnage::TournerVerticalement(float Value)
{
	AddControllerPitchInput(Value);
}

//géré par l'engine...
void APersonnage::DebuterSaut()
{
	bPressedJump = true;
}
//géré par l'engine...
void APersonnage::TerminerSaut()
{
	bPressedJump = false;
}

void APersonnage::DebloquerTeleportationFutur()
{
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &APersonnage::DebloquerTeleportation, 0.5f, false);
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
	if (SousClasseDeArme == arme->StaticClass())
	{
		return false;
	}
	arme->DestroyComponent();
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
	//UE_LOG(LogTemp, Warning, TEXT("classe arme : %s"), *SousClasseDeArme);
	UE_LOG(LogTemp, Warning, TEXT("nom arme : %s"), *arme->GetName());
	UE_LOG(LogTemp, Warning, TEXT("nom arme mesh : %s"), *arme->getMesh()->GetName());

	arme->SetupAttachment(camera);
	arme->getMesh()->SetupAttachment(camera);
	arme->getMesh()->SetRelativeLocation(FVector(50.0f, 35.0f, -20.0f));
	arme->getMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	arme->SetNoJoueur(GetNoJoueur());
	arme->RegisterComponent();

	ATH->MiseAJourBallesMax(arme->GetBallesMax());
	ATH->MiseAJourBallesDansChargeur(arme->GetBallesDansChargeur());

	return true;
}

void APersonnage::CommencerTir()
{
	arme->CommencerTirSuper();
}

void APersonnage::TerminerTir()
{
	arme->TerminerTirSuper();
}

void APersonnage::TirSecondaire()
{
	arme->TirSecondaire();
}

void APersonnage::Rechargement()
{
	arme->LancerRechargement();
}

void APersonnage::SetATH(AHUD * HUD)
{
	ATH = Cast<AATH>(HUD);
	ATH->MiseAJourBallesMax(arme->GetBallesMax());
	ATH->MiseAJourBallesDansChargeur(arme->GetBallesDansChargeur());
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
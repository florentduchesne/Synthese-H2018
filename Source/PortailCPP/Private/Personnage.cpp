// Fill out your copyright notice in the Description page of Project Settings.

#include "Personnage.h"
#include "./Public/ModeDeJeu_MenuPrincipal.h"

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

	UE_LOG(LogTemp, Warning, TEXT("constructeur personnage"));
	
	RootComponent = GetCapsuleComponent();

	corps->SetupAttachment(RootComponent);

	// Cree une camera avec le modele par defaut
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// attache la camera a la capsule
	camera->SetupAttachment(RootComponent);
	camera->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f + BaseEyeHeight));
	// Permet au personnage de controler la rotation de la camera
	camera->bUsePawnControlRotation = true;

	arme = CreateDefaultSubobject<UFusilARafales>(TEXT("Arme"));

	arme->SetNoJoueur(NoJoueur);

	arme->SetupAttachment(camera);

	arme->getMesh()->SetupAttachment(camera);

	arme->getMesh()->SetRelativeLocation(FVector(50.0f, 35.0f, -20.0f));

	arme->getMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

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
	//tir
	PlayerInputComponent->BindAction("Tirer", IE_Pressed, arme, &UArme::CommencerTirSuper);
	PlayerInputComponent->BindAction("Tirer", IE_Released, arme, &UArme::TerminerTirSuper);
	//recharger
	PlayerInputComponent->BindAction("Recharger", IE_Pressed, arme, &UArme::LancerRechargement);
}

void APersonnage::Avancer(float Value)
{
	// prend la direction dans laquelle le personnage regarde et le fait avancer sur cet axe
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
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

	if (PointsDeVie <= 0)
	{
		AModeDeJeu_MenuPrincipal * GameMode = Cast<AModeDeJeu_MenuPrincipal>(GetOuter()->GetWorld()->GetAuthGameMode());
		GameMode->JoueurEnTueUnAutre(NoJoueurAttaquant, NoJoueur);
		UE_LOG(LogTemp, Warning, TEXT("JE SUIS MORT"));
		PointsDeVie = 100;
	}
	UE_LOG(LogTemp, Warning, TEXT("ARMURE DU PERSONNAGE %d"), Armure);
	UE_LOG(LogTemp, Warning, TEXT("PV DU PERSONNAGE %d"), PointsDeVie);
	UE_LOG(LogTemp, Warning, TEXT("DEGATS INFLIGES PAR : %d"), NoJoueurAttaquant);
}

void APersonnage::Recharger()
{
	arme->LancerRechargement();
}
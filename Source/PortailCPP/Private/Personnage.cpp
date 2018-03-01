// Fill out your copyright notice in the Description page of Project Settings.

#include "Personnage.h"


// Sets default values
APersonnage::APersonnage()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*corps = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshCorps"));
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(TEXT("/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP"));
	corps->SetSkeletalMeshWithoutResettingAnimation(MeshObj.Object);*/
	//RootComponent = corps;
	//corps->SetRelativeLocation(FVector(0, 0, -88.0f));

	// Cree une camera avec le modele par defaut
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// attache la camera a la capsule
	camera->SetupAttachment(RootComponent);
	camera->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f + BaseEyeHeight));
	// Permet au personnage de controler la rotation de la camera
	camera->bUsePawnControlRotation = true;

	// Cree un mesh par defaut
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	// Il n'y a que ce joueur qui peut voir le mesh
	FPSMesh->SetOnlyOwnerSee(true);
	// Attache le mesh a la camera
	FPSMesh->SetupAttachment(camera);
	// Desactive les ombres
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;
}

// Called when the game starts or when spawned
void APersonnage::BeginPlay()
{
	Super::BeginPlay();
	/*if (GEngine)
	{
		// Put up a debug message for five seconds. The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));
	}*/
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

void APersonnage::DebuterSaut()
{
	bPressedJump = true;
}

void APersonnage::TerminerSaut()
{
	bPressedJump = false;
}

void APersonnage::debloquer()
{
	bPeutSeTeleporter = true;
}

void APersonnage::debloquerFutur()
{
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &APersonnage::debloquer, 0.5f, false);
}

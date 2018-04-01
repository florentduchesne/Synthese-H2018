// Fill out your copyright notice in the Description page of Project Settings.

#include "Portail.h"


// Sets default values
APortail::APortail()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	cadre = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshCadre"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/Geometry/Meshes/cadrePortail"));
	cadre->SetStaticMesh(MeshObj.Object);
	panneau = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshPanneau"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj2(TEXT("/Game/Geometry/Meshes/panneauPortail"));
	panneau->SetStaticMesh(MeshObj2.Object);
	RootComponent = cadre;
	panneau->SetupAttachment(cadre);

	panneau->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	cadre->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	FVector position = cadre->GetComponentLocation();
	position.Z += 120.0f;
	FRotator rotation = cadre->GetComponentRotation();
	rotation.Yaw += 90.0f;
	rotation.Roll += 90.0f;

	panneau->SetRelativeLocationAndRotation(position, rotation);

	OnActorBeginOverlap.AddDynamic(this, &APortail::OnTeleportation);
}

// Called when the game starts or when spawned
void APortail::BeginPlay()
{
	Super::BeginPlay();
	
}

void APortail::OnTeleportation(AActor* overlappedActor, AActor* otherActor)
{
	//cast de l'acteur en Personnage
	APersonnage * Personnage = Cast<APersonnage>(otherActor);
	if (Personnage)
	{
		if (Personnage->PeutSeTeleporter() && autrePortail)
		{
			//le personnage fait dos au portail quand il en sort
			//la rotation du personnage + la rotation de l'autre portail - ma rotation + 180
			FRotator rotation = Personnage->GetControlRotation();
			rotation.Yaw = Personnage->GetActorRotation().Yaw + autrePortail->GetActorRotation().Yaw - GetActorRotation().Yaw + 180.0f;
			Personnage->GetController()->SetControlRotation(rotation);
			//on lui enleve le droit de se teleporter pour eviter un stackoverflow
			Personnage->BloquerTeleportation();

			//teleporter le joueur un peu devant l'autre portail
			FVector position;
			position = autrePortail->GetActorLocation() - autrePortail->GetActorForwardVector() * 80;
			position.Z += 60.0f;
			FHitResult HitResult;
			Personnage->SetActorLocation(position, false, &HitResult, ETeleportType::None);//ETeleportType est None, ce qui (supposément) annule tout effet de physique lorsqu'on sort du portail.
			
			//on enlève la physique que le joueur pouvait avoir au moment de la téléportation (si il sautait, par exemple)
			Personnage->GetCharacterMovement()->StopMovementImmediately();

			//FVector Direction = autrePortail->GetActorForwardVector();//FRotationMatrix(autrePortail->GetActorRotation()).GetScaledAxis(EAxis::X);
			//Personnage->GetCharacterMovement()->AddForce((Direction * 150000000) + FVector(0, 0, 10000000));

			//on appelle une fonction qui lui redonne le droit de se teleporter dans une seconde
			Personnage->DebloquerTeleportationFutur();
		}
	}	
}

void APortail::connecterDeuxPortails(APortail * portail)
{
	this->autrePortail = portail;
	if (!autrePortail->estConnecte())
		autrePortail->connecterDeuxPortails(this);
}

// Called every frame
void APortail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool APortail::estConnecte()
{
	if (autrePortail)
		return true;
	else
		return false;
}
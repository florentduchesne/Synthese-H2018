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
	auto perso = Cast<APersonnage>(otherActor);
	if (perso)
	{
		if (perso->bPeutSeTeleporter && autrePortail)
		{
			//le personnage fait dos au portail quand il en sort
			//la rotation du personnage + la rotation de l'autre portail - ma rotation + 180
			FRotator rotation = perso->GetControlRotation();
			rotation.Yaw = perso->GetActorRotation().Yaw + autrePortail->GetActorRotation().Yaw - GetActorRotation().Yaw + 180.0f;
			perso->GetController()->SetControlRotation(rotation);
			//on lui enleve le droit de se teleporter pour eviter un stackoverflow
			perso->bPeutSeTeleporter = false;

			//teleporter le joueur sur la distance qui sépare les deux portails
			FVector position = perso->GetActorLocation();
			position += autrePortail->GetActorLocation();
			position -= GetActorLocation();
			position.Z = autrePortail->GetActorLocation().Z;
			FHitResult HitResult;
			perso->SetActorLocation(position, false, &HitResult, ETeleportType::None);//ETeleportType est None, ce qui annule tout effet de physique lorsqu'on sort du portail.

			//on envoie le personnage au sol pour eviter qu'il continue dans la mauvaise direction au cas ou il serait dans un saut au moment de passer le portail.
			FVector force;
			force.Z = -5000.0f;
			perso->AddMovementInput(force);
			perso->GetCharacterMovement()->Velocity += force;

			//on appelle une fonction qui lui redonne le droit de se teleporter dans une seconde
			perso->debloquerFutur();
		}
	}	
}

void APortail::connecterDeuxPortails(APortail * portail)
{
	this->autrePortail = portail;
}

// Called every frame
void APortail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


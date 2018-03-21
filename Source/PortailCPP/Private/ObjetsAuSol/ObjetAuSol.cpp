// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjetAuSol.h"


// Sets default values
AObjetAuSol::AObjetAuSol(){}

AObjetAuSol::AObjetAuSol(float _DelaisAvantReapparition, FString CheminMesh, ETypeDeMeshEnum TypeDeMesh)
	:DelaisAvantReapparition{ _DelaisAvantReapparition }
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereCollisions = CreateDefaultSubobject<USphereComponent>(FName("SphereCollisions"));
	SphereCollisions->InitSphereRadius(50.0f);
	SphereCollisions->SetRelativeLocation(FVector(0, 0, 50));
	SphereCollisions->SetCollisionProfileName(TEXT("OverlapAll"));
	SphereCollisions->SetSimulatePhysics(false);
	RootComponent = SphereCollisions;

	if (TypeDeMesh == ETypeDeMeshEnum::SkeletalMesh)
	{
		Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshObjet"));
		const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj((TEXT("%s"), *CheminMesh));

		USkeletalMeshComponent * SkeletalMesh = Cast<USkeletalMeshComponent>(Mesh);

		SkeletalMesh->SetSkeletalMesh(MeshObj.Object);

		SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		SkeletalMesh->SetupAttachment(RootComponent);
	}
	else if (TypeDeMesh == ETypeDeMeshEnum::StaticMesh)
	{
		Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshObjet"));
		const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj((TEXT("%s"), *CheminMesh));

		UStaticMeshComponent * StaticMesh = Cast<UStaticMeshComponent>(Mesh);

		StaticMesh->SetStaticMesh(MeshObj.Object);

		StaticMesh->SetupAttachment(RootComponent);

		StaticMesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));

		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	OnActorBeginOverlap.AddDynamic(this, &AObjetAuSol::OnCollision);
}

// Called when the game starts or when spawned
void AObjetAuSol::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjetAuSol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObjetAuSol::OnCollision(AActor * overlappedActor, AActor * otherActor)
{
	if (EstDisponible)
	{
		APersonnage * Personnage = Cast<APersonnage>(otherActor);
		if (Personnage)
		{
			if (RamasseParUnJoueur(Personnage))
			{
				//detruire le mesh
				UStaticMeshComponent * StaticMesh = Cast<UStaticMeshComponent>(Mesh);
				if (StaticMesh)
				{
					StaticMesh->SetVisibility(false);
				}
				else
				{
					USkeletalMeshComponent * SkeletalMesh = Cast<USkeletalMeshComponent>(Mesh);
					if (SkeletalMesh)
					{
						SkeletalMesh->SetVisibility(false);
					}
				}
				//reapparaitre avec delai
				EstDisponible = false;
				FTimerHandle UnusedHandle;
				GetWorldTimerManager().SetTimer(UnusedHandle, this, &AObjetAuSol::Reapparition, DelaisAvantReapparition, false);
			}
		}
	}
}

void AObjetAuSol::Reapparition()
{
	EstDisponible = true;

	UStaticMeshComponent * StaticMesh = Cast<UStaticMeshComponent>(Mesh);
	if (StaticMesh)
	{
		StaticMesh->SetVisibility(true);
	}
	else
	{
		USkeletalMeshComponent * SkeletalMesh = Cast<USkeletalMeshComponent>(Mesh);
		if (SkeletalMesh)
		{
			SkeletalMesh->SetVisibility(true);
		}
	}
}
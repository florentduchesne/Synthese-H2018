// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjetAuSol.h"


// Sets default values
AObjetAuSol::AObjetAuSol(){}

AObjetAuSol::AObjetAuSol(float _DelaisAvantReapparition, FString CheminMesh, ETypeDeMeshEnum TypeDeMesh, ECouleurSocleEnum CouleurSocle)
	:DelaisAvantReapparition{ _DelaisAvantReapparition }
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereCollisions = CreateDefaultSubobject<USphereComponent>(FName("SphereCollisions"));
	SphereCollisions->InitSphereRadius(30.0f);
	SphereCollisions->SetRelativeLocation(FVector(0, 0, 50));
	SphereCollisions->SetCollisionProfileName(FName("ObjetAuSol"));
	RootComponent = SphereCollisions;

	//set mesh socle
	MeshSocle = CreateDefaultSubobject<UStaticMeshComponent>(FName("Socle"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshSocleObj((TEXT("%s"), *(FString("/Game/Niveaux/Appartement/Assets/Meshes/HoloCone"))));
	MeshSocle->SetStaticMesh(MeshSocleObj.Object);

	MeshSocle->SetupAttachment(RootComponent);
	MeshSocle->SetRelativeLocation(FVector(0, 0, -65));

	MeshSocle->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//set materiau socle
	if (CouleurSocle == ECouleurSocleEnum::Bleu)
	{
		const ConstructorHelpers::FObjectFinder<UMaterial> MateriauObj(TEXT("/Game/Niveaux/Appartement/Assets/Materials/HoloCone"));
		MateriauSocle = (UMaterial*)MateriauObj.Object;
	}
	else if (CouleurSocle == ECouleurSocleEnum::Vert)
	{
		const ConstructorHelpers::FObjectFinder<UMaterial> MateriauObj(TEXT("/Game/Niveaux/Appartement/Assets/Materials/HoloCone"));
		MateriauSocle = (UMaterial*)MateriauObj.Object;
	}
	else
	{
		const ConstructorHelpers::FObjectFinder<UMaterial> MateriauObj(TEXT("/Game/Niveaux/Appartement/Assets/Materials/HoloCone"));
		MateriauSocle = (UMaterial*)MateriauObj.Object;
	}

	if (MateriauSocle)
	{
		MeshSocle->SetMaterial(0, MateriauSocle);
	}

	//set mesh objet flottant
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
		//StaticMesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	OnActorBeginOverlap.AddDynamic(this, &AObjetAuSol::OnCollision);
}

// Called when the game starts or when spawned
void AObjetAuSol::BeginPlay()
{
	Super::BeginPlay();
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
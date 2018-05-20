// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjetAuSol.h"


// Sets default values
AObjetAuSol::AObjetAuSol(){}

AObjetAuSol::AObjetAuSol(float _DelaisAvantReapparition, FString CheminMesh, ETypeDeMeshEnum TypeDeMesh, ECouleurObjetEnum CouleurSocle)
	:DelaisAvantReapparition{ _DelaisAvantReapparition }
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SphereCollisions = CreateDefaultSubobject<USphereComponent>(FName("SphereCollisions"));
	SphereCollisions->InitSphereRadius(50.0f);
	SphereCollisions->SetRelativeLocation(FVector(0, 0, 65));
	SphereCollisions->SetCollisionProfileName(FName("ObjetAuSol"));
	RootComponent = SphereCollisions;

	//set mesh socle
	MeshSocle = CreateDefaultSubobject<UStaticMeshComponent>(FName("Socle"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshSocleObj((TEXT("%s"), *(FString("/Game/Objets/Meshes/HoloPickup_Projector"))));
	MeshSocle->SetStaticMesh(MeshSocleObj.Object);

	MeshSocle->SetupAttachment(RootComponent);
	MeshSocle->SetRelativeLocation(FVector(0, 0, -65));

	MeshSocle->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//set mesh cone
	MeshCone = CreateDefaultSubobject<UStaticMeshComponent>(FName("Cone"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshConeObj((TEXT("%s"), *(FString("/Game/Objets/Meshes/HoloCone"))));
	MeshCone->SetStaticMesh(MeshConeObj.Object);

	MeshCone->SetupAttachment(RootComponent);
	MeshCone->SetRelativeLocation(FVector(0, 0, -65));

	MeshCone->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FString cheminMateriauSocle;
	FString CheminMateriauObjet;

	//set materiau socle
	switch (CouleurSocle)
	{
	case ECouleurObjetEnum::Bleu:
		cheminMateriauSocle = TEXT("/Game/Objets/Materiaux/HoloCone_Blue");
		CheminMateriauObjet = TEXT("/Game/Objets/Materiaux/Mat_Holo_Bleu");
		break;
	case ECouleurObjetEnum::Vert:
		cheminMateriauSocle = TEXT("/Game/Objets/Materiaux/HoloCone_Vert");
		CheminMateriauObjet = TEXT("/Game/Objets/Materiaux/Mat_Holo_Vert");
		break;
	case ECouleurObjetEnum::Rouge:
		cheminMateriauSocle = TEXT("/Game/Objets/Materiaux/HoloCone_Red");
		CheminMateriauObjet = TEXT("/Game/Objets/Materiaux/Mat_Holo_Rouge");
		break;
	}

	if (cheminMateriauSocle.Len())
	{
		UMaterialInstanceConstant * materiauPtr = ConstructorHelpers::FObjectFinderOptional<UMaterialInstanceConstant>(*cheminMateriauSocle).Get();
		if (materiauPtr)
		{
			MeshCone->SetMaterial(0, materiauPtr);
		}
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
		SkeletalMesh->SetRelativeLocation(FVector(0, 0, 50));
		UMaterialInstanceConstant * materiauPtr = ConstructorHelpers::FObjectFinderOptional<UMaterialInstanceConstant>(*CheminMateriauObjet).Get();
		if (materiauPtr)
		{
			SkeletalMesh->SetMaterial(0, materiauPtr);
		}
		SkeletalMesh->AddLocalOffset(FVector(0.0f, 0.0f, -25.f));
	}
	else if (TypeDeMesh == ETypeDeMeshEnum::StaticMesh)
	{
		Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshObjet"));
		const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj((TEXT("%s"), *CheminMesh));
		UStaticMeshComponent * StaticMesh = Cast<UStaticMeshComponent>(Mesh);
		StaticMesh->SetStaticMesh(MeshObj.Object);
		StaticMesh->SetupAttachment(RootComponent);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (CheminMateriauObjet.Len())
		{
			UMaterialInstanceConstant * materiauPtr = ConstructorHelpers::FObjectFinderOptional<UMaterialInstanceConstant>(*CheminMateriauObjet).Get();
			if (materiauPtr)
			{
				StaticMesh->SetMaterial(0, materiauPtr);
			}
		}
		StaticMesh->AddLocalOffset(FVector(0.0f, 0.0f, -25.f));
	}

	OnActorBeginOverlap.AddDynamic(this, &AObjetAuSol::OnCollision);
}

// Called when the game starts or when spawned
void AObjetAuSol::BeginPlay()
{
	Super::BeginPlay();
	MonteDescend();
}

void AObjetAuSol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UStaticMeshComponent * StaticMesh = Cast<UStaticMeshComponent>(Mesh);
	if (StaticMesh)
	{
		StaticMesh->AddLocalRotation(FRotator(0.f, DeltaTime * 30.f, 0.f));
		
		float Deplacement{ -50.0f * DeltaTime};
		if (bMonte)
		{
			Deplacement *= -1.0f;
		}
		StaticMesh->AddLocalOffset(FVector(0.0f, 0.0f, Deplacement));
	}
	else
	{
		USkeletalMeshComponent * SkeletalMesh = Cast<USkeletalMeshComponent>(Mesh);
		if (SkeletalMesh)
		{
			SkeletalMesh->AddLocalRotation(FRotator(0.f, DeltaTime * 30.f, 0.f));
			float Deplacement{ -50.0f * DeltaTime};
			if (bMonte)
			{
				Deplacement *= -1.0f;
			}
			SkeletalMesh->AddLocalOffset(FVector(0.0f, 0.0f, Deplacement));
		}
	}
}

void AObjetAuSol::MonteDescend()
{
	bMonte = (bMonte) ? false : true;
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AObjetAuSol::MonteDescend, 1.f, false);
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
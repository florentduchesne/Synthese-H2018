// Fill out your copyright notice in the Description page of Project Settings.

#include "Rebond.h"


// Sets default values
ARebond::ARebond()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Socle"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj((TEXT("%s"), *(FString("/Engine/BasicShapes/Cylinder"))));
	Mesh->SetStaticMesh(MeshObj.Object);

	Mesh->SetWorldScale3D(FVector(5.0f, 5.0f, 0.5f));

	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, "OnCollision");
	OnActorHit.AddUnique(Delegate);
}

// Called when the game starts or when spawned
void ARebond::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARebond::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARebond::OnCollision(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("collision !"));
	APersonnage * Personnage = Cast<APersonnage>(OtherActor);
	if (Personnage)
	{
		if (!PersonnagesRecents.Contains(Personnage))
		{
			UE_LOG(LogTemp, Warning, TEXT("personnage collisionne !"));
			//on ajoute le personnage a la liste de personnages qui ne peuvent pas sauter encore
			PersonnagesRecents.Add(Personnage);
			//on fait sauter le personnage
			Personnage->GetCharacterMovement()->AddImpulse(FVector(0, 0, 10000 * ForceDuBond));
			FTimerHandle UniqueHandle;
			FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &ARebond::DebloquerPersonnage, Personnage);
			//dans 0.1 seconde on enleve le personnage de la liste
			GetWorldTimerManager().SetTimer(UniqueHandle, RespawnDelegate, 0.1f, false);
		}
	}
}

void ARebond::DebloquerPersonnage(APersonnage * Personnage)
{
	PersonnagesRecents.Remove(Personnage);
}
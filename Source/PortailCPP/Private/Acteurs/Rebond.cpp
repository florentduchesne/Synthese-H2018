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

	//StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'
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
		UE_LOG(LogTemp, Warning, TEXT("personnage collisionne !"));
		
		Personnage->GetCharacterMovement()->AddForce(FVector(0, 0, 1000000 * ForceDuBond));
	}
}
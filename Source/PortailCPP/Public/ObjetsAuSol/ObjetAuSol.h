// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "GameFramework/Actor.h"
#include "../Personnage/Personnage.h"
#include "ObjetAuSol.generated.h"

UENUM(BlueprintType)
enum class ETypeDeMeshEnum : uint8
{
	StaticMesh 	UMETA(DisplayName = "Static"),
	SkeletalMesh 	UMETA(DisplayName = "Skeletal")
};

UENUM(BlueprintType)
enum class ECouleurObjetEnum : uint8
{
	Bleu 	UMETA(DisplayName = "Bleu"),
	Vert 	UMETA(DisplayName = "Vert"),
	Rouge	UMETA(DisplayName = "Rouge")
};

UCLASS()
class PORTAILCPP_API AObjetAuSol : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjetAuSol();
	AObjetAuSol(float _DelaisAvantReapparition, FString CheminMesh, ETypeDeMeshEnum TypeDeMesh, ECouleurObjetEnum CouleurSocle);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//mesh de l'objet à ramasser
	///DOIT ETRE UN USTATICMESH OU UN USKELETALMESH
	UObject * Mesh;

	UStaticMeshComponent * MeshSocle;

	UStaticMeshComponent * MeshCone;

	USphereComponent * SphereCollisions;

	bool bMonte = false;

	void MonteDescend();

	float DelaisAvantReapparition;

	bool EstDisponible = true;

	void Reapparition();

	UFUNCTION()
	virtual bool RamasseParUnJoueur(APersonnage * Personnage) PURE_VIRTUAL(AObjetAuSol::RamasseParUnJoueur, return false;);

	UFUNCTION()
	void OnCollision(AActor* overlappedActor, AActor* otherActor);
};

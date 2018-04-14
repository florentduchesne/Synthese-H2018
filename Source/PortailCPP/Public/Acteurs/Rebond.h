// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "../Personnage/Personnage.h"
#include "Components/StaticMeshComponent.h"
#include "Rebond.generated.h"

UCLASS()
class PORTAILCPP_API ARebond : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARebond();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UStaticMeshComponent * Mesh;

	TArray<APersonnage*> PersonnagesRecents;

	void DebloquerPersonnage(APersonnage * Personnage);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rebond")
	float ForceDuBond = 45.0f;

	UFUNCTION()
	void OnCollision(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

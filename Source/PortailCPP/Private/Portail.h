// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Personnage.h"
#include "GameFramework/Actor.h"
#include "Portail.generated.h"

UCLASS()
class APortail : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortail();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	APortail * autrePortail{nullptr};
	/*UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Portail Mesh")
		class UStaticMeshComponent * cadre;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Portail Mesh")
		class UStaticMeshComponent * panneau;*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Portail mesh")
	UStaticMeshComponent * cadre;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Portail mesh")
	UStaticMeshComponent * panneau;

	UFUNCTION()
	void OnTeleportation(AActor* overlappedActor, AActor* otherActor);

	UFUNCTION(BlueprintCallable, Category = "Portail")
	void connecterDeuxPortails(APortail * portail);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool bActif = true;

	
	
};

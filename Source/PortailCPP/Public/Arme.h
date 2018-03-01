// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Arme.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTAILCPP_API UArme : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UArme();
	UArme(const int tailleChargeur);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UStaticMeshComponent * mesh;
	int munitionsDansChargeur;
	int tailleChargeur;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};

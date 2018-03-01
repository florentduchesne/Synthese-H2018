// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Arme.generated.h"


UCLASS(abstract)
class PORTAILCPP_API UArme : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UArme();
	UArme(const int tailleChargeur, FString cheminMesh);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent * mesh;
	int munitionsDansChargeur;
	int tailleChargeur;



public:	
	// Called every frame
	USkeletalMeshComponent * getMesh();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
		virtual void Tirer() PURE_VIRTUAL(UArme::Tirer, );/* = 0; {
		UE_LOG(LogTemp, Warning, TEXT("tirer dans arme"));
	}*/
	
};

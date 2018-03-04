// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Arme.generated.h"


UCLASS(abstract)
class PORTAILCPP_API UArme : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UArme();
	UArme(const int _TailleChargeur, float _TempsRecharge, FString CheminMesh);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent * mesh;
	float DelaiEntreChaqueTir;
	float TempsRecharge;
	int8 MunitionsDansChargeur;
	int8 TailleChargeur;
	bool bPeutTirer = true;

	void Recharger();

public:
	// Called every frame
	USkeletalMeshComponent * getMesh();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Tirer() PURE_VIRTUAL(UArme::Tirer, ;);

	void LancerRechargement();
	
};

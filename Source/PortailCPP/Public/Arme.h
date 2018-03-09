// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "PortailCPP/Private/Projectile.h"
#include "Arme.generated.h"

UCLASS(abstract)
class PORTAILCPP_API UArme : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UArme();
	UArme(const int _TailleChargeur, float _TempsRecharge, int Degats, FString CheminMesh);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent * mesh;
	float DelaiEntreChaqueTir;//a remplacer
	float TempsRecharge;
	int8 MunitionsDansChargeur;
	int8 TailleChargeur;
	AProjectile * projectile;
	int Degats;

	//vrai si l'arme n'est pas en train de recharger
	bool bPeutTirer = true;

	void Recharger();

	AProjectile * FaireApparaitreProjectile();

public:
	// Called every frame
	USkeletalMeshComponent * getMesh();
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//retourne vrai si l'arme n'est pas en train de recharger ET qu'elle a encore une balle ou plus dans le chargeur
	bool PeutTirer();

	//lorsque le joueur appuie sur la touche de tir
	virtual void CommencerTir() PURE_VIRTUAL(UArme::CommencerTir, ;);
	//lorsque le joueur relache la touche de tir
	virtual void TerminerTir() PURE_VIRTUAL(UArme::TerminerTir, ;);

	void CommencerTirSuper();
	void TerminerTirSuper();

	void LancerRechargement();
};

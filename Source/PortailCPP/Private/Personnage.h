// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Arme.h"
#include "FusilSemiAuto.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "FusilAuto.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Personnage.generated.h"

UCLASS()
class APersonnage : public ACharacter
{
	GENERATED_BODY()
		
public:
	// Sets default values for this character's properties
	APersonnage();
	//USkeletalMeshComponent * corps;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Mesh troisieme personne (visible uniquement par les autres joueurs)
	USkeletalMeshComponent * corps;

	UPROPERTY()
	UArme * arme;

	int PointsDevie = 100;
	int Armure = 50;

	// Camera du personnage
	UPROPERTY(VisibleAnywhere)
	UCameraComponent * camera;

	bool bPeutSeTeleporter = true;

	UFUNCTION()
	void Recharger();

	// Gere les inputs pour deplacer le personnage avant-arriere
	UFUNCTION()
	void Avancer(float Value);

	// Gere les inputs pour deplacer le personnage lateralement
	UFUNCTION()
	void DeplacementLateral(float Value);

	// Gere les inputs pour tourner la camera horizontalement
	UFUNCTION()
	void TournerHorizontalement(float Value);

	// Gere les inputs pour tourner la camera verticalement
	UFUNCTION()
	void TournerVerticalement(float Value);

	//Sauter
	UFUNCTION()
	void DebuterSaut();

	UFUNCTION()
	void TerminerSaut();

	/*
	//tirer
	UFUNCTION()
	void Tirer();*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//débloque la téléportation avec un court délai
	void DebloquerTeleportationFutur();
	//permet au personnage de se téléporter
	void DebloquerTeleportation();
	//empêche le personnage de se téléporter
	void BloquerTeleportation();
	//retourne si oui ou non, le personnage a le droit de se téléporter
	bool PeutSeTeleporter();
	//Le personnage recoit des degats
	void InfligerDegats(int degats);
};

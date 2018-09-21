// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//superclasse
#include "GameFramework/Character.h"

//utilitaires
#include "ConstructorHelpers.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/UObjectGlobals.h"
#include "Materials/MaterialInstanceConstant.h"

//Animations
#include "Animation/BlendSpace.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimInstance.h"

//toutes les armes possibles
#include "Armes/Arme.h"
#include "Armes/FusilSemiAuto.h"
#include "Armes/FusilARafales.h"
#include "Armes/FusilAuto.h"
#include "Armes/FusilAPompe.h"

//tous les components autres que les armes
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ATH.h"

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

	int PointsDeVie = 100;
	int Armure = 0;

	//0 a 3, equivaut a l'emplacement dans le tableau StatistiquesDuJoueur
	int NoJoueur = -1;

	AATH * ATH;

	float SensibiliteHorizontale = 1.0f;
	float SensibiliteVerticale = 1.0f;

	// Camera du personnage
	UPROPERTY(VisibleAnywhere)
	UCameraComponent * camera;

	//vrai si le personnage ne s'est pas teleporte depuis un petit delai
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

	UFUNCTION()
	void CommencerTir();
	UFUNCTION()
	void TerminerTir();
	UFUNCTION()
	void Rechargement();

	UFUNCTION()
	void TirSecondaire();

	UFUNCTION()
	void RetourMenuPrincipal();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetNoJoueur(int _NoJoueur);
	int GetNoJoueur();
	void SetSensibilite(float sensiHorizontale, float sensiVerticale);

	void SetATH(AHUD * HUD);
	
	///on bloque et debloque la teleportation dans le but d'eviter que le joueur se teleporte a repetition d'un teleporteur a l'autre jusqu'au StackOverflow
	//debloque la teleportation apres un court delai
	void DebloquerTeleportationFutur();
	//permet au personnage de se teleporter
	void DebloquerTeleportation();
	//empeche le personnage de se teleporter
	void BloquerTeleportation();
	//retourne si oui ou non, le personnage a le droit de se teleporter
	bool PeutSeTeleporter();

	//Le personnage recoit des degats
	void InfligerDegats(int degats, int NoJoueurAttaquant);
	//le personnage est soigne
	bool Soigner(int NbPointsDeVie);
	//le personnage gagne de l'armure
	bool EquiperArmure(int NbArmure);

	void ReinitialiserStatistiques();

	bool ChangerArme(TSubclassOf<UArme> SousClasseDeArme);

	void MiseAJourNbMeurtresATH(int Meurtres);
	void MiseAJourBallesDansChargeur(int NbBalles);
	void MiseAJourBallesMax(int NbBalles);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations")
	UBlendSpace * BlendSpace;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations") 
	UAnimSequence * AnimSequence;
};

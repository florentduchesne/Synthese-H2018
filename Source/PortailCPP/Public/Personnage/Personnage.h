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

//toutes les armes possibles
#include "Armes/Arme.h"
#include "Armes/FusilSemiAuto.h"
#include "Armes/FusilARafales.h"
#include "Armes/FusilAuto.h"

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

	int NoJoueur;

	AATH * ATH;

	// Camera du personnage
	UPROPERTY(VisibleAnywhere)
	UCameraComponent * camera;

	//vrai si le personnage ne s'est pas t�l�port� depuis un petit d�lai
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetNoJoueur(int _NoJoueur);
	int GetNoJoueur();

	void SetATH(AHUD * HUD);
	
	///on bloque et d�bloque la t�l�portation dans le but d'�viter que le joueur se t�l�porte � r�p�tition d'un t�l�porteur � l'autre jusqu'au StackOverflow
	//d�bloque la t�l�portation apr�s un court d�lai
	void DebloquerTeleportationFutur();
	//permet au personnage de se t�l�porter
	void DebloquerTeleportation();
	//emp�che le personnage de se t�l�porter
	void BloquerTeleportation();
	//retourne si oui ou non, le personnage a le droit de se t�l�porter
	bool PeutSeTeleporter();

	//Le personnage recoit des degats
	void InfligerDegats(int degats, int NoJoueurAttaquant);
	//le personnage est soign�
	bool Soigner(int NbPointsDeVie);
	//le personnage gagne de l'armure
	bool EquiperArmure(int NbArmure);

	void ReinitialiserStatistiques();

	bool ChangerArme(TSubclassOf<UArme> SousClasseDeArme);

	void MiseAJourNbMeurtresATH(int Meurtres);
	void MiseAJourBallesDansChargeur(int NbBalles);
	void MiseAJourBallesMax(int NbBalles);

};

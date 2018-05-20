// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
//#include "Engine/SceneCapture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "ConstructorHelpers.h"
#include "UObjectGlobals.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Personnage/Personnage.h"
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

	UTextureRenderTarget2D * TextureRenderTarget;

	UFUNCTION()
	void OnTeleportation(AActor* overlappedActor, AActor* otherActor);

	int attente = 3;

	UTexture2D * textureTempo;
	UMaterial* MateriauPortail;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool bActif = true;
	bool bConnecte = false;
	bool estConnecte();

	//sert à afficher la "vision" de l'autre portail
	USceneCaptureComponent2D * Capture;
	UMaterialInstanceDynamic * MateriauDynamique;

	void SetMateriauPanneau(UMaterialInstanceDynamic * MateriauDynamique);

	void connecterDeuxPortails(APortail * portail);
	
};

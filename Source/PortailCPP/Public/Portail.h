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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	APortail * autrePortail{nullptr};
	/*UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Portail Mesh")
		class UStaticMeshComponent * cadre;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Portail Mesh")
		class UStaticMeshComponent * panneau;*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Portail mesh")
	UStaticMeshComponent * cadre;
	//le panneau sur lequel on affiche ce que "voit" l'autre portail
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Portail mesh")
	UStaticMeshComponent * panneau;
	//panneau en partie transparent, ajoute un effet d'hologramme
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Portail mesh")
	UStaticMeshComponent * panneau2;
	
	UTextureRenderTarget2D * TextureRenderTarget;

	UFUNCTION()
	void OnTeleportation(AActor* overlappedActor, AActor* otherActor);

	int32 attente = 3;

	int32 ID = 0;
	//commence a 0, augmente a chaque appel de "connecterDeuxPortails"
	static int32 NbPortails;

	UTexture2D * textureTempo;
	UMaterial* MateriauPortail;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool bActif = true;
	bool bConnecte = false;
	bool estConnecte();

	//sert a afficher la "vision" de l'autre portail
	USceneCaptureComponent2D * Capture;
	UMaterialInstanceDynamic * MateriauDynamique;

	void connecterDeuxPortails(APortail * portail);
};

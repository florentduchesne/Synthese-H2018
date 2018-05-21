// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Effets/EffetExplosion.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Projectile.h"
#include "ProjectileExplosif.generated.h"

/**
 * 
 */
UCLASS()
class PORTAILCPP_API AProjectileExplosif : public AProjectile
{
	GENERATED_BODY()

public:
	void SetRayonExplosion(float RayonExplosion) { this->RayonExplosion = RayonExplosion; };

private:

	AProjectileExplosif();

	float RayonExplosion;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	USphereComponent * SphereCollision;

	UParticleSystem * ParticuleSysteme;

	UFUNCTION()
	void DebutCollisionExplosion(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void Detruire();

	UAudioComponent * SonExplosion;
	USoundCue * SignalSonExplosion;

	virtual void BeginPlay() override;
};

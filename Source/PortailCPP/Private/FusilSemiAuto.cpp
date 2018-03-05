// Fill out your copyright notice in the Description page of Project Settings.

#include "FusilSemiAuto.h"


UFusilSemiAuto::UFusilSemiAuto()
	:UArme(5, 1.0f, 30, "/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun")
{}

void UFusilSemiAuto::CommencerTir() {
	UE_LOG(LogTemp, Warning, TEXT("tirer dans fusil semi auto"));
	UKismetSystemLibrary::PrintString(this, TEXT("PIOU! PIOU! PIOU!"), true, true, FColor::Red, 5.0f);
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		const FRotator SpawnRotation = this->GetComponentRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = this->GetComponentLocation() + SpawnRotation.RotateVector(FVector(100.0f, 0.0f, 10.0f));

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// spawn the projectile at the muzzle
		World->SpawnActor<AProjectile>(AProjectile::StaticClass(), SpawnLocation, SpawnRotation, ActorSpawnParams);
	}
}

void UFusilSemiAuto::TerminerTir()
{
	UE_LOG(LogTemp, Warning, TEXT("tir termine"));
}
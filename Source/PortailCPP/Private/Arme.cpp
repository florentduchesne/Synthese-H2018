// Fill out your copyright notice in the Description page of Project Settings.

#include "Arme.h"


// Sets default values for this component's properties
UArme::UArme()
	//:UArme(10, 1.0f, 10, "/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun")
{}

UArme::UArme(const int _TailleChargeur, float _TempsRecharge, int Degats, FString CheminMesh)
	:TailleChargeur( _TailleChargeur ), TempsRecharge(_TempsRecharge), Degats(Degats)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshArme"));
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj((TEXT("%s"), *CheminMesh));
	mesh->SetSkeletalMeshWithoutResettingAnimation(MeshObj.Object);
	//UKismetSystemLibrary::PrintString(this, *FString("balles dans chargeur : " + FString::FromInt(_tailleChargeur)), true, true, FColor::Red, 5.0f);

	MunitionsDansChargeur = TailleChargeur;
}

// Called when the game starts
void UArme::BeginPlay()
{
	Super::BeginPlay();
}

USkeletalMeshComponent * UArme::getMesh()
{
	return mesh;
}

//recharge l'arme
void UArme::Recharger()
{
	MunitionsDansChargeur = TailleChargeur;
	bPeutTirer = true;
	UKismetSystemLibrary::PrintString(this, TEXT("RECHARGEMENT TERMINE"), true, true, FColor::Red, 5.0f);
}

void UArme::LancerRechargement()
{
	if (bPeutTirer)
	{
		FTimerHandle TimerHandle;
		GetOuter()->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UArme::Recharger, TempsRecharge, false);
		bPeutTirer = false;
		UKismetSystemLibrary::PrintString(this, TEXT("RECHARGEMENT"), true, true, FColor::Red, 5.0f);
	}
}

bool UArme::PeutTirer()
{
	if (!MunitionsDansChargeur)
	{
		LancerRechargement();
	}
	return bPeutTirer;
	/*
	if (bPeutTirer)
	{
		if (MunitionsDansChargeur)
		{
			return true;
		}
		else
		{
			LancerRechargement();
			return false;
		}
	}
	else
	{
		return false;
	}*/
}

void UArme::CommencerTirSuper()
{
	if (PeutTirer())
	{
		CommencerTir();
	}
}

void UArme::TerminerTirSuper()
{
	if (PeutTirer())
	{
		TerminerTir();
	}
}

AProjectile * UArme::FaireApparaitreProjectile()
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		const FRotator SpawnRotation = this->GetComponentRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = this->GetComponentLocation() + SpawnRotation.RotateVector(FVector(100.0f, 25.0f, 0.0f));

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// fait apparaitre le projectile avec le constructeur par defaut
		AProjectile * projectile = World->SpawnActor<AProjectile>(AProjectile::StaticClass(), SpawnLocation, SpawnRotation, ActorSpawnParams);
		//initialise le projectile avec les valeurs propres a l'arme
		MunitionsDansChargeur -= 1;
		return projectile;
	}
	return nullptr;
}
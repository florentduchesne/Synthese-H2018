// Fill out your copyright notice in the Description page of Project Settings.

#include "Arme.h"


// Sets default values for this component's properties
UArme::UArme()
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
	bADesBallesDansChargeur = true;
	UKismetSystemLibrary::PrintString(this, TEXT("RECHARGEMENT TERMINE"), true, true, FColor::Red, 5.0f);
}

void UArme::LancerRechargement()
{
	if (bADesBallesDansChargeur)
	{
		FTimerHandle TimerHandle;
		GetOuter()->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UArme::Recharger, TempsRecharge, false);
		bADesBallesDansChargeur = false;
		UKismetSystemLibrary::PrintString(this, TEXT("RECHARGEMENT"), true, true, FColor::Red, 5.0f);
	}
}

bool UArme::ADesBallesDansChargeur()
{
	if (!MunitionsDansChargeur)
	{
		LancerRechargement();
	}
	return bADesBallesDansChargeur;
}

void UArme::CommencerTirSuper()
{
	if (ADesBallesDansChargeur())
	{
		if (bDelaiEntreChaqueTirTermine)
		{
			CommencerTir();
			bTirACommence = true;
		}
	}
}

void UArme::TerminerTirSuper()
{
	TerminerTir();
	if (bDelaiEntreChaqueTirTermine && bTirACommence)
	{
		FTimerHandle TimerHandle;
		GetOuter()->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UArme::DebloquerTirDelai, DelaiEntreChaqueTir, false);
		bDelaiEntreChaqueTirTermine = false;
	}
	bTirACommence = false;
}

void UArme::DebloquerTirDelai()
{
	bDelaiEntreChaqueTirTermine = true;
}

void UArme::FaireApparaitreProjectile()
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		const FRotator SpawnRotation = this->GetComponentRotation();
		// place la balle au bon endroit par rapport au personnage et à son orientation
		const FVector SpawnLocation = this->GetComponentLocation() + SpawnRotation.RotateVector(FVector(100.0f, 25.0f, 0.0f));

		//initialise les collisions pour la balle
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// fait apparaitre le projectile avec le constructeur par defaut
		AProjectile * projectile = World->SpawnActor<AProjectile>(AProjectile::StaticClass(), SpawnLocation, SpawnRotation, ActorSpawnParams);
		//initialise le projectile avec les valeurs propres a l'arme
		projectile->Initialiser(Degats, NoJoueur);
		//diminue le nombre de balles dans le chargeur
		MunitionsDansChargeur -= 1;
	}
}

void UArme::SetNoJoueur(int NoJoueur)
{
	this->NoJoueur = NoJoueur;
}
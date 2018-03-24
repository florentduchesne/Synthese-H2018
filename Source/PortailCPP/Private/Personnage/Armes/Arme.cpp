// Fill out your copyright notice in the Description page of Project Settings.

#include "Arme.h"
#include "PortailCPP/Public/Personnage/Personnage.h"


// Sets default values for this component's properties
UArme::UArme()
{}

UArme::UArme(const int _TailleChargeur, float _TempsRecharge, float _DelaiEntreChaqueTir, int Degats, FString CheminMesh)
	:TailleChargeur( _TailleChargeur ), TempsRecharge(_TempsRecharge), DelaiEntreChaqueTir(_DelaiEntreChaqueTir), Degats(Degats)
{
	PrimaryComponentTick.bCanEverTick = false;
	
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshArme"));
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj((TEXT("%s"), *CheminMesh));
	mesh->SetSkeletalMeshWithoutResettingAnimation(MeshObj.Object);

	if (!DelaiEntreChaqueTir)
	{
		DelaiEntreChaqueTir = 0.01f;
	}

	MunitionsDansChargeur = TailleChargeur;
}

// Called when the game starts
void UArme::BeginPlay()
{
	Super::BeginPlay();
	/*
	APersonnage * Personnage = Cast<APersonnage>(GetAttachmentRootActor());
	if (Personnage)
	{
		Personnage->MiseAJourBallesMax(TailleChargeur);
	}*/
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
	if (bADesBallesDansChargeur && MunitionsDansChargeur < TailleChargeur)
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
		if (projectile)
		{
			projectile->Initialiser(Degats, NoJoueur);
		}
		//diminue le nombre de balles dans le chargeur
		MunitionsDansChargeur -= 1;
	}
	APersonnage * Personnage = Cast<APersonnage>(GetAttachmentRootActor());
	if (Personnage)
	{
		Personnage->MiseAJourBallesDansChargeur(MunitionsDansChargeur);
	}
}

void UArme::SetNoJoueur(int NoJoueur)
{
	this->NoJoueur = NoJoueur;
}
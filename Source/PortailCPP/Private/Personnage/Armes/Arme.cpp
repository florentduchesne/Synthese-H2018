// Fill out your copyright notice in the Description page of Project Settings.

#include "Arme.h"
#include "PortailCPP/Public/Personnage/Personnage.h"

int UArme::ID = 0;

// Sets default values for this component's properties
UArme::UArme()
{}

UArme::UArme(const int _TailleChargeur, float _TempsRecharge, float _DelaiEntreChaqueTir, int Degats, int VitesseProjectiles, FString _CheminMesh)
	:TailleChargeur( _TailleChargeur ), TempsRecharge(_TempsRecharge), DelaiEntreChaqueTir(_DelaiEntreChaqueTir), Degats(Degats), VitesseProjectiles(VitesseProjectiles), CheminMesh{_CheminMesh}
{
	PrimaryComponentTick.bCanEverTick = false;

	FString nomMesh = FString("MeshArme");
	nomMesh.AppendInt(ID);
	ID++;
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(*nomMesh);
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj((TEXT("%s"), *CheminMesh));
	mesh->SetSkeletalMeshWithoutResettingAnimation(MeshObj.Object);

	if (!DelaiEntreChaqueTir)
	{
		DelaiEntreChaqueTir = 0.01f;
	}

	MunitionsDansChargeur = TailleChargeur;

	static ConstructorHelpers::FObjectFinder<USoundCue> SignalTir(TEXT("SoundCue'/Game/Personnage/Armes/Sons/Tir_SemiAuto.Tir_SemiAuto'"));
	SignalSonTir = SignalTir.Object;

	SonTir = CreateDefaultSubobject<UAudioComponent>(TEXT("SonTir"));
	SonTir->bAutoActivate = false;
	SonTir->SetupAttachment(mesh);
}

// Called when the game starts
void UArme::BeginPlay()
{
	Super::BeginPlay();
	if (SonTir->IsValidLowLevelFast())
	{
		SonTir->SetSound(SignalSonTir);
	}

	//apparemment ça sert à de quoi (et ça plante des fois sans le if)
	if (!mesh->IsRegistered())
		mesh->RegisterComponent();
	else
		UE_LOG(LogTemp, Warning, TEXT("mesh arme deja enregistre"));
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
	MiseAJourATHJoueur();
}

void UArme::LancerRechargement()
{
	if (bADesBallesDansChargeur && MunitionsDansChargeur < TailleChargeur)
	{
		FTimerHandle TimerHandle;
		GetOuter()->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UArme::Recharger, TempsRecharge, false);
		bADesBallesDansChargeur = false;
		MunitionsDansChargeur = 0;
		MiseAJourATHJoueur();
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

void UArme::DebloquerTirSecondaireDelai()
{
	bDelaiEntreChaqueTirSecondaireTermine = true;
}

void UArme::FaireApparaitreProjectile(ETypeDeTir TypeDeTir, FRotator Deviation)
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		const FRotator SpawnRotation = GetComponentRotation() + Deviation;
		// place la balle au bon endroit par rapport au personnage et à son orientation
		const FVector SpawnLocation = GetComponentLocation() + FVector(0.f, 0.f, 10.f);// +SpawnRotation.RotateVector(FVector(90.0f, 20.0f, 0.0f));

		//initialise les collisions pour la balle
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// fait apparaitre le projectile avec le constructeur par defaut
		int DegatsBalle = 0;
		AProjectile * Projectile{nullptr};
		switch (TypeDeTir)
		{
		case ETypeDeTir::Normal:
			//diminue le nombre de balles dans le chargeur
			MunitionsDansChargeur -= 1;
		case ETypeDeTir::Pompe:
			Projectile = World->SpawnActor<AProjectile>(AProjectile::StaticClass(), SpawnLocation, SpawnRotation, ActorSpawnParams);
			DegatsBalle = Degats;
			break;
		case ETypeDeTir::Explosif:
			Projectile = World->SpawnActor<AProjectileExplosif>(AProjectileExplosif::StaticClass(), SpawnLocation, SpawnRotation, ActorSpawnParams);
			AProjectileExplosif * ProjectileExplo = Cast<AProjectileExplosif>(Projectile);
			ProjectileExplo->SetRayonExplosion(100.0f);
			DegatsBalle = 90;
			//vide le chargeur
			MunitionsDansChargeur = 0;

			FTimerHandle TimerHandle;
			GetOuter()->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UArme::DebloquerTirSecondaireDelai, DelaiTirSecondaire, false);
			bDelaiEntreChaqueTirSecondaireTermine = false;
		}
		//initialise le projectile avec les valeurs propres a l'arme
		if (Projectile)
		{
			Projectile->Initialiser(DegatsBalle, NoJoueur, VitesseProjectiles);
		}
	}
	MiseAJourATHJoueur();
}

void UArme::TirSecondaire()
{
	if(ADesBallesDansChargeur())
		if(bDelaiEntreChaqueTirSecondaireTermine)
			FaireApparaitreProjectile(ETypeDeTir::Explosif, FRotator(0.0f));
}

void UArme::SetNoJoueur(int NoJoueur)
{
	this->NoJoueur = NoJoueur;
}

void UArme::MiseAJourATHJoueur()
{
	APersonnage * Personnage = Cast<APersonnage>(GetAttachmentRootActor());
	if (Personnage)
	{
		Personnage->MiseAJourBallesDansChargeur(MunitionsDansChargeur);
	}
}

void UArme::DetruireArme()
{
	//mesh->UnregisterComponent();
	mesh->DestroyComponent();
}

void UArme::Attacher(USceneComponent * Objet)
{
	//SetupAttachment(Objet);
	AttachToComponent(Objet, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	mesh->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	mesh->SetRelativeRotation(FRotator(0, -90.0f, 0));
}
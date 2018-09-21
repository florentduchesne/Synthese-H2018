// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileExplosif.h"
#include "PortailCPP/Public/Personnage/Personnage.h"

AProjectileExplosif::AProjectileExplosif() {
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffetExplosion(TEXT("ParticleSystem'/Game/Effets/Explosion/Effects/ParticleSystems/Weapons/RocketLauncher/Impact/P_Launcher_IH.P_Launcher_IH'"));
	ParticuleSysteme = EffetExplosion.Object;

	//son
	static ConstructorHelpers::FObjectFinder<USoundCue> Signal(TEXT("SoundCue'/Game/Personnage/Armes/Sons/Explosion_TirSecondaire.Explosion_TirSecondaire'"));
	SignalSonExplosion = Signal.Object;

	SonExplosion = CreateDefaultSubobject<UAudioComponent>(TEXT("SonExplosion"));
	SonExplosion->bAutoActivate = false;
	SonExplosion->bStopWhenOwnerDestroyed = false;
	SonExplosion->SetupAttachment(RootComponent);
}

void AProjectileExplosif::BeginPlay()
{
	Super::BeginPlay();
	if (SonExplosion->IsValidLowLevelFast())
	{
		SonExplosion->SetSound(SignalSonExplosion);
	}
}

void AProjectileExplosif::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Exploser(OtherActor);
}

void AProjectileExplosif::DebutOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (APersonnage * Personnage = Cast<APersonnage>(OtherActor))
	{
		Exploser(OtherActor);
	}
}

void AProjectileExplosif::Exploser(AActor * autreActeur)
{
	CollisionComp->OnComponentHit.RemoveDynamic(this, &AProjectile::OnHit);

	ProjectileMovement->StopMovementImmediately();

	SphereCollision = NewObject<USphereComponent>(this, TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetSphereRadius(250.0f);
	SphereCollision->RegisterComponent();
	SphereCollision->UpdateOverlaps();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectileExplosif::DebutOverlap);

	//on inflige des degats aux personnages deja presents dans la zone (ils ne declanchent pas la fonction OnComponentBeginOverlap)
	TArray<AActor*> Acteurs;
	SphereCollision->GetOverlappingActors(Acteurs);
	for (AActor * Acteur : Acteurs)
	{
		APersonnage * Personnage = Cast<APersonnage>(Acteur);
		if (Personnage)
		{
			Personnage->InfligerDegats(Degats, NoJoueur);
		}
	}

	//FX
	UGameplayStatics::SpawnEmitterAtLocation(this, ParticuleSysteme, GetActorLocation(), GetActorRotation(), true);

	//Son
	SonExplosion->Play(0.8f);

	FTimerHandle TimerHandle;
	GetOuter()->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AProjectileExplosif::Detruire, 0.5f, false);
}

//detruit le projectile
void AProjectileExplosif::Detruire()
{
	Destroy();
}
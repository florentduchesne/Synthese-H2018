// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileExplosif.h"
#include "PortailCPP/Public/Personnage/Personnage.h"

AProjectileExplosif::AProjectileExplosif() {
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffetExplosion(TEXT("ParticleSystem'/Game/Effets/Explosion/Effects/ParticleSystems/Weapons/RocketLauncher/Impact/P_Launcher_IH.P_Launcher_IH'"));
	ParticuleSysteme = EffetExplosion.Object;
}

void AProjectileExplosif::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// si l'acteur touché simule la physique, lui donne une poussée
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}
	
	SphereCollision = NewObject<USphereComponent>(this, TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetSphereRadius(250.0f);
	SphereCollision->RegisterComponent();
	SphereCollision->UpdateOverlaps();

	//on inflige des dégâts aux personnages déjà présents dans la zone (ils ne déclanchent pas la fonction OnComponentBeginOverlap)
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

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectileExplosif::DebutCollisionExplosion);

	CollisionComp->OnComponentHit.RemoveDynamic(this, &AProjectile::OnHit);

	//FX
	UGameplayStatics::SpawnEmitterAtLocation(this, ParticuleSysteme, GetActorLocation(), GetActorRotation(), true);

	FTimerHandle TimerHandle;
	GetOuter()->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AProjectileExplosif::Detruire, 0.5, false);
}

void AProjectileExplosif::DebutCollisionExplosion(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (APersonnage * Personnage = Cast<APersonnage>(OtherActor))
	{
		Personnage->InfligerDegats(Degats, NoJoueur);
	}
}

//détruit le projectile
void AProjectileExplosif::Detruire()
{
	Destroy();
}
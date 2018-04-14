// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileExplosif.h"
#include "PortailCPP/Public/Personnage/Personnage.h"

AProjectileExplosif::AProjectileExplosif() {}

void AProjectileExplosif::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// si l'acteur touch� simule la physique, lui donne une pouss�e
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}
	
	SphereCollision = NewObject<USphereComponent>(this, TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetSphereRadius(250.0f);
	SphereCollision->RegisterComponent();
	SphereCollision->UpdateOverlaps();

	//on inflige des d�g�ts aux personnages d�j� pr�sents dans la zone (ils ne d�clanchent pas la fonction OnComponentBeginOverlap)
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

//d�truit le projectile
void AProjectileExplosif::Detruire()
{
	Destroy();
}
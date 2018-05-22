// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "PortailCPP/Public/Personnage/Personnage.h"
#include "PortailCPP/Public/ObjetsAuSol/ObjetAuSol.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(8.0f);
	CollisionComp->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::DebutOverlap);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComp->SetCollisionProfileName(TEXT("Projectile"));

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 10000.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBalle"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/FirstPerson/Meshes/FirstPersonProjectileMesh"));
	Mesh->SetStaticMesh(MeshObj.Object);
	Mesh->SetWorldScale3D(FVector(0.03f, 0.03f, 0.03f));

	Mesh->SetupAttachment(RootComponent);
}

void AProjectile::DebutOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (APersonnage * personnageTouche = Cast<APersonnage>(OtherActor))
	{
		if (personnageTouche->GetNoJoueur() == NoJoueur)
		{
			return;
		}
		personnageTouche->InfligerDegats(Degats, NoJoueur);
	}
	//si l'acteur touché est un joueur, lui inflige les dégâts
	OtherActor = Cast<AProjectile>(OtherActor);
	if (!OtherActor)
		Destroy();
}

void AProjectile::Initialiser(int Degats, int NoJoueur, float vitesse)
{
	this->Degats = Degats;
	this->NoJoueur = NoJoueur;
	ProjectileMovement->MaxSpeed = vitesse;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//si l'acteur touché est un joueur, lui inflige les dégâts
	OtherActor = Cast<AProjectile>(OtherActor);
	if(!OtherActor)
		Destroy();
}
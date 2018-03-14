// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "PortailCPP/Public/Personnage/Personnage.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 6000.f;
	ProjectileMovement->MaxSpeed = 6000.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBalle"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/FirstPerson/Meshes/FirstPersonProjectileMesh"));
	Mesh->SetStaticMesh(MeshObj.Object);
	Mesh->SetWorldScale3D(FVector(0.05f, 0.05f, 0.05f));

	Mesh->SetupAttachment(RootComponent);

	Degats = 20;
}

void AProjectile::Initialiser(int Degats, int NoJoueur)
{
	this->Degats = Degats;
	this->NoJoueur = NoJoueur;
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
	// si l'acteur touch� simule la physique, lui donne une pouss�e
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}
	//si l'acteur touch� est un joueur, lui inflige les d�g�ts
	if (APersonnage * personnageTouche = Cast<APersonnage>(OtherActor))
	{
		personnageTouche->InfligerDegats(Degats, NoJoueur);
	}
	//d�truit le projectile
	Destroy();
}
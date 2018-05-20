// Fill out your copyright notice in the Description page of Project Settings.

#include "Portail.h"

int32 APortail::NbPortails = 0;

// Sets default values
APortail::APortail()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	cadre = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshCadre"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/Geometry/Meshes/cadrePortail"));
	cadre->SetStaticMesh(MeshObj.Object);
	panneau = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshPanneau"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj2(TEXT("/Game/Geometry/Meshes/panneauPortail"));
	panneau->SetStaticMesh(MeshObj2.Object);
	RootComponent = cadre;
	panneau->SetupAttachment(cadre);

	panneau->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	cadre->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	FVector position = cadre->GetComponentLocation();
	position.Z += 120.0f;
	FRotator rotation = cadre->GetComponentRotation();
	rotation.Yaw += 90.0f;
	rotation.Roll += 90.0f;

	panneau->SetRelativeLocationAndRotation(position, rotation);

	Capture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture"));
	Capture->SetupAttachment(RootComponent);
	Capture->AddLocalRotation(FRotator(0.0f, 180.0f, 0.0f));
	Capture->AddRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	//on update manuellement le SceneCapture2D pour r�duire la charge de calcul. Seulement un portail par tick. La fr�quence de mise � jour d�pend donc du nombre de portails dans le monde.
	Capture->bCaptureEveryFrame = false;
	Capture->UpdateContent();
	Capture->TextureTarget = nullptr;

	TextureRenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("PortalRenderTarget"));
	TextureRenderTarget->InitAutoFormat(300, 240);
	TextureRenderTarget->AddressX = TextureAddress::TA_Wrap;
	TextureRenderTarget->AddressY = TextureAddress::TA_Wrap;

	static ConstructorHelpers::FObjectFinder<UMaterial> PortalMaterialObj(TEXT("/Game/Objets/Mat_Panneau"));
	if (PortalMaterialObj.Succeeded())
	{
		MateriauPortail = PortalMaterialObj.Object;
	}

	OnActorBeginOverlap.AddDynamic(this, &APortail::OnTeleportation);
}

// Called when the game starts or when spawned
void APortail::BeginPlay()
{
	Super::BeginPlay();
	
}

void APortail::OnTeleportation(AActor* overlappedActor, AActor* otherActor)
{
	//cast de l'acteur en Personnage
	APersonnage * Personnage = Cast<APersonnage>(otherActor);
	if (Personnage)
	{
		if (Personnage->PeutSeTeleporter() && autrePortail)
		{
			//le personnage fait dos au portail quand il en sort
			//la rotation du personnage + la rotation de l'autre portail - ma rotation + 180
			FRotator rotation = Personnage->GetControlRotation();
			rotation.Yaw = Personnage->GetActorRotation().Yaw + autrePortail->GetActorRotation().Yaw - GetActorRotation().Yaw + 180.0f;
			Personnage->GetController()->SetControlRotation(rotation);
			//on lui enleve le droit de se teleporter pour eviter un stackoverflow
			Personnage->BloquerTeleportation();

			//teleporter le joueur un peu devant l'autre portail
			FVector position;
			position = autrePortail->GetActorLocation() - autrePortail->GetActorForwardVector() * 80;
			position.Z += 60.0f;
			FHitResult HitResult;
			Personnage->SetActorLocation(position, false, &HitResult, ETeleportType::None);//ETeleportType est None, ce qui (suppos�ment) annule tout effet de physique lorsqu'on sort du portail.
			
			//on enl�ve la physique que le joueur pouvait avoir au moment de la t�l�portation (si il sautait, par exemple)
			Personnage->GetCharacterMovement()->StopMovementImmediately();

			//on appelle une fonction qui lui redonne le droit de se teleporter dans une seconde
			Personnage->DebloquerTeleportationFutur();
		}
	}	
}

void APortail::connecterDeuxPortails(APortail * portail)
{
	this->autrePortail = portail;
	if (!autrePortail->estConnecte())
		autrePortail->connecterDeuxPortails(this);

	ID = NbPortails;
	NbPortails++;

	autrePortail->Capture->TextureTarget = TextureRenderTarget;
	autrePortail->Capture->UpdateContent();
	MateriauDynamique = panneau->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MateriauPortail);
	MateriauDynamique->SetTextureParameterValue(TEXT("TexturePrincipale"), TextureRenderTarget);
}

// Called every frame
void APortail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (NbPortails)
	{
		if (!attente)
		{
			attente = NbPortails;
		}
		if (attente == ID)
		{
			Capture->UpdateContent();
		}
		attente--;
	}
}

bool APortail::estConnecte()
{
	if (autrePortail)
		return true;
	else
		return false;
}
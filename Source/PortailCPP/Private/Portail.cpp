// Fill out your copyright notice in the Description page of Project Settings.

#include "Portail.h"


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
	//Capture->SetWorldLocation(FVector(0));
	Capture->AddLocalRotation(FRotator(0.0f, 180.0f, 0.0f));
	//Capture->AddLocalOffset(FVector(500.0f, 0, 5000.0f));
	Capture->AddRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	Capture->bCaptureEveryFrame = true;
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

	//NewObject<UTexture2D>(this, TEXT("/Engine/EngineResources/DefaultTexture"));

	//MateriauDynamique = NewObject<UMaterialInstanceDynamic>(this, FName("MateriauDynamique"));

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
			Personnage->SetActorLocation(position, false, &HitResult, ETeleportType::None);//ETeleportType est None, ce qui (supposément) annule tout effet de physique lorsqu'on sort du portail.
			
			//on enlève la physique que le joueur pouvait avoir au moment de la téléportation (si il sautait, par exemple)
			Personnage->GetCharacterMovement()->StopMovementImmediately();

			//FVector Direction = autrePortail->GetActorForwardVector();//FRotationMatrix(autrePortail->GetActorRotation()).GetScaledAxis(EAxis::X);
			//Personnage->GetCharacterMovement()->AddForce((Direction * 150000000) + FVector(0, 0, 10000000));

			//on appelle une fonction qui lui redonne le droit de se teleporter dans une seconde
			Personnage->DebloquerTeleportationFutur();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("erreur portail deconnecte"));
		}
	}	
}

void APortail::connecterDeuxPortails(APortail * portail)
{
	this->autrePortail = portail;
	if (!autrePortail->estConnecte())
		autrePortail->connecterDeuxPortails(this);

	UE_LOG(LogTemp, Warning, TEXT("deux portails connectes"));
	/*
	TextureRenderTarget = NewObject<UTextureRenderTarget2D>(this, UTextureRenderTarget2D::StaticClass());
	TextureRenderTarget->InitAutoFormat(512, 512);
	TextureRenderTarget->UpdateResourceImmediate();*/

	//Capture->TextureTarget = TextureRenderTarget;

	autrePortail->Capture->TextureTarget = TextureRenderTarget;
	autrePortail->Capture->UpdateContent();
	MateriauDynamique = panneau->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MateriauPortail);
	MateriauDynamique->SetTextureParameterValue(TEXT("TexturePrincipale"), TextureRenderTarget);

	/*
	if (MateriauDynamique)
	{
		if (TextureRenderTarget)
		{
			//MateriauDynamique->SetTextureParameterValue(FName("TexturePrincipale"), TextureRenderTarget);
			//change la couleur pour du noir (RGB)
			//meshCompMat->SetVectorParameterValue(FName("Color"), FLinearColor(0.0f, 0.0f, 0.0f));
			//panneau->SetMaterial(0, MateriauDynamique);
		}
	}*/
}

void APortail::SetMateriauPanneau(UMaterialInstanceDynamic * MateriauDynamique)
{
	//panneau->SetMaterial(0, MateriauDynamique);
}

// Called every frame
void APortail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*attente--;
	if (!attente)
	{
		if (TextureRenderTarget)
		{
			if (MateriauDynamique)
			{
				//MateriauDynamique->SetTextureParameterValue(FName("Emissive Color"), TextureRenderTarget);
				//MateriauDynamique->SetVectorParameterValue(FName("Color"), FLinearColor(0.0f, 0.0f, 0.0f));
				MateriauDynamique->SetTextureParameterValue(FName("TexturePrincipale"), TextureRenderTarget);
				if (autrePortail)
				{
					UE_LOG(LogTemp, Warning, TEXT("set texture"));
					autrePortail->SetMateriauPanneau(MateriauDynamique);
				}
			}
		}
		attente = 3;
	}
	if (attente == 3)
	{
		Capture->UpdateContent();
	}*/
}

bool APortail::estConnecte()
{
	if (autrePortail)
		return true;
	else
		return false;
}
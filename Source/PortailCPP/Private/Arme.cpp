// Fill out your copyright notice in the Description page of Project Settings.

#include "Arme.h"


// Sets default values for this component's properties
UArme::UArme()
	:UArme(10, 1.0f, "/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun") {}

UArme::UArme(const int _TailleChargeur, float _TempsRecharge, FString CheminMesh)
	:TailleChargeur( _TailleChargeur ), TempsRecharge(_TempsRecharge)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshArme"));
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj((TEXT("%s"), *CheminMesh));
	mesh->SetSkeletalMeshWithoutResettingAnimation(MeshObj.Object);
	UE_LOG(LogTemp, Warning, TEXT("constructeur UArme"));
	//UKismetSystemLibrary::PrintString(this, *FString("balles dans chargeur : " + FString::FromInt(_tailleChargeur)), true, true, FColor::Red, 5.0f);

	MunitionsDansChargeur = TailleChargeur;
}

// Called when the game starts
void UArme::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

USkeletalMeshComponent * UArme::getMesh()
{
	return mesh;
}

// Called every frame
void UArme::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//recharge l'arme
void UArme::Recharger()
{
	MunitionsDansChargeur = TailleChargeur;
	UE_LOG(LogTemp, Warning, TEXT("RECHARGEMENT"));
	bPeutTirer = true;
	UKismetSystemLibrary::PrintString(this, TEXT("RECHARGEMENT TERMINE"), true, true, FColor::Red, 5.0f);
}

void UArme::LancerRechargement()
{
	FTimerHandle TimerHandle;
	GetOuter()->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UArme::Recharger, TempsRecharge, false);
	bPeutTirer = false;
	UKismetSystemLibrary::PrintString(this, TEXT("RECHARGEMENT"), true, true, FColor::Red, 5.0f);
}

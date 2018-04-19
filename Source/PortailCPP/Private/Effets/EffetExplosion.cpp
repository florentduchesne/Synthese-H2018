// Fill out your copyright notice in the Description page of Project Settings.

#include "EffetExplosion.h"


// Sets default values
AEffetExplosion::AEffetExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEffetExplosion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEffetExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


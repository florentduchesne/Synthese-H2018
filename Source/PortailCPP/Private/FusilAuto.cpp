// Fill out your copyright notice in the Description page of Project Settings.

#include "FusilAuto.h"


UFusilAuto::UFusilAuto()
	:UArme(15, "/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun")
{}

void UFusilAuto::Tirer() {

	UE_LOG(LogTemp, Warning, TEXT("tirer dans fusil auto"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("dans tirer fusil auto"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PIOU PIOU PIOU!!!"));
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "FusilSemiAuto.h"


UFusilSemiAuto::UFusilSemiAuto()
	:UArme(10, "/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun")
{}

void UFusilSemiAuto::Tirer() {

	UE_LOG(LogTemp, Warning, TEXT("tirer dans fusil semi auto"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PIOU! PIOU! PIOU!"));
	}
}
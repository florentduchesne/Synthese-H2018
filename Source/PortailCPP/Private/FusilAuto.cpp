// Fill out your copyright notice in the Description page of Project Settings.

#include "FusilAuto.h"


UFusilAuto::UFusilAuto()
	:UArme(15, 1.0f, "/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun")
{
	UE_LOG(LogTemp, Warning, TEXT("constructeur UFusil auto"));
	TempsRecharge = 3.0f;
}

void UFusilAuto::Tirer() {
	if (bPeutTirer)
	{
		UE_LOG(LogTemp, Warning, TEXT("tirer dans fusil auto"));
		UKismetSystemLibrary::PrintString(this, TEXT("PIOU PIOU PIOU!!!"), true, true, FColor::Red, 5.0f);
	}
}
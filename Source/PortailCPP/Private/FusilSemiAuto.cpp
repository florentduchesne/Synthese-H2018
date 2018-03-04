// Fill out your copyright notice in the Description page of Project Settings.

#include "FusilSemiAuto.h"


UFusilSemiAuto::UFusilSemiAuto()
	:UArme(10, 1.0f, "/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun")
{}

void UFusilSemiAuto::Tirer() {
	if (bPeutTirer)
	{
		UE_LOG(LogTemp, Warning, TEXT("tirer dans fusil semi auto"));
		UKismetSystemLibrary::PrintString(this, TEXT("PIOU! PIOU! PIOU!"), true, true, FColor::Red, 5.0f);
	}
}
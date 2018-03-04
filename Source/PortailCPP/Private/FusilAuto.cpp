// Fill out your copyright notice in the Description page of Project Settings.

#include "FusilAuto.h"


UFusilAuto::UFusilAuto()
	:UArme(15, "/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun")
{
	UE_LOG(LogTemp, Warning, TEXT("constructeur UFusil auto"));
}

void UFusilAuto::Tirer() {
	UE_LOG(LogTemp, Warning, TEXT("tirer dans fusil auto"));
	UKismetSystemLibrary::PrintString(this, TEXT("PIOU PIOU PIOU!!!"), true, true, FColor::Red, 5.0f);
}
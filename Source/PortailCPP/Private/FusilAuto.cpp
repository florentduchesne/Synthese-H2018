// Fill out your copyright notice in the Description page of Project Settings.

#include "FusilAuto.h"


UFusilAuto::UFusilAuto()
	:UArme(3, 1.0f, 20, "/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun")
{
	UE_LOG(LogTemp, Warning, TEXT("constructeur UFusil auto"));
	TempsRecharge = 3.0f;
}

void UFusilAuto::CommencerTir() {
	UE_LOG(LogTemp, Warning, TEXT("tirer dans fusil auto"));
	UKismetSystemLibrary::PrintString(this, TEXT("PIOU PIOU PIOU!!!"), true, true, FColor::Red, 5.0f);
}

void UFusilAuto::TerminerTir()
{
	UE_LOG(LogTemp, Warning, TEXT("tirer dans fusil auto"));
	UKismetSystemLibrary::PrintString(this, TEXT("tir terminé!!!"), true, true, FColor::Red, 5.0f);
}
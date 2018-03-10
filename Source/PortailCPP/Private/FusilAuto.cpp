// Fill out your copyright notice in the Description page of Project Settings.

#include "FusilAuto.h"


UFusilAuto::UFusilAuto()
	:UArme(20, 3.0f, 20, "/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun")
{
	UE_LOG(LogTemp, Warning, TEXT("constructeur UFusil auto"));
	PrimaryComponentTick.bCanEverTick = true;
}

void UFusilAuto::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	
	if (bACommenceTir)
	{
		if (ADesBallesDansChargeur())
		{
			TempsDepuisDernierTir += DeltaTime;
			if (TempsDepuisDernierTir > TempsEntreChaqueTir)
			{
				FaireApparaitreProjectile();
				TempsDepuisDernierTir = 0.0f;
			}
		}
		else
		{
			bACommenceTir = false;
		}
	}
}

void UFusilAuto::CommencerTir() {
	UE_LOG(LogTemp, Warning, TEXT("tirer dans fusil auto"));
	UKismetSystemLibrary::PrintString(this, TEXT("PIOU! PIOU! PIOU!"), true, true, FColor::Red, 5.0f);
	
	bACommenceTir = true;
	
}

void UFusilAuto::TerminerTir()
{
	bACommenceTir = false;
}
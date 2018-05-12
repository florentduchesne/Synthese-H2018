// Fill out your copyright notice in the Description page of Project Settings.

#include "FusilAPompe.h"

UFusilAPompe::UFusilAPompe()
	:UArme(8, 2.0f, 1.0f, 10, 3000, "/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun")
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFusilAPompe::CommencerTir() {
	if (!bTirACommence)
	{
		float Angle1, Angle2, Angle3;
		for (int i = 0; i < BallesParTir; i++)
		{
			Angle1 = FMath::FRandRange(-5.0f, 5.0f);
			Angle2 = FMath::FRandRange(-5.0f, 5.0f);
			Angle3 = FMath::FRandRange(-5.0f, 5.0f);
			FaireApparaitreProjectile(ETypeDeTir::Pompe, FRotator(Angle1, Angle2, Angle3));
		}
		FaireApparaitreProjectile(ETypeDeTir::Pompe, FRotator(0.0f));
		--MunitionsDansChargeur;
		MiseAJourATHJoueur();
	}
}

void UFusilAPompe::TerminerTir()
{

}
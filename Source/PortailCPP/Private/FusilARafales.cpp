// Fill out your copyright notice in the Description page of Project Settings.

#include "FusilARafales.h"

UFusilARafales::UFusilARafales()
	:UArme(15, 2.0f, 20, "/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun")
{
	PrimaryComponentTick.bCanEverTick = true;
}



void UFusilARafales::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bACommenceTir)
	{
		if (PeutTirer())
		{
			if (BallesTireesDansRafale < TirsParRafale)
			{
				TempsDepuisDernierTir += DeltaTime;
				if (TempsDepuisDernierTir > TempsEntreChaqueTir)
				{
					UE_LOG(LogTemp, Warning, TEXT("tirer dans fusil a rafales"));
					BallesTireesDansRafale += 1;
					AProjectile * projectile = FaireApparaitreProjectile();
					projectile->Initialiser(Degats);
					TempsDepuisDernierTir = 0.0f;
				}
			}
			else
			{
				TerminerRafale();
			}
		}
		else
		{
			TerminerRafale();
		}
	}
}

void UFusilARafales::CommencerTir() {
	if (!bACommenceTir)
	{
		BallesTireesDansRafale = 0;
		bACommenceTir = true;
	}
}

void UFusilARafales::TerminerRafale()
{
	bACommenceTir = false;
}

void UFusilARafales::TerminerTir()
{

}
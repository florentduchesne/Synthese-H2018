// Fill out your copyright notice in the Description page of Project Settings.

#include "FusilARafales.h"

UFusilARafales::UFusilARafales()
	:UArme(15, 2.0f, 0.5f, 20, 8000, "SkeletalMesh'/Game/Personnage/Armes/Meshes/Rifle.Rifle'")
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFusilARafales::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bACommenceTir)
	{
		if (ADesBallesDansChargeur())
		{
			if (BallesTireesDansRafale < TirsParRafale)
			{
				TempsDepuisDernierTir += DeltaTime;
				if (TempsDepuisDernierTir > TempsEntreChaqueTir)
				{
					BallesTireesDansRafale += 1;
					SonTir->Play();
					FaireApparaitreProjectile(ETypeDeTir::Normal, FRotator(0.f));
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
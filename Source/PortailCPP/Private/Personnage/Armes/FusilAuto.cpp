// Fill out your copyright notice in the Description page of Project Settings.

#include "FusilAuto.h"


UFusilAuto::UFusilAuto()
	:UArme(20, 1.5f, 0.0f, 20, 5000, "SkeletalMesh'/Game/Personnage/Armes/Meshes/Rifle.Rifle'")
{
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
				SonTir->Play();
				FaireApparaitreProjectile(ETypeDeTir::Normal, FRotator(0.f));
				TempsDepuisDernierTir = 0.0f;
			}
		}
		else
		{
			bACommenceTir = false;
		}
	}
}

void UFusilAuto::CommencerTir() 
{
	bACommenceTir = true;
}

void UFusilAuto::TerminerTir()
{
	bACommenceTir = false;
}
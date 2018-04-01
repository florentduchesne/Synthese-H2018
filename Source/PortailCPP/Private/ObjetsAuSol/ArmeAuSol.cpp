// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmeAuSol.h"

AArmeAuSol::AArmeAuSol()
	:AObjetAuSol(15.0f, "/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun", ETypeDeMeshEnum::SkeletalMesh, ECouleurSocleEnum::Rouge)
{}

bool AArmeAuSol::RamasseParUnJoueur(APersonnage * Personnage)
{
	return Personnage->ChangerArme(ClasseArme);
}
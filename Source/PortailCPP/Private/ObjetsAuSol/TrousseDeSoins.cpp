// Fill out your copyright notice in the Description page of Project Settings.

#include "TrousseDeSoins.h"

ATrousseDeSoins::ATrousseDeSoins()
	:AObjetAuSol(10.0f, "/Engine/BasicShapes/Sphere", ETypeDeMeshEnum::StaticMesh)
{}


bool ATrousseDeSoins::RamasseParUnJoueur(APersonnage * Personnage)
{
	return Personnage->Soigner(NbPointsDeVieRestaures);
}
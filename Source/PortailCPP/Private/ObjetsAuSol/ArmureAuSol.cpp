// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmureAuSol.h"



AArmureAuSol::AArmureAuSol()
	:AObjetAuSol(10.0f, "/Engine/BasicShapes/Cube", ETypeDeMeshEnum::StaticMesh)
{}


bool AArmureAuSol::RamasseParUnJoueur(APersonnage * Personnage)
{
	return Personnage->EquiperArmure(NbArmure);
}
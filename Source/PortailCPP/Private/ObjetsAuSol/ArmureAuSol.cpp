// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmureAuSol.h"

AArmureAuSol::AArmureAuSol()
	:AObjetAuSol(10.0f, "/Game/Objets/Meshes/ArmorHolo", ETypeDeMeshEnum::StaticMesh, ECouleurObjetEnum::Bleu)
{}

bool AArmureAuSol::RamasseParUnJoueur(APersonnage * Personnage)
{
	return Personnage->EquiperArmure(NbArmure);
}
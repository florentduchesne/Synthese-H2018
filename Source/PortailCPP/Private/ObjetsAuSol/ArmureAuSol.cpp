// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmureAuSol.h"

AArmureAuSol::AArmureAuSol()
	:AObjetAuSol(10.0f, "/Game/Niveaux/Appartement/Assets/Meshes/ArmorHolo", ETypeDeMeshEnum::StaticMesh, ECouleurSocleEnum::Bleu)
{}

bool AArmureAuSol::RamasseParUnJoueur(APersonnage * Personnage)
{
	return Personnage->EquiperArmure(NbArmure);
}
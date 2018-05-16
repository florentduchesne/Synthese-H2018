// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmureAuSol.h"

AArmureAuSol::AArmureAuSol()
	:AObjetAuSol(10.0f, "/Game/Objets/Meshes/ArmorHolo", ETypeDeMeshEnum::StaticMesh, ECouleurSocleEnum::Bleu, "/Game/Objets/Materiaux/Mat_Holo_Bleu")
{}

bool AArmureAuSol::RamasseParUnJoueur(APersonnage * Personnage)
{
	return Personnage->EquiperArmure(NbArmure);
}
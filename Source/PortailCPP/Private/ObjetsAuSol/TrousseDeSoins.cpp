// Fill out your copyright notice in the Description page of Project Settings.

#include "TrousseDeSoins.h"

ATrousseDeSoins::ATrousseDeSoins()
	:AObjetAuSol(10.0f, "/Game/Objets/Meshes/HealthPackHolo", ETypeDeMeshEnum::StaticMesh, ECouleurSocleEnum::Vert, "/Game/Objets/Materiaux/Mat_Holo_Vert")
{}


bool ATrousseDeSoins::RamasseParUnJoueur(APersonnage * Personnage)
{
	return Personnage->Soigner(NbPointsDeVieRestaures);
}
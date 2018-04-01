// Fill out your copyright notice in the Description page of Project Settings.

#include "TrousseDeSoins.h"

ATrousseDeSoins::ATrousseDeSoins()
	:AObjetAuSol(10.0f, "/Game/Niveaux/Appartement/Assets/Meshes/HealthPackHolo", ETypeDeMeshEnum::StaticMesh, ECouleurSocleEnum::Vert)
{}


bool ATrousseDeSoins::RamasseParUnJoueur(APersonnage * Personnage)
{
	return Personnage->Soigner(NbPointsDeVieRestaures);
}
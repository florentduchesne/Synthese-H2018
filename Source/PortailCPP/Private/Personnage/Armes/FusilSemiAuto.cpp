// Fill out your copyright notice in the Description page of Project Settings.

#include "FusilSemiAuto.h"


UFusilSemiAuto::UFusilSemiAuto()
	:UArme(10, 1.0f, 0.0f, 35, 10000, "SkeletalMesh'/Game/Personnage/Armes/Meshes/Pistol.Pistol'")
{}

void UFusilSemiAuto::CommencerTir() {
	SonTir->Play();
	FaireApparaitreProjectile(ETypeDeTir::Normal, FRotator(0.f));
}

void UFusilSemiAuto::TerminerTir()
{
	UE_LOG(LogTemp, Warning, TEXT("tir termine"));
}
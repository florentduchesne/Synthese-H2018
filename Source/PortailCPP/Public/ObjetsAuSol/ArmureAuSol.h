// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjetsAuSol/ObjetAuSol.h"
#include "ArmureAuSol.generated.h"

/**
 * 
 */
UCLASS()
class PORTAILCPP_API AArmureAuSol : public AObjetAuSol
{
	GENERATED_BODY()

	AArmureAuSol();
	
	UFUNCTION()
	bool RamasseParUnJoueur(APersonnage * Personnage) override;

	int NbArmure = 50;
};

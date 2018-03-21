// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjetsAuSol/ObjetAuSol.h"
#include "TrousseDeSoins.generated.h"

/**
 * 
 */
UCLASS()
class PORTAILCPP_API ATrousseDeSoins : public AObjetAuSol
{
	GENERATED_BODY()

	ATrousseDeSoins();
	
	UFUNCTION()
	bool RamasseParUnJoueur(APersonnage * Personnage) override;
	
	int NbPointsDeVieRestaures = 50;
};

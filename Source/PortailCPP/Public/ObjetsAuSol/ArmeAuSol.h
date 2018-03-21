// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjetsAuSol/ObjetAuSol.h"
#include "../Personnage/Armes/Arme.h"
#include "ArmeAuSol.generated.h"

/**
 * 
 */
UCLASS()
class PORTAILCPP_API AArmeAuSol : public AObjetAuSol
{
	GENERATED_BODY()

	AArmeAuSol();

	UFUNCTION()
	bool RamasseParUnJoueur(APersonnage * Personnage) override;
	
public:
	//doit etre public pour pouvoir editer en blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmeAuSol")
	TSubclassOf<UArme> ClasseArme;
};

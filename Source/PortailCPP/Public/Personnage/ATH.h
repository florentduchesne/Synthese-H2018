// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ATH.generated.h"

/**
 * 
 */
UCLASS()
class PORTAILCPP_API AATH : public AHUD
{
	GENERATED_BODY()

	AATH();

public:
	UFUNCTION(BlueprintNativeEvent)
		void MiseAJourPV(int PV);
	UFUNCTION(BlueprintNativeEvent)
		void MiseAJourArmure(int Armure);
	UFUNCTION(BlueprintNativeEvent)
		void MiseAJourBallesDansChargeur(int BallesDansChargeur);
	UFUNCTION(BlueprintNativeEvent)
		void MiseAJourBallesMax(int BallesMaxChargeur);
	UFUNCTION(BlueprintNativeEvent)
		void MiseAJourMeutres(int Meurtres);
	
protected:
	int NbPV = 100;
	int Armure = 0;
	int BallesDansChargeur;
	int BallesMaxChargeur;
	int NbMeurtres;
};

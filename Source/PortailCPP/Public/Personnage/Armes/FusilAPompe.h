// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Personnage/Armes/Arme.h"
#include "FusilAPompe.generated.h"

/**
 * 
 */
UCLASS()
class PORTAILCPP_API UFusilAPompe : public UArme
{
	GENERATED_BODY()
	
	const int BallesParTir = 10;
	const float TempsEntreChaqueTir = 1.0f;
	float TempsDepuisDernierTir = 0.0f;

public:

	UFusilAPompe();

	void CommencerTir() override;
	void TerminerTir() override;
	
};

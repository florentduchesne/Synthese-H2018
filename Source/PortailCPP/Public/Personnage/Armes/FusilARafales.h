// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Arme.h"
#include "FusilARafales.generated.h"

/**
 * 
 */
UCLASS()
class PORTAILCPP_API UFusilARafales : public UArme
{
	GENERATED_BODY()

	const int TirsParRafale = 3;
	const float TempsEntreChaqueTir = 0.1f;
	float TempsDepuisDernierTir = 0.0f;

	int BallesTireesDansRafale = 0;

	bool bACommenceTir = false;
	
public:
	
	UFusilARafales();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void TerminerRafale();
	void CommencerTir() override;
	void TerminerTir() override;
	
};

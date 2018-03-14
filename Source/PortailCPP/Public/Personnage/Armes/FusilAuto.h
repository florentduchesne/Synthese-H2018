// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Arme.h"
#include "FusilAuto.generated.h"

/**
 * 
 */
UCLASS()
class PORTAILCPP_API UFusilAuto : public UArme
{
	GENERATED_BODY()

	const int TempsEntreChaqueTir = 0.1f;
	float TempsDepuisDernierTir = 0.0f;
	bool bACommenceTir = false;
	
public:
	UFusilAuto();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void CommencerTir() override;
	void TerminerTir() override;

};

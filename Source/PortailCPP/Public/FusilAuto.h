// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Arme.h"
#include "FusilAuto.generated.h"

/**
 * 
 */
UCLASS()
class PORTAILCPP_API UFusilAuto : public UArme
{
	GENERATED_BODY()

	int TirsParMinute = 1;
	float TempsDepuisDernierTir = 0.0f;
	bool bACommenceTir = false;
	
public:
	UFusilAuto();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void CommencerTir() override;
	void TerminerTir() override;

};

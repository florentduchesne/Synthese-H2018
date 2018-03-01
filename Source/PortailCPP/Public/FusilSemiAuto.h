// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Arme.h"
#include "FusilSemiAuto.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class PORTAILCPP_API UFusilSemiAuto : public UArme
{
	GENERATED_BODY()

public:
	UFusilSemiAuto();

	UFUNCTION()
	void Tirer() override;
	
};

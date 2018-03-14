// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Arme.h"
#include "FusilSemiAuto.generated.h"

/**
 * 
 */
UCLASS()
class PORTAILCPP_API UFusilSemiAuto : public UArme
{
	GENERATED_BODY()

public:
	UFusilSemiAuto();

	void CommencerTir() override;
	void TerminerTir() override;
	
};

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
	
public:
	UFusilAuto();
	
	void Tirer() override;

};

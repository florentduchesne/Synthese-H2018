// Fill out your copyright notice in the Description page of Project Settings.

#include "ModeDeJeu.h"

AModeDeJeu::AModeDeJeu()
	:Super()
{
	DefaultPawnClass = APersonnage::StaticClass();

	// use our custom HUD class
	//HUDClass = AATH::StaticClass();
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Projectile.h"
#include "Arme.generated.h"

UCLASS(abstract)
class PORTAILCPP_API UArme : public USceneComponent
{
	GENERATED_BODY()

public:
	//ne devrait pas être appelé, mais requis pour le bon fonctionnement de Unreal Engine
	UArme();
	//initialise l'arme
	UArme(const int _TailleChargeur, float _TempsRecharge, int Degats, FString CheminMesh);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent * mesh;

	//delai entre la fin d'un tir et le début du suivant
	float DelaiEntreChaqueTir = 0.8f;
	//temps requis pour recharger
	float TempsRecharge;
	//nombre de balles qu'il reste dans le chargeur
	int8 MunitionsDansChargeur;
	//nombre de balles maximal qui entrent dans le chargeur
	int8 TailleChargeur;
	//les dégâts qu'infligent chaque balle lorsqu'elles heurtent un personnage
	int Degats;
	int NoJoueur;

	//vrai si l'arme a terminé son temps de récupération après un tir et est prête à commencer un autre tir
	bool bDelaiEntreChaqueTirTermine = true;
	//vrai si l'arme n'est pas en train de recharger
	bool bADesBallesDansChargeur = true;
	//si oui ou non le tir a commencé
	bool bTirACommence = false;

	//remet des balles dans le chargeur
	void Recharger();
	//on appelle cette fonction après un délai ce qui force un délai minimal entre chaque fin et début de tir
	void DebloquerTirDelai();
	//fait apparaître un projectile et l'initialise
	void FaireApparaitreProjectile();

	///à implémenter dans chacune des sous-classes
	//marque le début d'un "tir" (ou une rafale, selon le type d'arme). C'est le moment où on commence à appuyer sur la gâchette
	virtual void CommencerTir() PURE_VIRTUAL(UArme::CommencerTir, ;);
	//marque la fin d'un tir, le moment où on relâche la gâchette
	virtual void TerminerTir() PURE_VIRTUAL(UArme::TerminerTir, ;);

	//retourne vrai si l'arme n'est pas en train de recharger ET qu'elle a encore une balle ou plus dans le chargeur
	bool ADesBallesDansChargeur();

public:
	USkeletalMeshComponent * getMesh();
	
	//lorsque le joueur appuie sur la touche de tir
	//gère les munitions dans le chargeur et le délai de récupération entre deux tirs, puis appelle "CommencerTir", implémenté dans la sous-classe
	void CommencerTirSuper();
	//lorsque le joueur relache la touche de tir
	void TerminerTirSuper();
	//bloque le tir de l'arme et appelle la méthode "Recharger" après un délai
	void LancerRechargement();

	void SetNoJoueur(int NoJoueur);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Projectile.h"
#include "ProjectileExplosif.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Arme.generated.h"

UENUM(BlueprintType)
enum class ETypeDeTir : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Explosif UMETA(DisplayName = "Explosif"),
	Pompe UMETA(DisplayName = "Pompe")
};

UCLASS(abstract)
class PORTAILCPP_API UArme : public USceneComponent
{
	GENERATED_BODY()

public:
	//ne devrait pas �tre appel�, mais requis pour le bon fonctionnement de Unreal Engine
	UArme();
	//initialise l'arme
	UArme(const int _TailleChargeur, float _TempsRecharge, float _DelaiEntreChaqueTir, int Degats, int VitesseProjectiles, FString CheminMesh);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent * mesh;

	//delai entre la fin d'un tir et le d�but du suivant
	float DelaiEntreChaqueTir;
	//temps requis pour recharger
	float TempsRecharge;
	//nombre de balles qu'il reste dans le chargeur
	int8 MunitionsDansChargeur;
	//nombre de balles maximal qui entrent dans le chargeur
	int8 TailleChargeur;

	float DelaiTirSecondaire = 10.0f;

	//munitions disponibles pour le tir secondaire
	int8 MunitionsSecondaires;
	//les d�g�ts qu'infligent chaque balle lorsqu'elles heurtent un personnage
	int Degats;
	int NoJoueur;
	int VitesseProjectiles;

	//vrai si l'arme a termin� son temps de r�cup�ration apr�s un tir et est pr�te � commencer un autre tir
	bool bDelaiEntreChaqueTirTermine = true;
	//vrai si l'arme a termin� son temps de r�cup�ration apr�s un tir secondaire
	bool bDelaiEntreChaqueTirSecondaireTermine = true;
	//vrai si l'arme n'est pas en train de recharger
	bool bADesBallesDansChargeur = true;
	//si oui ou non le tir a commenc�
	bool bTirACommence = false;

	//remet des balles dans le chargeur
	void Recharger();
	//on appelle cette fonction apr�s un d�lai ce qui force un d�lai minimal entre chaque fin et d�but de tir
	void DebloquerTirDelai();

	void DebloquerTirSecondaireDelai();
	//fait appara�tre un projectile et l'initialise
	void FaireApparaitreProjectile(ETypeDeTir TypeDeTir, FRotator Deviation);

	///� impl�menter dans chacune des sous-classes
	//marque le d�but d'un "tir" (ou une rafale, selon le type d'arme). C'est le moment o� on commence � appuyer sur la g�chette
	virtual void CommencerTir() PURE_VIRTUAL(UArme::CommencerTir, ;);
	//marque la fin d'un tir, le moment o� on rel�che la g�chette
	virtual void TerminerTir() PURE_VIRTUAL(UArme::TerminerTir, ;);

	//retourne vrai si l'arme n'est pas en train de recharger ET qu'elle a encore une balle ou plus dans le chargeur
	bool ADesBallesDansChargeur();

	void MiseAJourATHJoueur();

	UAudioComponent * SonTir;
	USoundCue * SignalSonTir;

public:
	USkeletalMeshComponent * getMesh();
	
	//lorsque le joueur appuie sur la touche de tir
	//g�re les munitions dans le chargeur et le d�lai de r�cup�ration entre deux tirs, puis appelle "CommencerTir", impl�ment� dans la sous-classe
	void CommencerTirSuper();
	//lorsque le joueur relache la touche de tir
	void TerminerTirSuper();

	void TirSecondaire();

	//bloque le tir de l'arme et appelle la m�thode "Recharger" apr�s un d�lai
	void LancerRechargement();

	void SetNoJoueur(int NoJoueur);

	int GetBallesDansChargeur() { return MunitionsDansChargeur; }
	int GetBallesMax() { return TailleChargeur; }
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	
	// Called every frame <AActor>
	virtual void Tick(float DeltaTime) override;
	// </AActor>

	// <IHitInterface>
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	// </IHitInterface>

protected:
	// Called when the game starts or when spawned <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	/**
	* Components
	*/

	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* Attributes;

	/*
	* Weapon
	*/

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class AWeapon* EquippedWeapon;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	void DisableCapsule();

	/**
	* Play Montage Functions
	*/

	virtual int32 PlayAttackMontage();

	virtual int32 PlayDeathMontage();

	void PlayHitReactMontage(const FName& SectionName);

	/**
	* SFX / VFX
	*/

	void PlayHitSound(const FVector& ImpactPoint);

	void SpawnHitParticles(const FVector& ImpactPoint);

	/**
	* Attack Functions
	*/

	virtual bool CanAttack();

	bool IsAlive();

	virtual void Attack();

	virtual void Die();

	virtual void HandleDamage(float DamageAmount);

	void DirectionalHitReact(const FVector& ImpactPoint);

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();	

	/**
	* Montage Sections
	*/

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;

	
private:

	/**
	* Montage Functions
	*/

	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	/**
	* SFX / VFX
	*/

	UPROPERTY(EditAnywhere, Category = Sounds)
	class USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Visual Effects")
	class UParticleSystem* HitParticles;

	/**
	* Animation Montages
	*/

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;
};

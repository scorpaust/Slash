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
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class AWeapon* EquippedWeapon;

	/**
	* Components
	*/

	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* Attributes;

	/**
	* Attack Functions
	*/

	virtual void Attack();

	virtual void Die();

	virtual bool CanAttack();

	bool IsAlive();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	virtual void HandleDamage(float DamageAmount);

	/**
	* Play Montage Functions
	*/

	virtual int32 PlayAttackMontage();

	virtual int32 PlayDeathMontage();

	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	void PlayHitReactMontage(const FName& SectionName);

	void DirectionalHitReact(const FVector& ImpactPoint);

	/**
	* Animation Montages
	*/

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	void DisableCapsule();

	/**
	* Montage Sections
	*/

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;

	/**
	* SFX / VFX
	*/

	void PlayHitSound(const FVector& ImpactPoint);

	void SpawnHitParticles(const FVector& ImpactPoint);

private:

	/**
	* SFX / VFX
	*/

	UPROPERTY(EditAnywhere, Category = Sounds)
	class USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Visual Effects")
	class UParticleSystem* HitParticles;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	AEnemy();

	// Called every frame <AActor>
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void Destroyed() override;	
	// </AActor>

	// <IHitInterface>
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	// </IHitInterface>

protected:
	// Called when the game starts or when spawned <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	/** Weapon */
	void SpawnDefaultWeapon();

	// Play death montage <ABaseCharacter>
	virtual void Die() override; 

	virtual void Attack() override;

	virtual bool CanAttack() override;

	virtual void HandleDamage(float DamageAmount) override;

	virtual void AttackEnd() override;

	virtual int32 PlayDeathMontage() override;
	// </ABaseCharacter>

	UPROPERTY(BlueprintReadOnly, Category = Combat)
	AActor* CombatTarget;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:

	/**
	* AI Behavior
	*/

	void InitializeEnemy();

	void CheckPatrolTarget();

	void CheckCombatTarget();

	void PatrolTimerFinished();
	
	void StartAttackTimer();

	void ClearAttackTimer();

	void HideHealthBar();

	void ShowHealthBar();

	void LoseInterest();

	void StartPatrolling();

	AActor* ChoosePatrolTarget();

	void ClearPatrolTimer();

	void ChaseTarget();

	bool IsOutsideCombatRadius();

	bool IsOutsideAttackRadius();

	bool IsInsideAttackRadius();

	bool IsEngaged();

	bool IsAttacking();

	bool IsChasing();

	bool IsDead();

	bool InTargetRange(AActor* Target, double Radius);

	void MoveToTarget(AActor* Target);

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // Callback for OnPawnSeen in UPawnSensingComponent

	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 1000.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;
	
	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 125.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;	

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 8.f;
};

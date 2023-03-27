// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "SlashCharacter.generated.h"

UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	ASlashCharacter();

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input <AActor>
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// </AActor>

	virtual void Jump() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual void SetOverlappingItem(AItem* Item) override;

	virtual void AddSouls(class ASoul* Soul) override;

	virtual void AddGold(class ATreasure* Treasure) override;

protected:

	// Called when the game starts or when spawned <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* SlashContext;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* DodgeAction;

	/**
	* Callbacks for input
	*/

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void EKeyPressed();

	virtual void Attack() override;

	void Dodge();

	/**
	* Play Montage Functions
	*/

	void PlayEquipMontage(const FName& SectionName);

	/** Combat */

	void EquipWeapon(AWeapon* Weapon);

	virtual void AttackEnd() override;

	virtual void DodgeEnd() override;

	virtual bool CanAttack() override;

	bool CanDisarm();

	bool CanArm();

	void Disarm();

	void Arm();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	virtual void Die() override;

	bool HasEnouthStamina();

	bool IsOccupied();

private:

	/** Character Components */

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	class UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	class AItem* OverlappingItem;

	/** Montages */

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	/** Slash States */

	bool IsUnoccupied();

	UPROPERTY(VisibleAnywhere)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	/** HUD / Overlay */

	UPROPERTY()
	class USlashOverlay* SlashOverlay;

	void InitializeSlashOverlay(APlayerController* PlayerController);

	void SetHUDHealth();

public:

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};

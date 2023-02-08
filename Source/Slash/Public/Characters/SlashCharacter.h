// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SlashCharacter.generated.h"

UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlashCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* SlashContext;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* JumpAction;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	class UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;
};

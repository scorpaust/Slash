// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"

// Sets default values
ASlashCharacter::ASlashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;

	bUseControllerRotationYaw = false;

	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(GetRootComponent());

	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));

	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));

	Hair->SetupAttachment(GetMesh());

	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));

	Eyebrows->SetupAttachment(GetMesh());

	Eyebrows->AttachmentName = FString("head");
}

// Called when the game starts or when spawned
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (Subsystem)
		{
			Subsystem->AddMappingContext(SlashContext, 0);
		}
	}

	Tags.Add(FName("SlashCharacter"));
}

// Called every frame
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState == EActionState::EAS_Unoccupied) {

		const FVector2D MovementVector = Value.Get<FVector2D>();

		const FRotator ControlRotation = GetControlRotation();

		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// const FVector Forward = GetActorForwardVector();

		AddMovementInput(Forward, MovementVector.Y);

		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Right, MovementVector.X);

	}
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookVector.Y);

	AddControllerYawInput(LookVector.X);
}

void ASlashCharacter::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);

	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);

		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;

		OverlappingItem = nullptr;

		EquippedWeapon = OverlappingWeapon;
	}
	else
	{
		if (CanDisarm())
		{
			PlayEquipMontage(FName("Unequip"));

			CharacterState = ECharacterState::ECS_Unequipped;

			ActionState = EActionState::EAS_EquippingWeapon;
		}

		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));

			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;

			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}

bool ASlashCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon;
}

void ASlashCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASlashCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::Attack()
{
	Super::Attack();

	if (CanAttack())
	{
		PlayAttackMontage();

		ActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);

		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

// Called to bind functionality to input
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);

		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EKeyPressed);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
	}
}


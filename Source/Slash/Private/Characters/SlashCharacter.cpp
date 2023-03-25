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
#include "Components/StaticMeshComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"

// Sets default values
ASlashCharacter::ASlashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;

	bUseControllerRotationYaw = false;

	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

	GetMesh()->SetGenerateOverlapEvents(true);

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

// Called to bind functionality to input
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);

		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EKeyPressed);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
	}
}

void ASlashCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

float ASlashCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);

	SetHUDHealth();

	return DamageAmount;
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void ASlashCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void ASlashCharacter::AddSouls(class ASoul* Soul)
{
	UE_LOG(LogTemp, Warning, TEXT("ASlashCharacter::AddSouls"));
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

		InitializeSlashOverlay(PlayerController);
	}

	Tags.Add(FName("EngageableTarget"));
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
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}

		else if (CanArm())
		{
			Arm();
		}
	}
}

void ASlashCharacter::Attack()
{
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

void ASlashCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);

	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;

	OverlappingItem = nullptr;

	EquippedWeapon = Weapon;
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
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

void ASlashCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));

	CharacterState = ECharacterState::ECS_Unequipped;

	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));

	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;

	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::Die()
{
	Super::Die();

	ActionState = EActionState::EAS_Dead;

	DisableMeshCollision();
}

bool ASlashCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void ASlashCharacter::InitializeSlashOverlay(APlayerController* PlayerController)
{
	ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());

	if (SlashHUD)
	{
		SlashOverlay = SlashHUD->GetSlashOverlay();

		if (SlashOverlay && Attributes)
		{
			SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());

			SlashOverlay->SetStaminaBarPercent(1.f);

			SlashOverlay->SetGold(0);

			SlashOverlay->SetSouls(0);
		}
	}
}

void ASlashCharacter::SetHUDHealth()
{
	if (SlashOverlay && Attributes)
	{
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Slash/Public/Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));

	Capsule->SetCapsuleHalfHeight(20.f);

	Capsule->SetCapsuleRadius(15.f);

	RootComponent = Capsule;

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));

	BirdMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	SpringArm->SetupAttachment(RootComponent);

	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));

	ViewCamera->SetupAttachment(SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (Subsystem)
		{
			Subsystem->AddMappingContext(BirdMappingContext, 0);
		}
	}
}

// Deprecated Input System
//void ABird::MoveForward(float Value)
//{
//	if (Controller  && (Value != 0.f))
//	{
//		FVector Forward = GetActorForwardVector();
//
//		AddMovementInput(Forward, Value);
//	}
//}

void ABird::Move(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();

	if (DirectionValue)
	{
		FVector Forward = GetActorForwardVector();
		
		AddMovementInput(Forward, DirectionValue);
	}
}

void ABird::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);

		AddControllerPitchInput(LookAxisValue.Y);
	}


}

// Deprecated Input System
//void ABird::Turn(float Value)
//{
//	AddControllerYawInput(Value);
//}
//
//void ABird::LookUp(float Value)
//{
//	AddControllerPitchInput(Value);
//}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
	}

	// Deprecated Input System
	// PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABird::MoveForward);

	// PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABird::Turn);

	// PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABird::LookUp);

}


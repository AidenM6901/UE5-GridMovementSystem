// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMovement/GridPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGridPlayerCharacter::AGridPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationYaw = false;

}

// Called when the game starts or when spawned
void AGridPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

    // Disable auto-rotation to use manual SetActorRotation or SetControlRotation
    GetCharacterMovement()->bOrientRotationToMovement = false;

    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMapping, 0);
        }
    }
	
}

// Called every frame
void AGridPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // SMOOTH MOVEMENT
    if (bIsMoving)
    {
        FVector CurrentLocation = GetActorLocation();
        FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, MovementInterpSpeed);
        SetActorLocation(NewLocation);

        // If close enough, snap to target
        if (FVector::Dist(NewLocation, TargetLocation) < 0.5f)
        {
            SetActorLocation(TargetLocation); // Snap to final position
            bIsMoving = false;
            bCanMove = true;
        }
    }

    // SMOOTH ROTATION
    if (bIsRotating)
    {
        FRotator CurrentRot = GetActorRotation();
        FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRotation, DeltaTime, RotationInterpSpeed);
        SetActorRotation(NewRot);

        // If close enough, snap to target
        if (FMath::Abs((NewRot - TargetRotation).Yaw) < 0.5f)
        {
            SetActorRotation(TargetRotation); // Snap to final rotation
            bIsRotating = false;
            bCanMove = true;
        }
    }

    FVector Location = GetActorLocation();
    // Print to screen for 5 seconds, white color
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White,
        FString::Printf(TEXT("Location: X=%.1f Y=%.1f Z=%.1f"), Location.X, Location.Y, Location.Z));

}

// Called to bind functionality to input
void AGridPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Started, this, &AGridPlayerCharacter::MoveForward);
        EnhancedInput->BindAction(MoveBackwardAction, ETriggerEvent::Started, this, &AGridPlayerCharacter::MoveBackward);
        EnhancedInput->BindAction(TurnLeftAction, ETriggerEvent::Started, this, &AGridPlayerCharacter::TurnLeft);
        EnhancedInput->BindAction(TurnRightAction, ETriggerEvent::Started, this, &AGridPlayerCharacter::TurnRight);
    }

}

void AGridPlayerCharacter::MoveForward(const FInputActionValue& Value)
{
    if (!bCanMove || bIsMoving || bIsRotating) return;

    float InputValue = Value.Get<float>();
    if (InputValue > 0.1f)
    {
        TargetLocation = GetActorLocation() + GetActorForwardVector() * GridStepDistance;
        bIsMoving = true;
        bCanMove = false;
    }
}

void AGridPlayerCharacter::MoveBackward(const FInputActionValue& Value)
{
    if (!bCanMove || bIsMoving || bIsRotating) return;

    float InputValue = Value.Get<float>();
    if (InputValue > 0.1f)
    {
        TargetLocation = GetActorLocation() - GetActorForwardVector() * GridStepDistance;
        bIsMoving = true;
        bCanMove = false;
    }
}

void AGridPlayerCharacter::TurnLeft(const FInputActionValue& Value)
{
    if (!bCanMove || bIsMoving || bIsRotating) return;

    float InputValue = Value.Get<float>();
    if (InputValue > 0.1f)
    {
        FRotator CurrentRot = GetActorRotation();
        TargetRotation = FRotator(0.0f, CurrentRot.Yaw - RotationAngle, 0.0f);
        bIsRotating = true;
        bCanMove = false;
    }
}

void AGridPlayerCharacter::TurnRight(const FInputActionValue& Value)
{
    if (!bCanMove || bIsMoving || bIsRotating) return;

    float InputValue = Value.Get<float>();
    if (InputValue > 0.1f)
    {
        FRotator CurrentRot = GetActorRotation();
        TargetRotation = FRotator(0.0f, CurrentRot.Yaw + RotationAngle, 0.0f);
        bIsRotating = true;
        bCanMove = false;
    }
}

void AGridPlayerCharacter::ResetMove()
{
    bCanMove = true;
}


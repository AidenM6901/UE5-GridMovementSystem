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
 	// Set this character to call Tick() every frame.
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
        MoveElapsed += DeltaTime;
        
        float Alpha = FMath::Clamp(MoveElapsed / MoveDuration, 0.f, 1.f);

        // Generic Movement
        //FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);

        // Ease In and Out System (For natural looking movement)
        float SmoothAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.0f); // Last param = curve strength
        FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, SmoothAlpha);

        SetActorLocation(NewLocation);

        if (Alpha >= 1.f)
        {
            //SetActorLocation(TargetLocation); // Final snap
            bIsMoving = false;
            bCanMove = true;
        }
    }

    // SMOOTH ROTATION
    if (bIsRotating)
    {
        RotationElapsed += DeltaTime;
        float Alpha = FMath::Clamp(RotationElapsed / RotationDuration, 0.f, 1.f);

        // Optional: easing
        float SmoothAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.0f);

        // Interpolate rotation manually
        FRotator NewRotation = FMath::Lerp(StartRotation, TargetRotation, SmoothAlpha);
        SetActorRotation(NewRotation);

        if (Alpha >= 1.f)
        {
            SetActorRotation(TargetRotation); // Final snap
            bIsRotating = false;
            bCanMove = true;
        }

        // Debug Rotation
        GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow,
            FString::Printf(TEXT("Alpha: %.2f | Yaw: %.2f"), Alpha, NewRotation.Yaw));
    }


    // Debug

    // Debug Location
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
        StartLocation = GetActorLocation();
        TargetLocation = StartLocation + GetActorForwardVector() * GridStepDistance;
        MoveElapsed = 0.f;
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
        StartLocation = GetActorLocation();
        TargetLocation = StartLocation - GetActorForwardVector() * GridStepDistance;
        MoveElapsed = 0.f;
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
        StartRotation = GetActorRotation();
        TargetRotation = FRotator(0.f, StartRotation.Yaw - RotationAngle, 0.f);
        RotationElapsed = 0.f;
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
        StartRotation = GetActorRotation();
        TargetRotation = FRotator(0.f, StartRotation.Yaw + RotationAngle, 0.f);
        RotationElapsed = 0.f;
        bIsRotating = true;
        bCanMove = false;
    }
}


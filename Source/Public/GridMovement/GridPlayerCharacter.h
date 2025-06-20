// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GridPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class CRAWLSPACE_API AGridPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGridPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Input callbacks
	void MoveForward(const FInputActionValue& Value);
	void MoveBackward(const FInputActionValue& Value);
	void TurnLeft(const FInputActionValue& Value);
	void TurnRight(const FInputActionValue& Value);

	// Variables
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	bool bCanMove = true;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float GridStepDistance = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float RotationAngle = 90.f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MovementCooldown = 1.f;

	// Target values to interpolate to
	FVector TargetLocation;
	FRotator TargetRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsMoving = false;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsRotating = false;

	// Configurable speed values
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementInterpSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationInterpSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveDuration = 0.2f;       // Time to complete one step
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveElapsed = 0.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector StartLocation;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationDuration = 0.2f;      // How long to complete a 90° turn
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationElapsed = 0.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	FRotator StartRotation;

	// Input system properties
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveForwardAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveBackwardAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* TurnLeftAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* TurnRightAction;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

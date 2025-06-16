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

	// Cooldown reset
	void ResetMove();

	// Variables
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	bool bCanMove = true;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float GridStepDistance = 500.f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float RotationAngle = 90.f;

	// Target values to interpolate to
	FVector TargetLocation;
	FRotator TargetRotation;

	bool bIsMoving = false;
	bool bIsRotating = false;

	// Configurable speed values
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementInterpSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationInterpSpeed = 5.0f;

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

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ApoZCharacter.generated.h"

// Forward declarations pour les composants utilis�s
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class ULocomotionComponent;

UCLASS()
class APOZ_API AApoZCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AApoZCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// === Camera ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FPSCameraComponent;

	// === Enhanced Input ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;

	// === Composants syst�mes (ex: plugin Locomotion) ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	ULocomotionComponent* LocomotionComponent;

	// === Fonctions appel�es par Enhanced Input ===
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SprintStart(const FInputActionValue& Value);
	void SprintStop(const FInputActionValue& Value);
};

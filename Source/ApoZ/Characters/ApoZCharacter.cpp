#include "ApoZCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "LocomotionComponent.h"

AApoZCharacter::AApoZCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	// Camera setup (TrueFPS avec spring arm = 0)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 0.f;
	SpringArm->bUsePawnControlRotation = true;

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCameraComponent->SetupAttachment(SpringArm);

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Ajout du composant plugin Locomotion
	LocomotionComponent = CreateDefaultSubobject<ULocomotionComponent>(TEXT("LocomotionComponent"));
}

void AApoZCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Ajoute le MappingContext EnhancedInput
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (PlayerMappingContext)
			{
				Subsystem->AddMappingContext(PlayerMappingContext, 0);
			}
		}
	}
}

void AApoZCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AApoZCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
			Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AApoZCharacter::Move);

		if (LookAction)
			Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AApoZCharacter::Look);

		if (JumpAction)
		{
			Input->BindAction(JumpAction, ETriggerEvent::Started, this, &AApoZCharacter::Jump);
			Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &AApoZCharacter::StopJumping);
		}

		// Sprint (plugin Locomotion)
		if (SprintAction)
		{
			Input->BindAction(SprintAction, ETriggerEvent::Started, this, &AApoZCharacter::SprintStart);
			Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &AApoZCharacter::SprintStop);
		}

		// Crouch (plugin Locomotion)
		if (CrouchAction)
		{
			Input->BindAction(CrouchAction, ETriggerEvent::Started, this, &AApoZCharacter::CrouchStart);
			Input->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AApoZCharacter::CrouchStop);
		}
	}
}

void AApoZCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller && (MovementVector != FVector2D::ZeroVector))
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, MovementVector.Y);
		AddMovementInput(Right, MovementVector.X);
	}
}

void AApoZCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Sprint
void AApoZCharacter::SprintStart(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Appel SprintStart depuis le perso !"));
	if (LocomotionComponent)
	{
		LocomotionComponent->StartSprinting();
	}
}
void AApoZCharacter::SprintStop(const FInputActionValue& Value)
{
	if (LocomotionComponent)
	{
		LocomotionComponent->StopSprinting();
	}
}

// Crouch
void AApoZCharacter::CrouchStart(const FInputActionValue& Value)
{
	if (LocomotionComponent)
	{
		LocomotionComponent->StartCrouching();
	}
}
void AApoZCharacter::CrouchStop(const FInputActionValue& Value)
{
	if (LocomotionComponent)
	{
		LocomotionComponent->StopCrouching();
	}
}

void AApoZCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Ajoute ici la réplication de propriétés supplémentaires si besoin
}

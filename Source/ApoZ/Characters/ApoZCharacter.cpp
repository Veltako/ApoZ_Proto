#include "ApoZCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InventoryWidget.h"
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

	// Création du composant d'inventaire (sera visible dans l'éditeur si besoin)
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void AApoZCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Donne le mapping des touches au joueurs
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


	// Ajoute 2 bandages dans le sac principal au début du jeu (exemple)
	if (InventoryComponent)
	{
		FGridItemInstance NewItem;
		NewItem.ItemID = FName("Bandage");
		NewItem.Quantity = 2;
		// X, Y, Rotated si tu veux les placer précisément (optionnel)
		InventoryComponent->AddItemToContainer(
			FName("MainBag"),
			FName("Bandage"), // ou NewItem.ItemID
			0,                // X
			0,                // Y
			false,            // bRotated
			2                 // Quantity
		);
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

		// Toggle inventory (plugin Inventory)
		if (ToggleInventoryAction)
		{
			Input->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AApoZCharacter::ToggleInventory);
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
	UE_LOG(LogTemp, Warning, TEXT("Appel CrouchStart depuis le perso !"));
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

void AApoZCharacter::ToggleInventory(const FInputActionValue& Value)
{
	if (InventoryWidget && InventoryWidget->IsInViewport())
	{
		InventoryWidget->RemoveFromParent();
		InventoryWidget = nullptr;
		return;
	}

	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		if (UInventoryWidget* InvWidget = Cast<UInventoryWidget>(InventoryWidget))
		{
			InvWidget->InventoryComponent = InventoryComponent;
			InvWidget->AddToViewport();
		}
	}
}


void AApoZCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Ajoute ici la réplication de propriétés supplémentaires si besoin
}

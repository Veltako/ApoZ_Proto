#include "ApoZCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "../UI/HUD/WBP_Inventory.h"
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

	// Ajout du composant plugin Inventory
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryWidget = nullptr;
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

	// Test de l'inventaire � supp
	if (InventoryComponent)
	{
		FInventoryItem Potion(FName(TEXT("Potion")), 2);
		InventoryComponent->AddItem(Potion);

		// Affiche tout l'inventaire dans le log
		for (const FInventoryItem& Item : InventoryComponent->Items)
		{
			if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				UE_LOG(LogTemp, Warning, TEXT("INVENTORY for %s: %s x%d"),
					*GetName(), *Item.ItemID.ToString(), Item.Quantity);
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

// Inventory
void AApoZCharacter::ToggleInventory()
{
	if (InventoryWidget && InventoryWidget->IsInViewport())
	{
		InventoryWidget->RemoveFromParent();
		InventoryWidget = nullptr;
		return;
	}
	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass);

		// Le cast permet d'acc�der � la variable InventoryComponent expos�e dans le BP widget (WBP_Inventory)
		if (UWBP_Inventory* InvWidget = Cast<UWBP_Inventory>(InventoryWidget))
		{
			InvWidget->InventoryComponent = InventoryComponent;
		}
		InventoryWidget->AddToViewport();
	}
}



void AApoZCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Ajoute ici la r�plication de propri�t�s suppl�mentaires si besoin
}

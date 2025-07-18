#include "ApoZCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Net/UnrealNetwork.h"

AApoZCharacter::AApoZCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// --------- Réplication réseau -----------
	bReplicates = true;
	SetReplicateMovement(true);

	// --------- Spring Arm et Caméra -----------
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 0.f; // FPS: caméra sur la tête
	SpringArm->bUsePawnControlRotation = true;

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCameraComponent->SetupAttachment(SpringArm);
	FPSCameraComponent->bUsePawnControlRotation = false; // Le springarm s'occupe de la rotation

	// --------- Options de rotation ----------
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AApoZCharacter::BeginPlay()
{
	Super::BeginPlay();

	// -------- Ajoute le MappingContext ENHANCED INPUT -------
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && PlayerMappingContext)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
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

void AApoZCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AApoZCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Ajoute ici tes propriétés répliquées si besoin (exemple : DOREPLIFETIME(AApoZCharacter, MaVariable))
}

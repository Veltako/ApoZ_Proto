#include "LocomotionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ULocomotionComponent::ULocomotionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

// Sprint
void ULocomotionComponent::StartSprinting()
{
    bIsSprinting = true;
    if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
    {
        Char->GetCharacterMovement()->MaxWalkSpeed = 1200.f;
        UE_LOG(LogTemp, Warning, TEXT("MaxWalkSpeed = %f"), Char->GetCharacterMovement()->MaxWalkSpeed);
    }
}

void ULocomotionComponent::StopSprinting()
{
    bIsSprinting = false;
    if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
    {
        Char->GetCharacterMovement()->MaxWalkSpeed = 400.f;
    }
}

// Crounch
void ULocomotionComponent::StartCrouching()
{
    bIsCrouching = true;
    if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    {
        OwnerChar->Crouch();
    }
}

void ULocomotionComponent::StopCrouching()
{
    bIsCrouching = false;
    if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    {
        OwnerChar->UnCrouch();
    }
}

//  Réplication
void ULocomotionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ULocomotionComponent, bIsSprinting);
    DOREPLIFETIME(ULocomotionComponent, bIsCrouching);
}

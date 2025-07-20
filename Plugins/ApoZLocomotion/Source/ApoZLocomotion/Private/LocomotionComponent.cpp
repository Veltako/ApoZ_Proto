#include "LocomotionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ULocomotionComponent::ULocomotionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void ULocomotionComponent::StartSprinting()
{
    bIsSprinting = true;
    if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
    {
        Char->GetCharacterMovement()->MaxWalkSpeed = 800.f;
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

void ULocomotionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ULocomotionComponent, bIsSprinting);
}

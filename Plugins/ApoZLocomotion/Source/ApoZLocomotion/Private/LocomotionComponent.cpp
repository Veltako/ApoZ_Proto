#include "LocomotionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ULocomotionComponent::ULocomotionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

// ------------- SPRINT -------------
void ULocomotionComponent::StartSprinting()
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        Server_SetSprinting(true);
        return;
    }
    bIsSprinting = true;
    OnRep_Sprinting();
}
void ULocomotionComponent::StopSprinting()
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        Server_SetSprinting(false);
        return;
    }
    bIsSprinting = false;
    OnRep_Sprinting();
}
void ULocomotionComponent::Server_SetSprinting_Implementation(bool bSprint)
{
    bIsSprinting = bSprint;
    OnRep_Sprinting();
}
void ULocomotionComponent::OnRep_Sprinting()
{
    if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    {
        float Speed = bIsSprinting ? 1200.f : 400.f;
        OwnerChar->GetCharacterMovement()->MaxWalkSpeed = Speed;
    }
}

// ------------- CROUCH -------------
void ULocomotionComponent::StartCrouching()
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        Server_SetCrouching(true);
        return;
    }
    bIsCrouching = true;
    OnRep_Crouching();
}
void ULocomotionComponent::StopCrouching()
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        Server_SetCrouching(false);
        return;
    }
    bIsCrouching = false;
    OnRep_Crouching();
}
void ULocomotionComponent::Server_SetCrouching_Implementation(bool bCrouch)
{
    bIsCrouching = bCrouch;
    OnRep_Crouching();
}
void ULocomotionComponent::OnRep_Crouching()
{
    if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    {
        // Securité pour la nav agent
        OwnerChar->GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

        // Essaie de mettre cette ligne même si l'autocomplétion rale :
        if (bIsCrouching)
            OwnerChar->Crouch();
        else
            OwnerChar->UnCrouch();
    }
}

// --------- REPLICATION ---------
void ULocomotionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ULocomotionComponent, bIsSprinting);
    DOREPLIFETIME(ULocomotionComponent, bIsCrouching);
}

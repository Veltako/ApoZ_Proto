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
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        // Demande au serveur d'exécuter la fonction
        Server_StartSprinting();
        return;
    }

    bIsSprinting = true;
    if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    {
        OwnerChar->GetCharacterMovement()->MaxWalkSpeed = 1200.f;
        UE_LOG(LogTemp, Warning, TEXT("MaxWalkSpeed (sprint) = %f"), OwnerChar->GetCharacterMovement()->MaxWalkSpeed);
    }
}

void ULocomotionComponent::StopSprinting()
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        // Demande au serveur d'exécuter la fonction
        Server_StopSprinting();
        return;
    }

    bIsSprinting = false;
    if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    {
        OwnerChar->GetCharacterMovement()->MaxWalkSpeed = 400.f;
        UE_LOG(LogTemp, Warning, TEXT("MaxWalkSpeed (unsprint) = %f"), OwnerChar->GetCharacterMovement()->MaxWalkSpeed);
    }
}

void ULocomotionComponent::Server_StartSprinting_Implementation()
{
    StartSprinting();
}

void ULocomotionComponent::Server_StopSprinting_Implementation()
{
    StopSprinting();
}

// --------- CROUCH ---------
void ULocomotionComponent::StartCrouching()
{
    bIsCrouching = true;
    if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    {
        // Active pour la nav agent
        OwnerChar->GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
        // Active pour le movement en général
        OwnerChar->Crouch();
        OwnerChar->GetCharacterMovement()->MaxWalkSpeedCrouched = 200.f;
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

void ULocomotionComponent::Server_StartCrouching_Implementation()
{
    StartCrouching();
}

void ULocomotionComponent::Server_StopCrouching_Implementation()
{
    StopCrouching();
}

// --------- REPLICATION ---------
void ULocomotionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ULocomotionComponent, bIsSprinting);
    DOREPLIFETIME(ULocomotionComponent, bIsCrouching);
}

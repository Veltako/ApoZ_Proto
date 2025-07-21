#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "LocomotionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class APOZLOCOMOTION_API ULocomotionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    ULocomotionComponent();

    // Réplication
    UPROPERTY(ReplicatedUsing = OnRep_Sprinting)
    bool bIsSprinting = false;

    UPROPERTY(ReplicatedUsing = OnRep_Crouching)
    bool bIsCrouching = false;

    // Sprint
    UFUNCTION(BlueprintCallable, Category = "Locomotion")
    void StartSprinting();
    UFUNCTION(BlueprintCallable, Category = "Locomotion")
    void StopSprinting();

    UFUNCTION(Server, Reliable)
    void Server_SetSprinting(bool bSprint);

    UFUNCTION()
    void OnRep_Sprinting();

    // Crouch
    UFUNCTION(BlueprintCallable, Category = "Locomotion")
    void StartCrouching();
    UFUNCTION(BlueprintCallable, Category = "Locomotion")
    void StopCrouching();

    UFUNCTION(Server, Reliable)
    void Server_SetCrouching(bool bCrouch);

    UFUNCTION()
    void OnRep_Crouching();

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

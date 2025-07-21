#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h" // Pour la réplication
#include "LocomotionComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class APOZLOCOMOTION_API ULocomotionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    ULocomotionComponent();

    // === SPRINt ===
    UPROPERTY(Replicated, BlueprintReadOnly, Category="Locomotion")
    bool bIsSprinting = false;

    UFUNCTION(BlueprintCallable, Category="Locomotion")
    void StartSprinting();

    UFUNCTION(BlueprintCallable, Category="Locomotion")
    void StopSprinting();

    UFUNCTION(Server, Reliable)
    void Server_StartSprinting();

    UFUNCTION(Server, Reliable)
    void Server_StopSprinting();

    // === CROUCH ===
    UPROPERTY(Replicated, BlueprintReadOnly, Category="Locomotion")
    bool bIsCrouching = false;

    UFUNCTION(BlueprintCallable, Category="Locomotion")
    void StartCrouching();

    UFUNCTION(BlueprintCallable, Category="Locomotion")
    void StopCrouching();

    UFUNCTION(Server, Reliable)
    void Server_StartCrouching();

    UFUNCTION(Server, Reliable)
    void Server_StopCrouching();

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

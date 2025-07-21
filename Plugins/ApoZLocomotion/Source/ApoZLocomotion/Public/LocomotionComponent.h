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

    // Etat du sprint (r�pliqu�)
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Locomotion")
    bool bIsSprinting = false;

    UFUNCTION(BlueprintCallable, Category = "Locomotion")
    void StartSprinting();

    UFUNCTION(BlueprintCallable, Category = "Locomotion")
    void StopSprinting();

    // Etat du crouch (r�pliqu�)
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Locomotion")
    bool bIsCrouching = false;

    UFUNCTION(BlueprintCallable, Category = "Locomotion")
    void StartCrouching();

    UFUNCTION(BlueprintCallable, Category = "Locomotion")
    void StopCrouching();

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

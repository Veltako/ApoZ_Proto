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

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Locomotion")
    bool bIsSprinting = false;

    UFUNCTION(BlueprintCallable, Category = "Locomotion")
    void StartSprinting();

    UFUNCTION(BlueprintCallable, Category = "Locomotion")
    void StopSprinting();

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

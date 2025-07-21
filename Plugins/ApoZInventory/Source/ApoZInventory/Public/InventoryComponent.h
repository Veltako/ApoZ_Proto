#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItem.h"
#include "Net/UnrealNetwork.h"
#include "InventoryComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class APOZINVENTORY_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

    // Tableau d’items dans l’inventaire, répliqué
    UPROPERTY(Replicated, BlueprintReadOnly, Category="Inventory")
    TArray<FInventoryItem> Items;

    // Fonction pour ajouter un item
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool AddItem(const FInventoryItem& NewItem);

    // Fonction pour retirer un item (ou diminuer la quantité)
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool RemoveItem(FName ItemID, int32 Quantity = 1);

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
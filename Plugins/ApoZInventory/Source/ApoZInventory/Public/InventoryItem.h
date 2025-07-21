#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.generated.h"

// Structure de base pour représenter un item dans l'inventaire
USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_BODY()

    // Un identifiant unique (Utiliser comme clé pour retrouver l'item dans une DataTable ou un DataAsset)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    // Quantité (pour les objets empilables)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity;

    // Constructeurs
    FInventoryItem()
        : ItemID(NAME_None), Quantity(1) {
    }

    FInventoryItem(FName InID, int32 InQuantity)
        : ItemID(InID), Quantity(InQuantity) {
    }
};

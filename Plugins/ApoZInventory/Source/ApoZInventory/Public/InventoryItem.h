#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.generated.h"

// Structure de base pour repr�senter un item dans l'inventaire
USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_BODY()

    // Un identifiant unique (Utiliser comme cl� pour retrouver l'item dans une DataTable ou un DataAsset)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    // Quantit� (pour les objets empilables)
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

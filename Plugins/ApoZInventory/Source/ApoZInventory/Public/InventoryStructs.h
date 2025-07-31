#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryStructs.generated.h"

// ----- Description d’un objet (row de DataTable) ----- //
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Width = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Height = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Weight = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StackMax = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon = nullptr;

    // On peut ajouter description, type, etc.
};

// ----- Instance d’un item posé dans la grille ----- //
USTRUCT(BlueprintType)
struct FGridItemInstance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 X = 0; // Colonne

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Y = 0; // Ligne

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRotated = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Durability = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AmmoInMag = 0;
    // Optionnel : GUID unique
};

// ----- Container (sac, poche, coffre…) ----- //
USTRUCT(BlueprintType)
struct FInventoryContainer
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ContainerID; // "MainBag", "Vest", etc.

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 GridWidth = 6;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 GridHeight = 6;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxWeight = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FGridItemInstance> Items;
};
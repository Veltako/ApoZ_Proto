#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryStructs.h"
#include "InventoryItemWidget.generated.h"

UCLASS()
class APOZINVENTORY_API UInventoryItemWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
    FGridItemInstance ItemInstance;

    // Pour l’UI : setup l’affichage avec data C++
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    virtual void Setup(const FGridItemInstance& InItemInstance);
};

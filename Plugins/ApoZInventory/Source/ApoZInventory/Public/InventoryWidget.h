#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "InventoryWidget.generated.h"

UCLASS()
class APOZINVENTORY_API UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    /** Set depuis le personnage avant l’affichage du widget */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
    UInventoryComponent* InventoryComponent;

    // Tu pourras faire une fonction d’update ici plus tard
};

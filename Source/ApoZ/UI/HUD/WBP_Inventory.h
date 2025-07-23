#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h" // Inclure si besoin d'accès direct à la classe inventaire
#include "WBP_Inventory.generated.h"

UCLASS()
class APOZ_API UWBP_Inventory : public UUserWidget
{
    GENERATED_BODY()

public:
    // Référence à l'inventaire à afficher
    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Inventory")
    UInventoryComponent* InventoryComponent;

    // Si tu veux un accès direct à ta VerticalBox pour le listing
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* VB_ItemList;

    // Tu peux override NativeConstruct pour faire ton refresh à chaque apparition du widget
    virtual void NativeConstruct() override;
};

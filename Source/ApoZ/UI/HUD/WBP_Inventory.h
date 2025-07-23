#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h" // Inclure si besoin d'acc�s direct � la classe inventaire
#include "WBP_Inventory.generated.h"

UCLASS()
class APOZ_API UWBP_Inventory : public UUserWidget
{
    GENERATED_BODY()

public:
    // R�f�rence � l'inventaire � afficher
    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Inventory")
    UInventoryComponent* InventoryComponent;

    // Si tu veux un acc�s direct � ta VerticalBox pour le listing
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* VB_ItemList;

    // Tu peux override NativeConstruct pour faire ton refresh � chaque apparition du widget
    virtual void NativeConstruct() override;
};

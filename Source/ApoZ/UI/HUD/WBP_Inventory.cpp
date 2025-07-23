#include "WBP_Inventory.h"
#include "Components/VerticalBox.h"

void UWBP_Inventory::NativeConstruct()
{
    Super::NativeConstruct();

    // Tu peux faire ici le refresh de l'inventaire à chaque ouverture
    // Par exemple : efface tout, puis affiche chaque item (en BP ou C++)

    if (!InventoryComponent || !VB_ItemList) return;

    VB_ItemList->ClearChildren(); // Vide la liste

    for (const FInventoryItem& Item : InventoryComponent->Items)
    {
        // Ici tu peux créer dynamiquement un widget item custom,
        // ou un simple TextBlock (plus simple à faire en Blueprint)
        // Exemple pseudo-code :
        /*
        UTextBlock* ItemText = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
        ItemText->SetText(FText::Format(NSLOCTEXT("Inv", "ItemFormat", "{0} x{1}"), FText::FromName(Item.ItemID), FText::AsNumber(Item.Quantity)));
        VB_ItemList->AddChild(ItemText);
        */
    }
}

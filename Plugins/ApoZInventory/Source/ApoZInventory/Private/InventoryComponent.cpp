#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

bool UInventoryComponent::AddItem(const FInventoryItem& NewItem)
{
    // Cherche si l’item existe déjà (empilable)
    for (FInventoryItem& Item : Items)
    {
        if (Item.ItemID == NewItem.ItemID)
        {
            Item.Quantity += NewItem.Quantity;
            return true;
        }
    }
    // Sinon, ajoute un nouvel item
    Items.Add(NewItem);
    return true;
}

bool UInventoryComponent::RemoveItem(FName ItemID, int32 Quantity)
{
    for (int32 i = 0; i < Items.Num(); ++i)
    {
        if (Items[i].ItemID == ItemID)
        {
            if (Items[i].Quantity > Quantity)
            {
                Items[i].Quantity -= Quantity;
                return true;
            }
            else
            {
                Items.RemoveAt(i);
                return true;
            }
        }
    }
    return false;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UInventoryComponent, Items);
}

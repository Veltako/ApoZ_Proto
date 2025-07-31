#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UInventoryComponent, Containers);
}

FInventoryContainer* UInventoryComponent::FindContainerByID(FName ContainerID)
{
    for (FInventoryContainer& Container : Containers)
    {
        if (Container.ContainerID == ContainerID)
            return &Container;
    }
    return nullptr;
}

float UInventoryComponent::GetContainerWeight(const FInventoryContainer& Container) const
{
    float Total = 0.f;
    if (!ItemDataTable) return 0.f;
    for (const FGridItemInstance& Item : Container.Items)
    {
        FItemData* Data = ItemDataTable->FindRow<FItemData>(Item.ItemID, TEXT("GetWeight"));
        if (Data)
        {
            Total += Data->Weight * Item.Quantity;
        }
    }
    return Total;
}

// EXEMPLE : Ajout d’un item (place dispo et poids max NON encore vérifiés !)
bool UInventoryComponent::AddItemToContainer(FName ContainerID, FName ItemID, int32 X, int32 Y, bool bRotated, int32 Quantity)
{
    FInventoryContainer* Container = FindContainerByID(ContainerID);
    if (!Container || !ItemDataTable) return false;

    // (Vérifier place libre et poids ICI !)

    FGridItemInstance NewItem;
    NewItem.ItemID = ItemID;
    NewItem.X = X;
    NewItem.Y = Y;
    NewItem.bRotated = bRotated;
    NewItem.Quantity = Quantity;

    Container->Items.Add(NewItem);
    // TODO: Notify events/UI

    return true;
}

bool UInventoryComponent::RemoveItemFromContainer(FName ContainerID, int32 GridIndex)
{
    FInventoryContainer* Container = FindContainerByID(ContainerID);
    if (!Container || !Container->Items.IsValidIndex(GridIndex))
        return false;
    Container->Items.RemoveAt(GridIndex);
    // TODO: Notify events/UI
    return true;
}

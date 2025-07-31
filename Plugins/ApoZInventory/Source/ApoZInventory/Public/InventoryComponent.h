#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryStructs.h"
#include "InventoryComponent.generated.h"

/**
 * Composant d'inventaire GRID
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class APOZINVENTORY_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

    // Containers de ce personnage (MainBag, Vest, Pockets, etc)
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
    TArray<FInventoryContainer> Containers;

    // DataTable des items possibles
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    UDataTable* ItemDataTable;

    // Ajouter un item à un container
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItemToContainer(FName ContainerID, FName ItemID, int32 X, int32 Y, bool bRotated, int32 Quantity);

    // Supprimer un item (par index dans la grille)
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItemFromContainer(FName ContainerID, int32 ItemIndex);

    // Chercher un container par ID
    FInventoryContainer* FindContainerByID(FName ContainerID);

    // Poids total dans un container
    float GetContainerWeight(const FInventoryContainer& Container) const;

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

class ItemSave
{
    string Name;

    string LocationType;
    int X;
    int Y;

    int Quantity;
    float ItemWet;
    float ItemHealth;

    bool HasAttachments;
    ref array<ItemSave> ItemAttachments;

    bool HasCargo;
    ref CargoSave Cargo;

    void ItemSave()
    {
        ItemAttachments = new array<ItemSave>;
        Cargo = new CargoSave;
    }

    void ~ItemSave()
    {
        ItemAttachments.Clear();
    }

    EntityAI Create(ref EntityAI oParent) {
        EntityAI oEntity;

        if (LocationType == "Cargo") {
            oEntity = oParent.GetInventory().CreateEntityInCargoEx(Name, 0, X, Y);
        } else if (LocationType == "Attachment") {
            oEntity = oParent.GetInventory().CreateAttachment(Name);
        } else {
            oEntity = oParent.GetInventory().CreateInInventory(Name);
        }

        ItemBase oItem = ItemBase.Cast(oEntity);
        oItem.SetWet(ItemWet);
        oItem.SetHealth("", "", ItemHealth);

        if (oItem.IsInherited(Magazine))
        {
            Magazine oMagazine = Magazine.Cast( oItem );
            oMagazine.LocalSetAmmoCount(Quantity);
        } else {
            oItem.SetQuantity(Quantity);
        }

        if (HasAttachments) {
            for (int i = 0; i < ItemAttachments.Count(); i++) 
            {
                ItemAttachments[i].Create(oItem);
            }
        }

        if (HasCargo) {
            Cargo.Create(oItem);
        }

        return oEntity;
    }
}
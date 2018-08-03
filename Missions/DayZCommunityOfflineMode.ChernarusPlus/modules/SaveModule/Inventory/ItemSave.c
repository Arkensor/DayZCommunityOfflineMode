class ItemSave
{
    string Name;

    string LocationType;
    int X;
    int Y;

    int Quantity;
    float Wet;
    float Health;

    bool HasAttachments;
    List<ItemSave> Attachments;

    bool HasCargo;
    CargoSave Cargo;

    void ItemSave()
    {
        Attachments = new List<ItemSave>;
        Cargo = new CargoSave;
    }

    void ~ItemSave()
    {
        Attachments.Clear();
    }

    EntityAI Create(EntityAI oParent) {
        
        EntityAI oEntity;

        if (LocationType == "Cargo") {
            oEntity = oParent.GetInventory().CreateEntityInCargoEx(Name, 0, X, Y);
        } else if (LocationType == "Attachment") {
            oEntity = oParent.GetInventory().CreateAttachment(Name);
        } else {
            oEntity = oParent.GetInventory().CreateInInventory(Name);
        }

        ItemBase oItem = ItemBase.Cast(oEntity);
        oItem.SetWet(Wet);
        oItem.SetHealth("", "", Health);

        if (oItem.IsInherited(Magazine))
        {
            Magazine oMagazine = Magazine.Cast( oItem );
            oMagazine.LocalSetAmmoCount(Quantity);
        } else {
            oItem.SetQuantity(Quantity);
        }

        if (HasAttachments) {
            for (int i = 0; i < Attachments.Length(); i++) 
            {
                Attachments[i].Create(oItem);
            }
        }

        if (HasCargo) {
            Cargo.Create(oItem);
        }
    }
}
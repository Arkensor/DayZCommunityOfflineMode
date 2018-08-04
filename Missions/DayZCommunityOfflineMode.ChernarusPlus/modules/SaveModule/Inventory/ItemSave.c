class ItemSave
{
    string Name;

    int LocationType; // 0: None, 1: Cargo, 2: Attachment
    int X;
    int Y;

    int Quantity;
    float ItemWet;
    float ItemHealth;

    bool HasAttachments;
    ref array<ref ItemSave> ItemAttachments;

    bool HasCargo;
    ref array<ref ItemSave> ItemCargo;

    void ItemSave()
    {
        HasAttachments = false;
        ItemAttachments = new array<ref ItemSave>;
        HasCargo = false;
        ItemCargo = new array<ref ItemSave>;
    }

    void ~ItemSave()
    {
        ItemAttachments.Clear();
        ItemCargo.Clear();
    }

    EntityAI Create(ref EntityAI oParent) {
        EntityAI oEntity;

        if (LocationType == 1) // Cargo
        {
            oEntity = oParent.GetInventory().CreateEntityInCargoEx(Name, 0, X, Y);
        } else if (LocationType == 2) // Attachment
        {
            oEntity = oParent.GetInventory().CreateAttachment(Name);
        } else // None
        {
            oEntity = oParent.GetInventory().CreateInInventory(Name);
        }

        ItemBase oItem = ItemBase.Cast(oEntity);
        oItem.SetWet(ItemWet);
        oItem.SetHealth("", "", ItemHealth);

        if (oItem.IsInherited(Magazine))
        {
            Magazine oMagazine = Magazine.Cast( oItem );
            oMagazine.LocalSetAmmoCount(Quantity);
        } else
        {
            oItem.SetQuantity(Quantity);
        }

        if (HasAttachments)
        {
            for (int iAttachment = 0; iAttachment < ItemAttachments.Count(); iAttachment++) 
            {
                ItemAttachments[iAttachment].Create(oItem);
            }
        }

        if (HasCargo)
        {
            for (int iCargo = 0; iCargo < ItemCargo.Count(); iCargo++) 
            {
                ItemCargo[iCargo].Create(oEntity);
            }
        }

        return oEntity;
    }

    bool Save(ref EntityAI oEntity) {
        auto oCargo = oEntity.GetInventory().GetCargo();
        ref ItemSave oItemSave = NULL;

        ItemBase oItem = NULL;
        if (Class.CastTo(oItem, oEntity) && oEntity.IsItemBase())
        {
            Name = oItem.GetType();

            Quantity = GetItemQuantity(oItem);
            ItemWet = oItem.GetWet();
            ItemHealth = oItem.GetHealth("", "");
        } else {
            // return false;
        }

		if (oCargo)
		{
            for (int iCargo = 0; iCargo < oCargo.GetItemCount(); iCargo++)
            {
                EntityAI oCEntity = oCargo.GetItem(iCargo);

                if (oCEntity)
                {
                    oItemSave = new ItemSave;
                    if (oItemSave.Save(oCEntity)) {
                        HasCargo = true;

                        oItemSave.LocationType = 1; // Cargo

                        int iX, iY;
                        oCargo.GetItemRowCol(iCargo, iX, iY );
                        oItemSave.X = iX;
                        oItemSave.Y = iY;

                        ItemCargo.Insert(oItemSave);
                    }
                }
            }
        }

        for (int iAttachment = 0; iAttachment < oEntity.GetInventory().AttachmentCount(); iAttachment++)
		{
			EntityAI oAttachment = oEntity.GetInventory().GetAttachmentFromIndex(iAttachment);

            if (oAttachment)
            {
                oItemSave = new ItemSave;
                if (oItemSave.Save(oAttachment)) {
                    HasAttachments = true;

                    oItemSave.LocationType = 2; // Attachment

                    ItemAttachments.Insert(oItemSave);
                }
            }
        }

        return true;
    }

    private float GetItemQuantity( InventoryItem oItem )
	{
		float fQuantity = 0;
		
		if ( oItem.IsInherited( InventoryItem ) )
		{
			ItemBase oItembase = ItemBase.Cast( oItem );
			
			if ( oItem.IsInherited( Magazine) )
			{
				Magazine oMagazine = Magazine.Cast( oItem );
				
				fQuantity = oMagazine.GetAmmoCount();
			}
			else
			{
				fQuantity = oItembase.GetQuantity();
			}
		}
		return fQuantity;
	}
}
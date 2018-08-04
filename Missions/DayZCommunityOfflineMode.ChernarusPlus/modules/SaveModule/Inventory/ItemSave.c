int COM_DEFAULT_TYPE = 0;
int COM_CARGO_TYPE = 1;
int COM_ATTACHMENT_TYPE = 2;

class ItemSave
{
    string Name;

    int LocationType; // 0: None, 1: Cargo, 2: Attachment
    int X;
    int Y;

    int Quantity;
    float ItemWet;
    float ItemHealth;

    int NumAttachments;
    ref array<ref ItemSave> ItemAttachments;

    int NumItems;
    ref array<ref ItemSave> Items;

    void ItemSave()
    {
        NumAttachments = 0;
        ItemAttachments = new array<ref ItemSave>;
        NumItems = 0;
        Items = new array<ref ItemSave>;
    }

    void ~ItemSave()
    {
        ItemAttachments.Clear();
        Items.Clear();
    }

    void Load(EntityAI oParent) {
        EntityAI oEntity;

        if (LocationType == COM_CARGO_TYPE) // Cargo
        {
            oEntity = oParent.GetInventory().CreateEntityInCargoEx(Name, 0, X, Y);
        } else if (LocationType == COM_ATTACHMENT_TYPE) // Attachment
        {
            oEntity = oParent.GetInventory().CreateAttachment(Name);
        } else // None
        {
            oEntity = oParent.GetInventory().CreateInInventory(Name);
        }

        ItemBase oItem = NULL;
        
        if (Class.CastTo(oItem, oEntity) && oEntity.IsItemBase())
        {
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
        } else {
            // return false;
        }

        if (NumAttachments > 0)
        {
            for (int iAttachment = 0; iAttachment < NumAttachments; iAttachment++) 
            {
                ItemAttachments[iAttachment].Load(oEntity);
            }
        }

        if (NumItems > 0)
        {
            for (int iItem = 0; iItem < NumItems; iItem++) 
            {
                Items[iItem].Load(oEntity);
            }
        }
    }

    bool Save(EntityAI oEntity) {
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
                        NumItems++;

                        oItemSave.LocationType = COM_CARGO_TYPE; // Cargo

                        int iX, iY;
                        oCargo.GetItemRowCol(iCargo, iX, iY );
                        oItemSave.X = iX;
                        oItemSave.Y = iY;

                        Items.Insert(oItemSave);
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
                    NumAttachments++;

                    oItemSave.LocationType = COM_ATTACHMENT_TYPE; // Attachment

                    ItemAttachments.Insert(oItemSave);
                }
            }
        }

        return true;
    }

    private float GetItemQuantity( InventoryItem oItem )
	{
		float fQuantity = 1;
		
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
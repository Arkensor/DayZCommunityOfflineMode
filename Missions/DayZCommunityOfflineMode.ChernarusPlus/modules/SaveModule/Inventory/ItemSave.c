int COM_DEFAULT_TYPE = 0;
int COM_CARGO_TYPE = 1;
int COM_ATTACHMENT_TYPE = 2;
int COM_GROUND = 2;

class ItemSave
{
    string Name;

    int LocationType; // 0: None, 1: Cargo, 2: Attachment
    int X;
    int Y;

    int Quantity;
    float ItemWet;
    float ItemHealth;

    int IQuickBarIndex;

    int NumAttachments;
    ref array<ref ItemSave> ItemAttachments;

    int NumItems;
    ref array<ref ItemSave> Items;

    void ItemSave()
    {
        IQuickBarIndex = -1;

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

    EntityAI Load(EntityAI oParent, PlayerBase oPlayer) {
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

            if (IQuickBarIndex != -1) 
            {
                oPlayer.SetQuickBarEntityShortcut(oItem, IQuickBarIndex);
            }
        }

        if (NumAttachments > 0)
        {
            for (int iAttachment = 0; iAttachment < NumAttachments; iAttachment++) 
            {
                FixAndLoadAttachment(ItemAttachments[iAttachment], oEntity, oPlayer);
            }
        }

        if (NumItems > 0)
        {
            for (int iItem = 0; iItem < NumItems; iItem++) 
            {
                Items[iItem].Load(oEntity, oPlayer);
            }
        }

        return oEntity;
    }

    bool Save(EntityAI oEntity, PlayerBase oPlayer) {
        auto oCargo = oEntity.GetInventory().GetCargo();
        ref ItemSave oItemSave = NULL;

        ItemBase oItem = NULL;

        if (Class.CastTo(oItem, oEntity) && oEntity.IsItemBase())
        {
            Name = oItem.GetType();

            Quantity = GetItemQuantity(oItem);
            ItemWet = oItem.GetWet();
            ItemHealth = oItem.GetHealth("", "");
            
            IQuickBarIndex = oPlayer.FindQuickBarEntityIndex(oItem);
        }

		if (oCargo)
		{
            for (int iCargo = 0; iCargo < oCargo.GetItemCount(); iCargo++)
            {
                EntityAI oCEntity = oCargo.GetItem(iCargo);

                if (oCEntity)
                {
                    oItemSave = new ItemSave;
                    if (oItemSave.Save(oCEntity, oPlayer)) {
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
                if (oItemSave.Save(oAttachment, oPlayer)) {
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

    private EntityAI FixAndLoadAttachment(ItemSave oItemSave, EntityAI oParent, PlayerBase oPlayer) {
        ItemBase oItem = NULL;
        EntityAI oEntity = NULL;

        if (oItemSave.Name.Contains("Mag")) {
            oItemSave.LocationType = COM_DEFAULT_TYPE;
            oEntity = oItemSave.Load(oPlayer, oPlayer);
        } else {
            oEntity = oItemSave.Load(oParent, oPlayer);
        }

        if (Class.CastTo(oItem, oEntity) && oEntity.IsItemBase())
        {
            if (oItem.IsInherited(Magazine))
            {
                Magazine oMagazine = Magazine.Cast( oItem );
                oPlayer.GetWeaponManager().AttachMagazine( oMagazine );
            }
        }
        
        return oEntity;
    }
}
class ItemLoad
{
    static EntityAI Load(EntityAI oParent, PlayerBase oPlayer, ItemData oData) {
        EntityAI oEntity;

        switch(oData.LocationType)
        {
            case COM_CARGO_TYPE:
                oEntity = oParent.GetInventory().CreateEntityInCargoEx(oData.Name, 0, oData.X, oData.Y);
                break;
            case COM_ATTACHMENT_TYPE:
                oEntity = oParent.GetInventory().CreateAttachment(oData.Name);
                break;
            case COM_GROUND:
		        InventoryLocation dest = new InventoryLocation;
		        dest.SetGround(NULL, oPlayer.GetPosition());
                oEntity = SpawnEntity(oData.Name, dest);
                break;
            case COM_DEFAULT_TYPE:
            default:
                oEntity = oParent.GetInventory().CreateInInventory(oData.Name);
                break;
        }

        ItemBase oItem = NULL;
        
        if (Class.CastTo(oItem, oEntity) && oEntity.IsItemBase())
        {
            oItem.SetWet(oData.ItemWet);
            oItem.SetHealth("", "", oData.ItemHealth);

            if (oItem.IsInherited(Magazine))
            {
                Magazine oMagazine = Magazine.Cast( oItem );
                oMagazine.LocalSetAmmoCount(oData.Quantity);
            } else
            {
                oItem.SetQuantity(oData.Quantity);
            }

            if (oData.IQuickBarIndex != -1) 
            {
                oPlayer.SetQuickBarEntityShortcut(oItem, oData.IQuickBarIndex);
            }
        }

        if (oData.NumAttachments > 0)
        {
            for (int iAttachment = 0; iAttachment < oData.NumAttachments; iAttachment++) 
            {
                FixAndLoadAttachment(oEntity, oPlayer, oData.ItemAttachments[iAttachment]);
            }
        }

        if (oData.NumItems > 0)
        {
            for (int iItem = 0; iItem < oData.NumItems; iItem++) 
            {
                ItemLoad.Load(oEntity, oPlayer, oData.Items[iItem]);
            }
        }

        return oEntity;
    }

    protected static EntityAI FixAndLoadAttachment(EntityAI oParent, PlayerBase oPlayer, ItemData oItemData) {
        ItemBase oItem = NULL;
        EntityAI oEntity = NULL;

        if (oItemData.Name.Contains("Mag")) {
            oItemData.LocationType = COM_DEFAULT_TYPE;
            oEntity = ItemLoad.Load(oPlayer, oPlayer, oItemData);
        } else {
            oEntity = ItemLoad.Load(oParent, oPlayer, oItemData);
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
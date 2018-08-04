class HandSave : ItemSave
{

    EntityAI LoadAsChild(ItemSave oItemSave, EntityAI oParent, PlayerBase oPlayer) {
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

    override EntityAI Load(EntityAI oParent, PlayerBase oPlayer) {
        EntityAI oEntity;

        ItemBase oItem = NULL;

        oEntity = oParent.GetInventory().CreateInInventory(Name);
        
        if (Class.CastTo(oItem, oEntity) && oEntity.IsItemBase())
        {
            oPlayer.LocalTakeEntityToHands( oItem );

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
                LoadAsChild(ItemAttachments[iAttachment], oEntity, oPlayer);
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
}
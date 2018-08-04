class HandSave : ItemSave
{

    EntityAI LoadAsChild(ItemSave oItemSave, EntityAI oParent, PlayerBase oPlayer) {
        ItemBase oItem = NULL;
        
        EntityAI oEntity = oItemSave.Load(oParent);

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

    EntityAI LoadHand(EntityAI oParent, PlayerBase oPlayer) {
        EntityAI oEntity;

        ItemBase oItem = NULL;

        oEntity = oParent.GetInventory().CreateInInventory(Name);
        
        if (Class.CastTo(oItem, oEntity) && oEntity.IsItemBase())
        {
            oItem.SetWet(ItemWet);
            oItem.SetHealth("", "", ItemHealth);

            if (oItem.IsInherited(Magazine))
            {
                Magazine oMagazine = Magazine.Cast( oItem );
                oMagazine.LocalSetAmmoCount(Quantity);

                // oPlayer.GetWeaponManager().AttachMagazine( oMagazine );
            } else
            {
                oItem.SetQuantity(Quantity);
            }

            oPlayer.LocalTakeEntityToHands( oItem );
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
                Items[iItem].Load(oEntity);
            }
        }

        return oEntity;
    }
}
class ItemSave
{
    static ItemData Save(EntityAI oEntity, PlayerBase oPlayer, ref ItemData oThisData) {
        auto oCargo = oEntity.GetInventory().GetCargo();
        ref ItemData oItemData = NULL;

        ItemBase oItem = NULL;

        if (Class.CastTo(oItem, oEntity) && oEntity.IsItemBase())
        {
            oThisData.Name = oItem.GetType();

            oThisData.Quantity = oThisData.GetItemQuantity(oItem);
            oThisData.ItemWet = oItem.GetWet();
            oThisData.ItemHealth = oItem.GetHealth("", "");
            
            oThisData.IQuickBarIndex = oPlayer.FindQuickBarEntityIndex(oItem);
        }

		if (oCargo)
		{
            for (int iCargo = 0; iCargo < oCargo.GetItemCount(); iCargo++)
            {
                EntityAI oCEntity = oCargo.GetItem(iCargo);

                if (oCEntity)
                {
                    oItemData = new ItemData;
                    if (ItemSave.Save(oCEntity, oPlayer, oItemData)) {
                        oThisData.NumItems++;

                        oItemData.LocationType = COM_CARGO_TYPE; // Cargo

                        int iX, iY;
                        oCargo.GetItemRowCol(iCargo, iX, iY );
                        oItemData.X = iX;
                        oItemData.Y = iY;

                        oThisData.Items.Insert(oItemData);
                    }
                }
            }
        }

        for (int iAttachment = 0; iAttachment < oEntity.GetInventory().AttachmentCount(); iAttachment++)
		{
			EntityAI oAttachment = oEntity.GetInventory().GetAttachmentFromIndex(iAttachment);

            if (oAttachment)
            {
                oItemData = new ItemData;
                if (ItemSave.Save(oAttachment, oPlayer, oItemData)) {
                    oThisData.NumAttachments++;

                    oItemData.LocationType = COM_ATTACHMENT_TYPE; // Attachment

                    oThisData.ItemAttachments.Insert(oItemData);
                }
            }
        }

        return oThisData;
    }
}
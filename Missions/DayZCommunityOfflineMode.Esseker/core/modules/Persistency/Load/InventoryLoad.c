class InventoryLoad : ItemLoad
{
    static override EntityAI Load(EntityAI oParent, PlayerBase oPlayer, ItemData oData) {
        if (oData.NumAttachments > 0)
        {
            for (int iAttachment = 0; iAttachment < oData.NumAttachments; iAttachment++) 
            {
                FixAndLoadAttachment(oParent, oPlayer, oData.ItemAttachments[iAttachment]);
            }
        }

        return NULL;
    }
}
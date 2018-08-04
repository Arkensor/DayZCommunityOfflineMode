class InventorySave : ItemSave
{
    override EntityAI Load(EntityAI oParent, PlayerBase oPlayer) {
        if (NumAttachments > 0)
        {
            for (int iAttachment = 0; iAttachment < NumAttachments; iAttachment++) 
            {
                ItemAttachments[iAttachment].Load(oParent, oPlayer);
            }
        }

        return NULL;
    }
}
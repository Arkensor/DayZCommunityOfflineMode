class InventorySave : ItemSave
{
    override EntityAI Load(EntityAI oParent, PlayerBase oPlayer) {
        if (NumAttachments > 0)
        {
            for (int iAttachment = 0; iAttachment < NumAttachments; iAttachment++) 
            {
                FixAndLoadAttachment(ItemAttachments[iAttachment], oParent, oPlayer);
            }
        }

        return NULL;
    }
}
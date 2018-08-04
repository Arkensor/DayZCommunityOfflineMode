class InventorySave : ItemSave
{
    override EntityAI Load(EntityAI oParent) {
        if (NumAttachments > 0)
        {
            for (int iAttachment = 0; iAttachment < NumAttachments; iAttachment++) 
            {
                ItemAttachments[iAttachment].Load(oParent);
            }
        }

        return NULL;
    }
}s
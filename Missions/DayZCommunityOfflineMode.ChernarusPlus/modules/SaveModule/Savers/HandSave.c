class HandSave : ItemSave
{
    override EntityAI Load(EntityAI oParent, PlayerBase oPlayer) {
        EntityAI oEntity = super.Load(oParent, oPlayer);
        ItemBase oItem = NULL;
        
        if (Class.CastTo(oItem, oEntity) && oEntity.IsItemBase())
        {
            oPlayer.LocalTakeEntityToHands( oItem );
        }

        return oEntity;
    }
}
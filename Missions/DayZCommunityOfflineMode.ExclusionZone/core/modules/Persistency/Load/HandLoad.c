class HandLoad : ItemLoad
{
    static override EntityAI Load(EntityAI oParent, PlayerBase oPlayer, ItemData oData) {
        EntityAI oEntity = ItemLoad.Load(oParent, oPlayer, oData);
        ItemBase oItem = NULL;
        
        if (Class.CastTo(oItem, oEntity) && oEntity.IsItemBase())
        {
            oPlayer.LocalTakeEntityToHands( oItem );
        }

        return oEntity;
    }
}
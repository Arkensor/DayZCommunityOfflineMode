class CargoSave extends ItemSave
{
    bool HasItems;
    List<ItemSave> Items;

    void CargoSave()
    {
        Items = new List<ItemSave>;
    }

    void ~CargoSave()
    {
        Items.Clear();
    }

    override EntityAI Create(EntityAI oParent)
    {
        EntityAI oEntity = super.Create(oParent);

        if (HasItems) {
            for (int i = 0; i < Items.Length(); i++) 
            {
                Items[i].Create(oEntity);
            }
        }
    }
}
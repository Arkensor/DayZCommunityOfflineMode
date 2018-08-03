class CargoSave extends ItemSave
{
    bool HasItems;
    ref array<ItemSave> Items;

    void CargoSave()
    {
        Items = new array<ItemSave>;
    }

    void ~CargoSave()
    {
        Items.Clear();
    }

    override EntityAI Create(ref EntityAI oParent)
    {
        ref EntityAI oEntity = super.Create(oParent);

        if (HasItems) {
            for (int i = 0; i < Items.Count(); i++) 
            {
                Items[i].Create(oEntity);
            }
        }

        return oEntity;
    }
}
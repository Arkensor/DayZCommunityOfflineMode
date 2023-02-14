int COM_DEFAULT_TYPE = 0;
int COM_CARGO_TYPE = 1;
int COM_ATTACHMENT_TYPE = 2;
int COM_GROUND = 3;

class ItemData
{
    string Name;

    int LocationType; // 0: None, 1: Cargo, 2: Attachment, 3: Ground
    int X;
    int Y;

    int Quantity;
    float ItemWet;
    float ItemHealth;

    int IQuickBarIndex;

    int NumAttachments;
    ref array<ref ItemData> ItemAttachments;

    int NumItems;
    ref array<ref ItemData> Items;

    void ItemData()
    {
        IQuickBarIndex = -1;

        NumAttachments = 0;
        ItemAttachments = new array<ref ItemData>;
        NumItems = 0;
        Items = new array<ref ItemData>;
    }

    void ~ItemData()
    {
        ItemAttachments.Clear();
        Items.Clear();
    }

    float GetItemQuantity( InventoryItem oItem )
	{
		float fQuantity = 1;
		
		if ( oItem.IsInherited( InventoryItem ) )
		{
			ItemBase oItembase = ItemBase.Cast( oItem );
			
			if ( oItem.IsInherited( Magazine) )
			{
				Magazine oMagazine = Magazine.Cast( oItem );
				
				fQuantity = oMagazine.GetAmmoCount();
			}
			else
			{
				fQuantity = oItembase.GetQuantity();
			}
		}
		return fQuantity;
	}
}
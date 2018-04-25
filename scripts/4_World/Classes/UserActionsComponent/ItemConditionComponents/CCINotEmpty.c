class CCINotEmpty : CCIBase
{
	override bool Can( PlayerBase player, ItemBase item )
	{
		if ( item && item.GetQuantity() > 0 )
		{ 
			return true;
		}
		return false;
	}
};
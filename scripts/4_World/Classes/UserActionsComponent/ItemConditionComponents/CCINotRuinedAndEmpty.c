class CCINotRuinedAndEmpty : CCIBase
{
	override bool Can( PlayerBase player, ItemBase item )
	{
		if ( item && !item.IsDamageDestroyed() && item.GetQuantity() > 0 ) 
		{
			return true;
		}
		return false;
	}
};
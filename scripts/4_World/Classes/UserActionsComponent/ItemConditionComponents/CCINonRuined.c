class CCINonRuined : CCIBase
{
	override bool Can( PlayerBase player, ItemBase item )
	{
		if ( item && !item.IsDamageDestroyed() )
		{
			return true;
		}
		return false;
	}
};
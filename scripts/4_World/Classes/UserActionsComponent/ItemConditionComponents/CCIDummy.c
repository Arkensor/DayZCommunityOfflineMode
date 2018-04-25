class CCIDummy : CCIBase
{
	override bool Can( PlayerBase player, ItemBase item )
	{
		if( item ) 
		{
			return true;
		}
		return false;
	}
};
class CCINotPresent : CCIBase
{
	override bool Can( PlayerBase player, ItemBase item )
	{
		if ( !item )
		{ 
			return true;
		}
		
		return false;
	}
	
	override bool CanContinue( PlayerBase player, ItemBase item )
	{
		if (Can( player,item) )
		{
			return true;
		}
		return false;
	}
};
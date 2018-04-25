class CCTBase
{
	bool Can( PlayerBase player, ActionTarget target )
	{
		return true;
	}
	
	bool CanContinue( PlayerBase player, ActionTarget target )
	{
		if( Can(player,target) )
		{
			return true;
		}
		return false;
	}
};
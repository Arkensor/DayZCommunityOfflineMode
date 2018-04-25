class CCTSelf : CCTBase
{
	override bool Can( PlayerBase player, ActionTarget target )
	{
		if ( player && !player.IsDamageDestroyed() ) 
		{
			return true;
		}
		return false;
	}
	
	override bool CanContinue( PlayerBase player, ActionTarget target )
	{
		if ( Can(player,target) ) 
		{
			return true;
		}
		return false;
	}
};
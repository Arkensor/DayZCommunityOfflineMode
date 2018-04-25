class CCTTree : CCTBase
{
	protected float m_MaximalActionDistance;
	
	void CCTTree ( float maximal_target_distance )
	{
		m_MaximalActionDistance = maximal_target_distance;
	}
	
	override bool Can( PlayerBase player, ActionTarget target )
	{	
		if( !target )
		{
			return false;
		}
		Object targetObject = target.GetObject();
		if ( player && targetObject )
		{
			if ( targetObject.IsDamageDestroyed() )
			{
				return false;
			}
			float distance; 
			if( !GetGame().IsMultiplayer() || GetGame().IsClient() ) 
			{
				distance = Math.AbsInt(vector.Distance(target.GetCursorHitPos(),player.GetPosition()));
				if ( distance <= m_MaximalActionDistance ) 
				{	
					return true;			
				}
			}
			else
			{
				//jtomasik - no way how to measure distance properly here
				return true;	
			}
		}
		return false;
	}
};
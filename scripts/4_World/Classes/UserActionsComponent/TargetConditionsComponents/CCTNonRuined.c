class CCTNonRuined : CCTBase
{
	protected float m_MaximalActionDistance;
	
	void CCTNonRuined ( float maximal_target_distance )
	{		
		m_MaximalActionDistance = maximal_target_distance * maximal_target_distance;
	}
	
	override bool Can( PlayerBase player, ActionTarget target )
	{	
		if( !target )
		{
			return false;
		}		
		
		Object targetObject = target.GetObject();
		if ( player && targetObject && !targetObject.IsMan() && !targetObject.IsDamageDestroyed() )
		{	
			float distance = Math.AbsFloat(vector.DistanceSq(targetObject.GetPosition(),player.GetPosition()));
			if ( distance <= m_MaximalActionDistance )
			{
				return true;
			}	
		}
		return false;
	}
};
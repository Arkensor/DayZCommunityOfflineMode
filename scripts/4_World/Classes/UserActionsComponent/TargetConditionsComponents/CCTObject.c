class CCTObject : CCTBase
{
	protected float m_MaximalActionDistanceSq;
	
	void CCTObject ( float maximal_target_distance )
	{		
		m_MaximalActionDistanceSq = maximal_target_distance * maximal_target_distance;
	}
	
	override bool Can( PlayerBase player, ActionTarget target )
	{	
		Object targetObject = target.GetObject();
		if( !targetObject || !player )
		{
			return false;
		}
		vector playerHeadPos;
		MiscGameplayFunctions.GetHeadBonePos(player, playerHeadPos);
		float distanceRoot = Math.AbsFloat(vector.DistanceSq(targetObject.GetPosition(), player.GetPosition()));
		float distanceHead = Math.AbsFloat(vector.DistanceSq(targetObject.GetPosition(), playerHeadPos));
		if ( distanceRoot <= m_MaximalActionDistanceSq || distanceHead <= m_MaximalActionDistanceSq )
		{
			return true;
		}
		return false;
	}		
};
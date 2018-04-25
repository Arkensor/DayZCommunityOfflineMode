class CCTMan : CCTBase
{
	protected float m_MaximalActionDistance;
	
	void CCTMan ( float maximal_target_distance )
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
		if ( player && targetObject && targetObject != player && targetObject.IsMan() )
		{
			PlayerBase man = PlayerBase.Cast(targetObject);
			float distance = Math.AbsFloat(vector.Distance(man.GetPosition(),player.GetPosition()));
			if ( !man.IsDamageDestroyed() && distance <= m_MaximalActionDistance && player.IsFacingTarget(targetObject) )
			{
				return true;
			}	
		}
		return false;
	}
};
class BulletHitReaction
{
	const float ALPHA_MIN = 0;
	const float ALPHA_MAX = 0.2;
	float m_HitDuration;
	float m_BreakPoint;
	float m_TimeActive;
	
	void BulletHitReaction()
	{
		m_HitDuration = 0.30;
		m_BreakPoint = 0.05;
	}
	
	void ~BulletHitReaction()
	{
		PPEffects.HitEffect(0);
	}
	
	void Update(float deltatime)
	{
		float value;
		
		if( m_TimeActive <= m_BreakPoint )
		{
			value = Math.InverseLerp(0, m_BreakPoint, m_TimeActive);
		}
		else
		{
			float tmp_value = Math.InverseLerp(m_BreakPoint, m_HitDuration, m_TimeActive);
			value = 1 - tmp_value;
		}
		m_TimeActive += deltatime;
		
		float val = Math.Lerp(ALPHA_MIN, ALPHA_MAX, value);
		//Print(val);
		PPEffects.HitEffect(val);
		if(m_TimeActive >= m_HitDuration)
		{
			delete this;
		}
	}
	
}
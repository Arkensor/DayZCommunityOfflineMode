class FeverBlurState extends StateBase
{
	Material m_MatGauss;
	const int BLUR_DURATION_MIN = 1;
	const int BLUR_DURATION_MAX = 5;
	bool m_EffectEnabled;
	float m_Value;
	float m_BlurDuration;
	float m_BlurStrength;
	bool m_ReachedMid;
	bool m_ReachedEnd;

	//this is just for the state parameters set-up and is called even if the state doesn't execute, don't put any gameplay code in here
	override void OnInit()
	{
		m_StateType = StateTypes.SECONDARY;
		m_Priority = 0;
		m_ID = StateIDs.STATE_FEVERBLUR;
		m_DestroyOnAnimFinish = true;
		m_IsPersistent = false;
	}
	
	//!gets called every frame
	override void OnUpdateServer(PlayerBase player, float deltatime)
	{
	}

	override void OnUpdateClient(PlayerBase player, float deltatime)
	{

		if( Math.RandomFloat01() < 0.01 && !m_EffectEnabled )
		{
			m_EffectEnabled = true;
			m_BlurDuration = Math.RandomFloat(BLUR_DURATION_MIN, BLUR_DURATION_MAX);
			m_BlurStrength = Math.RandomFloat(0, 1);
			m_Value = 0;
		}
		
		if( m_EffectEnabled )
		{
			if(!m_ReachedMid)
			{
				m_Value += deltatime;
			}
			else
			{
				m_Value -= deltatime;
			}
			
			if(m_Value >= m_BlurDuration)
			{
				m_ReachedMid = true;
			}
			
			if( m_ReachedMid && m_Value<= 0)
			{
				m_EffectEnabled = false;
				m_ReachedMid = false;	
			}
			float value = m_Value / m_BlurDuration * m_BlurStrength;
			
			PPEffects.SetBlurDrunk(value);
		}
	
		
		
	}
	
	//!gets called once on an state which is being activated
	override void OnGetActivatedServer(PlayerBase player)
	{

	}

	override void OnGetActivatedClient(PlayerBase player)
	{

	}

	override void OnGetDeactivatedServer(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated CoughState called", "PlayerState");
	}
	
	//!only gets called once on an active state that is being deactivated
	override void OnGetDeactivatedClient(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated CoughState called", "PlayerState");
	}
}

/* 
DayZPlayerImplement

this file is implemenation of dayzPlayer in script 
- logic of movement 
- camera switching logic

*/

class DayZPlayerImplementAiming
{

	//-------------------------------------------------------------
	//!
	//! This HeadingModel - Clamps heading
	//! 


	protected const float SWAY_WEIGHT_SCALER = 0.2;//use this to scale the sway effect up/down
	protected float CONST_SPEED = 2;
	protected float m_HorizontalNoise;
	protected float m_HorizontalTargetValue;
	protected float m_HorizontalNoiseVelocity[1] = {0};
	protected DayZPlayerImplement	m_PlayerDpi;
	protected PlayerBase m_PlayerPb;
	protected float m_TotalTime;
	protected bool	m_AimNoiseAllowed = true;	
	protected bool	m_ProceduralRecoilEnabled = true;	
	protected ref RecoilBase m_CurrentRecoil;
	
	protected float m_SwayWeight;
	protected float m_MaxVelocity;

	protected static float	m_AimXClampRanges[] = { -180, -20, 90, 	0, -50, 90,  180, -20, 90 };

	
	void DayZPlayerImplementAiming(DayZPlayerImplement player)
	{
		m_PlayerDpi = player;
		Class.CastTo(m_PlayerPb, player);
	}

	void SetRecoil( Weapon_Base weapon )
	{
		if( m_ProceduralRecoilEnabled )
		{
			m_CurrentRecoil = weapon.SpawnRecoilObject();
		}
	}
	
	float GetSwayWeight()
	{
		return m_SwayWeight;		
	}
	
	void SetAimNoiseAllowed(bool state)
	{
		m_AimNoiseAllowed = state;	
	}

	void SetProceduralRecoilEnabled(bool state)
	{
		m_ProceduralRecoilEnabled = state;
	}

	bool ProcessAimFilters(float pDt, SDayZPlayerAimingModel pModel, int stance_index)
	{
		PlayerBase player = PlayerBase.Cast( m_PlayerDpi );
		float player_stamina = player.GetStaminaHandler().GetStaminaNormalized();
		
		m_SwayWeight = CalculateAimingNoiseWeight(	stance_index, player_stamina );
		
		float breathing_offset_x;
		float breathing_offset_y;
		
		float noise_offset_x;
		float noise_offset_y;
		
		float recoil_offset_mouse_x;
		float recoil_offset_mouse_y;
		
		float recoil_offset_hands_x;
		float recoil_offset_hands_y;
		
		float speed = ((1.0 - player_stamina) * 4.0) + 1.0;
		if( m_PlayerPb.IsHoldingBreath() )
		{
			speed = 0.1;
		}
		
		m_TotalTime += pDt * speed;
		
		//! get sway
		ApplyBreathingPattern(breathing_offset_x, breathing_offset_y, 3.0, m_TotalTime, m_SwayWeight);
		ApplyHorizontalNoise(noise_offset_x, noise_offset_y, 0.2, 0.5, 3.0, speed, 3 * m_SwayWeight, pDt);

		//! get recoil
		if( m_CurrentRecoil )
		{
			m_CurrentRecoil.Update(recoil_offset_mouse_x, recoil_offset_mouse_y, recoil_offset_hands_x, recoil_offset_hands_y, pDt);
		}
		
		//! cam offset
		pModel.m_fAimXCamOffset = breathing_offset_x + noise_offset_x ;
		pModel.m_fAimYCamOffset	= breathing_offset_y + noise_offset_y;

		
		//! hands offset
		pModel.m_fAimXHandsOffset = breathing_offset_x + noise_offset_x + recoil_offset_hands_x;
		pModel.m_fAimYHandsOffset = breathing_offset_y + noise_offset_y + recoil_offset_hands_y;

		//! clamp aim ranges 
		if (stance_index == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
		{			
			float newVal = DayZPlayerUtils.LinearRangeClamp(pModel.m_fCurrentAimX, pModel.m_fCurrentAimY, m_AimXClampRanges);
			pModel.m_fAimYHandsOffset += newVal - pModel.m_fCurrentAimY;
		}
		
		//! mouse offset
		pModel.m_fAimXMouseShift = recoil_offset_mouse_x;
		pModel.m_fAimYMouseShift = recoil_offset_mouse_y;
		
		return true;
	}

	protected void ApplyBreathingPattern(out float x_axis, out float y_axis, float pAmplitude, out float pTotalTime, float weight)
	{
		x_axis = (Math.Sin(pTotalTime) * pAmplitude / 4) * weight;
		y_axis = (Math.Sin(pTotalTime * 0.8 + 0.6 ) * pAmplitude) * weight;
	}

	protected void ApplyHorizontalNoise(out float x_axis, out float y_axis, float smooth_time,float max_velocity_low, float max_velocity_high, float velocity_modifier,  float weight, float pDt)
	{
		if( Math.AbsFloat(m_HorizontalTargetValue - m_HorizontalNoise) < 0.01)
		{
			//acquire new target
			m_MaxVelocity = Math.RandomFloatInclusive(max_velocity_low, max_velocity_high);
			m_HorizontalTargetValue = Math.RandomFloat01() * 2;
			m_HorizontalNoiseVelocity[0] = 0;
		}
		/*
		PrintString("current="+ m_HorizontalNoise.ToString());
		PrintString("target="+ m_HorizontalTargetValue.ToString());
		*/
		m_HorizontalNoise = Math.SmoothCD( m_HorizontalNoise, m_HorizontalTargetValue, m_HorizontalNoiseVelocity, smooth_time, m_MaxVelocity * velocity_modifier, pDt);
		x_axis = (m_HorizontalNoise - 1) * weight;
	}

	protected float CalculateAimingNoiseWeight(int stance_index, float current_stamina)
	{
		float stance_modifier;
		switch( stance_index )
		{
			case DayZPlayerConstants.STANCEIDX_RAISEDERECT:
				stance_modifier = 0.5;
			break;
			case DayZPlayerConstants.STANCEIDX_RAISEDCROUCH:
				stance_modifier = 0.75;
			break;
			case DayZPlayerConstants.STANCEIDX_RAISEDPRONE:
				stance_modifier = 0.9;
			break;
			default:
				stance_modifier = 0.75;
				//Debug.LogError("stance mask out of definition");
			break;
		}

		//is_holding_breath = !is_holding_breath;
		//return (1 - current_stamina * stance_modifier) * m_AimNoiseAllowed;
		return (1 - stance_modifier) * m_AimNoiseAllowed * SWAY_WEIGHT_SCALER;
	}
}


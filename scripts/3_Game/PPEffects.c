class PPEffects
{
	//-------------------------------------------------------
	// BLUR START
	//-------------------------------------------------------
	static float 	m_BurlapBlindness[5];
	static float 	m_HitColor[5];
	static float 	m_BlurInventory;
	static float 	m_BlurDrunk;
	static float	m_BlurFever;
	static float 	m_BlurMenu;
	static float	m_BloodSaturation;
	
	static float 	color[4];
	
	//-------------------------------------------------------
	//! sets blur effect to an absolute value between 0..1
	static void SetBlur(float value)
	{
		Material mat_blur = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/gauss");
		
		if (mat_blur)
		{
			mat_blur.SetParam("Intensity", value);
		}
	}
	//-------------------------------------------------------
	//! updates the blur post process effect where the resulting blur is an aggregate of all individual blur effect values
	static void UpdateBlur()
	{
		SetBlur( m_BlurInventory + m_BlurDrunk + m_BlurFever + m_BlurMenu);//add here new blur effects
	}
	//-------------------------------------------------------
	//! Set blur inventory effect to a specified 'value' between 0..1
	static void SetBlurInventory(float value)
	{
		m_BlurInventory = value;
		UpdateBlur();
	}
	//-------------------------------------------------------
	//! Set blur drunk effect to a specified 'value' between 0..1
	static void SetBlurDrunk(float value)
	{
		m_BlurDrunk = value;
		UpdateBlur();
	}

	//-------------------------------------------------------
	//! Set blur drunk effect to a specified 'value' between 0..1
	static void SetBlurFever(float value)
	{
		m_BlurFever = value;
		UpdateBlur();
	}
	
	//-------------------------------------------------------
	//! Set menu blur to a specified 'value' between 0..1
	static void SetBlurMenu(float value)
	{
		m_BlurMenu = value;
		UpdateBlur();
	}
	
	//-------------------------------------------------------
	// BLUR END
	//-------------------------------------------------------
	/*
	static void SetOverlayColor(float r, float g, float b, float a)
	{
		Material matColors = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/colors");
		
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = a;
		
		matColors.SetParam("OverlayColor", color);
		matColors.SetParam("OverlayFactor", a);

	}*/
	
	static void UpdateColor()
	{
		Material matColors = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/colors");
		
		color[0] = m_BurlapBlindness[0] + m_HitColor[0]/*+add additional modifiers here*/;
		color[1] = m_BurlapBlindness[1] + m_HitColor[1]/*+add additional modifiers here*/;
		color[2] = m_BurlapBlindness[2] + m_HitColor[2]/*+add additional modifiers here*/;
		color[3] = m_BurlapBlindness[3] + m_HitColor[3]/*+add additional modifiers here*/;

		matColors.SetParam("OverlayColor", color);
		float overlay_factor = m_BurlapBlindness[4] + m_HitColor[4];
		matColors.SetParam("OverlayFactor", overlay_factor);
	}

	static void HitEffect(float value)
	{
		m_HitColor[0] = 1;
		m_HitColor[1] = 0;
		m_HitColor[2] = 0;
		m_HitColor[3] = 1;
		m_HitColor[4] = value;
		UpdateColor();
	}
	
	static void EnableBurlapSackBlindness()
	{
		m_BurlapBlindness[0] = 0;
		m_BurlapBlindness[1] = 0;
		m_BurlapBlindness[2] = 0;
		m_BurlapBlindness[3] = 1;
		m_BurlapBlindness[4] = 1.0;
		UpdateColor();
	}

	static void DisableBurlapSackBlindness()
	{
		m_BurlapBlindness[0] = 0;
		m_BurlapBlindness[1] = 0;
		m_BurlapBlindness[2] = 0;
		m_BurlapBlindness[3] = 0;
		m_BurlapBlindness[4] = 0.0;
		UpdateColor();
	}

	static void UpdateSaturation()
	{
		Material matColors = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/colors");
		matColors.SetParam("Saturation", m_BloodSaturation/*+add_additional_modifiers_here*/);
	}

	static void SetBloodSaturation(float value)
	{
		m_BloodSaturation = value;
		UpdateSaturation();
	}
	
	static void ResetAll()
	{
		DisableBurlapSackBlindness();
		SetBlurInventory(0);
		SetBlurDrunk(0);
		SetBlurFever(0);
		SetBloodSaturation(1);
	}
	
	
};
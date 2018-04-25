class World: Managed
{
	//proto private void ~World();
	//proto private void World();

	proto native void	GetPlayerList(out array<Man> players);
	
	/**
	\brief Get actual ingame world time
		\param year
		\param month in range <1, 12>
		\param day in range <1, 31>
		\param hour in range <0, 23>
		\param minute in range <0, 59>
		@code
			int year, month, day, hour, minute;
			GetGame().GetWorld().GetDate(year, month, day, hour, minute);
		@endcode
	*/
	proto void			GetDate(out int year, out int month, out int day, out int hour, out int minute);
	
	/**
	\brief Sets actual ingame world time
		\param year
		\param month in range <1, 12>
		\param day in range <1, 31>
		\param hour in range <0, 23>
		\param minute in range <0, 59>
		@code
			int year = 2016;
			int month = 5;
			int day = 4;
			int hour = 14;
			int minute = 57;
			GetGame().GetWorld().SetDate(year, month, day, hour, minute);
		@endcode
	*/
	proto native void	SetDate(int year, int month, int day, int hour, int minute);
	proto native float	GetLatitude();
	proto native float	GetLongitude();
	proto native float	GetMoonIntensity();
	proto native float	GetSunOrMoon();
		
	proto native float	GetEyeAccom();
	proto native void	SetEyeAccom(float eyeAccom);

	proto native void	StartFpsBenchmark(int sectorsCount, float preloadDistance, float fpsLimitRed, float fpsLimitGreen, float fpsLimitBlue, float minX, float minY, float maxX, float maxY);
	proto native void	StartFpsBenchmarkWithMask(float preloadDistance, float fpsLimitRed, float fpsLimitGreen, float fpsLimitBlue);
	proto native void	StartFpsBenchmarkViews(string xmlFileName, float preloadDistance);
	proto native void	SetBuldozerWaterEnabled(bool enable);
	/**
	\brief Sets preferred view distance, which persists between game sessions
		\param distance
	*/
	proto native void SetPreferredViewDistance(float distance);

	proto native void SetViewDistance(float distance);

	proto native void SetObjectViewDistance(float distance);

	proto native void	LoadNewLightingCfg(string path);
	
	/*!
	@code
	Material matColors = GetGame().GetWorld().GetMaterial("postprocess/colors");
	SetMaterialParam(matColors, "Saturation", 0.8);
	float color[4];
	color[0] = 0.5;
	color[1] = 0.8;
	color[2] = 0.7;
	color[3] = 0.6;
	SetMaterialParam(matColors, "OverlayColor", color);
	@endcode
	*/
	proto native Material GetMaterial(string materialName);

	/*!
	set new PP effect and its material
	\param camera					number of camera
	\param effectIndex		index of effect
	\param effectName			name of effect
	\param matName				name of material
	*/
	proto native void		SetCameraPostProcessEffect(int cam, int ppEffect, string effectName, string materialName);
	
	void SetAperture(float invDiameter)
	{
		if (invDiameter <= 0)
		{
			SetEyeAccom(-1);
		}
		else
		{
			float apertureArea = 1.0 / invDiameter;
			SetEyeAccom(apertureArea * apertureArea);
		}
	}
	
	/*!
	checks if 3rd person camera is disabled from server
	*/
	proto native bool Is3rdPersonDisabled();
	
	/*!
	checks if crosshair is disabled from server
	*/
	proto native bool IsCrosshairDisabled();
	
	/*!
	enables/disables player's voice communication
	\param on					enable or disable voice
	*/
	proto native void SetVoiceOn(bool on);
};

class PluginPlayerStatus extends PluginBase
{
	ref multiMap<int, string> 	m_NotifiersLabel;
	ref multiMap<int, int>		m_NotifiersIndexColor;

	private ref multiMap<int, string>	m_NotifiersIcons;

	void PluginPlayerStatus()
	{
		m_NotifiersLabel = new multiMap<int, string>; // [key] label
		m_NotifiersIndexColor = new multiMap<int, int>; // [key] index, color

		m_NotifiersIcons = new multiMap<int, string>; // [key] iconName
		m_NotifiersIcons.Insert( NTFKEY_HUNGRY, "iconHunger" );
		m_NotifiersIcons.Insert( NTFKEY_THIRSTY, "iconThirsty" );
		m_NotifiersIcons.Insert( NTFKEY_SICK, "iconHealth" );
		m_NotifiersIcons.Insert( NTFKEY_BACTERIA, "iconBacteria" );
		m_NotifiersIcons.Insert( NTFKEY_BLEEDISH, "iconBlood" );
		m_NotifiersIcons.Insert( NTFKEY_FEVERISH, "iconTemperature" );
	}
	
	void SetNotifier( int key, int index = 9, string label = "", int color =  0xFFFFFFFF )
	{
		if ( key )
		{
			if ( m_NotifiersLabel.HasKey( key ) )
			{
				m_NotifiersLabel.Remove( key );
				m_NotifiersIndexColor.Remove( key );
			}

			if ( label != "" ) 
			{
				m_NotifiersLabel.Insert( key, label );
				m_NotifiersIndexColor.Insert( key, index );
				m_NotifiersIndexColor.Insert( key, color );
			}
		}
	}

	void DisplayTendency( int key, int tendency, int status = 1 )
	{
		if ( key )
		{
			// display icon
			int icon_index = 0;  // maybe we'll have several icons for different tendencies
			if ( m_NotifiersIcons.HasKey( key ) )
			{
				string icon_name = m_NotifiersIcons.Get( key ).Get( icon_index );
				Mission mission = GetGame().GetMission();
				if ( mission )
				{
					Hud hud = mission.GetHud();
					if ( hud )
					{
						hud.DisplayNotifier( key, tendency, status );
					}
				}
			}
		}
	}

	void SetBadge( int key, bool show )
	{
		if ( key )
		{
			Mission mission = GetGame().GetMission();
			if ( mission )
			{
				Hud hud = mission.GetHud();
				if ( hud )
				{
					hud.DisplayBadge( key, show );
				}
			}
		}
	}

	void SetStamina( int value , int range )
	{
		// Log( String( "SetStamina" + itoa( value) ), LogTemplates.TEMPLATE_JANOSIK );
		Mission mission = GetGame().GetMission();
		if ( mission )
		{
			Hud hud = mission.GetHud();
			if ( hud )
			{
				hud.SetStamina( Math.Clamp( value, 0, range ), range );
			}
		}
	}
	
	void SetStance( int value  )
	{
		Mission mission = GetGame().GetMission();
		if ( mission )
		{
			Hud hud = mission.GetHud();
			if ( hud )
			{
				hud.DisplayStance( value );
			}
		}
	}
}

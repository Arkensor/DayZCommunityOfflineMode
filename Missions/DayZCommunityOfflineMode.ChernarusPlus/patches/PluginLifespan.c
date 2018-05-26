class PluginLifespanPatched : PluginLifespan
{
	override protected void UpdateLifespanLevel( PlayerBase player, float player_beard, bool force_update = false )
	{
	if ( m_PlayerCurrentLevel.Contains(player) )
	{
		LifespanLevel current_level = m_PlayerCurrentLevel.Get( player );

		if ( player_beard > current_level.GetThreshold() || force_update )
		{
			LifespanLevel next_level = GetLifespanLevel( player.GetPlayerClass(), player_beard );

			if ( next_level != NULL )
			{
				SetPlayerLifespanLevel( player, next_level );
				m_PlayerCurrentLevel.Set( player, next_level );
			}
		}
	}
	else
	{
		if ( m_LifespanLevels.Contains( player.GetPlayerClass() ) )
		{
			LifespanLevel level = GetLifespanLevel( player.GetPlayerClass(), player_beard );

			if ( level != NULL )
			{
				SetPlayerLifespanLevel( player, level );
				m_PlayerCurrentLevel.Set( player, level );
			}
		}
	}
	}
}
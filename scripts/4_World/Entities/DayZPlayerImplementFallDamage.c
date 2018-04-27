class DayZPlayerImplementFallDamage
{
	const int 	 	DT_FALL					= 3;			//!< DT_CUSTOM (Damage Type)

	const string 	FD_AMMO					= "FallDamage";	//!< ammo type used for damaging
	const float		FD_DMG_FROM_HEIGHT		= 2.5;			//!< damage will not be taken into account bellow this HeightToDamage
	const float		FD_MAX_DMG_AT_HEIGHT	= 15;			//!< height where player gets 100% damage
	
	DayZPlayer 		m_Player;

	void DayZPlayerImplementFallDamage(DayZPlayer pPlayer)
	{
		m_Player 	= pPlayer;
	}

	//! damage coef calculated from height
	float DamageCoef(float pHeight)
	{	
		if( pHeight >= 10 )
			return 0;

		return (pHeight - FD_DMG_FROM_HEIGHT);
	}

	//! handle fall damage
	void HandleFallDamage(float pHeight)
	{
		if (GetGame().IsServer())
		{
			//! no dmg height
			if (pHeight <= FD_DMG_FROM_HEIGHT)
				return;

			vector posMS = m_Player.WorldToModel(m_Player.GetPosition());

			//! global dmg multiplied byt damage coef
			m_Player.ProcessDirectDamage( DT_FALL, m_Player, "", FD_AMMO, posMS, DamageCoef(pHeight) );

			//! updates injury state of player immediately
			PlayerBase pb = PlayerBase.Cast(m_Player);
			if (pb) pb.ForceUpdateInjuredState();
		}
	}
};
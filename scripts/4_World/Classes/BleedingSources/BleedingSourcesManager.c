class BleedingSourcesManager
{
	ref map<string, ref BleedingSource> m_BleedingSources = new map<string, ref BleedingSource>;
	PlayerBase m_Player;
	
	void BleedingSourcesManager( PlayerBase player )
	{
		m_Player = player;
	}
	
	void AddBleedingSource(string zone, vector position)
	{
		if( !m_BleedingSources.Contains(zone) )
		{
			m_BleedingSources.Insert(zone, new BleedingSource(zone, m_Player, position) );
		}
	}

	void RemoveSingleBleedingSource()
	{
		if( m_BleedingSources.Count() > 0 )
		{
			m_BleedingSources.RemoveElement(0);		
		}
	}
	
	//damage must be to "Blood" healthType
	void ProcessHit(float damage, string component, string ammo, vector modelPos)
	{
		if( m_BleedingSources.Contains(component) )
		{
			return;
		}
		float dmg_max = m_Player.GetMaxHealth(component, "Blood");
		float dmg = damage;
		float bleed_threshold = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " DamageApplied " + "bleedThreshold" );
		//Print("dmg_max = " + dmg_max);
		//Print("dmg = " + dmg);
		//Print("bleed_threshold = " + bleed_threshold);
		if ( dmg > (dmg_max * (1 - bleed_threshold)) )
		{
			AddBleedingSource(component ,modelPos );
			//Print("BLEEDING");
		}
	}

	void OnStoreSave( ParamsWriteContext ctx )
	{
		int count = m_BleedingSources.Count();
		ctx.Write(count);
		
		for(int i = 0; i < m_BleedingSources.Count(); i++ )
		{
			BleedingSource bs = m_BleedingSources.GetElement(i);
			string zone 		= m_BleedingSources.GetKey(i);//key
			vector position 	= bs.GetPosition();
			ctx.Write(zone);
			ctx.Write(position);
		}
	}

	void OnStoreLoad( ParamsReadContext ctx )
	{
		int count;
		ctx.Read(count);
		
		for(int i = 0; i < count; i++ )
		{
			string zone; 		
			vector position;
			ctx.Read(zone);
			ctx.Read(position);
			
			AddBleedingSource(zone,position);
		}
	}
	
}
class StaroyeInfectedRespawner
{
	static ref array<EntityAI> m_SpawnedInfected = new array<EntityAI>;
	static ref array<vector> m_SpawnPositions = new array<vector>;
	static ref array<string> m_SpawnInfected = new array<string>;
	static bool m_IsInitialized;
	
	static void Init()
	{
		m_IsInitialized = true;
		
		m_SpawnPositions.Insert("939.833252 248.302444 852.479431");
		m_SpawnPositions.Insert("917.619812 255.813217 898.093933");
		m_SpawnPositions.Insert("967.591980 247.275818 916.682007");
		m_SpawnPositions.Insert("998.857056 246.498718 910.204651");
		m_SpawnPositions.Insert("1020.01367 245.313492 895.067627");
		m_SpawnPositions.Insert("1049.06164 243.881287 868.121155");
		m_SpawnPositions.Insert("1051.51647 244.137573 918.634399");
		m_SpawnPositions.Insert("1105.62744 232.115341 828.539063");
		m_SpawnPositions.Insert("1069.66833 237.762451 792.917847");
		m_SpawnPositions.Insert("999.793091 245.005981 805.171936");
		m_SpawnPositions.Insert("859.444031 253.966873 850.466309");
		m_SpawnPositions.Insert("831.932922 254.016174 814.170349");
		m_SpawnPositions.Insert("788.829102 254.009506 840.422302");
		m_SpawnPositions.Insert("766.784973 255.512833 760.250244");
		m_SpawnPositions.Insert("841.927551 260.156647 903.821472");
		m_SpawnPositions.Insert("950.477661 257.821930 1014.39270");
		m_SpawnPositions.Insert("890.763916 262.100861 1052.74157");
		m_SpawnPositions.Insert("412.224609 315.271271 1321.63537");
		m_SpawnPositions.Insert("381.741943 317.263214 1361.53894");
		m_SpawnPositions.Insert("1062.76916 244.162430 952.003845");
		m_SpawnPositions.Insert("1156.84240 256.482086 1099.88952");
		m_SpawnPositions.Insert("1335.11462 278.066956 1219.53918");
		m_SpawnPositions.Insert("1410.83166 274.394287 1281.89856");
		m_SpawnPositions.Insert("1314.77197 283.048248 1316.33718");
		m_SpawnPositions.Insert("1157.46044 245.031464 970.915527");
		m_SpawnPositions.Insert("1195.73230 244.914017 925.952515");
		m_SpawnPositions.Insert("1254.82490 248.692154 861.445679");
		m_SpawnPositions.Insert("1028.38085 229.903076 569.031677");
		m_SpawnPositions.Insert("1142.58520 238.130112 425.294952");
		m_SpawnPositions.Insert("1028.70459 243.872177 752.999573");
		m_SpawnPositions.Insert("988.919189 245.012436 804.805355");
		
		
		m_SpawnInfected.Insert("ZmbM_FarmerFat_Beige");
		m_SpawnInfected.Insert("ZmbM_FarmerFat_Blue");
		m_SpawnInfected.Insert("ZmbM_FarmerFat_Brown");
		m_SpawnInfected.Insert("ZmbM_FarmerFat_Green");
		m_SpawnInfected.Insert("ZmbM_VillagerOld_Blue");
		m_SpawnInfected.Insert("ZmbM_VillagerOld_Green");
		m_SpawnInfected.Insert("ZmbM_VillagerOld_White");
		m_SpawnInfected.Insert("ZmbF_HikerSkinny_Blue");
		m_SpawnInfected.Insert("ZmbF_HikerSkinny_Grey");
		m_SpawnInfected.Insert("ZmbF_HikerSkinny_Green");
		m_SpawnInfected.Insert("ZmbF_HikerSkinny_Red");
		m_SpawnInfected.Insert("ZmbM_SurvivorDean_Black");
		m_SpawnInfected.Insert("ZmbM_SurvivorDean_Blue");
		m_SpawnInfected.Insert("ZmbM_SurvivorDean_Grey");
		m_SpawnInfected.Insert("ZmbM_PatrolNormal_Flat");
		m_SpawnInfected.Insert("ZmbM_PatrolNormal_Autumn");
		m_SpawnInfected.Insert("ZmbM_FishermanOld_Grey");
		m_SpawnInfected.Insert("ZmbM_FishermanOld_Blue");
		m_SpawnInfected.Insert("ZmbM_FishermanOld_Red");		
	}

	static void Event_OnInfectedSpawn(EntityAI infected)
	{
		if ( m_IsInitialized == false )
		{
			Init();
		}
		
		m_SpawnedInfected.Insert(infected);
	}
	
	static void Event_OnInfectedDespawn(EntityAI infected)
	{
		if ( m_IsInitialized == false )
		{
			Init();
		}
		
		for ( int i = 0; i < m_SpawnedInfected.Count(); ++i )
		{
			if ( m_SpawnedInfected[i].GetID() == infected.GetID() )
			{
				m_SpawnedInfected.Remove(i);
				break;
			}
		}
	}
	
	static void InfectedRespawn()
	{
		if ( m_IsInitialized == false )
		{
			Init();
		}
		
		int i;
		vector spawn_pos;
		Man player = GetGame().GetPlayer();
		vector player_pos = player.GetPosition();
		float dist;
				
		array<int> remove_infected = new array<int>();
		for ( i = 0; i < m_SpawnedInfected.Count(); ++i )
		{
			EntityAI infected = m_SpawnedInfected[i];
			
			dist = vector.Distance(player_pos, infected.GetPosition());
			
			if ( dist > 300 )
			{
				remove_infected.Insert(infected.GetID());
				GetGame().ObjectDelete( infected );
Print( "Zombie despawn" );
			}
		}
		
		for ( i = 0; i < remove_infected.Count(); ++i )
		{
			for ( int j = 0; j < m_SpawnedInfected.Count(); ++j )
			{
				if ( m_SpawnedInfected[j].GetID() == remove_infected[i] )
				{
					m_SpawnedInfected.Remove(j);
					break;
				}
			}
		}
				
		int spawn_count = 8 - m_SpawnedInfected.Count();
		
		array<vector> valid_positions = new array<vector>();
		
		for ( i = 0; i < m_SpawnPositions.Count(); ++i )
		{
			spawn_pos = m_SpawnPositions[i];
			
			dist = vector.Distance(player_pos, spawn_pos);
			
			if ( dist > 100 && dist < 300 )
			{
				bool can_infected_spawn = true;
				for ( int k = 0; k < m_SpawnedInfected.Count(); ++k )
				{
					float inf_dist = vector.Distance(spawn_pos, m_SpawnedInfected[k].GetPosition());
					
					if ( inf_dist < 20 )
					{
						can_infected_spawn = false;
						break;
					}
				}
				
				if ( can_infected_spawn )
				{
					valid_positions.Insert(spawn_pos);
				}
			}
		}
		
		for ( i = 0; i < spawn_count; ++i )
		{
			if ( valid_positions.Count() > 0 )
			{
				spawn_pos = valid_positions.GetRandomElement();
				valid_positions.RemoveItem(spawn_pos);
Print("Zombie spawn");
				GetGame().CreateObject( GetInfectedRandomName(), spawn_pos, false, true, true );
			}
		}
	}
	
	static string GetInfectedRandomName()
	{
		return m_SpawnInfected.GetRandomElement();
	}
};

class PluginHorticulture extends PluginBase
{
	ref map<string, ref PlantMaterialHealth> m_PlantMaterials;
	
	void PluginHorticulture()
	{
		m_PlantMaterials = new map<string, ref PlantMaterialHealth>;
		
		LoadFromCfg();
	}

	void LoadFromCfg()
	{
		string cfg_access = "CfgHorticulture"; // it is stored in dz\modulesDayz\cfgHorticulture.cpp
		int cfg_horticulture_count = GetGame().ConfigGetChildrenCount( cfg_access );

		for ( int i = 0; i < cfg_horticulture_count; i++ )
		{
			string cfg_class_name = "";
			GetGame().ConfigGetChildName( cfg_access, i, cfg_class_name );
			string cfg_class_access = cfg_access + " " + cfg_class_name;

			int cfg_class_count = GetGame().ConfigGetChildrenCount( cfg_class_access );
		
			for ( int j = 0; j < cfg_class_count; j++ )
			{
				string cfg_subclass_name = "";
				GetGame().ConfigGetChildName( cfg_class_access, j, cfg_subclass_name );
				string cfg_subclass_access = cfg_class_access + " " + cfg_subclass_name;
		
				int cfg_subclass_count = GetGame().ConfigGetChildrenCount( cfg_subclass_access );
				
				PlantMaterialHealth plantMaterialHealth = NULL;
				
				if ( cfg_class_name == "Plants" )
				{
					plantMaterialHealth = new PlantMaterialHealth;
					m_PlantMaterials.Set( cfg_subclass_name, plantMaterialHealth );
				}
				
				for ( int k = 0; k < cfg_subclass_count; k++ )
				{
					string cfg_variable_name = "";
					GetGame().ConfigGetChildName( cfg_subclass_access, k, cfg_variable_name );
					string cfg_variable_access = cfg_subclass_access + " " + cfg_variable_name;
					
					if ( cfg_class_name == "Plants" )
					{
						string string_param = "";
						GetGame().ConfigGetText( cfg_variable_access, string_param );
						
						if ( cfg_variable_name == "infestedTex" )
						{
							plantMaterialHealth.m_InfestedTex = string_param;
						}
						else if ( cfg_variable_name == "infestedMat" )
						{
							plantMaterialHealth.m_InfestedMat = string_param;
						}
						else if ( cfg_variable_name == "healthyTex" )
						{
							plantMaterialHealth.m_HealthyTex = string_param;
						}
						else if ( cfg_variable_name == "healthyMat" )
						{
							plantMaterialHealth.m_HealthyMat = string_param;
						}
					}
				}
			}
		}
	}
	
	string GetPlantType( Object obj )
	{
		string seed_type = obj.GetType();
		
		string plant_type = "";
		GetGame().ConfigGetText( "cfgVehicles " + seed_type + " Horticulture PlantType", plant_type );
		
		return plant_type;
	}

	float GetSurfaceFertility( string surface_type )
	{
		float fertility = 0.8;
		if ( surface_type == "hlina" || surface_type == "CRGrass1" || surface_type == "CRGrass2" || surface_type == "CRForest1" || surface_type == "CRForest2" || surface_type == "CRGrit1" )
		{
			fertility = 0.8;
		}
		
		return fertility;
	}

	bool GiveWormsToPlayer( PlayerBase player, float chance )
	{
		if ( Math.RandomFloat01() <= chance )
		{
			ItemBase item = ItemBase.Cast( player.GetHumanInventory().CreateInInventory("Food_Worm") );
			item.SetQuantity( 1 );
			return true;
		}
		
		return false;
	}

	PlantMaterialHealth GetPlantMaterial( string plant_type )
	{
		if ( m_PlantMaterials.Contains(plant_type) )
		{
			return m_PlantMaterials.Get(plant_type);
		}
		
		return NULL;
	}
}

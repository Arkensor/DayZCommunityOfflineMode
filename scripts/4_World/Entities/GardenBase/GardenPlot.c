class GardenPlot_TO_BE_RELEASED extends GardenBase // garden plots are temporarily disabled for the first 063 exps.
{
	ClutterCutter6x6 	m_clutter_cutter;
	const string COMPATIBLE_SURFACES[] = {"cp_dirt", "cp_broadleaf_dense1", "cp_broadleaf_dense2", "cp_broadleaf_sparse1", "cp_broadleaf_sparse2", "cp_conifer_common1", "cp_conifer_common2", "cp_conifer_moss1", "cp_conifer_moss2", "cp_grass", "cp_grass_tall", "cp_gravel", "cp_rock", "textile_carpet_int"};
	static const int COMPATIBLE_SURFACES_COUNT = 14; // Count if elements of COMPATIBLE_SURFACES array
	
	void GardenPlot()
	{
		FIRST_SLOT_COMPONENT_INDEX = 2;
		m_BaseFertility = 0.8;
		InitializeSlots();
		DigAllSlots(); // TO DO: Slots should be digged by default, so remove this function when that change is made.
	}
	
	override void EEInit()
	{
		super.EEInit();
		
		if( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{
			if (!m_clutter_cutter)
			{
				vector pos = GetPosition();
				Print(pos);
				m_clutter_cutter = ClutterCutter6x6.Cast( GetGame().CreateObject( "ClutterCutter6x6", GetPosition(), false ) );
			}			
		}	
	}	
	
	void ~GardenPlot_TO_BE_RELEASED()
	{
		if (m_clutter_cutter  &&  GetGame())
		{
			GetGame().ObjectDelete(m_clutter_cutter);
			m_clutter_cutter = NULL;
		}
	}

	override int GetGardenSlotsCount()
	{
		return 9;
	}

	override void OnPlacementStarted( Man player )
	{
		RefreshSlots();
	}
	
	override void OnHologramBeingPlaced( Man player )
	{
		RefreshSlots();
	}

	void RefreshSlots()
	{
		HideSelection("slotDigged_01");
		HideSelection("slotDigged_02");
		HideSelection("slotDigged_03");
		HideSelection("slotDigged_04");
		HideSelection("slotDigged_05");
		HideSelection("slotDigged_06");
		HideSelection("slotDigged_07");
		HideSelection("slotDigged_08");
		HideSelection("slotDigged_09");
	}
	
	override void OnPlacementComplete( Man player )
	{		
		// To properly move the clutter cutter from spawn position, it must be deleted and created again.
		if (m_clutter_cutter)
		{
			GetGame().ObjectDelete(m_clutter_cutter);
			m_clutter_cutter = NULL;
		}
		
		if (!m_clutter_cutter)
		{
			m_clutter_cutter = ClutterCutter6x6.Cast( GetGame().CreateObject( "ClutterCutter6x6", GetPosition(), false ) );
		}
	}
	
	override bool CanBePlaced( Man player, vector position )
	{
		string surface_type;
		GetGame().SurfaceGetType( position[0], position[2], surface_type );
		
		for (int i=0; i < COMPATIBLE_SURFACES_COUNT; i++)
		{
			if (COMPATIBLE_SURFACES[i] == surface_type)
			{
				return true;
			}
		}
		
		return false;
	}
}
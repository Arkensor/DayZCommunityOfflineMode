class AttchmentsOutOfReach
{
	protected static ref map<string, ref map<string, vector>> m_AttData;
	
	static bool IsAttachmentReachable(EntityAI e, string att_slot_name, float range = 1.5)
	{
		vector pos_att = e.GetPosition() + GetAttachmentPosition(e, att_slot_name);
		vector pos_player = GetGame().GetPlayer().GetPosition();
		
		if ( vector.Distance(pos_player, pos_att) <= range )
		{
			return true;
		}
		
		return false;
	}
	
	static vector GetAttachmentPosition(EntityAI e, string att_slot_name)
	{		
		if ( m_AttData == NULL )
		{
			m_AttData = new map<string, ref map<string, vector>>();
		}
		
		string type_name = e.GetType();
		
		if ( !m_AttData.Contains( type_name ) )
		{
			map<string, vector> att = CreateAttachmentsPositions(e);
			
			m_AttData.Insert(type_name, att);			
		}
		
		return m_AttData.Get( type_name ).Get( att_slot_name );
	}
	
	static protected map<string, vector> CreateAttachmentsPositions(EntityAI entity)
	{
		map<string, vector> ret_val = new map<string, vector>();
		
		string			type_name = entity.GetType();
		TStringArray	cfg_attachments = new TStringArray;
		
		string cfg_path;
		
		if ( GetGame().ConfigIsExisting(CFG_VEHICLESPATH+" "+type_name) )
		{
			cfg_path = CFG_VEHICLESPATH+" "+type_name+" attachments";
		}
		else if ( GetGame().ConfigIsExisting(CFG_WEAPONSPATH+" "+type_name) )
		{
			cfg_path = CFG_WEAPONSPATH+" "+type_name+" attachments";
		}
		else if ( GetGame().ConfigIsExisting(CFG_MAGAZINESPATH+" "+type_name) )
		{
			cfg_path = CFG_MAGAZINESPATH+" "+type_name+" attachments";
		}
		
		GetGame().ConfigGetTextArray(cfg_path, cfg_attachments);
		
		//cfg_attachments.Debug();
		
		int child_count = GetGame().ConfigGetChildrenCount("CfgNonAIVehicles");
		
		for ( int x = 0; x < child_count; ++x )
		{
			string child_name;
			GetGame().ConfigGetChildName("CfgNonAIVehicles",x ,	child_name);
			
			string inventory_slot_name;
			GetGame().ConfigGetText("CfgNonAIVehicles "+ child_name +" inventorySlot", inventory_slot_name);
			
			if ( cfg_attachments.Find( inventory_slot_name ) > 0 )
			{
				string model_path;
				GetGame().ConfigGetText("CfgNonAIVehicles "+ child_name +" model", model_path);
				
				if ( model_path.Length() > 5 )
				{
					model_path = model_path.Substring(0, model_path.Length() - 4);
					
					array<Selection> selections = new array<Selection>();
					LOD lod = entity.GetLODByName("geometryView");
					
					if( lod )
					{
						lod.GetSelections(selections);
						
						for ( int i = 0; i < selections.Count(); ++i )
						{
							model_path.ToLower();
							string selection = selections[i].GetName();
							selection.ToLower();
							
							if ( selection.Contains(model_path) )
							{
								//Print("Attachment Slot: "+ inventory_slot_name +" = "+ selections[i].GetName());
								//Print("Vertex Count: "+ selections[i].GetVertexCount());
								//Print("Position: "+ selections[i].GetVertexPosition(lod, 0) );
								
								ret_val.Set(inventory_slot_name, selections[i].GetVertexPosition(lod, 0));
							}
						}
					}
				}
			}
		}
		
		return ret_val;
	}
}
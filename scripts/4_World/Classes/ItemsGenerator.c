class ItemsGenerator //Makes sure there is always one small Items near player if he has none in inventory
{	
	private PlayerBase 			m_Player;
	private ref array<Man>		m_Players;
	private ref array<ItemBase> m_ItemsArray;
	private ref TStringArray 	m_ItemsToGenerate;
	private float				m_Time;
	private int					m_Spawned;
	
	void ItemsGenerator(PlayerBase player)
	{
		m_Player = player;
		m_Spawned = 0;
		m_ItemsArray = new array<ItemBase>;
		m_Players = new array<Man>;
		m_ItemsToGenerate = new TStringArray;
		m_ItemsToGenerate.Insert("SmallStone");
		//m_ItemsToGenerate.Insert("WoodenStick");
		//m_ItemsToGenerate.Insert("Apple");
	}
	
	void ~ItemsGenerator()
	{
		CleanUp();
	}
		
	void Update(float deltaT)
	{
		m_Time += deltaT;	
		if ( m_Time > ITEMSGEN_TICK )
		{
			m_Time = 0;
			if ( m_Player )
			{
				if ( m_ItemsArray && m_ItemsArray.Count() == ITEMSGEN_ITEMS_AMOUNT )
				{
					vector player_pos = m_Player.GetPosition();
					for ( int i = 0; i < m_ItemsArray.Count(); i++ )
					{
						ItemBase item = m_ItemsArray.Get(i);
						if ( item && !item.GetHierarchyParent() )
						{
							if ( vector.Distance(item.GetPosition(),player_pos) > ITEMSGEN_MIN_DISTANCE_TO_REPOSITION )
							{
								vector pos = GeneratePos(item);
								item.SetPosition(pos);
								item.PlaceOnSurface();
							}
						}
						else
						{
							m_ItemsArray.Remove(i);
						}
					}
				}
				else
				{
					CreateItems();
				}
			}
		}
	}	
	
	vector GeneratePos(ItemBase item)
	{
		if ( m_Player )
		{
			vector player_pos = m_Player.GetPosition();
			vector new_pos = player_pos;
			if ( item )
			{
				new_pos = item.GetPosition();
			}
			player_pos[0] = player_pos[0] + Math.RandomInt(-ITEMSGEN_MAX_SPREAD,ITEMSGEN_MAX_SPREAD);
			player_pos[2] = player_pos[2] + Math.RandomInt(-ITEMSGEN_MAX_SPREAD,ITEMSGEN_MAX_SPREAD);;
			vector player_dir = m_Player.GetDirection()*ITEMSGEN_SPAWN_DISTANCE;
			vector possible_pos = player_pos+player_dir;	
			if ( !GetGame().SurfaceIsSea(possible_pos[0], possible_pos[2]) && !GetGame().SurfaceIsPond(possible_pos[0], possible_pos[2]) ) 
			{
				ref array<Object> excluded_objects = new array<Object>;
				if ( item ) 
				{
					excluded_objects.Insert(item);
				}				
				if( !GetGame().IsBoxColliding(possible_pos, m_Player.GetOrientation(), "0.25 0.25 0.25", excluded_objects) )
				{
					bool can_translate = true;
					if ( m_Players )
					{
						GetGame().GetPlayers(m_Players);
						if ( m_Players.Count() > 1)
						{
							vector other_player_pos;
							for ( int i = 0; i < m_Players.Count(); i++ )
							{
								other_player_pos = m_Players.Get(i).GetPosition();
								if ( vector.Distance(new_pos,other_player_pos) < ITEMSGEN_SPAWN_DISTANCE || vector.Distance(possible_pos,other_player_pos) < ITEMSGEN_SPAWN_DISTANCE )
								{
									can_translate = false;
									break;
								}
							}
						}
					}
					if (can_translate)
					{ 
						new_pos = possible_pos;
					}
				}
			}
			//Debug 
			//m_Player.MessageAction("pos vec: " + ToString(player_pos) + " dir vec: " + ToString(m_Player.GetDirection()) + " dv mult: " + ToString(player_dir) + " fin vec: " + ToString(new_pos));
		}
		return new_pos;
	}
	
	void CreateItems()
	{
		if ( m_ItemsArray && m_Player )
		{
			vector pos = GeneratePos(NULL);
			ItemBase item = ItemBase.Cast(GetGame().CreateObject(m_ItemsToGenerate.Get(m_Spawned),pos, false));
			item.SetQuantity(1);
			item.PlaceOnSurface();
			m_ItemsArray.Insert(item);
			if ( m_Spawned+1 < m_ItemsToGenerate.Count() ) //so each item in items array is spawned
			{
				m_Spawned++;
			}
			else
			{
				m_Spawned = 0;
			}
		}
	}
	
	void CleanUp()
	{
		if ( GetGame() && m_ItemsArray )
		{
			for ( int i = 0; i < m_ItemsArray.Count(); i++ )
			{
				ItemBase item = m_ItemsArray.Get(i);
				if ( item && !item.GetHierarchyParent() )
				{
					GetGame().ObjectDelete(item);
				}
			}
		}
	}
};
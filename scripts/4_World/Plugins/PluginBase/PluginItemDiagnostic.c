class PluginItemDiagnostic extends PluginBase
{
	ref Timer 						myTimer1;
	ref map<PlayerBase,ItemBase> 	m_ObserversMap = new map<PlayerBase,ItemBase>;
	ref array<string> 				m_Agents = new array<string>;
	ref map<string,float> 			m_Floats = new map<string,float>;
	ref map<string,float> 			m_VarsNumbersFinalServer = new map<string,float>;
	ref map<string,float> 			m_VarsFinalClient = new map<string,float>;
	ref array<ref Param>		m_Properties = new array<ref Param>;
	bool 								m_IsActive	= false;
	string								m_NoteClient;
	
	Widget 				m_DebugRootWidget;
	TextListboxWidget 	m_DebugAgentListWidget;
	TextListboxWidget 	m_DebugFloatsListWidget;
	TextListboxWidget 	m_DebugStringsListWidget;
	TextListboxWidget 	m_DebugFloatsProperListWidget;
	TextListboxWidget 	m_DebugClientVarsWidget;
	TextWidget 			m_ClassNameWidget;
	ItemPreviewWidget 	m_ItemPreviewWidget;


	void PluginItemDiagnostic()
	{
		#ifndef NO_GUI
		InitializeWidgets();
		ShowWidgets(false);
		#endif
	}
	
	
	void ~PluginItemDiagnostic()
	{
		ClearProperties();
	}

	void RegisterDebugItem(ItemBase item, PlayerBase player)
	{
		if(!myTimer1)
		{
			myTimer1 = new Timer();
			myTimer1.Run(1, this, "Tick", NULL, true);
		}
		
		if(m_ObserversMap.Contains(player))
		{
			m_ObserversMap.Set(player,item);
		}
		else
		{
			m_ObserversMap.Insert(player,item);
		}
	}
	
	
	bool IsActive()
	{
		return m_IsActive;
	}

	void ShowWidgets(bool show)
	{
		if(m_DebugRootWidget) m_DebugRootWidget.Show(show);
	}

	void ToggleDebugWindowEvent()
	{
		if(m_IsActive)
		{		
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			//GetGame().RPCSingleParam(player, RPC_ITEM_DIAG_CLOSE,NULL);
			ShowWidgets(false);
			ClearWidgets();
			m_IsActive = false;
		}
		else
		{
			ShowWidgets(true);
			m_IsActive = true;
		}

	}

	void ClearWidgets()
	{
		m_DebugAgentListWidget.ClearItems();
		m_DebugFloatsListWidget.ClearItems();
		m_DebugStringsListWidget.ClearItems();
		m_DebugFloatsProperListWidget.ClearItems();
		m_ItemPreviewWidget.SetItem(NULL);
		m_ClassNameWidget.SetText("");		
	}
	
	void Tick()
	{
		for(int i = 0; i < m_ObserversMap.Count();i++)
		{
			ItemBase item = m_ObserversMap.GetElement(i);
			PlayerBase player = m_ObserversMap.GetKey(i);

			if (item && player) 
			{
				GeneratePropertiesObject(item);
				SendRPC(item, player.GetIdentity());
			}
			else
			{
				Debug.LogError("PluginItemDiagnostic: dead debug record, removing record");
				m_ObserversMap.RemoveElement(i);
			}
		}
		
	}
	
	void ClearProperties()
	{
		m_Properties.Clear();
	}


	void GeneratePropertiesObject(ItemBase item)
	{
		ClearProperties();
		GetLocalProperties(item, m_Properties);
		/*
		int count = m_Properties.Count();
		Debug.Log("items saved = "+count.ToString(),"fuckup");
		*/
	}
	
	void SendRPC(ItemBase item, PlayerIdentity identity)
	{
		GetGame().RPC(item,ERPCs.RPC_ITEM_DIAG,m_Properties,true,identity);
	}
	
	void StopWatchRequest(PlayerBase player)//called from player after an RPC call
	{
		if( m_ObserversMap.Contains(player) )
		{
			m_ObserversMap.Remove(player);
		}
	}

	void OnRPC(ItemBase item, ParamsReadContext ctx)
	{

		ctx.Read(CashedObjectsParams.PARAM1_INT);
		int agents_count 	= CashedObjectsParams.PARAM1_INT.param1;

		ctx.Read(CashedObjectsParams.PARAM1_INT);
		int individual_items_count 	= CashedObjectsParams.PARAM1_INT.param1;
		
		FillAgentArray(ctx, agents_count);
		
		array<ref Param> vars_server = new array<ref Param>;
		FillServerFinalVars(individual_items_count, ctx, vars_server);
		
		
		array<ref Param> vars_client = new array<ref Param>;
		GetLocalProperties(item, vars_client, true);
		
		//FillClientVarsMap(item);
		//FillClientString(item);
		DisplayAll(item, vars_server, vars_client );
	}
	
	void FillServerFinalVars(int count, ParamsReadContext ctx, array<ref Param> params )
	{
		for(int i = 0; i < count; i++)
		{
			Param2<string,float> p2 = new Param2<string,float>("",0);
			
			ctx.Read(p2);
			/*
			string name = CashedObjectsParams.PARAM2_STRING_FLOAT.param1;
			float value = CashedObjectsParams.PARAM2_STRING_FLOAT.param2;
			PrintString(name+","+value.ToString());
			*/
			params.Insert(p2);
		}
		//PrintString("----------------------------------------------------");
			
	}
	
	void InitializeWidgets()
	{
		if(!m_DebugRootWidget) m_DebugRootWidget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/debug_item.layout");
		m_DebugAgentListWidget = TextListboxWidget.Cast(m_DebugRootWidget.FindAnyWidget("w_Agents"));
		m_DebugFloatsListWidget = TextListboxWidget.Cast(m_DebugRootWidget.FindAnyWidget("w_Floats"));
		m_DebugStringsListWidget = TextListboxWidget.Cast(m_DebugRootWidget.FindAnyWidget("w_Strings"));
		m_DebugFloatsProperListWidget = TextListboxWidget.Cast(m_DebugRootWidget.FindAnyWidget("w_FloatsProper"));
		m_ItemPreviewWidget = ItemPreviewWidget.Cast(m_DebugRootWidget.FindAnyWidget("w_ItemPreview"));
		m_ClassNameWidget = TextWidget.Cast(m_DebugRootWidget.FindAnyWidget("w_ClassName"));
	}

	void DisplayAll(ItemBase item, array<ref Param> vars_server, array<ref Param> vars_client)
	{
		m_ClassNameWidget.SetText( item.GetType() );
		m_ItemPreviewWidget.SetItem(item);
		UpdateAgentWidget();
		//UpdateFloatWidget();
		UpdateNumericalVarsWidget(vars_server, vars_client);
	}	

	void FillAgentArray(ParamsReadContext ctx, int agents_count)
	{
		m_Agents.Clear();
		for(int i = 0; i < agents_count; i++)
		{
			ctx.Read(CashedObjectsParams.PARAM1_STRING);
			string agent = CashedObjectsParams.PARAM1_STRING.param1;
			m_Agents.Insert(agent);
		}
	}


	void UpdateAgentWidget()
	{
		m_DebugAgentListWidget.ClearItems();
		for(int i = 0; i < m_Agents.Count(); i++)
		{
			string agent = m_Agents.Get(i);
			m_DebugAgentListWidget.AddItem(agent,NULL,0);
		}
	}	

	
	void UpdateNumericalVarsWidget(array<ref Param> vars_server, array<ref Param> vars_client)
	{
		m_DebugFloatsProperListWidget.ClearItems();
		for(int i = 0; i < vars_server.Count(); i++)
		{
			Param param_server = vars_server.Get(i);
			Param2<string,float> p2_server = Param2<string,float>.Cast(param_server);
			
			Param param_client = vars_client.Get(i);
			Param2<string,float> p2_client = Param2<string,float>.Cast(param_client);
			
			string name = p2_server.param1;
			m_DebugFloatsProperListWidget.AddItem(name,NULL,0);
			m_DebugFloatsProperListWidget.SetItem(i,p2_server.param2.ToString(),NULL,1);
			m_DebugFloatsProperListWidget.SetItem(i,p2_client.param2.ToString(),NULL,2);
		}
	}	
	
	
	void PrintOut()
	{
		Debug.LogArrayString(m_Agents,"ItemDebug");
		Debug.Log("----------------------","ItemDebug");
	}
}
	//-------------------------
	// QUERY FOR DEBUG PROPERTIES
	//-------------------------
	void GetLocalProperties(ItemBase item, array<ref Param> properties, bool client = false)
	{
		if(!client)
		{
			// -------- AGENTS ------------
			int num_of_agents = FillWithAgents(item, properties);
			Param1<int> agents_count = new Param1<int>(num_of_agents);
			properties.InsertAt(agents_count,0);
		}
	
		// ------ INDIVIDUAL VARS------
		int number_of_items = 0;
		Param2<string, float> quantity = new Param2<string, float>("quantity", item.GetQuantity() );
		properties.Insert(quantity);
		number_of_items++;
		//-----------------------------
		Param2<string, float> liquid_type = new Param2<string, float>("liquid_type", item.GetLiquidType() );
		properties.Insert(liquid_type);
		number_of_items++;
		//-----------------------------
		Param2<string, float> wetness = new Param2<string, float>("wetness", item.GetWet() );
		properties.Insert(wetness);
		number_of_items++;
		//-----------------------------
		Param2<string, float> temperature = new Param2<string, float>("temperature", item.GetTemperature() );
		properties.Insert(temperature);
		number_of_items++;
		//-----------------------------
		Param2<string, float> energy = new Param2<string, float>("energy", item.GetEnergy() );
		properties.Insert(energy);
		number_of_items++;
		//-----------------------------
		Param2<string, float> health;
		if(!client)
		{
			health = new Param2<string, float>("health", item.GetHealth("", "") );
		}
		else
		{
			health = new Param2<string, float>("NO SYNC",0 );
		}
		properties.Insert(health);
		number_of_items++;
		//-----------------------------
		int r,g,b,a;
		item.GetColor(r,g,b,a);
		Param2<string, float> p2r = new Param2<string, float>("R",r);
		properties.Insert(p2r);
		number_of_items++;
		Param2<string, float> p2g = new Param2<string, float>("G",g);
		properties.Insert(p2g);
		number_of_items++;
		Param2<string, float> p2b = new Param2<string, float>("B",b);
		properties.Insert(p2b);
		number_of_items++;
		Param2<string, float> p2a = new Param2<string, float>("A",a);
		properties.Insert(p2a);
		number_of_items++;
		//-----------------------------
		if(!client)
		{
			Param1<int> item_count = new Param1<int>(number_of_items);
			properties.InsertAt(item_count,1);
		}
	}

	int FillWithAgents(ItemBase item, array<ref Param> properties)
	{
		int agents = item.GetAgents();
		if(agents == 0) return 0;
		int num_of_agents = 0;
		ref array<int> agents_aray = new array<int>;
		PluginTransmissionAgents.BuildAgentArray(agents, agents_aray);
		int agents_count = agents_aray.Count();
	
		for(int i = 0; i < agents_count; i++)
		{
			PluginTransmissionAgents mta = PluginTransmissionAgents.Cast(GetPlugin(PluginTransmissionAgents));
			string agent_name = mta.GetNameByID(agents_aray.Get(i));
			Param1<string> param1 = new Param1<string>(agent_name);
			properties.Insert(param1);
			num_of_agents++;
		}

		return num_of_agents;
	}


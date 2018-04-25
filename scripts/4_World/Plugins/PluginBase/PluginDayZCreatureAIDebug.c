typedef Param4<float, string, int, string> 	DayZCreatureAnimScriptDebugAnimEventData;
typedef Param1<string> 						DayZCreatureAnimScriptDebugAnimPredictionData;
typedef Param1<string> 						DayZCreatureAnimScriptDebugAnimTagData;

class DayZCreatureAnimScriptDebug
{		
	proto native void 	SetCreature(DayZCreature creature);

	proto native int 	GetVariableCount();
	proto native string GetVariableName(int index);
	proto native int 	GetVariableType(int index);
	proto native int 	GetVariableInt(int index);
	proto native float 	GetVariableFloat(int index);
	proto native bool 	GetVariableBool(int index);
	
	proto native int 	SetVariableInt(int index, int value);
	proto native float 	SetVariableFloat(int index, float value);
	proto native bool 	SetVariableBool(int index, bool value);
		
	proto native int 	GetCommandCount();
	proto native string GetCommandName(int index);
	proto native int 	GetCommandID(int index);

	proto native void 	ActivateCommand(int index, int userInt, float userFloat);
	
	const int 														m_iMaxAnimEventsCount = 50;
	ref array<string> 												m_EventsFilter = new array<string>;
	
	ref array<ref DayZCreatureAnimScriptDebugAnimEventData>			m_AnimEvents = new array<ref DayZCreatureAnimScriptDebugAnimEventData>;
	ref array<ref DayZCreatureAnimScriptDebugAnimPredictionData>	m_AnimPredictions = new array<ref DayZCreatureAnimScriptDebugAnimPredictionData>;
	ref array<ref DayZCreatureAnimScriptDebugAnimTagData>			m_AnimTags = new array<ref DayZCreatureAnimScriptDebugAnimTagData>;
	
	void OnAnimationEventsStart()
	{
		m_AnimPredictions.Clear();
		m_AnimTags.Clear();
	}

	void OnAnimationEvent(string evType, int userInt, string userString)
	{
		if (m_EventsFilter.Find(evType) != -1)
		{
			return;
		}
		
		m_AnimEvents.InsertAt(new DayZCreatureAnimScriptDebugAnimEventData(GetWorldTime(), evType, userInt, userString), 0);
		
		if (m_AnimEvents.Count() > m_iMaxAnimEventsCount)
		{
			m_AnimEvents.Remove(m_AnimEvents.Count() - 1);
		}
	}
	
	void OnAnimationPrediction(string predName)
	{
		m_AnimPredictions.Insert(new DayZCreatureAnimScriptDebugAnimPredictionData(predName));
	}
	
	void OnAnimationTag(string tagName)
	{
		m_AnimTags.Insert(new DayZCreatureAnimScriptDebugAnimTagData(tagName));
	}
}

enum PluginDayZCreatureAIDebugSyncMessageType
{
	INIT_DEBUG_OBJECT,
	RELEASE_DEBUG_OBJECT,
	SET_VALUE_INT,
	SET_VALUE_FLOAT,
	SET_VALUE_BOOL,
	ACTIVATE_COMMAND,
	ENABLE_AI,
	DISABLE_AI,
	COUNT
}

class PluginDayZCreatureAIDebug extends PluginBase
{
	int 							m_iDebugMenu = -1;
	
	DayZCreatureAI					m_DebugEntity = NULL;
	string							m_sDebugEntityName = "";
	ref DayZCreatureAnimScriptDebug m_EntityAnimDbg = NULL;
	
	ref Timer						m_TickTimer;
	bool 							m_IsActive	= false;
	bool 							m_ShowDbgUI = false;
	
	bool							m_bIsCaptureMode = false;
	bool							m_bEditMode = false;
	bool							m_bAIEnabled = true;
	bool							m_bShowAnimEvents = false;
	
	bool							m_bBulkSet = false;
	ref	array<bool>					m_SetVariableCheckStates = new array<bool>;
	ref	array<bool>					m_SetCommandCheckStates = new array<bool>;
	
	ref array<ref Param> 			m_SyncMessages = new array<ref Param>;

	void PluginDayZCreatureAIDebug()
	{
	}
	
	void ~PluginDayZCreatureAIDebug()
	{
	}

	override void OnInit()
	{
	}
	
	override void OnDestroy()
	{
	}
	
	void CheckShowMode()
	{
		int	actMenuValue = DiagMenu.GetValue(DayZCreatureAIConstants.DEBUG_SHOWDEBUGPLUGIN);
		if (actMenuValue != m_iDebugMenu)
		{
			SetDebugShowMode(actMenuValue);
			m_iDebugMenu = actMenuValue;
		}
	}	
	
	override void OnUpdate(float delta_time)
	{
		if (!GetGame().IsMultiplayer() || GetGame().IsClient())
		{
			CheckShowMode();
		}
	}
	
	string GetStrValue(int index)
	{
		string strVal;
		switch (m_EntityAnimDbg.GetVariableType(index))
		{
			case DayZCreatureAnimScriptDebugVarType.INT:
				int valInt = m_EntityAnimDbg.GetVariableInt(index);
				strVal = valInt.ToString();
				break;
			case DayZCreatureAnimScriptDebugVarType.FLOAT:
				float valFloat = m_EntityAnimDbg.GetVariableFloat(index);
				strVal = valFloat.ToString();
				break;
			case DayZCreatureAnimScriptDebugVarType.BOOL:
				bool valBool = m_EntityAnimDbg.GetVariableBool(index);
				if (valBool)
				{
					strVal = "true";
				}
				else
				{
					strVal = "false";
				}
				break;
			default:
				strVal = "unk";
		}
		return strVal;
	}

	string GetStrValueType(int index)
	{
		string strValType;
		switch (m_EntityAnimDbg.GetVariableType(index))
		{
			case DayZCreatureAnimScriptDebugVarType.INT:
				strValType = "int";
				break;
			case DayZCreatureAnimScriptDebugVarType.FLOAT:
				strValType = "float";
				break;
			case DayZCreatureAnimScriptDebugVarType.BOOL:
				strValType = "bool";
				break;
			default:
				strValType = "unk";
		}
		
		return strValType;
	}
	
	void InitDebugObject(Object obj)
	{
		m_EntityAnimDbg = new DayZCreatureAnimScriptDebug();
		m_EntityAnimDbg.SetCreature( DayZCreature.Cast( obj ) );
		
		m_DebugEntity = DayZCreatureAI.Cast( obj );
		m_sDebugEntityName = obj.GetType();
		

		int varCount = m_EntityAnimDbg.GetVariableCount();
		int cmdCount = m_EntityAnimDbg.GetCommandCount();
		int toAddCount = 0;
		
		if (m_SetVariableCheckStates.Count() < varCount)
		{
			toAddCount = varCount - m_SetVariableCheckStates.Count();
			for (int idxVar = 0; idxVar < toAddCount; idxVar++)
			{
				m_SetVariableCheckStates.Insert(false);
			}
		}
			
		if (m_SetCommandCheckStates.Count() < cmdCount)
		{
			toAddCount = cmdCount - m_SetCommandCheckStates.Count();
			for (int idxCmd = 0; idxCmd < toAddCount; idxCmd++)
			{
				m_SetCommandCheckStates.Insert(false);
			}
		}
	}
	
	void ReleaseDebugObject()
	{
		m_DebugEntity = NULL;
		m_EntityAnimDbg = NULL;
		m_sDebugEntityName = "";
		
		m_SetVariableCheckStates.Clear();
		m_SetCommandCheckStates.Clear();
	}
	
	void SetValueInt(int index, int value)
	{
		m_EntityAnimDbg.SetVariableInt(index, value);
	}
	
	void SetValueFloat(int index, float value)
	{
		m_EntityAnimDbg.SetVariableFloat(index, value);
	}
	
	void SetValueBool(int index, bool value)
	{
		m_EntityAnimDbg.SetVariableBool(index, value);
	}
	
	void ActivateCommand(int commandIdx, int userInt, float userFloat)
	{
		int commandId = m_EntityAnimDbg.GetCommandID(commandIdx);
		m_EntityAnimDbg.ActivateCommand(commandId, userInt, userFloat);
	}
	
	void EnableAI(bool enable)
	{
		if (m_DebugEntity.IsInherited(DayZCreatureAI))
		{
			DayZCreatureAI creatureAI = m_DebugEntity;
			if (enable)
			{
				creatureAI.DebugRestoreAIControl();
			}
			else
			{
				creatureAI.DebugDisableAIControl();		
			}			
		}
	}
	
	DayZCreatureAnimScriptDebugVarType GetVariableType(int index)
	{
		return m_EntityAnimDbg.GetVariableType(index);
	}
	
	void OnGUI_Variables()
	{
		int varCount = m_EntityAnimDbg.GetVariableCount();
		for (int i=0; i < varCount; i++)
		{
			DbgUI.BeginCleanupScope();
			DbgUI.Text(m_EntityAnimDbg.GetVariableName(i) + ": " + GetStrValue(i));
			DbgUI.EndCleanupScope();
		}
	}
	
	void OnGUI_VariablesEdit()
	{
		const int INPUT_BOX_SIZE = 35;
		
		DbgUI.PushID_Str("VariablesEdit");
		
		int varCount = m_EntityAnimDbg.GetVariableCount();
		for (int i=0; i < varCount; i++)
		{
			DbgUI.BeginCleanupScope();
			
			DbgUI.PushID_Int(i);
			
			bool result;
			DbgUI.Check("", result);
			m_SetVariableCheckStates[i] = result;

			DbgUI.SameLine();			
			bool setButtonPressed = DbgUI.Button("Set");
			
			DbgUI.SameLine();
			
			string strUserVal;
			DbgUI.InputText(GetStrValueType(i), strUserVal, INPUT_BOX_SIZE);
			
			DbgUI.SameLine();
			DbgUI.Text(m_EntityAnimDbg.GetVariableName(i));
			
			if (setButtonPressed || (m_bBulkSet && m_SetVariableCheckStates[i]))
			{
				GUIAction_SetValue(i, strUserVal);
			}
			
			DbgUI.PopID();
			
			DbgUI.EndCleanupScope();
		}
		
		DbgUI.PopID();
	}
	
	void OnGUI_CommandsEdit()
	{
		const int INPUT_BOX_SIZE = 35;

		DbgUI.PushID_Str("CommandsEdit");
		int cmdCount = m_EntityAnimDbg.GetCommandCount();
		for (int i=0; i < cmdCount; i++)
		{
			DbgUI.BeginCleanupScope();

			DbgUI.PushID_Int(i);
			
			bool result;
			DbgUI.Check("", result);
			m_SetCommandCheckStates[i] = result;		

			DbgUI.SameLine();
			bool setButtonPressed = DbgUI.Button("Set");
			
			DbgUI.SameLine();			
			string strUserInt;
			DbgUI.InputText("int", strUserInt, INPUT_BOX_SIZE);
			
			DbgUI.SameLine();
			string strUserFloat;
			DbgUI.InputText("float", strUserFloat, INPUT_BOX_SIZE);			

			DbgUI.SameLine();
			DbgUI.Text(m_EntityAnimDbg.GetCommandName(i) + "[" + m_EntityAnimDbg.GetCommandID(i) + "]");
			
			if (setButtonPressed || (m_bBulkSet && m_SetCommandCheckStates[i]))
			{
				GUIAction_ActivateCommand(i, strUserInt.ToInt(), strUserFloat.ToFloat());
			}
			
			DbgUI.PopID();
			
			DbgUI.EndCleanupScope();
		}
		
		DbgUI.PopID();
	}
	
	void OnGUI_AIControlEdit()
	{
		bool buttonPressed = false;
		if (m_bAIEnabled)
		{
			buttonPressed = DbgUI.Button("Disable AI");
		}
		else
		{
			buttonPressed = DbgUI.Button("Enable AI");
		}
		
		if (buttonPressed)
		{
			m_bAIEnabled = !m_bAIEnabled;
			
			GUIAction_EnableAI(m_bAIEnabled);
		}
	}
	
	void OnGUI_BulkSet()
	{
		m_bBulkSet = false;
		
		if (DbgUI.Button("Bulk set"))
		{
			m_bBulkSet = true;			
		}
	}
	
	void OnGUI_AnimEvents()
	{
		DbgUI.PushID_Str("AnimEvents");
		
		const int panelMinSizeX = 350;

		DbgUI.Panel("MinimumSize", panelMinSizeX, 1);
		
		string strTags;
		for (int tagIdx = 0; tagIdx < m_EntityAnimDbg.m_AnimTags.Count(); ++tagIdx)
		{
			if (tagIdx != 0)
				strTags += ", ";
			
			strTags += m_EntityAnimDbg.m_AnimTags[tagIdx].param1;
		}
		
		string strPredictions;
		for (int predIdx = 0; predIdx < m_EntityAnimDbg.m_AnimPredictions.Count(); ++predIdx)
		{
			if (predIdx != 0)
				strPredictions += ", ";
			
			strPredictions += m_EntityAnimDbg.m_AnimPredictions[predIdx].param1;
		}
		
		DbgUI.PushID_Str("AnimTagsDisplay");
		DbgUI.Text(strTags);
		DbgUI.PopID();
		
		DbgUI.PushID_Str("AnimPredictionsDisplay");
		DbgUI.Text(strPredictions);
		DbgUI.PopID();
		
		string strFilter;
		DbgUI.InputText("Filter", strFilter, panelMinSizeX);

		m_EntityAnimDbg.m_EventsFilter.Clear();
		strFilter.Split(" ", m_EntityAnimDbg.m_EventsFilter);
				
		DbgUI.Button("TakeMyFocus");
		
		const int evDisplayCount = 15;
		int evToDisplayCount = (int)Math.Min(m_EntityAnimDbg.m_AnimEvents.Count(), evDisplayCount);
		
		for (int evIdx = 0; evIdx < evToDisplayCount; ++evIdx)
		{			
			DbgUI.PushID_Int(evIdx);
			if (m_EntityAnimDbg.m_AnimEvents[evIdx].param2.Length() > 0)
			{
				DbgUI.Text((m_EntityAnimDbg.m_AnimEvents[evIdx].param1 / 1000.0).ToString() + " - " + m_EntityAnimDbg.m_AnimEvents[evIdx].param2);
			}
			DbgUI.PopID();
		}	
		
		DbgUI.PopID();
	}
	
	void OnGUI(bool show)
	{
		const int windowPosX = 0;
		const int windowPosY = 300;
		const int mainPanelSizeX = 200;
		const int mainPanelSizeY = 1;
		const int margin = 10;
		
		DbgUI.BeginCleanupScope();
		DbgUI.Begin("CretureAI debug", windowPosX, windowPosY);
		if (show)
		{
			if (m_EntityAnimDbg == NULL)
			{
				if (m_bIsCaptureMode == true)
				{
					DbgUI.Text("Capturing...");	
				}
				else
				{
					if (DbgUI.Button("Capture"))
					{
						m_bIsCaptureMode = true;
					}
				}
				
				// Clear additional windows
				DbgUI.Begin("CreatureAI EditMenu");
				DbgUI.End();
				
				DbgUI.Begin("CreatureAI AnimEvents");
				DbgUI.End();
			}
			else
			{
				DbgUI.Panel("MinimumSize", mainPanelSizeX, mainPanelSizeY);
				
				DbgUI.Text(m_sDebugEntityName);
				DbgUI.SameLine();
				bool btnReleasePressed = DbgUI.Button("Release");
				
				DbgUI.Check("Edit", m_bEditMode);
				DbgUI.Check("ShowAnimEvents", m_bShowAnimEvents);
				
				#ifdef _DAYZ_CREATURE_DEBUG_SHADOW
				if (!GetGame().IsMultiplayer())
				{
					DbgUI.SameLine();
					if (DbgUI.Button("CreateShadow"))
					{
						GUIAction_CreateShadow();
					}
				}
				#endif
				
				if (!GetGame().IsMultiplayer())
				{
					const int simLODInputSize = 20;
					int simLOD;
					DbgUI.InputInt("SimLOD", simLOD, simLODInputSize);
					DbgUI.SameLine();
					if (DbgUI.Button("UpdateSimulationPrecision"))
					{
						GUIAction_UpdateSimulationPrecision(simLOD);
					}
				}
				
				if (btnReleasePressed)
				{
					GUIAction_ReleaseDebugObject();
				}
				else
				{
					OnGUI_Variables();
					DbgUI.Begin("CreatureAI EditMenu", windowPosX + mainPanelSizeX + margin, windowPosY);
					if (m_bEditMode)
					{						
						OnGUI_AIControlEdit();
						OnGUI_BulkSet();
						OnGUI_VariablesEdit();
						DbgUI.Spacer(20);
						OnGUI_CommandsEdit();	
					}	
					DbgUI.End();
					
					DbgUI.Begin("CreatureAI AnimEvents", windowPosX + mainPanelSizeX + margin, windowPosY);
					if (m_bShowAnimEvents)
					{
						OnGUI_AnimEvents();
					}
					DbgUI.End();
				}
			}
		}
		DbgUI.End();
		DbgUI.EndCleanupScope();
	}
	
	void OnGUITimer()
	{
		OnGUI(m_IsActive);
		
		if (GetGame().IsMultiplayer() && GetGame().IsClient())
		{
			SendSyncMessages();
		}	
	}
	
	void SetDebugShowMode(int mode)
	{
		switch (mode)
		{
			case 0:
			{
				Hide();
			} break;
			case 1:
			{
				Show();
			} break;
		}
	}
		
	void Show()
	{
		m_IsActive = true;
			
		m_TickTimer = new Timer();
		m_TickTimer.Run(0.1, this, "OnGUITimer", NULL, true);
	}
	
	void Hide()
	{
		m_IsActive = false;
		m_TickTimer = NULL;
		OnGUI(false);
	}
	
	void Event_OnClick()
	{
		if (m_IsActive == false)
			return;
		
		const float MAX_RAYCAST_RANGE = 1000;
		if (m_bIsCaptureMode)
		{	
			vector dir = GetGame().GetPointerDirection();
			vector pos = GetGame().GetCurrentCameraPosition();		
			
			// Raycast
			vector from = pos;
			vector to = pos + (dir * MAX_RAYCAST_RANGE);
			vector contact_pos;
			vector contact_dir;
			int contact_component;
			set<Object> objects = new set<Object>;
			
			if (DayZPhysics.RaycastRV(from, to, contact_pos, contact_dir, contact_component, objects, NULL, GetGame().GetPlayer()))
			{
				for ( int i = 0; i < objects.Count(); i++ )
				{
					Object obj = objects.Get(i);
					if (obj && obj.IsInherited(DayZCreature))
					{
						m_bIsCaptureMode = false;
						GUIAction_InitDebugObject(obj);
						return;
					}
				}
			}
		}
	}
	
	///	
	//! GUI actions
	///
	
	void GUIAction_InitDebugObject(Object obj)
	{
		InitDebugObject(obj);
		if (GetGame().IsMultiplayer())
		{
			SyncInitDebugObject(obj);
		}
	}

	#ifdef _DAYZ_CREATURE_DEBUG_SHADOW
	void GUIAction_CreateShadow()
	{
		auto shadowEntity = GetGame().CreateObject(m_sDebugEntityName, m_DebugEntity.GetPosition(), false, true);
		m_DebugEntity.DebugSetShadow(shadowEntity);
	}
	#endif
	
	void GUIAction_UpdateSimulationPrecision(int simLOD)
	{
		m_DebugEntity.UpdateSimulationPrecision(simLOD);
	}
	
	void GUIAction_ReleaseDebugObject()
	{
		ReleaseDebugObject();		
		if (GetGame().IsMultiplayer())
		{
			SyncReleaseDebugObject();
		}
	}
	
	void GUIAction_SetValue(int index, string strVal)
	{
		switch (GetVariableType(index))
		{
			case DayZCreatureAnimScriptDebugVarType.INT:
				int intValue = strVal.ToInt();
				if (GetGame().IsMultiplayer())
				{
					SyncSetValueInt(index, intValue);
				}
				else
				{
					SetValueInt(index, intValue);
				}
				break;
			case DayZCreatureAnimScriptDebugVarType.FLOAT:
				float floatValue = strVal.ToFloat();
				if (GetGame().IsMultiplayer())
				{
					SyncSetValueFloat(index, floatValue);
				}
				else
				{
					SetValueFloat(index, floatValue);
				}
				break;
			case DayZCreatureAnimScriptDebugVarType.BOOL:
				bool boolValue;
				strVal.ToLower();
				if (strVal.Contains("true"))
				{
					boolValue = true;
				}
				else if (strVal.Contains("false"))
				{
					boolValue = false;
				}
				else
				{
					boolValue = (bool)strVal.ToInt();
				}
			
				if (GetGame().IsMultiplayer())
				{
					SyncSetValueBool(index, boolValue);
				}
				else
				{
					SetValueBool(index, boolValue);
				}
				break;
		}
	}
	
	void GUIAction_ActivateCommand(int commandIdx, int userInt, float userFloat)
	{
		if (GetGame().IsMultiplayer())
		{
			SyncActivateCommand(commandIdx, userInt, userFloat);
		}
		else
		{
			ActivateCommand(commandIdx, userInt, userFloat);	
		}
		
	}
	
	void GUIAction_EnableAI(bool enable)
	{
		if (GetGame().IsMultiplayer())
		{
			SyncEnableAI(enable);
		}
		else
		{
			EnableAI(enable);
		}		
	}
	
	///
	//! sync
	///

	void SendSyncMessages()
	{
		int count = m_SyncMessages.Count();
		if (count > 0)
		{
			ref Param1<int> paramCount = new Param1<int>(count);
			m_SyncMessages.InsertAt(paramCount, 0);
			
			GetGame().GetPlayer().RPC(ERPCs.DEV_RPC_PLUGIN_DZCREATURE_DEBUG, m_SyncMessages, true);
			
			m_SyncMessages.Clear();
		}
	}
		
	void OnRpc(PlayerBase player, int rpc_type, ParamsReadContext ctx)
	{
		if (rpc_type == ERPCs.DEV_RPC_PLUGIN_DZCREATURE_DEBUG)
		{
			SyncReceiveMessage(ctx);
		}
	}	
	
	void SyncReceiveMessage(ParamsReadContext ctx)
	{
		ref Param1<int> count = new Param1<int>(0);
		
		if (ctx.Read(count))
		{
			ref Param1<int> type = new Param1<int>(PluginDayZCreatureAIDebugSyncMessageType.COUNT);
			for (int i=0; i < count.param1; ++i)
			{
				if (ctx.Read(type))
				{
					switch (type.param1)
					{
						case PluginDayZCreatureAIDebugSyncMessageType.INIT_DEBUG_OBJECT:
							SyncReceiveInitDebugObject(ctx);
							break;
						case PluginDayZCreatureAIDebugSyncMessageType.RELEASE_DEBUG_OBJECT:
							SyncReceiveReleaseDebugObject(ctx);
							break;
						case PluginDayZCreatureAIDebugSyncMessageType.SET_VALUE_INT:
							SyncReceiveValueInt(ctx);
							break;
						case PluginDayZCreatureAIDebugSyncMessageType.SET_VALUE_FLOAT:
							SyncReceiveValueFloat(ctx);
							break;
						case PluginDayZCreatureAIDebugSyncMessageType.SET_VALUE_BOOL:
							SyncReceiveValueBool(ctx);
							break;
						case PluginDayZCreatureAIDebugSyncMessageType.ACTIVATE_COMMAND:
							SyncReceiveActivateCommand(ctx);
							break;
						case PluginDayZCreatureAIDebugSyncMessageType.ENABLE_AI:
							SyncReceiveEnableAI(ctx);
							break;
						case PluginDayZCreatureAIDebugSyncMessageType.DISABLE_AI:
							SyncReceiveDisableAI(ctx);
							break;
					}
				}
				type.param1 = PluginDayZCreatureAIDebugSyncMessageType.COUNT;
			}
		}
	}
	
	void SyncInitDebugObject(Object obj)
	{
		ref Param1<int> type = new Param1<int>(PluginDayZCreatureAIDebugSyncMessageType.INIT_DEBUG_OBJECT);		
		ref Param1<Object> object = new Param1<Object>(obj);
		
		m_SyncMessages.Insert(type);
		m_SyncMessages.Insert(object);
	}
	
	void SyncReceiveInitDebugObject(ParamsReadContext ctx)
	{
		ref Param1<Object> object = new Param1<Object>(NULL);
		if (ctx.Read(object))
		{
			InitDebugObject(object.param1);
		}
	}
	
	void SyncReleaseDebugObject()
	{
		ref Param1<int> type = new Param1<int>(PluginDayZCreatureAIDebugSyncMessageType.RELEASE_DEBUG_OBJECT);
		
		m_SyncMessages.Insert(type);
	}
	
	void SyncReceiveReleaseDebugObject(ParamsReadContext ctx)
	{
		ReleaseDebugObject();
	}
	
	void SyncSetValueInt(int index, int value)
	{
		ref Param1<int> type = new Param1<int>(PluginDayZCreatureAIDebugSyncMessageType.SET_VALUE_INT);		
		ref Param2<int, int> paramValue = new Param2<int, int>(index, value);
		
		m_SyncMessages.Insert(type);
		m_SyncMessages.Insert(paramValue);
	}
	
	void SyncReceiveValueInt(ParamsReadContext ctx)
	{
		ref Param2<int, int> paramValue = new Param2<int, int>(-1, 0);
		
		if (ctx.Read(paramValue))
		{
			SetValueInt(paramValue.param1, paramValue.param2);
		}
	}
	
	void SyncSetValueFloat(int index, float value)
	{
		ref Param1<int> type = new Param1<int>(PluginDayZCreatureAIDebugSyncMessageType.SET_VALUE_FLOAT);		
		ref Param2<int, float> paramValue = new Param2<int, float>(index, value);
		
		m_SyncMessages.Insert(type);
		m_SyncMessages.Insert(paramValue);
	}
	
	void SyncReceiveValueFloat(ParamsReadContext ctx)
	{
		ref Param2<int, float> paramValue = new Param2<int, float>(-1, 0.0);
		
		if (ctx.Read(paramValue))
		{
			SetValueFloat(paramValue.param1, paramValue.param2);
		}
	}
	
	void SyncSetValueBool(int index, bool value)
	{
		ref Param1<int> type = new Param1<int>(PluginDayZCreatureAIDebugSyncMessageType.SET_VALUE_BOOL);		
		ref Param2<int, bool> paramValue = new Param2<int, bool>(index, value);
		
		m_SyncMessages.Insert(type);
		m_SyncMessages.Insert(paramValue);
	}
	
	void SyncReceiveValueBool(ParamsReadContext ctx)
	{
		ref Param2<int, bool> paramValue = new Param2<int, bool>(-1, false);
		
		if (ctx.Read(paramValue))
		{
			SetValueBool(paramValue.param1, paramValue.param2);
		}
	}
	
	void SyncActivateCommand(int commandIdx, int userInt, float userFloat)
	{
		ref Param1<int> type = new Param1<int>(PluginDayZCreatureAIDebugSyncMessageType.ACTIVATE_COMMAND);
		ref Param3<int, int, float> command = new Param3<int, int, float>(commandIdx, userInt, userFloat);
		
		m_SyncMessages.Insert(type);
		m_SyncMessages.Insert(command);
	}
	
	void SyncReceiveActivateCommand(ParamsReadContext ctx)
	{
		ref Param3<int, int, float> command = new Param3<int, int, float>(-1, 0, 0.0);
		
		if (ctx.Read(command))
		{
			ActivateCommand(command.param1, command.param2, command.param3);
		}
	}
	
	void SyncEnableAI(bool enable)
	{
		ref Param1<int> type = new Param1<int>(PluginDayZCreatureAIDebugSyncMessageType.COUNT);
		if (enable)
		{
			type.param1 = PluginDayZCreatureAIDebugSyncMessageType.ENABLE_AI;
		}
		else
		{
			type.param1 = PluginDayZCreatureAIDebugSyncMessageType.DISABLE_AI;
		}
		
		m_SyncMessages.Insert(type);
	}
		
	void SyncReceiveEnableAI(ParamsReadContext ctx)
	{
		EnableAI(true);
	}
		
	void SyncReceiveDisableAI(ParamsReadContext ctx)
	{
		EnableAI(false);
	}
}

/** @file */

/// tree traversal type, for more see http://en.wikipedia.org/wiki/Tree_traversal
enum InventoryTraversalType
{
	PREORDER,
	INORDER,
	POSTORDER,
	LEVELORDER
};

const int INDEX_NOT_FOUND = -1;
//-----------------------------------------------------------------------------
typedef Serializer ParamsReadContext;
typedef Serializer ParamsWriteContext;

/**
\brief Class for sending RPC over network
	@code
		// example sending
		void Send()
		{
			ScriptRPC rpc = new ScriptRPC();
	
			rpc.Write(645);
			rpc.Write("hello");
			
			array<float> farray = {1.2, 5.6, 8.1};
			rpc.Write(farray);
	
			rpc.Send(m_Player, ERPCs.RPC_TEST, true, m_Player.GetIdentity());
		}

		// example receive
		void OnRPC(ParamsReadContext ctx)
		{
			int num;
			string text;
			array<float> farray;

			ctx.Read(num);
			ctx.Read(text);
			ctx.Read(farray);
		}
	@endcode
*/

class JsonSerializer: Serializer
{
	void JsonSerializer() {}
	void ~JsonSerializer() {}
	
	/**
	\brief Script variable serialization to json string
	@param script variable to be serialized to string
	@param if the string should be formated for human readability
	@param result from the serialization, output or error depending on the return value 
	\return if the serialization was successful
	@code
		Data data;
		string textOut; 
		bool nice = false;
		JsonSerializer js = new JsonSerializer();
		bool ok = js.WriteToString(data, false, textOut);	
	@endcode	
	*/
	proto bool WriteToString(void variable_out, bool nice, out string result);
	
	/**
	\brief Json string deserialization to script variable
	@param script variable to be deserialized from string
	@param the input string
	@param error from the deserialization. Is used only if the return value of the function is false
	\return if the deserialization was successful
	@code
		// Example json
		//  {
		//    "i": 6,                                 // Int
		//    "f": 3.5,                               // Float
		//    "v": [1.1,2.2,3.3]                      // Vector3 is static array of size 3
		//    "s": "string",                          // String
		//    "subData": {                            // Object
		//      "staticIArr": [9,8],                  // Static array (of ints)
		//      "dynamicSArr": ["string1","string2"]  // Dynamic array (of strings)
		//    },                                      //
		//    "fSet": [12.3,14.7],                    // Set (of floats)
		//    "ifMap": {                              // Map (of int, float), only valid key type is int, enum, string
		//      "3": 4.5,							  // Map key has to be as string
		//      "4": 5.5
		//    }
		//  }
	
		Data data = new Data;
		string input = // valid json string;
		string error;
	    bool ok = js.ReadFromString(data, input, error);
	@endcode	
	*/
	proto bool ReadFromString(void variable_in, string jsonString, out string error);
}


class ScriptRPC: ParamsWriteContext
{
	void ScriptRPC();
	void ~ScriptRPC();
	//! Reset internal buffer which stores written data. After Reset is callded, ScriptRPC can be used again as "new" ScriptRPC
	proto native void Reset();
	/**
  \brief Initiate remote procedure call. When called on client, RPC is evaluated on server; When called on server, RPC is executed on all clients. 
	Do not reset ScriptRPC internal state, so if is Send called multiple times in a row, it sends same (previously written) data again and again, until is Reset() called.
	@param target object on which remote procedure is called, when NULL, RPC is evaluated by CGame as global
	@param rpc_type user defined identification of RPC
	@param recipient specified client to send RPC to. If NULL, RPC will be send to all clients (specifying recipient increase security and decrease network traffic)
	*/
	proto native void Send(Object target, int rpc_type, bool guaranteed,PlayerIdentity recipient = NULL);
};

class ScriptInputUserData : ParamsWriteContext
{
	void ScriptInputUserData ();
	void ~ScriptInputUserData ();

	proto native void Reset ();
	proto native void Send ();
	proto native static bool CanStoreInputUserData ();
};

class ScriptRemoteInputUserData : ParamsWriteContext
{
	void ScriptRemoteInputUserData ();
	void ~ScriptRemoteInputUserData ();

	proto native void Reset ();
};

class ScriptJunctureData : ParamsWriteContext
{
	void ScriptJunctureData ();
	void ~ScriptJunctureData ();

	proto native void Reset ();
};

//-----------------------------------------------------------------------------
class MeleeCombatData
{
	proto native int GetModesCount();

	proto native string GetModeName(int index);

	proto native string GetAmmoTypeName(int index);

	proto native float GetModeRange(int index);
	
	private void MeleeCombatData();
	private void ~MeleeCombatData();
}

//-----------------------------------------------------------------------------
const string NullStringArray[1] = { "" };

//-----------------------------------------------------------------------------
//! Selection class
class Selection
{
	proto native string GetName();
	proto native int GetVertexCount();
	proto native int GetLODVertexIndex(int sel_vertex_index);

	vector GetVertexPosition(LOD lod, int index)
	{
		int lodIndex = GetLODVertexIndex(index);
		if(lodIndex == -1)
		{
			Error("Vertex doesn't exist");
			return vector.Zero;
		}
		
		return lod.GetVertexPosition(lodIndex);
	}
};

//-----------------------------------------------------------------------------
//! LOD class
class LOD
{
	proto native int GetSelectionCount();
	proto native bool GetSelections(notnull out array<Selection> selections);

	proto native vector GetVertexPosition(int vertex_index);
	
	proto native string GetName(Object myObject);
	
	Selection GetSelectionByName( string name )
	{
		array<Selection> selections = new array<Selection>;
		GetSelections( selections );
		
		for ( int i = 0; i < selections.Count(); ++i )
		{
			string selection_name = selections.Get( i ).GetName();
			selection_name.ToLower();
			name.ToLower();
			if (selection_name == name)
			{
				return selections.Get( i );
			}
		}
		
		return NULL;
	}
}

class Plant extends Object
{
};

/*
class ParamEntry
{
	proto string GetString(string entryName);
	proto int GetInt(string entryName);
	proto float GetFloat(string entryName);
	proto ref ParamEntry GetEntry(string entryName); 
	proto int GetNumChildren();
	proto ref ParamEntry GetNumChildren(int n);	
};
*/
//-----------------------------------------------------------------------------
	
//-----------------------------------------------------------------------------
class ProxyInventory extends ObjectTyped
{
};

//-----------------------------------------------------------------------------
class ProxyCrew extends Entity
{
};

//-----------------------------------------------------------------------------
class ProxySubpart extends Entity
{
};

class EventParams: Managed
{
};

class EventParams1 extends EventParams
{
	Object obj;
	void EventParams1(Object o)
	{
		obj = o;
	}
};

class EntityEventHandler: Managed
{
	void OnEvent(EntityAI entity, int entity_event_type /* EE* */, Param params)
	{
	}
};

class EntityAnimEndEventHandler extends EntityEventHandler
{
	private string m_anim_name;
	private ref CallQueueContext m_call;

	void EntityAnimEndEventHandler(string anim_name, Class target, string fn, Param params = NULL)
	{
		m_anim_name = anim_name;
		m_call = new CallQueueContext(target, fn, params);
	}

	override void OnEvent(EntityAI entity, int entity_event_type /* EE* */, Param params)
	{
		if (entity_event_type != EEAnimDone) return;
		if (!m_call) return;

		Param1<string> param = Param1<string>.Cast( params );
		if (m_anim_name == "" || param.param1 == m_anim_name)
		{
			m_call.Call();
		}
	}
};

//-----------------------------------------------------------------------------

// custom widgets
//-----------------------------------------------------------------------------
class ItemPreviewWidget extends UIWidget
{
	proto native void SetItem(EntityAI object);
	proto native EntityAI GetItem();
	
	proto native int GetView();
	/**
	* 0 - default boundingbox_min + boundingbox_max + invView
	* 1 - boundingbox_min2 + boundingbox_max2 + invView2
	* 2 - boundingbox_min3 + boundingbox_max3 + invView3
	* ...	
	*/
	proto native void SetView(int viewIndex);

	proto native void		SetModelOrientation(vector vOrientation);
	proto native vector GetModelOrientation();
	proto native void		SetModelPosition(vector vPos);
	proto native vector GetModelPosition();
};

//-----------------------------------------------------------------------------
class PlayerPreviewWidget extends UIWidget
{
	proto native void		SetItemInHands(InventoryItem object);
	proto native InventoryItem		GetItemInHands();
	proto native void		SetPlayer(Man player);
	proto native Man		GetPlayer();

	proto native void		SetModelOrientation(vector vOrientation);
	proto native vector GetModelOrientation();
	proto native void		SetModelPosition(vector vPos);
	proto native vector GetModelPosition();
};

//-----------------------------------------------------------------------------
class HtmlWidget extends RichTextWidget
{
	proto native void LoadFile(string path);
};

//-----------------------------------------------------------------------------
//! Player description
class PlayerIdentity: Managed
{
	//! ping range estimation
	proto native int GetPingMin();
	//! ping range estimation
	proto native int GetPingMax();
	//! ping range estimation
	proto native int GetPingAvg();

	//! bandwidth estimation (in kbps)
	proto native int GetBandwidthMin();
	//! bandwidth estimation (in kbps)
	proto native int GetBandwidthMax();
	//! bandwidth estimation (in kbps)
	proto native int GetBandwidthAvg();
	
	//! nick (short) name of player
	proto native owned string GetName();
	//! full name of player
	proto native owned string GetFullName();
	//! unique id of player (derived from CD key, may include verified userId after a double colon)
	proto native owned string GetId();
	//!  id of player in one session (is reused after player disconnects)
	proto native owned int GetPlayerId();

	private void ~PlayerIdentity()
	{
	}
};

//-----------------------------------------------------------------------------
const int PROGRESS_START = 0;
const int PROGRESS_FINISH = 1;
const int PROGRESS_PROGRESS = 2;
const int PROGRESS_UPDATE = 3;

//-----------------------------------------------------------------------------
typedef int ChatChannel;

//-----------------------------------------------------------------------------
//! state, progress, title
typedef Param3<int, float, string> ProgressEventParams;
typedef Param1<string> ScriptLogEventParams;
typedef Param1<EntityAI> EntityNetworkUpdateEventParams;
//! channel, from, text, color config class
typedef Param4<int, string, string, string> ChatMessageEventParams;
typedef Param1<int> ChatChannelEventParams;
typedef Param1<int> SQFConsoleEventParams;
//! PlayerIdentity, useDB, pos, yaw, queueTime
typedef Param5<PlayerIdentity, bool, vector, float, int> PreloadEventParams;
//! PlayerIdentity, PlayerPos, Top, Bottom, Shoe, Skin
typedef Param3<PlayerIdentity, vector, Serializer> ClientNewEventParams; 
//! PlayerIdentity, Man
typedef Param2<PlayerIdentity, Man> ClientRespawnEventParams; 
//! PlayerIdentity, Man
typedef Param2<PlayerIdentity, Man> ClientReadyEventParams; 
//! PlayerIdentity, Man
typedef Param4<PlayerIdentity, Man, int, bool> ClientDisconnectedEventParams; 
//! PlayerIdentity, QueueTime, NewChar
typedef Param2<int, bool> ClientSpawningEventParams; 
//! Player
typedef Param1<Man> LogoutCancelEventParams; 
//! Error 
typedef Param1<string> DatabaseErrorEventParams; 
//! logoutTime
typedef Param1<int> LogoutEventParams; 
//! Width, Height, Windowed
typedef Param3<int, int, bool> WindowsResizeEventParams;
//! Enabled
typedef Param1<bool> VONStateEventParams;
//! Camera
typedef Param1<FreeDebugCamera> SetFreeCameraEventParams;
//! Duration
typedef Param1<int>MPConnectionLostEventParams;


//-----------------------------------------------------------------------------
#ifdef DOXYGEN
// just because of doc

enum ChatChannel
{
	CCNone,
	CCGlobal,
	CCVehicle,
	CCItemTransmitter,
	CCPublicAddressSystem,
	CCItemMegaphone,
	CCDirect,
	CCCustom1,
	CCCustom2,
	CCCustom3,
	CCCustom4,
	CCCustom5,
	CCCustom6,
	CCCustom7,
	CCCustom8,
	CCCustom9,
	CCCustom10,
	CCCustomLast = CCCustom10,
	CCStatus,
	CCSystem,
	CCN
};

enum EventType
{
	//! no params
	StartupEventTypeID,
	//! no params
	WorldCleaupEventTypeID,

	//-----------------------------------------------------------------------------
	//! no params
	MPSessionStartEventTypeID,
	//! no params
	MPSessionEndEventTypeID,
	//! no params
	MPSessionFailEventTypeID,
	//! no params
	MPSessionPlayerReadyEventTypeID,

	//-----------------------------------------------------------------------------
	//! params: \ref ProgressEventParams
	ProgressEventTypeID,
	//! no params
	NetworkManagerClientEventTypeID,
	//! no params
	NetworkManagerServerEventTypeID,
	//! no params
	DialogQueuedEventTypeID,
	//! params: \ref EntityNetworkUpdateEventParams
	EntityNetworkUpdateEventTypeID,
	//! params: \ref ChatMessageEventParams
	ChatMessageEventTypeID,
	//! params: \ref ChatChannelEventParams
	ChatChannelEventTypeID,
	//! params: \ref SQFConsoleEventParams
	SQFConsoleEventTypeID,
	//! params: \ref PreloadEventParams
	PreloadEventTypeID,
	//! params: \ref ClientNewEventParams
	ClientNewEventTypeID,	
	//! params: \ref ClientRespawnEventParams
	ClientRespawnEventTypeID,
	//! params: \ref ClientReadyEventParams
	ClientReadyEventTypeID,
	//! params: \ref ClientDisconnectedEventParams
	ClientDisconnectedEventTypeID,
	//! params: \ref LogoutCancelEventParams
	LogoutCancelEventTypeID,
	//! params: \ref ClientSpawningEventParams
	ClientSpawningEventTypeID,
	//! params: \ref LogoutEventParams
	LogoutEventTypeID,	
	//! params: \ref DatabaseErrorEventParams
	DatabaseErrorEventTypeID,	
	//! params: \ref ScriptLogEventParams
	ScriptLogEventTypeID,
	//! params: \ref VONStateEventParams
	VONStateEventTypeID,
	//! params: \ref SetFreeCameraEventParams
	SetFreeCameraEventTypeID,
	//! params: \ref MPConnectionLostEventParams
	MPConnectionLostEventTypeID
	
	//possible in engine events not accessable from script
	//ReloadShadersEvent
	//LoadWorldProgressEvent
	
	//SignStatusEvent
	//SetPausedEvent
	//TerminationEvent
	//UserSettingsChangedEvent
	//StorageChangedEvent
	//BeforeResetEvent
	//AfterRenderEvent
	//AfterResetEvent
	//CrashLogEvent
	//ConsoleEvent
};

#endif

class CursorIcons
{
	const string None 			= "";
	const string Cursor			= "set:dayz_gui image:cursor";
	const string CloseDoors 	= "set:dayz_gui image:close";
	const string OpenDoors 		= "set:dayz_gui image:open";
	const string OpenCarDoors 	= "set:dayz_gui image:open_car";
	const string CloseCarDoors 	= "set:dayz_gui image:close_car";
	const string EngineOff 		= "set:dayz_gui image:engine_off";
	const string EngineOn 		= "set:dayz_gui image:engine_on";
	const string LadderDown 	= "set:dayz_gui image:ladderdown";
	const string LadderOff 		= "set:dayz_gui image:ladderoff";
	const string LadderUp 		= "set:dayz_gui image:ladderup";
	const string LootCorpse 	= "set:dayz_gui image:gear";
	const string CloseHood 		= "set:dayz_gui image:close_hood";
	const string OpenHood 		= "set:dayz_gui image:open_hood";
	const string GetOut 		= "set:dayz_gui image:getout";
	const string GetInCargo 	= "set:dayz_gui image:get_in_cargo";
	const string Reload 		= "set:dayz_gui image:reload";
	const string GetInDriver 	= "set:dayz_gui image:get_in_driver";
	const string GetInCommander = "set:dayz_gui image:get_in_commander";
	const string GetInPilot 	= "set:dayz_gui image:get_in_pilot";
	const string GetInGunner 	= "set:dayz_gui image:get_in_gunner";
};

	

// some defs for CGame::ShowDialog()
/*
const int DBB_NONE = 0;
const int DBB_OK = 1;
const int DBB_YES = 2;
const int DBB_NO = 3;
const int DBB_CANCEL = 4;

const int DBT_OK = 0;		//just OK button
const int DBT_YESNO = 1;	//Yes and No buttons
const int DBT_YESNOCANCEL = 2; //Yes, No, Cancel buttons

const int DMT_NONE = 0;
const int DMT_INFO = 1;
const int DMT_WARNING = 2;
const int DMT_QUESTION = 3;
const int DMT_EXCLAMATION = 4;
*/

proto native CGame GetGame();

class Hud: Managed
{
	ref Timer m_Timer;
	void Init( Widget hud_panel_widget ) {}
	void DisplayNotifier( int key, int tendency, int status ) {}
	void DisplayBadge( int key, bool show ) {}
	void SetStamina( int value, int range ) {}
	void DisplayStance( int stance ) {}
	void DisplayPresence() {}
	void ShowCursor() { }
	void HideCursor() { }
	void SetCursorIcon( string icon ) { }
	void SetCursorIconScale( string type, float percentage ) { }
	void SetCursorIconOffset( string type, float x, float y ) { }
	void SetCursorIconSize( string type, float x, float y ) {	}
	void ShowWalkieTalkie( bool show ) { }
	void ShowWalkieTalkie( int fadeOutSeconds ) { }
	void SetWalkieTalkieText( string text ) { }
	void RefreshQuickbar( bool itemChanged = false ) {}
	void Show(bool show) {}
	void RefreshQuantity( EntityAI item_to_refresh ) {}
	void UpdateBloodName() {}
	void RefreshItemPosition( EntityAI item_to_refresh ) {}
	void Update(){}

	void SetPermanentCrossHair( bool show ) {}

};

//-----------------------------------------------------------------------------
//! Mission class
class Mission
{
	ScriptModule MissionScript;

	private void ~Mission();
	
	void OnInit()	{}
	void OnMissionStart() {}
	void OnMissionFinish()	{}
	void OnUpdate(float timeslice) {}
	void OnKeyPress(int key) {}
	void OnKeyRelease(int key) {}
	void OnMouseButtonPress(int button){}
	void OnMouseButtonRelease(int button){}
	void OnEvent(EventType eventTypeId, Param params) {}
	void OnItemUsed(InventoryItem item, Man owner) {}	
	void AddDummyPlayerToScheduler(Man player){}
	
	Hud GetHud()
	{ 
		return NULL;
	}
	
	UIScriptedMenu	CreateScriptedMenu( int id ) 
	{ 
		return NULL;
	}
	
	UIScriptedWindow	CreateScriptedWindow( int id ) 
	{ 
		return NULL;
	}
	
	bool IsPaused() 
	{ 
		return false; 
	}
	
	bool IsGame()
	{
		return false;
	}
	
	bool IsServer()
	{
		return false;
	}

	void Pause() {}
	void Continue() {}
	
	void AbortMission() {}

	void CreateLogoutMenu(UIMenuPanel parent) {}
	void StartLogoutMenu(int time) {}

	void CreateDebugMonitor() {}
	
	void RefreshCrosshairVisibility() {}

	void HideCrosshairVisibility() {}
	
	bool IsMissionGameplay()
	{
		return false;
	}
};

// -------------------------------------------------------------------------

class MenuData: Managed
{
	proto native int	GetCharactersCount();
	proto native int 	GetLastPlayedCharacter();
	proto native Man 	CreateCharacterPerson(int index);
	
	proto void 			GetCharacterName(int index, out string name);
	proto native void 	SetCharacterName(int index, string newName);
	
	proto native void 	SaveCharacters();
	proto native void 	LoadCharacters();
	
	//! User's Gamertag from xbox live
	proto bool			GetUserGamertag(out string gamertag);

	//proto native void	GetCharacterStringList(int characterID, string name, out TStringArray values);
	//proto bool			GetCharacterString(int characterID,string name, out string value);
}


// -------------------------------------------------------------------------
/*
// Option Access Type
const int AT_UNKNOWN = 0;
const int AT_OBJECTS_DETAIL = 1;
const int AT_TEXTURE_DETAIL = 2;
const int AT_VRAM_VALUE = 3;
const int AT_HDR_DETAIL = 4;
const int AT_FSAA_DETAIL = 5;
const int AT_VSYNC_VALUE = 6;
const int AT_ANISO_DETAIL = 7;
const int AT_OPTIONS_FXAA_VALUE = 8;
const int AT_OPTIONS_PIP_VALUE = 9;
const int AT_OPTIONS_SW_VALUE = 10;
const int AT_POSTPROCESS_EFFECTS = 11;
const int AT_QUALITY_PREFERENCE = 12;
const int AT_ATOC_DETAIL = 13;
const int AT_AMBIENT_OCCLUSION = 14;
const int AT_BLOOM = 15;
const int AT_ROTATION_BLUR = 16;
const int AT_SHADING_DETAIL = 17;
const int AT_SHADOW_DETAIL = 18;
const int AT_OPTIONS_TERRAIN = 19;
const int AT_OPTIONS_RESOLUTION = 20;
const int AT_OPTIONS_SLIDER_FILLRATE = 21;
const int AT_OPTIONS_REFRESH = 22;
const int AT_OPTIONS_GAMMA_SLIDER = 23;
const int AT_OPTIONS_BRIGHT_SLIDER = 24;
const int AT_OPTIONS_VISIBILITY_SLIDER = 25;
const int AT_OPTIONS_OBJECT_VISIBILITY_SLIDER = 26;
const int AT_OPTIONS_TRAFFIC_VISIBILITY_SLIDER = 27;
const int AT_OPTIONS_SHADOW_VISIBILITY_SLIDER = 28;
const int AT_OPTIONS_DRAWDISTANCE_SLIDER = 29;
const int AT_OPTIONS_BLOOD = 30;
const int AT_OPTIONS_IMPERIALUNITS = 31;
const int AT_OPTIONS_VEHICLEFREELOOK = 32;
const int AT_OPTIONS_WBUFFER = 33;
const int AT_ASPECT_RATIO = 34;
const int AT_OPTIONS_IGUISIZE = 35;
const int AT_CONFIG_YREVERSED = 36;
const int AT_OPTIONS_PERSPECTIVE = 37;
const int AT_OPTIONS_FIELD_OF_VIEW = 38;
const int AT_OPTIONS_MUSIC_SLIDER = 39;
const int AT_OPTIONS_EFFECTS_SLIDER = 40;
const int AT_OPTIONS_VON_SLIDER = 41;
const int AT_OPTIONS_MASTER_VOLUME = 42;
const int AT_OPTIONS_VOICES_SLIDER = 43;
const int AT_OPTIONS_MIC_SENS_SLIDER = 44;
const int AT_OPTIONS_SAMPLES_SLIDER = 45;
const int AT_OPTIONS_HWACC = 46;
const int AT_OPTIONS_EAX = 47;
const int AT_OPTIONS_SINGLE_VOICE = 48;
const int AT_OPTIONS_LANGUAGE = 49;
const int AT_OPTIONS_SUBTITLES = 50;
const int AT_OPTIONS_RADIO = 51;
const int AT_CONFIG_XAXIS = 52;
const int AT_CONFIG_YAXIS = 53;
const int AT_CONFIG_FLOATING_ZONE = 54;
const int AT_CONFIG_MOUSE_FILTERING = 55;
const int AT_CONFIG_HEAD_BOB = 56;

// Option Access Control Type
const int OA_CT_NUMERIC = 0;
const int OA_CT_SWITCH = 1;
const int OA_CT_LIST = 2;

// Option Field of view constants
const float OPTIONS_FIELD_OF_VIEW_MIN = 0.75242724772f;
const float OPTIONS_FIELD_OF_VIEW_MAX = 1.30322025726f;
*/

class OptionsAccess: Managed
{
	//proto private void ~OptionsAccess();
	//proto private void OptionsAccess();

	proto native  int		GetAccessType();
	proto native  int		GetControlType();
	proto native  void	Apply();
	proto native  void	Test();
	proto native  void	Revert();
	proto native  int		IsChanged();
	proto native  int		NeedRestart();
	proto native  int		SetChangeImmediately();
};

// -------------------------------------------------------------------------
class NumericOptionsAccess extends OptionsAccess
{
	proto native  float	ReadValue();
	proto native  void	WriteValue(float value);
	proto native	float	GetMin();
	proto native	float	GetMax();
};

// -------------------------------------------------------------------------
class ListOptionsAccess extends OptionsAccess
{
	proto native int		GetIndex();
	proto native void		SetIndex(int index);
	proto native int		GetItemsCount();
	proto	void					GetItemText(int index, out string value);
};

// -------------------------------------------------------------------------
class SwitchOptionsAccess extends OptionsAccess
{
	proto native void		Switch();
	proto	void					GetItemText(out string value);
};

// -------------------------------------------------------------------------
class GameOptions: Managed
{
	proto native void	Apply();
	proto native void	Revert();
	proto native void	Test();
	proto native OptionsAccess GetOption(int index);
	proto native int GetOptionsCount();
	proto native int NeedRestart();
};

// -------------------------------------------------------------------------
class Hive
{
	proto native void InitOnline( string host = "" );
	proto native void InitOffline();

	proto native void SetShardID( string shard );
	proto native void SetEnviroment( string env );

	proto native void CharacterSave( Man player );
	proto native void CharacterKill( Man player );
	proto native void CharacterExit( Man player );
};

proto native Hive CreateHive();
proto native void DestroyHive();
proto native Hive GetHive();

// -------------------------------------------------------------------------
class CETesting
{
	proto native void ExportSpawnData();
	proto native void ExportProxyData( vector vCenter, float fRadius );
	proto native void ExportProxyProto();
	proto native void MarkCloseProxy( float fRadius, bool bAllSelections );
	proto native void RemoveCloseProxy();
	proto native void ListCloseProxy( float fRadius );

	proto native void SpawnAnalyze( string sClassName );

	proto native void TimeShift( float fShift );
	proto native void OverrideLifeTime( float fLifeTime );

	proto native void NewRestock( bool bEnable );

	proto native void SpawnGroup( string sEvName, vector vPos );
	proto native void SpawnDE( string sEvName, vector vPos, float fAngle );
	proto native void SpawnLoot( string sEvName, vector vPos, float fAngle, int iCount, float fRange );

	proto native void SpawnDynamic( vector vPos, bool bShowCylinders );
	proto native void SpawnVehicles( vector vPos, bool bShowCylinders );
	proto native void SpawnBuilding( vector vPos, bool bShowCylinders );

	proto native void SpawnEntity( string sClassName, vector vPos, float fRange, int iCount );
	proto native void SpawnRotation( string sClassName, vector vPos, float fRange, int iCount, int iFlags );
	proto native void SpawnPerfTest( string sClassName, int iCount );
	proto native void CleanMap();

	proto native void PlatformStatTest();

	proto native void EconomyLog( string sLogType );
	proto native void EconomyMap( string sMapType );
	proto native void EconomyOutput( string sOutputType, float fRange );

};

proto native CETesting GetTesting();

// -------------------------------------------------------------------------
class CEItemProfile
{
	proto native int GetNominal(); // nominal - how many items should be aproximately in map
	proto native int GetMin(); // min - minimal count should be available in map

	proto native float GetQuantityMin(); // min quantity (0.0 - 1.0) (like ammobox - this determine how many bullets are there, or water bottle)
	proto native float GetQuantityMax(); // max quantity (0.0 - 1.0) (like ammobox - this determine how many bullets are there, or water bottle)

	proto native float GetQuantity(); // random quantity (0.0 - 1.0)

	proto native float GetLifetime(); // lifetime in (seconds) - what is the idle before item abandoned at ground gets deleted
	proto native float GetRestock(); // restock is oposite of lifetime - idle before item is allowed to respawn when required

	proto native int GetCost(); // cost of item determines its 'value' for players (this serve as priority during respawn and cleanup operation)

	proto native int GetUsageFlags(); // area usage flags (each bit has assigned group - which as part of map overlay effectively affects spawning)
	proto native int GetValueFlags(); // area value flags (each bit has assigned group - which as part of map overlay effectively affects spawning)
};


typedef Link<Object> OLinkT;

/**
 * @fn	SpawnEntity
 * @brief	Spawns entity in known inventory location
 *
 * @param[in]	object_name \p string Name of item class
 * @param[in] inv_loc \p Inventory Location of the spawned item
 * @return		created entity in case of success, null otherwise
 **/
EntityAI SpawnEntity (string object_name, notnull InventoryLocation inv_loc)
{
	return GameInventory.LocationCreateEntity(inv_loc, object_name);
}

class Static : Object
{
};


class PrtTest // Temporal class for toggling particles on guns
{
	static bool m_GunParticlesState = true;
}

// #include "Scripts/Classes/Component/_include.c"

/**
 *  Game Class provide most "world" or global engine API functions.
 */
class CGame
{
	// -enOldInventory
	bool m_ParamOldInventory;
	// -enNewMenu
	//bool m_ParamNewMenu;
	static bool m_ParamDoNoLogs;
	
	// enableDebugMonitor in server config
	int m_DebugMonitorEnabled;

	ScriptModule GameScript;

	private ref array<ref Param> m_ParamCache;
	
	void CGame()
	{
		m_ParamCache = new array<ref Param>;
		m_ParamCache.Insert(NULL);
	}
	
	private void ~CGame();
	
	proto native WorkspaceWidget GetWorkspace();
	
	//! 
	/**
  \brief Called when some system event occur.
  @param eventTypeId event type.
	@param params Param object, cast to specific param class to get parameters for particular event.
	*/
	void OnEvent(EventType eventTypeId, Param params)
	{
	}

	/**
	\brief Called after creating of CGame instance
	*/
	void OnAfterCreate()
	{
	}
	
	/**
  \brief Called on World update
  @param doSim False when simulation is paused, True otherwise
	@param timeslice time elapsed from last call
	*/
	void OnUpdate(bool doSim, float timeslice) 
	{
	}

	/**
  \brief Called when key is pressed
  @param key direct input key code (DIK)
	*/
	void OnKeyPress(int key)
	{
	}

	/**
  \brief Called when key is released
  @param key direct input key code (DIK)
	*/
	void OnKeyRelease(int key)
	{
	}

	/**
  \brief Called when mouse button is pressed
  @param button - number of button \ref Mouse
	*/
	void OnMouseButtonPress(int button)
	{
	}
	
	/**
  \brief Called when mouse button is released
  @param button - number of button \ref Mouse
	*/
	void OnMouseButtonRelease(int button)
	{
	}
	
	/**
  \brief create custom main menu part (submenu)
	*/
	UIScriptedMenu			CreateScriptedMenu( int id ) { }

	/**
  \brief create custom window part
	*/
	UIScriptedWindow			CreateScriptedWindow( int id ) { }	
	
	/**
  \brief Called after remote procedure call is recieved for this object
		@param target object on which remote procedure is called, when NULL, RPC is evaluated by CGame as global
		@param rpc_type user defined identification of RPC
		@param ctx read context for params
	*/
	void	OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
	}
	
	/**
  \brief Sets exit code and quits in the right moment
	*/
	proto native void		RequestExit( int code );

	/**
  \brief Sets exit code and restart in the right moment
	*/
	proto native void		RequestRestart(int code);

	/**
  \brief Connects to IsServer
	@param IpAddress of the server
	@param port of the server set to 0 for default port
	@param password of the server
	\return true on success, false is not success(server does not exist)
	*/
	proto native int		Connect( UIScriptedMenu parent , string IpAddress, int port, string password );	
	/**
  \brief Connects to last success network session
	\return true on success, false if there is no previous session
	*/
	proto native int		ConnectLastSession( UIScriptedMenu parent , int selectedCharacter = -1 );
	/**
  \brief Disconnects from current multiplayer session
	*/
	proto native void		DisconnectSession();

	// profile functions
	/**
  \brief Gets array of strings from profile variable
	@param name of the variable
	@param values output values
	\n usage:
	@code
	TStringArray lastInventoryArray = new TStringArray;
	GetGame().GetProfileStringList("lastInventory", lastInventoryArray);
	@endcode
	*/
	proto native void		GetProfileStringList(string name, out TStringArray values);

	/**
  \brief Gets string from profile variable
	@param name of the variable
	@param value output value
	\return true when successful
	*/
	proto bool					GetProfileString(string name, out string value);

	/**
  \brief Sets array of strings to profile variable
	@param name of the variable
	@param values to set
	*/
	proto native void		SetProfileStringList(string name, TStringArray values);

	/**
  \brief Sets string to profile variable
	@param name of the variable
	@param value to set
	*/
	proto native void		SetProfileString(string name, string value);

	/**
  \brief Saves profile on disk.
	*/
	proto native void		SaveProfile();

	/**
  \brief Gets current player name
	@param name output value
	*/
	proto void					GetPlayerName(out string name);

	/**
  \brief Sets current player name
	@param name
	*/
	proto native void		SetPlayerName(string name);
	
	/**
  	\brief Assign player entity to client (in multiplayer)
	\note Works only on server
	@param name
	*/	
	proto native Entity	CreatePlayer(PlayerIdentity identity, string name, vector pos, float radius, string spec);
	
	/**
  	\brief Selects player's controlled object
		@param identity identity used in MP (use NULL in singleplayer)
		@param player object which should be controlled by player
		\note We can use only instances of DayZPlayer now
	*/	
	proto native void		SelectPlayer(PlayerIdentity identity, Object player);
	
	/**
  	\brief Creates spectator object (mostly cameras)
		@param identity identity used in MP (use NULL in singleplayer)
		@param spectatorObjType classname for spectator object (like cameras)
		@param position where to create spectator instance
	*/	
	proto native void		SelectSpectator(PlayerIdentity identity, string spectatorObjType, vector position);

	/**
  	\brief Updates spectator position on server = position of network bubble
		@param position 
	*/	
	proto native void		UpdateSpectatorPosition(vector position);
	
	/**
  	\brief Destroy player info and disconnect 
	\note Works only on server
	*/
	proto native void		DisconnectPlayer(PlayerIdentity identity, string uid);

	/**
  	\brief Returns current daytime on server
	*/
	proto native float		GetDayTime();
	
	// config functions
	/**
	\brief Get string value from config on path.
		@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
		@param value output
		\return true on success
	*/
	proto bool					ConfigGetText(string path, out string value);
	
	/**
	\brief Get string value from config on path.
		@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
		\return value output string
	*/
	string ConfigGetTextOut(string path)
	{
		string ret_s;
		ConfigGetText(path, ret_s);
		return ret_s;
	}
	
	/**
	\brief Get name of the p3d file of the given class name.
		@param class name of the desired object
		\return Name of the object's p3d file (without the '.p3d' suffix)
	*/
	string GetModelName(string class_name)
	{
		string cfg = "CfgVehicles " + class_name + " model";
		string model_path;
		GetGame().ConfigGetText(cfg, model_path); 
		int to_substring_end = model_path.Length() - 4; // -4 to leave out the '.p3d' suffix
		int to_substring_start = 0;
		
		// Currently we have model path. To get the name out of it we need to parse this string from the end and stop at the first found '\' sign
		for (int i = to_substring_end; i > 0; i--)
		{
			string sign = model_path.Get(i);
			if ( sign == "\\" )
			{
				to_substring_start = i + 1;
				break
			}
		}
		
		string model_name = model_path.Substring(to_substring_start, to_substring_end - to_substring_start);
		return model_name;
	}
	
	/**
  \brief Get float value from config on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	\return value
	*/
	proto native float	ConfigGetFloat(string path);
	

	/**
  \brief Get vector value from config on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	\return value
	*/
	proto native vector ConfigGetVector(string path);

	/**
  \brief Get int value from config on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	\return value
	*/
	proto native int		ConfigGetInt(string path);

	/**
  \brief Returns type of config value
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	\return one of constants CT_INT, CT_FLOAT, CT_STRING, CT_ARRAY, CT_CLASS, CT_OTHER
	*/
	proto native int		ConfigGetType(string path);

	/**
  \brief Get array of strings from config on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	@param value output
	\n usage :
	@code
	TStringArray characterAnimations = new TStringArray;
	GetGate().ConfigGetTextArray("CfgMovesMaleSdr2 States menu_idleUnarmed0 variantsPlayer", characterAnimations);
	@endcode
	*/
	proto native void		ConfigGetTextArray(string path, out TStringArray values);

	/**
  \brief Get array of floats from config on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	@param value output
	*/
	proto native void		ConfigGetFloatArray(string path, out TFloatArray values);
	
	/**
  \brief Get array of integers from config on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	@param value output
	*/
	proto native void		ConfigGetIntArray(string path, out TIntArray values);

	/**
  \brief Get name of subclass in config class on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	@param index of subclass in class
	@param name output
	\return true on success, false if path or child is not found
	*/
	proto bool					ConfigGetChildName(string path, int index, out string name);

	/**
  \brief Get name of base class of config class on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	@param name output
	\return true on success, false if path or base is not exists
	*/
	proto bool					ConfigGetBaseName(string path, out string base_name);

	/**
  \brief Get count of subclasses in config class on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	@param index of parameter in class
	@param name output
	\return true on success, false if path or child is not found
	*/
	proto native int		ConfigGetChildrenCount(string path);
	proto native bool		ConfigIsExisting(string path);

	proto native void		ConfigGetFullPath(string path, out TStringArray full_path);
	proto native void		ConfigGetObjectFullPath(Object obj, out TStringArray full_path);

/**
\brief Converts array of strings into single string.
	\param Array of strings like {"All", "AllVehicles", "Land"}
	\return Converts given array into something lile "All AllVehicles Land".
	@code
		???
	@endcode
*/
	string					ConfigPathToString(TStringArray array_path)
	{
		string return_path = "";
		int count = array_path.Count();
		
		for (int i = 0; i < count; i++)
		{
			return_path += array_path.Get(i);
			
			if ( i < count - 1 )
			{
				return_path += " ";
			}
		}
		
		return return_path;
	}

	/**
  \brief Get command line parameter value.
	@param name of parameter
	@param value output
	\return true when parameter exists, false otherwise
	\n usage :
	@code
	// you run e.g.: DayZInt.exe -scriptDebug=true
	string value;
	if (GetGame().CommandlineGetParam("scriptDebug", value) && value == "true")
	{
		Print("Script debugging on!");
	}
	@endcode
	*/
	proto bool					CommandlineGetParam(string name, out string value);
	
	proto native void CopyToClipboard(string text);
	proto void				CopyFromClipboard(out string text);

	proto native void 	BeginOptionsVideo();
	proto native void 	EndOptionsVideo();
	
	// entity functions
	/**
	\brief Preload objects with certain type in certain distance from camera.
	@param type of objects which should be preloaded
	@param distance from camera in which objects should be preloaded
	\return true on success
	*/
	proto native bool		PreloadObject( string type, float distance );

	/**
	\brief Creates object of certain type
	@param type of objects to create
	@param pos position where to create object
	@param create_local if True, object is not spawned on clients only on server
	@param init_ai if creating object is LightAI class, by this param is initialised AI or not
	\return new Object
	*/
	proto native Object CreateObject( string type, vector pos, bool create_local = false, bool init_ai = false, bool create_physics = true );
	proto native SoundOnVehicle CreateSoundOnObject(Object source, string sound_name, float distance, bool looped, bool create_local = false);
	proto native void   ObjectDelete( Object obj );
	proto native void   RemoteObjectDelete( Object obj ); /// RemoteObjectDelete - deletes only remote object. do not use if not sure what you do
	proto native int    ObjectRelease( Object obj );
	proto void          ObjectGetType( Object obj, out string type );
	proto void          ObjectGetDisplayName( Object obj, out string name );
	proto native vector ObjectGetSelectionPosition(Object obj, string name);
	proto native vector ObjectModelToWorld(Object obj, vector modelPos);
	proto native vector ObjectWorldToModel(Object obj, vector worldPos);
	//! returns true if player can access item's cargo/attachments (check only distance)
	proto native bool		IsObjectAccesible(EntityAI item, Man player);
	//! returns true if player can access item's cargo/attachments (check only inventory conditions)
	proto native bool		IsInventoryTreeLocked(EntityAI item, EntityAI player);	

	// input
	proto native Input	GetInput();

	// camera	
	proto native vector	GetCurrentCameraPosition();	
	proto native vector	GetCurrentCameraDirection();

	// sound
	proto native AbstractSoundScene GetSoundScene();
	
	// noise
	proto native NoiseSystem GetNoiseSystem();
	
	// inventory
	proto native bool		AddInventoryJuncture(Man player, notnull EntityAI item, InventoryLocation dst, bool test_dst_occupancy, int timeout_ms);
	proto native bool		AddActionJuncture(Man player, notnull EntityAI item, int timeout_ms);
	proto native bool		ClearJuncture(Man player, notnull EntityAI item);

	// support
	//! Delevoper only: Executes Enforce Script expression, if there is an error, is printed into the script console
	proto native bool 	ExecuteEnforceScript(string expression, string mainFnName);

	proto native bool 	ScriptTest();
	//! Get list of all debug modes
	proto native void		GetDiagModeNames(out TStringArray diag_names);
	//! Set specific debug mode
	proto native void		SetDiagModeEnable(int diag_mode, bool enabled);
	//! Gets state of specific debug mode
	proto native bool		GetDiagModeEnable(int diag_mode);

	//! Get list of all debug draw modes
	proto native void		GetDiagDrawModeNames(out TStringArray diag_names);
	//! Set debug draw mode
	proto native void		SetDiagDrawMode(int diag_draw_mode);
	//! Gets current debug draw mode
	proto native int		GetDiagDrawMode();
	
	/**
	\brief Returns average FPS of last 16 frames
	@return fps in milliseconds
	*/
	proto native float		GetFps();
	
	/**
	\brief Returns current time from start of the game
	@return time in milliseconds
	*/
	proto native float		GetTickTime();

	proto void					GetInventoryItemSize(InventoryItem item, out int width, out int height);
	/**
  \brief Returns list of all objects in radius "radius" around position "pos"
	@param pos
	@param radius
	@param objects output array
	*/
	proto native void		GetObjectsAtPosition(vector pos, float radius, out array<Object> objects, out array<Cargo> proxyCargos);
	proto native World	GetWorld();
	proto void					GetWorldName( out string world_name );
	proto void					FormatString( string format, string params[], out string output);
	proto void					GetVersion( out string version );
	proto native UIManager	GetUIManager();
	proto native Man		GetPlayer();
	proto native void		GetPlayers( out array<Man> players );
	
	//! Returns the direction where the mouse points, from the camera view
	proto native vector		GetPointerDirection();
	//! Transforms position in world to position in screen in pixels as x, y component of vector, z parameter represents distance between camera and world_pos
	proto native vector		GetScreenPos(vector world_pos);
	//! Transforms position in world to position in screen in percentage (0.0 - 1.0) as x, y component of vector, z parameter represents distance between camera and world_pos
	proto native vector		GetScreenPosRelative(vector world_pos);
	
	//! Stores login userdata as parameters which are sent to server	
	proto native void		StoreLoginData(notnull array<ref Param> params);
		
	//! Return singleton of MenuData class - at main menu contains characters played with current profile.
	proto native MenuData	GetMenuData();
	/**
  \brief Initiate remote procedure call. When called on client, RPC is evaluated on server; When called on server, RPC is executed on all clients
	@param target object on which remote procedure is called, when NULL, RPC is evaluated by CGame as global
	@param rpc_type user defined identification of RPC
	@param params custom params array
	@param recipient specified client to send RPC to. If NULL, RPC will be send to all clients (specifying recipient increase security and decrease network traffic)
	@code
	const int RPC_LOW_BLOOD_PRESSURE_EFFECT = 0;
	...
	// on server side, after blood pressure of player is low...
	Param1<float> p = new Param1<float>(m_blood_pressure);
	array<param> params = new array<param>;
	params.Insert(p);
	GetGame().RPC(player, RPC_LOW_BLOOD_PRESSURE_EFFECT, params);
	// or shortcut
	GetGame().RPCSingleParam(player, RPC_LOW_BLOOD_PRESSURE_EFFECT, p);
	...
	// on client
	class PlayerBase
	{
		// override OnRPC function
		void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
		{
			// dont forget to propagate this call trough class hierarchy!
			super.OnRPC(rpc_type, ctx);

			switch(rpc_type)
			{
				case RPC_LOW_BLOOD_PRESSURE_EFFECT:
					Param1<float> p = Param1<float>(0);
					if (ctx.Read(p))
					{
						float blood_pressure = p.param1;
						float effect_intensity = 1.0 - blood_pressure;
						ShowFancyScreenEffect(effect_intensity);
					}
				break;
			}
		}
	}
	@endcode
	*/
	proto native void		RPC(Object target, int rpc_type, notnull array<ref Param> params, bool guaranteed,PlayerIdentity recipient = NULL);
	//! see CGame.RPC
	void					RPCSingleParam(Object target, int rpc_type, Param param, bool guaranteed, PlayerIdentity recipient = NULL)
	{
		m_ParamCache.Set(0, param);
		RPC(target, rpc_type, m_ParamCache, guaranteed, recipient);
	}

	//! Use for profiling code from start to stop, they must match have same name, look wiki pages for more info: https://confluence.bistudio.com/display/EN/Profiler
	proto native void		ProfilerStart(string name);
	//! Use for profiling code from start to stop, they must match have same name, look wiki pages for more info: https://confluence.bistudio.com/display/EN/Profiler
	proto native void		ProfilerStop(string name); 

	
/**
 \brief Prints text into game chat.
	\param text to print
	\param colorClass ??
	\n usage :
	@code
		GetGame().Chat("Item splitted", "colorAction");
	@endcode
*/
	proto native void		Chat(string text, string colorClass);
	proto native void		ChatMP(Man recipient, string text, string colorClass);
	proto native void		ChatPlayer(ChatChannel channel, string text);
	//! Returns current chat channel.
	//proto native ChatChannel ChatGetChannel();

	// mission
	proto native Mission 	GetMission();
	proto native void		SetMission(Mission mission);
	
	//! Starts mission (equivalent for SQF playMission). You MUST use double slash \\ 
	proto native void		PlayMission(string path);
	
	//! Create only enforce script mission, used for mission script reloading 
	proto protected native void		CreateMission(string path);
	proto native void		RestartMission();
	proto native void		AbortMission();
	proto native void		RespawnPlayer();
	proto native bool		CanRespawnPlayer();

	//! Logout methods
	proto native void		LogoutRequestTime();
	proto native void		LogoutRequestCancel();

	proto native bool		IsMultiplayer();
	proto native bool		IsClient();
	proto native bool		IsServer();

	//! Server config parsing
	proto native int		ServerConfigGetInt(string name);
	
	// Interny build
	proto native bool		IsDebug();
	
	bool IsOldInventory()
	{
		return m_ParamOldInventory;
	}
	
	static bool IsDoNoLogs()
	{
		return m_ParamDoNoLogs;
	}
	
	/*bool IsNewMenu()
	{
		return m_ParamNewMenu;
	}*/

	void SetDebugMonitorEnabled(int value)
	{
		m_DebugMonitorEnabled = value;
	}
	
	bool IsDebugMonitor()
	{
		return IsServer() && m_DebugMonitorEnabled;
	}
	
	proto native void		GetPlayerIndentities( out array<PlayerIdentity> identities );
	
	proto native float		SurfaceY(float x, float z);
	proto native float		SurfaceRoadY(float x, float z);
	proto void				SurfaceGetType(float x, float z, out string type);
	proto void				SurfaceGetType3D(float x, float y, float z, out string type);
	proto void				SurfaceGetTypeUnderObject(Object object, out string type);
	proto native float		SurfaceGetNoiseMultiplier(float x, float z);
	proto native vector		SurfaceGetNormal(float x, float z);
	proto native float		SurfaceGetSeaLevel();
	proto native bool		SurfaceIsSea(float x, float z);
	proto native bool		SurfaceIsPond(float x, float z);
	
	proto native void 		UpdatePathgraphRegion(vector regionMin, vector regionMax);
	
	void 					UpdatePathgraphRegionByObject(Object object)
	{
		vector pos = object.GetPosition();
		vector min_max[2];
		float radius = object.ClippingInfo ( min_max );
		vector min = Vector ( pos[0] - radius, pos[1], pos[2] - radius );
		vector max = Vector ( pos[0] + radius, pos[1], pos[2] + radius );
		UpdatePathgraphRegion( min, max );
	}
	
	/**
	\brief Finds all objects that are in choosen oriented bounding box (OBB)
		\param center \p vector, center of OBB
		\param orientation \p vector, direction (front vector), used for calculation of OBB rotation
		\param edgeLength \p vector, sizes of whole box
		\param excludeObjects \p array<Object>, objects that should be excluded from collision check
		\param collidedObjects \p array<Object>, out parameter, objects that collided with OBB
		\returns \p bool, \p true if at least one object collided with OBB, \p false otherwise
		\note Object that doesn't have collision geometry will be ignored
		
		@code
			vector pos = GetPosition();
			vector orientation = GetOrientation();
			vector size = "10 4 8";
			array<Object> excluded_objects = new array<Object>;
			excluded_objects.Insert(this);
			array<Object> nearby_objects = new array<Object>;
			
			if(GetGame().IsBoxColliding( pos, orientation, size, excluded_objects, nearby_objects))
			{
				for (local int i = 0, c = nearby_objects.Count(); i < c; ++i)
				{
					PrintString("object " + i.ToString());
				}
			}
		@endcode
	*/
	proto native bool		IsBoxColliding(vector center, vector orientation, vector edgeLength, array<Object> excludeObjects, array<Object> collidedObjects = NULL); 
	
	//! Returns weather controller object.
	proto native Weather GetWeather();

	//! Sets custom camera camera EV.
	proto native void	SetEVUser(float value);
	
	proto native void OpenURL(string url);
	
	//! Initialization of damage effects
	proto native void		InitDamageEffects(Object effect);

//-----------------------------------------------------------------------------
// persitence
//-----------------------------------------------------------------------------

	//! Returns EntityAI by its persistent ID parts
	//! or null if entity with given persistent ID does not exists.
	/*!
		This function returns valid data only inside AfterLoad event.
		Do not use this in anywhere else.

		Its main purpose is only for keep track on object connections
		after server restarts, all data related to this function are deleted
		when server initialization is done.
	*/
	proto native EntityAI GetEntityByPersitentID( int low, int high );

//-----------------------------------------------------------------------------

/**
\brief Returns is class name inherited from parent class name
	\param cfg_class_name \p Config Class name ("Animal_CervusElaphus")
	\param cfg_parent_name \p Parent Config Class name ("DZ_LightAI")
	\returns \p bool is class name inherited from parent class name
	@code
		bool is_kind = GetGame().IsKindOf( "Animal_CervusElaphus", "DZ_LightAI");
		PrintString(ToString(is_kind));
		
		>> 1
	@endcode
*/
	bool IsKindOf(string cfg_class_name, string cfg_parent_name)
	{
		TStringArray full_path = new TStringArray;
		
		ConfigGetFullPath("CfgVehicles " + cfg_class_name, full_path);
	
		if (full_path.Count() == 0)
		{
			ConfigGetFullPath("CfgAmmo " + cfg_class_name, full_path);
		}
		
		if (full_path.Count() == 0)
		{
			ConfigGetFullPath("CfgMagazines " + cfg_class_name, full_path);
		}
		
		if (full_path.Count() == 0)
		{
			ConfigGetFullPath("cfgWeapons " + cfg_class_name, full_path);
		}
	
		if (full_path.Count() == 0)
		{
			ConfigGetFullPath("CfgNonAIVehicles " + cfg_class_name, full_path);
		}
		
		cfg_parent_name.ToLower();
		for (int i = 0; i < full_path.Count(); i++)
		{
			string tmp = full_path.Get(i);
			tmp.ToLower();
			if (tmp == cfg_parent_name)
			{
				return true;
			}
		}
	
		return false;
	}

/**
\brief Returns is object inherited from parent class name
	\param object \p Object
	\param cfg_parent_name \p Parent Config Class name ("DZ_LightAI")
	\returns \p bool is object inherited from parent class name
	@code
		bool is_kind = GetGame().IsKindOf( my_animal, "DZ_LightAI");
		PrintString(ToString(is_kind));
		
		>> 1
	@endcode
*/
	bool ObjectIsKindOf(Object object, string cfg_parent_name)
	{
		TStringArray full_path = new TStringArray;
		ConfigGetObjectFullPath(object, full_path);
	
		cfg_parent_name.ToLower();
	
		for (int i = 0; i < full_path.Count(); i++)
		{
			string tmp = full_path.Get(i);
			tmp.ToLower();
			if (tmp == cfg_parent_name)
			{
				return true;
			}
		}
	
		return false;
	}
	
	/**
	Searches given config path (config_path) for the given member (searched_member) and returns its index.
	\nReturns -1 if not found.
	\n usage:
	* @code
	* int skinning_class_index = g_Game.ConfigFindClassIndex(cfgPath_animal, "Skinning");
	* @endcode
	*/
	int ConfigFindClassIndex(string config_path, string searched_member)
	{
		int class_count = ConfigGetChildrenCount(config_path);
		for (int index = 0; index < class_count; index++)
		{
			string found_class = "";
			ConfigGetChildName(config_path, index, found_class);
			if (found_class == searched_member)
			{
				return index;
			}
		}
		return -1;
	}

	//!returns mission time in milliseconds	
	proto int GetTime();

	/**
  Returns CallQueue for certain category
  @param call_category call category, valid values are:
	\n CALL_CATEGORY_SYSTEM - calls & timers in this queue are processed every time without any restrictions
	\n CALL_CATEGORY_GUI - calls & timers in this queue are processed when GUI is enabled (even during pase game)
	\n CALL_CATEGORY_GAMEPLAY - calls & timers in this queue are processed only during mission, when game is not paused
	\n usage:
	* @code
	* GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "Refresh"); // calls "Refresh" function on "this" with no arguments
	* GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "Show", new Param1<bool>(true)); // calls "Show" function on "this" with one bool argument
	* GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "SetPos", new Param2<float, float>(0.2, 0.5)); // calls "SetPos" function on "this" with two float arguments
	* @endcode
	*/
	ScriptCallQueue GetCallQueue(int call_category) {}

	ScriptInvoker GetUpdateQueue(int call_category) {}
	/**
  Returns TimerQueue for certain category
  @param call_category call category, valid values are:
	\n CALL_CATEGORY_SYSTEM - calls & timers in this queue are processed every time without any restrictions
	\n CALL_CATEGORY_GUI - calls & timers in this queue are processed when GUI is enabled (even during pase game)
	\n CALL_CATEGORY_GAMEPLAY - calls & timers in this queue are processed only during mission, when game is not paused
	*/
	TimerQueue GetTimerQueue(int call_category) {}

	/**
  Returns DragQueue. Callbacks are called on mouse move until mouse button is released, then the queue is cleaned.
	*/
	DragQueue GetDragQueue() {}

	//!returns class name of first valid survivor
	string CreateDefaultPlayer() {}
	
	//!returns class name of random survivor
	string CreateRandomPlayer() {}
	
	//!outputs array of all valid survivor class names
	TStringArray ListAvailableCharacters() {}
	
	//TODO implement into character creator
	//!saves new character's equipment and type to g_Game
	void SetCharacterInfo(int top,int bottom,int shoes,int characterName) {}

	bool IsInventoryOpen()
	{
	}
};

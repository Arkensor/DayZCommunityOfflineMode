#ifdef GAME_TEMPLATE
Game g_Game;

Game GetGame()
{
	return g_Game;
}

class Game
{
	ScriptModule GameScript;
	
	ScriptModule GetScriptModule()
	{
		return GameScript;
	}
	
	void SetDebug(bool isDebug) {}
	//! 
	/**
  	\brief Called when some system event occur.
  	@param eventTypeId event type.
	@param params Param object, cast to specific param class to get parameters for particular event.
	*/
	void OnEvent(EventType eventTypeId, Param params)
	{
		Print("OnEvent");
	}

	/**
	\brief Called after full initialization of Game instance
	*/
	void OnAfterInit()
	{
		Print("OnAfterInit");
	}
	
	/**
  	\brief Called on World update
	@param timeslice time elapsed from last call
	*/
	void OnUpdate(float timeslice) 
	{

	}

	/**
  	\brief Sets world file to be loaded. Returns false if file doesn't exist.
	@param path Path to the ent file
	@param reload Force reload the world
	*/
	proto native bool SetWorldFile(string path, bool reload);

	/**
  	\brief Returns path of world file loaded
	*/
	proto native owned string GetWorldFile();

	/**
  	\brief Event which is called right before game starts (all entities are created and initialized). Returns true if the game can start.
	*/
	bool OnGameStart()
	{
		return true;
	}

	/**
  	\brief Event which is called right before game end.
	*/
	void OnGameEnd()
	{
	}

	/**
  	\brief Creates loading screen
	*/
	void ShowLoadingAnim()
	{
	}

	/**
  	\brief Hides loading screen
	*/
	void HideLoadingAnim()
	{
	}

	/**
  	\brief Used for updating the loading screen
	@param timeslice
	@param progress loading progress between 0 and 1
	*/
	void UpdateLoadingAnim(float timeslice, float progress)
	{
	}

	/**
	\brief Safely instantiate the entity and calls EOnInit if the entity sets event mask EntityEvent.INIT.
	@param typename Name of entity's type to instantiate.
	@return instantiated entity
	*/
	proto native IEntity SpawnEntity(typename typeName);

	/**
	\brief Safely instantiate the entity from template (with all components) and calls EOnInit if the entity sets event mask EntityEvent.INIT.
	@param templateResource Template resource of the entity to instantiate.
	@return instantiated entity
	*/
	proto native IEntity SpawnEntityTemplate(vobject templateResource);

	/**
	\brief Safely instantiate the component from template, insert it to entity and calls EOnInit if the component sets event mask EntityEvent.INIT.
	@param owner Entity which will own the component
	@param templateResource Template resource of the component to instantiate.
	@return instantiated component
	*/
	proto native GenericComponent SpawnComponentTemplate(IEntity owner, vobject templateResource);

	proto native IEntity FindEntity(string name);

	proto native WorkspaceWidget GetWorkspace();

	/**
	\brief Setting request flag for engine to exit the game
	*/
	proto native void RequestClose();

	/**
	\brief Setting request flag for the engine to reinitialize the game
	* Doesn't do anything in Workbench
	*/
	proto native void RequestReload();

	/**
	\brief Returns version of the game
	*/
	proto native owned string GetBuildVersion();

	/**
	\brief Returns date and time when the game was built
	*/
	proto native owned string GetBuildTime();

	/**
  	\brief Returns World entity when in game or in play mode in WE. NULL otherwise.
	*/
	proto native GenericWorldEntity GetWorldEntity();
	
	proto native InputManager GetInputManager();
	proto native MenuManager GetMenuManager();
	proto native int GetTickCount();
}

void GameLibInit()
{

}
#endif
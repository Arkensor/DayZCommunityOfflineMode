/**
 * \defgroup Components
 * \desc Constants for Components
 * @{
 */
const string	COMP_NAME_NONE				= "None";
const int		COMP_TYPE_UNDEFINED			= -1;
const int		COMP_TYPE_ETITY_DEBUG		= 0;
const int		COMP_TYPE_ENERGY_MANAGER	= 1;
const int		COMP_TYPE_BODY_STAGING		= 2;
const int		COMP_TYPE_ANIMAL_BLEEDING	= 3;
const int		COMP_TYPE_COUNT				= 4;
/** @}*/

class Component
{

	//==========================================
	// Variables Private Static
	private static string m_CompNames[COMP_TYPE_COUNT];	
	
	//==========================================
	// Variables Private
	protected EntityAI		m_ThisEntityAI;
	
	//==========================================
	
	void 	Event_OnFrame(IEntity other, float timeSlice);
	Shape 	DebugBBoxDraw();
	void	DebugBBoxSetColor(int color);
	void	DebugBBoxDelete();
	Shape	DebugDirectionDraw(float distance = 1);
	void	DebugDirectionSetColor(int color);
	void	DebugDirectionDelete();
	
	// Methods Public Static
	static void	Init()
	{
		m_CompNames[COMP_TYPE_ETITY_DEBUG]			= "ComponentEntityDebug";
		m_CompNames[COMP_TYPE_ENERGY_MANAGER]		= "ComponentEnergyManager";
		m_CompNames[COMP_TYPE_BODY_STAGING]			= "ComponentBodyStaging";
		m_CompNames[COMP_TYPE_ANIMAL_BLEEDING]		= "ComponentAnimalBleeding";
	}

	///////////////////////////////////public/static
	// GetNameByType
	////////////////////////////////////////////////
	static string GetNameByType(int comp_type)
	{
		if ( IsTypeExist(comp_type) == false )
		{
			LogErrorBadCompType(comp_type, "Component->GetNameByType()");
			return "None";
		}
		
		return m_CompNames[comp_type];
	}

	///////////////////////////////////public/static
	// IsTypeExist
	////////////////////////////////////////////////
	static bool IsTypeExist(int comp_type)
	{
		if ( comp_type < 0 || comp_type >= COMP_TYPE_COUNT )
		{
			return false;
		}
		
		return true;
	}


	///////////////////////////////////public/static
	// LogErrorBadCompType
	////////////////////////////////////////////////
	static void	LogErrorBadCompType(int comp_type, string fnc_name)
	{
		string msg = "Bad parameter comp_type='"+comp_type.ToString()+"'. Parameter must be 0-"+(COMP_TYPE_COUNT - 1).ToString()+". Returning component name 'None'.";
		Debug.LogError(msg, "Component", "n/a", fnc_name);
	}

	///////////////////////////////////public/static
	// LogWarningAlredyExist
	////////////////////////////////////////////////
	static void	LogWarningAlredyExist(int comp_type, string fnc_name)
	{
		string msg = "Component '"+Component.GetNameByType(comp_type)+"' already exists!";
		Debug.LogError(msg, "Component", "n/a", fnc_name);
	}

	//=======================================public
	// SetParentEntityAI
	//=============================================
	void SetParentEntityAI(EntityAI e)
	{
		m_ThisEntityAI = e;
	}

	//====================================protected
	// Awake
	//=============================================
	void Event_OnAwake()
	{
		
	}

	//====================================protected
	// Init
	//=============================================
	void Event_OnInit()
	{
		
	}

	//=======================================public
	// Log
	//=============================================
	void LogThis(string msg, string fnc_name = "n/a")
	{
		Debug.Log(msg, GetCompName(), "n/a", fnc_name, m_ThisEntityAI.GetType());
	}

	//=======================================public
	// LogWarning
	//=============================================
	void LogThisWarning(string msg, string fnc_name = "n/a")
	{
		Debug.LogWarning(msg, GetCompName(), "n/a", fnc_name, m_ThisEntityAI.GetType());
	}

	//=======================================public
	// LogError
	//=============================================
	void LogThisError(string msg, string fnc_name = "n/a")
	{
		Debug.LogError(msg, GetCompName(), "n/a", fnc_name, m_ThisEntityAI.GetType());
	}

	//=======================================public
	// GetType
	//=============================================
	string GetCompName()
	{
		return Component.GetNameByType(this.GetCompType());
	}

	//=======================================public
	// GetType
	//=============================================
	int GetCompType()
	{
		return COMP_TYPE_UNDEFINED;
	}

	//=======================================public
	// Event_OnItemAttached
	//=============================================
	void Event_OnItemAttached(EntityAI item, string slot_name)
	{
		Log("Component=>Event_OnItemAttached");
	}

	//=======================================public
	// Event_OnItemDetached
	//=============================================
	void Event_OnItemDetached(EntityAI item, string slot_name)
	{
		Log("Component=>Event_OnItemDetached");
	}
}
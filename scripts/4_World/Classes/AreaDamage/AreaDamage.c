class AreaDamageBase
{
	EntityAI				m_ParentObject;
	AreaDamageTrigger		m_AreaDamageTrigger;
	AreaDamageType			m_AreaDamageType;

	float					m_LoopInterval;
	float					m_DeferredInterval;
	float					m_PlayerDamage;
	float					m_OthersDamage;
	ref TStringArray 		m_HitZones;
	ref TStringArray		m_RaycastSources;
	string					m_AmmoName;
	vector					m_ExtentMin;
	vector 					m_ExtentMax;
	
	void AreaDamageBase(EntityAI parent)
	{
		m_ParentObject		= parent;
		m_ExtentMin 		= Vector(0, 0, 0);
		m_ExtentMax 		= Vector(0, 0, 0);
		m_LoopInterval 		= 0.0;
		m_DeferredInterval	= 0.0;
		m_PlayerDamage		= 0.0;
		m_PlayerDamage		= 0.0;
		m_HitZones			= new TStringArray;
		m_HitZones.Insert("Head");
		m_HitZones.Insert("Lungs");
		m_HitZones.Insert("LeftHand");

		m_RaycastSources	= new TStringArray;
		m_RaycastSources.Insert("0.0 0.1 0.0");

		m_AmmoName			= "MeleeDamage";

		LoadConfigParams();
	}
	void ~AreaDamageBase() {}

	void Spawn()
	{
		CreateDamageTrigger();
	}
	
	void Destroy()
	{
		DestroyDamageTrigger();
	}
	
	void SetExtents( vector mins, vector maxs )
		{ m_ExtentMin = mins; m_ExtentMax = maxs; }
	void SetAreaDamageType( int area_dmg_type )
		{ m_AreaDamageType = area_dmg_type; }
	void SetHitZones( array<string> hitzones )
		{ m_HitZones = hitzones; }
	void SetRaycastSources( array<string> raycast_sources )
		{ m_RaycastSources = raycast_sources; }
	void SetAmmoName( string ammo_name )
		{ m_AmmoName = ammo_name; }
	void SetLoopInterval( float time )
		{ m_LoopInterval = time; }
	void SetDeferInterval( float time )
		{ m_DeferredInterval = time; }
    void SetParentObject( EntityAI obj )
		{ m_ParentObject = obj };
	
	void CreateDamageTrigger()
	{
		if(Class.CastTo(m_AreaDamageTrigger, g_Game.CreateObject( "AreaDamageTrigger", m_ParentObject.GetPosition(), false, false, false )))
		{
			m_AreaDamageTrigger.SetOrientation( m_ParentObject.GetOrientation() );
			m_AreaDamageTrigger.SetExtents( m_ExtentMin, m_ExtentMax );
			m_AreaDamageTrigger.SetAreaDamageType( m_AreaDamageType );
			m_AreaDamageTrigger.SetHitZones( m_HitZones );
			m_AreaDamageTrigger.SetRaycastSources( m_RaycastSources );
			m_AreaDamageTrigger.SetAmmoName( m_AmmoName );
			m_AreaDamageTrigger.SetLoopTime( m_LoopInterval );
			m_AreaDamageTrigger.SetDeferTime( m_DeferredInterval );
			m_AreaDamageTrigger.SetParentObject( m_ParentObject );
		}
	}
	
	void DestroyDamageTrigger()
	{
		if ( GetGame() && m_AreaDamageTrigger ) // It's necesarry to check if the game exists. Otherwise a crash occurs while quitting.
		{
			GetGame().ObjectDelete( m_AreaDamageTrigger );
			m_AreaDamageTrigger = NULL;
		}
	}

	protected void LoadConfigParams()
	{
		string areadamage_subcfg = "CfgVehicles " + m_ParentObject.GetType() + " " + ClassName() + " ";
		
		// Read all config parameters
		if ( ConfigParamExists(areadamage_subcfg, "extentMin") )
			{ m_ExtentMin = GetGame().ConfigGetVector(areadamage_subcfg + "extentMin"); }
		if ( ConfigParamExists(areadamage_subcfg, "extentMax") )
			{ m_ExtentMax = GetGame().ConfigGetVector(areadamage_subcfg + "extentMax"); }
		if ( ConfigParamExists(areadamage_subcfg, "loopInterval") )
			{ m_LoopInterval = GetGame().ConfigGetFloat(areadamage_subcfg + "loopInterval"); }
		if ( ConfigParamExists(areadamage_subcfg, "deferredInterval") )
			{ m_DeferredInterval = GetGame().ConfigGetFloat(areadamage_subcfg + "deferredInterval"); }
		if ( ConfigParamExists(areadamage_subcfg, "hitZones") )
			{ GetGame().ConfigGetTextArray(areadamage_subcfg + "hitZones", m_HitZones); }
		if ( ConfigParamExists(areadamage_subcfg, "raycastSources") )
			{ GetGame().ConfigGetTextArray(areadamage_subcfg + "raycastSources", m_RaycastSources); }
		if ( ConfigParamExists(areadamage_subcfg, "ammoName") )
			{ GetGame().ConfigGetText(areadamage_subcfg + "ammoName", m_AmmoName); }
	}
	
	protected bool ConfigParamExists(string path, string option)
	{
		return GetGame().ConfigIsExisting(path + option);
	}
}